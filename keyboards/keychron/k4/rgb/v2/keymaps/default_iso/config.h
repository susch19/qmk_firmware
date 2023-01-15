/* USB Device descriptor parameter */

#pragma once

#define VENDOR_ID       0x3434
#define PRODUCT_ID      0xFE07
#define DEVICE_VER		0x0001

#define KEYMAP_ISO 1

#define WEAR_LEVELING_SN32_EMULATION_PAGE_COUNT 39
// #define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 1536
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 1000
#define VIA_QMK_RGBLIGHT_ENABLE


#define LAYER_STATE_8BIT
#define DYNAMIC_KEYMAP_LAYER_COUNT 3
#include "config_common.h"

// #define VIA_QMK_RGBLIGHT_ENABLE
