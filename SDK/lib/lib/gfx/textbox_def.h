/*
 * textbox_def.h
 *
 *  Created on: Nov 13, 2015
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_TEXTBOX_DEF_H_
#define LIB_LIB_GFX_TEXTBOX_DEF_H_


#include "controls_definition.h"
#include "scrollbar_def.h"
#include "lib/string_lib.h"
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
	//controls_caption_t Caption;
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
	Align TextAlign;
	String_t *Text;
	bool WordWrap;
	tFont* Font;
	CursorState StateChangedOn;
	void (*idle)(void *_settings, tControlCommandData* control_comand);
	bool (*free)(void *settings);
	void (*text)(void* _settings, char* string);
	void (*append)(void* _settings, char* string);
	void (*insert)(void* _settings, char* string, unsigned int location);
	void (*to_uper)(void* _settings);
	void (*to_lower)(void* _settings);
	void (*font)(void* _settings, tFont* font);
	void (*wordwrap)(void* _settings, bool word_wrap);
	void (*align)(void* _settings, bool align);

}tTextBox;
//#######################################################################################
#define textbox_static_item(name, parent_window, pDisplay)\
		tTextBox name = {\
				.idle = textbox,\
				.free = free_textbox,\
				.text = textbox_text,\
				.append = textbox_text_append,\
				.insert = textbox_text_insert,\
				.to_uper = textbox_text_to_uper,\
				.to_lower = textbox_text_to_lower,\
				.font = textbox_font,\
				.wordwrap = textbox_wordwrap,\
				.align = textbox_align,\
\
				.WordWrap = true,\
				.Font = (tFont*)&DEFAULT_FONT,\
\
				.Color.Scren = 0x989CB6,\
				.Color.Enabled.BackGround = 0x00373737,\
				.Color.Enabled.Border.Pull = 0x00FFFFFF,\
				.Color.Enabled.Border.Push = 0x00FFFFFF,\
				.Color.Enabled.Buton.Pull = 0x00FFFFFF,\
				.Color.Enabled.Buton.Push = 0x00919191,\
				.Color.Enabled.Ink.Pull = 0x00393939,\
				.Color.Enabled.Ink.Push = 0x00000000,\
\
				.Color.Disabled.BackGround = 0x00373737,\
				.Color.Disabled.Border = 0x00989CB6,\
				.Color.Disabled.Buton = 0x00989CB6,\
				.Color.Disabled.Ink = 0x00393939,\
\
				.Enabled = true,\
				.Position.X = 10,\
				.Position.Y = 50,\
				.Size.X = 70,\
				.Size.Y = 70,\
				.Size.ScrollSize = 20,\
				.StateChangedOn = Cursor_Up,\
				.Visible = true,\
\
		};
//#######################################################################################
#define _new_textbox(name) tTextBox *name = NULL
//#define free_textbox(address) free(address);



#endif /* LIB_LIB_GFX_TEXTBOX_DEF_H_ */
