/* ----------------------------------------------------------------------------
 * key functions : special : code
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (MIT) (see "license.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */


#include <stdbool.h>
#include <stdint.h>
#include "../../../lib-other/pjrc/usb_keyboard/usb_keyboard.h"
#include "../../../lib/usb/usage-page/keyboard.h"
#include "../../../keyboard/layout.h"
#include "../../../main.h"
#include "../public.h"
#include "../private.h"

// ----------------------------------------------------------------------------

// convenience macros
#define  LAYER         main_arg_layer
#define  LAYER_OFFSET  main_arg_layer_offset
#define  ROW           main_arg_row
#define  COL           main_arg_col
#define  IS_PRESSED    main_arg_is_pressed
#define  WAS_PRESSED   main_arg_was_pressed


// ----------------------------------------------------------------------------

/*
 * [name]
 *   Shift + press|release
 *
 * [description]
 *   Generate a 'shift' press or release before the normal keypress or
 *   keyrelease
 */
void kbfun_shift_press_release(void) {
	_kbfun_press_release(IS_PRESSED, KEY_LeftShift);
	kbfun_press_release();
}

/*
 * [name]
 *   LeftGUI + press|release
 *
 * [description]
 *   Generate a 'shift' press or release before the normal keypress or
 *   keyrelease
 */
void kbfun_command_press_release(void) {
	_kbfun_press_release(IS_PRESSED, KEY_LeftGUI);
	kbfun_press_release();
}

/*
 * [name]
 *   LeftGUI + Shift + press|release
 *
 * [description]
 *   Generate a 'shift' press or release before the normal keypress or
 *   keyrelease
 */
void kbfun_command_shift_press_release(void) {
	_kbfun_press_release(IS_PRESSED, KEY_LeftGUI);
	_kbfun_press_release(IS_PRESSED, KEY_LeftShift);
	kbfun_press_release();
}


/*
 * [name]
 *   Ctrl + press|release
 *
 * [description]
 *   Generate a 'shift' press or release before the normal keypress or
 *   keyrelease
 */
void kbfun_ctrl_press_release(void) {
	_kbfun_press_release(IS_PRESSED, KEY_LeftControl);
	kbfun_press_release();
}


/*
 * [name]
 *   Ctrl + Command + press|release
 *
 * [description]
 *   Generate a 'shift' press or release before the normal keypress or
 *   keyrelease
 */
void kbfun_ctrl_command_press_release(void) {
	_kbfun_press_release(IS_PRESSED, KEY_LeftControl);
	_kbfun_press_release(IS_PRESSED, KEY_LeftGUI);
	kbfun_press_release();
}


/*
 * [name]
 *   ALT + press|release
 *
 * [description]
 *   Generate a 'shift' press or release before the normal keypress or
 *   keyrelease
 */
void kbfun_alt_press_release(void) {
	_kbfun_press_release(IS_PRESSED, KEY_LeftAlt);
	kbfun_press_release();
}

/*
 * [name]
 *   Push 3 and Press left Alt
 *
 * [description]
 *   Push layer 3 down and hold alt.
 */
void kbfun_push3_and_alt(void) {
	kbfun_layer_push_3();
	_kbfun_press_release(true, KEY_LeftAlt);
	usb_keyboard_send();
}

/*
 * [name]
 *   Pop 3 and Release Left Alt
 *
 * [description]
 *   Pop layer 3 off and release the alt key.
 */
void kbfun_pop3_and_alt(void) {
	kbfun_layer_pop_3();
	_kbfun_press_release(false, KEY_LeftAlt);
	usb_keyboard_send();
}

/*
 * [name]
 *   Push 3 and Press left Alt and Shift
 *
 * [description]
 *   Push layer 3 down and hold alt and shift
 */
void kbfun_push3_and_alt_shift(void) {
	kbfun_layer_push_3();
	_kbfun_press_release(true, KEY_LeftAlt);
	_kbfun_press_release(true, KEY_LeftShift);
	usb_keyboard_send();
}

/*
 * [name]
 *   Pop 3 and Release Left Alt and Shift
 *
 * [description]
 *   Pop layer 3 off and release the alt and shift keys.
 */
void kbfun_pop3_and_alt_shift(void) {
	kbfun_layer_pop_3();
	_kbfun_press_release(false, KEY_LeftAlt);
	_kbfun_press_release(false, KEY_LeftShift);
	usb_keyboard_send();
}

/*
 * Helper function for toggling another key if no other keys are pressed.
 */
