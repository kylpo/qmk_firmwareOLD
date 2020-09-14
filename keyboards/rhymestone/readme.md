# Rhymestone

![Rhymestone](https://github.com/marksard/Keyboards/raw/master/_image/20181214-PC140125.jpg)  

This is 40 keys split Ortholinear keyboard.  

Keyboard Maintainer: [marksard](https://github.com/marksard)  
Hardware Supported: The PCBs, controllers supported  
Hardware Availability: links to where you can find this hardware

Make example for this keyboard (after setting up your build environment):

    make rhymestone:default

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

[Build guide](https://github.com/marksard/Keyboards/blob/master/rhymestone/documents/rhymestone_buildguide.md)  
[Firmware](https://github.com/marksard/qmk_firmware/tree/my_customize/keyboards/rhymestone)  

## Kylpo Notes

From project root (`/qmk_firmware`):

```sh
make rhymestone:kylpo2

cp .build/rhymestone_rev1_kylpo2.hex ~/Downloads

```

or all in one:

```sh
make rhymestone:kylpo2 && cp .build/rhymestone_rev1_kylpo2.hex ~/Downloads
```

Then flash it with QMK Toolbox

### To Do
Use Leader key for media controls?
- [QMK Basics: Leader Key, using sequences for shortcuts](https://thomasbaart.nl/2018/12/20/qmk-basics-leader-key/#how-the-leader-key-works)
- > It allows you to define sequences with which you can trigger keypresses and functions
- > can make them up to five keypresses long after the leader key itself
- [Leader Key - QMK](https://beta.docs.qmk.fm/using-qmk/advanced-keycodes/feature_leader_key)

### Code notes
- `SEND_STRING()` - type out a string (C-level preprocessor macro)
- `register_code(KC_)` - send KC_ keydown event
- `unregister_code(KC_)` - send KC_ keyup event
- `tap_code(KC_)` - same as register_code(KC_); unregister_code(KC_);
- `clear_keyboard()` - clear all mods and keys currently pressed
- `clear_mods()` - clear all mods currently pressed
- return `true` in `process_record_user()` to indicate to the caller that the key press we just processed should continue to be processed as normal
- `static` will retain the value between separate calls of the function

See also: [action_util.c](https://github.com/qmk/qmk_firmware/blob/master/tmk_core/common/action_util.c)

### Resources
- [Keycode Full List - QMK](https://beta.docs.qmk.fm/using-qmk/simple-keycodes/keycodes)
- [Mouse Keys - QMK](https://beta.docs.qmk.fm/using-qmk/advanced-keycodes/feature_mouse_keys)
- [Macros - QMK](https://beta.docs.qmk.fm/using-qmk/advanced-keycodes/feature_macros)

Alt-shifted:
- [gavinenns/qmk_firmware](https://github.com/gavinenns/qmk_firmware/blob/773dbdb095d4f48f39ce6ca1c0a9cb49a4cd1a52/keyboards/planck/keymaps/gavinenns/keymap.c#L158)
- [qmk_firmware/users/spacebarracecar](https://github.com/qmk/qmk_firmware/tree/master/users/spacebarracecar)
- https://github.com/qmk/qmk_firmware/issues/6705
- [QMK: Using ctrl as fn-key only for a few keys? : olkb](https://www.reddit.com/r/olkb/comments/9eohzf/qmk_using_ctrl_as_fnkey_only_for_a_few_keys/)

Mod-tap:
- [QMK Basics: Tap and hold actions: Tap into your modifiers](https://thomasbaart.nl/2018/12/09/qmk-basics-tap-and-hold-actions/#mod-tap)
- [Custom modified keycode handling PR](https://github.com/qmk/qmk_firmware/pull/4795/files)