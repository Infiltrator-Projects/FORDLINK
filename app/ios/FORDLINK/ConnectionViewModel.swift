// SPDX-License-Identifier: GPL-3.0-or-later
import Combine
import Foundation

@MainActor
final class ConnectionViewModel: NSObject, ObservableObject, @preconcurrency FordLinkDiagnosticsControllerDelegate {
    @Published private(set) var statusText = "Idle"
    @Published private(set) var peripheralName = "No adapter"
    @Published private(set) var adapterIdentifier = "Unknown"
    @Published private(set) var vehicleVINText = "Waiting for standard VIN"
    @Published private(set) var faultScanStatusText = "Not scanned"
    @Published private(set) var faultScanPresentationStateName = "not-scanned"
    @Published private(set) var storedDTCs = [String]()
    @Published private(set) var pendingDTCs = [String]()
    @Published private(set) var permanentDTCs = [String]()
    @Published private(set) var readinessStatusText = "Not read"
    @Published private(set) var readinessMonitorStatus = [String]()
    @Published private(set) var freezeFrameContext = [String]()
    @Published private(set) var diagnosticCapabilityText = "Unknown / probing"
    @Published private(set) var diagnosticCapabilityDetailText = ""
    @Published private(set) var standardResponderSummary = "0 physical responders"
    @Published private(set) var supportedPIDSummary = "0 advertised PIDs"
    @Published private(set) var standardLiveRows = [String]()
    @Published private(set) var fordModuleSummary = "Connect to scan Ford modules"
    @Published private(set) var fordModuleRows = [String]()
    @Published private(set) var fordProcedureCapabilityRows = [String]()
    @Published private(set) var fordModuleCatalogueRows = [String]()
    @Published private(set) var rpmHistory = [Double]()
    @Published private(set) var speedHistory = [Double]()
    @Published private(set) var coolantHistory = [Double]()
    @Published private(set) var throttleHistory = [Double]()
    @Published private(set) var languageTags = [String]()
    @Published private(set) var languageNames = [String]()
    @Published private(set) var selectedLanguageID = "en-AU"
    @Published private(set) var measurementKeys = [String]()
    @Published private(set) var measurementNames = [String]()
    @Published private(set) var selectedMeasurementID = "metric"
    @Published private(set) var rpmDisplayUnit = "rpm"
    @Published private(set) var speedDisplayUnit = "km/h"
    @Published private(set) var coolantDisplayUnit = "degC"
    @Published private(set) var throttleDisplayUnit = "%"
    @Published private(set) var rpmGraphMinimum = 0.0
    @Published private(set) var rpmGraphMaximum = 7000.0
    @Published private(set) var speedGraphMinimum = 0.0
    @Published private(set) var speedGraphMaximum = 220.0
    @Published private(set) var coolantGraphMinimum = -40.0
    @Published private(set) var coolantGraphMaximum = 130.0
    @Published private(set) var throttleGraphMinimum = 0.0
    @Published private(set) var throttleGraphMaximum = 100.0
    @Published private(set) var isActive = false
    @Published private(set) var isReady = false
    @Published private(set) var recordedSampleCount = 0
    @Published private(set) var versionText = "Unknown"
    @Published private(set) var csvExportURL: URL?

    private let controller = FordLinkDiagnosticsController()

    override init() {
        super.init()
        controller.delegate = self
        if let value = fordlink_version() { versionText = String(cString: value) }
        refresh()
    }

    func connect() { if !isActive { controller.start() } }
    func disconnect() { controller.disconnect() }
    func scanFordModules() {
        _ = controller.scanFordModules()
        refresh()
    }

    var interfaceLocaleIdentifier: String { selectedLanguageID }

    func localizedText(_ key: String) -> String {
        controller.localizedText(forKey: key)
    }

    func selectLanguage(_ id: String) {
        controller.setSelectedLanguageTag(id)
        refresh()
    }

