/*
 * window_def.h
 *
 *  Created on: Sep 25, 2014
 *      Author: Morgoth
 */

#ifndef WINDOW_DEF_H_
#define WINDOW_DEF_H_

#include "button_def.h"
#include "checkbox_def.h"
#include "listbox_def.h"
#include "progressbar_def.h"
#include "scrollbar_def.h"
#include "textbox_def.h"
#include "picturebox_def.h"
#include "keyboard_def.h"
//#######################################################################################
#define WindowButtonChildren		1
#define WindowCheckboxChildren		2
#define WindowListboxChildren		3
#define WindowProgressbarChildren	4
#define WindowScrollbarChildren		5
#define WindowTextboxChildren		6
#define WindowPictureboxChildren	7
#define WindowKeyboardChildren		8
#define WindowWindowChildren		9
#define WindowTabGroupChildren		10
//#######################################################################################
typedef struct
{
	struct
	{
		bool OldStateVisible;
	}Internals;
	bool Visible;
	char *ChildrenName;
	void *Children;
	unsigned int Type;
	unsigned int TabLocNr;
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
		struct
		{
			void* CallbackData;
			void* CallbackReturnData;
			void*(*CallBack)(void*);
		}OnTabChange;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
		bool TabChanged;
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
				signed int TabGroupSelectorSize;
			}Size;
			signed int TabGroupSelectorPosition;
			tButton *Close;
			tCheckBox *MaxMin;
			tButton *Minimize;
			tButton *Resize;
			tButton *TabGroupScrollLeft;
			tButton *TabGroupScrollRight;
			tCheckBox **TabGroupTabsList;
			int TabGroupTabsListNr;
			int OldTabGroupTabsListNr;

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
		bool NoPaintBackGround;
		bool CursorDownInsideChildrenWindow;
		bool TabGroupMode;
		bool OldTabGroupMode;
		bool HideHeader;
		//bool TabChanged;
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
	bool AllowVScroll;
	bool AllowHScroll;
	bool HideVScroll;
	bool HideHScroll;
	bool HideHeader;
	unsigned int SelectedTab;
	CursorState StateChangedOn;
	tRectangle WindowMoveLimits;
	//tControlCommandData*(*Idle)(void*, tControlCommandData*);

	void (*idle)(struct Window_s *, tControlCommandData *);//window
	void* (*add_children)(struct Window_s *, unsigned int, char *);
	signed int (*get_children_index)(struct Window_s *, char *);
	window_children_t *(*get_children_address)(struct Window_s *, char *);
	bool (*set_list_of_childrens)(struct Window_s *, window_children_t **, unsigned int);

}tWindow;
//#######################################################################################
/* Definitions to add new private items to a parent window */
#define window_new_button(window_addr, button_name)\
			tButton *button_name = (tButton *)window_add_children(window_addr, WindowButtonChildren, #button_name)

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

#define window_new_picturebox(window_addr, picturebox_name)\
			tPictureBox *picturebox_name = (tPictureBox *)window_add_children(window_addr, WindowPictureboxChildren, #picturebox_name)

#define window_new_keyboard(window_addr, keyboard_name)\
			tVKbd_Qwerty *keyboard_name = (tVKbd_Qwerty *)window_add_children(window_addr, WindowKeyboardChildren, #keyboard_name)

#define window_new_window(window_addr, window_name)\
			tWindow *window_name = (tWindow *)window_add_children(window_addr, WindowWindowChildren, #window_name)

#define window_new_tab_group(window_addr, tab_group_name)\
			tWindow *tab_group_name = (tWindow *)window_add_children(window_addr, WindowTabGroupChildren, #tab_group_name)
//#######################################################################################
/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_button(<item name>); before main
 *  and <item name> = window_new_button_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_button_global(window_addr, button_name)\
			(tButton *)window_add_children(window_addr, WindowButtonChildren, #button_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_checkbox(<item name>); before main
 *  and <item name> = window_new_checkbox_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_checkbox_global(window_addr, checkbox_name)\
			(tCheckBox *)window_add_children(window_addr, WindowCheckboxChildren, #checkbox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_listbox(<item name>); before main
 *  and <item name> = window_new_listbox_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_listbox_global(window_addr, listbox_name)\
			(tListBox *)window_add_children(window_addr, WindowListboxChildren, #listbox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_progressbar(<item name>); before main
 *  and <item name> = window_new_progressbar_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_progressbar_global(window_addr, progressbar_name)\
			(tProgressBar *)window_add_children(window_addr, WindowProgressbarChildren, #progressbar_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_scrollbar(<item name>); before main
 *  and <item name> = window_new_scrollbar_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_scrollbar_global(window_addr, scrollbar_name)\
			(tScrollBar *)window_add_children(window_addr, WindowScrollbarChildren, #scrollbar_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_textbox(<item name>); before main
 *  and <item name> = window_new_textbox_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_textbox_global(window_addr, textbox_name)\
			(tTextBox *)window_add_children(window_addr, WindowTextboxChildren, #textbox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_picturebox(<item name>); before main
 *  and <item name> = window_new_picturebox_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_picturebox_global(window_addr, picturebox_name)\
			(tPictureBox *)window_add_children(window_addr, WindowPictureboxChildren, #picturebox_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_vqkbd(<item name>); before main
 *  and <item name> = window_new_keyboard_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_keyboard_global(window_addr, keyboard_name)\
			(tVKbd_Qwerty *)window_add_children(window_addr, WindowKeyboardChildren, #keyboard_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_window(<item name>); before main
 *  and <item name> = window_new_window_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_window_global(window_addr, window_name)\
			(tWindow *)window_add_children(window_addr, WindowWindowChildren, #window_name)

/*
 * Definitions to add new global items to a parent window
 *  to declare a global item pionter write:  _new_tabgroup(<item name>); before main
 *  and <item name> = window_new_tab_group_global(<window name>, <item name>); after used window declaration.
*/
#define window_new_tab_group_global(window_addr, tab_group_name)\
			(tWindow *)window_add_children(window_addr, WindowTabGroupChildren, #tab_group_name)
//#######################################################################################
/* Definitions to add new private items to a parent tab group */
#define tab_group_new_button(window_addr, button_name, tab_nr)\
			tButton *button_name = (tButton *)tab_group_add_children(window_addr, WindowButtonChildren, #button_name, tab_nr)

#define tab_group_new_checkbox(window_addr, checkbox_name, tab_nr)\
			tCheckBox *checkbox_name = (tCheckBox *)tab_group_add_children(window_addr, WindowCheckboxChildren, #checkbox_name, tab_nr)

#define tab_group_new_listbox(window_addr, listbox_name, tab_nr)\
			tListBox *listbox_name = (tListBox *)tab_group_add_children(window_addr, WindowListboxChildren, #listbox_name, tab_nr)

#define tab_group_new_progressbar(window_addr, progressbar_name, tab_nr)\
			tProgressBar *progressbar_name = (tProgressBar *)tab_group_add_children(window_addr, WindowProgressbarChildren, #progressbar_name, tab_nr)

#define tab_group_new_scrollbar(window_addr, scrollbar_name, tab_nr)\
			tScrollBar *scrollbar_name = (tScrollBar *)tab_group_add_children(window_addr, WindowScrollbarChildren, #scrollbar_name, tab_nr)

#define tab_group_new_textbox(window_addr, textbox_name, tab_nr)\
			tTextBox *textbox_name = (tTextBox *)tab_group_add_children(window_addr, WindowTextboxChildren, #textbox_name, tab_nr)

#define tab_group_new_picturebox(window_addr, picturebox_name, tab_nr)\
			tPictureBox *picturebox_name = (tPictureBox *)tab_group_add_children(window_addr, WindowPictureboxChildren, #picturebox_name, tab_nr)

#define tab_group_new_keyboard(window_addr, keyboard_name, tab_nr)\
			tVKbd_Qwerty *keyboard_name = (tVKbd_Qwerty *)tab_group_add_children(window_addr, WindowKeyboardChildren, #keyboard_name, tab_nr)

#define tab_group_new_window(window_addr, window_name, tab_nr)\
			tWindow *window_name = (tWindow *)tab_group_add_children(window_addr, WindowWindowChildren, #window_name, tab_nr)

#define tab_group_new_tab_group(window_addr, window_name, tab_nr)\
			tWindow *window_name = (tWindow *)tab_group_add_children(window_addr, WindowTabGroupChildren, #window_name, tab_nr)
//#######################################################################################
/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_button(<item name>); before main
 *  and tab_group_new_button_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_button_global(window_addr, button_name, tab_nr)\
			(tButton *)tab_group_add_children(window_addr, WindowButtonChildren, #button_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_checkbox(<item name>); before main
 *  and tab_group_new_checkbox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_checkbox_global(window_addr, checkbox_name, tab_nr)\
			(tCheckBox *)tab_group_add_children(window_addr, WindowCheckboxChildren, #checkbox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_listbox(<item name>); before main
 *  and tab_group_new_listbox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_listbox_global(window_addr, listbox_name, tab_nr)\
			(tListBox *)tab_group_add_children(window_addr, WindowListboxChildren, #listbox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_progressbar(<item name>); before main
 *  and tab_group_new_progressbar_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_progressbar_global(window_addr, progressbar_name, tab_nr)\
			(tProgressBar *)tab_group_add_children(window_addr, WindowProgressbarChildren, #progressbar_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_scrollbar(<item name>); before main
 *  and tab_group_new_scrollbar_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_scrollbar_global(window_addr, scrollbar_name, tab_nr)\
			(tScrollBar *)tab_group_add_children(window_addr, WindowScrollbarChildren, #scrollbar_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_textbox(<item name>); before main
 *  and tab_group_new_textbox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_textbox_global(window_addr, textbox_name, tab_nr)\
			(tTextBox *)tab_group_add_children(window_addr, WindowTextboxChildren, #textbox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_picturebox(<item name>); before main
 *  and tab_group_new_picturebox_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_picturebox_global(window_addr, picturebox_name, tab_nr)\
			(tPictureBox *)tab_group_add_children(window_addr, WindowPictureboxChildren, #picturebox_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_vqkbd(<item name>); before main
 *  and tab_group_new_keyboard_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_keyboard_global(window_addr, keyboard_name, tab_nr)\
			(tVKbd_Qwerty *)tab_group_add_children(window_addr, WindowKeyboardChildren, #keyboard_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_window(<item name>); before main
 *  and tab_group_new_window_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_window_global(window_addr, window_name, tab_nr)\
			(tWindow *)tab_group_add_children(window_addr, WindowWindowChildren, #window_name, tab_nr)

/*
 * Definitions to add new global items to a parent tab group
 *  to declare a global item pionter write:  _new_tabgroup(<item name>); before main
 *  and tab_group_new_tab_group_global(<tab group name>, <item name>, <parent tab number>); after used tab group declarations.
*/
#define tab_group_new_tab_group_global(window_addr, window_name, tab_nr)\
			(tWindow *)tab_group_add_children(window_addr, WindowTabGroupChildren, #window_name, tab_nr)
//#######################################################################################
#define _new_window(name) tWindow *name = NULL
//#define free_window(address) free(address);
//#######################################################################################
#define _new_tabgroup(name) tWindow *name = NULL
//#define free_tabgroup(address) free(address);



#endif /* WINDOW_DEF_H_ */
