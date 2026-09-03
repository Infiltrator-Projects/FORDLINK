// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_NETWORK_H
#define FORDLINK_NETWORK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FordlinkNetworkKind {
    FORDLINK_NETWORK_UNKNOWN = 0,
    FORDLINK_NETWORK_HSCAN,
    FORDLINK_NETWORK_MSCAN,
    FORDLINK_NETWORK_HSCAN2,
    FORDLINK_NETWORK_HSCAN3,
    FORDLINK_NETWORK_HSCAN4,
    FORDLINK_NETWORK_HSCAN5,
    FORDLINK_NETWORK_MSCAN2,
    FORDLINK_NETWORK_COUNT
} FordlinkNetworkKind;

typedef struct FordlinkNetworkDescriptor {
    FordlinkNetworkKind kind;
    const char *key;
    const char *display_name;
    uint32_t default_bitrate;
    uint8_t dlc_can_high_pin;
    uint8_t dlc_can_low_pin;
    bool directly_exposed_at_dlc;
    bool standard_obd_lane;
    bool enhanced_access_required;
    bool profile_specific;
} FordlinkNetworkDescriptor;

size_t fordlink_network_descriptor_count(void);
const FordlinkNetworkDescriptor *fordlink_network_descriptor_at(size_t index);
const FordlinkNetworkDescriptor *fordlink_network_descriptor(FordlinkNetworkKind kind);
const char *fordlink_network_kind_name(FordlinkNetworkKind kind);

/*
 * Classify only evidence that is unambiguous from the SAE OBD responder range.
 * Enhanced Ford physical addresses are deliberately NOT guessed here.
 */
FordlinkNetworkKind fordlink_network_for_standard_obd_responder(
    uint32_t responder_id, bool extended_id);

#ifdef __cplusplus
}
#endif

#endif
