// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI

private struct Panel<Content: View>: View {
    let title: String
    let content: Content
    init(_ title: String, @ViewBuilder content: () -> Content) {
        self.title = title
        self.content = content()
    }
    var body: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text(title).font(.headline)
            content
        }
        .padding(16)
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(RoundedRectangle(cornerRadius: 16).fill(Color.white.opacity(0.07)))
        .overlay(RoundedRectangle(cornerRadius: 16).stroke(Color.white.opacity(0.12)))
    }
}

struct ContentView: View {
    @StateObject private var model = ConnectionViewModel()

    var body: some View {
        NavigationStack {
            ScrollView {
                VStack(spacing: 14) {
                    HStack {
                        VStack(alignment: .leading, spacing: 3) {
                            Text("FORDLINK").font(.largeTitle.bold())
                            Text("Ford diagnostics · LINK standards core")
                                .foregroundStyle(.secondary)
                        }
                        Spacer()
                        Text(model.versionText).font(.caption.monospaced())
                    }

                    Panel("Connection") {
                        Text(model.statusText)
                        Text(model.peripheralName).foregroundStyle(.secondary)
                        Text(model.adapterIdentifier).font(.caption.monospaced()).foregroundStyle(.secondary)
                        Button(model.isActive ? "Disconnect" : "Connect") {
                            model.isActive ? model.disconnect() : model.connect()
                        }
                        .buttonStyle(.borderedProminent)
                    }

                    Panel("Vehicle") {
                        Text("VIN").font(.caption).foregroundStyle(.secondary)
                        Text(model.vehicleVINText).font(.body.monospaced())
                        Text("Readiness").font(.caption).foregroundStyle(.secondary)
                        Text(model.readinessStatusText)
                    }

                    Panel("Fault memory") {
                        Text(model.faultScanStatusText).foregroundStyle(.secondary)
                        faultGroup("Stored", model.storedDTCs)
                        faultGroup("Pending", model.pendingDTCs)
                        faultGroup("Permanent", model.permanentDTCs)
                    }

                    Panel("Live data") {
                        if model.standardLiveRows.isEmpty {
                            Text("Waiting for advertised SAE live parameters.")
                                .foregroundStyle(.secondary)
                        } else {
                            ForEach(model.standardLiveRows, id: \.self) { row in
                                Text(row).font(.subheadline.monospacedDigit())
                                Divider()
                            }
                        }
                    }

                    Panel("Evidence") {
                        Text("\(model.recordedSampleCount) samples recorded")
                        Button("Prepare diagnostic evidence CSV") {
                            model.prepareCSVExport()
                        }
                        if let url = model.csvExportURL {
                            ShareLink(item: url) { Label("Share CSV", systemImage: "square.and.arrow.up") }
                        }
                    }
                }
                .padding()
            }
            .background(Color.black.ignoresSafeArea())
            .foregroundStyle(.white)
        }
        .preferredColorScheme(.dark)
    }

    @ViewBuilder
    private func faultGroup(_ title: String, _ values: [String]) -> some View {
        Text(title).font(.caption.bold()).foregroundStyle(.secondary)
        if values.isEmpty { Text("None reported").foregroundStyle(.secondary) }
        else { ForEach(values, id: \.self) { Text($0).font(.body.monospaced()) } }
    }
}
