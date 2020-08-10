#pragma once

#include "quantum.h"

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

/*
// Inspired by https://github.com/qmk/qmk_firmware/tree/master/users/spacebarracecar
// Shifted key is unshifted. Unshifted key is shifted.
#define SHIFT_INVERTED(kc_with_shift, kc_without_shift, shifted) \
if (record->event.pressed) { \
  if (get_mods() & MOD_MASK_SHIFT) { \
    shifted = true; \
    unregister_code(KC_LSHIFT); \
    register_code(kc_with_shift); \
  } else { \
    register_code16(S(kc_without_shift)); \
  } \
} else { \
  if (shifted) { \
    unregister_code(kc_with_shift); \
    if (is_shift_pressed) { \
      register_code(KC_LSHIFT); \
    } \
  } else { \
    unregister_code16(S(kc_without_shift)); \
  } \
} \
return false;

// Variant using mods functions
#define SHIFT_INVERTED_USE_MODS(kc_with_shift, kc_without_shift, shifted) \
if (record->event.pressed) { \
  if (get_mods() & MOD_MASK_SHIFT) { \
    shifted = true; \
    del_mods(MOD_MASK_SHIFT); \
    send_keyboard_report(); \
    register_code(kc_with_shift); \
  } else { \
    register_code16(S(kc_without_shift)); \
  } \
} else { \
  if (shifted) { \
    unregister_code(kc_with_shift); \
    if (is_shift_pressed) { \
      add_mods(get_mods() & MOD_MASK_SHIFT); \
    } \
  } else { \
    unregister_code16(S(kc_without_shift)); \
  } \
} \
return false;
*/