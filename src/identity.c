// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/identity.h"
#include <string.h>
static const FordlinkIdentityDid dids[]={
 {0xF180U,"boot_software_id","Boot software identification",false},{0xF181U,"application_software_id","Application software identification",false},
 {0xF182U,"application_data_id","Application data identification",false},{0xF183U,"boot_fingerprint","Boot software fingerprint",false},
 {0xF184U,"application_fingerprint","Application software fingerprint",false},{0xF185U,"application_data_fingerprint","Application data fingerprint",false},
 {0xF186U,"active_session","Active diagnostic session",false},{0xF187U,"spare_part_number","Manufacturer spare part number",true},
 {0xF188U,"ecu_software_number","Manufacturer ECU software number",true},{0xF189U,"ecu_software_version","Manufacturer ECU software version",false},
 {0xF18AU,"system_supplier","System supplier identifier",false},{0xF18BU,"manufacturing_date","ECU manufacturing date",false},
 {0xF18CU,"serial_number","ECU serial number",true},{0xF18DU,"supported_function_units","Supported functional units",false},
 {0xF18EU,"kit_assembly_part_number","Manufacturer kit assembly part number",false},{0xF190U,"vin","Vehicle identification number",true},
 {0xF191U,"ecu_hardware_number","Manufacturer ECU hardware number",true},{0xF194U,"supplier_software_number","Supplier ECU software number",false},
 {0xF195U,"supplier_software_version","Supplier ECU software version",false},{0xF197U,"system_name","System name / engine type",true},
 {0xF199U,"programming_date","Programming date",false},{0xF19DU,"installation_date","ECU installation date",false},
 {0xF19EU,"odx_file","ODX file identifier",false}};
size_t fordlink_identity_did_count(void){return sizeof(dids)/sizeof(dids[0]);}
const FordlinkIdentityDid *fordlink_identity_did_at(size_t i){return i<fordlink_identity_did_count()?&dids[i]:NULL;}
const FordlinkIdentityDid *fordlink_identity_did(uint16_t did){size_t i;for(i=0U;i<fordlink_identity_did_count();++i)if(dids[i].did==did)return &dids[i];return NULL;}
size_t fordlink_identity_basic_requests(LinkEcuProbeDidRequest *r,size_t cap){
 size_t i,w=0U;if(r==NULL&&cap!=0U)return 0U;for(i=0U;i<fordlink_identity_did_count();++i){if(!dids[i].basic_scan)continue;if(w>=cap)return 0U;r[w].did=dids[i].did;r[w].key=dids[i].key;r[w].name=dids[i].name;++w;}return w;}
bool fordlink_identity_probe_profile(const FordlinkDiagnosticEndpoint *e,LinkEcuProbeDidRequest *r,size_t cap,LinkEcuProbeProfile *p){
 size_t n;if(e==NULL||r==NULL||p==NULL)return false;n=fordlink_identity_basic_requests(r,cap);if(n==0U)return false;memset(p,0,sizeof(*p));
 p->channel.tx_can_id=e->request_id;p->channel.rx_can_id=e->response_id;p->channel.extended_id=e->extended_id;p->dids=r;p->did_count=n;p->tester_present=false;p->read_dtcs=true;
 return link_ecu_probe_profile_is_valid(p);}
