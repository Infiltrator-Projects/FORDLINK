// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_SIGNAL_H
#define FORDLINK_SIGNAL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum FordlinkSignalSource{FORDLINK_SIGNAL_UDS_DID=0,FORDLINK_SIGNAL_CAN_FRAME}FordlinkSignalSource;
typedef enum FordlinkSignalEndian{FORDLINK_SIGNAL_BIG_ENDIAN=0,FORDLINK_SIGNAL_LITTLE_ENDIAN}FordlinkSignalEndian;
enum{FORDLINK_SIGNAL_VIEW_TABLE=1U<<0,FORDLINK_SIGNAL_VIEW_DASHBOARD=1U<<1,FORDLINK_SIGNAL_VIEW_GRAPH=1U<<2};
typedef struct FordlinkSignalDefinition{
 const char *key,*name,*module_key;FordlinkSignalSource source;uint32_t identifier;size_t byte_offset;uint8_t byte_length,bit_offset,bit_length;
 FordlinkSignalEndian endian;bool signed_value;double factor,additive_offset;const char *unit;double display_min,display_max;uint8_t precision,view_flags;
}FordlinkSignalDefinition;
typedef struct FordlinkSignalSample{int64_t raw_signed;uint64_t raw_unsigned;double value;}FordlinkSignalSample;
bool fordlink_signal_definition_is_valid(const FordlinkSignalDefinition *definition);
bool fordlink_signal_decode(const FordlinkSignalDefinition *definition,const uint8_t *data,size_t data_length,FordlinkSignalSample *sample);
#ifdef __cplusplus
}
#endif
#endif
