/*
 *  lib/gfx/keyboard.h
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

#ifndef VKBD_QWERTY_H_
#define VKBD_QWERTY_H_
//#######################################################################################
#include <stdbool.h>
#include "keyboard_def.h"
#include "api/timer_api.h"
#include "controls_definition.h"
//#include "window_def.h"
//#######################################################################################
void v_keyboard(tVKbd_Qwerty *settings, tControlCommandData* control_comand);
tVKbd_Qwerty *new_v_keyboard(void *ParentWindow);
bool free_v_keyboard(tVKbd_Qwerty* settings);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "keyboard.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
