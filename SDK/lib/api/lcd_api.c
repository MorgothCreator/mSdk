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
#include "lib/gfx/controls_definition.h"
/**********************************************/
void lcd_set_params(void* pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.init = SetUpLCD;
	LcdStruct->lcd_func.enable = _lcd_enable;
	LcdStruct->lcd_func.disable = _lcd_disable;
	LcdStruct->lcd_func.backlight_on = _screen_backlight_on;
	LcdStruct->lcd_func.backlight_off = _screen_backlight_off;
	//LcdStruct->lcd_func.copy = screen_copy;
	LcdStruct->lcd_func.put_rgb_array_16 = _screen_put_rgb_array_16;
	LcdStruct->lcd_func.put_rgb_array_24 = _screen_put_rgb_array_24;
	LcdStruct->lcd_func.put_rgb_array_32 = _screen_put_rgb_array_32;
	LcdStruct->lcd_func.put_rectangle = _put_rectangle;
	LcdStruct->lcd_func.put_pixel = _put_pixel;
	LcdStruct->lcd_func.put_horizontal_line = _screen_put_horizontal_line;
	LcdStruct->lcd_func.put_vertical_line = _screen_put_vertical_line;
	LcdStruct->lcd_func.box_cache_clean = _box_cache_clean;
	LcdStruct->lcd_func.clear = _screen_clear;

	LcdStruct->lcd_func.put_circle = put_circle;
	LcdStruct->lcd_func.put_line = put_line;
	LcdStruct->lcd_func.put_elipse = put_elipse;
	LcdStruct->lcd_func.put_triangle = put_triangle;
	LcdStruct->lcd_func.put_string = put_string;
}
/**********************************************/
bool lcd_init(void* pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	return LcdStruct->lcd_func.init(LcdStruct);
}
/**********************************************/
void lcd_enable(void* pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.enable(LcdStruct);
	//_lcd_enable();
}
/**********************************************/
void lcd_disable(void* pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.disable(LcdStruct);
	//_lcd_disable();
}
/**********************************************/
void lcd_backlight_on(void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.backlight_on(pDisplay);
	//_screen_backlight_on(pDisplay);
}

void lcd_backlight_off(void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.backlight_off(pDisplay);
	//_screen_backlight_off(pDisplay);
}

bool screen_copy(void *pDisplayTo, void *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplayFrom;
	return LcdStruct->lcd_func.copy(pDisplayTo, pDisplayFrom, put_cursor, X, Y, color);
	//return _screen_copy(pDisplayTo, pDisplayFrom, put_cursor, X, Y, color);
}
void lcd_put_rgb_array_16(void *pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_rgb_array_16(pDisplay, rgb_buffer, x1, y1,width, height);
	//_screen_put_rgb_array_16(_pDisplay, rgb_buffer, x1, y1,width, height);
}

void lcd_put_rgb_array_24(void *pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_rgb_array_24(pDisplay, rgb_buffer, x1, y1,width, height);
	//_screen_put_rgb_array_24(_pDisplay, rgb_buffer, x1, y1, width, height);
}

void lcd_put_rgb_array_32(void *pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_rgb_array_32(pDisplay, rgb_buffer, x1, y1,width, height);
	//_screen_put_rgb_array_32(_pDisplay, rgb_buffer, x1, y1, width, height);
}

void lcd_box_cache_clean(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.box_cache_clean(pDisplay, x_start, y_start, x_len, y_len);
	//_box_cache_clean(pDisplay, x_start, y_start, x_len, y_len);
}

void lcd_put_rectangle(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_rectangle(pDisplay, x_start, y_start, x_len, y_len, fill, color);
	//_put_rectangle(pDisplay, x_start, y_start, x_len, y_len, fill, color);
}

void lcd_put_pixel(void *pDisplay, signed int X, signed int Y, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_pixel(pDisplay, X, Y, color);
	//_put_pixel(pDisplay, X, Y, color);
}

void lcd_put_horizontal_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_horizontal_line(pDisplay, X1, X2, Y, width, color);
	//_screen_put_horizontal_line(pDisplay, X1, X2, Y, width, color);
}

void lcd_put_vertical_line(void *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_vertical_line(pDisplay, Y1, Y2, X, width, color);
	//_screen_put_vertical_line(pDisplay, Y1, Y2, X, width, color);
}

void lcd_clear(void *pDisplay, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.clear(pDisplay, color);
	//_screen_clear(pDisplay, color);
}

