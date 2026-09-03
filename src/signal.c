// SPDX-License-Identifier: GPL-3.0-or-later
#include "fordlink/signal.h"
#include <stdint.h>
#include <string.h>
bool fordlink_signal_definition_is_valid(const FordlinkSignalDefinition *d){
 unsigned int bits;if(d==NULL||d->key==NULL||d->name==NULL||d->module_key==NULL||d->unit==NULL||d->byte_length==0U||d->byte_length>8U||d->bit_length==0U||d->bit_length>64U)return false;
 bits=(unsigned int)d->byte_length*8U;if((unsigned int)d->bit_offset+(unsigned int)d->bit_length>bits)return false;
 return d->view_flags!=0U&&(d->view_flags&~(FORDLINK_SIGNAL_VIEW_TABLE|FORDLINK_SIGNAL_VIEW_DASHBOARD|FORDLINK_SIGNAL_VIEW_GRAPH))==0U;}
static uint64_t raw_value(const FordlinkSignalDefinition *d,const uint8_t *data){uint64_t v=0U;size_t i;if(d->endian==FORDLINK_SIGNAL_BIG_ENDIAN){for(i=0U;i<d->byte_length;++i)v=(v<<8U)|data[d->byte_offset+i];}
 else for(i=0U;i<d->byte_length;++i)v|=(uint64_t)data[d->byte_offset+i]<<(8U*i);return v;}
bool fordlink_signal_decode(const FordlinkSignalDefinition *d,const uint8_t *data,size_t n,FordlinkSignalSample *s){
 uint64_t raw,mask,field;int64_t signed_field;if(!fordlink_signal_definition_is_valid(d)||data==NULL||s==NULL||d->byte_offset>n||d->byte_length>n-d->byte_offset)return false;
 raw=raw_value(d,data);mask=d->bit_length==64U?UINT64_MAX:((UINT64_C(1)<<d->bit_length)-UINT64_C(1));field=(raw>>d->bit_offset)&mask;signed_field=(int64_t)field;
 if(d->signed_value&&d->bit_length<64U&&(field&(UINT64_C(1)<<(d->bit_length-1U)))!=0U)signed_field=(int64_t)(field|~mask);
 memset(s,0,sizeof(*s));s->raw_unsigned=field;s->raw_signed=signed_field;s->value=(d->signed_value?(double)signed_field:(double)field)*d->factor+d->additive_offset;return true;}
