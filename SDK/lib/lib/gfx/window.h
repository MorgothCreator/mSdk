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
#include "window_def.h"
#include "button.h"
#include "checkbox.h"
#include "listbox.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "textbox.h"
#include "picturebox.h"
#include "keyboard.h"
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
tWindow *new_window(void *ParentWindow, tDisplay *ScreenDisplay);
tWindow *new_tab_group(void *ParentWindow, tDisplay *ScreenDisplay);
signed int tab_group_new_tab(struct Window_s *settings, char *tab_name);

void window(struct Window_s *settings, tControlCommandData *control_comand);
void* window_add_children(struct Window_s *settings, unsigned int children_type, char *children_name);
void* tab_group_add_children(struct Window_s *settings, unsigned int children_type, char *children_name, unsigned int tab_nr);
signed int window_get_children_index(struct Window_s *settings, char *name);
window_children_t *window_get_children_address(struct Window_s *settings, char *name);
bool window_set_list_of_childrens(struct Window_s *settings, window_children_t **list, unsigned int num_of_childrens);
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
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "window.c"
#endif
//#######################################################################################
#endif /* WINDOW_H_ */
//#######################################################################################

