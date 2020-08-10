/* Copyright 2020 marksard, modified by @kylpo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 /*
 * Notes from kylpo:
 * Since this board only has a single Shift and Ctrl, by convention,
 *   only KC_LSHIFT, KC_LCTRL are checked against. If you add a Right,
 *   be sure to check using MOD_MASK_SHIFT and such.
 *
 * Also using only L_ because it simplifies code to only needing
 *   register/unregister, and not add_mods/del_mods/send_keyboard_report.
 *   The _mods() functions aren't even documented yet...
 */

#include QMK_KEYBOARD_H

enum layer_number {
  _BASE = 0,
  _ALTERNATE,
};

enum custom_keycodes {
  DOT_CTL = SAFE_RANGE,
  DOT_COLON,
  M_ASTR_1,
  M_SLSH_2,
  M_PLUS_3,
  M_QUO_4,
  M_DQUO_5
};

bool is_shift_pressed = false;

// Defines for layer movement
#define L_ALT MO(_ALTERNATE)

// mod taps
#define KC_DOTCTL  CTL_T(KC_DOT)
#define KC_COMCTL  CTL_T(KC_COMMA)
#define KC_SPCMD  CMD_T(KC_SPC)
#define KC_QUOCMD  CMD_T(KC_QUOT)

// Inverted shift status
#define SHIFT_SWITCH(kc1, kc2) \
if (record->event.pressed) { \
  timer_timeout(); \
  if (lshift || rshift) { \
    unregister_code(KC_LSFT); \
    unregister_code(kc2); \
    register_code(kc2); \
    add_to_prev(kc2); \
  } else { \
    register_code(KC_LSFT); \
    unregister_code(kc1); \
    register_code(kc1); \
    add_to_prev(kc1); \
  } \
} else { \
  unregister_code(kc1); \
  unregister_code(kc2); \
  unreg_prev(); \
  if (lshift || rshift) \
    register_code(KC_LSFT); \
  else \
    unregister_code(KC_LSFT); \
} \
return false;

// https://beta.docs.qmk.fm/using-qmk/guides/keymap#keymap-and-layers-id-keymap-and-layers
//
// Keymap: 32 Layers                   Layer: action code matrix
// -----------------                   ---------------------
// stack of layers                     array_of_action_code[row][column]
//        ____________ precedence               _______________________
//       /           / | high                  / ESC / F1  / F2  / F3   ....
//   31 /___________// |                      /-----/-----/-----/-----
//   30 /___________// |                     / TAB /  Q  /  W  /  E   ....
//   29 /___________/  |                    /-----/-----/-----/-----
//    :   _:_:_:_:_:__ |               :   /LCtrl/  A  /  S  /  D   ....
//    :  / : : : : : / |               :  /  :     :     :     :
//    2 /___________// |               2 `--------------------------
//    1 /___________// |               1 `--------------------------
//    0 /___________/  V low           0 `--------------------------
//
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 
 * ,----------------------------------.           ,----------------------------------.
 * |      |   d  |   h  |   c  |      |           |      |   l  |   s  |   r  |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   f  |   a  |   e  |   i  |   u  |           |   m  |   n  |   t  |   o  |   w  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   .  |   p  |   g  |   v  |   x  |           |   j  |   k  |   y  |   b  |  SPC |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   z  |      |      |      | SHFT |           |  ALT |      |      |      |   q  |
 * `----------------------------------'           `----------------------------------'
 */
 /* 
 * ,----------------------------------.           ,----------------------------------.
 * |      |   D  |   H  |   C  |      |           |      |   L  |   S  |   R  |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   F  |   A  |   E  |   I  |   U  |           |   M  |   N  |   T  |   O  |   W  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   :  |   P  |   G  |   V  |   X  |           |   J  |   K  |   Y  |   B  |  RET |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   Z  |      |      |      | ▓▓▓▓ |           |  ALT |      |      |      |   Q  |
 * `----------------------------------'           `----------------------------------'
 */
  [_BASE] = LAYOUT( \
  // TODO: remove RESET when done coding
  //,---------------------------------------------------------------------------------------------------.
         RESET/*XXXXXXX*/,     KC_D,     KC_H,     KC_C,  XXXXXXX,  XXXXXXX,     KC_L,     KC_S,     KC_R,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
          L_ALT/*KC_F*/,     KC_A,     KC_E,     KC_I,     KC_U,     KC_M,     KC_N,     KC_T,     KC_O,     KC_W,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
       DOT_CTL,     KC_P,     KC_G,     KC_V,     KC_X,     KC_J,     KC_K,     KC_Y,     KC_B, KC_SPCMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
          KC_Z,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_LSFT,    L_ALT,  XXXXXXX,  XXXXXXX,  XXXXXXX,     KC_Q
  //,---------------------------------------------------------------------------------------------------.
  ),

