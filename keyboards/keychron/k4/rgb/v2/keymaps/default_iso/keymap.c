/*
Copyright 2021 Dimitris Mantzouranis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "print.h"
#include "commands.h"
#include "tap_dance_custom.h"
#include "raw_hid_custom.h"
#include "keymap.h"


extern bool last_suspend_state;

static bool calcMode = false;


const key_override_t space_key_override = ko_make_with_layers_and_negmods(MOD_MASK_SHIFT, KC_SPACE, KC_BACKSPACE, ~0, MOD_MASK_CAG);
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_DELETE, KC_INSERT);


const key_override_t **key_overrides = (const key_override_t *[]){
    &space_key_override,
    &delete_key_override,
    NULL // Null terminate the array of overrides!
};

// #pragma region TapDance

// #pragma endregion TapDance

#define MG_NKRO MAGIC_TOGGLE_NKRO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*				+--------------------------------------------------------------------------+-------------------+
                    | ESC | F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8 | F9 | F10| F11| F12|DEL|HOME| END|PGUP|PGDN| RGB|
                    +--------------------------------------------------------------------------+----|----|----|----+
                    |  ~  |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = | BACKSP |NLCK|  / |  * |  - |
                    +--------------------------------------------------------------------------+----|----|----|----+
                    |  TAB  |  Q |  W |  E |  R |  T |  Y |  U |  I |  O |  P |  [ |  ] |      |  7 |  8 |  9 |    |
                    +--------------------------------------------------------------------  RET +----|----|----|  + +
                    | CAPSLCK |  A |  S |  D |  F |  G |  H |  J |  K |  L |  ; |  ' | # |     |  4 |  5 |  6 |    |
                    +--------------------------------------------------------------------------+----|----|----|----+
                    |LSHFT|  \ |  Z |  X |  C |  V |  B |  N |  M |  , |  . |  / | RSHIFT | UP |  1 |  2 |  3 |    |
                    +--------------------------------------------------------------------------+----|----|----| ENT+
                    |LCTRL| LGUI|  LALT|            SPACE            | RALT| FN|RCTRL| LFT| DWN| RGT|  0 |  . |    |
                    +--------------------------------------------------------------------------+-------------------+
    */
    /*  Row:        0          1          2          3        4        5        6         7        8        9          10         11         12         13         14         15         16         17         18     */
    [_BASE] =  {{KC_ESC,    KC_F1,      KC_F2,      KC_F3,   KC_F4,    KC_F5,   KC_F6, KC_F7,   KC_F8,      KC_F9,     KC_F10, KC_F11, KC_F12, KC_DEL, KC_HOME, KC_END, KC_PGUP, KC_PGDN, RGB_TOG},
                {KC_GRV,    KC_1,       KC_2,       KC_3,    KC_4,     KC_5,    KC_6,  KC_7,    KC_8,       KC_9,      KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_NO, KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS},
                {KC_TAB,    KC_Q,       KC_W,       KC_E,    KC_R,     KC_T,    KC_Y,  KC_U,    KC_I,       KC_O,      KC_P, KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_P7, KC_P8, KC_P9, KC_PPLS},
                {TD(EASYSHIFT),   KC_A,       KC_S,       KC_D,    KC_F,     KC_G,    KC_H,  KC_J,    KC_K,       KC_L,      KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, KC_NO, KC_P4, KC_P5, KC_P6, KC_NO},
                {KC_LSFT,   KC_NUBS,    KC_Z,       KC_X,    KC_C,     KC_V,    KC_B,  KC_N,    KC_M,       KC_COMM,   KC_DOT, KC_SLSH, KC_NO, KC_RSFT, KC_UP, KC_P1, KC_P2, KC_P3, KC_PENT},
                {KC_LCTL,   KC_LGUI,    KC_LALT,    KC_NO,   KC_NO,    KC_NO,   KC_SPC, KC_NO,  KC_NO,      KC_NO,      KC_RALT, MO(_FL), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_P0, TD(BETTERNUM), KC_NO}},

    /*  Row:        0          1          2          3        4        5        6         7         8        9          10        11           12          13        14        15       16         17        18     */
    [_FL] = {{RESET, KC_SLCK, KC_PAUS, KC_APP, _______, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC__MUTE, KC__VOLDOWN, KC__VOLUP, KC_INS, KC_PSCR, _______, _______, _______, RGB_MOD},
             {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NO, _______, _______, RGB_MODE_RAINBOW, RGB_HUI},
             {_______, _______, _______, _______, _______, MYCALC, _______, _______, _______, _______, KC_PSCR, _______, _______, _______, KC_NO, RGB_MODE_XMAS, RGB_MODE_GRADIENT, RGB_MODE_RGBTEST, _______},
             {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_NO, RGB_MODE_SWIRL, RGB_MODE_SNAKE, RGB_MODE_KNIGHT, KC_NO},
             {KC_LSFT, _______, _______, _______, _______, _______, _______, _______, MG_NKRO, _______, _______, KC_APP, KC_NO, KC_RSFT, RGB_SPI, RGB_MODE_PLAIN, RGB_MODE_BREATHE, RGB_MODE_RAINBOW, RGB_SAI},
             {_______, KC_LALT, KC_LGUI, KC_NO  , KC_NO  , KC_NO  , _______, KC_NO  , KC_NO  , KC_NO  , LCLS(KC_M), MO(_FL), LCLS(KC_U), _______, RGB_SPD, _______, RGB_MODE_TWINKLE, _______, KC_NO}},

    [_MYCKC] =  {{MYCKC_ESC,    MYCKC_F1,      MYCKC_F2,      MYCKC_F3,   MYCKC_F4,    MYCKC_F5,   MYCKC_F6, MYCKC_F7,   MYCKC_F8,      MYCKC_F9,     MYCKC_F10, MYCKC_F11, MYCKC_F12, MYCKC_DEL, MYCKC_HOME, MYCKC_END, MYCKC_PGUP, MYCKC_PGDN, MYCKC_RGB},
                {MYCKC_GRV,    MYCKC_1,       MYCKC_2,       MYCKC_3,    MYCKC_4,     MYCKC_5,    MYCKC_6,  MYCKC_7,    MYCKC_8,       MYCKC_9,      MYCKC_0, MYCKC_MINS, MYCKC_EQL, MYCKC_BSPC, KC_NO, MYCKC_NLCK, MYCKC_PSLS, MYCKC_PAST, MYCKC_PMNS},
                {MYCKC_TAB,    MYCKC_Q,       MYCKC_W,       MYCKC_E,    MYCKC_R,     MYCKC_T,    MYCKC_Y,  MYCKC_U,    MYCKC_I,       MYCKC_O,      MYCKC_P, MYCKC_LBRC, MYCKC_RBRC, KC_NO, KC_NO, MYCKC_P7, MYCKC_P8, MYCKC_P9, MYCKC_PPLS},
                {MYCKC_CAPS,   MYCKC_A,       MYCKC_S,       MYCKC_D,    MYCKC_F,     MYCKC_G,    MYCKC_H,  MYCKC_J,    MYCKC_K,       MYCKC_L,      MYCKC_SCLN, MYCKC_QUOT, MYCKC_NUHS, MYCKC_ENT, KC_NO, MYCKC_P4, MYCKC_P5, MYCKC_P6, KC_NO},
                {MYCKC_LSFT,   MYCKC_NUBS,    MYCKC_Z,       MYCKC_X,    MYCKC_C,     MYCKC_V,    MYCKC_B,  MYCKC_N,    MYCKC_M,       MYCKC_COMM,   MYCKC_DOT, MYCKC_SLSH, KC_NO, MYCKC_RSFT, MYCKC_UP, MYCKC_P1, MYCKC_P2, MYCKC_P3, MYCKC_PENT},
                {MYCKC_LCTL,   MYCKC_LGUI,    MYCKC_LALT,    KC_NO,   KC_NO,    KC_NO,   MYCKC_SPC, KC_NO,  KC_NO,      KC_NO,      MYCKC_RALT, MYCKC_FN, MYCKC_RCTL, MYCKC_LEFT, MYCKC_DOWN, MYCKC_RGHT, MYCKC_P0, MYCKC_NUMCOL, KC_NO}},

};

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {  // BT mode
                // do stuff
            } else {  // Cable mode
                // do stuff
            }
            break;
        case 1:
            if (active) {   // Mac/iOS mode
                layer_on(2);
            } else {      // Win/Android mode
                layer_clear();
            }
            break;
    }
    return true;
}


