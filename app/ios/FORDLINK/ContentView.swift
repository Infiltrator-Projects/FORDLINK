// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI

private enum ProductTheme {
    static let backgroundTop = Color(red: 0.02, green: 0.09, blue: 0.17)
    static let backgroundMiddle = Color(red: 0.025, green: 0.13, blue: 0.23)
    static let backgroundBottom = Color(red: 0.03, green: 0.18, blue: 0.32)
    static let panel = Color(red: 0.04, green: 0.16, blue: 0.27)
    static let panelRaised = Color(red: 0.055, green: 0.22, blue: 0.37)
    static let border = Color(red: 0.17, green: 0.39, blue: 0.60)
    static let accent = Color(red: 0.13, green: 0.48, blue: 0.82)
    static let primary = Color(red: 0.96, green: 0.98, blue: 1.0)
    static let secondary = Color(red: 0.60, green: 0.75, blue: 0.87)
    static let success = Color(red: 0.34, green: 0.78, blue: 0.48)
    static let warning = Color(red: 0.95, green: 0.68, blue: 0.25)
    static let fault = Color(red: 0.92, green: 0.28, blue: 0.31)
}

private let productTheme = LinkDiagnosticTheme(
    backgroundTop: ProductTheme.backgroundTop,
    backgroundMiddle: ProductTheme.backgroundMiddle,
    backgroundBottom: ProductTheme.backgroundBottom,
    panel: ProductTheme.panel,
    panelRaised: ProductTheme.panelRaised,
    primaryText: ProductTheme.primary,
    secondaryText: ProductTheme.secondary,
    mutedText: ProductTheme.secondary.opacity(0.72),
    border: ProductTheme.border.opacity(0.72),
    accent: ProductTheme.accent,
    success: ProductTheme.success,
    warning: ProductTheme.warning,
    fault: ProductTheme.fault,
    typography: LinkDiagnosticTypography(
        display: .system(size: 29, weight: .semibold),
        body: .body,
        bodyBold: .body.bold(),
        subheadline: .subheadline,
        subheadlineBold: .subheadline.bold(),
        headline: .headline,
        caption: .caption,
        captionBold: .caption.bold(),
        caption2: .caption2,
        caption2Bold: .caption2.bold(),
        title3: .title3,
        title2: .title2.bold()))

private struct ProductBadge: View {
    var body: some View {
        Image("FORDLINKEmblem")
            .resizable()
            .scaledToFit()
            .frame(width: 56, height: 56)
            .shadow(color: .black.opacity(0.28), radius: 7, x: 0, y: 4)
            .accessibilityHidden(true)
    }
}

private extension View {
    func productDiagnosticScreen(_ title: String) -> some View {
        linkDiagnosticScreen(title)
    }
}

struct ContentView: View {
    @StateObject private var model = ConnectionViewModel()

    var body: some View {
        LinkCommandCentreShell(
            showProgress: model.isActive && !model.isReady,
            header: { header },
            progress: { connectionProgress },
            connection: { connectionCard },
            primary: { primaryGrid },
            tools: { supportingTools })
            .linkDiagnosticTheme(productTheme)
    }

    private var header: some View {
        LinkBrandHeader {
            HStack(spacing: 14) {
                ProductBadge()
                VStack(alignment: .leading, spacing: 3) {
                    Text("FORDLINK")
                        .font(.system(size: 29, weight: .bold))
                        .tracking(1.2)
                        .foregroundStyle(ProductTheme.primary)
                    Text("FORD · LINK DIAGNOSTICS")
                        .font(.caption2.bold())
                        .tracking(1.2)
                        .foregroundStyle(ProductTheme.accent)
                    Text("Standards core with Ford-specific diagnostics layered on top")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                        .lineLimit(1)
                }
            }
        } status: {
            LinkStatusPill(text: model.statusText, active: model.isReady)
        }
    }

