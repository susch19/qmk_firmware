/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "raw_hid_custom.h"
#include "keymap_german.h"
#include "keymap.h"
#include "sendstring_german.h"
#include "custom_space_cadet.h"
#include "raw_hid.h"
#include "print.h"
#include "commands.h"
#include "quantum/unicode/unicode.h"
#include "wireless.h"

#if defined(LED_MATRIX_ENABLE) || defined(RGB_MATRIX_ENABLE)
#    ifdef LED_MATRIX_ENABLE
#        include "led_matrix.h"
#    endif
#    ifdef RGB_MATRIX_ENABLE
#        include "rgb_matrix.h"
#    endif
#endif


extern bool      last_suspend_state;
extern uint32_t *instscval;
extern host_driver_t   wireless_driver;
bool             is_suspended     = false;
bool             via_mode         = false;
bool             raw_mode         = true;
bool             openrgb_mode     = false;
bool             autoshift_mode   = false;
bool             testMode         = false;
uint8_t          inputLanguage    = 0;
uint8_t          receivedPackages = 0;
uint8_t          layerMask        = 0;
uint8_t         rspTestVal = 1;
bool recRaw = false;
// long long _Accum  something      = 0.0k;

uint32_t lastLightLevel   = 0;
uint8_t  tempVariable     = 0;
uint16_t autoshiftTimeout = AUTO_SHIFT_TIMEOUT;

#define KC_EASH KC_EASYSHIFT
#define KC_EATB KC_EASYTAB
#define KC_EAHO KC_EASYHOME
#define KC_EAEN KC_EASYEND

#define MG_NKRO MAGIC_TOGGLE_NKRO
#define RGB_PCD MODE_PC_DRIVEN
#define RGB_NON MODE_EMPTY
#define CL_MODS CLEAR_MODS
#define SU_PCMA SU_PCMACRO

