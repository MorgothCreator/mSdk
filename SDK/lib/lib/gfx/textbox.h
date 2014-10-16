/*
 *  lib/gfx/textbox.h
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

#ifndef TEXTBOX_H_
#define TEXTBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "scrollbar.h"
#include "controls_definition.h"
//#include "window_def.h"
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
		signed int ScrollSize;
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
			signed int ScrollSize;
		}Size;
		controls_caption_t Caption;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool CursorDownInsideTextBox;
		bool IsChildren;
		bool NoPaintBackGround;
		signed int OldSelStart;
		signed int OldSelLen;
		signed int PenPushX;
		signed int PenPushY;
		signed int Start;
		CursorState OldStateCursor;
		tScrollBar *Hscrollbar;
		tScrollBar *Vscrollbar;
		tDisplay *pDisplay;
		void *ParentWindow;
		bool ParentWindowStateEnabled;
	}Internals;
	bool Visible;
	bool Enabled;
	unsigned int SelectionStart;
	unsigned int SelectionLength;
	signed int SelStart;
	signed int SelLen;
	CursorState StateChangedOn;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
}tTextBox;
//#######################################################################################
void textbox(tTextBox *settings, tControlCommandData* control_comand);
tTextBox *new_textbox(void *ParentWindow);
bool free_textbox(tTextBox* settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "textbox.c"
#endif
//#######################################################################################
#endif /* TEXTBOX_H_ */
//#######################################################################################
