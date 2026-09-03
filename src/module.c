// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/module.h"
#include <string.h>
static const FordlinkModuleDefinition modules[] = {
 {"PCM","Powertrain Control Module"},{"TCM","Transmission Control Module"},{"ABS","Anti-lock Brake System"},
 {"BCM","Body Control Module"},{"BECMB","Battery Energy Control Module B"},{"BCMB","Body Control Module B"},
 {"IPC","Instrument Panel Cluster"},{"APIM","Accessory Protocol Interface Module"},{"GWM","Gateway Module"},
 {"IPMA","Image Processing Module A"},{"IPMB","Image Processing Module B"},{"RCM","Restraints Control Module"},
 {"DDM","Driver Door Module"},{"PDM","Passenger Door Module"},{"DSM","Driver Seat Module"},
 {"FCIM","Front Controls Interface Module"},{"FCDIM","Front Control/Display Interface Module"},
 {"ACM","Audio Control Module"},{"DSP","Digital Signal Processing Module"},{"PSCM","Power Steering Control Module"},
 {"PAM","Parking Aid Module"},{"CCM","Cruise/Collision Control Module"},{"GFM","Generic Function Module"},
 {"GFM2","Generic Function Module 2"},{"GPSM","Global Positioning System Module"},{"HUD","Head-Up Display"},
 {"HSWM","Heated Steering Wheel Module"},{"RTM","Radio Transceiver Module"},{"RBM","Running Board Module"},
 {"HVAC","Heating Ventilation and Air Conditioning Module"}
};
#define HSCAN FORDLINK_NETWORK_MASK(FORDLINK_NETWORK_HSCAN)
#define MSCAN FORDLINK_NETWORK_MASK(FORDLINK_NETWORK_MSCAN)
static const FordlinkDiagnosticEndpoint endpoints[] = {
 {"PCM",0x7E0U,0x7E8U,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"Ford CAN/UDS platforms"},
 {"TCM",0x7E1U,0x7E9U,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"Ford CAN/UDS platforms"},
 {"ABS",0x760U,0x768U,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"service information + public implementations"},
 {"BCM",0x726U,0x72EU,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"platform/network varies"},
 {"IPC",0x720U,0x728U,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"service information + public implementations"},
 {"APIM",0x7D0U,0x7D8U,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"SYNC-equipped platforms"},
 {"GWM",0x716U,0x71EU,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"gateway-equipped platforms"},
 {"IPMA",0x706U,0x70EU,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"camera-equipped platforms"},
 {"RCM",0x737U,0x73FU,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"service information + public implementations"},
 {"DDM",0x740U,0x748U,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"platform/network varies"},
 {"FCIM",0x7A7U,0x7AFU,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"equipped platforms"},
 {"PSCM",0x730U,0x738U,false,HSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"EPAS-equipped platforms"},
 {"PAM",0x736U,0x73EU,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_CORROBORATED,true,"parking-aid equipped platforms"},
 {"DSM",0x744U,0x74CU,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_CORROBORATED,false,"seat-module equipped platforms"},
 {"HVAC",0x733U,0x73BU,false,MSCAN|HSCAN,FORDLINK_ENDPOINT_CANDIDATE,false,"platform/network varies"},
 {"GFM2",0x7D2U,0x7DAU,false,HSCAN,FORDLINK_ENDPOINT_CANDIDATE,false,"equipped platforms"},
 {"PDM",0x741U,0x749U,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"service-manual family"},
 {"PDM",0x742U,0x74AU,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_CANDIDATE,false,"alternate platform family"},
 {"ACM",0x727U,0x72FU,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"service-manual family"},
 {"ACM",0x754U,0x75CU,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_CANDIDATE,false,"alternate platform family"},
 {"FCDIM",0x7A5U,0x7ADU,false,HSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"},
 {"GFM",0x7A1U,0x7A9U,false,HSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"},
 {"GPSM",0x701U,0x709U,false,HSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"},
 {"HUD",0x7B2U,0x7BAU,false,HSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"},
 {"HSWM",0x714U,0x71CU,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"},
 {"IPMB",0x7B1U,0x7B9U,false,HSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"},
 {"RTM",0x751U,0x759U,false,HSCAN|MSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"},
 {"RBM",0x766U,0x76EU,false,HSCAN,FORDLINK_ENDPOINT_SERVICE_DOCUMENTED,false,"equipped platforms"}
};
size_t fordlink_module_definition_count(void){return sizeof(modules)/sizeof(modules[0]);}
const FordlinkModuleDefinition *fordlink_module_definition_at(size_t i){return i<fordlink_module_definition_count()?&modules[i]:NULL;}
const FordlinkModuleDefinition *fordlink_module_definition(const char *key){
 size_t i;if(key==NULL)return NULL;for(i=0U;i<fordlink_module_definition_count();++i)if(strcmp(modules[i].key,key)==0)return &modules[i];return NULL;}
size_t fordlink_diagnostic_endpoint_count(void){return sizeof(endpoints)/sizeof(endpoints[0]);}
const FordlinkDiagnosticEndpoint *fordlink_diagnostic_endpoint_at(size_t i){return i<fordlink_diagnostic_endpoint_count()?&endpoints[i]:NULL;}
const FordlinkDiagnosticEndpoint *fordlink_diagnostic_endpoint_for_module(const char *key,size_t occurrence){
 size_t i,found=0U;if(key==NULL)return NULL;for(i=0U;i<fordlink_diagnostic_endpoint_count();++i){if(strcmp(endpoints[i].module_key,key)!=0)continue;if(found++==occurrence)return &endpoints[i];}return NULL;}
bool fordlink_endpoint_supports_network(const FordlinkDiagnosticEndpoint *e,FordlinkNetworkKind n){
 if(e==NULL||n<=FORDLINK_NETWORK_UNKNOWN||n>=FORDLINK_NETWORK_COUNT)return false;return (e->network_candidates&FORDLINK_NETWORK_MASK(n))!=0U;}
const char *fordlink_endpoint_confidence_name(FordlinkEndpointConfidence v){
 switch(v){case FORDLINK_ENDPOINT_CANDIDATE:return "candidate";case FORDLINK_ENDPOINT_SERVICE_DOCUMENTED:return "service-documented";case FORDLINK_ENDPOINT_CORROBORATED:return "corroborated";}return "unknown";}