// clang-format off

    /* ISO-DE
    				+--------------------------------------------------------------------------+-------------------+
                    | ESC |   | F1 | F2 | F3 | F4 |  | F5 | F6 | F7 | F8 |  | F9 | F10 |F11|F12| DEL|HOME|END|KNOB|
                    +--------------------------------------------------------------------------+----|----|----|----+
                    |  ^  |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  ß |  ´ | BACKSP |NLCK|  / |  * |  - |
                    +--------------------------------------------------------------------------+----|----|----|----+
                    |  TAB  |  Q |  W |  E |  R |  T |  Z |  U |  I |  O |  P |  Ü |  + |      |  7 |  8 |  9 |    |
                    +--------------------------------------------------------------------  RET +----|----|----|  + +
                    | CAPSLCK |  A |  S |  D |  F |  G |  H |  J |  K |  L |  ö |  Ä | # |     |  4 |  5 |  6 |    |
                    +--------------------------------------------------------------------------+----|----|----|----+
                    |LSHFT|  < |  Y |  X |  C |  V |  B |  N |  M |  , |  . |  - | RSHIFT | UP |  1 |  2 |  3 |    |
                    +--------------------------------------------------------------------------+----|----|----| ENT+
                    |LCTRL| LGUI|  LALT|            SPACE            | RALT| FN|RCTRL| LFT| DWN| RGT|  0 |  . |    |
                    +--------------------------------------------------------------------------+-------------------+
    */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_iso_98(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   KC_EAHO,  KC_EAEN,     KC_MUTE,
        DE_CIRC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     DE_SS,      DE_ACUT,  KC_BSPC,            KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_EATB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     DE_Z,     KC_U,     KC_I,     KC_O,     KC_P,     DE_UDIA,    DE_PLUS,                      KC_P7,    KC_P8,    KC_P9,      KC_PPLS,
        KC_EASH,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     DE_ODIA,  DE_ADIA,    DE_HASH,  KC_ENT,             KC_P4,    KC_P5,    KC_P6,
        SC_LSPO,  DE_LABK,  DE_Y,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   DE_MINS,              SC_RSPC,  KC_UP,    KC_P1,    KC_P2,    KC_P3,      KC_PENT,
        SC_LCPO,  KC_LWIN,  SC_LAPO,                                KC_SPC,                                 SC_RAPC,  MO(_FL),    SC_RCPC,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,    KC_PDOT            ),
    [_FL] = LAYOUT_iso_98(
        QK_BOOT,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            _______,  _______,  _______,    RGB_TOG,
        RGB_PCD,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,    _______,
     CLEAR_MODS,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  DE_MICR,  _______,  _______,  KC_PSCR,  _______,    _______,                      _______,  _______,  _______,    _______,
        KC_CAPS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  MYVIA,    BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  KC_PGUP,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,                                MYRAW,                                  RCS(KC_M),_______,  RCS(KC_U),  KC_HOME,  KC_PGDN,  KC_END,   _______,  _______            ),
    [_D1] = LAYOUT_iso_98(
        _______,            KC_F13,   KC_F14,   KC_F15,   KC_F16,   KC_F17,   KC_F18,   KC_F19,   KC_F20,   KC_F21,   KC_F22,     KC_F23,   KC_F24,            _______,  _______,  _______,    RGB_NON,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,    _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,                      _______,  _______,  _______,    _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            K_TEST4,  K_TEST5,  K_TEST6,
        _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,  K_TEST1,  K_TEST2,  K_TEST3,    _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______            ),
    [_D2] = LAYOUT_iso_98(
    _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,                      _______,  _______,  _______,    _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,  _______,  _______,  _______,    _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______            ),

    [_KEEKC] = LAYOUT_iso_98(
        KEEKC_ESC,                KEEKC_F1,    KEEKC_F2,    KEEKC_F3,    KEEKC_F4,    KEEKC_F5,    KEEKC_F6,    KEEKC_F7,    KEEKC_F8,    KEEKC_F9,    KEEKC_F10,     KEEKC_F11,   KEEKC_F12,                KEEKC_DEL,   KEEKC_HOME,  KEEKC_END,     KEEKC_KNOBP,
        KEEKC_GRV,   KEEKC_1,     KEEKC_2,     KEEKC_3,     KEEKC_4,     KEEKC_5,     KEEKC_6,     KEEKC_7,     KEEKC_8,     KEEKC_9,     KEEKC_0,     KEEKC_MINS,    KEEKC_EQL,   KEEKC_BSPC,               KEEKC_NLCK,  KEEKC_PSLS,  KEEKC_PAST,    KEEKC_PMNS,
        KC_EATB,     KEEKC_Q,     KEEKC_W,     KEEKC_E,     KEEKC_R,     KEEKC_T,     KEEKC_Y,     KEEKC_U,     KEEKC_I,     KEEKC_O,     KEEKC_P,     KEEKC_LBRC,    KEEKC_RBRC,                            KEEKC_P7,    KEEKC_P8,    KEEKC_P9,      KEEKC_PPLS,
        KEEKC_CAPS,  KEEKC_A,     KEEKC_S,     KEEKC_D,     KEEKC_F,     KEEKC_G,     KEEKC_H,     KEEKC_J,     KEEKC_K,     KEEKC_L,     KEEKC_SCLN,  KEEKC_QUOT,    KEEKC_NUHS,  KEEKC_ENT,                KEEKC_P4,    KEEKC_P5,    KEEKC_P6,
        KEEKC_LSFT,  KEEKC_NUBS,  KEEKC_Z,     KEEKC_X,     KEEKC_C,     KEEKC_V,     KEEKC_B,     KEEKC_N,     KEEKC_M,     KEEKC_COMM,  KEEKC_DOT,   KEEKC_SLSH,                 KEEKC_RSFT,  KEEKC_UP,    KEEKC_P1,    KEEKC_P2,    KEEKC_P3,      KEEKC_PENT,
        KEEKC_LCTL,  KEEKC_LGUI,  KEEKC_LALT,                                         KEEKC_SPC,                                          KEEKC_RALT,  KEEKC_FN,      KEEKC_RCTL,  KEEKC_LEFT,  KEEKC_DOWN,  KEEKC_RGHT,  KEEKC_P0,    KEEKC_NUMCOL            ),
    [_EN] = LAYOUT_iso_98(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            _______,  _______,  _______,    RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,    _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,                      _______,  _______,  _______,    _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______            ),
    [_MYCKC] = LAYOUT_iso_98(
        MYCKC_ESC,                MYCKC_F1,    MYCKC_F2,     MYCKC_F3,    MYCKC_F4,    MYCKC_F5,    MYCKC_F6,    MYCKC_F7,    MYCKC_F8,    MYCKC_F9,    MYCKC_F10,     MYCKC_F11,   MYCKC_F12,                MYCKC_DEL,   MYCKC_HOME,  MYCKC_END,     MYCKC_KNOBP,
        MYCKC_GRV,   MYCKC_1,     MYCKC_2,     MYCKC_3,  MYCKC_4,     MYCKC_5,     MYCKC_6,     MYCKC_7,     MYCKC_8,     MYCKC_9,     MYCKC_0,     MYCKC_MINS,    MYCKC_EQL,   MYCKC_BSPC,               MYCKC_NLCK,  MYCKC_PSLS,  MYCKC_PAST,    MYCKC_PMNS,
        MYCKC_TAB,   KC_HOME,     KC_UP,       KC_END,   MYCKC_R,     MYCKC_T,     MYCKC_Y,     MYCKC_U,     MYCKC_I,     MYCKC_O,     MYCKC_P,     MYCKC_LBRC,    MYCKC_RBRC,                            MYCKC_P7,    MYCKC_P8,    MYCKC_P9,      MYCKC_PPLS,
        MYCKC_CAPS,  KC_LEFT,     KC_DOWN,     KC_RIGHT, MYCKC_F,     MYCKC_G,     MYCKC_H,     MYCKC_J,     MYCKC_K,     MYCKC_L,     MYCKC_SCLN,  MYCKC_QUOT,    MYCKC_NUHS,  MYCKC_ENT,                MYCKC_P4,    MYCKC_P5,    MYCKC_P6,
        KC_LSFT,  MYCKC_NUBS,  MYCKC_Z,     MYCKC_X,  MYCKC_C,     MYCKC_V,     MYCKC_B,     MYCKC_N,     MYCKC_M,     MYCKC_COMM,  MYCKC_DOT,   MYCKC_SLSH,                 MYCKC_RSFT,  MYCKC_UP,    MYCKC_P1,    MYCKC_P2,    MYCKC_P3,      MYCKC_PENT,
        KC_LCTL,  MYCKC_LGUI,  MYCKC_LALT,                                         KC_SPC,                                          MYCKC_RALT,  MYCKC_FN,      MYCKC_RCTL,  MYCKC_LEFT,  MYCKC_DOWN,  MYCKC_RGHT,  MYCKC_P0,    MYCKC_NUMCOL            ),
 };

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_FL] = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [_D1] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_D2] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_MYCKC] = {ENCODER_CCW_CW(MYCKC_KNOBL, MYCKC_KNOBR)},
    [_EN] = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [_MYCKC] = {ENCODER_CCW_CW(MYCKC_KNOBL, MYCKC_KNOBR)},
};
#endif // ENCODER_MAP_ENABLE

