// SPDX-License-Identifier: GPL-3.0-or-later
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif
#if defined(__APPLE__) && TARGET_OS_IOS
#include "../link/src/obd2/obd2.c"
#include "../link/src/obd2/pid_catalogue.c"
#include "../link/src/obd2/j1979da.c"
#include "../link/src/obd2/dtc_knowledge.c"
#else
typedef int fordlink_obd2_translation_unit;
#endif
