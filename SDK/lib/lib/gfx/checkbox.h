/*
 *  lib/gfx/checkbox.h
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

#ifndef CHECKBOX_H_
#define CHECKBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "controls_definition.h"
#include "checkbox_def.h"
//#include "window_def.h"
//#######################################################################################
//#######################################################################################
void checkbox(tCheckBox *settings, tControlCommandData* control_comand);
tCheckBox *new_checkbox(void *ParentWindow);
bool free_checkbox(tCheckBox* settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "checkbox.c"
#endif
//#######################################################################################
#endif /* CHECKBOX_H_ */
