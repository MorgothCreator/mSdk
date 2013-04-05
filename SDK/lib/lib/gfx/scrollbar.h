/*
 *  lib/gfx/scrollbar.h
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

#ifndef SCROLLBAR_H_
#define SCROLLBAR_H_

#include <stdbool.h>
#include "controls_definition.h"
#include "buton.h"

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
		signed int MinBtnSize;
	}Size;
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
		}OnValueChanged;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
		bool ValueChanged;
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
			signed int MinBtnSize;
		}Size;
		bool CursorDownInsideBox;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool NoPaintBackGround;
		bool IsChildren;
		signed int OldMinimum;
		signed int OldMaximum;
		signed int OldValue;
		signed int CoordonateOfTouchDown;
		signed int CoordonateOfButtonDown;
		signed int OffsetButtonCoord;
		CursorState OldStateCursor;
		tButton* BtnSettings;
		tButton* BtnUpSettings;
		tButton* BtnDnSettings;
		tDisplay *pDisplay;
	}Internals;
	bool Visible;
	bool Enabled;
	signed int Minimum;
	signed int Maximum;
	signed int Value;
	CursorState StateChangedOn;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
}tScrollBar;
//#######################################################################################
void scrollbar(tScrollBar *settings, tControlCommandData* control_comand);
tScrollBar *new_scrollbar(tDisplay *ScreenDisplay);
bool free_scrollbar(tScrollBar* settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "scrollbar.c"
#endif
#endif /* SCROLLBAR_H_ */
