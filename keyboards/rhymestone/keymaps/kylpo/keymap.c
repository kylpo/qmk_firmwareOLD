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
 *   be sure to check using MOD_MASK_SHIFT and MOD_MASK_CTRL.
 *
 * Also using only L_ because it simplifies code to only needing
 *   register/unregister, and not add_mods/del_mods/send_keyboard_report.
 *   The _mods() functions aren't even documented yet...
 */

#include QMK_KEYBOARD_H
#include "keymap.h"

typedef struct {
  bool is_press_action;
  uint8_t state;
} tap;

enum layer_number {
  _BASE = 0,
  _ALTERNATE,
  _MOUSE
};

enum custom_keycodes {
  M_DOT_CTL = SAFE_RANGE,
  M_SPC_CMD,
  M_COM_CTL,
  M_UND_CMD,
  M_EXLM,
  M_ASTR,
  M_SLSH,
  M_PLUS,
  M_QUO,
  M_DQUO,
  M_MINS,
  M_LPRN,
  M_RPRN,
  M_QUES,
  M_AMPR,
  M_PIPE,
  M_EQUAL,
  M_LBRC,
  M_RBRC,
  M_DLR,
  M_AT,
  M_TICK,
  M_ESC,
  M_ENT
};

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD,
  DOUBLE_TAP,
  DOUBLE_HOLD
};

// Create a global instance of the tapdance state type
static td_state_t td_state;

enum {
  CT_SE,
  CT_CLN,
  CT_EGG,
  CT_FLSH,
  X_TAP_DANCE,
  ALT_MOUSE
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
uint8_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_data);

// Defines for layer movement
// #define L_ALT MO(_ALTERNATE)
#define L_ALT TD(ALT_MOUSE)
#define L_MOUSE MO(_MOUSE)

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
 *
 * SHIFT
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
  // TODO: remove RESETs when done coding
  //,---------------------------------------------------------------------------------------------------.
         RESET/*XXXXXXX*/,     KC_D,     KC_H,     KC_C,  XXXXXXX,  XXXXXXX,     KC_L,     KC_S,     KC_R,  RESET/*XXXXXXX*/,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
          KC_F,     KC_A,     KC_E,     KC_I,     KC_U,     KC_M,     KC_N,     KC_T,     KC_O,     KC_W,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
     M_DOT_CTL,     KC_P,     KC_G,     KC_V,     KC_X,     KC_J,     KC_K,     KC_Y,     KC_B, M_SPC_CMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
          KC_Z,  XXXXXXX,  XXXXXXX,  TG(_MOUSE),  KC_LSFT,    L_ALT,  L_MOUSE,  XXXXXXX,  XXXXXXX,     KC_Q
  //,---------------------------------------------------------------------------------------------------.
  ),

/*
 * ALT 
 * ,----------------------------------.           ,----------------------------------.
 * |      |   *  |   /  |   +  |      |           |      |  ESC |  UP  |  ENT |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   `  |   '  |   "  |   -  |   (  |           |   )  | LEFT | DOWN | RIGHT|   @  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ,  |   !  |   |  |   =  |   [  |           |   ]  | BKSP |   $  |  DEL |   _  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ?  |      |      |      | SHFT |           | ▓▓▓▓ |      |      |      |   &  |
 * `----------------------------------'           `----------------------------------'
 *
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
       XXXXXXX,   M_ASTR,   M_SLSH,   M_PLUS,  XXXXXXX,  XXXXXXX,    M_ESC,    KC_UP,    M_ENT,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        M_TICK,    M_QUO,   M_DQUO,   M_MINS,   M_LPRN,   M_RPRN,  KC_LEFT,  KC_DOWN,  KC_RGHT,     M_AT,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
     M_COM_CTL,   M_EXLM,   M_PIPE,  M_EQUAL,   M_LBRC,   M_RBRC,  KC_BSPC,    M_DLR,   KC_DEL,M_UND_CMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        M_QUES,  XXXXXXX,  XXXXXXX,  _______,  _______,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,    M_AMPR
  //,---------------------------------------------------------------------------------------------------.
  ),

/*
 * Mouse 
 * ,----------------------------------.           ,----------------------------------.
 * |      |      |      |      |      |           |      |      |  UP  |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      | CLICK|      |           |      | LEFT | DOWN | RIGHT|      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      | ▓▓▓▓ | SHFT |           |  ALT |      |      |      |      |
 * `----------------------------------'           `----------------------------------'
 *
 * ALT - Mouse
 * ,----------------------------------.           ,----------------------------------.
 * |      |   1  |   2  |   3  |      |           |      |  ESC |  UP  |  ENT |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   0  |   4  |   5  |   4  |   {  |           |   }  | LEFT | DOWN | RIGHT|   #  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ;  |   7  |   8  |   9  |   <  |           |   >  | BKSP |   %  |  DEL |   ~  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ^  |      |      | ▓▓▓▓ | SHFT |           | ▓▓▓▓ |      |      |      |   \  |
 * `----------------------------------'           `----------------------------------'
 */
  [_MOUSE] = LAYOUT( \
  //,---------------------------------------------------------------------------------------------------.
       XXXXXXX,  XXXXXXX,  KC_WH_U,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MS_U,  XXXXXXX,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
       XXXXXXX,  KC_ACL1,  KC_WH_D,  KC_BTN1,  XXXXXXX,  XXXXXXX,  KC_MS_L,  KC_MS_D,  KC_MS_R,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
       _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
       XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,  _______,  XXXXXXX,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX
  //,---------------------------------------------------------------------------------------------------.
  )
};

