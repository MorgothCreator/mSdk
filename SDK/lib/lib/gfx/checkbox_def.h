/*
 * checkbox_def.h
 *
 *  Created on: Nov 13, 2015
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_CHECKBOX_DEF_H_
#define LIB_LIB_GFX_CHECKBOX_DEF_H_

typedef enum
{
	checkbox_style_normal = 0,
	checkbox_style_radio_btn
}CHECK_BOX_STYLE;
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
		}PositionOffset;
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
		void *ParentWindow;
		bool ParentWindowStateEnabled;
	}Internals;
	CHECK_BOX_STYLE Style;
	bool Visible;
	bool Enabled;
	bool Cheched;
	CursorState StateChangedOn;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
}tCheckBox;
//#######################################################################################
#define _new_checkbox(name) tCheckBox *name = NULL
//#define free_checkbox(address) free(address);




#endif /* LIB_LIB_GFX_CHECKBOX_DEF_H_ */
