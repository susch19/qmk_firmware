// Copyright 2023 1Conan (@1Conan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>
#include <hal.h>
#include "gpio.h"
#include "config.h"
#include "sn32_spi.h"
#include "iton_bt.h"

#define SN32_SPI0 ((sn32_spi_t *)SN_SPI0_BASE)

#ifndef ITON_BT_IRQ_LINE
#    define ITON_BT_IRQ_LINE A0
#endif

#ifndef ITON_BT_INT_LINE
#    define ITON_BT_INT_LINE A1
#endif

#ifdef KEYBOARD_SHARED_EP
#    define HID_REPORT_OFFSET 1
#else
#    define HID_REPORT_OFFSET 0
#endif

#ifndef ITON_BT_BUFFER_LEN
#  ifndef ITON_BT_ENABLE_6KRO_HACK
#    define ITON_BT_BUFFER_LEN 9
#  else
#    define ITON_BT_BUFFER_LEN 16
#  endif
#endif

/**
 * Driver Macros
 */
#define HIGH_BITS(x)         ((uint8_t)(x >> 8))
#define LOW_BITS(x)          ((uint8_t)(x & 0x00FF))

/**
 * Function definitions
 */

/**
 * Callbacks
 */
__attribute__((weak)) void iton_bt_battery_voltage_low(void) {}
__attribute__((weak)) void iton_bt_battery_exit_low_battery_mode(void) {}
__attribute__((weak)) void iton_bt_battery_low_power_shutdown(void) {}

__attribute__((weak)) void iton_bt_connection_successful(void) {}
__attribute__((weak)) void iton_bt_entered_pairing(void) {}
__attribute__((weak)) void iton_bt_disconnected(void) {}
__attribute__((weak)) void iton_bt_enters_connection_state(void) {}

/**
 * Driver variables
 */
bool iton_bt_is_connected = false;
uint8_t iton_bt_led_state = 0x00;

uint8_t iton_bt_count = 0;
uint8_t iton_bt_index = 0;
uint8_t iton_bt_buffer[ITON_BT_BUFFER_LEN];

/**
 * Driver Functions
 */
void iton_bt_init(void) {
    setPinOutput(ITON_BT_IRQ_LINE);
    setPinInput(ITON_BT_INT_LINE);

    writePinLow(ITON_BT_IRQ_LINE);

    sys1EnableSPI0();

#define SPI_DL_8 (7 << 8)
#define SPI_SLAVE_MODE (1 << 3)
#define SPI_AUTOSEL_ENABLE (0 << 18)
#define SPI_SLAVE_DATA_OUTPUT_ENABLE (0 << 2)

    SN32_SPI0->CTRL0 = SPI_DL_8 | SPI_SLAVE_MODE | SPI_AUTOSEL_ENABLE | SPI_SLAVE_DATA_OUTPUT_ENABLE;

    SN32_SPI0->CLKDIV = 2;
    SN32_SPI0->CTRL0_b.FRESET = 0b11;

    SN32_SPI0->IC = 0b1111;
    SN32_SPI0->IE = 0b0100;

    nvicClearPending(SN32_SPI0_NUMBER);
    nvicEnableVector(SN32_SPI0_NUMBER, 0);

    SN32_SPI0->CTRL0 |= (1 << 0);
}

void iton_bt_send(uint8_t cmd, uint8_t *data, uint8_t len) {
    while (readPin(ITON_BT_IRQ_LINE));

    writePinHigh(ITON_BT_IRQ_LINE);
    iton_bt_index = 0;
    iton_bt_count = len + 1;
    iton_bt_buffer[0] = cmd;
    memcpy(&iton_bt_buffer[1], data, len);

    SN32_SPI0->DATA = iton_bt_buffer[0];
}

void iton_bt_send2(uint8_t cmd, uint8_t b1, uint8_t b2) {
    uint8_t buf[] = {b1, b1};
    iton_bt_send(cmd, &buf[0], 2);
}

void iton_bt_send_fn(bool pressed) {
    uint8_t data = pressed ? 0xA3 : 0x00;

    iton_bt_send(report_fn, &data, 1);
}

void iton_bt_send_system(uint16_t data) {
    iton_bt_send(report_system, (uint8_t *)&data, 1);
}

void iton_bt_send_consumer(uint16_t data) {
    iton_bt_send2(report_consumer, HIGH_BITS(data), LOW_BITS(data));
}

void iton_bt_send_keyboard(report_keyboard_t *report) {
    // iton module only reads 5 of the keys in the hid report
    // so we send the last key as an nkro report.
#ifdef ITON_BT_ENABLE_6KRO_HACK
    if (report->keys[5] != iton_bt_send_kb_last_key) {
        uint8_t nkro_report[15] = {0};
        nkro_report[report->keys[5] >> 3] |= (1 << (report->keys[5] & 7));
        static uint8_t iton_bt_send_kb_last_key = report->keys[5];

        return iton_bt_send(report_nkro, &nkro_report[0], 15);
    }
#endif

    iton_bt_send(report_hid, &report->raw[HID_REPORT_OFFSET], 8);
}

OSAL_IRQ_HANDLER(SN32_SPI0_HANDLER) {
    OSAL_IRQ_PROLOGUE();
    if (SN32_SPI0->RIS_b.RXFIFOTHIF) {
        uint8_t data = SN32_SPI0->DATA;
        SN32_SPI0->IC = 0b0100;

        if (!readPin(ITON_BT_INT_LINE)) {
            if (++(iton_bt_index) >= iton_bt_count) {
                iton_bt_index = 0;
                writePinLow(ITON_BT_IRQ_LINE);
            } else {
                SN32_SPI0->DATA = iton_bt_buffer[iton_bt_index];
            }
        } else {
            iton_bt_buffer[iton_bt_index++] = data;
            SN32_SPI0->DATA = 0x00;

            if (iton_bt_buffer[0] == led_state && iton_bt_index == 2) {
                iton_bt_index = 0;
                iton_bt_led_state = iton_bt_buffer[1];
            } else if (iton_bt_buffer[0] == notification && iton_bt_index == 4) {
                iton_bt_index = 0;
                switch (iton_bt_buffer[1]) {
                    case notif_battery:
                        switch (iton_bt_buffer[2]) {
                            case batt_voltage_low:
                                iton_bt_battery_voltage_low();
                                break;
                            case batt_exit_low_battery_mode:
                                iton_bt_battery_exit_low_battery_mode();
                                break;
                            case batt_low_power_shutdown:
                                iton_bt_battery_low_power_shutdown();
                                break;
                        }
                        break;
                    case notif_bluetooth:
                        switch (iton_bt_buffer[2]) {
                            case bt_connection_success:
                                iton_bt_is_connected = true;
                                iton_bt_connection_successful();
                                break;
                            case bt_entered_pairing:
                                iton_bt_entered_pairing();
                                break;
                            case bt_disconected:
                                iton_bt_is_connected = false;
                                iton_bt_disconnected();
                                break;
                            case bt_enters_connection:
                                iton_bt_enters_connection_state();
                                break;
                        }
                        break;
                }
            }
        }
    }
    OSAL_IRQ_EPILOGUE();
}
