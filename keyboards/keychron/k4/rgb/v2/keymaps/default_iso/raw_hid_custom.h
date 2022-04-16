#pragma once

#include QMK_KEYBOARD_H
#include "raw_hid.h"

void sendKeyCodeOverRawHid(uint16_t keycode, keyrecord_t *record);