__attribute__((weak)) void raw_hid_receive_keychron(uint8_t *data, uint8_t length) {
    return;
}

// clang-format on

bool dip_switch_update_kb(uint8_t index, bool active) {

    switch (index) {
        case 0:
            if (active) { // BT mode

            } else { // Cable mode
                // do stuff
            }
            break;
        case 1:
            if (active) { // Mac/iOS mode
                layer_on(_MYCKC);
            } else { // Win/Android mode
                layer_off(_MYCKC);
            }
            break;
    }

    dip_switch_update_user(index, active);
    return true;
}


void interpret_pc_macro(uint8_t *data, uint8_t length) {
    for (size_t i = 0; i < length; i++) {
        if (data[i] == KC_NO) break;

        tap_code(data[i]);
    }
}

void interpret_pc_macro_16_bit(uint8_t *data, uint8_t length) {
    for (size_t i = 0; i + 1 < length; i += 2) {
        if (data[i] == KC_NO) break;

        tap_code16(((uint16_t)data[i] << 8) | data[i + 1]);
    }
}

int setColorsFor(uint8_t *data, uint8_t dataLength, uint8_t index) {
    int ret = 0;
    for (size_t i = 0; i < dataLength; i++) {
        uint32_t newVal = data[i * 3] | (data[i * 3 + 1] << 8) | (data[i * 3 + 2] << 16);
        if (newVal != (lastLightLevel + 1)) {
            ret = newVal - (lastLightLevel + 1);
        }
        lastLightLevel = newVal;
        rgb_matrix_set_color((index + i) % 101, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
    return ret;
}

rgb_header getRGBHeader(uint8_t *data) {
    rgb_header header     = {};
    uint16_t   headerData = ((uint16_t)data[0] << 8) | data[1];
    header.mode           = (headerData >> 14) & 0b11;
    header.count          = (headerData >> 9) & 0b11111;
    header.rgb            = (headerData >> 7) & 0b11;
    header.index          = (headerData)&0b1111111;

    return header;
}

command_header getCommandHeader(uint8_t *data) {
    command_header header     = {};
    uint16_t       headerData = ((uint16_t)data[0] << 8) | data[1];
    header.mode               = (headerData >> 14) & 0b11;
    header.categorie          = (headerData >> 6) & 0xff;
    header.reserved           = (headerData)&0b111111;

    return header;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    layerMask = IS_LAYER_ON_STATE(state, 1);
    layerMask |= IS_LAYER_ON_STATE(state, 2) << 1;
    layerMask |= IS_LAYER_ON_STATE(state, 3) << 2;
    layerMask |= IS_LAYER_ON_STATE(state, 4) << 3;
    layerMask |= IS_LAYER_ON_STATE(state, 5) << 4;
    layerMask |= IS_LAYER_ON_STATE(state, 6) << 5;
    return state;
}


bool rgb_matrix_indicators_user() {
    bool custom = false;
    if (via_mode) {
        rgb_matrix_set_color(42, 255, 255, 0);
        rgb_matrix_set_color(52, 255, 255, 0);
        rgb_matrix_set_color(73, 255, 255, 0);
        custom = true;
    }
    if(!raw_mode){
        rgb_matrix_set_color(89, 255, 255, 255);
    }
    if (inputLanguage > 0) {
        rgb_matrix_set_color(37, 255, 0, 255); // E
        rgb_matrix_set_color(75, 255, 0, 255); // N
        rgb_matrix_set_color(56, 255, 0, 255); // G
        rgb_matrix_set_color(60, 255, 0, 255); // L
        rgb_matrix_set_color(42, 255, 0, 255); // I
        rgb_matrix_set_color(53, 255, 0, 255); // S
        rgb_matrix_set_color(57, 255, 0, 255); // H
        custom = true;
    }
    if (layerMask > 0) {
        if (layerMask & 0b1) rgb_matrix_set_color(17, 255, 0, 255);    // 1
        if (layerMask & 0b10) rgb_matrix_set_color(18, 255, 0, 255);   // 2
        if (layerMask & 0b100) rgb_matrix_set_color(19, 255, 0, 255);  // 3
        if (layerMask & 0b1000) rgb_matrix_set_color(20, 255, 0, 255); // 4
        if (layerMask & 0b10000) rgb_matrix_set_color(21, 255, 0, 255); // 5
        if (layerMask & 0b100000) rgb_matrix_set_color(22, 255, 0, 255); // 6
        custom = true;
    }
    if (tempVariable > 0) {
        rgb_matrix_set_color(tempVariable, 0, 255, 255);
        custom = true;
    }
    if(recRaw){
        for (int i = 0;i < 5; i++)
            rgb_matrix_set_color(i, 255, 0, 255);
    }
    if(testMode){
        for (int i = 5;i < 9; i++)
            rgb_matrix_set_color(i, 255, 0, 0);

    }
    if(!host_keyboard_led_state().num_lock && !is_suspended){
        rgb_matrix_set_color(NUM_LOCK_INDEX, 255, 255, 255);
    }

    return custom;
}
void wireless_enter_sleep_kb(){
    rgb_matrix_set_color_all(0, 0, 0);
}

int process_raw_message(uint8_t *data, uint8_t length) {
    command_header header = getCommandHeader(data);
    switch (header.mode) {
        case RGB_COMMAND: {
            rgb_header rgbheader = getRGBHeader(data);

            if (rgbheader.rgb == 0 && (rgbheader.count < 20 || rgbheader.count > 21)) {
            } else
                switch (rgbheader.rgb) {
                    case IndexItereationRGBZero:
                    case Reserved: { // Special case, only first byte is used for header, because index gets stripped, so data starts at index 1!
                        return setColorsFor(&data[1], rgbheader.count, 0);
                        break;
                    }
                    case PerKeyRGB:
                        for (size_t i = 0; i < rgbheader.count; i++) {
                            rgb_matrix_set_color(data[i * 4 + 2], data[i * 4 + 3], data[i * 4 + 4], data[i * 4 + 5]); // First 2 bytes always used
                        }
                        break;
                    case IndexItereationRGB: {
                        return setColorsFor(&data[2], rgbheader.count, rgbheader.index);
                        break;
                    }
                }
            break;
        }
        case LAYER_COMMAND: {
            switch (header.categorie) {
                case SwitchToLayer:
                    layer_invert(data[2]);
                    break;
                case ResetLayer:
                    layer_move(data[2]);
                    tempVariable = data[2];
                    break;
                case ChangeLanguage:
                    inputLanguage = data[2];
                    break;
            }

            break;
        }
        // case MACRO_COMMAND: {
        //     switch (header.categorie) {
        //         case PCMacro:
        //             interpret_pc_macro(&data[2], dataLength);
        //             break;
        //         case PCMacro16Bit:
        //             interpret_pc_macro_16_bit(&data[2], dataLength);
        //             break;
        //         case PCMacroString:
        //             send_string((char *)&data[2]);
        //             break;
        //     }
        //     break;
        // }
        case OTHER_COMMAND: {
            switch (header.categorie) {
                case MULTI: {
                    uint8_t index = 2;
                    for (;;) {
                        uint8_t newLen = data[index++];
                        if (newLen == 0) break;
                        process_raw_message(&data[index++], newLen);
                    }
                    break;
                }
                case RESET_KEYBOARD:
                    reset_keyboard();
                    break;
                case GO_INTO_BOOTLOADER:
                    bootloader_jump();
                    break;
                default:
                    break;
            }
        } break;
    }
    return 0;
}
#ifdef RAW_ENABLE
bool via_command_user(uint8_t *data, uint8_t length) {
    receivedPackages++;
    recRaw = true;
    // raw_hid_send(data, length);
    // Modify data and lenght
    // return;
    // raw_hid_send(data, length);
    // return;
    // uint8_t dataLength = length - 1;

#    if VIA_ENABLE
    if (via_mode) {
        return false;
    }
#    endif
#    if OPENRGB_ENABLE
    if (openrgb_mode) {
        raw_hid_receive_openrgb(data, length);
        return true;
    }
#    endif

    int diff = process_raw_message(data, length);

    if (diff != 0) {
        char buffer[8];
        memset(buffer, 0, 8);
        memset(data, 0, 64);
        itoa(diff, buffer, 16);

        data[0] = buffer[0];
        data[1] = buffer[1];
        data[2] = buffer[2];
        data[3] = buffer[3];
        data[4] = buffer[4];
        data[5] = buffer[5];
        data[6] = buffer[6];
        data[7] = buffer[7];
#    ifndef VIA_ENABLE
        raw_hid_send(data, length);
        return true;
#    endif
    }
    return false;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // static uint16_t fnx_layer_timer;asdfasdfasdfasdf
    bool ret = true;

    if (record->event.pressed) {
        if(keycode == KEEKC_Q){
            // recRaw = false;
        }
        else if (keycode == KEEKC_W){
            // testMode = !testMode;
        }
        else if(keycode == K_TEST1){
    //          if (wireless_get_state() == WT_CONNECTED) {
    //             report_extra_t rep = {REPORT_ID_CONSUMER, rspTestVal};
    //             wireless_driver.send_extra(&rep);
    // //         report_keyboard_t empty_report = {0};
    // //         wireless_driver.send_keyboard(&empty_report);
    //         }
        }
        else if(keycode == K_TEST2){
            rspTestVal += 1;
        }
        else if(keycode == K_TEST3){
            rspTestVal -= 1;
        }
        else if(keycode == K_TEST4){
            rspTestVal *= 2;
        }
        else if(keycode == K_TEST5){
            rspTestVal /= 2;
        }

    }

    if (keycode == CLEAR_MODS) {
        register_mods(0xFF);
        unregister_mods(0xFF);
    }
    sendKeyCodeOverRawHid(keycode, record);
    if ((keycode >= MYCKC_ESC && keycode <= MYCKC_NUMCOL)
        || (keycode >= KEEKC_ESC && keycode <= KEEKC_NUMCOL)) {
        ret = false;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case MYVIA:
                via_mode = !via_mode;
                break;
            case MYOPENRGB:
                openrgb_mode = !openrgb_mode;
                break;
            case MODE_PC_DRIVEN:
                rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_PC_DRIVEN);
                rgb_matrix_set_color_all(0, 0, 0);
                break;
            case MODE_EMPTY:
                rgb_matrix_mode(RGB_MATRIX_CUSTOM_EMPTY);
                break;
            case SAS_OFF:
                autoshift_mode = false;
                break;
            case SAS_ON:
                autoshift_mode = true;
                break;
            case SAS_TOG:
                autoshift_mode = !autoshift_mode;
                break;
            case MYRAW:
                raw_mode = !raw_mode;
                break;
        }
    } else {
        switch (keycode) {
            case SU_PCMA: {
                uint8_t buffer[RAW_EPSIZE];
                buffer[0] = MACRO_COMMAND;
                buffer[1] = TypeClipboard;

                raw_hid_send(buffer, RAW_EPSIZE);
                break;
            }
        }
    }

    // if(keycode >= (0x5700 | BETTERNUM) && keycode <= (0x5700 | STARONHOLD))
    // {
    //     return true;
    // }
    if (keycode >= MYCKC_ESC && keycode <= MYCKC_NUMCOL) {
        ret = false;
    }
    if (process_record_keychron_common(keycode, record)) {
        ret  = true;
    }

    // if(keycode >= MYCKC_ESC && keycode <= MYCKC_NUMCOL){
    //     uint8_t data[RAW_EPSIZE];

    //     data[0] = CustomKeyCode;
    //     for (size_t i = 0; i < sizeof(uint16_t); i++)
    //     {
    //         data[i+1] = (uint8_t)(keycode >> (i * 8));
    //     }

    //     data[3] = ((uint8_t*)&record->tap)[0];

    //     uint8_t* eventData = (uint8_t*) &record->event;
    //     for (size_t i = 0; i < sizeof(keyevent_t); i++)
    //     {
    //         data[i+2+sizeof(uint16_t)] = eventData[i];
    //     }
    //     for (size_t i = 2+sizeof(uint16_t)+sizeof(keyevent_t); i < RAW_EPSIZE; i++)
    //     {
    //         data[i] = 0;
    //     }
    //     raw_hid_send(data, RAW_EPSIZE);
    //     return false;
    // }

#ifdef ON_KEYBOARD_CALCULATOR_ENABLE
    process_calc_input(keycode, record);
#endif
    ret = ret & process_custom_space_cadet(keycode, record);

    // if(keycode == KC_EASYSHIFT){
    //     if(record->event.pressed)
    //         layer_on(_MYCKC);
    //     else
    //         layer_off(_MYCKC);

    // }
    return ret;
}