// Key macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t dot_mod_tap_timer;
  static uint16_t spc_mod_tap_timer;
  static uint16_t com_mod_tap_timer;
  static uint16_t und_mod_tap_timer;
  static bool is_clicking = false;
  // static bool is_accelerated = false;
  static bool is_shift_key_pressed = false;
  // Track ctrl being pressed for when mouse dragging occurs,
  // and we still want to be able to move the mouse.
  static bool is_ctl_key_pressed = false;

  switch (keycode) {
    // Since we have an Alt layer with shift-inverted keys that unregister shift,
    //   track whether the shift key is pressed separately from get_mods().
    // Also, ctrl + shift speeds up mouse movement.
    case KC_LSFT: {
      if (record->event.pressed){
        is_shift_key_pressed = true;

        // if (is_ctl_key_pressed) {
          // is_accelerated = true;
          register_code(KC_ACL2);
        // }
      } else {
        is_shift_key_pressed = false;

        // if (is_accelerated) {
          unregister_code(KC_ACL2);
        // }
      }
      return true;
    }

    // ------------------------------------------- 
    //   Base layer
    // ------------------------------------------- 

    // tap: Dot
    // shift+tap: Colon
    // hold: CTL
    case M_DOT_CTL: {
      if(record->event.pressed) {
        dot_mod_tap_timer = timer_read();
        register_code(KC_LCTL); // hold
        is_ctl_key_pressed = true;

        // set acceleration when shift is already held
        // if (is_shift_key_pressed) {
          // register_code(KC_ACL2);
          // is_accelerated = true;
        // }
      } else {
        unregister_code(KC_LCTL);
        is_ctl_key_pressed = false;
        if (timer_elapsed(dot_mod_tap_timer) < TAPPING_TERM) {
          if (get_mods() & MOD_BIT(KC_LSHIFT)){
            tap_code(KC_SCLN); // shift + tap
          } else {
            tap_code(KC_DOT); // tap
          }
        }
      }
      return false;
    }
    // tap: Space
    // shift+tap: Return
    // hold: CMD
    case M_SPC_CMD: {
      if(record->event.pressed) {
        spc_mod_tap_timer = timer_read();
        register_code(KC_LGUI); // hold
      } else {
        unregister_code(KC_LGUI);
        if (timer_elapsed(spc_mod_tap_timer) < TAPPING_TERM) {
          if (get_mods() & MOD_BIT(KC_LSHIFT)){
            unregister_code(KC_LSHIFT);
            tap_code(KC_ENTER); // shift + tap
            register_code(KC_LSHIFT);
          } else {
            tap_code(KC_SPACE); // tap
          }
        }
      }
      return false;
    }
    // // Mouse click on CTRL + i
    // case KC_I: {
    //   if(record->event.pressed) {
    //     // CTRL modifier makes this a mouse click
    //     if (get_mods() & MOD_BIT(KC_LCTL)) {
    //       is_clicking = true;

    //       unregister_code(KC_LCTL);
    //       send_keyboard_report(); // send mods modifications
    //       register_code(KC_MS_BTN1);
    //     }
    //     // Otherwise, handle as normal alpha key
    //     else {
    //       register_code(keycode);
    //     }
    //   } else {
    //     if (is_clicking) {
    //       unregister_code(KC_MS_BTN1);

    //       register_code(KC_LCTL);

    //       is_clicking = false;
    //     } else {
    //       unregister_code(keycode);
    //     }
    //   }
    //   return false;
    // }

    // // Mouse movement when CTL held for S,T,N,O
    // case KC_S: {
    //   WHEN_CTRL(KC_MS_UP)
    // }
    // case KC_T: {
    //   WHEN_CTRL(KC_MS_DOWN)
    // }
    // case KC_N: {
    //   WHEN_CTRL(KC_MS_LEFT)
    // }
    // case KC_O: {
    //   WHEN_CTRL(KC_MS_RIGHT)
    // }

    // // Mouse scroll when CTL held for H,E
    // case KC_H: {
    //   WHEN_CTRL(KC_MS_WH_UP)
    // }
    // case KC_E: {
    //   WHEN_CTRL(KC_MS_WH_DOWN)
    // }

    // Media
    case KC_U: {
      WHEN_CTRL(KC__VOLUP)
    }
    case KC_X: {
      WHEN_CTRL(KC__VOLDOWN)
    }

    // ------------------------------------------- 
    //   Alt layer
    // ------------------------------------------- 

    // tap: Comma
    // shift+tap: Semicolon
    // hold: CTL
    case M_COM_CTL: {
      if(record->event.pressed) {
        com_mod_tap_timer = timer_read();
        register_code(KC_LCTL); // hold
        is_ctl_key_pressed = true;

        // set acceleration when shift is already held
        // if (is_shift_key_pressed) {
          // register_code(KC_ACL2);
          // is_accelerated = true;
        // }
      } else {
        unregister_code(KC_LCTL);
        is_ctl_key_pressed = false;
        if (timer_elapsed(com_mod_tap_timer) < TAPPING_TERM) {
          if (get_mods() & MOD_BIT(KC_LSHIFT)){
            unregister_code(KC_LSHIFT);
            tap_code(KC_SCLN); // shift + tap
            register_code(KC_LSHIFT);
          } else {
            tap_code(KC_COMMA); // tap
          }
        }
      }
      return false;
    }  
    // tap: Underscore
    // shift+tap: Tilda
    // hold: CMD
    case M_UND_CMD: {
      if(record->event.pressed) {
        und_mod_tap_timer = timer_read();
        register_code(KC_LGUI); // hold
      } else {
        unregister_code(KC_LGUI);
        if (timer_elapsed(und_mod_tap_timer) < TAPPING_TERM) {
          if (get_mods() & MOD_BIT(KC_LSHIFT)){
            tap_code(KC_GRAVE); // shift + tap
          } else {
            register_code16(S(KC_MINUS)); // tap
            unregister_code16(S(KC_MINUS));
          }
        }
      }

      return false;
    }
    case M_TICK: {
      static bool m_tick_shifted = false;
      ALT_SHIFT(SEND_STRING("`"), SEND_STRING("0"), m_tick_shifted)
    }
    case M_ASTR: {
      static bool m_astr_shifted = false;
      ALT_SHIFT(SEND_STRING("*"), SEND_STRING("1"), m_astr_shifted)
    }
    case M_SLSH: {
      static bool m_slsh_shifted = false;
      ALT_SHIFT(SEND_STRING("/"), SEND_STRING("2"), m_slsh_shifted)
    }
    case M_PLUS: {
      static bool m_plus_shifted = false;
      ALT_SHIFT(SEND_STRING("+"), SEND_STRING("3"), m_plus_shifted)
    }
    case M_QUO: {
      static bool m_quo_shifted = false;
      ALT_SHIFT(SEND_STRING("'"), SEND_STRING("4"), m_quo_shifted)
    }
    case M_DQUO: {
      static bool m_dquo_shifted = false;
      ALT_SHIFT(SEND_STRING("\""), SEND_STRING("5"), m_dquo_shifted)
    }
    // Right Mouse click on CTRL. - or 6 otherwise.
    case M_MINS: {
      static bool m_mins_shifted = false;

      if(record->event.pressed) {
        // CTRL modifier makes this a mouse click
        if (get_mods() & MOD_BIT(KC_LCTL)) {
          is_clicking = true;
          register_code(KC_MS_BTN1);
        }
        // Otherwise, handle as normal ALT_SHIFT key
        else {
          if (is_shift_key_pressed) {
            m_mins_shifted = true;
            unregister_code(KC_LSHIFT);
            SEND_STRING("6");
          } else {
            SEND_STRING("-");
          }
        }
      } else {
        if (is_clicking) {
          unregister_code(KC_MS_BTN1);
          is_clicking = false;
        } else {
          if (m_mins_shifted && is_shift_key_pressed) { 
            register_code(KC_LSHIFT); 
          }
        }
      }
      return false;
    }
    case M_LPRN: {
      static bool m_lprn_shifted = false;
      ALT_SHIFT(SEND_STRING("("), SEND_STRING("{"), m_lprn_shifted)
    }
    case M_RPRN: {
      static bool m_rprn_shifted = false;
      ALT_SHIFT(SEND_STRING(")"), SEND_STRING("}"), m_rprn_shifted)
    }
    case M_QUES: {
      static bool m_ques_shifted = false;
      ALT_SHIFT(SEND_STRING("?"), SEND_STRING("^"), m_ques_shifted)
    }
    case M_EXLM: {
      static bool m_exlm_shifted = false;
      ALT_SHIFT(SEND_STRING("!"), SEND_STRING("7"), m_exlm_shifted)
    }
    case M_PIPE: {
      static bool m_pipe_shifted = false;
      ALT_SHIFT(SEND_STRING("|"), SEND_STRING("8"), m_pipe_shifted)
    }
    case M_EQUAL: {
      static bool m_equal_shifted = false;
      ALT_SHIFT(SEND_STRING("="), SEND_STRING("9"), m_equal_shifted)
    }
    case M_LBRC: {
      static bool m_lbrc_shifted = false;
      ALT_SHIFT(SEND_STRING("["), SEND_STRING("<"), m_lbrc_shifted)
    }
    case M_RBRC: {
      static bool m_rbrc_shifted = false;
      ALT_SHIFT(SEND_STRING("]"), SEND_STRING(">"), m_rbrc_shifted)
    }
    case M_DLR: {
      static bool m_dlr_shifted = false;
      ALT_SHIFT(SEND_STRING("$"), SEND_STRING("\%"), m_dlr_shifted)
    }
    case M_AT: {
      static bool m_at_shifted = false;
      ALT_SHIFT(SEND_STRING("@"), SEND_STRING("#"), m_at_shifted)
    }
    case M_AMPR: {
      static bool m_ampr_shifted = false;
      ALT_SHIFT(SEND_STRING("&"), SEND_STRING("\\"), m_ampr_shifted)
    }
    case M_ESC: {
      if (record->event.pressed) { 
        if (is_shift_key_pressed) { 
          register_code(KC_TAB);
        } else { 
          register_code(KC_ESC);
        }
      } else { 
        unregister_code(KC_TAB);
        unregister_code(KC_ESC);
      } 
      return false;
    }
    case M_ENT: {
      static bool m_ent_shifted = false;
      ALT_SHIFT(SEND_STRING(SS_TAP(X_ENT)), SEND_STRING(SS_TAP(X_TAB)), m_ent_shifted);
    }
    default: {
      return true;
    }
  }
}

