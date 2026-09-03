// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/module_scan.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
static bool default_hscan(const FordlinkDiagnosticEndpoint *e){return e!=NULL&&e->scan_hscan_by_default&&fordlink_endpoint_supports_network(e,FORDLINK_NETWORK_HSCAN);}
static void bytes_text(const uint8_t *d,size_t n,char *b,size_t cap){
 size_t i;bool printable=n!=0U;if(b==NULL||cap==0U)return;b[0]='\0';if(d==NULL||n==0U)return;
 for(i=0U;i<n;++i){if(d[i]==0U)continue;if(!isprint((unsigned char)d[i])){printable=false;break;}}
 if(printable){size_t o=0U;for(i=0U;i<n&&o+1U<cap;++i){if(d[i]!=0U)b[o++]=(char)d[i];}while(o!=0U&&isspace((unsigned char)b[o-1U]))--o;b[o]='\0';return;}
 {size_t o=0U;for(i=0U;i<n&&o+3U<cap;++i){int q=snprintf(b+o,cap-o,"%02X",d[i]);if(q<=0)break;o+=(size_t)q;if(i+1U<n&&o+2U<cap)b[o++]=' ';}b[o<cap?o:cap-1U]='\0';}}
static void did_text(const LinkEcuProbe *p,uint16_t did,char *b,size_t cap){
 size_t i;if(b==NULL||cap==0U)return;b[0]='\0';for(i=0U;i<link_ecu_probe_did_result_count(p);++i){const LinkEcuProbeDidResult *v=link_ecu_probe_did_result_at(p,i);
 if(v!=NULL&&v->request!=NULL&&v->request->did==did&&v->status==LINK_ECU_PROBE_READ_AVAILABLE){bytes_text(v->data,v->data_length,b,cap);return;}}}
static bool responded(const LinkEcuProbe *p){size_t i;for(i=0U;i<link_ecu_probe_did_result_count(p);++i){const LinkEcuProbeDidResult *v=link_ecu_probe_did_result_at(p,i);
 if(v!=NULL&&(v->status==LINK_ECU_PROBE_READ_AVAILABLE||v->status==LINK_ECU_PROBE_READ_NEGATIVE_RESPONSE))return true;}
 return p->dtc_status==LINK_ECU_PROBE_READ_AVAILABLE||p->dtc_status==LINK_ECU_PROBE_READ_NEGATIVE_RESPONSE;}
static bool identity(const LinkEcuProbe *p){size_t i;for(i=0U;i<link_ecu_probe_did_result_count(p);++i){const LinkEcuProbeDidResult *v=link_ecu_probe_did_result_at(p,i);if(v!=NULL&&v->status==LINK_ECU_PROBE_READ_AVAILABLE)return true;}return false;}
static bool start_next(FordlinkModuleScanner *s){size_t n=fordlink_diagnostic_endpoint_count();while(s->endpoint_index<n){const FordlinkDiagnosticEndpoint *e=fordlink_diagnostic_endpoint_at(s->endpoint_index++);LinkEcuProbeProfile p;
 if(!default_hscan(e))continue;if(!fordlink_identity_probe_profile(e,s->requests,sizeof(s->requests)/sizeof(s->requests[0]),&p))return false;
 if(link_ecu_probe_begin(&s->probe,&p)!=LINK_ECU_PROBE_RESULT_OK)return false;s->probe_active=true;return true;}s->probe_active=false;s->complete=true;return true;}
static void capture(FordlinkModuleScanner *s){
 FordlinkModuleScanResult *r;const FordlinkDiagnosticEndpoint *e=NULL;bool seen;if(s==NULL||!s->probe_active)return;if(s->endpoint_index!=0U)e=fordlink_diagnostic_endpoint_at(s->endpoint_index-1U);
 if(e==NULL||s->result_count>=FORDLINK_MODULE_SCAN_MAX_RESULTS)return;r=&s->results[s->result_count++];memset(r,0,sizeof(*r));r->endpoint=e;seen=responded(&s->probe);r->responded=seen;r->identity_available=identity(&s->probe);
 if(s->probe.dtc_status==LINK_ECU_PROBE_READ_AVAILABLE)r->dtc_count=s->probe.dtcs.count;
 did_text(&s->probe,0xF190U,r->vin,sizeof(r->vin));did_text(&s->probe,0xF187U,r->part_number,sizeof(r->part_number));did_text(&s->probe,0xF188U,r->software_number,sizeof(r->software_number));
 did_text(&s->probe,0xF18CU,r->serial_number,sizeof(r->serial_number));did_text(&s->probe,0xF191U,r->hardware_number,sizeof(r->hardware_number));did_text(&s->probe,0xF197U,r->system_name,sizeof(r->system_name));
 if(seen){++s->responsive_count;s->progress_changed=true;}}
bool fordlink_module_scanner_begin(FordlinkModuleScanner *s){if(s==NULL)return false;memset(s,0,sizeof(*s));return start_next(s);}
bool fordlink_module_scanner_next_command(FordlinkModuleScanner *s,char *b,size_t cap,size_t *w,uint64_t *timeout){
 LinkEcuProbeResult q;if(w!=NULL)*w=0U;if(timeout!=NULL)*timeout=UINT64_C(2500);if(s==NULL||s->complete||!s->probe_active||b==NULL||cap==0U||w==NULL||timeout==NULL)return false;
 q=link_ecu_probe_command(&s->probe,b,cap,w);return q==LINK_ECU_PROBE_RESULT_OK&&*w!=0U;}
bool fordlink_module_scanner_accept(FordlinkModuleScanner *s,const LinkElm327Response *response,bool *complete){
 LinkEcuProbeResult q;if(complete!=NULL)*complete=false;if(s==NULL||response==NULL||complete==NULL||s->complete||!s->probe_active)return false;q=link_ecu_probe_accept(&s->probe,response);
 if(q==LINK_ECU_PROBE_RESULT_COMPLETE){capture(s);s->probe_active=false;if(!start_next(s))return false;*complete=s->complete;return true;}return q==LINK_ECU_PROBE_RESULT_OK;}
bool fordlink_module_scanner_take_progress_changed(FordlinkModuleScanner *s){bool c;if(s==NULL)return false;c=s->progress_changed;s->progress_changed=false;return c;}
size_t fordlink_module_scanner_result_count(const FordlinkModuleScanner *s){return s==NULL?0U:s->result_count;}
size_t fordlink_module_scanner_responsive_count(const FordlinkModuleScanner *s){return s==NULL?0U:s->responsive_count;}
const FordlinkModuleScanResult *fordlink_module_scanner_result_at(const FordlinkModuleScanner *s,size_t i){return s!=NULL&&i<s->result_count?&s->results[i]:NULL;}
int fordlink_module_scan_format_result(const FordlinkModuleScanResult *r,char *b,size_t cap){
 const FordlinkModuleDefinition *m;if(b==NULL||cap==0U||r==NULL||r->endpoint==NULL)return -1;m=fordlink_module_definition(r->endpoint->module_key);
 return snprintf(b,cap,"%s · %s · 0x%03X/0x%03X · DTCs %zu",r->endpoint->module_key,m!=NULL?m->name:"Ford module",(unsigned int)r->endpoint->request_id,(unsigned int)r->endpoint->response_id,r->dtc_count);}
