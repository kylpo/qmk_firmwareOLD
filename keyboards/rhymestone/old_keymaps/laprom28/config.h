/* Copyright 2020 marksard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *l
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define TAPPING_TERM 175
#define PREVENT_STUCK_MODIFIERS
#define IGNORE_MOD_TAP_INTERRUPT

// #define TAPPING_TOGGLE 1

// https://github.com/qmk/qmk_firmware/blob/master/quantum/process_keycode/process_combo.h
#define COMBO_COUNT 4
#define COMBO_TERM 40  // try 25?

/* Mouse stuff */
/* from https://www.reddit.com/r/olkb/comments/72u8ou/qmk_mouse_keys_rock/ */
// #define MOUSEKEY_INTERVAL 16
// #define MOUSEKEY_DELAY 0
// #define MOUSEKEY_TIME_TO_MAX 30
// #define MOUSEKEY_MAX_SPEED 4
// #define MOUSEKEY_WHEEL_DELAY 0

// Constant speeds
#define MK_3_SPEED
// Hold accel to use it
#define MK_MOMENTARY_ACCEL

#define MK_C_OFFSET_UNMOD 4     // cursor offset per movement (default: 16)
#define MK_C_INTERVAL_UNMOD 16  // time between cursor movements (default: 16)
#define MK_C_OFFSET_2 20        // (default: 32)

// #define MK_COMBINED

// #define MOUSEKEY_MOVE_MAX 40

// If you use the HashTwenty(alpha), please enable USE_HASHTWENTY
// #define USE_HASHTWENTY

// If you plug in the USB on the right side, please enable MASTER_RIGHT
// #define MASTER_RIGHT
