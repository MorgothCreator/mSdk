/*
 *  lib/gfx/gfx_util.c
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

#include "gfx_util.h"
//#######################################################################################
/*unsigned int gfx_modify_color(unsigned int color, signed char modify)
{
	if(modify == 0) return color;
	unsigned char R = color;
	unsigned char G = color>>8;
	unsigned char B = color>>16;
	if (modify > 0)
	{
		R = (R<<modify);
		G = (G<<modify);
		B = (B<<modify);
	} 
	else
	{
		unsigned char _modify = 255-modify;
		R = R>>_modify;
		G = G>>_modify;
		B = B>>_modify;
	}
	return R | (G<<8) | (B<<16);
}*/
//#######################################################
