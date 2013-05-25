/*
 *  lib/gfx/listbox.h
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

#ifndef LISTBOX_H_
#define LISTBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "scrollbar.h"
#include "controls_definition.h"
//#######################################################################################
typedef struct
{
	struct
	{
		signed int X;
		signed int Y;
	}Size;
	struct
	{
		Align TextAlign;
		char *Text;
		bool WordWrap;
		tFont* Font;
	}Caption;
	controls_color_struct_t Color;
	bool CursorDownInsideBox;
}listbox_item;
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
		signed int ItemSizeY;
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
			signed int ItemSizeY;
			signed int ScrollSize;
		}Size;
		controls_caption_t Caption;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool IsChildren;
		bool NoPaintBackGround;
		unsigned int ItemStartOnBox;
		unsigned int OldItemStartOnBox;
		CursorState OldStateCursor;
		tDisplay *pDisplay;
		tScrollBar* ScrollBar;
	}Internals;
	bool Visible;
	bool Enabled;
	bool AlwaisShowScrollbar;
	CursorState StateChangedOn;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
#ifdef USE_FULL_GRAPHICS
	struct
	{

	}Text;
	struct
	{
		bool (*insert)(void* /*_settings*/, char* /*text*/, unsigned int /*location*/);
		bool (*add)(void* /*_settings*/, char* /*text*/);
		bool (*remove)(void* /*_settings*/, unsigned int /*location*/);
		bool (*remove_all)(void* /*_settings*/);
	}Item;
#endif
	bool (*free)(void */*settings*/);
	unsigned int ItemsCount;
	unsigned int SelectedItem;
	listbox_item **Items;
}tListBox;
//#######################################################################################
void listbox(tListBox *settings, tControlCommandData* control_comand);
tListBox *new_listbox(tDisplay *ScreenDisplay);
bool free_listbox(void* _settings);
bool listbox_item_insert(void* _settings, char* text, unsigned int location);
bool listbox_item_add(void* _settings, char* text);
bool listbox_item_remove(void* _settings, unsigned int location);
bool listbox_item_remove_all(void* _settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "listbox.c"
#endif
//#######################################################################################
#endif /* LISTBOX_H_ */
