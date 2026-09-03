// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/fordlink.h"
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif
#if defined(__APPLE__) && TARGET_OS_IOS
#include "link/src/core/workspace.c"
#include "link/src/core/i18n.c"
#include "link/src/core/i18n_platform.c"
#include "link/src/core/language_pack.c"
#include "link/src/core/units.c"
#include "link/src/core/fuel_economy.c"
#include "link/src/core/diagnostic_request.c"
#include "link/src/core/doip.c"
#include "link/src/core/diagnostic_flow.c"
#include "link/src/core/diagnostic_capability.c"
#include "link/src/core/isotp.c"
#include "link/src/core/parameter.c"
#include "link/src/core/scheduler.c"
#ifndef LINK_SOURCE_REVISION
#define LINK_SOURCE_REVISION "a6bf14208bbb358d5098911fcbc380ce1a76d161"
#define FORDLINK_DEFINED_LINK_SOURCE_REVISION 1
#endif
#include "link/src/core/telemetry.c"
#ifdef FORDLINK_DEFINED_LINK_SOURCE_REVISION
#undef FORDLINK_DEFINED_LINK_SOURCE_REVISION
#undef LINK_SOURCE_REVISION
#endif
#include "link/src/core/transport.c"
#include "link/src/core/mercedes_me_adapter.c"
#define read_u16_be fordlink_native_read_u16_be
#define write_u16_be fordlink_native_write_u16_be
#include "link/src/core/mercedes_me_native_protocol.c"
#undef read_u16_be
#undef write_u16_be
#include "link/src/core/mercedes_me_diagnostic.c"
#include "link/src/core/mercedes_me_data_ids.c"
#include "link/src/core/mercedes_me_diaglogic.c"
#include "link/src/core/mercedes_me_whisper.c"
#include "link/src/elm327/elm327.c"
#include "link/src/elm327/can.c"
#include "link/src/elm327/probe.c"
#include "link/src/elm327/session.c"
#include "link/src/kwp2000/kwp2000.c"
#include "link/src/discover/safety.c"
#include "link/src/discover/ecu_probe.c"
/* Xcode compiles one product-owned C translation unit. */
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
