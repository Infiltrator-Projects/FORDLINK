// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/signal.h"
#include <math.h>
#include <stdio.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"FAIL: %s\n",#x);return 1;}}while(0)
int main(void){const uint8_t data[]={0x01U,0x90U,0xF0U};FordlinkSignalSample s;FordlinkSignalDefinition d={"temp","Temperature","PCM",FORDLINK_SIGNAL_UDS_DID,0x1234U,0U,2U,0U,16U,FORDLINK_SIGNAL_BIG_ENDIAN,false,0.1,-40.0,"degC",-40.0,215.0,1U,FORDLINK_SIGNAL_VIEW_TABLE|FORDLINK_SIGNAL_VIEW_DASHBOARD|FORDLINK_SIGNAL_VIEW_GRAPH};
CHECK(fordlink_signal_decode(&d,data,sizeof(data),&s));CHECK(s.raw_unsigned==400U);CHECK(fabs(s.value)<0.0001);d.byte_offset=2U;d.byte_length=1U;d.bit_offset=4U;d.bit_length=4U;d.factor=1.0;d.additive_offset=0.0;d.signed_value=true;CHECK(fordlink_signal_decode(&d,data,sizeof(data),&s));CHECK(s.raw_signed==-1);puts("FORDLINK signal decoder passed");return 0;}
