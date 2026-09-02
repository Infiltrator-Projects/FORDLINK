// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/fordlink.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    if (strcmp(fordlink_product_name(), "FORDLINK") != 0) return 1;
    if (strcmp(fordlink_brand_name(), "Ford") != 0) return 2;
    if (strcmp(fordlink_version(), "0.2.8") != 0) return 3;
    puts("FORDLINK GUI product smoke test passed");
    return 0;
}
