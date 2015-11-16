/*
 * scrollbar_def.h
 *
 *  Created on: Nov 13, 2015
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_SCROLLBAR_DEF_H_
#define LIB_LIB_GFX_SCROLLBAR_DEF_H_

#include "controls_definition.h"
#include "button_def.h"
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
		}PositionOffset;
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
		//signed int OffsetButtonCoord;
		CursorState OldStateCursor;
		tButton* BtnSettings;
		tButton* BtnUpSettings;
		tButton* BtnDnSettings;
		tDisplay *pDisplay;
		void *ParentWindow;
		bool ParentWindowStateEnabled;
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
#define _new_scrollbar(name) tScrollBar *name = NULL
//#define free_scrollbar(address) free(address);




#endif /* LIB_LIB_GFX_SCROLLBAR_DEF_H_ */
