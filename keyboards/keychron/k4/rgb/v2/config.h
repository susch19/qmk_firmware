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

#define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_CYCLE_ALL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define ENABLE_RGB_MATRIX_HUE_BREATHING
#define ENABLE_RGB_MATRIX_HUE_PENDULUM
#define ENABLE_RGB_MATRIX_HUE_WAVE

#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CUSTOM_PC_DRIVEN
#define RGB_DISABLE_WHEN_USB_SUSPENDED
// #define DEBUG_MATRIX_SCAN_RATE

#ifndef KEYBOARD_SHARED_EP
#define KEYBOARD_SHARED_EP
#endif

#ifndef SHARED_EP_ENABLE
#define SHARED_EP_ENABLE
#endif
