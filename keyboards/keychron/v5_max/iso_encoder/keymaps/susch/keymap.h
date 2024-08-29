#pragma once
#include QMK_KEYBOARD_H

#ifndef RAW_EPSIZE
#    define RAW_EPSIZE 32
#endif

#ifndef KEYMAP_ISO
#    define KEYMAP_ISO
#endif

extern bool    is_suspended;
extern bool    via_mode;
extern bool    raw_mode;
extern bool    testMode;
extern uint8_t rspTestVal;
extern bool    openrgb_mode;
extern uint8_t inputLanguage;

enum layer_names { _BASE = 0, _FL, _D1, _D2,_KEEKC, _EN, _MYCKC };
enum my_keycodes {
    MYCALC = SAFE_RANGE,
    KC_BTNM,
    KC_STARONHOLD,
    CLEAR_MODS,
    MYVIA,
    MYOPENRGB,
    SU_LSPO,
    SU_LCBO,
    SU_LACBO,
    SU_RSPC,
    SU_RCBC,
    SU_RACBC,
    MODE_PC_DRIVEN,
    SAS_OFF,
    SAS_ON,
    SAS_TOG,
    SU_PCMACRO,
    MYRAW,
    MODE_EMPTY,
    K_TEST1,
    K_TEST2,
    K_TEST3,
    K_TEST4,
    K_TEST5,
    K_TEST6,

    KC_EASYHOME,
    TD_EASYHOME,
    KC_EASYEND,
    TD_EASYEND,
    KC_EASYTAB,
    TD_EASYTAB,
    KC_EASYSHIFT,
    TD_EASYSHIFT,

    MYCKC_ESC    = QK_USER_MAX - 300, // 208 already used, 92 space. 147 before this
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
    MYCKC_NUMCOL,
    KEEKC_ESC,
    KEEKC_F1,
    KEEKC_F2,
    KEEKC_F3,
    KEEKC_F4,
    KEEKC_F5,
    KEEKC_F6,
    KEEKC_F7,
    KEEKC_F8,
    KEEKC_F9,
    KEEKC_F10,
    KEEKC_F11,
    KEEKC_F12,
    KEEKC_DEL,
    KEEKC_HOME,
    KEEKC_END,
    KEEKC_PGUP,
    KEEKC_PGDN,
    KEEKC_RGB,
    KEEKC_GRV,
    KEEKC_1,
    KEEKC_2,
    KEEKC_3,
    KEEKC_4,
    KEEKC_5,
    KEEKC_6,
    KEEKC_7,
    KEEKC_8,
    KEEKC_9,
    KEEKC_0,
    KEEKC_MINS,
    KEEKC_EQL,
    KEEKC_BSPC,
    KEEKC_NLCK,
    KEEKC_PSLS,
    KEEKC_PAST,
    KEEKC_PMNS,
    KEEKC_TAB,
    KEEKC_Q,
    KEEKC_W,
    KEEKC_E,
    KEEKC_R,
    KEEKC_T,
    KEEKC_Y,
    KEEKC_U,
    KEEKC_I,
    KEEKC_O,
    KEEKC_P,
    KEEKC_LBRC,
    KEEKC_RBRC,
    KEEKC_P7,
    KEEKC_P8,
    KEEKC_P9,
    KEEKC_PPLS,
    KEEKC_CAPS,
    KEEKC_A,
    KEEKC_S,
    KEEKC_D,
    KEEKC_F,
    KEEKC_G,
    KEEKC_H,
    KEEKC_J,
    KEEKC_K,
    KEEKC_L,
    KEEKC_SCLN,
    KEEKC_QUOT,
    KEEKC_NUHS,
    KEEKC_ENT,
    KEEKC_P4,
    KEEKC_P5,
    KEEKC_P6,
    KEEKC_LSFT,
    KEEKC_NUBS,
    KEEKC_Z,
    KEEKC_X,
    KEEKC_C,
    KEEKC_V,
    KEEKC_B,
    KEEKC_N,
    KEEKC_M,
    KEEKC_COMM,
    KEEKC_DOT,
    KEEKC_SLSH,
    KEEKC_RSFT,
    KEEKC_UP,
    KEEKC_P1,
    KEEKC_P2,
    KEEKC_P3,
    KEEKC_PENT,
    KEEKC_LCTL,
    KEEKC_LGUI,
    KEEKC_LALT,
    KEEKC_SPC,
    KEEKC_RALT,
    KEEKC_FN,
    KEEKC_RCTL,
    KEEKC_LEFT,
    KEEKC_DOWN,
    KEEKC_RGHT,
    KEEKC_P0,
    KEEKC_NUMCOL,
    MYCKC_KNOBL,
    MYCKC_KNOBR,
    MYCKC_KNOBP,
    KEEKC_KNOBL,
    KEEKC_KNOBR,
    KEEKC_KNOBP,
};

#define CustomRegisterCodes(code) (code != TD_EASYSHIFT && code != TD_EASYTAB  && code != TD_EASYHOME && code != TD_EASYEND)

#ifndef CCLCK_KEYS
// #    define CCLCK_KEYS KC_RALT, KC_TRANSPARENT, KC_LSFT, KC_COMMA
#    define CCLCK_KEYS KC_TRANSPARENT, TD_EASYSHIFT, KC_LSFT, KC_COMMA
#endif

#ifndef CTAEA_KEYS
// #    define CCLCK_KEYS KC_TRANSPARENT, TD_EASYSHIFT, KC_TRANSPARENT, KC_CAPS_LOCK
#    define CTAEA_KEYS KC_TRANSPARENT, TD_EASYTAB, KC_TRANSPARENT, KC_TAB
#endif

#ifndef CHOEA_KEYS
#    define CHOEA_KEYS KC_TRANSPARENT, TD_EASYHOME, KC_TRANSPARENT, KC_HOME
#endif
#ifndef CENEA_KEYS
#    define CENEA_KEYS KC_TRANSPARENT, TD_EASYEND, KC_TRANSPARENT, KC_END
#endif

#ifndef CustomLayer
#    define CustomLayer(keycode) (keycode == TD_EASYSHIFT ? _MYCKC : (keycode == TD_EASYTAB ? _KEEKC : (keycode == TD_EASYHOME ? _D2 : (keycode == TD_EASYEND ? _D1 : -1))));
#endif

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
