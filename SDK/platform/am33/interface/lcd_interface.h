/*
 * lcd_interface.h
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/**********************************************/
#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_
/**********************************************/
#include <stdbool.h>
#include "api/lcd_def.h"
/**********************************************/
extern tDisplay* ScreenRander;
/**********************************************/
/*
** Values that can be passed to RasterDMAConfig API as endian  to enable or
** disable bigendian for data reordering
*/

/* Enable bigendian for data reordering */
#define RASTER_BYTE_SWAP_ENABLE           (1 << 3)

/* Disable bigendian for data reordering */
#define RASTER_BYTE_SWAP_DISABLE           0


bool SetUpLCD(tDisplay* LcdStruct);
void _lcd_enable();
void _lcd_disable();
void _screen_backlight_on(tDisplay *pDisplay);
void _screen_backlight_off(tDisplay *pDisplay);
void _screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void _screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height);
void _screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void _box_cache_clean(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len);
void _put_rectangle(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color);
void _put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color);
void _screen_put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void _screen_put_vertical_line(tDisplay *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "lcd_interface.c"
#endif
/**********************************************/
#endif /* LCD_INTERFACE_H_ */
/**********************************************/
