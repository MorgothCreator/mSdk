/*
 *  lib/gfx/3d.c
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

#include <math.h>
#include "3d.h"
#include "controls_definition.h"
//#######################################################################################
void make3DPoint(_3d_points *_point, unsigned int Cell, double x, double y, double z)
{
	_point->x[Cell] = x;
	_point->y[Cell] = y;
	_point->z[Cell] = z;
}
 //#######################################################################################
void make2DPoint(_3d_points *_point, unsigned int Cell, double x, double y, double depth, double scaleFactor)
{
	_point->x[Cell] = x;
	_point->y[Cell] = y;
	_point->depth[Cell] = depth;
	_point->scaleFactor[Cell] = scaleFactor;
}
//#######################################################################################
void Transform3DPointsTo2DPoints(_3d_points *screenPoints, _3d_points *Points, axisRotations AxisRotations)
{
	double sx = sin(AxisRotations.x);
	double cx = cos(AxisRotations.x);
	double sy = sin(AxisRotations.y);
	double cy = cos(AxisRotations.y);
	double sz = sin(AxisRotations.z);
	double cz = cos(AxisRotations.z);
	volatile double x,y,z, xy,xz, yx,yz, zx,zy, scaleFactor;

	unsigned int i = Points->length;
	while (i--){
		x = Points->x[i];
		y = Points->y[i];
		z = Points->z[i];

		// rotation around x
		xy = cx*y - sx*z;
		xz = sx*y + cx*z;
		// rotation around y
		yz = cy*xz - sy*x;
		yx = sy*xz + cy*x;
		// rotation around z
		zx = cz*yx - sz*xy;
		zy = sz*yx + cz*xy;

		scaleFactor = Points->focalLength /(Points->focalLength + yz);
		x = (zx*scaleFactor) / Points->depthScale;
		y = (zy*scaleFactor) / Points->depthScale;
		z = yz / Points->depthScale;

		make2DPoint(screenPoints, i, x, y, -z, scaleFactor);
	}
	screenPoints->length = Points->length;
}
//#######################################################################################
//#######################################################################################
//#######################################################################################
void put_3d_triangle(tDisplay *pDisplay, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color)
{
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 4;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);

	//double triangle_median = (screenPoints.z[1] + screenPoints.z[2] + screenPoints.z[3]) / 3;
	unsigned int color = Color;//(controlls_change_color(Color, (-triangle_median))<<8) & 0xFFFFFF00;
	//signed int X_start;// = (signed int)screenPoints.x[0];
	//signed int Y_start;// = (signed int)screenPoints.y[0];
	//signed int X_end;// = (signed int)screenPoints.x[1];
	//signed int Y_end;// = (signed int)screenPoints.y[1];
/*	put_line(pDisplay, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[0];
	Y_start = (signed int)screenPoints.y[0];
	X_end = (signed int)screenPoints.x[2];
	Y_end = (signed int)screenPoints.y[2];
	put_line(pDisplay, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[0];
	Y_start = (signed int)screenPoints.y[0];
	X_end = (signed int)screenPoints.x[3];
	Y_end = (signed int)screenPoints.y[3];
	put_line(pDisplay, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);*/

	signed int X_start = (signed int)screenPoints.x[1];
	signed int Y_start = (signed int)screenPoints.y[1];
	signed int X_end = (signed int)screenPoints.x[2];
	signed int Y_end = (signed int)screenPoints.y[2];
	put_line(pDisplay, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[2];
	Y_start = (signed int)screenPoints.y[2];
	X_end = (signed int)screenPoints.x[3];
	Y_end = (signed int)screenPoints.y[3];
	put_line(pDisplay, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);

	X_start = (signed int)screenPoints.x[3];
	Y_start = (signed int)screenPoints.y[3];
	X_end = (signed int)screenPoints.x[1];
	Y_end = (signed int)screenPoints.y[1];
	put_line(pDisplay, X_offset + X_start, Y_offset + Y_start, X_offset + X_end, Y_offset + Y_end, 1, color);
}
//#######################################################################################
void put_3d_rectangle(tDisplay *pDisplay, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color)
{
	_3d_points screenPoints;

	axisRotations cubeAxisRotations;
	cubeAxisRotations.y = Y_Angle;
	cubeAxisRotations.x = X_Angle;
	cubeAxisRotations.z = Z_Angle;
	Points->length = 8;
	Transform3DPointsTo2DPoints(&screenPoints, Points, cubeAxisRotations);
	put_line(pDisplay, (signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[0] + X_offset, (signed int)screenPoints.y[0] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);

	put_line(pDisplay, (signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[2] + X_offset, (signed int)screenPoints.y[2] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);

	put_line(pDisplay, (signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[1] + X_offset, (signed int)screenPoints.y[1] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[5] + X_offset, (signed int)screenPoints.y[5] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);

	put_line(pDisplay, (signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[3] + X_offset, (signed int)screenPoints.y[3] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[4] + X_offset, (signed int)screenPoints.y[4] + Y_offset, 1, Color);
	put_line(pDisplay, (signed int)screenPoints.x[7] + X_offset, (signed int)screenPoints.y[7] + Y_offset, (signed int)screenPoints.x[6] + X_offset, (signed int)screenPoints.y[6] + Y_offset, 1, Color);
}
//#######################################################################################



