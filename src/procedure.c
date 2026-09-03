// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/procedure.h"
#include <string.h>
static const FordlinkProcedureDefinition p[]={
 {"module_self_test","Module self-test",FORDLINK_PROCEDURE_SELF_TEST,"multi-module",false,false,false,true},
 {"component_test","Component/actuator test",FORDLINK_PROCEDURE_COMPONENT_TEST,"multi-module",true,true,true,true},
 {"abs_bleed","ABS hydraulic bleed",FORDLINK_PROCEDURE_BLEED,"ABS",true,true,true,true},
 {"abs_calibration","ABS/ESC calibration",FORDLINK_PROCEDURE_CALIBRATION,"ABS",true,true,true,true},
 {"bms_reset","Battery monitoring system reset",FORDLINK_PROCEDURE_RESET,"BCM/BMS",true,false,false,true},
 {"dpf_regeneration","Diesel particulate filter regeneration",FORDLINK_PROCEDURE_REGENERATION,"PCM",true,true,true,true},
 {"dpf_reset","DPF learned-value reset",FORDLINK_PROCEDURE_RESET,"PCM",true,true,true,true},
 {"egr_service","EGR clean/reset routine",FORDLINK_PROCEDURE_MAINTENANCE,"PCM",true,true,true,true},
 {"epb_maintenance","Electronic parking brake maintenance mode",FORDLINK_PROCEDURE_MAINTENANCE,"ABS/EPB",true,true,false,true},
 {"kam_reset","Keep-alive memory reset",FORDLINK_PROCEDURE_RESET,"PCM",true,true,false,true},
 {"injector_learning","Injector learning",FORDLINK_PROCEDURE_RELEARN,"PCM",true,true,true,true},
 {"pilot_injection_learning","Pilot injection learning/reset",FORDLINK_PROCEDURE_RELEARN,"PCM",true,true,true,true},
 {"fuel_pump_learning","High-pressure fuel pump learning",FORDLINK_PROCEDURE_RELEARN,"PCM",true,true,true,true},
 {"turbo_vane_reset","Turbo variable-vane reset",FORDLINK_PROCEDURE_RESET,"PCM",true,true,true,true},
 {"tpms_learning","TPMS sensor learning",FORDLINK_PROCEDURE_RELEARN,"BCM/TPMS",true,true,false,true},
 {"tcm_adaptation","Transmission adaptation/teaching",FORDLINK_PROCEDURE_RELEARN,"TCM",true,true,true,true},
 {"module_reset","Control module reset",FORDLINK_PROCEDURE_RESET,"multi-module",true,true,true,true},
 {"module_configuration","Module configuration / As-Built",FORDLINK_PROCEDURE_CONFIGURATION,"multi-module",true,true,true,true},
 {"camera_calibration","Camera / LDWS calibration",FORDLINK_PROCEDURE_CALIBRATION,"IPMA/IPMB",true,true,true,true},
 {"headlamp_calibration","HID/adaptive headlamp calibration",FORDLINK_PROCEDURE_CALIBRATION,"HCM",true,true,true,true},
 {"occupant_calibration","Occupant classification calibration",FORDLINK_PROCEDURE_CALIBRATION,"OCS/RCM",true,true,true,true},
 {"ride_height_calibration","Ride-height calibration",FORDLINK_PROCEDURE_CALIBRATION,"suspension",true,true,true,true},
 {"misfire_profile_learning","Misfire profile learning",FORDLINK_PROCEDURE_RELEARN,"PCM",true,true,false,true}};
size_t fordlink_procedure_definition_count(void){return sizeof(p)/sizeof(p[0]);}
const FordlinkProcedureDefinition *fordlink_procedure_definition_at(size_t i){return i<fordlink_procedure_definition_count()?&p[i]:NULL;}
const FordlinkProcedureDefinition *fordlink_procedure_definition(const char *key){size_t i;if(key==NULL)return NULL;for(i=0U;i<fordlink_procedure_definition_count();++i)if(strcmp(p[i].key,key)==0)return &p[i];return NULL;}
const char *fordlink_procedure_category_name(FordlinkProcedureCategory c){switch(c){case FORDLINK_PROCEDURE_SELF_TEST:return "self-test";case FORDLINK_PROCEDURE_COMPONENT_TEST:return "component-test";case FORDLINK_PROCEDURE_RESET:return "reset";case FORDLINK_PROCEDURE_MAINTENANCE:return "maintenance";case FORDLINK_PROCEDURE_RELEARN:return "relearn";case FORDLINK_PROCEDURE_CALIBRATION:return "calibration";case FORDLINK_PROCEDURE_BLEED:return "bleed";case FORDLINK_PROCEDURE_REGENERATION:return "regeneration";case FORDLINK_PROCEDURE_CONFIGURATION:return "configuration";}return "unknown";}
