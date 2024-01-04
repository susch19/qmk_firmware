
#include QMK_KEYBOARD_H
#include "custom_space_cadet.h"
#include "keymap.h"

// ********** OBSOLETE DEFINES, STOP USING! (pls?) **********
// Shift / paren setup
#ifndef LSPO_KEY
#    define LSPO_KEY KC_9
#endif
#ifndef RSPC_KEY
#    define RSPC_KEY KC_0
#endif

// Shift / Enter setup
#ifndef SFTENT_KEY
#    define SFTENT_KEY KC_ENTER
#endif

#ifdef DISABLE_SPACE_CADET_MODIFIER
#    ifndef LSPO_MOD
#        define LSPO_MOD KC_TRANSPARENT
#    endif
#    ifndef RSPC_MOD
#        define RSPC_MOD KC_TRANSPARENT
#    endif
#else
#    ifndef LSPO_MOD
#        define LSPO_MOD KC_LEFT_SHIFT
#    endif
#    ifndef RSPC_MOD
#        define RSPC_MOD KC_RIGHT_SHIFT
#    endif
#endif
// **********************************************************

// Shift / Enter setup
#ifndef CSFTENT_KEYS
#    define CSFTENT_KEYS KC_RIGHT_SHIFT, KC_TRANSPARENT, KC_TRANSPARENT, SFTENT_KEY
#endif

#ifndef NUMDOT_KEYS
#    define NUMDOT_KEYS KC_TRANSPARENT, KC_COMMA, KC_TRANSPARENT, KC_DOT
#endif

#ifndef CCLCK_KEYS
#    define CCLCK_KEYS KC_LEFT_SHIFT, KC_TRANSPARENT, KC_TRANSPARENT, KC_CAPS_LOCK
#endif

#ifndef CSTARONHOLD_KEYS
#    define CSTARONHOLD_KEYS KC_LEFT_SHIFT, KC_RBRC, KC_TRANSPARENT, KC_RBRC
#endif

#ifndef CustomRegisterCodes
#    define CustomRegisterCodes(code) (false)
#endif

#ifndef CustomLayer
#    define CustomLayer(keycode) (-1);
#endif

static uint8_t  sc_last  = 0;
static uint16_t sc_timer = 0;
#ifdef SPACE_CADET_MODIFIER_CARRYOVER
static uint8_t sc_mods = 0;
#endif

void perform_custom_space_cadet(keyrecord_t *record, uint16_t sc_keycode, uint8_t holdMod, uint16_t holdKey, uint8_t tapMod, uint16_t keycode) {
    uint8_t layer = CustomLayer(holdKey);
    if (record->event.pressed) {
        sc_last  = holdMod;
        sc_timer = timer_read();
#ifdef SPACE_CADET_MODIFIER_CARRYOVER
        sc_mods = get_mods();
#endif
        if (IS_MODIFIER_KEYCODE(holdMod)) {
            register_mods(MOD_BIT(holdMod));
        } else if CustomRegisterCodes (holdKey) {
            register_code16(holdKey);
        }
        if (layer != -1 && !IS_LAYER_ON(layer)) {
            layer_on(layer);
        }
    } else {
#ifdef TAPPING_TERM_PER_KEY
        if (sc_last == holdMod && timer_elapsed(sc_timer) < get_tapping_term(sc_keycode, record))
#else
        if (sc_last == holdMod && timer_elapsed(sc_timer) < TAPPING_TERM)
#endif
        {
            if (layer != -1 && IS_LAYER_ON(layer)) {
                layer_off(layer);
            }
            if (holdMod != tapMod) {
                if (IS_MODIFIER_KEYCODE(holdMod)) {
                    unregister_mods(MOD_BIT(holdMod));
                    if (holdMod == KC_LEFT_ALT) {
                        register_mods(MOD_BIT(holdMod));
                        unregister_mods(MOD_BIT(holdMod));
                    }
                }
                if (IS_MODIFIER_KEYCODE(tapMod)) {
                    register_mods(MOD_BIT(tapMod));
                }
            }
#ifdef SPACE_CADET_MODIFIER_CARRYOVER
            set_weak_mods(sc_mods);
#endif
            tap_code16(keycode);
#ifdef SPACE_CADET_MODIFIER_CARRYOVER
            clear_weak_mods();
#endif
            if (IS_MODIFIER_KEYCODE(tapMod)) {
                unregister_mods(MOD_BIT(tapMod));
            }
        } else {
            tap_code16(holdKey);
            if (IS_MODIFIER_KEYCODE(holdMod)) {
                unregister_mods(MOD_BIT(holdMod));
            } else if CustomRegisterCodes (holdKey) {
                unregister_code16(holdKey);
            }

            if (layer != -1 && IS_LAYER_ON(layer)) {
                layer_off(layer);
            }
        }
    }
}

bool process_custom_space_cadet(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SC_LSPO:
        case SU_LSPO: {
            if (inputLanguage == 0)
                perform_custom_space_cadet(record, keycode, CLSPO_KEYS);
            else
                perform_custom_space_cadet(record, keycode, CLSPOEN_KEYS);
            return false;
        }
        case SC_RSPC:
        case SU_RSPC: {
            if (inputLanguage == 0)
                perform_custom_space_cadet(record, keycode, CRSPC_KEYS);
            else
                perform_custom_space_cadet(record, keycode, CRSPCEN_KEYS);
            return false;
        }
        case SC_LCPO:
        case SU_LCBO: {
            if (inputLanguage == 0)
                perform_custom_space_cadet(record, keycode, CLCPO_KEYS);
            else
                perform_custom_space_cadet(record, keycode, CLCPOEN_KEYS);
            return false;
        }
        case SC_RCPC:
        case SU_RCBC: {
            if (inputLanguage == 0)
                perform_custom_space_cadet(record, keycode, CRCPC_KEYS);
            else
                perform_custom_space_cadet(record, keycode, CRCPCEN_KEYS);
            return false;
        }
        case SC_LAPO:
        case SU_LACBO: {
            if (inputLanguage == 0)
                perform_custom_space_cadet(record, keycode, CLAPO_KEYS);
            else
                perform_custom_space_cadet(record, keycode, CLAPOEN_KEYS);
            return false;
        }
        case SC_RAPC:
        case SU_RACBC: {
            if (inputLanguage == 0)
                perform_custom_space_cadet(record, keycode, CRAPC_KEYS);
            else
                perform_custom_space_cadet(record, keycode, CRAPCEN_KEYS);
            return false;
        }
        case SC_SENT: {
            perform_custom_space_cadet(record, keycode, CSFTENT_KEYS);
            return false;
        }
        case KC_BTNM: {
            perform_custom_space_cadet(record, keycode, NUMDOT_KEYS);
            return false;
        }
        case KC_STARONHOLD: {
            perform_custom_space_cadet(record, keycode, CSTARONHOLD_KEYS);
            return false;
        }
        case KC_EASYSHIFT: {
            perform_custom_space_cadet(record, keycode, CCLCK_KEYS);
            return false;
        }
        default: {
            if (record->event.pressed) {
                sc_last = 0;
            }
            break;
        }
    }
    return true;
}
