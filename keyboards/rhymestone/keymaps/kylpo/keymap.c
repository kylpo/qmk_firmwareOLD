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
#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
  _BASE = 0,
  _ALTERNATE,
};

enum custom_keycodes {
  ALTERNATE = SAFE_RANGE,
  DOT_CTL,
  DOT_COLON
};

// Defines for layer movement
// #define LOWER MO(_LOWER)
// #define RAISE MO(_RAISE)


// mod taps
#define KC_DOTCTL  CTL_T(KC_DOT)
#define KC_COMCTL  CTL_T(KC_COMMA)
#define KC_SPCMD  CMD_T(KC_SPC)
#define KC_QUOCMD  CMD_T(KC_QUOT)

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
 * |      |   D  |   H  |   C  |      |           |      |   L  |   S  |   R  |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   F  |   A  |   E  |   I  |   U  |           |   M  |   N  |   T  |   O  |   W  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   .  |   P  |   G  |   V  |   X  |           |   J  |   K  |   Y  |   B  |  SPC |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   Z  |      |      |      | SHFT |           |  ALT |      |      |      |   Q  |
 * `----------------------------------'           `----------------------------------'
 */
  [_BASE] = LAYOUT( \
  //,---------------------------------------------------------------------------------------------------.
       XXXXXXX,     KC_D,     KC_H,     KC_C,  XXXXXXX,  XXXXXXX,     KC_L,     KC_S,     KC_R,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
          KC_F,     KC_A,     KC_E,     KC_I,     KC_U,     KC_M,     KC_N,     KC_T,     KC_O,     KC_W,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
       DOT_CTL,     KC_P,     KC_G,     KC_V,     KC_X,     KC_J,     KC_K,     KC_Y,     KC_B, KC_SPCMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
          KC_Z,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_LSFT,ALTERNATE,  XXXXXXX,  XXXXXXX,  XXXXXXX,     KC_Q
  //,---------------------------------------------------------------------------------------------------.
  ),

/* 
 * ,----------------------------------.           ,----------------------------------.
 * |      |   1  |   2  |   3  |      |           |      |  ESC |  UP  |  ENT |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   [  |   4  |   5  |   4  |   ]  |           |   -  | LEFT | DOWN | RIGHT|   `  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   ,  |   7  |   8  |   9  |   0  |           |   =  | BKSP |   ;  |  DEL |   '  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   /  |      |      |      | SHFT |           |  ALT |      |      |      |   \  |
 * `----------------------------------'           `----------------------------------'
 */
  [_ALTERNATE] = LAYOUT( \
  //,---------------------------------------------------------------------------------------------------.
       XXXXXXX,     KC_1,     KC_2,     KC_3,  XXXXXXX,  XXXXXXX,   KC_ESC,    KC_UP,   KC_ENT,  XXXXXXX,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
       KC_LBRC,     KC_4,     KC_5,     KC_6,  KC_RBRC,  KC_MINS,  KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_GRV,
  //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
     KC_COMCTL,     KC_7,     KC_8,     KC_9,     KC_0,   KC_EQL,  KC_BSPC,  KC_SCLN,   KC_DEL,KC_QUOCMD,
  //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
      KC_SLASH,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_BSLS
  //,---------------------------------------------------------------------------------------------------.
  )
};

#define L_BASE _BASE
#define L_ALTERNATE (1<<_ALTERNATE)


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



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t dot_ctl_timer;
  static uint16_t alternate_timer;
  
  bool result = false;
  switch (keycode) {
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
            // send :
            register_code(KC_SCLN);
            unregister_code(KC_SCLN);
          } else {
            // send .
            register_code(KC_DOT);
            unregister_code(KC_DOT);
          }
        }
      }
      break;

    // case KC_H: {
    //   // if(record->event.pressed) {
    //   //   if (get_mods() & MOD_BIT(KC_LCTL)){

    //   //     del_mods(MOD_BIT(KC_LCTL));
    //   //     register_code(KC_MS_UP);
    //   //     add_mods(MOD_BIT(KC_LCTL));

    //   //   } else {
    //   //     register_code(KC_H);
    //   //   }
    //   // } else {
    //   //   unregister_code(KC_MS_UP);
    //   //   unregister_code(KC_H);
    //   // }

    //   if(record->event.pressed) {
    //     if (get_mods() & MOD_BIT(KC_LCTL)){
    //         // TODO: this isn't working
    //         // del_mods(MOD_BIT(KC_LCTL));

    //         unregister_code(KC_LCTL);

    //         /* send mods modifications */
    //         send_keyboard_report();
            

    //         register_code(KC_MS_BTN1);


    //         /* send mods modifications */
    //         // send_keyboard_report();
    //       }
    //   } else {
    //         unregister_code(KC_MS_BTN1);

    //         register_code(KC_LCTL);
    //   }

    //   // alternate_ctl(KC_MS_BTN1, keycode, record);
    //   // if (get_mods() & MOD_BIT(KC_LCTL)){
    //   //       // TODO: this isn't working
    //   //       // del_mods(MOD_BIT(KC_LCTL));

    //   //       unregister_code(KC_LCTL);

    //   //       /* send mods modifications */
    //   //       send_keyboard_report();
            

    //   //       register_code(KC_MS_BTN1);

    //   //       unregister_code(KC_MS_BTN1);

    //   //       register_code(KC_LCTL);

    //   //       /* send mods modifications */
    //   //       // send_keyboard_report();
    //   // }
    //   break;
    // }

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
    
    // Mouse click on tap, ALT layer on hold
    case ALTERNATE:
      if(record->event.pressed) {
        alternate_timer = timer_read();
        // layer_move(_RAISE)
        layer_on(_ALTERNATE);
      } else {
        // layer_clear()
        layer_off(_ALTERNATE);

        if (timer_elapsed(alternate_timer) < TAPPING_TERM) {
          if (get_mods() & MOD_BIT(KC_LCTL)){
            unregister_code(KC_LCTL);

            /* send mods modifications */
            send_keyboard_report();
            
            register_code(KC_MS_BTN1);
            unregister_code(KC_MS_BTN1);

            register_code(KC_LCTL);
          }
        }
      }
      break;
    default:
      result = true;
      break;
  }

  return result;
}
