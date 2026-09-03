// SPDX-License-Identifier: GPL-3.0-or-later
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif
#if defined(__APPLE__) && TARGET_OS_IOS
#include "../link/src/uds/uds.c"
#include "../link/src/uds/uds_services.c"
#include "../link/src/uds/uds_server.c"
#include "../link/src/uds/uds_dtc.c"
#else
typedef int fordlink_uds_translation_unit;
#endif