    private var connectionCard: some View {
        LinkPanel {
            VStack(alignment: .leading, spacing: 12) {
                HStack(alignment: .firstTextBaseline) {
                    VStack(alignment: .leading, spacing: 3) {
                        Text(model.isActive ? "Diagnostic session" : "Vehicle connection")
                            .font(.headline)
                            .foregroundStyle(ProductTheme.primary)
                        Text(model.statusText)
                            .font(.caption)
                            .foregroundStyle(ProductTheme.secondary)
                            .fixedSize(horizontal: false, vertical: true)
                    }
                    Spacer(minLength: 12)
                    Image(systemName: model.isReady
                          ? "checkmark.circle.fill"
                          : model.isActive ? "dot.radiowaves.left.and.right" : "cable.connector")
                        .foregroundStyle(model.isReady ? ProductTheme.success : ProductTheme.accent)
                }

                Button {
                    model.isActive ? model.disconnect() : model.connect()
                } label: {
                    Label(model.isActive ? "Disconnect" : "Connect to vehicle",
                          systemImage: model.isActive ? "cable.connector.slash" : "cable.connector")
                        .font(.subheadline.weight(.semibold))
                        .foregroundStyle(ProductTheme.primary)
                        .frame(maxWidth: .infinity)
                        .padding(.vertical, 11)
                        .background(
                            RoundedRectangle(cornerRadius: 12, style: .continuous)
                                .fill(ProductTheme.accent))
                }
                .buttonStyle(.plain)

                if model.isReady {
                    HStack(spacing: 9) {
                        Image(systemName: "car.side.fill")
                            .foregroundStyle(ProductTheme.accent)
                        VStack(alignment: .leading, spacing: 2) {
                            Text(model.diagnosticCapabilityText)
                                .font(.subheadline.weight(.semibold))
                                .foregroundStyle(ProductTheme.primary)
                            Text(model.vehicleVINText)
                                .font(.caption2.monospaced())
                                .foregroundStyle(ProductTheme.secondary)
                                .lineLimit(1)
                        }
                    }
                } else if !model.isActive {
                    HStack(spacing: 10) {
                        productMetric("(model.fordModuleCatalogueRows.count)", "Ford modules", "cpu")
                        productMetric("(model.fordProcedureCapabilityRows.count)", "Service/test families", "wrench.and.screwdriver")
                    }
                    Text("FORDLINK is ready to scan Ford networks and merge live vehicle evidence with the built-in catalogue.")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                }
            }
        }
    }

    private var connectionProgress: some View {
        LinkPanel {
            VStack(alignment: .leading, spacing: 7) {
                Label("Connecting to vehicle", systemImage: "dot.radiowaves.left.and.right")
                    .font(.headline)
                    .foregroundStyle(ProductTheme.primary)
                Text(model.statusText)
                    .font(.subheadline)
                    .foregroundStyle(ProductTheme.secondary)
                    .fixedSize(horizontal: false, vertical: true)
                if model.peripheralName != "No adapter" {
                    Text(model.peripheralName)
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                }
            }
        }
    }

    private var primaryGrid: some View {
        LinkDiagnosticGrid {
            LinkTaskTile(.vehicle) { ProductVehicleView(model: model) }
            LinkTaskTile(.log) { ProductEvidenceView(model: model) }
            LinkTaskTile(.errors) { ProductFaultsView(model: model) }
            LinkTaskTile(.dashboard) { ProductDashboardView(model: model) }
            LinkTaskTile(.table) { ProductTableView(model: model) }
            LinkTaskTile(.graph) { ProductGraphView(model: model) }
            LinkTaskTile(.tests) { ProductTestsView(model: model) }
            LinkTaskTile(.services) { ProductServicesView(model: model) }
        }
    }

    private var supportingTools: some View {
        LinkPanel {
            VStack(alignment: .leading, spacing: 7) {
                LinkSectionHeader(title: "Settings", kicker: "Application")
                LinkCompactLink("Settings", "Adapter and application information", "gearshape.fill") {
                    ProductSettingsView(model: model)
                }
            }
        }
    }


}

