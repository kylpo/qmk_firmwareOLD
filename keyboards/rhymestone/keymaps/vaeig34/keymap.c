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
 *
 * For help on ALT-level COMBOs/Chords, see:
 * - https://github.com/qmk/qmk_firmware/blob/master/quantum/process_keycode/process_combo.c
 * - https://www.reddit.com/r/olkb/comments/cqt9ff/qmk_making_a_chorded_keymap/
 * - https://pastebin.com/ymtS5j5K
 */

#include QMK_KEYBOARD_H
#include "keymap.h"

enum layer_number { _BASE = 0, _ALTERNATE, _MOUSE };
enum custom_keycodes { R4_C6 = SAFE_RANGE, R2_C7, R2_C8, R2_C9, R4_C1, R3_C1, R3_C10, A_R3_C1, A_R3_C10, A_R3_C2, R4_C5, A_R4_C5, A_R1_C2, A_R1_C3, A_R1_C4, A_R3_C5, A_R3_C6, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, A_R4_C1, A_R4_C10, A_R3_C3, A_R3_C4, A_R3_C9, A_R3_C7, A_R3_C8, A_R2_C10, A_R2_C1, A_R1_C7, A_R1_C9, M_R4_C6 };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ,----------------------------------.           ,----------------------------------.
     * |   z  |   l  |   s  |   h  |  DEL |           | BKSP |   r  |   n  |   c  |   q  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   g  |   a  |   e  |   i  |   v  |           |   k  |   o  |   t  |  SPC |   y  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   p  |   u  |   f  |   x  |           |   j  |   m  |   d  |   w  |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   .  |      |      |      |  SFT |           |  ALT |      |      |      |   b  |
     * `----------------------------------'           `----------------------------------'
     *
     * SHIFT
     * ,----------------------------------.           ,----------------------------------.
     * |   Z  |   L  |   S  |   H  |  DEL |           | BKSP |   R  |   N  |   C  |   Q  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   G  |   A  |   E  |   I  |   V  |           |   K  |   O  |   T  |   _  |   Y  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   P  |   U  |   F  |   X  |           |   J  |   M  |   D  |   W  |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   ,  |      |      |      | ▓▓▓▓ |           |  ALT |      |      |      |   B  |
     * `----------------------------------'           `----------------------------------'
     */
    [_BASE] = LAYOUT(  // TODO: remove RESETs when done coding
                       //,---------------------------------------------------------------------------------------------------.
        KC_Z, KC_L, KC_S, KC_H, KC_DEL, KC_BSPC, KC_R, KC_N, KC_C, KC_Q,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        KC_V, KC_A, KC_E, KC_I, KC_G, KC_Y, KC_O, KC_T, KC_SPC_UND, KC_K,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        KC_LCTL, KC_P, KC_F, KC_U, KC_X, KC_J, KC_D, KC_M, KC_W, KC_LCMD,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        R4_C1, XXXXXXX, RESET, XXXXXXX, KC_LSFT, R4_C6, XXXXXXX, RESET, XXXXXXX, KC_B
        //,---------------------------------------------------------------------------------------------------.
        ),

    /*
     * ALT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   *  |   /  |   +  |   ~  |           |  TAB |  ESC |  UP  |  ENT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   `  |   '  |   "  |   -  |   [  |           |   ]  | LEFT | DOWN | RIGHT|   @  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   ?  |   !  |   =  |   &  |           |   |  | BKSP |   :  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   (  |      |      | BSPC | SHFT |           | ▓▓▓▓ |  DEL |      |      |   )  |
     * `----------------------------------'           `----------------------------------'
     *
     * ALT - SHIFT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   1  |   2  |   3  |   ^  |           |   \  | PREV |  UP  | NEXT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   0  |   4  |   5  |   6  |   <  |           |   >  | LEFT | DOWN | RIGHT|   #  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   7  |   8  |   9  |   $  |           |   %  | BKSP |   ;  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   {  |      |      | BSPC | ▓▓▓▓ |           | ▓▓▓▓ |  DEL |      |      |   }  |
     * `----------------------------------'           `----------------------------------'
     */
    [_ALTERNATE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, A_R1_C2, A_R1_C3, A_R1_C4, _______, _______, A_R1_C7, KC_UP, A_R1_C9, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        A_R2_C1, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, KC_LEFT, KC_DOWN, KC_RGHT, A_R2_C10,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        _______, A_R3_C2, A_R3_C3, A_R3_C4, A_R3_C5, A_R3_C6, A_R3_C7, A_R3_C8, A_R3_C9, _______,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        A_R4_C1, XXXXXXX, XXXXXXX, _______, _______, _______, _______, XXXXXXX, XXXXXXX, A_R4_C10
        //,---------------------------------------------------------------------------------------------------.
        ),

    /*
     * Mouse
     * ,----------------------------------.           ,----------------------------------.
     * |      | SPC  |SCR ^ | SCLK |      |           |      | F19  |  UP  | F18  |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      | RCLK |SCR v | CLICK|      |           |      | LEFT | DOWN | RIGHT|      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |< SCR | MCLK | SCR >|      |           |      | F15  | F16  | F17  |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |      |      |      | ACCEL|           |  ALT |      |      |      |      |
     * `----------------------------------'           `----------------------------------'
     */
    [_MOUSE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, KC_SPACE, KC_WH_U, LSFT(KC_MS_BTN1), _______, _______, KC_F19, KC_MS_U, KC_F18, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        XXXXXXX, KC_BTN2, KC_WH_D, KC_BTN1, XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        _______, KC_WH_L, LGUI(KC_MS_BTN1), KC_WH_R, XXXXXXX, XXXXXXX, KC_F15, KC_F16, KC_F17, _______,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_ACL2, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
        //,---------------------------------------------------------------------------------------------------.
        )};

