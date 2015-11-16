/*
 * progressbar_def.h
 *
 *  Created on: Nov 13, 2015
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_PROGRESSBAR_DEF_H_
#define LIB_LIB_GFX_PROGRESSBAR_DEF_H_

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
		}OnValueChange;
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
		}Size;
		controls_caption_t Caption;
		unsigned int OldValue;
		unsigned int OldMinimValue;
		unsigned int OldMaximValue;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool OldStateCheched;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool NoPaintBackGround;
		bool CursorDownInsideBox;
		CursorState OldStateCursor;
		tDisplay *pDisplay;
		void *ParentWindow;
		bool ParentWindowStateEnabled;
	}Internals;
	bool Visible;
	bool Enabled;
	signed int Value;
	signed int MinimValue;
	signed int MaximValue;
	CursorState StateChangedOn;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
}tProgressBar;
//#######################################################################################
#define _new_progressbar(name) tProgressBar *name = NULL
//#define free_progressbar(address) free(address);



#endif /* LIB_LIB_GFX_PROGRESSBAR_DEF_H_ */
