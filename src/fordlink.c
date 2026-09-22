// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/fordlink.h"
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif
#if defined(__APPLE__) && TARGET_OS_IOS
#ifndef LINK_SOURCE_REVISION
#define LINK_SOURCE_REVISION "38826aa2dca78072343a201553460e61cc2f726f"
#define FORDLINK_DEFINED_LINK_SOURCE_REVISION 1
#endif
#include "link/platform/apple/LinkPortableCore.c"
#ifdef FORDLINK_DEFINED_LINK_SOURCE_REVISION
#undef FORDLINK_DEFINED_LINK_SOURCE_REVISION
#undef LINK_SOURCE_REVISION
#endif
/* Xcode compiles one product-owned C translation unit for Ford-specific code. */
#include "network.c"
#include "module.c"
#include "identity.c"
#include "module_scan.c"
#include "signal.c"
#include "procedure.c"
#endif
const char *fordlink_product_name(void){return "FORDLINK";}
const char *fordlink_brand_name(void){return "Ford";}
const char *fordlink_version(void){return FORDLINK_VERSION;}
