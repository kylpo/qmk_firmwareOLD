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

// one key chords
#define CHORD_1 KC_B
#define CHORD_2 KC_C

// ... define other chords here
// two key chords
#define CHORD_1_2 KC_L

void process_chorded_key_value(int value);
bool process_chorded_result(int value);

enum layer_number { _BASE = 0, _ALTERNATE, _MOUSE };

enum custom_keycodes { R4_C6 = SAFE_RANGE, R4_C1, R3_C1, R3_C10, A_R3_C1, A_R3_C10, A_R3_C2, R4_C5, A_R4_C5, A_R1_C2, A_R1_C3, A_R1_C4, A_R1_C5, A_R1_C6, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, A_R4_C1, A_R4_C10, A_R3_C3, A_R3_C4, A_R3_C5, A_R3_C6, A_R3_C8, A_R2_C10, A_R2_C1, A_R1_C7, A_R1_C9, M_R4_C6, ONEPASS, OP1, OP2, OP3 };

#define M_CMD_CLICK LGUI(KC_MS_BTN1)

enum combo_events { ZC_COPY, XV_PASTE, C_Q, C_BSLASH };

const uint16_t PROGMEM copy_combo[]   = {KC_A, KC_E, KC_I, COMBO_END};
const uint16_t PROGMEM paste_combo[]  = {A_R2_C2, A_R2_C3, A_R2_C4, COMBO_END};
const uint16_t PROGMEM q_combo[]      = {KC_O, KC_T, KC_SPACE, COMBO_END};
const uint16_t PROGMEM bslash_combo[] = {KC_LEFT, KC_DOWN, KC_RIGHT, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    [ZC_COPY]  = COMBO_ACTION(copy_combo),
    [XV_PASTE] = COMBO_ACTION(paste_combo),
    [C_Q]      = COMBO_ACTION(q_combo),
    [C_BSLASH] = COMBO_ACTION(bslash_combo),

};

void process_combo_event(uint16_t combo_index, bool pressed) {
#ifdef CONSOLE_ENABLE
    uprintf("combo_index: %u\n", combo_index);
#endif
    switch (combo_index) {
        case ZC_COPY:
            if (pressed) {
                tap_code16(KC_Z);
            }
            break;
        case XV_PASTE:
            if (pressed) {
                tap_code16(KC_V);
            }
            break;
        case C_Q:
            if (pressed) {
                tap_code16(KC_Q);
            }
            break;
        case C_BSLASH:
            if (pressed) {
                tap_code16(KC_BSLASH);
            }
            break;
    }
}

int             chord_value       = 0;
int             first_chord_press = 0;
static uint16_t chord_combo_timer;

static uint16_t chord_value_buffer[MAX_COMBO_LENGTH];
// static uint16_t record_buffer[MAX_COMBO_LENGTH];

