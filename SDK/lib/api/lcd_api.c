/*
 *  lib/api/lcd_api.c
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

#include <stdbool.h>
#include "lcd_api.h"
#include "lcd_def.h"
#include "interface/lcd_interface.h"
/**********************************************/
bool screen_init(tDisplay* LcdStruct)
{
	return SetUpLCD(LcdStruct);
}
/**********************************************/
void lcd_enable()
{
	_lcd_enable();
}
/**********************************************/
void lcd_disable()
{
	_lcd_disable();
}
/**********************************************/
void screen_backlight_on(tDisplay *pDisplay)
{
	_screen_backlight_on(pDisplay);
}

void screen_backlight_off(tDisplay *pDisplay)
{
	_screen_backlight_off(pDisplay);
}

void screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	_screen_put_rgb_array_16(_pDisplay, rgb_buffer, x1, y1,width, height);
}

void screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{
	_screen_put_rgb_array_24(_pDisplay, rgb_buffer, x1, y1, width, height);
}

void screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	_screen_put_rgb_array_32(_pDisplay, rgb_buffer, x1, y1, width, height);
}

void box_cache_clean(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{
	_box_cache_clean(pDisplay, x_start, y_start, x_len, y_len);
}

void put_rectangle(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	_put_rectangle(pDisplay, x_start, y_start, x_len, y_len, fill, color);
}

void put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color)
{
	_put_pixel(pDisplay, X, Y, color);
}

void screen_put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	_screen_put_horizontal_line(pDisplay, X1, X2, Y, width, color);
}

void screen_put_vertical_line(tDisplay *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	_screen_put_vertical_line(pDisplay, Y1, Y2, X, width, color);
}

