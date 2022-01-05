#pragma once

#include "quantum.h"

// send SEND_STRING() macros depending on whether shift is held
#define ALT_SHIFT(alt_string_macro, shift_string_macro, shifted) \
    if (record->event.pressed) {                                 \
        if (is_shift_down) {                                     \
            shifted = true;                                      \
            unregister_code(KC_LSHIFT);                          \
            shift_string_macro;                                  \
        } else {                                                 \
            alt_string_macro;                                    \
        }                                                        \
    } else {                                                     \
        if (shifted && is_shift_down) {                          \
            register_code(KC_LSHIFT);                            \
        }                                                        \
    }                                                            \
    return true;

// Different keycode when Ctrl is pressed
// Inspired by https://github.com/qmk/qmk_firmware/tree/master/users/spacebarracecar
#define WHEN_CTRL(kc, shiftedKc)          \
    if (record->event.pressed) {          \
        if (is_ctl_down) {                \
            if (is_shift_down) {          \
                register_code(shiftedKc); \
            } else {                      \
                register_code(kc);        \
            }                             \
        } else {                          \
            register_code(keycode);       \
        }                                 \
    } else {                              \
        unregister_code(shiftedKc);       \
        unregister_code(kc);              \
        unregister_code(keycode);         \
    }                                     \
    return false;

#define ENABLE_MOUSE()                          \
    if (!host_keyboard_led_state().caps_lock) { \
        tap_code(KC_CAPS);                      \
    }

#define DISABLE_MOUSE()                        \
    if (host_keyboard_led_state().caps_lock) { \
        tap_code(KC_CAPS);                     \
    }

#define REENABLE_MOUSE()                       \
    if (host_keyboard_led_state().caps_lock) { \
        layer_on(_MOUSE);                      \
    }
