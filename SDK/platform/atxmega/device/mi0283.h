/*
 * mi0283.h
 *
 * Created: 2/8/2013 11:38:37 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef MI0283_H_
#define MI0283_H_
/*#####################################################*/
#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include <stdbool.h>
#include "board_properties.h"
#include "api/lcd_def.h"
/*#####################################################*/
static const unsigned char MI0283_screen_setup_data[] PROGMEM = 
{
	0xEA,0x00, 0xEB,0x20, 0xEC,0x0C, 0xED,0xC4, 0xE8,0x40, 0xE9,0x38, 0xF1,0x01, 0xF2,0x10, 0x27,0xA3,
	//power voltage
	0x1B,0x1B, 0x1A,0x01, 0x24,0x2F, 0x25,0x57,
	//VCOM offset
	0x23,0x8D, //for flicker adjust
	//power on
	0x18,0x36, 0x19,0x01, //start osc
	0x01,0x00, //wakeup
	0x1F,0x88, 254, 0x1F,0x80, 254, 0x1F,0x90, 254, 0x1F,0xD0, 254,
	//color selection
	0x17,0x05, //0x0005=65k, 0x0006=262k
	//panel characteristic
	0x36,0x00, 254 ,
	//lcd_cmd(0x3c, 0x00FF);
	//lcd_cmd(0x3d, 0x0004);
	//lcd_cmd(0x3f, 0x00FF);
	//display on
	0x28,0x38, 254, 0x28,0x3C, 255
};
/*#####################################################*/
void screen_set_area(signed int x0, signed int y0, signed int x1, signed int y1);
void lcd_cursor(signed int x, signed int y);
void screen_open(tDisplay* LcdStruct);
void screen_close(tDisplay* LcdStruct);
bool screen_set_orientation(tDisplay* LcdStruct);
void screen_put_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, uint16_t Color16);
uint16_t screen_get_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, uint16_t Color16);
void screen_put_pixel24(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned char Red, unsigned char Green, unsigned char Blue);
//void screen_fill_area(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);
void screen_draw_rectangle(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, bool fill, unsigned int color);
void screen_put_horizontal_line_(tDisplay *LcdStruct, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void screen_put_vertical_line_(tDisplay *LcdStruct, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mi0283.c"
#endif
/*#####################################################*/
#endif /* MI0283_H_ */
/*#####################################################*/
