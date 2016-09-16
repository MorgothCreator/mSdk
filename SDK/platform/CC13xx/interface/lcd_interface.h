/*
 * lcd_interface.h
 *
 * Created: 2/8/2013 10:26:56 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/lcd_def.h"
/*#####################################################*/
bool SetUpLCD(void* LcdStruct);
void _lcd_enable();
void _lcd_disable();
void _screen_backlight_on(void *pDisplay);
void _screen_backlight_off(void *pDisplay);
bool _screen_copy(void *pDisplayTo, void *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color);
void _box_cache_clean(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len);
void _put_rectangle(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color);
void _put_pixel(void *pDisplay, signed int X, signed int Y, unsigned int color);
void _screen_put_horizontal_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void _screen_put_vertical_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void _screen_put_rgb_array_16(void *pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void _screen_put_rgb_array_24(void *pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height);
void _screen_put_rgb_array_32(void *pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void _screen_clear(void *pDisplay, unsigned int color);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "lcd_interface.c"
#endif
/*#####################################################*/
#endif /* LCD_INTERFACE_H_ */
/*#####################################################*/