private struct ProductVehicleView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Vehicle", systemImage: "car.side.fill") {
                    productValueRow("VIN", model.vehicleVINText, icon: "number")
                    productDivider
                    productValueRow("Diagnostic generation", model.diagnosticCapabilityText, icon: "cpu")
                    productDivider
                    productValueRow("Readiness", model.readinessStatusText, icon: "checklist")
                }
                LinkLabeledPanel(title: "Connection", systemImage: "cable.connector") {
                    productValueRow("Adapter", model.peripheralName, icon: "antenna.radiowaves.left.and.right")
                    productDivider
                    productValueRow("ELM identity", model.adapterIdentifier, icon: "memorychip")
                    productDivider
                    productValueRow("State", model.statusText, icon: "checkmark.seal")
                }
                LinkLabeledPanel(title: "Control units", systemImage: "square.stack.3d.up.fill") {
                    NavigationLink {
                        ProductModulesView(model: model)
                    } label: {
                        HStack {
                            VStack(alignment: .leading, spacing: 3) {
                                Text("Responder and module inventory")
                                    .font(.headline)
                                    .foregroundStyle(ProductTheme.primary)
                                Text("Open standard responders and manufacturer capability details")
                                    .font(.caption)
                                    .foregroundStyle(ProductTheme.secondary)
                            }
                            Spacer()
                            Image(systemName: "chevron.right")
                                .foregroundStyle(ProductTheme.accent)
                        }
                    }
                    .buttonStyle(.plain)
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Vehicle")
    }
}

private struct ProductModulesView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Ford networks", systemImage: "point.3.filled.connected.trianglepath.dotted") {
                    productValueRow("HS-CAN", "Standard OBD lane · direct DLC network", icon: "network")
                    productDivider
                    productValueRow("MS-CAN", "Enhanced Ford lane · supported adapter required", icon: "arrow.triangle.branch")
                    productDivider
                    productValueRow("Additional CAN lanes", "Vehicle/profile specific · never guessed", icon: "square.stack.3d.up")
                    Text("FORDLINK now models Ford networks separately from modules. This lets discovery grow into HS-CAN, MS-CAN and later gateway-exposed lanes without folding everything into generic OBD.")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                        .fixedSize(horizontal: false, vertical: true)
                }
                LinkLabeledPanel(title: "Ford module scan", systemImage: "rectangle.3.group.bubble.left.fill") {
                    productValueRow("Status", model.fordModuleSummary, icon: "magnifyingglass")
                    if model.fordModuleRows.isEmpty {
                        Text(model.isActive
                             ? "The read-only Ford module census runs automatically after the standard fault pass."
                             : "Connect to identify corroborated Ford HS-CAN module endpoints.")
                            .font(.caption)
                            .foregroundStyle(ProductTheme.secondary)
                    } else {
                        ForEach(model.fordModuleRows, id: \.self) { row in
                            productDivider
                            Text(row)
                                .font(.caption.monospaced())
                                .foregroundStyle(ProductTheme.primary)
                                .fixedSize(horizontal: false, vertical: true)
                        }
                    }
                    Text("Only UDS identity reads and module DTC inventory are transmitted by this scan.")
                        .font(.caption2)
                        .foregroundStyle(ProductTheme.secondary)
                }
                LinkLabeledPanel(title: "Ford module catalogue", systemImage: "books.vertical.fill") {
                    Text("(model.fordModuleCatalogueRows.count) module families available before connection")
                        .font(.caption.bold())
                        .foregroundStyle(ProductTheme.secondary)
                    ForEach(model.fordModuleCatalogueRows, id: \.self) { row in
                        productDivider
                        Text(row)
                            .font(.caption.monospaced())
                            .foregroundStyle(ProductTheme.primary)
                            .fixedSize(horizontal: false, vertical: true)
                    }
                }
                LinkLabeledPanel(title: "Standard responders", systemImage: "square.stack.3d.up.fill") {
                    productValueRow("Physical responders", model.standardResponderSummary, icon: "point.3.connected.trianglepath.dotted")
                    productDivider
                    productValueRow("Advertised parameters", model.supportedPIDSummary, icon: "waveform.path.ecg")
                    productDivider
                    productValueRow("Capability", model.diagnosticCapabilityText, icon: "cpu")
                    Text("This is the LINK-owned standard responder inventory. Ford-specific module discovery is added only when it is evidence-backed.")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                        .fixedSize(horizontal: false, vertical: true)
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Modules")
    }
}

