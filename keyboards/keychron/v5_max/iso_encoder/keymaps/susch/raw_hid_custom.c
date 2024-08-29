
#include <string.h>
#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "commands.h"
#include "raw_hid_custom.h"
#include "keymap.h"
#include "battery.h"
#include "keychron_raw_hid.h"

uint8_t sendBatteryCounter = 0;

void sendKeyCodeOverRawHid(uint16_t keycode, keyrecord_t* record) {
    // #ifdef SEND_KEYS_RAW
    if (via_mode || !raw_mode) return;
    uint8_t data[RAW_EPSIZE];
    uint8_t offset = 0;
    data[offset++] = OTHER_COMMAND;
    data[offset++] = CustomKeyCode;
    for (size_t i = 0; i < sizeof(uint16_t); i++) {
        data[offset++] = (uint8_t)(keycode >> (i * 8));
    }

    data[offset++] = ((uint8_t*)&record->tap)[0];

    uint8_t* eventData = (uint8_t*)&record->event;
    for (size_t i = 0; i < sizeof(keyevent_t); i++) {
        data[offset++] = eventData[i];
    }
    bool sendBattery = sendBatteryCounter++ == 0;
    if (sendBattery)
        data[offset++] = battery_get_percentage(); // Offset: 11
    else
        data[offset++] = 0;

    for (size_t i = offset; i < RAW_EPSIZE; i++) {
        data[i] = 0;
    }

    kc_raw_hid_send(RAW_HID_SRC_USB, data, RAW_EPSIZE);
    // #endif
}

// void test_raw_send(uint8_t rsp, uint8_t* data, uint8_t len, uint8_t sn) {
//     uint16_t checksum = 0;
//     uint8_t buf[RAW_EPSIZE] = {0};
//     uint8_t i               = 0;

//     buf[i++] = 0x03;
//     buf[i++] = 0xAA;
//     buf[i++] = 0x57;
//     buf[i++] = len;
//     buf[i++] = ~len;
//     buf[i++] = sn;
//     buf[i++] = rsp;
//     memcpy(&buf[i], data, len);
//     i += len;

//     for (uint8_t k = 0; k < i; k++)
//         checksum += buf[i];

//     raw_hid_send(buf, RAW_EPSIZE);

//     if (len > 25) {
//         i = 0;
//         memset(buf, 0, RAW_EPSIZE);
//         buf[i++] = 0x03;
//         memcpy(&buf[i], data + 25, len - 25);
//         i = i + len - 25;
//         raw_hid_send(buf, RAW_EPSIZE);
//     }
// }
