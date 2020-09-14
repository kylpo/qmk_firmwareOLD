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

enum layer_number { _BASE = 0, _ALTERNATE, _MOUSE };

enum custom_keycodes { R3_C1 = SAFE_RANGE, R3_C10, A_R3_C1, A_R3_C10, A_R3_C2, A_R1_C2, A_R1_C3, A_R1_C4, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, A_R4_C1, A_R4_C10, A_R3_C3, A_R3_C4, A_R3_C5, A_R3_C6, A_R3_C8, A_R2_C10, A_R2_C1, A_R1_C7, A_R1_C9 };

enum { CT_SE, CT_CLN, CT_EGG, CT_FLSH, X_TAP_DANCE, ALT_MOUSE };

enum combos { NTO, AEI };

// Defines for layer movement
#define L_ALT MO(_ALTERNATE)
#define L_MOUSE MO(_MOUSE)

const uint16_t PROGMEM nto_combo[] = {KC_N, KC_T, KC_O, COMBO_END};
const uint16_t PROGMEM aei_combo[] = {KC_A, KC_E, KC_I, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {[AEI] = COMBO_ACTION(aei_combo), [NTO] = COMBO_ACTION(nto_combo)};

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
    [_BASE] = LAYOUT(  // TODO: remove RESETs when done coding
                       //,---------------------------------------------------------------------------------------------------.
        RESET /*XXXXXXX*/, KC_D, KC_H, KC_C, XXXXXXX, XXXXXXX, KC_L, KC_S, KC_R, RESET /*XXXXXXX*/,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        KC_F, KC_A, KC_E, KC_I, KC_U, KC_M, KC_N, KC_T, KC_O, KC_W,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        R3_C1, KC_P, KC_G, KC_V, KC_X, KC_J, KC_K, KC_Y, KC_B, R3_C10,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        KC_Z, XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, L_ALT, XXXXXXX, XXXXXXX, XXXXXXX, KC_Q
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
    [_ALTERNATE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, A_R1_C2, A_R1_C3, A_R1_C4, XXXXXXX, XXXXXXX, A_R1_C7, KC_UP, A_R1_C9, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        A_R2_C1, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, KC_LEFT, KC_DOWN, KC_RGHT, A_R2_C10,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        A_R3_C1, A_R3_C2, A_R3_C3, A_R3_C4, A_R3_C5, A_R3_C6, KC_BSPC, A_R3_C8, KC_DEL, A_R3_C10,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        A_R4_C1, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, A_R4_C10
        //,---------------------------------------------------------------------------------------------------.
        ),

    /*
     * Mouse
     * ,----------------------------------.           ,----------------------------------.
     * |      |      |SCR ^ |      |      |           |      |      |  UP  |      |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |      |SCR v | CLICK|      |           |      | LEFT | DOWN | RIGHT|      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |      |      |      |      |           |      |      |      |      |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |      |      | ▓▓▓▓ | SHFT |           |  ALT |      |      |      |      |
     * `----------------------------------'           `----------------------------------'
     */
    [_MOUSE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, XXXXXXX, KC_WH_U, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MS_U, XXXXXXX, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        XXXXXXX, KC_BTN2, KC_WH_D, KC_BTN1, XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        _______, KC_WH_L, KC_BTN3, KC_WH_R, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
        //,---------------------------------------------------------------------------------------------------.
        )};

// Key macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool is_clicking           = false;
    static bool is_shift_down         = false;
    static bool is_ctl_down           = false;
    static bool is_cmd_down           = false;
    static bool has_ctl_been_used     = false;
    static bool has_cmd_been_used     = false;
    static bool should_reenable_mouse = false;

    // if CTL+keycode was used, make sure CTL's tap value isn't sent
    if (is_ctl_down && !has_ctl_been_used && keycode != R3_C1 && keycode != A_R3_C1) {
        has_ctl_been_used = true;
    }

    // if CMD+keycode was used, make sure CMD's tap value isn't sent
    if (is_cmd_down && !has_cmd_been_used && keycode != R3_C10 && keycode != A_R3_C10) {
        has_cmd_been_used = true;
    }

    switch (keycode) {
        // Since we have an Alt layer with shift-inverted keys that unregister shift,
        //   track whether the shift key is pressed separately from get_mods().
        // Also, ctrl + shift speeds up mouse movement.
        case KC_LSFT: {
            if (record->event.pressed) {
                is_shift_down = true;
                register_code(KC_ACL2);
            } else {
                is_shift_down = false;
                unregister_code(KC_ACL2);
            }

            // Do not send Shift during a click
            return !is_clicking;
        }

        case L_ALT: {
            DISABLE_MOUSE();
            return true;
        }

        // -------------------------------------------
        //   Base layer
        // -------------------------------------------

        // tap: Dot
        // shift+tap: Colon
        // hold: CTL
        case R3_C1: {
            // if (IS_LAYER_ON(_MOUSE)) {
            //     layer_off(_MOUSE);
            //     should_reenable_mouse = true;
            // }
            static uint16_t r3_c1_timer;

            if (record->event.pressed) {
                r3_c1_timer = timer_read();
                register_code(KC_LCTL);  // hold
                is_ctl_down       = true;
                has_ctl_been_used = false;

                if (IS_LAYER_ON(_MOUSE)) {
                    layer_off(_MOUSE);
                    should_reenable_mouse = true;
                }
            } else {
                unregister_code(KC_LCTL);
                is_ctl_down = false;
                if (!has_ctl_been_used && !should_reenable_mouse && timer_elapsed(r3_c1_timer) < TAPPING_TERM) {
                    if (get_mods() & MOD_BIT(KC_LSHIFT)) {
                        tap_code(KC_SCLN);  // shift + tap
                    } else {
                        tap_code(KC_DOT);  // tap
                    }
                } else if (should_reenable_mouse) {
                    layer_on(_MOUSE);
                    should_reenable_mouse = false;
                }
            }
            return false;
        }
        // tap: Space
        // shift+tap: Underscore
        // hold: CMD
        case R3_C10: {
            static uint16_t r3_c10_timer;

            if (record->event.pressed) {
                r3_c10_timer = timer_read();
                register_code(KC_LGUI);  // hold
                is_cmd_down       = true;
                has_cmd_been_used = false;

                if (IS_LAYER_ON(_MOUSE)) {
                    layer_off(_MOUSE);
                    should_reenable_mouse = true;
                }
            } else {
                unregister_code(KC_LGUI);
                is_cmd_down = false;
                if (!has_cmd_been_used && !should_reenable_mouse && timer_elapsed(r3_c10_timer) < TAPPING_TERM) {
                    if (get_mods() & MOD_BIT(KC_LSHIFT)) {
                        tap_code(KC_MINUS);  // shift + tap
                    } else {
                        tap_code(KC_SPACE);  // tap
                    }
                } else if (should_reenable_mouse) {
                    layer_on(_MOUSE);
                    should_reenable_mouse = false;
                }
            }
            return false;
        }

        // Enable mouse-layer when CTL T,Y,E
        case KC_T: {
            if (record->event.pressed && is_ctl_down) {
                // layer_on(_MOUSE);
                ENABLE_MOUSE();
                return false;
            }
            return true;
        }

        case KC_Y: {
            if (record->event.pressed && is_ctl_down) {
                // layer_on(_MOUSE);
                ENABLE_MOUSE();
            }

            // Return true to pass CTRL+Y to OS, even if Mouse layer was enabled
            return true;
        }

        case KC_E: {
            if (record->event.pressed && is_ctl_down) {
                // layer_on(_MOUSE);
                ENABLE_MOUSE();
                return false;
            }

            // Return true to pass CTRL+Y to OS, even if Mouse layer was enabled
            return true;
        }

            // Mouse click on CTRL + i
        // case KC_I: {
        //     if (record->event.pressed) {
        //         // CTRL modifier makes this a mouse click
        //         if (get_mods() & MOD_BIT(KC_LCTL)) {
        //             is_clicking = true;
        //             register_code(KC_MS_BTN1);
        //         }
        //         // Otherwise, handle as normal alpha key
        //         else {
        //             register_code(keycode);
        //         }
        //     } else {
        //         if (is_clicking) {
        //             unregister_code(KC_MS_BTN1);
        //             is_clicking = false;
        //         } else {
        //             unregister_code(keycode);
        //         }
        //     }
        //     return false;
        // }

        // Media
        case KC_U: {
            WHEN_CTRL(KC__VOLUP)
        }
        case KC_X: {
            WHEN_CTRL(KC__VOLDOWN)
        }

        // -------------------------------------------
        //   Mouse layer
        // -------------------------------------------
        case KC_BTN1: {
            if (record->event.pressed) {
                is_clicking = true;
            } else {
                is_clicking = false;
            }
            return true;
        }

        // -------------------------------------------
        //   Alt layer
        // -------------------------------------------

        // tap: Comma
        // shift+tap: Semicolon
        // hold: CTL
        case A_R3_C1: {
            static uint16_t a_r3_c1_timer;

            if (record->event.pressed) {
                a_r3_c1_timer = timer_read();
                register_code(KC_LCTL);  // hold
                is_ctl_down       = true;
                has_ctl_been_used = false;
            } else {
                unregister_code(KC_LCTL);
                is_ctl_down = false;
                if (!has_ctl_been_used && timer_elapsed(a_r3_c1_timer) < TAPPING_TERM) {
                    if (get_mods() & MOD_BIT(KC_LSHIFT)) {
                        unregister_code(KC_LSHIFT);
                        tap_code(KC_SCLN);  // shift + tap
                        register_code(KC_LSHIFT);
                    } else {
                        tap_code(KC_COMMA);  // tap
                    }
                }
            }
            return false;
        }
        // tap: Underscore
        // shift+tap: Tilda
        // hold: CMD
        case A_R3_C10: {
            static uint16_t a_r3_c10_timer;

            if (record->event.pressed) {
                a_r3_c10_timer = timer_read();
                register_code(KC_LGUI);  // hold
                is_cmd_down       = true;
                has_cmd_been_used = false;
            } else {
                unregister_code(KC_LGUI);
                is_cmd_down = false;
                if (!has_cmd_been_used && timer_elapsed(a_r3_c10_timer) < TAPPING_TERM) {
                    if (get_mods() & MOD_BIT(KC_LSHIFT)) {
                        tap_code(KC_GRAVE);  // shift + tap
                    } else {
                        tap_code(KC_ENTER);  // tap
                    }
                }
            }
            return false;
        }
        case A_R1_C2: {
            static bool a_r1_c2_timer = false;
            ALT_SHIFT(SEND_STRING("*"), SEND_STRING("1"), a_r1_c2_timer)
        }
        case A_R1_C3: {
            static bool a_r1_c3_timer = false;
            ALT_SHIFT(SEND_STRING("/"), SEND_STRING("2"), a_r1_c3_timer)
        }
        case A_R1_C4: {
            static bool a_r1_c4_timer = false;
            ALT_SHIFT(SEND_STRING("+"), SEND_STRING("3"), a_r1_c4_timer)
        }
        case A_R2_C1: {
            static bool a_r1_c1_timer = false;
            ALT_SHIFT(SEND_STRING("`"), SEND_STRING("0"), a_r1_c1_timer)
        }
        case A_R2_C2: {
            static bool a_r2_c2_timer = false;
            ALT_SHIFT(SEND_STRING("'"), SEND_STRING("4"), a_r2_c2_timer)
        }
        case A_R2_C3: {
            static bool a_r2_c3_timer = false;
            ALT_SHIFT(SEND_STRING("\""), SEND_STRING("5"), a_r2_c3_timer)
        }
        case A_R2_C4: {
            static bool a_r2_c4_timer = false;
            ALT_SHIFT(SEND_STRING("-"), SEND_STRING("6"), a_r2_c4_timer)
        }
        case A_R2_C5: {
            static bool a_r2_c5_timer = false;
            ALT_SHIFT(SEND_STRING("("), SEND_STRING("{"), a_r2_c5_timer)
        }
        case A_R2_C6: {
            static bool a_r2_c6_timer = false;
            ALT_SHIFT(SEND_STRING(")"), SEND_STRING("}"), a_r2_c6_timer)
        }
        case A_R4_C1: {
            static bool a_r4_c1_timer = false;
            ALT_SHIFT(SEND_STRING("?"), SEND_STRING("^"), a_r4_c1_timer)
        }
        case A_R3_C2: {
            static bool a_r3_c2_timer = false;
            ALT_SHIFT(SEND_STRING("!"), SEND_STRING("7"), a_r3_c2_timer)
        }
        case A_R3_C3: {
            static bool a_r3_c3_timer = false;
            ALT_SHIFT(SEND_STRING("|"), SEND_STRING("8"), a_r3_c3_timer)
        }
        case A_R3_C4: {
            static bool a_r3_c4_timer = false;
            ALT_SHIFT(SEND_STRING("="), SEND_STRING("9"), a_r3_c4_timer)
        }
        case A_R3_C5: {
            static bool a_r3_c5_timer = false;
            ALT_SHIFT(SEND_STRING("["), SEND_STRING("<"), a_r3_c5_timer)
        }
        case A_R3_C6: {
            static bool a_r3_c6_timer = false;
            ALT_SHIFT(SEND_STRING("]"), SEND_STRING(">"), a_r3_c6_timer)
        }
        case A_R3_C8: {
            static bool a_r3_c8_timer = false;
            ALT_SHIFT(SEND_STRING("$"), SEND_STRING("\%"), a_r3_c8_timer)
        }
        case A_R2_C10: {
            static bool a_r2_c10_timer = false;
            ALT_SHIFT(SEND_STRING("@"), SEND_STRING("#"), a_r2_c10_timer)
        }
        case A_R4_C10: {
            static bool a_r4_c10_timer = false;
            ALT_SHIFT(SEND_STRING("&"), SEND_STRING("\\"), a_r4_c10_timer)
        }
        case A_R1_C7: {
            if (record->event.pressed) {
                if (is_shift_down) {
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
        case A_R1_C9: {
            static bool a_r1_c9_timer = false;
            ALT_SHIFT(SEND_STRING(SS_TAP(X_ENT)), SEND_STRING(SS_TAP(X_TAB)), a_r1_c9_timer);
        }

        default: {
            return true;
        }
    }
}

void process_combo_event(uint8_t combo_index, bool pressed) {
    // switch(combo_index) {
    //   case AEI: {

    //   }
    // }
    if (pressed) {
        ENABLE_MOUSE();
        // if (IS_LAYER_ON(_MOUSE)) {
        //     layer_off(_MOUSE);
        // } else {
        //     layer_on(_MOUSE);
        // }
    }
}

// use Capslock to toggle MOUSE mode
void led_set_user(uint8_t usb_led) {
#ifdef CONSOLE_ENABLE
    uprintf("usb_led: %u\n", usb_led);
#endif

    if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
        layer_on(_MOUSE);
    } else {
        layer_off(_MOUSE);
    }
}

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    // debug_matrix = true;
    // debug_keyboard=true;
    // debug_mouse=true;
}