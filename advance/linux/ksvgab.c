/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * In addition, as a special exception, Andrea Mazzoleni
 * gives permission to link the code of this program with
 * the MAME library (or with modified versions of MAME that use the
 * same license as MAME), and distribute linked combinations including
 * the two.  You must obey the GNU General Public License in all
 * respects for all of the code used other than MAME.  If you modify
 * this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to
 * do so, delete this exception statement from your version.
 */

#include "ksvgab.h"
#include "log.h"
#include "oslinux.h"
#include "error.h"

#include <vgakeyboard.h>

#ifdef USE_VIDEO_SDL
#include "SDL.h"
#endif

struct keyb_svgalib_context {
	unsigned map_os_to_code[KEYB_MAX];
};

static struct keyb_pair {
	int os;
	int code;
} KEYS[] = {
{ KEYB_A, SCANCODE_A },
{ KEYB_B, SCANCODE_B },
{ KEYB_C, SCANCODE_C },
{ KEYB_D, SCANCODE_D },
{ KEYB_E, SCANCODE_E },
{ KEYB_F, SCANCODE_F },
{ KEYB_G, SCANCODE_G },
{ KEYB_H, SCANCODE_H },
{ KEYB_I, SCANCODE_I },
{ KEYB_J, SCANCODE_J },
{ KEYB_K, SCANCODE_K },
{ KEYB_L, SCANCODE_L },
{ KEYB_M, SCANCODE_M },
{ KEYB_N, SCANCODE_N },
{ KEYB_O, SCANCODE_O },
{ KEYB_P, SCANCODE_P },
{ KEYB_Q, SCANCODE_Q },
{ KEYB_R, SCANCODE_R },
{ KEYB_S, SCANCODE_S },
{ KEYB_T, SCANCODE_T },
{ KEYB_U, SCANCODE_U },
{ KEYB_V, SCANCODE_V },
{ KEYB_W, SCANCODE_W },
{ KEYB_X, SCANCODE_X },
{ KEYB_Y, SCANCODE_Y },
{ KEYB_Z, SCANCODE_Z },
{ KEYB_0, SCANCODE_0 },
{ KEYB_1, SCANCODE_1 },
{ KEYB_2, SCANCODE_2 },
{ KEYB_3, SCANCODE_3 },
{ KEYB_4, SCANCODE_4 },
{ KEYB_5, SCANCODE_5 },
{ KEYB_6, SCANCODE_6 },
{ KEYB_7, SCANCODE_7 },
{ KEYB_8, SCANCODE_8 },
{ KEYB_9, SCANCODE_9 },
{ KEYB_0_PAD, SCANCODE_KEYPAD0 },
{ KEYB_1_PAD, SCANCODE_KEYPAD1 },
{ KEYB_2_PAD, SCANCODE_KEYPAD2 },
{ KEYB_3_PAD, SCANCODE_KEYPAD3 },
{ KEYB_4_PAD, SCANCODE_KEYPAD4 },
{ KEYB_5_PAD, SCANCODE_KEYPAD5 },
{ KEYB_6_PAD, SCANCODE_KEYPAD6 },
{ KEYB_7_PAD, SCANCODE_KEYPAD7 },
{ KEYB_8_PAD, SCANCODE_KEYPAD8 },
{ KEYB_9_PAD, SCANCODE_KEYPAD9 },
{ KEYB_F1, SCANCODE_F1 },
{ KEYB_F2, SCANCODE_F2 },
{ KEYB_F3, SCANCODE_F3 },
{ KEYB_F4, SCANCODE_F4 },
{ KEYB_F5, SCANCODE_F5 },
{ KEYB_F6, SCANCODE_F6 },
{ KEYB_F7, SCANCODE_F7 },
{ KEYB_F8, SCANCODE_F8 },
{ KEYB_F9, SCANCODE_F9 },
{ KEYB_F10, SCANCODE_F10 },
{ KEYB_F11, SCANCODE_F11 },
{ KEYB_F12, SCANCODE_F12 },
{ KEYB_ESC, SCANCODE_ESCAPE },
{ KEYB_BACKQUOTE, SCANCODE_GRAVE },
{ KEYB_MINUS, SCANCODE_MINUS },
{ KEYB_EQUALS, SCANCODE_EQUAL },
{ KEYB_BACKSPACE, SCANCODE_BACKSPACE },
{ KEYB_TAB, SCANCODE_TAB },
{ KEYB_OPENBRACE, SCANCODE_BRACKET_LEFT },
{ KEYB_CLOSEBRACE, SCANCODE_BRACKET_RIGHT },
{ KEYB_ENTER, SCANCODE_ENTER },
{ KEYB_SEMICOLON, SCANCODE_SEMICOLON },
{ KEYB_QUOTE, SCANCODE_APOSTROPHE },
{ KEYB_BACKSLASH, SCANCODE_BACKSLASH },
{ KEYB_LESS, SCANCODE_LESS },
{ KEYB_COMMA, SCANCODE_COMMA },
{ KEYB_PERIOD, SCANCODE_PERIOD },
{ KEYB_SLASH, SCANCODE_SLASH },
{ KEYB_SPACE, SCANCODE_SPACE },
{ KEYB_INSERT, SCANCODE_INSERT },
{ KEYB_DEL, SCANCODE_REMOVE },
{ KEYB_HOME, SCANCODE_HOME },
{ KEYB_END, SCANCODE_END },
{ KEYB_PGUP, SCANCODE_PAGEUP },
{ KEYB_PGDN, SCANCODE_PAGEDOWN },
{ KEYB_LEFT, SCANCODE_CURSORBLOCKLEFT },
{ KEYB_RIGHT, SCANCODE_CURSORBLOCKRIGHT },
{ KEYB_UP, SCANCODE_CURSORBLOCKUP },
{ KEYB_DOWN, SCANCODE_CURSORBLOCKDOWN },
{ KEYB_SLASH_PAD, SCANCODE_KEYPADDIVIDE },
{ KEYB_ASTERISK, SCANCODE_KEYPADMULTIPLY },
{ KEYB_MINUS_PAD, SCANCODE_KEYPADMINUS },
{ KEYB_PLUS_PAD, SCANCODE_KEYPADPLUS },
{ KEYB_PERIOD_PAD, SCANCODE_KEYPADPERIOD },
{ KEYB_ENTER_PAD, SCANCODE_KEYPADENTER },
{ KEYB_PRTSCR, SCANCODE_PRINTSCREEN },
{ KEYB_PAUSE, SCANCODE_BREAK },
{ KEYB_LSHIFT, SCANCODE_LEFTSHIFT },
{ KEYB_RSHIFT, SCANCODE_RIGHTSHIFT },
{ KEYB_LCONTROL, SCANCODE_LEFTCONTROL },
{ KEYB_RCONTROL, SCANCODE_RIGHTCONTROL },
{ KEYB_ALT, SCANCODE_LEFTALT },
{ KEYB_ALTGR, SCANCODE_RIGHTALT },
{ KEYB_LWIN, SCANCODE_LEFTWIN },
{ KEYB_RWIN, SCANCODE_RIGHTWIN },
{ KEYB_MENU, 127 /* Not defined by SVGALIB */ },
{ KEYB_SCRLOCK, SCANCODE_SCROLLLOCK },
{ KEYB_NUMLOCK, SCANCODE_NUMLOCK },
{ KEYB_CAPSLOCK, SCANCODE_CAPSLOCK },
{ KEYB_MAX, 0 }
};

