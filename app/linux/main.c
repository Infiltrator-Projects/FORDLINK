// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/fordlink.h"
#include "fordlink/network.h"
#include "fordlink/module_scan.h"
#include "link-gtk-shell.h"
#include "link-gtk-widgets.h"
#include "link/workspace.h"
#include "link/obd2.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    bool connected;
    bool diagnostic_valid;
    bool diagnostic_ready;
    char adapter_identity[160];
    LinkDiagnosticFlow diagnostic;
    bool sample_valid[256];
    LinkObd2Sample samples[256];
    size_t live_sample_count;
    FordlinkModuleScanner ford_scan;
    bool ford_scan_started;
    bool ford_scan_complete;
    bool ford_scan_success;
} ProductContext;

static const char css[] =
"window { background:#06182c; color:#f4f7fb; }"
".link-connection-bar { background:#0a2a4a; border-color:#2f6ea5; }"
".link-link-button { background:#dbe7f2; color:#071727; }"
".link-brand { color:#ffffff; }"
".link-brand-subtitle { color:#8eb8dd; }"
".link-section-title { color:#f4f7fb; }"
".link-section-summary { color:#82a8c9; }"
".link-card { background:#0b2844; border-color:#285f8f; }"
".link-card-kicker { color:#76a9d4; font-weight:800; }"
".link-card-title { color:#ffffff; font-weight:800; }"
".link-detail-label { color:#8eb0cc; }"
".link-detail-value { color:#f4f7fb; font-weight:700; }"
".link-card-note { color:#a8c2d8; }"
".link-status-chip { border-color:#3d76a7; font-weight:700; }"
".state-success { color:#a8e0b9; border-color:#4f8c63; }"
".state-warning { color:#f2cf8e; border-color:#9b7940; }";

static const char *stage(const ProductContext *context)
{
    if (!context->connected) return "Not linked";
    if (!context->diagnostic_valid) return "Starting diagnostics";
    if (context->diagnostic.stage == LINK_DIAGNOSTIC_FLOW_FAILED)
        return "Diagnostic session failed";
    if (context->diagnostic_ready) return "Live diagnostics active";
    return link_diagnostic_flow_stage_name(context->diagnostic.stage);
}

static size_t pid_count(const LinkObd2PidSet *set)
{
    size_t count = 0U;
    unsigned int pid;
    for (pid = 1U; pid < 256U; ++pid)
        if (link_obd2_pid_set_contains(set, (uint8_t)pid)) ++count;
    return count;
}

static void format_sample(const LinkObd2Sample *sample, char *out, size_t size)
{
    const char *unit = link_obd2_unit_name(sample->unit);
    (void)snprintf(out, size, "%.3f%s%s", sample->value,
                   unit != NULL && unit[0] != '\0' ? " " : "",
                   unit != NULL ? unit : "");
}

static void append_dtcs(GtkWidget *card, const char *prefix,
                        const LinkObd2DtcList *list)
{
    size_t index;
    if (list->count == 0U) {
        char key[48];
        (void)snprintf(key, sizeof(key), "%s faults", prefix);
        link_gtk_card_append_detail(card, key, "None reported");
        return;
    }
    for (index = 0U; index < list->count; ++index) {
        char key[48];
        (void)snprintf(key, sizeof(key), "%s %zu", prefix, index + 1U);
        link_gtk_card_append_detail(card, key, list->entries[index].code);
    }
}

static void append_vehicle(GtkWidget *body, ProductContext *context)
{
    GtkWidget *vehicle = link_gtk_card_new("VEHICLE", "Ford diagnostic identity");
    GtkWidget *connection = link_gtk_card_new("CONNECTION", "LINK adapter and diagnostic session");
    const char *vin = context->diagnostic_valid
        ? link_diagnostic_flow_standard_vin(&context->diagnostic) : NULL;

    link_gtk_card_append_detail(vehicle, "Manufacturer", "Ford");
    link_gtk_card_append_detail(vehicle, "VIN",
        vin != NULL && vin[0] != '\0' ? vin : "Waiting for SAE Mode 09 VIN");
    link_gtk_card_append_detail(vehicle, "Standards core",
        "LINK · SAE J1979 / OBD-II / ISO-TP / UDS");

    link_gtk_card_append_status(connection,
        context->connected ? "LINKED" : "NOT LINKED",
        context->connected ? "state-success" : "state-warning");
    link_gtk_card_append_detail(connection, "Adapter",
        context->adapter_identity[0] != '\0'
            ? context->adapter_identity
            : "Select an adapter above and press LINK UP");
    link_gtk_card_append_detail(connection, "Diagnostic state", stage(context));
    link_gtk_card_append_note(connection,
        "FORDLINK is GUI-first. LINK owns the common standards flow. Ford-specific claims remain evidence-gated.");

    gtk_box_append(GTK_BOX(body), vehicle);
    gtk_box_append(GTK_BOX(body), connection);
}