bool led_update_user(led_t led_state) {
    // writePin(LED_NUM_LOCK_PIN, !led_state.num_lock && !is_suspended);
    // writePin(LED_CAPS_LOCK_PIN, led_state.caps_lock && !is_suspended);

    return false;
}

void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);

    rgb_matrix_set_color_all(0, 0, 0); // turn off all LEDs when suspending
    is_suspended = true;
}

void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
    is_suspended = false;
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch (keycode) {
        case KC_PDOT:
            register_code16((!shifted) ? KC_DOT : KC_PDOT);
            break;
        default:
            if (shifted) {
                add_weak_mods(MOD_BIT(KC_LSFT));
            }
            register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch (keycode) {
        case KC_PDOT:
            unregister_code16((!shifted) ? KC_DOT : KC_PDOT);
            break;
        default:
            unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_PDOT) return true;
    if (!autoshift_mode) return false;

    switch (keycode) {
#ifndef NO_AUTO_SHIFT_ALPHA
        case AUTO_SHIFT_ALPHA:
#endif
#ifndef NO_AUTO_SHIFT_NUMERIC
        case AUTO_SHIFT_NUMERIC:
#endif
#ifndef NO_AUTO_SHIFT_SPECIAL
#    ifndef NO_AUTO_SHIFT_TAB
        case KC_TAB:
#    endif
#    ifndef NO_AUTO_SHIFT_SYMBOLS
        case AUTO_SHIFT_SYMBOLS:
#    endif
#endif
#ifdef AUTO_SHIFT_ENTER
        case KC_ENT:
#endif
            return true;
    }
    return get_custom_auto_shifted_key(keycode, record);
}


#if defined(LED_MATRIX_ENABLE) || defined(RGB_MATRIX_ENABLE)
void os_state_indicate(void) {}
#endif