static struct keyb_svgalib_context svgalib_state;

static adv_device DEVICE[] = {
{ "auto", -1, "SVGALIB keyboard" },
{ 0, 0, 0 }
};

adv_error keyb_svgalib_init(int keyb_id, adv_bool disable_special)
{
	struct keyb_pair* i;
	unsigned j;

	log_std(("key:svgalib: keyb_svgalib_init(id:%d,disable_special:%d)\n",keyb_id,(int)disable_special));

	if (!os_internal_svgalib_get()) {
		log_std(("keyb:svgalib: svgalib not initialized\n"));
		error_nolog_cat("svgalib: Not supported without the svgalib library\n");
		return -1;
	}
	
#ifdef USE_VIDEO_SDL
	if (SDL_WasInit(SDL_INIT_VIDEO)) {
		log_std(("keyb:svgalib: Incompatible with the SDL video driver\n"));
		error_nolog_cat("svgalib: Incompatible with the SDL video driver\n");
		return -1; 
	}
#endif

	for(j=0;j<KEYB_MAX;++j) {
		svgalib_state.map_os_to_code[j] = 0;
	}
	for(i=KEYS;i->os != KEYB_MAX;++i) {
		svgalib_state.map_os_to_code[i->os] = i->code;
	}

	if (keyboard_init() != 0) {
		log_std(("keyb:svgalib: keyboard_init() failed\n"));
		return -1;
	}

	return 0;
}

void keyb_svgalib_done(void)
{
	log_std(("keyb:svgalib: keyb_svgalib_done()\n"));

	keyboard_close();
}

unsigned keyb_svgalib_get(unsigned code)
{
	unsigned svgalib_code;

	assert( code < KEYB_MAX);

	log_debug(("keyb:svgalib: keyb_svgalib_get(code:%d)\n",code));

	/* disable the pause key */
	if (code == KEYB_PAUSE)
		return 0;

	svgalib_code = svgalib_state.map_os_to_code[code];

	log_debug(("keyb:svgalib: keyb_svgalib_get() svgalib_code:%d\n",svgalib_code));

	if (!svgalib_code)
		return 0;

	return keyboard_keypressed(svgalib_code);
}

void keyb_svgalib_all_get(unsigned char* code_map)
{
	unsigned i;

	log_debug(("keyb:svgalib: keyb_svgalib_all_get()\n"));

	for(i=0;i<KEYB_MAX;++i) {
		unsigned svgalib_code = svgalib_state.map_os_to_code[i];
		if (svgalib_code)
			code_map[i] = keyboard_keypressed(svgalib_code);
		else
			code_map[i] = 0;
	}

	/* disable the pause key */
	code_map[KEYB_PAUSE] = 0;
}

void keyb_svgalib_poll()
{
	log_debug(("keyb:svgalib: keyb_svgalib_poll()\n"));

	keyboard_update();
}

unsigned keyb_svgalib_flags(void)
{
	return 0;
}

adv_error keyb_svgalib_load(adv_conf* context)
{
	return 0;
}

void keyb_svgalib_reg(adv_conf* context)
{
}

/***************************************************************************/
/* Driver */

keyb_driver keyb_svgalib_driver = {
	"svgalib",
	DEVICE,
	keyb_svgalib_load,
	keyb_svgalib_reg,
	keyb_svgalib_init,
	keyb_svgalib_done,
	keyb_svgalib_flags,
	keyb_svgalib_get,
	keyb_svgalib_all_get,
	keyb_svgalib_poll
};

