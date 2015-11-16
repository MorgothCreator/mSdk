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
#include "listbox_def.h"
#include "scrollbar.h"
#include "scrollbar_def.h"
#include "controls_definition.h"
//#include "window_def.h"
//#######################################################################################
void listbox(tListBox *settings, tControlCommandData* control_comand);
tListBox *new_listbox(void *ParentWindow);
bool free_listbox(void* _settings);
void* listbox_item_insert(void* _settings, char* text, unsigned int location);
void* listbox_item_add(void* _settings, char* text);
bool listbox_item_remove(void* _settings, unsigned int location);
bool listbox_item_remove_all(void* _settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "listbox.c"
#endif
//#######################################################################################
#endif /* LISTBOX_H_ */
