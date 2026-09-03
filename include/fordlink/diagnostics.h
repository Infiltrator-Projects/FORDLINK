// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_DIAGNOSTICS_H
#define FORDLINK_DIAGNOSTICS_H

#include "fordlink/obd2.h"
#include "fordlink/network.h"
#include "fordlink/module.h"
#include "fordlink/identity.h"
#include "fordlink/module_scan.h"
#include "fordlink/signal.h"
#include "fordlink/procedure.h"
#include "link/diagnostic_flow.h"
#include "link/diagnostic_request.h"
#include "link/discover.h"
#include "link/doip.h"
#include "link/ecu_probe.h"
#include "link/elm327.h"
#include "link/elm327_can.h"
#include "link/elm327_probe.h"
#include "link/elm327_session.h"
#include "link/isotp.h"
#include "link/kwp2000.h"
#include "link/parameter.h"
#include "link/parameter_store.h"
#include "link/scheduler.h"
#include "link/telemetry.h"
#include "link/transport.h"
#include "link/uds.h"
#include "link/uds_dtc.h"
#include "link/uds_services.h"

typedef LinkTransport FordlinkTransport;
typedef LinkTransportStatus FordlinkTransportStatus;
typedef LinkAdapterKind FordlinkAdapterKind;
typedef LinkAdapterCapabilities FordlinkAdapterCapabilities;
typedef LinkElm327Response FordlinkElm327Response;
typedef LinkElm327Session FordlinkElm327Session;
typedef LinkIsoTpCanFrame FordlinkIsoTpCanFrame;
typedef LinkUdsResponse FordlinkUdsResponse;
typedef LinkDoipHeader FordlinkDoipHeader;
typedef LinkParameterStore FordlinkParameterStore;
typedef LinkScheduler FordlinkScheduler;
typedef LinkTelemetryStore FordlinkTelemetryStore;
typedef LinkDiagnosticFlow FordlinkDiagnosticFlow;
typedef LinkDiagnosticRequestDefinition FordlinkDiagnosticRequestDefinition;
typedef LinkEcuProbe FordlinkEcuProbe;

#define fordlink_adapter_kind_from_bluetooth_name link_adapter_kind_from_bluetooth_name
#define fordlink_adapter_kind_name link_adapter_kind_name
#define fordlink_adapter_capabilities link_adapter_capabilities
#define fordlink_adapter_has_capability link_adapter_has_capability
#define fordlink_transport_is_valid link_transport_is_valid
#define fordlink_elm327_protocol_definition_count link_elm327_protocol_definition_count
#define fordlink_elm327_protocol_definition_at link_elm327_protocol_definition_at
#define fordlink_elm327_protocol_definition link_elm327_protocol_definition
#define fordlink_isotp_can_data_length_is_valid link_isotp_can_data_length_is_valid
#define fordlink_uds_standard_service_count link_uds_standard_service_count
#define fordlink_uds_standard_service_find link_uds_standard_service_find
#define fordlink_doip_build_diagnostic_message link_doip_build_diagnostic_message
#define fordlink_doip_decode_diagnostic_message link_doip_decode_diagnostic_message
#define fordlink_safety_classify link_safety_classify
#define fordlink_parameter_obd2_definition_count link_parameter_obd2_definition_count
#define fordlink_parameter_store_init link_parameter_store_init
#define fordlink_parameter_store_definition_count link_parameter_store_definition_count
#define fordlink_scheduler_init link_scheduler_init
#define fordlink_scheduler_next link_scheduler_next
#define fordlink_telemetry_store_init link_telemetry_store_init
#define fordlink_telemetry_store_history_count link_telemetry_store_history_count
#define fordlink_diagnostic_execution_mode_for_adapter link_diagnostic_execution_mode_for_adapter
#define fordlink_diagnostic_request_supported_by_adapter link_diagnostic_request_supported_by_adapter
#define fordlink_ecu_probe_profile_is_valid link_ecu_probe_profile_is_valid

#endif