// Determine the current tap dance state
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        // if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // else return SINGLE_HOLD;
      // if (state->pressed) return SINGLE_HOLD;
      // else return SINGLE_TAP;
      return SINGLE_HOLD;
    } else if (state->count == 2) {
      // if (state->pressed) return DOUBLE_HOLD;
      // else return DOUBLE_TAP;
      return DOUBLE_HOLD;
    }
    else return 8;
}

// Initialize tap structure associated with example tap dance key
static tap ql_tap_state = {
    .is_press_action = true,
    .state = 0
};

// Functions that control what our tap dance key does
void ql_finished(qk_tap_dance_state_t *state, void *user_data) {
    // ql_tap_state.state = cur_dance(state);
    td_state = cur_dance(state);

    switch (td_state) {
        case SINGLE_TAP:
            // tap_code(KC_QUOT);
            break;
        case SINGLE_HOLD:
            layer_on(_ALTERNATE);
            break;
        case DOUBLE_TAP:
            // Check to see if the layer is already set
            // if (layer_state_is(_MOUSE)) {
            //     // If already set, then switch it off
            //     layer_off(_MOUSE);
            // } else {
            //     // If not already set, then switch the layer on
            //     layer_on(_MOUSE);
            // }
            // layer_on(_MOUSE);
            break;
        case DOUBLE_HOLD:
            layer_on(_MOUSE);
          break;
    }
}

void ql_reset(qk_tap_dance_state_t *state, void *user_data) {
   switch (td_state) {
        case SINGLE_TAP: break;
        case SINGLE_HOLD: layer_off(_ALTERNATE); break;
        case DOUBLE_TAP: break;
        case DOUBLE_HOLD: layer_off(_MOUSE); break;
    }
    // ql_tap_state.state = 0;
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [ALT_MOUSE] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, ql_finished, ql_reset, 300)
    // [ALT_MOUSE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset)
};