private struct ProductFaultsView: View {
    @ObservedObject var model: ConnectionViewModel
    private var total: Int { model.storedDTCs.count + model.pendingDTCs.count + model.permanentDTCs.count }

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Errors", systemImage: "exclamationmark.triangle.fill") {
                    HStack {
                        Text(model.faultScanStatusText)
                            .font(.subheadline)
                            .foregroundStyle(ProductTheme.secondary)
                        Spacer()
                        Text("\(total)")
                            .font(.title2.monospacedDigit().weight(.bold))
                            .foregroundStyle(total == 0 ? ProductTheme.success : ProductTheme.fault)
                    }
                    faultGroup("Stored", model.storedDTCs)
                    faultGroup("Pending", model.pendingDTCs)
                    faultGroup("Permanent", model.permanentDTCs)
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Errors")
    }

    @ViewBuilder
    private func faultGroup(_ title: String, _ values: [String]) -> some View {
        VStack(alignment: .leading, spacing: 5) {
            Text(title)
                .font(.caption.bold())
                .foregroundStyle(ProductTheme.secondary)
            if values.isEmpty {
                Text("None reported")
                    .font(.subheadline)
                    .foregroundStyle(ProductTheme.secondary)
            } else {
                ForEach(values, id: \.self) { value in
                    Text(value)
                        .font(.body.monospaced())
                        .foregroundStyle(ProductTheme.primary)
                }
            }
        }
        .padding(.top, 4)
    }
}

private struct ProductTableView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Table", systemImage: "waveform.path.ecg") {
                    if model.standardLiveRows.isEmpty {
                        Text(model.isActive ? "Waiting for advertised standard parameters and samples." : "Connect to populate supported standard live data.")
                            .font(.subheadline)
                            .foregroundStyle(ProductTheme.secondary)
                    } else {
                        ForEach(model.standardLiveRows, id: \.self) { row in
                            Text(row)
                                .font(.subheadline.monospacedDigit())
                                .foregroundStyle(ProductTheme.primary)
                            if row != model.standardLiveRows.last { productDivider }
                        }
                    }
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Table")
    }
}

private struct ProductDashboardView: View {
    @ObservedObject var model: ConnectionViewModel
    private var totalFaults: Int { model.storedDTCs.count + model.pendingDTCs.count + model.permanentDTCs.count }

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "At a glance", systemImage: "gauge.with.dots.needle.67percent") {
                    HStack(spacing: 10) {
                        productMetric("(model.fordModuleRows.count)", "Detected modules", "cpu")
                        productMetric("(totalFaults)", "Faults", "exclamationmark.triangle")
                    }
                    HStack(spacing: 10) {
                        productMetric("(model.recordedSampleCount)", "Samples", "waveform")
                        productMetric("(model.fordProcedureCapabilityRows.count)", "Procedures", "wrench")
                    }
                    productDivider
                    productValueRow("Session", model.isReady ? "Live diagnostics active" : model.statusText, icon: "dot.radiowaves.left.and.right")
                    productDivider
                    productValueRow("Readiness", model.readinessStatusText, icon: "checklist")
                }

                LinkLabeledPanel(title: "Live highlights", systemImage: "speedometer") {
                    let highlights = Array(model.standardLiveRows.prefix(6))
                    if highlights.isEmpty {
                        Text("Live channels are ready; connect to populate values.")
                            .font(.subheadline)
                            .foregroundStyle(ProductTheme.secondary)
                    } else {
                        ForEach(highlights, id: \.self) { row in
                            Text(row)
                                .font(.subheadline.monospacedDigit())
                                .foregroundStyle(ProductTheme.primary)
                            if row != highlights.last { productDivider }
                        }
                    }
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Dashboard")
    }
}

private struct ProductEvidenceView: View {private struct ProductEvidenceView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Diagnostic evidence", systemImage: "doc.text.magnifyingglass") {
                    productValueRow("Recorded samples", "\(model.recordedSampleCount)", icon: "waveform")
                    productDivider
                    productValueRow("Session state", model.statusText, icon: "checkmark.seal")
                    Button {
                        model.prepareCSVExport()
                    } label: {
                        Label("Prepare evidence CSV", systemImage: "doc.badge.plus")
                            .font(.subheadline.weight(.semibold))
                            .frame(maxWidth: .infinity)
                            .padding(.vertical, 10)
                    }
                    .buttonStyle(.borderedProminent)
                    .tint(ProductTheme.accent)

