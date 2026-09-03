// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/network.h"

static const FordlinkNetworkDescriptor fordlink_networks[] = {
    {
        FORDLINK_NETWORK_HSCAN, "hscan", "HS-CAN",
        500000U, 6U, 14U,
        true, true, false, false
    },
    {
        FORDLINK_NETWORK_MSCAN, "mscan", "MS-CAN",
        125000U, 3U, 11U,
        true, false, true, false
    },
    {
        FORDLINK_NETWORK_HSCAN2, "hscan2", "HS-CAN2",
        0U, 0U, 0U,
        false, false, true, true
    },
    {
        FORDLINK_NETWORK_HSCAN3, "hscan3", "HS-CAN3",
        0U, 0U, 0U,
        false, false, true, true
    },
    {
        FORDLINK_NETWORK_HSCAN4, "hscan4", "HS-CAN4",
        0U, 0U, 0U,
        false, false, true, true
    },
    {
        FORDLINK_NETWORK_HSCAN5, "hscan5", "HS-CAN5",
        0U, 0U, 0U,
        false, false, true, true
    },
    {
        FORDLINK_NETWORK_MSCAN2, "mscan2", "MS-CAN2",
        0U, 0U, 0U,
        false, false, true, true
    }
};

size_t fordlink_network_descriptor_count(void)
{
    return sizeof(fordlink_networks) / sizeof(fordlink_networks[0]);
}

const FordlinkNetworkDescriptor *fordlink_network_descriptor_at(size_t index)
{
    if (index >= fordlink_network_descriptor_count()) return NULL;
    return &fordlink_networks[index];
}

const FordlinkNetworkDescriptor *fordlink_network_descriptor(FordlinkNetworkKind kind)
{
    size_t index;
    for (index = 0U; index < fordlink_network_descriptor_count(); ++index) {
        if (fordlink_networks[index].kind == kind) return &fordlink_networks[index];
    }
    return NULL;
}

const char *fordlink_network_kind_name(FordlinkNetworkKind kind)
{
    const FordlinkNetworkDescriptor *descriptor = fordlink_network_descriptor(kind);
    return descriptor != NULL ? descriptor->display_name : "Unknown";
}

FordlinkNetworkKind fordlink_network_for_standard_obd_responder(
    uint32_t responder_id, bool extended_id)
{
    if (!extended_id && responder_id >= 0x7E8U && responder_id <= 0x7EFU)
        return FORDLINK_NETWORK_HSCAN;
    return FORDLINK_NETWORK_UNKNOWN;
}
