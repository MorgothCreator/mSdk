/*
 *  lib/gfx/textbox.h
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

#ifndef TEXTBOX_H_
#define TEXTBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "textbox_def.h"
#include "scrollbar.h"
#include "scrollbar_def.h"
#include "controls_definition.h"
//#include "window_def.h"
//#######################################################################################
void textbox(void *_settings, tControlCommandData* control_comand);
tTextBox *new_textbox(void *ParentWindow);
bool free_textbox(void* settings);
void textbox_text(void* _settings, char* string);
void textbox_text_append(void* _settings, char* string);
void textbox_text_insert(void* _settings, char* string, unsigned int location);
void textbox_text_to_uper(void* _settings);
void textbox_text_to_lower(void* _settings);
void textbox_font(void* _settings, tFont* font);
void textbox_wordwrap(void* _settings, bool word_wrap);
void textbox_align(void* _settings, bool align);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "textbox.c"
#endif
//#######################################################################################
#endif /* TEXTBOX_H_ */
//#######################################################################################