/* 
 * ,----------------------------------.           ,----------------------------------.
 * |      |   *  |   /  |   +  |      |           |      |  ESC |  UP  |  ENT |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   !  |   '  |   "  |   -  |   (  |           |   )  | LEFT | DOWN | RIGHT|   @  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ,  |   &  |   |  |   =  |   [  |           |   ]  | BKSP |   ;  |  DEL |   _  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ?  |      |      |      | SHFT |           | ▓▓▓▓ |      |      |      |   `  |
 * `----------------------------------'           `----------------------------------'
 */
 /* 
 * ALT - SHIFT
 * ,----------------------------------.           ,----------------------------------.
 * |      |   1  |   2  |   3  |      |           |      |  ESC |  UP  |  ENT |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   0  |   4  |   5  |   4  |   {  |           |   }  | LEFT | DOWN | RIGHT|   #  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ;  |   7  |   8  |   9  |   <  |           |   >  | BKSP |   %  |  DEL |   ~  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ^  |      |      |      | ▓▓▓▓ |           | ▓▓▓▓ |      |      |      |   \  |
 * `----------------------------------'           `----------------------------------'
 */
  [_ALTERNATE] = LAYOUT( \
  //,---------------------------------------------------------------------------------------------------.
       XXXXXXX, M_ASTR_1, M_SLSH_2, M_PLUS_3,  XXXXXXX,  XXXXXXX,   KC_ESC,    KC_UP,   KC_ENT,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
       KC_LBRC,     KC_4, M_DQUO_5,     KC_6,  KC_RBRC,  KC_MINS,  KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_GRV,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
     KC_COMCTL,     KC_7,     KC_8,     KC_9,     KC_0,   KC_EQL,  KC_BSPC,  KC_SCLN,   KC_DEL,KC_QUOCMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
      KC_SLASH,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_BSLS
  //,---------------------------------------------------------------------------------------------------.
  )

};

