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

enum layer_number {
  _BASE = 0,
  _ALTERNATE,
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
  M_TICK
};

// Defines for layer movement
#define L_ALT MO(_ALTERNATE)

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
  // TODO: remove RESET when done coding
  //,---------------------------------------------------------------------------------------------------.
         RESET/*XXXXXXX*/,     KC_D,     KC_H,     KC_C,  XXXXXXX,  XXXXXXX,     KC_L,     KC_S,     KC_R,  RESET/*XXXXXXX*/,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
          KC_F,     KC_A,     KC_E,     KC_I,     KC_U,     KC_M,     KC_N,     KC_T,     KC_O,     KC_W,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
     M_DOT_CTL,     KC_P,     KC_G,     KC_V,     KC_X,     KC_J,     KC_K,     KC_Y,     KC_B, M_SPC_CMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
          KC_Z,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_LSFT,    L_ALT,  XXXXXXX,  XXXXXXX,  XXXXXXX,     KC_Q
  //,---------------------------------------------------------------------------------------------------.
  ),

/*
 * ALT 
 * ,----------------------------------.           ,----------------------------------.
 * |      |   *  |   /  |   +  |      |           |      |  ESC |  UP  |  ENT |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   !  |   '  |   "  |   -  |   (  |           |   )  | LEFT | DOWN | RIGHT|   @  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ,  |   &  |   |  |   =  |   [  |           |   ]  | BKSP |   $  |  DEL |   _  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ?  |      |      |      | SHFT |           | ▓▓▓▓ |      |      |      |   `  |
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
       XXXXXXX,   M_ASTR,   M_SLSH,   M_PLUS,  XXXXXXX,  XXXXXXX,   KC_ESC,    KC_UP,   KC_ENT,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        M_EXLM,    M_QUO,   M_DQUO,   M_MINS,   M_LPRN,   M_RPRN,  KC_LEFT,  KC_DOWN,  KC_RGHT,     M_AT,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
     M_COM_CTL,   M_AMPR,   M_PIPE,  M_EQUAL,   M_LBRC,   M_RBRC,  KC_BSPC,    M_DLR,   KC_DEL,M_UND_CMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        M_QUES,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,    M_TICK
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
  static bool is_accelerated = false;
  static bool is_shift_key_pressed = false;

  switch (keycode) {
    // Since we have an Alt layer with shift-inverted keys that unregister shift,
    //   track whether the shift key is pressed separately from get_mods().
    // Also, ctrl + shift speeds up mouse movement.
    case KC_LSFT: {
      if (record->event.pressed){
        is_shift_key_pressed = true;

        if (get_mods() & MOD_BIT(KC_LCTL)) {
          is_accelerated = true;
          register_code(KC_ACL2);
        }
      } else {
        is_shift_key_pressed = false;

        if (is_accelerated) {
          unregister_code(KC_ACL2);
        }
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
      } else {
        unregister_code(KC_LCTL);
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
    // Mouse click on CTRL + i
    case KC_I: {
      if(record->event.pressed) {
        // CTRL modifier makes this a mouse click
        if (get_mods() & MOD_BIT(KC_LCTL)) {
          is_clicking = true;

          unregister_code(KC_LCTL);
          send_keyboard_report(); // send mods modifications
          register_code(KC_MS_BTN1);
        }
        // Otherwise, handle as normal alpha key
        else {
          register_code(keycode);
        }
      } else {
        if (is_clicking) {
          unregister_code(KC_MS_BTN1);

          register_code(KC_LCTL);

          is_clicking = false;
        } else {
          unregister_code(keycode);
        }
      }
      return false;
    }

    // Mouse movement when CTL held for S,T,N,O
    case KC_S: {
      WHEN_CTRL(KC_MS_UP)
    }
    case KC_T: {
      WHEN_CTRL(KC_MS_DOWN)
    }
    case KC_N: {
      WHEN_CTRL(KC_MS_LEFT)
    }
    case KC_O: {
      WHEN_CTRL(KC_MS_RIGHT)
    }

    // Mouse scroll when CTL held for H,E
    case KC_H: {
      WHEN_CTRL(KC_MS_WH_UP)
    }
    case KC_E: {
      WHEN_CTRL(KC_MS_WH_DOWN)
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
      } else {
        unregister_code(KC_LCTL);
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
    case M_EXLM: {
      static bool m_exlm_shifted = false;
      ALT_SHIFT(SEND_STRING("!"), SEND_STRING("0"), m_exlm_shifted)
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
    case M_AMPR: {
      static bool m_ampr_shifted = false;
      ALT_SHIFT(SEND_STRING("&"), SEND_STRING("7"), m_ampr_shifted)
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
    case M_TICK: {
      static bool m_tick_shifted = false;
      ALT_SHIFT(SEND_STRING("`"), SEND_STRING("\\"), m_tick_shifted)
    }
    default: {
      return true;
    }
  }
}
