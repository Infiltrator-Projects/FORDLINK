// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_MODULE_H
#define FORDLINK_MODULE_H
#include "fordlink/network.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define FORDLINK_NETWORK_MASK(kind) (UINT32_C(1) << (unsigned int)(kind))
typedef enum FordlinkEndpointConfidence {
    FORDLINK_ENDPOINT_CANDIDATE = 0,
    FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,
    FORDLINK_ENDPOINT_CORROBORATED
} FordlinkEndpointConfidence;
typedef struct FordlinkModuleDefinition {
    const char *key;
    const char *name;
} FordlinkModuleDefinition;
typedef struct FordlinkDiagnosticEndpoint {
    const char *module_key;
    uint32_t request_id;
    uint32_t response_id;
    bool extended_id;
    uint32_t network_candidates;
    FordlinkEndpointConfidence confidence;
    bool scan_hscan_by_default;
    const char *scope;
} FordlinkDiagnosticEndpoint;
size_t fordlink_module_definition_count(void);
const FordlinkModuleDefinition *fordlink_module_definition_at(size_t index);
const FordlinkModuleDefinition *fordlink_module_definition(const char *key);
size_t fordlink_diagnostic_endpoint_count(void);
const FordlinkDiagnosticEndpoint *fordlink_diagnostic_endpoint_at(size_t index);
const FordlinkDiagnosticEndpoint *fordlink_diagnostic_endpoint_for_module(const char *module_key, size_t occurrence);
bool fordlink_endpoint_supports_network(const FordlinkDiagnosticEndpoint *endpoint, FordlinkNetworkKind network);
const char *fordlink_endpoint_confidence_name(FordlinkEndpointConfidence value);
#ifdef __cplusplus
}
#endif
#endif
