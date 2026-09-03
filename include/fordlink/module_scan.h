// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_MODULE_SCAN_H
#define FORDLINK_MODULE_SCAN_H
#include "fordlink/identity.h"
#include "link/elm327.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define FORDLINK_MODULE_SCAN_MAX_RESULTS 32U
#define FORDLINK_MODULE_SCAN_TEXT 96U
typedef struct FordlinkModuleScanResult{
 const FordlinkDiagnosticEndpoint *endpoint;bool responded;bool identity_available;size_t dtc_count;
 char vin[FORDLINK_MODULE_SCAN_TEXT];char part_number[FORDLINK_MODULE_SCAN_TEXT];char software_number[FORDLINK_MODULE_SCAN_TEXT];
 char serial_number[FORDLINK_MODULE_SCAN_TEXT];char hardware_number[FORDLINK_MODULE_SCAN_TEXT];char system_name[FORDLINK_MODULE_SCAN_TEXT];
} FordlinkModuleScanResult;
typedef struct FordlinkModuleScanner{
 LinkEcuProbe probe;LinkEcuProbeDidRequest requests[LINK_ECU_PROBE_MAX_DIDS];size_t endpoint_index;bool probe_active;bool complete;bool progress_changed;
 FordlinkModuleScanResult results[FORDLINK_MODULE_SCAN_MAX_RESULTS];size_t result_count;size_t responsive_count;
} FordlinkModuleScanner;
bool fordlink_module_scanner_begin(FordlinkModuleScanner *scanner);
bool fordlink_module_scanner_next_command(FordlinkModuleScanner *scanner,char *buffer,size_t buffer_size,size_t *written,uint64_t *timeout_ms);
bool fordlink_module_scanner_accept(FordlinkModuleScanner *scanner,const LinkElm327Response *response,bool *complete);
bool fordlink_module_scanner_take_progress_changed(FordlinkModuleScanner *scanner);
size_t fordlink_module_scanner_result_count(const FordlinkModuleScanner *scanner);
size_t fordlink_module_scanner_responsive_count(const FordlinkModuleScanner *scanner);
const FordlinkModuleScanResult *fordlink_module_scanner_result_at(const FordlinkModuleScanner *scanner,size_t index);
int fordlink_module_scan_format_result(const FordlinkModuleScanResult *result,char *buffer,size_t buffer_size);
#ifdef __cplusplus
}
#endif
#endif
