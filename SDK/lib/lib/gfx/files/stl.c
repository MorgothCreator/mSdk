/*
 *  lib/files/stl.c
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

#include <stdlib.h>
#include <string.h>
#include "stl.h"
#include "../3d.h"

void stl_get_triangle(_3d_points *triangle_bounds, unsigned char *file, unsigned long triangle_nr)
{

	unsigned char *triangle = (((unsigned char*)file) + 84 + (50 * triangle_nr));
	float tmp;
	memcpy(&tmp, (void*)(triangle + 0) , sizeof(float));
	//tmp = triangle[0] | triangle[1]<<8 | triangle[2]<<16 | triangle[3]<<24;
	triangle_bounds->x[0] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 4), sizeof(float));
	//tmp = triangle[4] | triangle[5]<<8 | triangle[6]<<16 | triangle[7]<<24;
	triangle_bounds->y[0] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 8), sizeof(float));
	//tmp = triangle[8] | triangle[9]<<8 | triangle[10]<<16 | triangle[11]<<24;
	triangle_bounds->z[0] = (double)tmp;

	memcpy(&tmp, (void*)(triangle + 12), sizeof(float));
	//tmp = triangle[12] | triangle[13]<<8 | triangle[14]<<16 | triangle[15]<<24;
	triangle_bounds->x[1] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 16), sizeof(float));
	//tmp = triangle[16] | triangle[17]<<8 | triangle[18]<<16 | triangle[19]<<24;
	triangle_bounds->y[1] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 20), sizeof(float));
	//tmp = triangle[20] | triangle[21]<<8 | triangle[22]<<16 | triangle[23]<<24;
	triangle_bounds->z[1] = (double)tmp;

	memcpy(&tmp, (void*)(triangle + 24), sizeof(float));
	//tmp = triangle[24] | triangle[25]<<8 | triangle[26]<<16 | triangle[27]<<24;
	triangle_bounds->x[2] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 28), sizeof(float));
	//tmp = triangle[28] | triangle[29]<<8 | triangle[30]<<16 | triangle[31]<<24;
	triangle_bounds->y[2] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 32), sizeof(float));
	//tmp = triangle[32] | triangle[33]<<8 | triangle[34]<<16 | triangle[35]<<24;
	triangle_bounds->z[2] = (double)tmp;

	memcpy(&tmp, (void*)(triangle + 36), sizeof(float));
	//tmp = triangle[36] | triangle[37]<<8 | triangle[38]<<16 | triangle[39]<<24;
	triangle_bounds->x[3] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 40), sizeof(float));
	//tmp = triangle[40] | triangle[41]<<8 | triangle[42]<<16 | triangle[43]<<24;
	triangle_bounds->y[3] = (double)tmp;
	memcpy(&tmp, (void*)(triangle + 44), sizeof(float));
	//tmp = triangle[44] | triangle[45]<<8 | triangle[46]<<16 | triangle[47]<<24;
	triangle_bounds->z[3] = (double)tmp;

}

unsigned long stl_get_nr_of_triangles(unsigned char *file)
{
	unsigned long tmp;
	memcpy(&tmp, (void*)(file + (sizeof(stl_header) - sizeof(unsigned long))), sizeof(unsigned long));
	return tmp;
}