static void kbfun_release_other_key(uint8_t alternate_key) {
	uint8_t keycode = kb_layout_get(LAYER, ROW, COL);
	kbfun_press_release();
	usb_keyboard_send();
	if (!IS_PRESSED) {
		if (_kbfun_last_key_pressed(keycode)) {
			_kbfun_press_release(true, alternate_key);
			usb_keyboard_send();
			_kbfun_press_release(false, alternate_key);
			usb_keyboard_send();
		}
	}
} 


/*
 * [name]
 *   Key Release or Escape
 *
 * [description]
 *   Press a key, or if there were no keys pressed when the layer
 *   was pressed, send escape.
 */
void kbfun_escape_release_key() {
	kbfun_release_other_key(KEY_Escape);
}


/*
 * [name]
 *   Key Release or Space
 *
 * [description]
 *   Press a key, or if there were no keys pressed when the layer
 *   was pressed, send escape.
 */
void kbfun_space_release_key() {
	kbfun_release_other_key(KEY_Spacebar);
}


/*
 * [name]
 *   Key Release or Return/Enter
 *
 * [description]
 *   Press a key, or if there were no keys pressed when the layer
 *   was pressed, send escape.
 */
void kbfun_enter_release_key() {
	kbfun_release_other_key(KEY_ReturnEnter);
}

/*
 * [name]
 *   Two keys => capslock
 *
 * [description]
 *   When assigned to two keys (e.g. the physical left and right shift keys)
 *   (in both the press and release matrices), pressing and holding down one of
 *   the keys will make the second key toggle capslock
 *
 * [note]
 *   If either of the shifts are pressed when the second key is pressed, they
 *   wil be released so that capslock will register properly when pressed.
 *   Capslock will then be pressed and released, and the original state of the
 *   shifts will be restored
 */
void kbfun_2_keys_capslock_press_release(void) {
	static uint8_t keys_pressed;
	static bool lshift_pressed;
	static bool rshift_pressed;

	uint8_t keycode = kb_layout_get(LAYER, ROW, COL);

	if (!IS_PRESSED) keys_pressed--;

	// take care of the key that was actually pressed
	_kbfun_press_release(IS_PRESSED, keycode);

	// take care of capslock (only on the press of the 2nd key)
	if (keys_pressed == 1 && IS_PRESSED) {
		// save the state of left and right shift
		lshift_pressed = _kbfun_is_pressed(KEY_LeftShift);
		rshift_pressed = _kbfun_is_pressed(KEY_RightShift);
		// disable both
		_kbfun_press_release(false, KEY_LeftShift);
		_kbfun_press_release(false, KEY_RightShift);

		// press capslock, then release it
		_kbfun_press_release(true, KEY_CapsLock);
		usb_keyboard_send();
		_kbfun_press_release(false, KEY_CapsLock);
		usb_keyboard_send();

		// restore the state of left and right shift
		if (lshift_pressed)
			_kbfun_press_release(true, KEY_LeftShift);
		if (rshift_pressed)
			_kbfun_press_release(true, KEY_RightShift);
	}

	if (IS_PRESSED) keys_pressed++;
}

/* ----------------------------------------------------------------------------
 * numpad functions
 * ------------------------------------------------------------------------- */

static uint8_t numpad_layer_id;

static inline void numpad_toggle_numlock(void) {
	_kbfun_press_release(true, KEY_LockingNumLock);
	usb_keyboard_send();
	_kbfun_press_release(false, KEY_LockingNumLock);
	usb_keyboard_send();
}

/*
 * [name]
 *   Numpad on
 *
 * [description]
 *   Set the numpad to on (put the numpad layer, specified in the keymap, in an
 *   element at the top of the layer stack, and record that element's id) and
 *   toggle numlock (regardless of whether or not numlock is currently on)
 *
 * [note]
 *   Meant to be assigned (along with "numpad off") instead of a normal numlock
 *   key
 */
void kbfun_layer_push_numpad(void) {
	uint8_t keycode = kb_layout_get(LAYER, ROW, COL);
	main_layers_pop_id(numpad_layer_id);
	numpad_layer_id = main_layers_push(keycode);
	numpad_toggle_numlock();
}

/*
 * [name]
 *   Numpad off
 *
 * [description]
 *   Set the numpad to off (pop the layer element created by "numpad on" out of
 *   the stack) and toggle numlock (regardless of whether or not numlock is
 *   currently on)
 *
 * [note]
 *   Meant to be assigned (along with "numpad on") instead of a normal numlock
 *   key
 */
void kbfun_layer_pop_numpad(void) {
	main_layers_pop_id(numpad_layer_id);
	numpad_layer_id = 0;
	numpad_toggle_numlock();
}

/* ----------------------------------------------------------------------------
 * ------------------------------------------------------------------------- */