static void append_modules(GtkWidget *body, ProductContext *context)
{
    GtkWidget *card = link_gtk_card_new(
        "MODULE RESPONDERS",
        "Physical standard responders seen during capability discovery");
    if (!context->diagnostic_valid ||
        context->diagnostic.supported_pid_responders.count == 0U) {
        link_gtk_card_append_status(card, "No responder inventory yet", "state-warning");
    } else {
        size_t index;
        for (index = 0U;
             index < context->diagnostic.supported_pid_responders.count;
             ++index) {
            const LinkObd2ResponderPidSet *responder =
                &context->diagnostic.supported_pid_responders.entries[index];
            char key[48];
            char value[160];
            FordlinkNetworkKind network_kind =
                fordlink_network_for_standard_obd_responder(
                    responder->responder_id, responder->extended_id);
            const FordlinkNetworkDescriptor *network =
                fordlink_network_descriptor(network_kind);
            (void)snprintf(key, sizeof(key), "ECU %zu", index + 1U);
            (void)snprintf(value, sizeof(value),
                "%s · %s CAN 0x%X · %zu advertised PIDs",
                network != NULL ? network->display_name : "Unclassified network",
                responder->extended_id ? "29-bit" : "11-bit",
                (unsigned int)responder->responder_id,
                pid_count(&responder->supported_pids));
            link_gtk_card_append_detail(card, key, value);
        }
    }
    link_gtk_card_append_detail(card, "HS-CAN",
        "Standard OBD lane · direct DLC network");
    link_gtk_card_append_detail(card, "MS-CAN",
        "Enhanced Ford lane · explicit adapter/network support required");
    link_gtk_card_append_note(card,
        "Additional Ford CAN lanes are profile-specific. FORDLINK records them explicitly rather than guessing module placement or copying proprietary databases.");
    gtk_box_append(GTK_BOX(body), card);

    {
        GtkWidget *ford = link_gtk_card_new(
            "FORD MODULE SCAN",
            "Read-only UDS identity and module DTC census");
        size_t index;
        size_t shown = 0U;
        char status[96];
        if (!context->ford_scan_started) {
            link_gtk_card_append_status(ford, "Pending manufacturer scan", "state-warning");
        } else {
            (void)snprintf(status, sizeof(status), "%zu detected · %zu endpoints scanned",
                fordlink_module_scanner_responsive_count(&context->ford_scan),
                fordlink_module_scanner_result_count(&context->ford_scan));
            link_gtk_card_append_status(
                ford, status,
                context->ford_scan_complete ? "state-success" : "state-warning");
            for (index = 0U;
                 index < fordlink_module_scanner_result_count(&context->ford_scan);
                 ++index) {
                const FordlinkModuleScanResult *result =
                    fordlink_module_scanner_result_at(&context->ford_scan, index);
                char key[48];
                char value[256];
                if (result == NULL || !result->responded) continue;
                (void)snprintf(key, sizeof(key), "%s",
                    result->endpoint != NULL ? result->endpoint->module_key : "ECU");
                if (fordlink_module_scan_format_result(
                        result, value, sizeof(value)) < 0)
                    (void)snprintf(value, sizeof(value), "Responded");
                if (result->part_number[0] != '\0') {
                    size_t used = strlen(value);
                    if (used + 10U < sizeof(value))
                        (void)snprintf(value + used, sizeof(value) - used,
                            " · Part %s", result->part_number);
                }
                link_gtk_card_append_detail(ford, key, value);
                ++shown;
            }
            if (shown == 0U && context->ford_scan_complete)
                link_gtk_card_append_note(ford,
                    "No corroborated HS-CAN module endpoint answered this read-only pass.");
        }
        link_gtk_card_append_note(ford,
            "The scan uses only UDS 0x22 identity reads and 0x19 DTC inventory. State-changing services remain blocked.");
        gtk_box_append(GTK_BOX(body), ford);
    }
}