static uint8_t buffer_size = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ,----------------------------------.           ,----------------------------------.
     * |      |   l  |   s  |   h  |   z  |           |   q  |   r  |   n  |   c  |      |
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
     * |      |   L  |   S  |   H  |   Z  |           |   Q  |   R  |   N  |   C  |      |
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
        RESET /*XXXXXXX*/, KC_L, KC_S, KC_H, KC_Z, KC_Q, KC_R, KC_N, KC_C, RESET /*XXXXXXX*/,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        KC_G, KC_A, KC_E, KC_I, KC_V, KC_K, KC_O, KC_T, KC_SPACE, KC_Y,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        KC_LCTL, KC_P, KC_U, KC_F, KC_X, KC_J, KC_M, KC_D, KC_W, KC_LCMD,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        R4_C1, XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, R4_C6, XXXXXXX, XXXXXXX, XXXXXXX, KC_B
        //,---------------------------------------------------------------------------------------------------.
        ),

    /*
     * ALT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   *  |   /  |   +  |   ~  |           |  TAB |  ESC |  UP  |  ENT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   `  |   '  |   "  |   -  |   [  |           |   ]  | LEFT | DOWN | RIGHT|   @  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   ?  |   !  |   =  |   &  |           |   |  | BKSP |   ;  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   (  |      |      |      | SHFT |           | ▓▓▓▓ |      |      |      |   )  |
     * `----------------------------------'           `----------------------------------'
     *
     * ALT - SHIFT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   1  |   2  |   3  |   ^  |           |   \  | PREV |  UP  | NEXT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   0  |   4  |   5  |   6  |   <  |           |   >  | LEFT | DOWN | RIGHT|   #  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   7  |   8  |   9  |   $  |           |   %  | BKSP |   :  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   {  |      |      |      | ▓▓▓▓ |           | ▓▓▓▓ |      |      |      |   }  |
     * `----------------------------------'           `----------------------------------'
     */

    /*
     *********************
     * Other layout ideas
     *********************
     * ALT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   *  |   /  |   +  |   ~  |           |  TAB |  ESC |  UP  |  ENT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   `  |   '  |   "  |   -  |   (  |           |   )  | LEFT | DOWN | RIGHT|   @  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   ?  |   !  |   =  |   [  |           |   ]  | BKSP |   ;  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   &  |      |      |      | SHFT |           | ▓▓▓▓ |      |      |      |   |  |
     * `----------------------------------'           `----------------------------------'
     *
     * ALT - SHIFT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   1  |   2  |   3  |   ^  |           |   \  | PREV |  UP  | NEXT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   0  |   4  |   5  |   4  |   {  |           |   }  | LEFT | DOWN | RIGHT|   #  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   7  |   8  |   9  |   <  |           |   >  | BKSP |   :  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   $  |      |      |      | ▓▓▓▓ |           | ▓▓▓▓ |      |      |      |   %  |
     * `----------------------------------'           `----------------------------------'
     *
     *
     *
     * ALT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   *  |   /  |   +  |   ~  |           |  TAB |  ESC |  UP  |  ENT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   `  |   '  |   "  |   -  |   (  |           |   )  | LEFT | DOWN | RIGHT|   @  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   ?  |   !  |   =  |   [  |           |   ]  | BKSP |   |  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   :  |      |      |      | SHFT |           | ▓▓▓▓ |      |      |      |   $  |
     * `----------------------------------'           `----------------------------------'
     *
     * ALT - SHIFT
     * ,----------------------------------.           ,----------------------------------.
     * |      |   1  |   2  |   3  |   ^  |           |   \  | PREV |  UP  | NEXT |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   0  |   4  |   5  |   4  |   {  |           |   }  | LEFT | DOWN | RIGHT|   #  |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |   7  |   8  |   9  |   <  |           |   >  | BKSP |   &  |  DEL |  APP |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |   ;  |      |      |      | ▓▓▓▓ |           | ▓▓▓▓ |      |      |      |   %  |
     * `----------------------------------'           `----------------------------------'
     */
    [_ALTERNATE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, A_R1_C2, A_R1_C3, A_R1_C4, A_R1_C5, A_R1_C6, A_R1_C7, KC_UP, A_R1_C9, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        A_R2_C1, A_R2_C2, A_R2_C3, A_R2_C4, A_R2_C5, A_R2_C6, KC_LEFT, KC_DOWN, KC_RGHT, A_R2_C10,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        _______, A_R3_C2, A_R3_C3, A_R3_C4, A_R3_C5, A_R3_C6, KC_BSPC, KC_SCLN, KC_DEL, _______,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        A_R4_C1, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, A_R4_C10
        //,---------------------------------------------------------------------------------------------------.
        ),

    /*
     * Mouse
     * ,----------------------------------.           ,----------------------------------.
     * |      |      |SCR ^ |      |      |           |      |      |  UP  |      |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      | RCLK |SCR v | CLICK|      |           |      | LEFT | DOWN | RIGHT|      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |  OS  |< SCR | MCLK | SCR >|      |           |      |      |      |      |      |
     * |------+------+------+------+------|           |------+------+------+------+------|
     * |      |      |      | ▓▓▓▓ | SHFT |           |  ALT |      |      |      |      |
     * `----------------------------------'           `----------------------------------'
     */
    [_MOUSE] = LAYOUT(  //,---------------------------------------------------------------------------------------------------.
        XXXXXXX, XXXXXXX, KC_WH_U, XXXXXXX, XXXXXXX, XXXXXXX, KC_F19, KC_MS_U, KC_F18, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        XXXXXXX, KC_BTN2, KC_WH_D, KC_BTN1, XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,
        //|---------+---------+---------+---------+---------+---------+---------+---------+---------+---------|
        _______, KC_WH_L, M_CMD_CLICK, KC_WH_R, XXXXXXX, XXXXXXX, KC_F15, KC_F16, KC_F17, _______,
        //`---------+---------+---------+---------+---------+---------+---------+---------+---------+---------'
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
        //,---------------------------------------------------------------------------------------------------.
        )};