                    if let url = model.csvExportURL {
                        ShareLink(item: url) {
                            Label("Share CSV", systemImage: "square.and.arrow.up")
                                .frame(maxWidth: .infinity)
                        }
                        .buttonStyle(.bordered)
                        .tint(ProductTheme.accent)
                    }
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Log")
    }
}

private struct ProductGraphView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Live graphs", systemImage: "chart.xyaxis.line") {
                    Text(model.isActive
                         ? "Live LINK telemetry · newest sample at right"
                         : "Connect to populate the traces; graph channels are already configured.")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                    productTrace("Engine RPM", "rpm", model.rpmHistory, 0...7000)
                    productDivider
                    productTrace("Vehicle speed", "km/h", model.speedHistory, 0...220)
                    productDivider
                    productTrace("Coolant temperature", "°C", model.coolantHistory, -40...130)
                    productDivider
                    productTrace("Throttle position", "%", model.throttleHistory, 0...100)
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Graph")
    }
}

private struct ProductTestsView: View {private struct ProductTestsView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Readiness", systemImage: "checkmark.square.fill") {
                    productValueRow("Status", model.readinessStatusText, icon: "checklist")
                    if model.readinessMonitorStatus.isEmpty {
                        Text("No readiness-monitor detail has been returned yet.")
                            .font(.subheadline)
                            .foregroundStyle(ProductTheme.secondary)
                    } else {
                        ForEach(model.readinessMonitorStatus, id: \.self) { row in
                            Text(row).font(.subheadline).foregroundStyle(ProductTheme.primary)
                        }
                    }
                }
                LinkLabeledPanel(title: "Freeze-frame context", systemImage: "camera.metering.matrix") {
                    if model.freezeFrameContext.isEmpty {
                        Text("No standard freeze-frame context captured.")
                            .font(.subheadline)
                            .foregroundStyle(ProductTheme.secondary)
                    } else {
                        ForEach(model.freezeFrameContext, id: \.self) { row in
                            Text(row).font(.subheadline).foregroundStyle(ProductTheme.primary)
                        }
                    }
                }
                LinkLabeledPanel(title: "Additional tests", systemImage: "checkmark.seal") {
                    Text("FORDLINK now has explicit capability metadata for module self-tests and component/actuator tests. Actual commands remain disabled until the active module/profile is independently verified.")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Tests")
    }
}

private struct ProductServicesView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Services", systemImage: "wrench.and.screwdriver.fill") {
                    Text(model.isActive
                         ? "No verified service procedure is enabled for this session."
                         : "Connect to evaluate supported service procedures.")
                        .font(.headline)
                        .foregroundStyle(ProductTheme.primary)
                    Text("Capability catalogue")
                        .font(.caption.bold())
                        .foregroundStyle(ProductTheme.secondary)
                    ForEach(model.fordProcedureCapabilityRows, id: \.self) { row in
                        HStack(alignment: .top, spacing: 9) {
                            Image(systemName: "wrench.adjustable.fill")
                                .foregroundStyle(ProductTheme.accent)
                                .frame(width: 20)
                            Text(row)
                                .font(.caption)
                                .foregroundStyle(ProductTheme.primary)
                                .fixedSize(horizontal: false, vertical: true)
                            Spacer(minLength: 0)
                            Image(systemName: "lock.fill")
                                .font(.caption2)
                                .foregroundStyle(ProductTheme.secondary)
                        }
                        .padding(9)
                        .background(
                            RoundedRectangle(cornerRadius: 10, style: .continuous)
                                .fill(ProductTheme.panelRaised.opacity(0.65)))
                    }
                    Text("These are recognised Ford procedure families, not enabled commands. A procedure becomes executable only after its module, session, security and preconditions are verified.")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Services")
    }
}

