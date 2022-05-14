
#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "commands.h"
#include "raw_hid_custom.h"
#include "keymap.h"

void sendKeyCodeOverRawHid(uint16_t keycode, keyrecord_t *record){
//     uint8_t data[RAW_EPSIZE];
//     data[0] = OTHER_COMMAND;
//     data[1] = CustomKeyCode;
//     for (size_t i = 0; i < sizeof(uint16_t); i++)
//     {
//        data[i+2] = (uint8_t)(keycode >> (i * 8));
//     }

//     data[4] = ((uint8_t*)&record->tap)[0];

//     uint8_t* eventData = (uint8_t*) &record->event;
//     for (size_t i = 0; i < sizeof(keyevent_t); i++)
//     {
//        data[i+3+sizeof(uint16_t)] = eventData[i];
//     }
//     for (size_t i = 3+sizeof(uint16_t)+sizeof(keyevent_t); i < RAW_EPSIZE; i++)
//     {
//        data[i] = 0;
//    }

    // raw_hid_send(data, RAW_EPSIZE);
}
