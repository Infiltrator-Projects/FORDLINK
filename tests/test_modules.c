// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/module.h"
#include <stdio.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"FAIL: %s\n",#x);return 1;}}while(0)
int main(void){const FordlinkDiagnosticEndpoint *pcm,*bcm,*p0,*p1;CHECK(fordlink_module_definition_count()>=25U);CHECK(fordlink_diagnostic_endpoint_count()>=20U);CHECK(fordlink_module_definition("PCM")!=NULL);
pcm=fordlink_diagnostic_endpoint_for_module("PCM",0U);CHECK(pcm!=NULL&&pcm->request_id==0x7E0U&&pcm->response_id==0x7E8U&&pcm->scan_hscan_by_default);
bcm=fordlink_diagnostic_endpoint_for_module("BCM",0U);CHECK(bcm!=NULL&&fordlink_endpoint_supports_network(bcm,FORDLINK_NETWORK_HSCAN)&&fordlink_endpoint_supports_network(bcm,FORDLINK_NETWORK_MSCAN));
p0=fordlink_diagnostic_endpoint_for_module("PDM",0U);p1=fordlink_diagnostic_endpoint_for_module("PDM",1U);CHECK(p0!=NULL&&p1!=NULL&&p0->request_id!=p1->request_id);puts("FORDLINK module catalogue passed");return 0;}
