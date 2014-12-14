/*
 *  lib/gfx/keyboard.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VKBD_QWERTY_H_
#define VKBD_QWERTY_H_
//#######################################################################################
#include <stdbool.h>
#include "api/timer_api.h"
#include "controls_definition.h"
//#include "window_def.h"
//#######################################################################################
typedef enum kbd_qwerty_type_enum
{
	Kbd_Qwerty_LittleLetters = 0x0,
	Kbd_Qwerty_BigLetters,
	Kbd_Qwerty_Numeric,
}kbd_qwerty_type_t;

typedef enum kbd_qwerty_spectial_response
{
	kbd_qwerty_ctrl = 128,
	kbd_qwerty_left = 256,
	kbd_qwerty_right = 257,
	kbd_qwerty_up = 258,
	kbd_qwerty_dn = 259,
}kbd_qwerty_spectial_response_t;
//#######################################################################################
typedef struct
{
	struct
	{
		signed int X;
		signed int Y;
	}Position;
	struct
	{
		signed int X;
		signed int Y;
	}Size;
	controls_caption_t Caption;
	controls_color_struct_t Color;
	struct
	{
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*, unsigned int);
		}OnMove;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*, unsigned int);
		}OnUp;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*, unsigned int);
		}OnDown;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
	}Events;
	struct
	{
		struct
		{
			bool Initiated;
		}Control;
		struct
		{
			signed int X;
			signed int Y;
		}Position;
		struct
		{
			signed int X;
			signed int Y;
		}PositionOffset;
		struct
		{
			signed int X;
			signed int Y;
		}Size;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool IsChildren;
		bool NoPaintBackGround;
		kbd_qwerty_type_t old_keboard_type;
		kbd_qwerty_type_t keboard_type;
		CursorState OldStateCursor;
		tDisplay *pDisplay;
		tButton *Keys[38];
		void *ParentWindow;
		bool ParentWindowStateEnabled;
	}Internals;
	bool Visible;
	bool Enabled;
	unsigned int Return;
	unsigned int kbd_border_size;
	unsigned int key_space_size;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
}tVKbd_Qwerty;
//#######################################################################################
void v_keyboard(tVKbd_Qwerty *settings, tControlCommandData* control_comand);
tVKbd_Qwerty *new_v_keyboard(void *ParentWindow);
bool free_v_keyboard(tVKbd_Qwerty* settings);
//#######################################################################################
#define _new_vqkbd(name) tVKbd_Qwerty *name = NULL
//#define free_vqkbd(address) free(address);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "keyboard.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
