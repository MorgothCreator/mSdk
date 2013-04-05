/*
 *  lib/gfx/checkbox.h
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

#ifndef CHECKBOX_H_
#define CHECKBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "controls_definition.h"
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
			void*(*CallBack)(void*);
		}OnMove;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnUp;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnDown;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnCheched;
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnUnCheched;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
		bool Checked;
		bool UnChecked;
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
		}Size;
		controls_caption_t Caption;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool OldStateCheched;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool NoPaintBackGround;
		CursorState OldStateCursor;
		tDisplay *pDisplay;
	}Internals;
	bool Visible;
	bool Enabled;
	bool Cheched;
	CursorState StateChangedOn;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
}tCheckBox;
//#######################################################################################
void checkbox(tCheckBox *settings, tControlCommandData* control_comand);
tCheckBox *new_checkbox(tDisplay *ScreenDisplay);
bool free_checkbox(tCheckBox* settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "checkbox.c"
#endif
//#######################################################################################
#endif /* CHECKBOX_H_ */
