/*
 * keyboard_def.h
 *
 *  Created on: Nov 13, 2015
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_KEYBOARD_DEF_H_
#define LIB_LIB_GFX_KEYBOARD_DEF_H_


#include "controls_definition.h"
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
#define _new_vqkbd(name) tVKbd_Qwerty *name = NULL
//#define free_vqkbd(address) free(address);



#endif /* LIB_LIB_GFX_KEYBOARD_DEF_H_ */
