/*
 * window_def.h
 *
 *  Created on: Sep 25, 2014
 *      Author: Morgoth
 */

#ifndef WINDOW_DEF_H_
#define WINDOW_DEF_H_

#include "button.h"
#include "checkbox.h"
#include "listbox.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "textbox.h"
#include "picturebox.h"
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
		bool OldStateVisible;
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
	//struct
	//{
	//	signed int X;
	//	signed int Y;
	//}SizeFullScreen;
	//struct
	//{
	//	signed int X;
	//	signed int Y;
	//}PositionFullScreen;
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
			unsigned int WindowBorder;
			unsigned int WindowHeader;
		}Enabled;
		struct
		{
			unsigned int WindowBorder;
			unsigned int WindowHeader;
		}Disabled;
	}WindowColor;
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
			tButton *Resize;
		}Header;
		struct
		{
			signed int X;
			signed int Y;
		}NormalScreenPosition;
		struct
		{
			signed int X;
			signed int Y;
		}PositionOffset;
		//struct
		//{
		//	signed int X;
		//	signed int Y;
		//}SizeFullScreen;
		//struct
		//{
		//	signed int X;
		//	signed int Y;
		//}PositionFullScreen;
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
		signed int WindowResizeTouchDownPointX;
		signed int WindowResizeTouchDownPointY;
		signed int WindowResizeTouchDownSizeX;
		signed int WindowResizeTouchDownSizeY;
		bool CursorDownOnHeader;
		bool CursorDownOnResizeBtn;
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
		tRectangle WindowMoveLimits;
		void *ParentWindow;
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
	tRectangle WindowMoveLimits;
	//tControlCommandData*(*Idle)(void*, tControlCommandData*);

	void (*idle)(struct Window_s *, tControlCommandData *);//window
	void* (*add_children)(struct Window_s *, unsigned int, char *);
	signed int (*get_children_index)(struct Window_s *, char *);
	window_children_t *(*get_children_address)(struct Window_s *, char *);
	bool (*set_list_of_childrens)(struct Window_s *, window_children_t **, unsigned int);

}tWindow;



#endif /* WINDOW_DEF_H_ */