// static uint16_t current_combo_index = 0;
// extern int      COMBO_LEN;

void matrix_scan_user(void) {
    if (timer_elapsed(chord_combo_timer) >= COMBO_TERM && chord_value != 0) {
        bool completed_chord = process_chorded_result(chord_value);

        chord_value       = 0;
        first_chord_press = 0;

        if (buffer_size == 0) {
            return;
        }

        if (!completed_chord) {
            for (uint8_t i = 0; i < buffer_size; i++) {
                process_chorded_key_value(chord_value_buffer[i]);
            }
        }

        // for (current_combo_index = 0; current_combo_index < COMBO_LEN; ++current_combo_index) {
        //     combo_t *combo = &key_combos[current_combo_index];

        //     uint8_t  count = 0;
        //     uint16_t index = -1;

        //     for (uint8_t i = 0; i < buffer_size; i++) {
        //         /* Find index of keycode and number of combo keys */
        //         for (const uint16_t *keys = combo->keys;; ++count) {
        //             uint16_t key = pgm_read_word(&keys[count]);
        //             if (chord_value_buffer[i].keycode == key) index = count;
        //             if (COMBO_END == key) break;
        //         }

        //         /* Continue processing if not a combo key */
        //         if (-1 == (int8_t)index) {
        //             // process_record_user(&(key_buffer[i]), action);
        //             process_chorded_key_value(chord_value_buffer[i]);
        //         }
        //     }
        // }

        buffer_size = 0;
    }
}

