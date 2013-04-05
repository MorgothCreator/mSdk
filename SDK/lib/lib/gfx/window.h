/*
 *  lib/gfx/window.h
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

#ifndef WINDOW_H_
#define WINDOW_H_
//#######################################################################################
#include "controls_definition.h"
#include "buton.h"
#include "checkbox.h"
#include "listbox.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "textbox.h"
//#######################################################################################
#define WindowButonChildren			1
#define WindowCheckboxChildren		2
#define WindowListboxChildren		3
#define WindowProgressbarChildren	4
#define WindowScrollbarChildren		5
#define WindowTextboxChildren		6
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
	struct
	{
		bool ChildrenModified;
	}Internals;
	bool Visible;
	bool Enabled;
	char *ChildrenName;
	void *Children;
	controls_color_struct_t Color;
	unsigned int Type;
	CursorState StateChangedOn;
}window_children_t;
//#######################################################################################
typedef struct
{
	signed int X;
	signed int Y;
}ChildrenWindowSize_t;

typedef struct Window_s
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
		signed int ScrollBarSize;
	}Size;
	struct
	{
		signed int X;
		signed int Y;
	}SizeFullScreen;
	struct
	{
		signed int X;
		signed int Y;
	}SizeNormalScreen;
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
			signed int ChildrenPosition_X;
			signed int ChildrenPosition_Y;
		}Position;
		struct
		{
			signed int X;
			signed int Y;
			signed int ScrollBarSize;
		}Size;
		struct
		{
			struct
			{
				signed int Y;
			}Size;
			tButton *Close;
			tCheckBox *MaxMin;
			tButton *Minimize;
		}Header;
		struct
		{
			signed int X;
			signed int Y;
		}NormalScreenPosition;
		controls_caption_t Caption;
		unsigned int ButonCnt;
		unsigned int CheckBoxCnt;
		unsigned int ListBoxCnt;
		unsigned int ProgressBarCnt;
		unsigned int ScrollBarCnt;
		unsigned int TextBoxCnt;
		signed int HeaderTouchDownPointX;
		signed int HeaderTouchDownPointY;
		signed int WindowTouchDownPointX;
		signed int WindowTouchDownPointY;
		bool CursorDownOnHeader;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool CloseButonEnabled;
		bool CloseButonVisible;
		bool MaxMinEnabled;
		bool MaxMinVisible;
		bool MinimizeButonEnabled;
		bool MinimizeButonVisible;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool FullScreen;
		bool OneChildrenHasBeenModified;
		volatile unsigned int ChildrensNr;
		window_children_t **Childrens;
		CursorState OldStateCursor;
		tDisplay *pDisplay;
		tScrollBar *H_ScrollBar;
		tScrollBar *V_ScrollBar;
		ChildrenWindowSize_t ChildrenWindowSize;
	}Internals;
	bool Visible;
	bool Enabled;
	bool CloseButonEnabled;
	bool CloseButonVisible;
	bool MaxMinEnabled;
	bool MaxMinVisible;
	bool MinimizeButonEnabled;
	bool MinimizeButonVisible;
	CursorState StateChangedOn;
	//tControlCommandData*(*Idle)(void*, tControlCommandData*);

	void (*idle)(struct Window_s *, tControlCommandData *);//window
	void* (*add_children)(struct Window_s *, unsigned int, char *);
	signed int (*get_children_index)(struct Window_s *, char *);
	window_children_t *(*get_children_address)(struct Window_s *, char *);
	bool (*set_list_of_childrens)(struct Window_s *, window_children_t **, unsigned int);

}tWindow;
//#######################################################################################
tWindow *new_window(tDisplay *ScreenDisplay);

void window(struct Window_s *settings, tControlCommandData *control_comand);
void* window_add_children(struct Window_s *settings, unsigned int children_type, char *children_name);
signed int window_get_children_index(struct Window_s *settings, char *name);
window_children_t *window_get_children_address(struct Window_s *settings, char *name);
bool window_set_list_of_childrens(struct Window_s *settings, window_children_t **list, unsigned int num_of_childrens);
//#######################################################################################
#define window_new_buton(window_addr, buton_name)\
			tButton *buton_name = (tButton *)window_add_children(window_addr, WindowButonChildren, #buton_name)

#define window_new_checkbox(window_addr, checkbox_name)\
			tCheckBox *checkbox_name = (tCheckBox *)window_add_children(window_addr, WindowCheckboxChildren, #checkbox_name)

#define window_new_listbox(window_addr, listbox_name)\
			tListBox *listbox_name = (tListBox *)window_add_children(window_addr, WindowListboxChildren, #listbox_name)

#define window_new_progressbar(window_addr, progressbar_name)\
			tProgressBar *progressbar_name = (tProgressBar *)window_add_children(window_addr, WindowProgressbarChildren, #progressbar_name)

#define window_new_scrollbar(window_addr, scrollbar_name)\
			tScrollBar *scrollbar_name = (tScrollBar *)window_add_children(window_addr, WindowScrollbarChildren, #scrollbar_name)

#define window_new_textbox(window_addr, textbox_name)\
			tTextBox *textbox_name = (tTextBox *)window_add_children(window_addr, WindowTextboxChildren, #textbox_name)


//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "window.c"
#endif
//#######################################################################################
#endif /* WINDOW_H_ */
//#######################################################################################

