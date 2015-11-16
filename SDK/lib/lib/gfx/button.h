/*
 *  lib/gfx/buton.h
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

#ifndef BUTON_H_
#define BUTON_H_
//#######################################################################################
#include <stdbool.h>
#include "api/timer_api.h"
#include "controls_definition.h"
#include "button_def.h"
//#include "window_def.h"
//#######################################################################################
void button(tButton *settings, tControlCommandData* control_comand);
tButton *new_button(void *ParentWindow);
bool free_button(tButton* settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "button.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
