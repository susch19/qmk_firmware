/* Copyright 2022 1Conan <me@1conan.com>
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

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0x05AC
#define PRODUCT_ID      0x0256
#define DEVICE_VER		0x0001

#define MANUFACTURER    "miller"
#define PRODUCT         "GM807"

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 17
#define RGB_MATRIX_LED_COUNT 87

#define DIODE_DIRECTION COL2ROW

#define MATRIX_COL_PINS { A8, A9, A10, A11, A12, A13, A14, A15, B0, B1, B2, B3, B4, B5, B6, B7, B8 }
#define MATRIX_ROW_PINS { C15, D11, D10, D9, D8, D7 }

// Encoder Config
#define ENCODERS_PAD_A { B10 }
#define ENCODERS_PAD_B { B9 }

// LED Status Indicators
#define LED_CAPS_LOCK_PIN D6

// Exta button LEDs
#define LED_K4 A6
#define LED_K5 A7

// USB "Passthrough" LED
#define LED_USB_PORT D4

// LEDs and Resistor unpopulated
// Resistor pad is near the BT module
// LEDs: LED6-10
// Resistor: RA42
#define LED_ENCODER D5

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 0

// RGB Matrix Config
#define LED_MATRIX_ROWS MATRIX_ROWS
#define LED_MATRIX_ROW_CHANNELS 3
#define LED_MATRIX_ROWS_HW (LED_MATRIX_ROWS * LED_MATRIX_ROW_CHANNELS)
#define LED_MATRIX_ROW_PINS { C3, C1, C0, C6, C5, C4, C9, C8, C7, C12, C11, C10, B13, C14, C13, B14, B15, D3 }

#define LED_MATRIX_COLS MATRIX_COLS
#define LED_MATRIX_COL_PINS MATRIX_COL_PINS

#define DRIVER_LED_TOTAL RGB_MATRIX_LED_COUNT

// RGB Matrix Effects
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_NONE
