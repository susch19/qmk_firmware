#pragma once

#include QMK_KEYBOARD_H

void perform_custom_space_cadet(keyrecord_t *record, uint16_t sc_keycode, uint8_t holdMod, uint16_t holdKey, uint8_t tapMod, uint16_t keycode);
bool process_custom_space_cadet(uint16_t keycode, keyrecord_t *record);