static void append_faults(GtkWidget *body, ProductContext *context)
{
    GtkWidget *card = link_gtk_card_new(
        "FAULT MEMORY",
        "Stored, pending and permanent standard diagnostic faults");
    if (!context->diagnostic_valid) {
        link_gtk_card_append_status(card, "Not scanned", "state-warning");
    } else {
        append_dtcs(card, "Stored", &context->diagnostic.stored_dtcs);
        append_dtcs(card, "Pending", &context->diagnostic.pending_dtcs);
        append_dtcs(card, "Permanent", &context->diagnostic.permanent_dtcs);
    }
    gtk_box_append(GTK_BOX(body), card);
}

static void append_live(GtkWidget *body, ProductContext *context,
                        const char *kicker, const char *title)
{
    GtkWidget *card = link_gtk_card_new(kicker, title);
    size_t shown = 0U;
    unsigned int pid;
    for (pid = 1U; pid < 256U; ++pid) {
        const LinkObd2PidDefinition *definition;
        char key[180];
        char value[96];
        if (!context->diagnostic_valid ||
            !link_obd2_pid_set_contains(
                &context->diagnostic.supported_pids, (uint8_t)pid))
            continue;
        definition = link_obd2_pid_definition(1U, (uint8_t)pid);
        (void)snprintf(key, sizeof(key), "PID 0x%02X · %s",
            pid,
            definition != NULL && definition->name != NULL
                ? definition->name : link_obd2_pid_name((uint8_t)pid));
        if (context->sample_valid[pid])
            format_sample(&context->samples[pid], value, sizeof(value));
        else
            (void)snprintf(value, sizeof(value), "Supported · waiting for sample");
        link_gtk_card_append_detail(card, key, value);
        ++shown;
    }
    if (shown == 0U)
        link_gtk_card_append_status(card, "No live PID data yet", "state-warning");
    gtk_box_append(GTK_BOX(body), card);
}

static void append_dashboard(GtkWidget *body, ProductContext *context)
{
    static const uint8_t dashboard_pids[] = {0x0cU, 0x0dU, 0x05U, 0x10U, 0x11U, 0x42U};
    GtkWidget *card = link_gtk_card_new("AT-A-GLANCE", "Ford powertrain dashboard");
    size_t index;

    link_gtk_card_append_status(card,
        context->diagnostic_ready ? "LIVE SAMPLES" : stage(context),
        context->diagnostic_ready ? "state-success" : "state-warning");

    for (index = 0U; index < sizeof(dashboard_pids) / sizeof(dashboard_pids[0]); ++index) {
        uint8_t pid = dashboard_pids[index];
        const LinkObd2PidDefinition *definition =
            link_obd2_pid_definition(1U, pid);
        const char *name = definition != NULL && definition->name != NULL
            ? definition->name : link_obd2_pid_name(pid);
        char value[96];
        if (context->sample_valid[pid])
            format_sample(&context->samples[pid], value, sizeof(value));
        else if (context->diagnostic_valid &&
                 !link_obd2_pid_set_contains(&context->diagnostic.supported_pids, pid))
            (void)snprintf(value, sizeof(value), "Not supported");
        else
            (void)snprintf(value, sizeof(value), "Waiting");
        link_gtk_card_append_detail(card, name, value);
    }
    gtk_box_append(GTK_BOX(body), card);
}

static void append_graphs(GtkWidget *body, ProductContext *context)
{
    GtkWidget *card = link_gtk_card_new("INSTRUMENT TRACES", "Signal history");
    char samples[64];
    char channels[64];
    size_t channel_count = 0U;
    unsigned int pid;
    for (pid = 1U; pid < 256U; ++pid)
        if (context->sample_valid[pid]) ++channel_count;
    (void)snprintf(samples, sizeof(samples), "%zu samples observed",
                   context->live_sample_count);
    (void)snprintf(channels, sizeof(channels), "%zu live channels",
                   channel_count);
    link_gtk_card_append_detail(card, "Current session", samples);
    link_gtk_card_append_detail(card, "Sampled parameters", channels);
    link_gtk_card_append_detail(card, "Source", "Real LINK telemetry only");
    link_gtk_card_append_note(card,
        "Signal history is retained by the shared telemetry/evidence layer; synthetic traces are never generated.");
    gtk_box_append(GTK_BOX(body), card);
}

