// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/module_scan.h"
#include "../src/link/tests/support/elm_trace_replay.h"

#include <stdio.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "FAIL: %s\n", #x); return 1; } } while (0)

static LinkElm327Response ok(void)
{
    LinkElm327Response response;
    memset(&response, 0, sizeof(response));
    response.result = LINK_ELM327_RESULT_OK;
    response.ok_seen = true;
    return response;
}

static LinkElm327Response nodata(void)
{
    LinkElm327Response response;
    memset(&response, 0, sizeof(response));
    response.result = LINK_ELM327_RESULT_NO_DATA;
    return response;
}

static LinkElm327Response vin(void)
{
    LinkElm327Response response;
    memset(&response, 0, sizeof(response));
    response.result = LINK_ELM327_RESULT_OK;
    strcpy(response.text, "62F190314641464F52444C494E4B544553543031");
    response.length = strlen(response.text);
    response.line_count = 1U;
    return response;
}

static int test_ford_trace_replay_fixture(void)
{
    static const LinkTestElmTraceEntry entries[] = {
        { "ATSH7E0", LINK_ELM327_RESULT_OK, "OK", true },
        { "ATCRA7E8", LINK_ELM327_RESULT_OK, "OK", true },
        { "22F190", LINK_ELM327_RESULT_OK,
          "62F190314641464F52444C494E4B544553543031", false }
    };
    static const char *commands[] = { "ATSH7E0", "ATCRA7E8", "22F190" };
    LinkTestElmTraceReplay replay;
    LinkElm327Response response;
    size_t index;

    link_test_elm_trace_replay_init(
        &replay, entries, sizeof(entries) / sizeof(entries[0]));
    for (index = 0U; index < sizeof(commands) / sizeof(commands[0]); ++index) {
        CHECK(link_test_elm_trace_replay_next(&replay, commands[index], &response));
    }
    CHECK(strstr(response.text, "62F190") == response.text);
    CHECK(link_test_elm_trace_replay_complete(&replay));
    return 0;
}

static int test_bounded_module_scan(void)
{
    FordlinkModuleScanner scanner;
    bool done = false;
    bool injected = false;
    unsigned int guard = 0U;

    CHECK(fordlink_module_scanner_begin(&scanner));
    while (!done && guard++ < 500U) {
        char command[64];
        size_t written = 0U;
        uint64_t timeout = 0U;
        LinkElm327Response response;

        CHECK(fordlink_module_scanner_next_command(
            &scanner, command, sizeof(command), &written, &timeout));
        if (strncmp(command, "AT", 2U) == 0) {
            response = ok();
        } else if (!injected &&
                   scanner.probe.profile.channel.tx_can_id == 0x7E0U &&
                   strcmp(command, "22F190") == 0) {
            response = vin();
            injected = true;
        } else {
            response = nodata();
        }
        CHECK(fordlink_module_scanner_accept(&scanner, &response, &done));
    }

    CHECK(done && injected);
    CHECK(fordlink_module_scanner_result_count(&scanner) > 5U);
    CHECK(fordlink_module_scanner_responsive_count(&scanner) == 1U);
    {
        const FordlinkModuleScanResult *result =
            fordlink_module_scanner_result_at(&scanner, 0U);
        CHECK(result != NULL && result->endpoint != NULL &&
              strcmp(result->endpoint->module_key, "PCM") == 0 &&
              result->responded);
        CHECK(strncmp(result->vin, "1FAFORDLINKTEST01", 15U) == 0);
    }
    return 0;
}

int main(void)
{
    CHECK(test_ford_trace_replay_fixture() == 0);
    CHECK(test_bounded_module_scan() == 0);
    puts("FORDLINK read-only module scanner and trace replay passed");
    return 0;
}
