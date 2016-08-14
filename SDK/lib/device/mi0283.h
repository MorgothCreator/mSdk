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
#include "api/gpio_def.h"
/*#####################################################*/
static const unsigned char MI0283_screen_setup_data[]
#ifdef FLASH_DEVICE
													PROGMEM
#endif
													=
{
	0xEA,0x00, 0xEB,0x20, 0xEC,0x0C, 0xED,0xC4, 0xE8,0x40, 0xE9,0x38, 0xF1,0x01, 0xF2,0x10, 0x27,0xA3,
	//power voltage
	0x1B,0x1B, 0x1A,0x01, 0x24,0x2F, 0x25,0x57,
	//VCOM offset
	0x23,0x8D, //for flicker adjust
	/* Gamma settings  -----------------------------------------------------------*/
	0x40,0x00,   //   default setup
	0x41,0x00,   //
	0x42,0x01,   //
	0x43,0x13,   //
	0x44,0x10,   //
	0x45,0x26,   //
	0x46,0x08,   //
	0x47,0x51,   //
	0x48,0x02,   //
	0x49,0x12,   //
	0x4A,0x18,   //
	0x4B,0x19,   //
	0x4C,0x14,   //
	0x50,0x19,   //
	0x51,0x2F,   //
	0x52,0x2C,   //
	0x53,0x3E,   //
	0x54,0x3F,   //
	0x55,0x3F,   //
	0x56,0x2E,   //
	0x57,0x77,   //
	0x58,0x0B,   //
	0x59,0x06,   //
	0x5A,0x07,   //
	0x5B,0x0D,   //
	0x5C,0x1D,   //
	0x5D,0xCC,   //

	//power on
	0x18,0x36, 0x19,0x01, //start osc
	0x01,0x00, //wakeup
	0x1F,0x88, 254, 0x1F,0x80, 254, 0x1F,0x90, 254, 0x1F,0xD0, 254,
	//color selection
	0x17,0x05, //0x0005=65k, 0x0006=262k
	//panel characteristic
	0x36,0x00, 254 ,
	//0x3c, 0xFF,
	//0x3d, 0x04,
	//0x3f, 0xFF,
	//display on
	0x28,0x38, 254, 0x28,0x3C, 255
};
/*#####################################################*/
typedef struct{
	new_gpio* LCD_DATA;
	new_gpio* LCD_BACKLIGHT;
	new_gpio* LCD_TOUCH_DRIVE_A;
	new_gpio* LCD_TOUCH_DRIVE_B;
	new_gpio* LCD_TOUCH_XL;
	new_gpio* LCD_TOUCH_YD;
	new_gpio* LCD_DISPLAY_RST;
	new_gpio* LCD_DISPLAY_RS;
	new_gpio* LCD_DISPLAY_CS;
	new_gpio* LCD_DISPLAY_PMWR;
	new_gpio* LCD_DISPLAY_PMRD;
}MI0283_t;
/*#####################################################*/
void screen_set_area(tDisplay* LcdStruct, signed int x0, signed int y0, signed int x1, signed int y1);
void lcd_cursor(tDisplay* LcdStruct, signed int x, signed int y);
void mi0283_set_params(void *pDisplay);
bool mi0283_open(void *pDisplay);
void mi0283_close(void *pDisplay);
bool mi0283_set_orientation(tDisplay* LcdStruct);
void mi0283_put_pixel16(void *pDisplay, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16);
unsigned short mi0283_get_pixel16(void *pDisplay, signed int X_Coordonate, signed int Y_Coordonate);
void mi0283_put_pixel24(void *pDisplay, signed int X_Coordonate, signed int Y_Coordonate, unsigned char Red, unsigned char Green, unsigned char Blue);
//void screen_fill_area(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);
void mi0283_draw_rectangle(void *pDisplay, signed int x1, signed int y1, signed int x2, signed int y2, bool fill, unsigned int color);
void mi0283_put_horizontal_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void mi0283_put_vertical_line(void *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
void  mi0283_clear(void *pDisplay, unsigned int color);
/*#####################################################*/
#define new_mi0283 MI0283_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_mi0283(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mi0283.c"
#endif
/*#####################################################*/
#endif /* MI0283_H_ */
/*#####################################################*/