    func selectMeasurementSystem(_ id: String) {
        controller.setSelectedMeasurementSystemKey(id)
        refresh()
    }

    func prepareCSVExport() {
        guard let snapshot = controller.csvDataSnapshot() else { return }
        let data = snapshot as Data
        let url = FileManager.default.temporaryDirectory
            .appendingPathComponent("FORDLINK-diagnostic-evidence-\(UUID().uuidString).csv")
        do {
            try data.write(to: url, options: .atomic)
            csvExportURL = url
        } catch {
            csvExportURL = nil
        }
    }

    func diagnosticsControllerDidUpdate(_ controller: FordLinkDiagnosticsController) {
        refresh()
    }

    private func refresh() {
        statusText = controller.statusText
        peripheralName = controller.peripheralName ?? "No adapter"
        adapterIdentifier = controller.adapterIdentifier ?? "Unknown"
        vehicleVINText = controller.vehicleVINText
        faultScanStatusText = controller.faultScanStatusText
        faultScanPresentationStateName = controller.faultScanPresentationStateName
        storedDTCs = controller.storedDTCs
        pendingDTCs = controller.pendingDTCs
        permanentDTCs = controller.permanentDTCs
        readinessStatusText = controller.readinessStatusText
        readinessMonitorStatus = controller.readinessMonitorStatus
        freezeFrameContext = controller.freezeFrameContext
        diagnosticCapabilityText = controller.diagnosticCapabilityText
        diagnosticCapabilityDetailText = controller.diagnosticCapabilityDetailText
        standardResponderSummary = controller.standardResponderSummary
        supportedPIDSummary = controller.supportedPIDSummary
        standardLiveRows = controller.standardLiveValueRows
        fordModuleSummary = controller.fordModuleSummary
        fordModuleRows = controller.fordModuleRows
        fordProcedureCapabilityRows = controller.fordProcedureCapabilityRows
        fordModuleCatalogueRows = controller.fordModuleCatalogueRows
        rpmHistory = controller.rpmHistory.map { $0.doubleValue }
        speedHistory = controller.speedHistory.map { $0.doubleValue }
        coolantHistory = controller.coolantHistory.map { $0.doubleValue }
        throttleHistory = controller.throttleHistory.map { $0.doubleValue }

        languageTags = controller.availableLanguageTags
        languageNames = controller.availableLanguageNames
        selectedLanguageID = controller.selectedLanguageTag
        measurementKeys = controller.availableMeasurementSystemKeys
        measurementNames = controller.availableMeasurementSystemNames
        selectedMeasurementID = controller.selectedMeasurementSystemKey

        rpmDisplayUnit = controller.rpmDisplayUnit
        speedDisplayUnit = controller.speedDisplayUnit
        coolantDisplayUnit = controller.coolantDisplayUnit
        throttleDisplayUnit = controller.throttleDisplayUnit
        applyRange(controller.rpmDisplayRange, fallback: 0...7000) {
            rpmGraphMinimum = $0; rpmGraphMaximum = $1
        }
        applyRange(controller.speedDisplayRange, fallback: 0...220) {
            speedGraphMinimum = $0; speedGraphMaximum = $1
        }
        applyRange(controller.coolantDisplayRange, fallback: -40...130) {
            coolantGraphMinimum = $0; coolantGraphMaximum = $1
        }
        applyRange(controller.throttleDisplayRange, fallback: 0...100) {
            throttleGraphMinimum = $0; throttleGraphMaximum = $1
        }

        isActive = controller.isActive
        isReady = controller.isReady
        recordedSampleCount = Int(clamping: controller.recordedSampleCount)
    }

    private func applyRange(
        _ values: [NSNumber],
        fallback: ClosedRange<Double>,
        _ assign: (Double, Double) -> Void
    ) {
        guard values.count >= 2 else {
            assign(fallback.lowerBound, fallback.upperBound)
            return
        }
        assign(values[0].doubleValue, values[1].doubleValue)
    }
}
