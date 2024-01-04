// Copyright 2022 1Conan (@1Conan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>
#include <ch.h>
#include <hal.h>
#include "config.h"
#include "gpio.h"
#include "iton_bt.h"
#include "report.h"
#include "keycode_config.h"

#ifndef ITON_BT_SPI_PORT
#define ITON_BT_SPI_PORT     SPID1
#endif

#ifndef ITON_BT_IRQ_LINE
#define ITON_BT_IRQ_LINE     A0
#endif

#ifndef ITON_BT_INT_LINE
#define ITON_BT_INT_LINE     A1
#endif

#ifndef ITON_BT_MAX_PROFILE
#define ITON_BT_MAX_PROFILE  3
#endif

#ifndef ITON_BT_TX_LEN
#define ITON_BT_TX_LEN       16
#endif

#ifndef ITON_BT_RX_LEN
#define ITON_BT_RX_LEN       3
#endif

#ifdef KEYBOARD_SHARED_EP
#define HID_REPORT_OFFSET    1
#else
#define HID_REPORT_OFFSET    0
#endif

/**
 * Driver Macros
 */
#define HIGH_BITS(x)         ((uint8_t)(x >> 8))
#define LOW_BITS(x)          ((uint8_t)(x & 0x00FF))
#define WAIT_FOR_FINISH()    while(readPin(ITON_BT_IRQ_LINE))

/**
 * Function definitions
 */
void iton_bt_data_cb(SPIDriver *spip);
static void iton_bt_rx_end(void *arg);

/**
 * Driver variables
 */
uint8_t iton_bt_rx[ITON_BT_RX_LEN];
uint8_t iton_bt_tx[ITON_BT_TX_LEN];
uint8_t iton_bt_led_state = 0x00;
uint8_t iton_bt_send_kb_last_key = 0x00;
#if defined(PAL_USE_CALLBACKS) || defined(PAL_USE_WAIT)
static THD_WORKING_AREA(iton_bt_rx_thd_wa, 128);
thread_t *iton_bt_rx_thd;
#endif

const SPIConfig iton_bt_spicfg = {
    .slave = true,
    .data_cb = iton_bt_data_cb,
    // mcu specific
    .ctrl0 = (8 << 8), // Data Length = 8
};

/**
 * Callbacks
 */
#if defined(PAL_USE_CALLBACKS) || defined(PAL_USE_WAIT)
static THD_FUNCTION(iton_bt_rx_thd_func, arg) {
    iton_bt_rx_thd = chThdGetSelfX();
    while (true) {
        chEvtWaitAny((eventmask_t)1);

        if (iton_bt_rx[0] == led_state) {
            iton_bt_led_state = iton_bt_rx[1];
        }
    }
}

static void iton_bt_rx_end(void *arg) {
    if (readPin(ITON_BT_INT_LINE)) {
        chSysLockFromISR();
        spiStartReceiveI(&ITON_BT_SPI_PORT, ITON_BT_RX_LEN, &iton_bt_rx[0]);
        chSysUnlockFromISR();
    } else {
        chSysLockFromISR();
        spiStopTransferI(&ITON_BT_SPI_PORT, NULL);
        chEvtSignalI(iton_bt_rx_thd, (eventmask_t)1);
        chSysUnlockFromISR();
    }
}
#endif

void iton_bt_data_cb(SPIDriver *spip) {
    writePinLow(ITON_BT_IRQ_LINE);
}

/**
 * Driver Functions
 */
void iton_bt_init(void) {
    setPinOutput(ITON_BT_IRQ_LINE);
    setPinInput(ITON_BT_INT_LINE);
#if defined(PAL_USE_CALLBACKS) || defined(PAL_USE_WAIT)
    palSetLineCallback(ITON_BT_INT_LINE, iton_bt_rx_end, NULL);
#endif
}

void iton_bt_start(void) {
    spiStart(&ITON_BT_SPI_PORT, &iton_bt_spicfg);
#if defined(PAL_USE_CALLBACKS) || defined(PAL_USE_WAIT)
    palEnableLineEvent(ITON_BT_INT_LINE, PAL_EVENT_MODE_BOTH_EDGES);
    chThdCreateStatic(
        iton_bt_rx_thd_wa,
        sizeof(iton_bt_rx_thd_wa),
        HIGHPRIO,
        iton_bt_rx_thd_func,
        NULL
    );
#endif
}

void iton_bt_stop(void) {
    spiStopTransfer(&ITON_BT_SPI_PORT, NULL);
#if defined(PAL_USE_CALLBACKS) || defined(PAL_USE_WAIT)
    palDisableLineEvent(ITON_BT_INT_LINE);
#endif
    spiStop(&ITON_BT_SPI_PORT);
}

void iton_bt_send(uint8_t cmd, uint8_t *data, uint8_t len) {
    WAIT_FOR_FINISH();
    writePinHigh(ITON_BT_IRQ_LINE);
    iton_bt_tx[0] = cmd;
    memcpy(&iton_bt_tx[1], data, len);
    spiStartSend(&ITON_BT_SPI_PORT, len + 1, &iton_bt_tx[0]);
}

void iton_bt_send2(uint8_t cmd, uint8_t b1, uint8_t b2) {
    WAIT_FOR_FINISH();
    writePinHigh(ITON_BT_IRQ_LINE);
    iton_bt_tx[0] = cmd;
    iton_bt_tx[1] = b1;
    iton_bt_tx[2] = b2;
    spiStartSend(&ITON_BT_SPI_PORT, 3, &iton_bt_tx[0]);
}

void iton_bt_report_hid(uint8_t *raw) {
    iton_bt_send(report_hid, raw, 8);
}

void iton_bt_report_nkro(uint8_t *raw) {
    iton_bt_send(report_nkro, raw, 15);
}

void iton_bt_report_media(uint16_t data) {
    iton_bt_send2(report_media, HIGH_BITS(data), LOW_BITS(data));
}

void iton_bt_report_system(uint16_t data) {
    iton_bt_send(report_system, (uint8_t *)&data, 1);
}

void iton_bt_send_keyboard(report_keyboard_t *report) {

#ifndef ITON_BT_NO_6KRO_HACK
    // iton module only reads 5 of the keys in the hid report
    // so we send the last key as an nkro report.
    if (report->keys[5] != iton_bt_send_kb_last_key) {
        uint8_t nkro_report[16] = {0};
        nkro_report[report->keys[5] >> 3] |= (1 << (report->keys[5] & 7));
        iton_bt_send_kb_last_key = report->keys[5];

        return iton_bt_report_nkro(&nkro_report[0]);
    }
#endif

    iton_bt_report_hid(&report->raw[HID_REPORT_OFFSET]);
}
