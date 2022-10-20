// /* Return an integer that corresponds to what kind of tap dance should be executed.
//  *
//  * How to figure out tap dance state: interrupted and pressed.
//  *
//  * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
//  *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
//  *
//  * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
//  *  has ended, but the key is still being pressed down. This generally means the key is being "held".
//  *
//  * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
//  *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
//  *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
//  *
//  * Good places to put an advanced tap dance:
//  *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
//  *
//  * Criteria for "good placement" of a tap dance key:
//  *  Not a key that is hit frequently in a sentence
//  *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
//  *    in a web form. So 'tab' would be a poor choice for a tap dance.
//  *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
//  *    letter 'p', the word 'pepper' would be quite frustating to type.
//  *
//  * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
//  *
//  */
// #include QMK_KEYBOARD_H
// #include "tap_dance_custom.h"
// #include "raw_hid_custom.h"
// #include "keymap.h"


// static td_tap_t xtap_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };


// td_state_t cur_dance(qk_tap_dance_state_t *state) {
//     if (state->count == 1) {
//         if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
//         // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
//         else return TD_SINGLE_HOLD;
//     } else if (state->count == 2) {
//         // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
//         // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
//         // keystrokes of the key, and not the 'double tap' action/macro.
//         if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
//         else if (state->pressed) return TD_DOUBLE_HOLD;
//         else return TD_DOUBLE_TAP;
//     }


//     if (state->count == 3) {
//         if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
//         else return TD_TRIPLE_HOLD;
//     } else return TD_UNKNOWN;
// }

// void betternum_finished(qk_tap_dance_state_t *state, void *user_data) {
//     xtap_state.state = cur_dance(state);
//     keyrecord_t pressed = {{{17,5},true,timer_read()}, {0,0,0,0,0}};
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP: tap_code(KC_DOT);
//             sendKeyCodeOverRawHid(KC_DOT, &pressed);break;
//         case TD_SINGLE_HOLD: tap_code(KC_COMMA);
//             sendKeyCodeOverRawHid(KC_COMMA, &pressed);break;
//         default:
//         break;
//     }
// }

// void betternum_reset(qk_tap_dance_state_t *state, void *user_data) {
//     keyrecord_t depressed = {{{17,5},false,timer_read()}, {0,0,0,0,0}};
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP: sendKeyCodeOverRawHid(KC_DOT, &depressed);break;
//         case TD_SINGLE_HOLD: sendKeyCodeOverRawHid(KC_COMMA, &depressed); break;
//         default:
//         break;
//     }
// }

// void easyshift_finished(qk_tap_dance_state_t *state, void *user_data) {
//     xtap_state.state = cur_dance(state);
//     keyrecord_t pressed = {{{0,3},true,timer_read()}, {0,0,0,0,0}};
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP:
//             register_code(KC_CAPSLOCK);
//             sendKeyCodeOverRawHid(KC_CAPSLOCK, &pressed);
//         break;
//         case TD_SINGLE_HOLD:
//             register_code16(TD_EASYSHIFT);
//             layer_on(_MYCKC);
//             sendKeyCodeOverRawHid(TD_EASYSHIFT, &pressed); break;
//         default:
//         break;
//     }
// }

// void easyshift_reset(qk_tap_dance_state_t *state, void *user_data){
//     keyrecord_t depressed = {{{0,3},false,timer_read()}, {0,0,0,0,0}};
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP:
//             unregister_code(KC_CAPSLOCK);
//             sendKeyCodeOverRawHid(KC_CAPSLOCK, &depressed);
//             break;
//         case TD_SINGLE_HOLD:
//             unregister_code16(TD_EASYSHIFT);
//             layer_off(_MYCKC);
//             sendKeyCodeOverRawHid(TD_EASYSHIFT, &depressed);
//             break;
//         default:
//         break;
//     }

// }


// void staronhold_finished(qk_tap_dance_state_t *state, void *user_data) {
//     xtap_state.state = cur_dance(state);
//     keyrecord_t pressed = {{{12,2},true,timer_read()}, {0,0,0,0,0}};
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP:
//             tap_code(KC_RBRC);
//             sendKeyCodeOverRawHid(KC_RBRC, &pressed);break; //KC_RBRC => +*~ Key for iso
//         case TD_SINGLE_HOLD:
//             tap_code16(S(KC_RBRC));
//             sendKeyCodeOverRawHid(S(KC_RBRC), &pressed);break;
//         default:
//         break;
//     }
// }

// void staronhold_reset(qk_tap_dance_state_t *state, void *user_data) {
//     keyrecord_t depressed = {{{12,2},false,timer_read()}, {0,0,0,0,0}};
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP: sendKeyCodeOverRawHid(KC_RBRC, &depressed);break;
//         case TD_SINGLE_HOLD: sendKeyCodeOverRawHid(S(KC_RBRC), &depressed); break;
//         default:
//         break;
//     }
// }




// qk_tap_dance_action_t tap_dance_actions[] = {
//     [BETTERNUM] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, betternum_finished, betternum_reset, 125),
//     [EASYSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, easyshift_finished, easyshift_reset, 125),
//     [STARONHOLD] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, staronhold_finished, staronhold_reset, 125),
// };

