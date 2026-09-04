// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/module_scan.h"
#include "../src/link/tests/support/elm_trace_replay.h"

#include <stdio.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "FAIL: %s\n", #x); return 1; } } while (0)
#define FORD_TRACE_MAX_ENTRIES 192U

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

static bool append_trace_entry(LinkTestElmTraceEntry *entries,
                               char commands[][LINK_ELM327_MAX_COMMAND],
                               size_t *count,
                               const char *command,
                               LinkElm327Result result,
                               const char *response_text,
                               bool ok_seen)
{
    size_t length;
    if (entries == NULL || commands == NULL || count == NULL || command == NULL ||
        *count >= FORD_TRACE_MAX_ENTRIES) return false;
    length = strlen(command);
    if (length >= LINK_ELM327_MAX_COMMAND) return false;
    memcpy(commands[*count], command, length + 1U);
    entries[*count].command = commands[*count];
    entries[*count].result = result;
    entries[*count].response_text = response_text;
    entries[*count].ok_seen = ok_seen;
    (*count)++;
    return true;
}

static bool build_reference_trace(LinkTestElmTraceEntry *entries,
                                  char commands[][LINK_ELM327_MAX_COMMAND],
                                  size_t *entry_count)
{
    static const uint16_t dids[] = {
        0xF187U, 0xF188U, 0xF18CU, 0xF190U, 0xF191U, 0xF197U
    };
    static const char vin_response[] =
        "62F190314641464F52444C494E4B544553543031";
    size_t endpoint_index;

    if (entries == NULL || commands == NULL || entry_count == NULL) return false;
    *entry_count = 0U;
    for (endpoint_index = 0U;
         endpoint_index < fordlink_diagnostic_endpoint_count();
         ++endpoint_index) {
        const FordlinkDiagnosticEndpoint *endpoint =
            fordlink_diagnostic_endpoint_at(endpoint_index);
        char command[LINK_ELM327_MAX_COMMAND];
        size_t did_index;

        if (endpoint == NULL || !endpoint->scan_hscan_by_default ||
            endpoint->network != FORDLINK_NETWORK_HSCAN) continue;

        (void)snprintf(command, sizeof(command), "ATSH%03X",
                       (unsigned int)endpoint->request_id);
        if (!append_trace_entry(entries, commands, entry_count, command,
                                LINK_ELM327_RESULT_OK, "OK", true)) return false;
        (void)snprintf(command, sizeof(command), "ATCRA%03X",
                       (unsigned int)endpoint->response_id);
        if (!append_trace_entry(entries, commands, entry_count, command,
                                LINK_ELM327_RESULT_OK, "OK", true)) return false;
        if (!append_trace_entry(entries, commands, entry_count, "ATCAF1",
                                LINK_ELM327_RESULT_OK, "OK", true)) return false;
        if (!append_trace_entry(entries, commands, entry_count, "ATCFC1",
                                LINK_ELM327_RESULT_OK, "OK", true)) return false;

        for (did_index = 0U; did_index < sizeof(dids) / sizeof(dids[0]); ++did_index) {
            const bool pcm_vin = endpoint->request_id == 0x7E0U &&
                dids[did_index] == 0xF190U;
            (void)snprintf(command, sizeof(command), "22%04X",
                           (unsigned int)dids[did_index]);
            if (!append_trace_entry(
                    entries, commands, entry_count, command,
                    pcm_vin ? LINK_ELM327_RESULT_OK : LINK_ELM327_RESULT_NO_DATA,
                    pcm_vin ? vin_response : NULL,
                    false)) return false;
        }
        if (!append_trace_entry(entries, commands, entry_count, "1902FF",
                                LINK_ELM327_RESULT_NO_DATA, NULL, false)) return false;
    }
    return *entry_count != 0U;
}

static int test_ford_trace_replay_fixture(void)
{
    LinkTestElmTraceEntry entries[FORD_TRACE_MAX_ENTRIES];
    char commands[FORD_TRACE_MAX_ENTRIES][LINK_ELM327_MAX_COMMAND];
    LinkTestElmTraceReplay replay;
    FordlinkModuleScanner scanner;
    size_t entry_count = 0U;
    bool complete = false;
    unsigned int guard = 0U;

    CHECK(build_reference_trace(entries, commands, &entry_count));
    CHECK(entry_count > 100U);
    link_test_elm_trace_replay_init(&replay, entries, entry_count);
    CHECK(fordlink_module_scanner_begin(&scanner));

    while (!complete && guard++ < FORD_TRACE_MAX_ENTRIES + 8U) {
        char command[LINK_ELM327_MAX_COMMAND];
        size_t written = 0U;
        uint64_t timeout = 0U;
        LinkElm327Response response;

        CHECK(fordlink_module_scanner_next_command(
            &scanner, command, sizeof(command), &written, &timeout));
        CHECK(written == strlen(command));
        CHECK(timeout > 0U);
        CHECK(link_test_elm_trace_replay_next(&replay, command, &response));
        CHECK(fordlink_module_scanner_accept(&scanner, &response, &complete));
    }

    CHECK(complete);
    CHECK(link_test_elm_trace_replay_complete(&replay));
    CHECK(fordlink_module_scanner_result_count(&scanner) > 5U);
    CHECK(fordlink_module_scanner_responsive_count(&scanner) == 1U);
    {
        const FordlinkModuleScanResult *result =
            fordlink_module_scanner_result_at(&scanner, 0U);
        CHECK(result != NULL && result->endpoint != NULL &&
              strcmp(result->endpoint->module_key, "PCM") == 0 &&
              result->responded);
        CHECK(strcmp(result->vin, "1FAFORDLINKTEST01") == 0);
    }
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
    puts("FORDLINK read-only module scanner and end-to-end trace replay passed");
    return 0;
}
