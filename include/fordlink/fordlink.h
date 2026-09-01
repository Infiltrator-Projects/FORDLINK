// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FORDLINK_H
#define FORDLINK_H

#ifdef __cplusplus
extern "C" {
#endif

#define FORDLINK_VERSION "0.1.1"
#define FORDLINK_BRAND "Ford"

const char *fordlink_product_name(void);
const char *fordlink_brand_name(void);
const char *fordlink_version(void);

#ifdef __cplusplus
}
#endif

#endif
