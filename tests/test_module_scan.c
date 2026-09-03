// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/module_scan.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"FAIL: %s\n",#x);return 1;}}while(0)
static LinkElm327Response ok(void){LinkElm327Response r;memset(&r,0,sizeof(r));r.result=LINK_ELM327_RESULT_OK;r.ok_seen=true;return r;}
static LinkElm327Response nodata(void){LinkElm327Response r;memset(&r,0,sizeof(r));r.result=LINK_ELM327_RESULT_NO_DATA;return r;}
static LinkElm327Response vin(void){LinkElm327Response r;memset(&r,0,sizeof(r));r.result=LINK_ELM327_RESULT_OK;strcpy(r.text,"62F190314641464F52444C494E4B544553543031");r.length=strlen(r.text);r.line_count=1U;return r;}
int main(void){FordlinkModuleScanner s;bool done=false,injected=false;unsigned int guard=0U;CHECK(fordlink_module_scanner_begin(&s));while(!done&&guard++<500U){char cmd[64];size_t w=0U;uint64_t timeout=0U;LinkElm327Response r;CHECK(fordlink_module_scanner_next_command(&s,cmd,sizeof(cmd),&w,&timeout));if(strncmp(cmd,"AT",2U)==0)r=ok();else if(!injected&&s.probe.profile.channel.tx_can_id==0x7E0U&&strcmp(cmd,"22F190")==0){r=vin();injected=true;}else r=nodata();CHECK(fordlink_module_scanner_accept(&s,&r,&done));}CHECK(done&&injected);CHECK(fordlink_module_scanner_result_count(&s)>5U);CHECK(fordlink_module_scanner_responsive_count(&s)==1U);{const FordlinkModuleScanResult *r=fordlink_module_scanner_result_at(&s,0U);CHECK(r!=NULL&&r->endpoint!=NULL&&strcmp(r->endpoint->module_key,"PCM")==0&&r->responded);CHECK(strncmp(r->vin,"1FAFORDLINKTEST01",15U)==0);}puts("FORDLINK read-only module scanner passed");return 0;}