// Key macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool is_ctl_down           = false;
    static bool is_shift_down         = false;
    static bool is_mouse_acl_down     = false;
    static bool should_reenable_mouse = false;
    static bool should_unshift        = false;

    switch (keycode) {
        // -------------------------------------------
        //   Base layer
        // -------------------------------------------
        case KC_LCTL: {
            if (record->event.pressed) {
                is_ctl_down = true;

                if (IS_LAYER_ON(_MOUSE)) {
                    layer_off(_MOUSE);
                    should_reenable_mouse = true;
                }

                if (is_mouse_acl_down && !should_unshift) {
                    register_code(KC_LSHIFT);
                    is_shift_down  = true;
                    should_unshift = true;
                }
            } else {
                is_ctl_down = false;
                if (should_reenable_mouse) {
                    REENABLE_MOUSE();
                    should_reenable_mouse = false;
                }

                if (should_unshift) {
                    should_unshift = false;
                    unregister_code(KC_LSHIFT);
                    is_shift_down = false;
                }
            }
            return true;
        }
        case KC_LCMD: {
            if (record->event.pressed) {
                if (IS_LAYER_ON(_MOUSE)) {
                    layer_off(_MOUSE);
                    should_reenable_mouse = true;
                }

                if (is_mouse_acl_down && !should_unshift) {
                    register_code(KC_LSHIFT);
                    is_shift_down  = true;
                    should_unshift = true;
                }
            } else {
                if (should_reenable_mouse) {
                    REENABLE_MOUSE();
                    should_reenable_mouse = false;
                }

                if (should_unshift) {
                    should_unshift = false;
                    unregister_code(KC_LSHIFT);
                    is_shift_down = false;
                }
            }
            return true;
        }

        case KC_LSFT: {
            if (record->event.pressed) {
                is_shift_down = true;
            } else {
                is_shift_down = false;
            }
            return true;
        }

        case R4_C1: {
            NORM_SHIFT_EVENT(".", ",")
        }

        // hold: ALT
        case R4_C6: {
            if (record->event.pressed) {
                layer_on(_ALTERNATE);  // hold

                if (IS_LAYER_ON(_MOUSE)) {
                    DISABLE_MOUSE();

                    // Mouse ACL is now Shift
                    if (is_mouse_acl_down && !should_unshift) {
                        register_code(KC_LSHIFT);
                        is_shift_down  = true;
                        should_unshift = true;
                    }
                }

            } else {
                layer_off(_ALTERNATE);
            }
            return false;
        }

        // Mouse-layer enables
        case KC_E: {
            if (record->event.pressed && is_ctl_down) {
                ENABLE_MOUSE();
                return false;
            }
            return true;
        }

        // Mod-Overrides
        //
        // CTRL P,F,U => Media
        // CMD H <=> C
        // CMD P <=> V
        // CMD U <=> Z
        case KC_P: {
            WHEN_CTRL(KC__VOLDOWN, KC_MEDIA_PREV_TRACK)
            WHEN_CMD(KC_V)
            return true;
        }
        case KC_F: {
            WHEN_CTRL(KC__MUTE, KC_MEDIA_PLAY_PAUSE)
            return true;
        }
        case KC_U: {
            WHEN_CTRL(KC__VOLUP, KC_MEDIA_NEXT_TRACK)
            WHEN_CMD(KC_Z)
            return true;
        }
        case KC_V: {
            WHEN_CMD(KC_P)
            return true;
        }
        case KC_Z: {
            WHEN_CMD(KC_U)
            return true;
        }
        case KC_H: {
            WHEN_CMD(KC_C)
            return true;
        }
        case KC_C: {
            WHEN_CMD(KC_H)
            return true;
        }

        // -------------------------------------------
        //   Mouse layer
        // -------------------------------------------
        case KC_ACL2: {
            if (record->event.pressed) {
                is_mouse_acl_down = true;
            } else {
                is_mouse_acl_down = false;

                // Unshift if CTL, CMD, or ALT auto-shifted
                if (should_unshift) {
                    should_unshift = false;
                    unregister_code(KC_LSHIFT);
                    is_shift_down = false;
                }
            }
            return true;
        }

        // TODO: see if this works
        // case KC_F19: {
        //     if (record->event.pressed) {
        //         if (is_mouse_acl_down) {
        //             register_code(KC_F14);

        //             return false;
        //         }
        //     } else {
        //         if (is_mouse_acl_down) {
        //             unregister_code(KC_F14);
        //         }
        //     }

        //     return true;
        // }

        // -------------------------------------------
        //   Alt layer
        // -------------------------------------------
        case A_R1_C2: {
            NORM_SHIFT_EVENT("!", "1");
        }
        // [?, 2]
        case A_R1_C3: {
            // NORM_SHIFT_EVENT("?", "2");
            if (record->event.pressed) {
                if (is_shift_down) {
                    register_code(KC_KP_2);
                } else {
                    register_code16(S(KC_SLASH));
                }
            } else {
                if (is_shift_down) {
                    unregister_code(KC_KP_2);
                } else {
                    unregister_code16(S(KC_SLASH));
                }
            }

            return true;
        }
        case A_R1_C4: {
            NORM_SHIFT_EVENT("+", "3");
        }
        case A_R3_C5: {
            NORM_SHIFT_EVENT("~", "^");
        }
        case A_R3_C6: {
            NORM_SHIFT_EVENT("\\", SS_TAP(X_TAB));
        }
        case A_R1_C7: {
            if (record->event.pressed) {
                if (is_shift_down) {
                    SEND_STRING(SS_TAP(X_TAB));

                } else {
                    SEND_STRING(SS_TAP(X_ESC));
                }
            }

            return true;
        }
        case A_R1_C9: {
            NORM_SHIFT_EVENT(SS_TAP(X_ENT), SS_TAP(X_TAB));
        }
        case A_R2_C1: {
            NORM_SHIFT_EVENT("`", "0");
        }
        case A_R2_C2: {
            NORM_SHIFT_EVENT("'", "4");
        }
        // [", 5]
        case A_R2_C3: {
            // NORM_SHIFT_EVENT("\"", "5");
            if (record->event.pressed) {
                if (is_shift_down) {
                    register_code(KC_KP_5);
                } else {
                    register_code16(S(KC_QUOTE));
                }
            } else {
                if (is_shift_down) {
                    unregister_code(KC_KP_5);
                } else {
                    unregister_code16(S(KC_QUOTE));
                }
            }

            return true;
        }
        case A_R2_C4: {
            NORM_SHIFT_EVENT("-", "6")
        }
        case A_R2_C5: {
            NORM_SHIFT_EVENT("[", "<");
        }
        case A_R2_C6: {
            NORM_SHIFT_EVENT("]", ">");
        }
        case A_R2_C10: {
            NORM_SHIFT_EVENT("@", "#");
        }
        case A_R3_C2: {
            NORM_SHIFT_EVENT("*", "7");
        }
        case A_R3_C3: {
            NORM_SHIFT_EVENT("/", "8");
        }
        case A_R3_C4: {
            NORM_SHIFT_EVENT("=", "9");
        }
        case A_R3_C9: {
            NORM_SHIFT_EVENT("&", "$");
        }
        case A_R3_C7: {
            NORM_SHIFT_EVENT("|", "%");
        }
        case A_R3_C8: {
            NORM_SHIFT_EVENT(":", ";");
        }
        case A_R4_C1: {
            NORM_SHIFT_EVENT("(", "{");
        }
        case A_R4_C10: {
            NORM_SHIFT_EVENT(")", "}");
        }
        default: {
            return true;
        }
    }
}

// use Capslock to toggle MOUSE mode
bool led_update_user(led_t led_state) {
#ifdef CONSOLE_ENABLE
    uprintf("led_state: %u\n", led_state);
#endif

    if (led_state.caps_lock) {
        layer_on(_MOUSE);
    } else {
        layer_off(_MOUSE);
    }

    return true;
}

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;
// debug_mouse=true;
#endif
}