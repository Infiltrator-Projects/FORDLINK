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
        ZStack {
            Circle()
                .fill(ProductTheme.primary)
                .frame(width: 52, height: 52)
            Text("F")
                .font(.system(size: 25, weight: .black, design: .rounded))
                .foregroundStyle(ProductTheme.backgroundTop)
        }
        .overlay(Circle().stroke(ProductTheme.accent, lineWidth: 2))
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
                    Text("Connect once to identify the standard diagnostic surface, faults, responders and live data.")
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
            LinkHomeTile("OBD", "Common legacy, transitional and standard diagnostics", "cpu") {
                LinkStandardObdView(snapshot: obdSnapshot)
            }
            LinkHomeTile("Faults", "Stored, pending and permanent fault memory", "exclamationmark.triangle.fill") {
                ProductFaultsView(model: model)
            }
            LinkHomeTile("Live Data", "Advertised standard sensors and measurements", "waveform.path.ecg") {
                ProductLiveDataView(model: model)
            }
            LinkHomeTile("Vehicle", "VIN, adapter and diagnostic identity", "car.side.fill") {
                ProductVehicleView(model: model)
            }
            LinkHomeTile("Modules", "Standard responder inventory and capability", "square.stack.3d.up.fill") {
                ProductModulesView(model: model)
            }
        }
    }

    private var supportingTools: some View {
        LinkPanel {
            VStack(alignment: .leading, spacing: 7) {
                LinkSectionHeader(title: "Tools", kicker: "Secondary")
                LinkCompactLink("Dashboard", "At-a-glance diagnostic measurements", "gauge.with.dots.needle.67percent") {
                    ProductDashboardView(model: model)
                }
                Divider().overlay(ProductTheme.border.opacity(0.55))
                LinkCompactLink("Evidence", "Session samples and CSV export", "doc.text.magnifyingglass") {
                    ProductEvidenceView(model: model)
                }
                Divider().overlay(ProductTheme.border.opacity(0.55))
                LinkCompactLink("Settings", "Adapter and application information", "gearshape.fill") {
                    ProductSettingsView(model: model)
                }
            }
        }
    }

    private var obdSnapshot: LinkStandardObdSnapshot {
        LinkStandardObdSnapshot(
            capability: model.diagnosticCapabilityText,
            capabilityDetail: model.diagnosticCapabilityDetailText,
            vin: model.vehicleVINText,
            responderSummary: model.standardResponderSummary,
            pidSummary: model.supportedPIDSummary,
            readiness: model.readinessStatusText,
            readinessMonitors: model.readinessMonitorStatus,
            freezeFrame: model.freezeFrameContext,
            storedDTCs: model.storedDTCs,
            pendingDTCs: model.pendingDTCs,
            permanentDTCs: model.permanentDTCs,
            liveRows: model.standardLiveRows)
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
                LinkLabeledPanel(title: "Fault memory", systemImage: "exclamationmark.triangle.fill") {
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
        .productDiagnosticScreen("Faults")
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

private struct ProductLiveDataView: View {
    @ObservedObject var model: ConnectionViewModel

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "Live data", systemImage: "waveform.path.ecg") {
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
        .productDiagnosticScreen("Live Data")
    }
}

private struct ProductDashboardView: View {
    @ObservedObject var model: ConnectionViewModel
    private var totalFaults: Int { model.storedDTCs.count + model.pendingDTCs.count + model.permanentDTCs.count }

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 15) {
                LinkLabeledPanel(title: "At a glance", systemImage: "gauge.with.dots.needle.67percent") {
                    productValueRow("Session", model.isReady ? "Live diagnostics active" : model.statusText, icon: "dot.radiowaves.left.and.right")
                    productDivider
                    productValueRow("Readiness", model.readinessStatusText, icon: "checklist")
                    productDivider
                    productValueRow("Fault records", "\(totalFaults)", icon: "exclamationmark.triangle")
                    productDivider
                    productValueRow("Samples recorded", "\(model.recordedSampleCount)", icon: "waveform")
                }

                LinkLabeledPanel(title: "Live highlights", systemImage: "speedometer") {
                    let highlights = Array(model.standardLiveRows.prefix(6))
                    if highlights.isEmpty {
                        Text("No live measurements yet.")
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

private struct ProductEvidenceView: View {
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
        .productDiagnosticScreen("Evidence")
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
