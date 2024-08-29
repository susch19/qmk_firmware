#pragma once

#include QMK_KEYBOARD_H
#include "raw_hid.h"

void sendKeyCodeOverRawHid(uint16_t keycode, keyrecord_t *record);
void test_raw_send(uint8_t rsp, uint8_t* data, uint8_t len, uint8_t sn);