private struct ProductSettingsView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "FORDLINK", systemImage: "gearshape.fill") {
                    productValueRow("Version", model.versionText, icon: "number.circle")
                    productDivider
                    productValueRow("Adapter", model.peripheralName, icon: "antenna.radiowaves.left.and.right")
                    productDivider
                    productValueRow("Standards core", "LINK · read-only diagnostic flow", icon: "shield.lefthalf.filled")
                    Text("Common OBD presentation and geometry come from LINK. Ford-specific diagnostic knowledge remains a separate evidence-backed layer.")
                        .font(.caption)
                        .foregroundStyle(ProductTheme.secondary)
                        .fixedSize(horizontal: false, vertical: true)
                }
            }
            .padding(16)
        }
        .productDiagnosticScreen("Settings")
    }
}

private func productMetric(_ value: String, _ label: String, _ icon: String) -> some View {
    VStack(alignment: .leading, spacing: 6) {
        HStack {
            Image(systemName: icon).foregroundStyle(ProductTheme.accent)
            Spacer()
        }
        Text(value)
            .font(.title2.monospacedDigit().weight(.bold))
            .foregroundStyle(ProductTheme.primary)
        Text(label)
            .font(.caption2.bold())
            .foregroundStyle(ProductTheme.secondary)
    }
    .frame(maxWidth: .infinity, alignment: .leading)
    .padding(11)
    .background(
        RoundedRectangle(cornerRadius: 12, style: .continuous)
            .fill(ProductTheme.panelRaised.opacity(0.72)))
    .overlay(
        RoundedRectangle(cornerRadius: 12, style: .continuous)
            .stroke(ProductTheme.border.opacity(0.55), lineWidth: 1))
}

private func productTrace(
    _ title: String,
    _ unit: String,
    _ values: [Double],
    _ nominalRange: ClosedRange<Double>
) -> some View {
    VStack(alignment: .leading, spacing: 7) {
        HStack {
            Text(title)
                .font(.subheadline.bold())
                .foregroundStyle(ProductTheme.primary)
            Spacer()
            Text(values.last.map { String(format: "%.1f %@", $0, unit) } ?? "—")
                .font(.caption.monospacedDigit().bold())
                .foregroundStyle(ProductTheme.accent)
        }
        GeometryReader { geometry in
            ZStack {
                RoundedRectangle(cornerRadius: 9, style: .continuous)
                    .fill(ProductTheme.panelRaised.opacity(0.55))
                Path { path in
                    let count = max(values.count, 2)
                    let width = max(geometry.size.width - 12, 1)
                    let height = max(geometry.size.height - 12, 1)
                    let lo = nominalRange.lowerBound
                    let hi = nominalRange.upperBound
                    guard !values.isEmpty, hi > lo else { return }
                    for (index, raw) in values.enumerated() {
                        let x = 6 + CGFloat(index) / CGFloat(count - 1) * width
                        let clipped = min(max(raw, lo), hi)
                        let norm = (clipped - lo) / (hi - lo)
                        let y = 6 + (1 - CGFloat(norm)) * height
                        if index == 0 { path.move(to: CGPoint(x: x, y: y)) }
                        else { path.addLine(to: CGPoint(x: x, y: y)) }
                    }
                }
                .stroke(ProductTheme.accent, style: StrokeStyle(lineWidth: 2, lineJoin: .round, lineCap: .round))
                if values.isEmpty {
                    Text("waiting for samples")
                        .font(.caption2)
                        .foregroundStyle(ProductTheme.secondary)
                }
            }
        }
        .frame(height: 92)
    }
}

private func productValueRow(_ label: String, _ value: String, icon: String) -> some View {
    HStack(alignment: .top, spacing: 11) {
        Image(systemName: icon)
            .foregroundStyle(ProductTheme.accent)
            .frame(width: 22)
        VStack(alignment: .leading, spacing: 2) {
            Text(label)
                .font(.caption.bold())
                .foregroundStyle(ProductTheme.secondary)
            Text(value)
                .font(.subheadline)
                .foregroundStyle(ProductTheme.primary)
                .fixedSize(horizontal: false, vertical: true)
        }
        Spacer(minLength: 0)
    }
}

private var productDivider: some View {
    Divider().overlay(ProductTheme.border.opacity(0.55))
}