// Key macros
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // static bool is_clicking           = false;
    static bool is_ctl_down           = false;
    static bool is_cmd_down           = false;
    static bool is_shift_down         = false;
    static bool is_alt_down           = false;
    static bool has_ctl_been_used     = false;
    static bool has_cmd_been_used     = false;
    static bool has_shift_been_used   = false;
    static bool has_alt_been_used     = false;
    static bool should_reenable_mouse = false;
    // bool        is_combo_key          = false;
    // // drop_buffer                       = false;
    // bool no_combo_keys_pressed = true;

    // for (current_combo_index = 0; current_combo_index < COMBO_LEN; ++current_combo_index) {
    //     combo_t *combo = &key_combos[current_combo_index];
    //     is_combo_key |= process_single_combo(combo, keycode, record);
    //     no_combo_keys_pressed = no_combo_keys_pressed && NO_COMBO_KEYS_ARE_DOWN;
    // }

    print("process_record_user\r");

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
        case KC_LCTL: {
            // static uint16_t r3_c1_timer;

            if (record->event.pressed) {
                // r3_c1_timer = timer_read();
                // register_code(KC_LCTL);  // hold
                is_ctl_down       = true;
                has_ctl_been_used = false;

                if (IS_LAYER_ON(_MOUSE)) {
                    layer_off(_MOUSE);
                    should_reenable_mouse = true;
                }
            } else {
                // unregister_code(KC_LCTL);
                is_ctl_down = false;
                // if (!has_ctl_been_used && timer_elapsed(r3_c1_timer) < TAPPING_TERM && !should_reenable_mouse) {
                //     tap_code(KC_Q);
                // } else
                if (should_reenable_mouse) {
                    REENABLE_MOUSE();
                    should_reenable_mouse = false;
                }
            }
            return true;
        }
        case KC_LCMD: {
            // static uint16_t r3_c10_timer;

            if (record->event.pressed) {
                // r3_c10_timer = timer_read();
                // register_code(KC_LGUI);  // hold
                is_cmd_down       = true;
                has_cmd_been_used = false;

                if (IS_LAYER_ON(_MOUSE)) {
                    layer_off(_MOUSE);
                    should_reenable_mouse = true;
                }
            } else {
                // unregister_code(KC_LGUI);
                is_cmd_down = false;
                // if (!has_cmd_been_used && timer_elapsed(r3_c10_timer) < TAPPING_TERM && !should_reenable_mouse) {
                //     tap_code(KC_Z);
                // } else
                if (should_reenable_mouse) {
                    REENABLE_MOUSE();
                    should_reenable_mouse = false;
                }
            }
            return true;
        }

        case KC_LSFT: {
            // static uint16_t r4_c5_timer;

            if (record->event.pressed) {
                // r4_c5_timer = timer_read();
                // register_code(KC_LSHIFT);  // hold
                is_shift_down       = true;
                has_shift_been_used = false;

                // shift speeds up mouse movement.
                register_code(KC_ACL2);
            } else {
                // unregister_code(KC_LSHIFT);
                unregister_code(KC_ACL2);
                is_shift_down = false;
                // if (!has_shift_been_used && timer_elapsed(r4_c5_timer) < TAPPING_TERM) {
                //     tap_code(KC_DOT);
                // }
            }
            return true;
        }

        // tap: .
        // shift+tap: ,
        case R4_C1: {
            NORM_SHIFT_EVENT(".", ",")
        }

        // hold: ALT
        case R4_C6: {
            // static uint16_t r4_c6_timer;
            // static bool r4_c6_did_disable_mouse = false;

            if (record->event.pressed) {
                // r4_c6_timer = timer_read();
                layer_on(_ALTERNATE);  // hold
                is_alt_down       = true;
                has_alt_been_used = false;

                if (IS_LAYER_ON(_MOUSE)) {
                    DISABLE_MOUSE();
                    // r4_c6_did_disable_mouse = true;
                }

            } else {
                layer_off(_ALTERNATE);
                is_alt_down = false;
                // if (!has_alt_been_used && !r4_c6_did_disable_mouse && timer_elapsed(r4_c6_timer) < TAPPING_TERM) {
                //     if (get_mods() & MOD_BIT(KC_LSHIFT)) {
                //         tap_code(KC_MINUS);  // shift + tap
                //     } else {
                //         tap_code(KC_SPACE);  // tap
                //     }
                // }
                // r4_c6_did_disable_mouse = false;  // reset bool
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
        case KC_P: {
            WHEN_CTRL(KC__VOLDOWN, KC_MEDIA_PREV_TRACK)
        }
        case KC_U: {
            WHEN_CTRL(KC__MUTE, KC_MEDIA_PLAY_PAUSE)
        }
        case KC_F: {
            WHEN_CTRL(KC__VOLUP, KC_MEDIA_NEXT_TRACK)
        }

            // -------------------------------------------
            //   Mouse layer
            // -------------------------------------------
            // case KC_LSFT: {
            //     if (record->event.pressed) {
            //         is_shift_down = true;
            //         // shift speeds up mouse movement.
            //         register_code(KC_ACL2);
            //     } else {
            //         is_shift_down = false;
            //         unregister_code(KC_ACL2);
            //     }

            //     // Do not send Shift during a click
            //     return !is_clicking;
            // }

        // case KC_ACL2: {
        //     if (record->event.pressed) {
        //         is_shift_down = true;
        //         // shift speeds up mouse movement.
        //         // register_code(KC_ACL2);
        //     } else {
        //         is_shift_down = false;
        //         // unregister_code(KC_ACL2);
        //     }

        //     // Do not send Shift during a click
        //     return true;
        // }

        // case KC_BTN1: {
        //     if (record->event.pressed) {
        //         is_clicking = true;
        //     } else {
        //         is_clicking = false;
        //     }
        //     return true;
        // }

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

            // // tap: ,
            // // hold: SHIFT
            // case A_R4_C5: {
            //     static uint16_t a_r4_c5_timer;

            //     if (record->event.pressed) {
            //         a_r4_c5_timer = timer_read();
            //         register_code(KC_LSHIFT);  // hold
            //         is_shift_down       = true;
            //         has_shift_been_used = false;

            //     } else {
            //         unregister_code(KC_LSHIFT);
            //         is_shift_down = false;
            //         if (!has_shift_been_used && timer_elapsed(a_r4_c5_timer) < TAPPING_TERM) {
            //             tap_code(KC_COMMA);
            //         }
            //     }
            //     return false;
            // }

        case A_R1_C2: {
            NORM_SHIFT_EVENT("*", "1");
        }
        case A_R1_C3: {
            NORM_SHIFT_EVENT("/", "2");
        }
        case A_R1_C4: {
            NORM_SHIFT_EVENT("+", "3");
        }
        // case A_R1_C5: {
        //     NORM_SHIFT_EVENT("~", "^");
        // }
        // case A_R1_C6: {
        //     NORM_SHIFT_EVENT(SS_TAP(X_TAB), "\\");
        // }
        case A_R1_C7: {
            if (record->event.pressed) {
                if (is_shift_down) {
                    SEND_STRING(SS_TAP(X_TAB));

                } else {
                    SEND_STRING(SS_TAP(X_ESC));

                    register_code(KC_ESC);
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
            if (record->event.pressed) {
                // register_code16(KC_QUOTE);

                if (first_chord_press == 0) {
                    // then we start, and must set first_chord_press to 1
                    first_chord_press = 1;
                    chord_value       = 0;
                    chord_combo_timer = timer_read();
                }
                if (timer_elapsed(chord_combo_timer) < COMBO_TERM) {
                    chord_value                       = chord_value + 1;
                    chord_value_buffer[buffer_size++] = 1;
                }
            }

            return false;
            // else {
            //     unregister_code16(KC_QUOTE);
            // }
            // return true;
            // static bool is_a_r2_c2_shifted = false;
            // ALT_SHIFT(SEND_STRING("'"), SEND_STRING("4"), is_a_r2_c2_shifted)
        }
        case A_R2_C3: {
            if (record->event.pressed) {
                if (first_chord_press == 0) {
                    // then we start, and must set first_chord_press to 1
                    first_chord_press = 1;
                    chord_value       = 0;
                    chord_combo_timer = timer_read();
                }
                if (timer_elapsed(chord_combo_timer) < COMBO_TERM) {
                    chord_value                       = chord_value + 2;
                    chord_value_buffer[buffer_size++] = 2;
                }
            }

            return false;
        }
        // case A_R2_C3: {
        //     static bool is_a_r2_c3_shifted = false;
        //     ALT_SHIFT(SEND_STRING("\""), SEND_STRING("5"), is_a_r2_c3_shifted)
        // }
        case A_R2_C4: {
            if (record->event.pressed) {
                if (first_chord_press == 0) {
                    // then we start, and must set first_chord_press to 1
                    first_chord_press = 1;
                    chord_value       = 0;
                    chord_combo_timer = timer_read();
                }
                if (timer_elapsed(chord_combo_timer) < COMBO_TERM) {
                    chord_value                       = chord_value + 4;
                    chord_value_buffer[buffer_size++] = 4;
                }
            }

            return false;
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
            NORM_SHIFT_EVENT("?", "7");
        }
        case A_R3_C3: {
            NORM_SHIFT_EVENT("!", "8");
        }
        case A_R3_C4: {
            NORM_SHIFT_EVENT("=", "9");
        }
        case A_R3_C5: {
            NORM_SHIFT_EVENT("&", "$");
        }
        case A_R3_C6: {
            NORM_SHIFT_EVENT("|", "%");
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

void process_chorded_key_value(int value) {
    if (value == 1) {
        // chord 1
        NORM_SHIFT("'", "4")
    } else if (value == 2) {
        // chord 2
        NORM_SHIFT("\"", "5")
    } else if (value == 4) {
        // chord 3
        NORM_SHIFT("-", "6")
    } else {
        // unknown chord, possibly incomplete.
    }
}

bool process_chorded_result(int value) {
    switch (value) {
        case 7:
            if (keyboard_report->mods & MOD_BIT(KC_LSFT)) {
                SEND_STRING(SS_UP(X_LSFT) "^" SS_DOWN(X_LSFT));
            } else {
                SEND_STRING("~");
            }

            return true;

        default:
            return false;
    }
}

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    debug_enable = true;
    // debug_matrix   = true;
    // debug_keyboard = true;
// debug_mouse=true;
#endif
}