// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/procedure.h"
#include <stdio.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"FAIL: %s\n",#x);return 1;}}while(0)
int main(void){size_t i;CHECK(fordlink_procedure_definition_count()>=20U);CHECK(fordlink_procedure_definition("abs_bleed")!=NULL);CHECK(fordlink_procedure_definition("module_configuration")!=NULL);for(i=0U;i<fordlink_procedure_definition_count();++i){const FordlinkProcedureDefinition *v=fordlink_procedure_definition_at(i);CHECK(v!=NULL&&v->key!=NULL&&v->requires_verified_profile);}puts("FORDLINK procedure taxonomy passed");return 0;}
