// Copyright 2021 1Conan (@1Conan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _FN,
};

enum keycodes {
    RGB_BRI_CTRL_TOG = GM807_SAFE_RANGE,
};

#define BT_PRO1 BT_PROFILE1
#define BT_PRO2 BT_PROFILE2
#define BT_PRO3 BT_PROFILE3
#define BTTOG BT_MODE_TOGGLE

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,           KC_F1,   KC_F2, KC_F3, KC_F4,  KC_F5, KC_F6, KC_F7,   KC_F8,  KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUSE,
        KC_GRV,  KC_1,    KC_2,    KC_3,  KC_4,  KC_5,   KC_6,  KC_7,  KC_8,    KC_9,   KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,  KC_R,  KC_T,   KC_Y,  KC_U,  KC_I,    KC_O,   KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,  KC_F,  KC_G,   KC_H,  KC_J,  KC_K,    KC_L,   KC_SCLN, KC_QUOT,          KC_ENT,     GUI_TOG, RGB_BRI_CTRL_TOG,
        KC_LSFT, KC_Z,    KC_X,    KC_C,  KC_V,  KC_B,   KC_N,  KC_M,  KC_COMM, KC_DOT, KC_SLSH,                   KC_RSFT,             KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,               KC_SPC,                                KC_RALT, MO(_FN), KC_APP,  KC_RCTL,    KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_FN] = LAYOUT(
        QK_BOOT,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, KC_USB_LED,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______,
        BTTOG,   BT_PRO1, BT_PRO2, BT_PRO3, _______, _______, _______, _______, _______, _______, BT_PAIR, _______, _______, _______,    _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,    _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                               _______,
        _______, _______, _______,                   _______,                                     _______, _______, _______, _______,    _______, _______, _______
    )
};

bool ctrl_brightness = true;

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        if (ctrl_brightness) {
            rgb_matrix_increase_val();
        } else {
            tap_code(KC_VOLU);
        }
    } else {
        if (ctrl_brightness) {
            rgb_matrix_decrease_val();
        } else {
            tap_code(KC_VOLD);
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_BRI_CTRL_TOG:
            if (record->event.pressed) {
                ctrl_brightness = !ctrl_brightness;
                writePin(LED_K4, ctrl_brightness);
            }
            break;

        case GUI_TOG:
            if (record->event.pressed) {
                writePin(LED_K5, !keymap_config.no_gui);
            }
            break;

        default:
            return true;
    }
    return true;
}

void keyboard_pre_init_user() {
    writePinHigh(LED_USB_PORT);
    writePinHigh(LED_K4);
}
