#pragma once
#include QMK_KEYBOARD_H

#ifndef RAW_EPSIZE
#define RAW_EPSIZE 64
#endif

#ifndef KEYMAP_ISO
#define KEYMAP_ISO
#endif

enum layer_names { _BASE = 0, _FL, _MYCKC};
enum my_keycodes {
    MYCALC= SAFE_RANGE,
    KC_BTNM,
    KC_STARONHOLD,
    CLEAR_MODS,
    MYVIA,
    MYOPENRGB,
    KC_EASYSHIFT = 0x5FFE,
    TD_EASYSHIFT = 0x5FFF,
    MYCKC_ESC = 0x6000, //6000 - 5DA6 => 602 Keys before
    MYCKC_F1,
    MYCKC_F2,
    MYCKC_F3,
    MYCKC_F4,
    MYCKC_F5,
    MYCKC_F6,
    MYCKC_F7,
    MYCKC_F8,
    MYCKC_F9,
    MYCKC_F10,
    MYCKC_F11,
    MYCKC_F12,
    MYCKC_DEL,
    MYCKC_HOME,
    MYCKC_END,
    MYCKC_PGUP,
    MYCKC_PGDN,
    MYCKC_RGB,
    MYCKC_GRV,
    MYCKC_1,
    MYCKC_2,
    MYCKC_3,
    MYCKC_4,
    MYCKC_5,
    MYCKC_6,
    MYCKC_7,
    MYCKC_8,
    MYCKC_9,
    MYCKC_0,
    MYCKC_MINS,
    MYCKC_EQL,
    MYCKC_BSPC,
    MYCKC_NLCK,
    MYCKC_PSLS,
    MYCKC_PAST,
    MYCKC_PMNS,
    MYCKC_TAB,
    MYCKC_Q,
    MYCKC_W,
    MYCKC_E,
    MYCKC_R,
    MYCKC_T,
    MYCKC_Y,
    MYCKC_U,
    MYCKC_I,
    MYCKC_O,
    MYCKC_P,
    MYCKC_LBRC,
    MYCKC_RBRC,
    MYCKC_P7,
    MYCKC_P8,
    MYCKC_P9,
    MYCKC_PPLS,
    MYCKC_CAPS,
    MYCKC_A,
    MYCKC_S,
    MYCKC_D,
    MYCKC_F,
    MYCKC_G,
    MYCKC_H,
    MYCKC_J,
    MYCKC_K,
    MYCKC_L,
    MYCKC_SCLN,
    MYCKC_QUOT,
    MYCKC_NUHS,
    MYCKC_ENT,
    MYCKC_P4,
    MYCKC_P5,
    MYCKC_P6,
    MYCKC_LSFT,
    MYCKC_NUBS,
    MYCKC_Z,
    MYCKC_X,
    MYCKC_C,
    MYCKC_V,
    MYCKC_B,
    MYCKC_N,
    MYCKC_M,
    MYCKC_COMM,
    MYCKC_DOT,
    MYCKC_SLSH,
    MYCKC_RSFT,
    MYCKC_UP,
    MYCKC_P1,
    MYCKC_P2,
    MYCKC_P3,
    MYCKC_PENT,
    MYCKC_LCTL,
    MYCKC_LGUI,
    MYCKC_LALT,
    MYCKC_SPC,
    MYCKC_RALT,
    MYCKC_FN,
    MYCKC_RCTL,
    MYCKC_LEFT,
    MYCKC_DOWN,
    MYCKC_RGHT,
    MYCKC_P0,
    MYCKC_NUMCOL
};

#define CustomRegisterCodes(code) (code == TD_EASYSHIFT)

#ifndef CCLCK_KEYS
#    define CCLCK_KEYS KC_TRANSPARENT, TD_EASYSHIFT, KC_TRANSPARENT, KC_CAPSLOCK
#endif

#ifndef CustomLayer
    #define CustomLayer(keycode) (keycode == 0x5FFF  ? _MYCKC : -1);
#endif