void interpret_pc_macro(uint8_t *data, uint8_t length) {
    for (size_t i = 0; i < length; i++)
    {
        if(data[i] == KC_NO)
            break;

        tap_code(data[i]);
    }
}

void interpret_pc_macro_16_bit(uint8_t *data, uint8_t length) {
    for (size_t i = 0; i + 1 < length; i+=2)
    {
        if(data[i] == KC_NO)
            break;

        tap_code16(((uint16_t)data[i] << 8) | data[i+1]);
    }
}

void setColorsFor(uint8_t *data, uint8_t dataLength, uint8_t index){
     for (size_t i = 0; i < dataLength; i++)
    {
        rgb_matrix_set_color((index+i) % 101, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
}

#pragma pack(1)
typedef struct {
    uint8_t mode : 2;
    uint8_t categorie : 8;
    uint8_t reserved : 6;
} command_header;

typedef struct {
    uint8_t mode : 2;
    uint8_t count : 5;
    uint8_t rgb : 2;
    uint8_t index : 7;
} rgb_header;

rgb_header getRGBHeader(uint8_t *data){
    rgb_header header= {};
    uint16_t headerData = ((uint16_t)data[0] << 8) | data[1];
    header.mode = (headerData  >> 14) & 0b11;
    header.count = (headerData >> 9) & 0b11111;
    header.rgb = (headerData >> 7) & 0b11;
    header.index = (headerData) & 0b1111111;

    return header;
}

command_header getCommandHeader(uint8_t *data){
    command_header header = {};
    uint16_t headerData = ((uint16_t)data[0] << 8) | data[1];
    header.mode = (headerData  >> 14) & 0b11;
    header.categorie = (headerData >> 6 ) & 0xff;
    header.reserved = (headerData) & 0b111111;

    return header;
}
void raw_hid_receive(uint8_t *data, uint8_t length) {
    // raw_hid_send(data, length);
    // Modify data and lenght
    // return;
    // raw_hid_send(data, length);
    // return;
    uint8_t dataLength = length -1;

    command_header header = getCommandHeader(data);

    switch(header.mode){
        case RGB_COMMAND:{
            rgb_header rgbheader = getRGBHeader(data);
            switch(rgbheader.rgb){
                case IndexItereationRGBZero:
                case Reserved: { //Special case, only first byte is used for header, because index gets stripped, so data starts at index 1!
                    setColorsFor(&data[1], rgbheader.count, 0);
                    break;
                }
                case PerKeyRGB:
                    for (size_t i = 0; i < rgbheader.count; i++)
                    {
                        rgb_matrix_set_color(data[i * 4 + 2], data[i * 4 + 3], data[i * 4 + 4], data[i * 4 + 5]); //First 2 bytes always used
                    }
                    break;
                case IndexItereationRGB:{
                    setColorsFor(&data[2], rgbheader.count, rgbheader.index);
                    break;
                }
                }
            break;
        }
        case LAYER_COMMAND:{
            switch (header.categorie)
            {
            case SwitchToLayer:
                layer_invert(data[2]);
                break;
            case ResetLayer:
                layer_clear();
                layer_on(data[2]);
                break;
            }

            break;
        }
        case MACRO_COMMAND:
        {
            switch (header.categorie)
            {
            case PCMacro:
                interpret_pc_macro(&data[2], dataLength);
                break;
            case PCMacro16Bit:
                interpret_pc_macro_16_bit(&data[2], dataLength);
                break;
            case PCMacroString:
                send_string((char *)&data[2]);
                break;
            }
            break;
        }
        case OTHER_COMMAND:{
            switch (header.categorie)
            {
                case ResetKeyboard:
                    reset_keyboard();
                    break;
                case GoIntoBootloader:
                    bootloader_jump();
                    break;
                default:
                    break;
            }
        }
        break;
    }
    // raw_hid_send(data, length);
}

// void keyboard_post_init_user(void) {
//   // Customise these values to desired behaviour
//   debug_enable=true;
//   //debug_keyboard=true;
//   //debug_mouse=true;

// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // static uint16_t fnx_layer_timer;
    bool ret = true;

    if(keycode >= (0x5700 | BETTERNUM) && keycode <= (0x5700 | EASYSHIFT))
    {
        return true;
    }
    if(last_suspend_state){
        tap_code16(KC_WAKE);
    }

    if(keycode == MYCALC)
    {
        calcMode = !calcMode;
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


    if(keycode >= MYCKC_ESC && keycode <= MYCKC_NUMCOL){
        ret = false;
    }
    sendKeyCodeOverRawHid(keycode, record);
    return ret;
}


bool led_update_user(led_t led_state){
    writePin(LED_NUM_LOCK_PIN, !led_state.num_lock && !last_suspend_state);
    writePin(LED_CAPS_LOCK_PIN, led_state.caps_lock && !last_suspend_state);
    return false;
}
