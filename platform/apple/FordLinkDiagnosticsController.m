// SPDX-License-Identifier: GPL-3.0-or-later
#import "FordLinkDiagnosticsController.h"
#import "link/obd2.h"
#import "fordlink/module_scan.h"
#import "fordlink/procedure.h"
#import "fordlink/module.h"
#include <stdlib.h>

@interface FordLinkDiagnosticsController () <LinkDiagnosticsControllerDelegate>
@property(nonatomic, strong) LinkDiagnosticsController *shared;
@property(nonatomic, assign) FordlinkModuleScanner *fordScanner;
@end

@implementation FordLinkDiagnosticsController
- (instancetype)init {
    self = [super init];
    if (self) {
        LinkDiagnosticFlowConfig config = LINK_DIAGNOSTIC_FLOW_CONFIG_INIT;
        config.preserve_pid_discovery_response_headers = true;
        config.preserve_live_response_headers = true;
        config.manufacturer_extension_after_standard_dtcs = true;
        config.restore_adapter_after_manufacturer_extension = true;
        _shared = [[LinkDiagnosticsController alloc]
            initWithProductSlug:@"fordlink"
            flowConfig:config
            liveStatusText:@"Live diagnostics active"
            simulatedLiveStatusText:@"Simulated diagnostics active"
            standardVINStatusText:@"Reading standard VIN"];
        _shared.delegate = self;
        _fordScanner = calloc(1U, sizeof(*_fordScanner));
        if (_fordScanner == NULL) return nil;
    }
    return self;
}

- (void)dealloc
{
    free(_fordScanner);
}
- (NSString *)statusText { return self.shared.statusText; }
- (NSString *)peripheralName { return self.shared.peripheralName; }
- (NSString *)adapterIdentifier { return self.shared.adapterIdentifier; }
- (NSString *)vehicleVINText {
    const LinkDiagnosticFlow *flow = [self.shared diagnosticFlow];
    if (flow != NULL && flow->standard_vin_available && flow->standard_vin[0] != '\0')
        return [NSString stringWithUTF8String:flow->standard_vin];
    return @"Waiting for standard VIN";
}
- (NSString *)faultScanStatusText { return self.shared.faultScanStatusText; }
- (NSArray<NSString *> *)storedDTCs { return self.shared.storedDTCs; }
- (NSArray<NSString *> *)pendingDTCs { return self.shared.pendingDTCs; }
- (NSArray<NSString *> *)permanentDTCs { return self.shared.permanentDTCs; }
- (NSString *)readinessStatusText { return self.shared.readinessStatusText; }
- (NSArray<NSString *> *)readinessMonitorStatus { return self.shared.readinessMonitorStatus; }
- (NSArray<NSString *> *)freezeFrameContext { return self.shared.freezeFrameContext; }
- (NSString *)diagnosticCapabilityText { return self.shared.diagnosticCapabilityText; }
- (NSString *)diagnosticCapabilityDetailText { return self.shared.diagnosticCapabilityDetailText; }
- (NSString *)standardResponderSummary { return self.shared.standardResponderSummary; }
- (NSString *)supportedPIDSummary { return self.shared.supportedPIDSummary; }
- (BOOL)isActive { return self.shared.isActive; }
- (BOOL)isReady { return self.shared.isReady; }
- (NSUInteger)recordedSampleCount { return self.shared.recordedSampleCount; }
- (NSArray<NSString *> *)standardLiveValueRows
{
    return self.shared.standardLiveValueRows;
}

- (NSString *)fordModuleSummary
{
    if (self.fordScanner == NULL) return @"Ford module scanner unavailable";
    if (self.shared.isManufacturerExtensionActive)
        return [NSString stringWithFormat:@"Scanning Ford modules · %zu detected",
            fordlink_module_scanner_responsive_count(self.fordScanner)];
    if (fordlink_module_scanner_result_count(self.fordScanner) == 0U)
        return self.shared.isActive ? @"Ford module scan pending" : @"Connect to scan Ford modules";
    return [NSString stringWithFormat:@"%zu Ford modules detected · %zu endpoints scanned",
        fordlink_module_scanner_responsive_count(self.fordScanner),
        fordlink_module_scanner_result_count(self.fordScanner)];
}

- (NSArray<NSString *> *)fordModuleRows
{
    NSMutableArray<NSString *> *rows = [NSMutableArray array];
    size_t index;
    if (self.fordScanner == NULL) return rows;
    for (index = 0U; index < fordlink_module_scanner_result_count(self.fordScanner); ++index) {
        const FordlinkModuleScanResult *result =
            fordlink_module_scanner_result_at(self.fordScanner, index);
        char formatted[256];
        if (result == NULL || !result->responded) continue;
        if (fordlink_module_scan_format_result(result, formatted, sizeof(formatted)) < 0)
            continue;
        NSMutableString *row =
            [NSMutableString stringWithUTF8String:formatted];
        if (result->part_number[0] != '\0')
            [row appendFormat:@" · Part %s", result->part_number];
        if (result->software_number[0] != '\0')
            [row appendFormat:@" · SW %s", result->software_number];
        if (result->serial_number[0] != '\0')
            [row appendFormat:@" · S/N %s", result->serial_number];
        [rows addObject:row];
    }
    return rows;
}

