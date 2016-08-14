/*
 *  lib/lib/gfx/paint.c
 *
 *  Copyright (C) 2015  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  Created on: Jan 9, 2015
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
/*#####################################################*/
#include "stdbool.h"
#include "gfx_gui_paint.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "controls_definition.h"
/*#####################################################*/
Gfx_res_t sys_def_gui_res;
/*#####################################################*/
void _gui_put_simple_item(signed int x, signed int y, signed int x_size, signed int y_size, unsigned char fill, unsigned int color)
{
	signed int radius = sys_def_gui_res.corner_radius;
	if(radius < 0) radius = ~radius;
	signed int a, b, P;
	void *pDisplay = sys_def_gui_res.pDisplay;
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	signed int __x_size = x_size, __y_size = y_size;
	if(__x_size < 0) __x_size = ~__x_size;
	if(__y_size < 0) __y_size = ~__y_size;
	unsigned int _color = color;// << 8;

   a = 0;
   b = radius;
   P = 1 - radius;
   signed int Tmp1;
   signed int Tmp2;
   signed int Tmp3;
   signed int Tmp4;
   signed int Tmp5;
   signed int Tmp6;
   signed int Tmp7;
   signed int Tmp8;
   signed int _Tmp5 = 5, _Tmp7 = 0;

   do
   {
	      Tmp1 = ((x + __x_size + a)- radius - 1);
	      Tmp2 = ((x + radius) - a);
	      Tmp3 = ((x + __x_size + b)- radius - 1);
	      Tmp4 = ((x + radius) - b);
	      Tmp5 = ((y + __y_size + a)- radius - 1);
	      Tmp6 = ((y + radius) - a);
	      Tmp7 = ((y + __y_size + b)- radius - 1);
	      Tmp8 = ((y + radius) - b);
	  if(fill)
      {

		 if(_Tmp7 != Tmp7)
		 {
			 LcdStruct->lcd_func.put_horizontal_line(pDisplay,Tmp2, Tmp1 - Tmp2, Tmp7, 1, color);
			 LcdStruct->lcd_func.put_horizontal_line(pDisplay,Tmp2, Tmp1 - Tmp2, Tmp8, 1, color);
		 }
         if(_Tmp5 != Tmp5)
		 {
        	 LcdStruct->lcd_func.put_horizontal_line(pDisplay,Tmp4, Tmp3 - Tmp4, Tmp5, 1, color);
        	 LcdStruct->lcd_func.put_horizontal_line(pDisplay,Tmp4, Tmp3 - Tmp4, Tmp6, 1, color);
		 }
      _Tmp5 = Tmp5;
      _Tmp7 = Tmp7;
      }
      else
      {
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp1, Tmp7, _color);
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp3, Tmp5, _color);
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp2, Tmp7, _color);
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp4, Tmp5, _color);
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp3, Tmp6, _color);
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp1, Tmp8, _color);
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp2, Tmp8, _color);
    	  LcdStruct->lcd_func.put_pixel(pDisplay,Tmp4, Tmp6, _color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
   if(!fill)
   {
	   LcdStruct->lcd_func.put_horizontal_line(pDisplay ,x + radius + 1, x_size - (radius + radius) - 2, y, 1, color);
	   LcdStruct->lcd_func.put_horizontal_line(pDisplay ,x + radius + 1, x_size - (radius + radius) - 2, (y + y_size) - 1, 1, color);
	   LcdStruct->lcd_func.put_vertical_line(pDisplay ,y + radius + 1, y_size - (radius + radius) - 2, x, 1, color);
	   LcdStruct->lcd_func.put_vertical_line(pDisplay ,y + radius + 1, y_size - (radius + radius) - 2, (x + x_size) - 1, 1, color);
   } else {
	   LcdStruct->lcd_func.put_rectangle(pDisplay, x, y + radius + 1, x_size, y_size - (radius + radius) - 2, true, color);
   }
}
/*#####################################################*/
void gui_put_item(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, unsigned int int_color, unsigned int border_color, CursorState cursor, GUI_PAINT_STYLE style, bool enabled)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	sys_def_gui_res.pDisplay = pDisplay;
	switch(style) {
		case PAINT_STYLE_ROUNDED_CORNERS:
			sys_def_gui_res.corner_radius = 5;
			_gui_put_simple_item(x_start, y_start, x_len, y_len, true, int_color);
			_gui_put_simple_item(x_start, y_start, x_len, y_len, false, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
			break;
		case PAINT_STYLE_DEFAULT:
		default:
			if(enabled)
			{
				switch(cursor) {
					case Cursor_Down:
					case Cursor_Move:
						LcdStruct->lcd_func.put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, int_color);
						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start, x_len, y_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));

						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));
						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));

						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start, y_len, x_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));

						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));
						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));
						break;
					default:
						LcdStruct->lcd_func.put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, int_color);
						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start, x_len, y_start, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));
						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));

						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));
						LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));

						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start, y_len, x_start, 1, controlls_change_color(border_color, +BORDER_LINE_ONE_LIGHT));
						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(border_color, +BORDER_LINE_TWO_LIGHT));

						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_DARK));
						LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_DARK));
						break;
				}
			} else
			{
				LcdStruct->lcd_func.put_rectangle(pDisplay, x_start + 2, y_start + 2, x_len - 4, y_len - 4, true, int_color);
				LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start, x_len, y_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));
				LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start + 1, x_len - 2, y_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));

				LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start + 1, x_len - 2, y_start + y_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));
				LcdStruct->lcd_func.put_horizontal_line(pDisplay, x_start, x_len, (y_start + y_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));

				LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start, y_len, x_start, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));
				LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start + 1, y_len - 2, x_start + 1, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));

				LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start + 1, y_len - 2, x_start + x_len - 2, 1, controlls_change_color(border_color, -BORDER_LINE_TWO_LIGHT));
				LcdStruct->lcd_func.put_vertical_line(pDisplay, y_start, y_len, (x_start + x_len) - 1, 1, controlls_change_color(border_color, -BORDER_LINE_ONE_LIGHT));
		}
	}
}
/*#####################################################*/
bool gui_paint_def_init(void) {
	return false;
}
/*#####################################################*/
bool gui_paint_item(Gfx_paint_res_t *resource) {
	return false;
}
/*#####################################################*/
/*#####################################################*/



