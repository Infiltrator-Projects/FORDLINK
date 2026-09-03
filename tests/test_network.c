// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/network.h"

#include <stdio.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "FAIL: %s\n", #x); return 1; } } while (0)

int main(void)
{
    const FordlinkNetworkDescriptor *hs;
    const FordlinkNetworkDescriptor *ms;
    const FordlinkNetworkDescriptor *hs2;

    CHECK(fordlink_network_descriptor_count() == 7U);

    hs = fordlink_network_descriptor(FORDLINK_NETWORK_HSCAN);
    CHECK(hs != NULL);
    CHECK(strcmp(hs->display_name, "HS-CAN") == 0);
    CHECK(hs->default_bitrate == 500000U);
    CHECK(hs->dlc_can_high_pin == 6U && hs->dlc_can_low_pin == 14U);
    CHECK(hs->directly_exposed_at_dlc);
    CHECK(hs->standard_obd_lane);
    CHECK(!hs->enhanced_access_required);
    CHECK(!hs->profile_specific);

    ms = fordlink_network_descriptor(FORDLINK_NETWORK_MSCAN);
    CHECK(ms != NULL);
    CHECK(strcmp(ms->display_name, "MS-CAN") == 0);
    CHECK(ms->default_bitrate == 125000U);
    CHECK(ms->dlc_can_high_pin == 3U && ms->dlc_can_low_pin == 11U);
    CHECK(ms->directly_exposed_at_dlc);
    CHECK(!ms->standard_obd_lane);
    CHECK(ms->enhanced_access_required);

    hs2 = fordlink_network_descriptor(FORDLINK_NETWORK_HSCAN2);
    CHECK(hs2 != NULL);
    CHECK(hs2->profile_specific);
    CHECK(!hs2->directly_exposed_at_dlc);
    CHECK(hs2->default_bitrate == 0U);

    CHECK(fordlink_network_for_standard_obd_responder(0x7E8U, false) ==
          FORDLINK_NETWORK_HSCAN);
    CHECK(fordlink_network_for_standard_obd_responder(0x7EFU, false) ==
          FORDLINK_NETWORK_HSCAN);
    CHECK(fordlink_network_for_standard_obd_responder(0x726U, false) ==
          FORDLINK_NETWORK_UNKNOWN);
    CHECK(fordlink_network_for_standard_obd_responder(0x18DAF110U, true) ==
          FORDLINK_NETWORK_UNKNOWN);
    CHECK(strcmp(fordlink_network_kind_name(FORDLINK_NETWORK_UNKNOWN), "Unknown") == 0);

    puts("FORDLINK Ford network model passed");
    return 0;
}
