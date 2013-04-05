/*
 *  lib/api/lcd_api.h
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

#ifndef LCD_API_H_
#define LCD_API_H_
/**********************************************/
#include <stdbool.h>
#include "lcd_def.h"
#include "interface/lcd_interface.h"
/**********************************************/
bool screen_init(tDisplay* LcdStruct);
void lcd_enable();
void lcd_disable();
void screen_backlight_on(tDisplay *pDisplay);
void screen_backlight_off(tDisplay *pDisplay);
void screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height);
void screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void box_cache_clean(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len);
void put_rectangle(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color);
void put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color);
void screen_put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void screen_put_vertical_line(tDisplay *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "lcd_api.c"
#endif
/**********************************************/
#endif /* LCD_API_H_ */
/**********************************************/