static void append_log(GtkWidget *body, ProductContext *context)
{
    GtkWidget *card = link_gtk_card_new("SESSION RECORDER", "Diagnostic evidence");
    char samples[64];
    (void)snprintf(samples, sizeof(samples), "%zu live samples",
                   context->live_sample_count);
    link_gtk_card_append_status(card, stage(context),
        context->diagnostic_ready ? "state-success" : "state-warning");
    link_gtk_card_append_detail(card, "Telemetry", samples);
    link_gtk_card_append_detail(card, "Evidence path", "LINK shared recorder");
    link_gtk_card_append_note(card,
        "Use SAVE SESSION in the shared shell to preserve the timestamped investigation record.");
    gtk_box_append(GTK_BOX(body), card);
}

static void append_settings(GtkWidget *body, ProductContext *context)
{
    GtkWidget *card = link_gtk_card_new("FORDLINK", "Application and adapter information");
    link_gtk_card_append_detail(card, "Version", fordlink_version());
    link_gtk_card_append_detail(card, "Product", "Ford diagnostics");
    link_gtk_card_append_detail(card, "Adapter", context->adapter_identity[0] != '\0'
        ? context->adapter_identity : "No adapter connected");
    link_gtk_card_append_detail(card, "Standards core", "LINK");
    link_gtk_card_append_detail(card, "Diagnostic mode", "Read-only discovery and live data");
    link_gtk_card_append_detail(card, "Manufacturer layer",
        "Ford-specific knowledge remains evidence-backed");
    gtk_box_append(GTK_BOX(body), card);
}

static void append_tests(GtkWidget *body, ProductContext *context)
{
    GtkWidget *card = link_gtk_card_new("TESTS", "Readiness and diagnostic checks");
    link_gtk_card_append_status(
        card, context->diagnostic_ready ? "DIAGNOSTICS READY" : stage(context),
        context->diagnostic_ready ? "state-success" : "state-warning");
    link_gtk_card_append_note(
        card,
        "Readiness, monitor results and verified manufacturer self-tests belong in this task.");
    gtk_box_append(GTK_BOX(body), card);
}

static void append_services(GtkWidget *body, ProductContext *context)
{
    GtkWidget *card = link_gtk_card_new("SERVICES", "Vehicle procedures");
    link_gtk_card_append_status(
        card,
        context->diagnostic_ready ? "NO VERIFIED PROCEDURE ENABLED" : "CONNECT TO EVALUATE SERVICES",
        "state-warning");
    link_gtk_card_append_note(
        card,
        "Only explicitly supported service procedures are presented here. Unverified operations remain unavailable.");
    gtk_box_append(GTK_BOX(body), card);
}

static void render(size_t section, GtkWidget *body, void *opaque)
{
    ProductContext *context = opaque;
    switch ((LinkWorkspaceSection)section) {
    case LINK_WORKSPACE_VEHICLE:
        append_vehicle(body, context);
        append_modules(body, context);
        break;
    case LINK_WORKSPACE_FAULTS:
        append_faults(body, context);
        break;
    case LINK_WORKSPACE_TABLE:
        append_live(body, context, "PARAMETER TABLE", "Dense live parameter table");
        break;
    case LINK_WORKSPACE_DASHBOARD:
        append_dashboard(body, context);
        break;
    case LINK_WORKSPACE_GRAPHS:
        append_graphs(body, context);
        break;
    case LINK_WORKSPACE_TESTS:
        append_tests(body, context);
        break;
    case LINK_WORKSPACE_SERVICES:
        append_services(body, context);
        break;
    case LINK_WORKSPACE_LOG:
        append_log(body, context);
        break;
    case LINK_WORKSPACE_SETTINGS:
        append_settings(body, context);
        break;
    case LINK_WORKSPACE_SECTION_COUNT:
        break;
    case LINK_WORKSPACE_OBD:
    case LINK_WORKSPACE_MODULES:
    case LINK_WORKSPACE_LIVE_DATA:
        break;
    }
}

