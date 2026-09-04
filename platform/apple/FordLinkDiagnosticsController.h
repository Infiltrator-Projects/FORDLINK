// SPDX-License-Identifier: GPL-3.0-or-later
#import <Foundation/Foundation.h>
#import "../../src/link/platform/apple/LinkDiagnosticsController.h"

NS_ASSUME_NONNULL_BEGIN
@class FordLinkDiagnosticsController;
@protocol FordLinkDiagnosticsControllerDelegate <NSObject>
- (void)diagnosticsControllerDidUpdate:(FordLinkDiagnosticsController *)controller;
@end

@interface FordLinkDiagnosticsController : NSObject
@property(nonatomic, weak, nullable) id<FordLinkDiagnosticsControllerDelegate> delegate;
@property(nonatomic, copy, readonly) NSString *statusText;
@property(nonatomic, copy, readonly, nullable) NSString *peripheralName;
@property(nonatomic, copy, readonly, nullable) NSString *adapterIdentifier;
@property(nonatomic, copy, readonly) NSString *vehicleVINText;
@property(nonatomic, copy, readonly) NSString *faultScanStatusText;
@property(nonatomic, copy, readonly) NSString *faultScanPresentationStateName;
@property(nonatomic, copy, readonly) NSArray<NSString *> *storedDTCs;
@property(nonatomic, copy, readonly) NSArray<NSString *> *pendingDTCs;
@property(nonatomic, copy, readonly) NSArray<NSString *> *permanentDTCs;
@property(nonatomic, copy, readonly) NSString *readinessStatusText;
@property(nonatomic, copy, readonly) NSArray<NSString *> *readinessMonitorStatus;
@property(nonatomic, copy, readonly) NSArray<NSString *> *freezeFrameContext;
@property(nonatomic, copy, readonly) NSString *diagnosticCapabilityText;
@property(nonatomic, copy, readonly) NSString *diagnosticCapabilityDetailText;
@property(nonatomic, copy, readonly) NSString *standardResponderSummary;
@property(nonatomic, copy, readonly) NSString *supportedPIDSummary;
@property(nonatomic, copy, readonly) NSArray<NSString *> *standardLiveValueRows;
@property(nonatomic, copy, readonly) NSString *fordModuleSummary;
@property(nonatomic, copy, readonly) NSArray<NSString *> *fordModuleRows;
@property(nonatomic, copy, readonly) NSArray<NSString *> *fordProcedureCapabilityRows;
@property(nonatomic, copy, readonly) NSArray<NSString *> *fordModuleCatalogueRows;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *rpmHistory;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *speedHistory;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *coolantHistory;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *throttleHistory;
@property(nonatomic, copy, readonly) NSArray<NSString *> *availableLanguageTags;
@property(nonatomic, copy, readonly) NSArray<NSString *> *availableLanguageNames;
@property(nonatomic, copy, readonly) NSString *selectedLanguageTag;
@property(nonatomic, copy, readonly) NSArray<NSString *> *availableMeasurementSystemKeys;
@property(nonatomic, copy, readonly) NSArray<NSString *> *availableMeasurementSystemNames;
@property(nonatomic, copy, readonly) NSString *selectedMeasurementSystemKey;
@property(nonatomic, copy, readonly) NSString *rpmDisplayUnit;
@property(nonatomic, copy, readonly) NSString *speedDisplayUnit;
@property(nonatomic, copy, readonly) NSString *coolantDisplayUnit;
@property(nonatomic, copy, readonly) NSString *throttleDisplayUnit;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *rpmDisplayRange;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *speedDisplayRange;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *coolantDisplayRange;
@property(nonatomic, copy, readonly) NSArray<NSNumber *> *throttleDisplayRange;
@property(nonatomic, readonly, getter=isActive) BOOL active;
@property(nonatomic, readonly, getter=isReady) BOOL ready;
@property(nonatomic, readonly) NSUInteger recordedSampleCount;
- (void)start;
- (void)disconnect;
- (BOOL)scanFordModules;
- (NSString *)localizedTextForKey:(NSString *)key;
- (void)setSelectedLanguageTag:(NSString *)tag;
- (void)setSelectedMeasurementSystemKey:(NSString *)key;
- (nullable NSData *)csvDataSnapshot;
@end
NS_ASSUME_NONNULL_END
