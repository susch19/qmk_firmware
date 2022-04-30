 /* Copyright 2021 Dimitris Mantzouranis <d3xter93@gmail.com>
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

#define MANUFACTURER    Keychron
#define PRODUCT         K4v2 RGB

/* LED Status indicators */
#define LED_CAPS_LOCK_PIN B12
#define LED_NUM_LOCK_PIN  D4
#define LED_PIN_ON_STATE  1

// #define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
// #define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_CYCLE_ALL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
// #define ENABLE_RGB_MATRIX_HUE_BREATHING
// #define ENABLE_RGB_MATRIX_HUE_PENDULUM
#define ENABLE_RGB_MATRIX_HUE_WAVE

#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT
#define RGB_DISABLE_WHEN_USB_SUSPENDED
// #define DEBUG_MATRIX_SCAN_RATE

// #ifndef KEYBOARD_SHARED_EP
// #define KEYBOARD_SHARED_EP
// #endif

#ifndef SHARED_EP_ENABLE
#define SHARED_EP_ENABLE
#endif

#ifndef CLSPO_KEYS
    #define CLSPO_KEYS KC_LSFT, KC_TRANSPARENT, KC_LSFT, KC_8
#endif
#ifndef CRSPC_KEYS
    #define CRSPC_KEYS KC_RSFT, KC_TRANSPARENT, KC_LSFT, KC_9
#endif

#ifndef CLCPO_KEYS
    #define CLCPO_KEYS KC_LCTL, KC_TRANSPARENT, KC_RALT, KC_8
#endif
#ifndef CRCPC_KEYS
    #define CRCPC_KEYS KC_RCTL, KC_TRANSPARENT, KC_RALT, KC_9
#endif

#ifndef CLAPO_KEYS
    #define CLAPO_KEYS KC_LALT, KC_TRANSPARENT, KC_RALT, KC_7
#endif
#ifndef CRAPC_KEYS
    #define CRAPC_KEYS KC_RALT, KC_TRANSPARENT, KC_RALT, KC_0
#endif


#define SPACE_CADET_DISABLE 1
