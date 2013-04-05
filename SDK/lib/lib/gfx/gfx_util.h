/*
 *  lib/gfx/gfx_util.h
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

#ifndef __Gfx_Utility_H__
#define __Gfx_Utility_H__
//#######################################################
#define RGB_TO_UINT(R,G,B) ((R)<<16) | ((G)<<8) | ((B))
//#######################################################
// Utility to divide or multiply the base color
//unsigned int gfx_modify_color(unsigned int color, signed char modify);
//#######################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "gfx_util.c"
#endif
//#######################################################
#endif