static bool ford_scan_begin(void *opaque)
{
    ProductContext *context = opaque;
    context->ford_scan_started = true;
    context->ford_scan_complete = false;
    context->ford_scan_success = false;
    return fordlink_module_scanner_begin(&context->ford_scan);
}

static bool ford_scan_next_command(char *buffer, size_t buffer_size,
                                   size_t *written, uint64_t *timeout_ms,
                                   void *opaque)
{
    ProductContext *context = opaque;
    return fordlink_module_scanner_next_command(
        &context->ford_scan, buffer, buffer_size, written, timeout_ms);
}

static bool ford_scan_accept_response(const LinkElm327Response *response,
                                      bool *complete, void *opaque)
{
    ProductContext *context = opaque;
    bool done = false;
    if (!fordlink_module_scanner_accept(&context->ford_scan, response, &done))
        return false;
    if (done) context->ford_scan_complete = true;
    *complete = done;
    return true;
}

static bool ford_scan_progress_changed(void *opaque)
{
    ProductContext *context = opaque;
    return fordlink_module_scanner_take_progress_changed(&context->ford_scan);
}

static void ford_scan_finished(bool complete, void *opaque)
{
    ProductContext *context = opaque;
    context->ford_scan_complete = true;
    context->ford_scan_success = complete;
}

static const LinkGtkManufacturerExtension ford_manufacturer_extension = {
    .begin = ford_scan_begin,
    .next_command = ford_scan_next_command,
    .accept_response = ford_scan_accept_response,
    .progress_changed = ford_scan_progress_changed,
    .finished = ford_scan_finished
};

static void connection_changed(LinkTransport *transport, bool connected,
                               const char *identity, void *opaque)
{
    ProductContext *context = opaque;
    (void)transport;
    context->connected = connected;
    (void)snprintf(context->adapter_identity, sizeof(context->adapter_identity),
                   "%s", identity != NULL ? identity : "");
    if (!connected) {
        context->diagnostic_valid = false;
        context->diagnostic_ready = false;
        context->live_sample_count = 0U;
        memset(context->sample_valid, 0, sizeof(context->sample_valid));
        memset(&context->ford_scan, 0, sizeof(context->ford_scan));
        context->ford_scan_started = false;
        context->ford_scan_complete = false;
        context->ford_scan_success = false;
    }
}

static void diagnostic_changed(const LinkDiagnosticFlow *flow,
                               const LinkDiagnosticFlowEvent *event,
                               bool active, bool ready, void *opaque)
{
    ProductContext *context = opaque;
    (void)active;
    if (flow != NULL) {
        context->diagnostic = *flow;
        context->diagnostic_valid = true;
    }
    context->diagnostic_ready = ready;
    if (event != NULL && event->kind == LINK_DIAGNOSTIC_FLOW_EVENT_LIVE_SAMPLE) {
        uint8_t pid = event->sample.pid;
        context->samples[pid] = event->sample;
        context->sample_valid[pid] = true;
        ++context->live_sample_count;
    }
}

int main(int argc, char **argv)
{
    ProductContext context = {0};
    LinkGtkShellDescriptor descriptor = {0};
    descriptor.app_id = "com.github.InfiltratorProjects.FORDLINK";
    descriptor.window_title = "FORDLINK · Ford Diagnostics";
    descriptor.brand_name = "FORDLINK";
    descriptor.brand_subtitle = "FORD · LINK DIAGNOSTICS";
    descriptor.version = fordlink_version();
    descriptor.emblem_resource = "/com/github/Infiltrator-Projects/FORDLINK/fordlink-emblem.png";
    descriptor.css = css;
    descriptor.render_section = render;
    descriptor.connection_changed = connection_changed;
    descriptor.diagnostic_changed = diagnostic_changed;
    descriptor.manufacturer_extension = &ford_manufacturer_extension;
    descriptor.use_client_side_titlebar = true;
    descriptor.context = &context;
    return link_gtk_shell_run(argc, argv, &descriptor);
}
