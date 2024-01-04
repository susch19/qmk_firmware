// Copyright 2021 1Conan (@1Conan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "gm807.h"
#ifdef BLUETOOTH_ENABLE
#include "outputselect.h"
#include "iton_bt.h"
#endif

#define XX NO_LED

#ifdef RGB_MATRIX_ENABLE
__attribute__ ((weak))
led_config_t g_led_config = { {
    {  0,  1, 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, XX,  13, 14, 15},
    { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,  30, 31, 32},
    { 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,  47, 48, 49},
    { 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, XX, 62,  XX, XX, XX},
    { 63, XX, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, XX, 74,  XX, 75, XX},
    { 76, 77, 78, XX, XX, XX, 79, XX, XX, XX, 80, 81, 82, 83,  84, 85, 86}
}, {
    {0  ,  0}, {26 ,  0}, {39 ,  0}, {52 ,  0}, {65 ,  0}, {84 ,  0}, {97 ,  0}, {110,  0}, {123,  0}, {143,  0}, {156,  0}, {169,  0}, {182,  0},            {198,  0}, {211,  0}, {224,  0},
    {0  , 17}, {13 , 17}, {26 , 17}, {39 , 17}, {52 , 17}, {65 , 17}, {78 , 17}, {91 , 17}, {104, 17}, {117, 17}, {130, 17}, {143, 17}, {156, 17}, {175, 17}, {198, 17}, {211, 17}, {224, 17},
    {3  , 29}, {19 , 29}, {32 , 29}, {45 , 29}, {58 , 29}, {71 , 29}, {84 , 29}, {97 , 29}, {110, 29}, {123, 29}, {136, 29}, {149, 29}, {162, 29}, {179, 29}, {198, 29}, {211, 29}, {224, 29},
    {5  , 41}, {23 , 41}, {36 , 41}, {49 , 41}, {62 , 41}, {75 , 41}, {88 , 41}, {101, 41}, {114, 41}, {127, 41}, {140, 41}, {153, 41},            {174, 41},
    {8  , 52},            {29 , 52}, {42 , 52}, {55 , 52}, {68 , 52}, {81 , 52}, {94 , 52}, {107, 52}, {120, 52}, {133, 52}, {146, 52},            {170, 52},            {211, 52},
    {2  , 64}, {18 , 64}, {34 , 64},                                  {83 , 64},                                  {131, 64}, {148, 64}, {164, 64}, {180, 64}, {198, 64}, {211, 64}, {224, 64},
}, {
    4,    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,   4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,   4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,   4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4,
    4,    4, 4, 4, 4, 4, 4, 4, 4, 4, 4,    4,      4,
    4, 4, 4,          4,          4, 4, 4, 4,   4, 4, 4
} };
#endif

void keyboard_pre_init_kb(void)  {
    setPinOutput(LED_K4);
    setPinOutput(LED_K5);
    setPinOutput(LED_USB_PORT);
    setPinOutput(LED_ENCODER);

    keyboard_pre_init_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            case KC_USB_LED:
                togglePin(LED_USB_PORT);
                break;
#ifdef BLUETOOTH_ENABLE
            case BT_MODE_TOGGLE:
                uint8_t where = where_to_send();
                if (where == OUTPUT_BLUETOOTH) {
                    iton_bt_mode_usb();
                    set_output(OUTPUT_USB);
                } else if (where == OUTPUT_USB) {
                    iton_bt_mode_bt();
                    set_output(OUTPUT_BLUETOOTH);
                }
                break;
            case BT_PROFILE1:
                iton_bt_switch_profile(0);
                break;
            case BT_PROFILE2:
                iton_bt_switch_profile(1);
                break;
            case BT_PROFILE3:
                iton_bt_switch_profile(2);
                break;
            case BT_PAIR:
                iton_bt_enter_pairing();
                break;
#endif
            default:
                break;
        }
    }

    return process_record_user(keycode, record);
}
