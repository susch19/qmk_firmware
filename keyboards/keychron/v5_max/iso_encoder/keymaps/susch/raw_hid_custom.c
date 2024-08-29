
#include <string.h>
#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "commands.h"
#include "raw_hid_custom.h"
#include "keymap.h"
#include "lkbt51.h"

void sendKeyCodeOverRawHid(uint16_t keycode, keyrecord_t* record) {

//#ifdef SEND_KEYS_RAW
    if (via_mode || !raw_mode) return;
    uint8_t data[RAW_EPSIZE];
    data[0] = OTHER_COMMAND;
    data[1] = CustomKeyCode;
    for (size_t i = 0; i < sizeof(uint16_t); i++) {
        data[i + 2] = (uint8_t)(keycode >> (i * 8));
    }

    data[4] = ((uint8_t*)&record->tap)[0];

    uint8_t* eventData = (uint8_t*)&record->event;
    for (size_t i = 0; i < sizeof(keyevent_t); i++) {
        data[i + 3 + sizeof(uint16_t)] = eventData[i];
    }
    for (size_t i = 3 + sizeof(uint16_t) + sizeof(keyevent_t); i < RAW_EPSIZE; i++) {
        data[i] = 0;
    }
    if(testMode){
        for (int i = 0; i < 255; i++)
        for (int i2 = 0; i < 255; i++) {

            test_raw_send(i, data, 20, i2);
        }
        lkbt51_write_customize_data(data, RAW_EPSIZE);
    }
    raw_hid_send(data, RAW_EPSIZE);
//#endif
}

void test_raw_send(uint8_t rsp, uint8_t* data, uint8_t len, uint8_t sn) {
    uint16_t checksum = 0;
    uint8_t buf[RAW_EPSIZE] = {0};
    uint8_t i               = 0;

    buf[i++] = 0x03;
    buf[i++] = 0xAA;
    buf[i++] = 0x57;
    buf[i++] = len;
    buf[i++] = ~len;
    buf[i++] = sn;
    buf[i++] = rsp;
    memcpy(&buf[i], data, len);
    i += len;

    for (uint8_t k = 0; k < i; k++)
        checksum += buf[i];

    raw_hid_send(buf, RAW_EPSIZE);

    if (len > 25) {
        i = 0;
        memset(buf, 0, RAW_EPSIZE);
        buf[i++] = 0x03;
        memcpy(&buf[i], data + 25, len - 25);
        i = i + len - 25;
        raw_hid_send(buf, RAW_EPSIZE);
    }
}
