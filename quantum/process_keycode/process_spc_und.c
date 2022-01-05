/* Copyright 2020
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
#include "process_spc_und.h"

static bool spc_und_was_shifted = false;

bool process_spc_und(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_SPC_UND) {
        const uint8_t mods    = get_mods();
        uint8_t       shifted = mods & MOD_MASK_SHIFT;

        if (record->event.pressed) {
            dprint("process_spc_und::pressed");

            spc_und_was_shifted = shifted;
            add_key(shifted ? KC_MINUS : KC_SPACE);
        } else {
            dprint("process_spc_und::released");

            // del_key(spc_und_was_shifted ? KC_MINUS : KC_SPACE);
            del_key(KC_MINUS);
            del_key(KC_SPACE);
        }

        send_keyboard_report();
        return false;
    }

    // Not a grave keycode so continue processing
    return true;
}
