/*
 *  lib/api/touchscreen_def.h
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

#ifndef TOUCHSCREEN_DEF_H_
#define TOUCHSCREEN_DEF_H_
/*#####################################################*/
#include <stdbool.h>
#include "lcd_def.h"
#include "twi_def.h"
#include "gpio_def.h"
/*#####################################################*/
#define TouchScreen_Type_Int	0
#define TouchScreen_Type_FT5x06	1
#define TouchScreen_Type_AR1020	2
/*#####################################################*/
/*typedef struct
{
	signed int	x1;
	signed int	y1;
	signed int	x2;
	signed int	y2;
	signed int	x3;
	signed int	y3;
	signed int	x4;
	signed int	y4;
	signed int	x5;
	signed int	y5;
	signed int	pressure;
	signed int touch_ID1;
	signed int touch_ID2;
	signed int touch_ID3;
	signed int touch_ID4;
	signed int touch_ID5;
	signed int touch_event1;
	signed int touch_event2;
	signed int touch_event3;
	signed int touch_event4;
	signed int touch_event5;
	unsigned char  touch_point;
}TouchScreen_Data_t;*/
/*#####################################################*/
typedef struct LcdTouch_Struct
{
	double LcdTouch_U_Calibration_Value;
	double LcdTouch_R_Calibration_Value;
	double LcdTouch_D_Calibration_Value;
	double LcdTouch_L_Calibration_Value;
	double LcdTouch_MID_Calibration_Value;
	//double screen_max_x;
	//double screen_max_y;
	unsigned int touch_max_x;
	unsigned int touch_max_y;
	unsigned int press_max;
	unsigned int TouchScreen_Type;
	unsigned char LastState1;
	unsigned char LastState2;
	unsigned char LastState3;
	unsigned char LastState4;
	unsigned char LastState5;
	struct
	{
		signed int	x1;
		signed int	y1;
		signed int	x2;
		signed int	y2;
		signed int	x3;
		signed int	y3;
		signed int	x4;
		signed int	y4;
		signed int	x5;
		signed int	y5;
		signed int	pressure;
		signed int touch_ID1;
		signed int touch_ID2;
		signed int touch_ID3;
		signed int touch_ID4;
		signed int touch_ID5;
		signed int touch_event1;
		signed int touch_event2;
		signed int touch_event3;
		signed int touch_event4;
		signed int touch_event5;
		unsigned char  touch_point;
	}TouchResponse;
	bool flip_x;
	bool flip_y;
	unsigned char twi_addr;
	tDisplay *pDisplay;
	new_twi* TwiStruct;
	new_gpio* IrqStruct;
	void *UsrData;
} LcdTouch_t;
/*#####################################################*/
#define new_touchscreen LcdTouch_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_touchscreen(address) free(address);\
		address = NULL
/*#####################################################*/
#endif /* TOUCHSCREEN_DEF_H_ */
