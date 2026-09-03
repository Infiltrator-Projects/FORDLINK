// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_IDENTITY_H
#define FORDLINK_IDENTITY_H
#include "fordlink/module.h"
#include "link/ecu_probe.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct FordlinkIdentityDid {uint16_t did;const char *key;const char *name;bool basic_scan;} FordlinkIdentityDid;
size_t fordlink_identity_did_count(void);
const FordlinkIdentityDid *fordlink_identity_did_at(size_t index);
const FordlinkIdentityDid *fordlink_identity_did(uint16_t did);
size_t fordlink_identity_basic_requests(LinkEcuProbeDidRequest *requests,size_t capacity);
bool fordlink_identity_probe_profile(const FordlinkDiagnosticEndpoint *endpoint,LinkEcuProbeDidRequest *requests,size_t request_capacity,LinkEcuProbeProfile *profile);
#ifdef __cplusplus
}
#endif
#endif
