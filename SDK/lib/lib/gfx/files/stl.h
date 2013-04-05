/*
 *  lib/files/stl.h
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

#ifndef STL_H_
#define STL_H_

#include "../3d.h"

typedef struct
{
	unsigned char Header[80];
	unsigned long TriangleNr;
}stl_header;

typedef struct
{
	struct
	{
		float x;
		float y;
		float z;
	}NormalVector;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex1;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex2;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex3;
	unsigned short Attribute;
}stl_solid_t;

void stl_get_triangle(_3d_points *triangle_bounds, unsigned char *file, unsigned long triangle_nr);
unsigned long stl_get_nr_of_triangles(unsigned char *file);

#ifdef HEADER_INCLUDE_C_FILES
#include "stl.c"
#endif

#endif /* STL_H_ */
