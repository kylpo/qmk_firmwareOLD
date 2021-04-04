#pragma once

#include "quantum.h"

#define CHORD_VALUE(value)                                             \
    if (record->event.pressed) {                                       \
        if (chord_value == 0) {                                        \
            chord_combo_timer = timer_read();                          \
        }                                                              \
        chord_value                             = chord_value + value; \
        chord_value_buffer[chord_buffer_size++] = value;               \
    }                                                                  \
    return false;

// send SEND_STRING() strings depending on whether shift is held
#define NORM_SHIFT(norm_string, shift_string)                    \
    if (keyboard_report->mods & MOD_BIT(KC_LSFT)) {              \
        SEND_STRING(SS_UP(X_LSFT) shift_string SS_DOWN(X_LSFT)); \
    } else {                                                     \
        SEND_STRING(norm_string);                                \
    }

#define NORM_SHIFT_EVENT(norm_string, shift_string) \
    if (record->event.pressed) {                    \
        NORM_SHIFT(norm_string, shift_string)       \
    }                                               \
                                                    \
    return true;

// Different keycode when Ctrl is pressed
// Inspired by https://github.com/qmk/qmk_firmware/tree/master/users/spacebarracecar
#define WHEN_CTRL(kc, shiftedKc)                \
    if (record->event.pressed && is_ctl_down) { \
        unregister_code(KC_LCTL);               \
        if (is_shift_down) {                    \
            unregister_code(KC_LSFT);           \
            tap_code(shiftedKc);                \
            register_code(KC_LSFT);             \
        } else {                                \
            tap_code(kc);                       \
        }                                       \
        register_code(KC_LCTL);                 \
        return false;                           \
    }

// Different keycode when Ctrl is pressed
// Inspired by https://github.com/qmk/qmk_firmware/tree/master/users/spacebarracecar
#define WHEN_CMD(kc)                                                         \
    if (record->event.pressed && keyboard_report->mods & MOD_BIT(KC_LCMD)) { \
        tap_code(kc);                                                        \
        return false;                                                        \
    }

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
