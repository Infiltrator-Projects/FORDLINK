// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_PROCEDURE_H
#define FORDLINK_PROCEDURE_H
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum FordlinkProcedureCategory{FORDLINK_PROCEDURE_SELF_TEST=0,FORDLINK_PROCEDURE_COMPONENT_TEST,FORDLINK_PROCEDURE_RESET,FORDLINK_PROCEDURE_MAINTENANCE,FORDLINK_PROCEDURE_RELEARN,FORDLINK_PROCEDURE_CALIBRATION,FORDLINK_PROCEDURE_BLEED,FORDLINK_PROCEDURE_REGENERATION,FORDLINK_PROCEDURE_CONFIGURATION}FordlinkProcedureCategory;
typedef struct FordlinkProcedureDefinition{const char *key,*name;FordlinkProcedureCategory category;const char *module_family;bool changes_vehicle_state,may_require_extended_session,may_require_security_access,requires_verified_profile;}FordlinkProcedureDefinition;
size_t fordlink_procedure_definition_count(void);
const FordlinkProcedureDefinition *fordlink_procedure_definition_at(size_t index);
const FordlinkProcedureDefinition *fordlink_procedure_definition(const char *key);
const char *fordlink_procedure_category_name(FordlinkProcedureCategory category);
#ifdef __cplusplus
}
#endif
#endif