- (NSArray<NSString *> *)fordProcedureCapabilityRows
{
    NSMutableArray<NSString *> *rows = [NSMutableArray array];
    size_t index;
    for (index = 0U; index < fordlink_procedure_definition_count(); ++index) {
        const FordlinkProcedureDefinition *procedure =
            fordlink_procedure_definition_at(index);
        if (procedure == NULL) continue;
        [rows addObject:[NSString stringWithFormat:@"%s · %s · %s",
            procedure->name,
            fordlink_procedure_category_name(procedure->category),
            procedure->module_family]];
    }
    return rows;
}

- (NSArray<NSString *> *)fordModuleCatalogueRows
{
    NSMutableArray<NSString *> *rows = [NSMutableArray array];
    size_t index;
    for (index = 0U; index < fordlink_module_definition_count(); ++index) {
        const FordlinkModuleDefinition *module = fordlink_module_definition_at(index);
        const FordlinkDiagnosticEndpoint *endpoint;
        NSMutableString *row;
        size_t occurrence = 0U;
        if (module == NULL) continue;
        row = [NSMutableString stringWithFormat:@"%s · %s", module->key, module->name];
        endpoint = fordlink_diagnostic_endpoint_for_module(module->key, occurrence++);
        if (endpoint != NULL) {
            [row appendFormat:@" · 0x%03X/0x%03X · %s",
                (unsigned int)endpoint->request_id,
                (unsigned int)endpoint->response_id,
                fordlink_endpoint_confidence_name(endpoint->confidence)];
            while ((endpoint = fordlink_diagnostic_endpoint_for_module(
                        module->key, occurrence++)) != NULL) {
                [row appendFormat:@" · alt 0x%03X/0x%03X",
                    (unsigned int)endpoint->request_id,
                    (unsigned int)endpoint->response_id];
            }
        } else {
            [row appendString:@" · endpoint profile pending"];
        }
        [rows addObject:row];
    }
    return rows;
}

- (NSArray<NSNumber *> *)rpmHistory
{
    return [self.shared recentValuesForPID:0x0C limit:60U];
}
- (NSArray<NSNumber *> *)speedHistory
{
    return [self.shared recentValuesForPID:0x0D limit:60U];
}
- (NSArray<NSNumber *> *)coolantHistory
{
    return [self.shared recentValuesForPID:0x05 limit:60U];
}
- (NSArray<NSNumber *> *)throttleHistory
{
    return [self.shared recentValuesForPID:0x11 limit:60U];
}

- (void)start
{
    if (self.fordScanner != NULL) memset(self.fordScanner, 0, sizeof(*self.fordScanner));
    [self.shared start];
}
- (void)disconnect
{
    if (self.fordScanner != NULL) memset(self.fordScanner, 0, sizeof(*self.fordScanner));
    [self.shared disconnect];
}
- (NSData *)csvDataSnapshot { return [self.shared csvDataSnapshot]; }
- (BOOL)driveFordModuleScanner
{
    char command[LINK_ELM327_MAX_COMMAND];
    size_t written = 0U;
    uint64_t timeoutMs = 0U;
    if (self.fordScanner == NULL ||
        !fordlink_module_scanner_next_command(
            self.fordScanner, command, sizeof(command), &written, &timeoutMs)) {
        return [self.shared completeManufacturerExtensionRestoringAdapter:YES];
    }
    [self.shared updateStatusText:@"Scanning Ford modules"];
    if (![self.shared beginManufacturerCommand:command timeout:timeoutMs]) {
        return [self.shared completeManufacturerExtensionRestoringAdapter:YES];
    }
    return YES;
}

- (void)linkDiagnosticsControllerBeginManufacturerExtension:
    (LinkDiagnosticsController *)controller
{
    (void)controller;
    if (self.fordScanner == NULL || !fordlink_module_scanner_begin(self.fordScanner)) {
        [self.shared completeManufacturerExtensionRestoringAdapter:YES];
        return;
    }
    (void)[self driveFordModuleScanner];
}

- (void)linkDiagnosticsController:(LinkDiagnosticsController *)controller
  didReceiveManufacturerResponse:(const LinkElm327Response *)response
{
    bool complete = false;
    (void)controller;
    if (self.fordScanner == NULL ||
        !fordlink_module_scanner_accept(self.fordScanner, response, &complete)) {
        [self.shared completeManufacturerExtensionRestoringAdapter:YES];
    } else if (complete) {
        [self.shared completeManufacturerExtensionRestoringAdapter:YES];
    } else {
        (void)[self driveFordModuleScanner];
    }
    [self.delegate diagnosticsControllerDidUpdate:self];
}

- (void)linkDiagnosticsController:(LinkDiagnosticsController *)controller
 manufacturerExtensionDidFailWithStatus:(NSString *)status
{
    (void)controller;
    (void)status;
    [self.delegate diagnosticsControllerDidUpdate:self];
}

- (void)linkDiagnosticsControllerDidUpdate:(LinkDiagnosticsController *)controller {
    (void)controller;
    [self.delegate diagnosticsControllerDidUpdate:self];
}
@end
