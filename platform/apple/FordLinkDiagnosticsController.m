// SPDX-License-Identifier: GPL-3.0-or-later
#import "FordLinkDiagnosticsController.h"
#import "link/obd2.h"

@interface FordLinkDiagnosticsController () <LinkDiagnosticsControllerDelegate>
@property(nonatomic, strong) LinkDiagnosticsController *shared;
@end

@implementation FordLinkDiagnosticsController
- (instancetype)init {
    self = [super init];
    if (self) {
        LinkDiagnosticFlowConfig config = LINK_DIAGNOSTIC_FLOW_CONFIG_INIT;
        config.preserve_pid_discovery_response_headers = true;
        config.preserve_live_response_headers = true;
        _shared = [[LinkDiagnosticsController alloc]
            initWithProductSlug:@"fordlink"
            flowConfig:config
            liveStatusText:@"Live diagnostics active"
            simulatedLiveStatusText:@"Simulated diagnostics active"
            standardVINStatusText:@"Reading standard VIN"];
        _shared.delegate = self;
    }
    return self;
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
- (BOOL)isActive { return self.shared.isActive; }
- (BOOL)isReady { return self.shared.isReady; }
- (NSUInteger)recordedSampleCount { return self.shared.recordedSampleCount; }
- (NSArray<NSString *> *)standardLiveValueRows {
    const LinkDiagnosticFlow *flow = [self.shared diagnosticFlow];
    NSMutableArray<NSString *> *rows = [NSMutableArray array];
    if (flow == NULL) return rows;
    for (NSUInteger raw = 1U; raw <= UINT8_MAX; ++raw) {
        const uint8_t pid = (uint8_t)raw;
        if (!link_obd2_pid_set_contains(&flow->supported_pids, pid)) continue;
        const LinkObd2PidDefinition *definition = link_obd2_pid_definition(1U, pid);
        const char *name = definition != NULL && definition->name != NULL
            ? definition->name : link_obd2_pid_name(pid);
        NSArray<NSNumber *> *history = [self.shared recentValuesForPID:pid limit:1U];
        if (history.count != 0U) {
            NSString *unit = definition != NULL && definition->unit != NULL
                ? [NSString stringWithUTF8String:definition->unit] : @"";
            [rows addObject:[NSString stringWithFormat:@"PID %02lX · %s — %.3f%@%@",
                (unsigned long)pid, name != NULL ? name : "Unknown",
                history.lastObject.doubleValue,
                unit.length != 0U ? @" " : @"", unit]];
        } else {
            [rows addObject:[NSString stringWithFormat:@"PID %02lX · %s — waiting",
                (unsigned long)pid, name != NULL ? name : "Unknown"]];
        }
    }
    return rows;
}
- (void)start { [self.shared start]; }
- (void)disconnect { [self.shared disconnect]; }
- (NSData *)csvDataSnapshot { return [self.shared csvDataSnapshot]; }
- (void)linkDiagnosticsControllerDidUpdate:(LinkDiagnosticsController *)controller {
    (void)controller;
    [self.delegate diagnosticsControllerDidUpdate:self];
}
@end
