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

enum custom_keycodes { R4_C6 = SAFE_RANGE, R3_C1, R3_C10, A_R3_C1, A_R3_C10, A_R3_C2, R4_C5, A_R4_C5, A_R1_C2, A_R1_C3, A_R1_C4, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, A_R4_C1, A_R4_C10, A_R3_C3, A_R3_C4, A_R3_C5, A_R3_C6, A_R3_C8, A_R2_C10, A_R2_C1, A_R1_C7, A_R1_C9, M_R4_C6 };

enum combos { NTO, AEI };

const uint16_t PROGMEM aei_combo[] = {KC_A, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM nto_combo[] = {KC_N, KC_T, KC_O, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {[AEI] = COMBO(aei_combo, KC_X), [NTO] = COMBO(nto_combo, KC_J)};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ,----------------------------------.           ,----------------------------------.
     * |      |   d  |   h  |   c  |      |           |      |   l  |   r  |   s  |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   p  |   a  |   e  |   i  |   x  |           |   j  |   o  |   t  |   n  |   b  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   q  |   f  |   u  |   g  |      |           |      |   w  |   m  |   y  |   z  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   v  |      |      |      |  DOT |           |  SPC |      |      |      |   k  |
     * `----------------------------------'           `----------------------------------'
     *
     * SHIFT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   D  |   H  |   C  |      |           |      |   L  |   R  |   S  |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   P  |   A  |   E  |   I  |   X  |           |   J  |   O  |   T  |   N  |   B  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   Q  |   F  |   U  |   G  |      |           |      |   W  |   M  |   Y  |   Z  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   V  |      |      |      | ▓▓▓▓ |           |   _  |      |      |      |   K  |
     * `----------------------------------'           `----------------------------------'
     */
    [_BASE] = LAYOUT(  // TODO: remove RESETs when done coding
                       //,---------------------------------------------------------------------------------------------------.
        RESET /*XXXXXXX*/, KC_D, KC_H, KC_C, XXXXXXX, XXXXXXX, KC_L, KC_R, KC_S, RESET /*XXXXXXX*/,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        KC_P, KC_A, KC_E, KC_I, KC_X, KC_J, KC_O, KC_T, KC_N, KC_B,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        R3_C1, KC_F, KC_U, KC_G, XXXXXXX, XXXXXXX, KC_W, KC_M, KC_Y, R3_C10,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        KC_V, XXXXXXX, XXXXXXX, XXXXXXX, R4_C5, R4_C6, XXXXXXX, XXXXXXX, XXXXXXX, KC_K
        //,---------------------------------------------------------------------------------------------------.
        ),

    /*
     * ALT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   *  |   /  |   +  |      |           |      |  ESC |  UP  |  ENT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   `  |   '  |   "  |   -  |   &  |           |   |  | LEFT | DOWN | RIGHT|   @  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   (  |   ?  |   !  |   =  |      |           |      | BKSP |   ;  |  DEL |   )  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   [  |      |      |      |   ,  |           | ▓▓▓▓ |      |      |      |   ]  |
     * `----------------------------------'           `----------------------------------'
     *
     * ALT - SHIFT                     v-- temporary ^ addition
     * ,----------------------------------.           ,----------------------------------.
     * |      |   1  |   2  |   3  |   ^  |           |      |  ESC |  UP  |  ENT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   0  |   4  |   5  |   4  |   ~  |           |   \  | LEFT | DOWN | RIGHT|   #  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   {  |   7  |   8  |   9  |      |           |      |   $  |   :  |   %  |   }  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   <  |      |      |      | ▓▓▓▓ |           | ▓▓▓▓ |      |      |      |   >  |
     * `----------------------------------'           `----------------------------------'
     */
    [_ALTERNATE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, A_R1_C2, A_R1_C3, A_R1_C4, KC_CIRC, XXXXXXX, A_R1_C7, KC_UP, A_R1_C9, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        A_R2_C1, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, KC_LEFT, KC_DOWN, KC_RGHT, A_R2_C10,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        A_R3_C1, A_R3_C2, A_R3_C3, A_R3_C4, XXXXXXX, XXXXXXX, KC_BSPC, KC_SCLN, KC_DEL, A_R3_C10,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        A_R4_C1, XXXXXXX, XXXXXXX, XXXXXXX, A_R4_C5, _______, XXXXXXX, XXXXXXX, XXXXXXX, A_R4_C10
        //,---------------------------------------------------------------------------------------------------.
        ),

    /*
     * Mouse
     * ,----------------------------------.           ,----------------------------------.
     * |      |      |SCR ^ |      |      |           |      |      |  UP  |      |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |      |SCR v | CLICK|      |           |      | LEFT | DOWN | RIGHT|      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |< SCR |      | SCR >|      |           |      |      |      |      |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |      |      | ▓▓▓▓ | SHFT |           |  ALT |      |      |      |      |
     * `----------------------------------'           `----------------------------------'
     */
    [_MOUSE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, XXXXXXX, KC_WH_U, XXXXXXX, XXXXXXX, XXXXXXX, KC_F19, KC_MS_U, KC_F18, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        XXXXXXX, KC_BTN2, KC_WH_D, KC_BTN1, XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        _______, KC_WH_L, KC_BTN3, KC_WH_R, XXXXXXX, XXXXXXX, KC_F15, KC_F16, KC_F17, _______,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
        //,---------------------------------------------------------------------------------------------------.
        )};

// Key macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool is_clicking           = false;
    static bool is_ctl_down           = false;
    static bool is_cmd_down           = false;
    static bool is_shift_down         = false;
    static bool is_alt_down           = false;
    static bool has_ctl_been_used     = false;
    static bool has_cmd_been_used     = false;
    static bool has_shift_been_used   = false;
    static bool has_alt_been_used     = false;
    static bool should_reenable_mouse = false;

    // if CTL+keycode was used, make sure CTL's tap value isn't sent
    if (is_ctl_down && !has_ctl_been_used && keycode != R3_C1 && keycode != A_R3_C1) {
        has_ctl_been_used = true;
    }

    // if CMD+keycode was used, make sure CMD's tap value isn't sent
    if (is_cmd_down && !has_cmd_been_used && keycode != R3_C10 && keycode != A_R3_C10) {
        has_cmd_been_used = true;
    }

    // if SHIFT+keycode was used, make sure SHIFT's tap value isn't sent
    if (is_shift_down && !has_shift_been_used && keycode != R4_C5 && keycode != A_R4_C5) {
        has_shift_been_used = true;
    }

    // if ALT+keycode was used, make sure ALT's tap value isn't sent
    if (is_alt_down && !has_alt_been_used && keycode != R4_C6) {
        has_alt_been_used = true;
    }

    switch (keycode) {
        // -------------------------------------------
        //   Base layer
        // -------------------------------------------
        // tap: (
        // shift+tap: {
        // hold: CTL
        case R3_C1: {
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
                if (!has_ctl_been_used && timer_elapsed(r3_c1_timer) < TAPPING_TERM && !should_reenable_mouse) {
                    tap_code(KC_Q);
                } else if (should_reenable_mouse) {
                    REENABLE_MOUSE();
                    should_reenable_mouse = false;
                }
            }
            return false;
        }
        // tap: z
        // shift+tap: Z
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
                if (!has_cmd_been_used && timer_elapsed(r3_c10_timer) < TAPPING_TERM && !should_reenable_mouse) {
                    tap_code(KC_Z);
                } else if (should_reenable_mouse) {
                    REENABLE_MOUSE();
                    should_reenable_mouse = false;
                }
            }
            return false;
        }

        // tap: .
        // hold: SHIFT
        case R4_C5: {
            static uint16_t r4_c5_timer;

            if (record->event.pressed) {
                r4_c5_timer = timer_read();
                register_code(KC_LSHIFT);  // hold
                is_shift_down       = true;
                has_shift_been_used = false;

            } else {
                unregister_code(KC_LSHIFT);
                is_shift_down = false;
                if (!has_shift_been_used && timer_elapsed(r4_c5_timer) < TAPPING_TERM) {
                    tap_code(KC_DOT);
                }
            }
            return false;
        }

        // tap: Space
        // shift+tap: Underscore
        // hold: ALT
        case R4_C6: {
            static uint16_t r4_c6_timer;
            static bool     r4_c6_did_disable_mouse = false;

            if (record->event.pressed) {
                r4_c6_timer = timer_read();
                layer_on(_ALTERNATE);  // hold
                is_alt_down       = true;
                has_alt_been_used = false;

                if (IS_LAYER_ON(_MOUSE)) {
                    DISABLE_MOUSE();
                    r4_c6_did_disable_mouse = true;
                }

            } else {
                layer_off(_ALTERNATE);
                is_alt_down = false;
                if (!has_alt_been_used && !r4_c6_did_disable_mouse && timer_elapsed(r4_c6_timer) < TAPPING_TERM) {
                    if (get_mods() & MOD_BIT(KC_LSHIFT)) {
                        tap_code(KC_MINUS);  // shift + tap
                    } else {
                        tap_code(KC_SPACE);  // tap
                    }
                }
                r4_c6_did_disable_mouse = false;  // reset bool
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

        // Media
        case KC_G: {
            WHEN_CTRL(KC__VOLUP, KC_MEDIA_NEXT_TRACK)
        }
        case KC_F: {
            WHEN_CTRL(KC__VOLDOWN, KC_MEDIA_PREV_TRACK)
        }
        case KC_U: {
            WHEN_CTRL(KC__MUTE, KC_MEDIA_PLAY_PAUSE)
        }

        // -------------------------------------------
        //   Mouse layer
        // -------------------------------------------
        case KC_LSFT: {
            if (record->event.pressed) {
                is_shift_down = true;
                // shift speeds up mouse movement.
                register_code(KC_ACL2);
            } else {
                is_shift_down = false;
                unregister_code(KC_ACL2);
            }

            // Do not send Shift during a click
            return !is_clicking;
        }

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

        // tap: (
        // shift+tap: {
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
                        // unregister_code(KC_LSHIFT);
                        tap_code(KC_LBRACKET);  // shift + tap
                        // register_code(KC_LSHIFT);
                    } else {
                        tap_code16(S(KC_9));  // tap
                    }
                }
            }
            return false;
        }
        // tap: )
        // shift+tap: }
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
                        tap_code(KC_RBRACKET);  // shift + tap
                    } else {
                        tap_code16(S(KC_0));  // tap
                    }
                }
            }
            return false;
        }

        // tap: ,
        // hold: SHIFT
        case A_R4_C5: {
            static uint16_t a_r4_c5_timer;

            if (record->event.pressed) {
                a_r4_c5_timer = timer_read();
                register_code(KC_LSHIFT);  // hold
                is_shift_down       = true;
                has_shift_been_used = false;

            } else {
                unregister_code(KC_LSHIFT);
                is_shift_down = false;
                if (!has_shift_been_used && timer_elapsed(a_r4_c5_timer) < TAPPING_TERM) {
                    tap_code(KC_COMMA);
                }
            }
            return false;
        }

        case A_R1_C2: {
            static bool is_a_r1_c2_shifted = false;
            ALT_SHIFT(SEND_STRING("*"), SEND_STRING("1"), is_a_r1_c2_shifted)
        }
        case A_R1_C3: {
            static bool is_a_r1_c3_shifted = false;
            ALT_SHIFT(SEND_STRING("/"), SEND_STRING("2"), is_a_r1_c3_shifted)
        }
        case A_R1_C4: {
            static bool is_a_r1_c4_shifted = false;
            ALT_SHIFT(SEND_STRING("+"), SEND_STRING("3"), is_a_r1_c4_shifted)
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
            static bool is_a_r1_c9_shifted = false;
            ALT_SHIFT(SEND_STRING(SS_TAP(X_ENT)), SEND_STRING(SS_TAP(X_TAB)), is_a_r1_c9_shifted);
        }
        case A_R2_C1: {
            static bool is_a_r1_c1_shifted = false;
            ALT_SHIFT(SEND_STRING("`"), SEND_STRING("0"), is_a_r1_c1_shifted)
        }
        case A_R2_C2: {
            static bool is_a_r2_c2_shifted = false;
            ALT_SHIFT(SEND_STRING("'"), SEND_STRING("4"), is_a_r2_c2_shifted)
        }
        case A_R2_C3: {
            static bool is_a_r2_c3_shifted = false;
            ALT_SHIFT(SEND_STRING("\""), SEND_STRING("5"), is_a_r2_c3_shifted)
        }
        case A_R2_C4: {
            static bool is_a_r2_c4_shifted = false;
            ALT_SHIFT(SEND_STRING("-"), SEND_STRING("6"), is_a_r2_c4_shifted)
        }
        case A_R2_C5: {
            static bool is_a_r2_c5_shifted = false;
            ALT_SHIFT(SEND_STRING("&"), SEND_STRING("~"), is_a_r2_c5_shifted)
        }
        case A_R2_C6: {
            static bool is_a_r2_c6_shifted = false;
            ALT_SHIFT(SEND_STRING("|"), SEND_STRING("\\"), is_a_r2_c6_shifted)
        }
        case A_R2_C10: {
            static bool is_a_r2_c10_shifted = false;
            ALT_SHIFT(SEND_STRING("@"), SEND_STRING("#"), is_a_r2_c10_shifted)
        }
        case A_R3_C2: {
            static bool is_a_r3_c2_shifted = false;
            ALT_SHIFT(SEND_STRING("?"), SEND_STRING("7"), is_a_r3_c2_shifted)
        }
        case A_R3_C3: {
            static bool is_a_r3_c3_shifted = false;
            ALT_SHIFT(SEND_STRING("!"), SEND_STRING("8"), is_a_r3_c3_shifted)
        }
        case A_R3_C4: {
            static bool is_a_r3_c4_shifted = false;
            ALT_SHIFT(SEND_STRING("="), SEND_STRING("9"), is_a_r3_c4_shifted)
        }
        case KC_BSPC: {
            if (is_shift_down) {
                if (record->event.pressed) {
                    register_code(KC_4);
                } else {
                    unregister_code(KC_4);
                }
                return false;
            }
            return true;
        }
        case KC_DEL: {
            if (is_shift_down) {
                if (record->event.pressed) {
                    register_code(KC_5);
                } else {
                    unregister_code(KC_5);
                }
                return false;
            }
            return true;
        }
        case A_R4_C1: {
            static bool is_a_r4_c1_shifted = false;
            ALT_SHIFT(SEND_STRING("["), SEND_STRING("<"), is_a_r4_c1_shifted)
        }
        case A_R4_C10: {
            static bool is_a_r4_c10_shifted = false;
            ALT_SHIFT(SEND_STRING("]"), SEND_STRING(">"), is_a_r4_c10_shifted)
        }
        default: {
            return true;
        }
    }
}

// use Capslock to toggle MOUSE mode
bool led_update_user(led_t led_state) {
#ifdef CONSOLE_ENABLE
    uprintf("usb_led: %u\n", usb_led);
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
    debug_enable = true;
    debug_matrix = true;
// debug_keyboard=true;
// debug_mouse=true;
#endif
}