#pragma once

#include "quantum.h"

// send SEND_STRING() macros depending on whether shift is held
#define ALT_SHIFT(alt_string_macro, shift_string_macro, shifted) \
if (record->event.pressed) { \
  if (is_shift_key_pressed) { \
    shifted = true; \
    unregister_code(KC_LSHIFT); \
    shift_string_macro; \
  } else { \
    alt_string_macro; \
  } \
} else { \
  if (shifted && is_shift_key_pressed) { \
    register_code(KC_LSHIFT); \
  } \
} \
return false;

// Different keycode when Ctrl is pressed
// Inspired by https://github.com/qmk/qmk_firmware/tree/master/users/spacebarracecar
#define WHEN_CTRL(kc) \
if(record->event.pressed) { \
  if (get_mods() & MOD_BIT(KC_LCTL)) {\
    register_code(kc); \
  } else { \
    register_code(keycode); \
  } \
} else { \
    unregister_code(kc); \
    unregister_code(keycode); \
} \
return false;