#define maybe_add_weak_mods(keycode, mod)                               \
  if (keycode < QK_MODS_MAX &&                                          \
      (keycode & 0xff00) == QK_ ## mod)                                 \
    add_weak_mods(MOD_BIT(KC_ ## mod))

static bool alternate_modifier(uint16_t modifier, uint16_t keycode, keyrecord_t *record) {
  static bool in_alternate_modifier;

  /* when it's a key press and modifier state is pressed */
  if (record->event.pressed && (get_mods() & MOD_BIT(modifier))) {
    in_alternate_modifier = true;

    /* will send modifier up so that the os won't shift the keycode we will send */
    del_mods(MOD_BIT(modifier));

    /* send mods if keycode needs it */
    maybe_add_weak_mods(keycode, LCTL);
    maybe_add_weak_mods(keycode, LSFT);
    maybe_add_weak_mods(keycode, LALT);
    maybe_add_weak_mods(keycode, LGUI);
    maybe_add_weak_mods(keycode, RCTL);
    maybe_add_weak_mods(keycode, RSFT);
    maybe_add_weak_mods(keycode, RALT);
    maybe_add_weak_mods(keycode, RGUI);

    /* send mods modifications */
    send_keyboard_report();

    /* send alternate key code */
    register_code(keycode);

    /* we changed the internal state by releasing the modifier key, marked against
       as pressed so that we are back in the real state */
    add_mods(MOD_BIT(modifier));

    return false;
  }

  /* when releasing the key and we activated alternate modifier */
  if (!record->event.pressed && in_alternate_modifier) {
    in_alternate_modifier = false;

    /* release the alternate key */
    unregister_code(keycode);

    /* make sure all mods we sat up earlier are released */
    clear_weak_mods();

    /* send mods modification */
    send_keyboard_report();

    return false;
  }


  return true;
}

static void alternate_modifier_basic(uint8_t modifier_mask, uint16_t to_keycode, uint16_t from_keycode, keyrecord_t *record) {
  if(record->event.pressed) {
    if (get_mods() & modifier_mask){
      del_mods(modifier_mask);
      register_code(to_keycode);
      add_mods(modifier_mask);
    } else {
      register_code(from_keycode);
    }
  } else {
    unregister_code(to_keycode);
    unregister_code(from_keycode);
  }
}

static void alternate_ctl(uint16_t to_keycode, uint16_t from_keycode, keyrecord_t *record) {
  alternate_modifier_basic(MOD_BIT(KC_LCTL), to_keycode, from_keycode, record);
}

static void inverted_shift_key(uint16_t keycode_when_shift, uint16_t keycode_when_not_shift, uint8_t *mods, keyrecord_t *record) {
  if (record->event.pressed) {
    *mods = get_mods() & MOD_MASK_SHIFT; // 0 if no shift, positive if shift pressed

    if (*mods) {
      del_mods(*mods); // Remove any Shifts present
      send_keyboard_report(); // send mods modifications
      register_code(keycode_when_shift);
    } else {
      register_code16(S(keycode_when_not_shift));
    }
  } else {
    if (*mods) {
      unregister_code(keycode_when_shift);
      add_mods(*mods);
    } else {
      unregister_code16(S(keycode_when_not_shift));
    }
  }
}

// static void shift_unshift(uint16_t keycode_when_shift, uint16_t keycode_when_not_shift, uint8_t *mods, keyrecord_t *record) {
//   if (record->event.pressed) {
//     *mods = get_mods() & MOD_MASK_SHIFT; // 0 if no shift, positive if shift pressed

//     if (*mods) {
//       del_mods(*mods); // Remove any Shifts present
//       send_keyboard_report(); // send mods modifications
//       register_code(keycode_when_shift);
//     } else {
//       register_code16(S(keycode_when_not_shift));
//     }
//   } else {
//     if (*mods) {
//       unregister_code(keycode_when_shift);
//       add_mods(*mods);
//     } else {
//       unregister_code16(S(keycode_when_not_shift));
//     }
//   }
// }

// shift_normal
// shift_all
// shift_switch

// Different keycode when Ctrl is pressed


/*
  Notes/docs:

  SEND_STRING() - type out a string (C-level preprocessor macro)
  register_code(KC_) - send KC_ keydown event
  unregister_code(KC_) - send KC_ keyup event
  tap_code(KC_) - same as register_code(KC_); unregister_code(KC_);
  clear_keyboard() - clear all mods and keys currently pressed
  clear_mods() - clear all mods currently pressed

  return `true` to indicate to the caller that the key press we just processed should continue to be processed as normal

  See also: action_util.c
  https://github.com/qmk/qmk_firmware/blob/master/tmk_core/common/action_util.c
*/

// Key macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // `static` will retain the value between separate calls of the function
  static uint16_t dot_ctl_timer;
  static bool isClicking = false;
  
  bool result = false;
  switch (keycode) {
    case KC_LSFT: {
      if (record->event.pressed){
        is_shift_pressed = true;
      } else {
        is_shift_pressed = false;
      }
      result = true;

      break;
    }
    case KC_DOT:
      // if (record->event.pressed){
      //   if (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT)){
      //     register_code(KC_SCLN); // send :
      //   } else {
      //     register_code(KC_DOT); // send .
      //   }
      // } else {
      //   unregister_code(KC_SCLN);
      //   unregister_code(KC_DOT);
      // }
      // Only keeping this around for now to make Make happy about unused `alternate_modifier`
      result = alternate_modifier(KC_LSFT, KC_COLON, record);
      break;
    case DOT_CTL:
      // mod-tap with different shift value.
      // See https://thomasbaart.nl/2018/12/09/qmk-basics-tap-and-hold-actions/
      // and https://github.com/gavinenns/qmk_firmware/blob/773dbdb095d4f48f39ce6ca1c0a9cb49a4cd1a52/keyboards/planck/keymaps/gavinenns/keymap.c#L158
      if(record->event.pressed) {
        dot_ctl_timer = timer_read();
        register_code(KC_LCTL); // Change the key to be held here
      } else {
        unregister_code(KC_LCTL); // Change the key that was held here, too!
        if (timer_elapsed(dot_ctl_timer) < TAPPING_TERM) {
          if (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT)){
            tap_code(KC_SCLN); // :
          } else {
            tap_code(KC_DOT); // .
          }
        }
      }
      break;

    case KC_I: {
      if(record->event.pressed) {
        // CTRL modifier makes this a mouse click
        if (get_mods() & MOD_BIT(KC_LCTL)) {
          isClicking = true;

          unregister_code(KC_LCTL);
          send_keyboard_report(); // send mods modifications
          register_code(KC_MS_BTN1);
        }
        // Otherwise, handle as normal alpha key
        else {
          register_code(keycode);
        }
      } else {
        if (isClicking) {
          unregister_code(KC_MS_BTN1);

          register_code(KC_LCTL);

          isClicking = false;
        } else {
          unregister_code(keycode);
        }
      }
      break;
    }

    // Mouse movement when CTL held for S,T,N,O
    case KC_S:
      alternate_ctl(KC_MS_UP, keycode, record);
      // result = alternate_modifier(KC_LCTL, KC_MS_UP, record);
      break;
    case KC_T:
      alternate_ctl(KC_MS_DOWN, keycode, record);
      // result = alternate_modifier(KC_LCTL, KC_MS_DOWN, record);
      break;
    case KC_N:
      alternate_ctl(KC_MS_LEFT, keycode, record);
      // result = alternate_modifier(KC_LCTL, KC_MS_LEFT, record);
      break;
    case KC_O:
      alternate_ctl(KC_MS_RIGHT, keycode, record);
      // result = alternate_modifier(KC_LCTL, KC_MS_RIGHT, record);
      break;
    
    // Mouse scroll when CTL held for M,J
    case KC_M:
      alternate_ctl(KC_MS_WH_UP, keycode, record);
      // result = alternate_modifier(KC_LCTL, KC_MS_WH_UP, record);
      break;
    case KC_J:
      alternate_ctl(KC_MS_WH_DOWN, keycode, record);
      // result = alternate_modifier(KC_LCTL, KC_MS_WH_DOWN, record);
      break;
    
    // Alt-layer
    // Simple register/unregisters
    case M_ASTR_1: {
      static bool m_astr_1_shifted = false;
    
      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          m_astr_1_shifted = true;
          unregister_code(KC_LSHIFT);
          register_code(KC_1);
        } else {
          register_code16(S(KC_8));
        }
      } else {
        if (m_astr_1_shifted) {
          unregister_code(KC_1);
          if (is_shift_pressed) {
            register_code(KC_LSHIFT);
          }
        } else {
          unregister_code16(S(KC_8));
        }
      }

      break;
    }
    case M_SLSH_2: {
      static uint8_t saved_mods_astr_1 = 0;

      // PLACEHOLDER
      inverted_shift_key(KC_9, KC_2, &saved_mods_astr_1, record);

      break;
    }
    // uses del_mods and add_mods, along with holding on to mods in a variable
    case M_PLUS_3: {
      static uint8_t saved_mods_plus_3 = 0;

      if (record->event.pressed) {
        saved_mods_plus_3 = get_mods() & MOD_MASK_SHIFT; // 0 if no shift, positive if shift pressed

        if (saved_mods_plus_3) {
          del_mods(saved_mods_plus_3);
          send_keyboard_report();
          register_code(KC_3);
        } else {
          register_code16(S(KC_EQL));
        }
      } else {
        if (saved_mods_plus_3) {
          unregister_code(KC_3);
          if (is_shift_pressed) {
            add_mods(saved_mods_plus_3);
          }
        } else {
          unregister_code16(S(KC_EQL));
        }
      }
      break;
    }
    // using SEND_STRING macro
    case M_DQUO_5: {
      static bool m_dquo_5_shifted = false;

      if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
          m_dquo_5_shifted = true;
          unregister_code(KC_LSHIFT);
          SEND_STRING("5");
        } else {
          SEND_STRING("\"");
        }
      } else {
        if (m_dquo_5_shifted && is_shift_pressed) {
          register_code(KC_LSHIFT);
        }
      }
      break;
    }

    default:
      result = true;
      break;
  }

  return result;
}
