/*
 *  lib/gfx/graphic_string.h
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

#ifndef GRAPHIC_STRING_H_
#define GRAPHIC_STRING_H_
/*#####################################################*/
#include "api/lcd_def.h"
#include "controls_definition.h"
/*#####################################################*/
char *gfx_change_str(char *dest, char *src);
bool put_roll_string(tDisplay *pDisplay, graphic_strings_t *StringsStruct);
#ifdef FLASH_DEVICE
bool put_roll_string_P(tDisplay *pDisplay, graphic_strings_t *StringsStruct);
#endif
/*#####################################################*/
#define new_roll_str strings_t
#ifndef new_
#define new_(structure) (structure*)calloc(1, sizeof(structure));
#endif
#define free_roll_str(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "graphic_string.c"
#endif
/*#####################################################*/
#endif /* GRAPHIC_STRING_H_ */
/*#####################################################*/

