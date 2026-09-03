// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/fordlink.h"
#include "link/version.h"

#include <stdio.h>
#include <string.h>

#ifndef FORDLINK_EXPECTED_VERSION
#error "FORDLINK_EXPECTED_VERSION must be supplied by the build"
#endif
#ifndef FORDLINK_EXPECTED_LINK_VERSION
#error "FORDLINK_EXPECTED_LINK_VERSION must be supplied by the build"
#endif

int main(void)
{
    if (strcmp(fordlink_product_name(), "FORDLINK") != 0) return 1;
    if (strcmp(fordlink_brand_name(), "Ford") != 0) return 2;
    if (strcmp(fordlink_version(), FORDLINK_EXPECTED_VERSION) != 0) return 3;
    if (strcmp(LINK_VERSION_STRING, FORDLINK_EXPECTED_LINK_VERSION) != 0) return 4;
    puts("FORDLINK product and pinned LINK smoke test passed");
    return 0;
}
