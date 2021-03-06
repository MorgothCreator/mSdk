/*
 * mi0283.c
 *
 * Created: 2/8/2013 11:38:47 PM
 *  Author: XxXx
 */ 
//#######################################################################################
#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
//#include <util/delay.h>
#include <stdbool.h>
//#include <util/atomic.h>
//#include "general/unions.h"
#include "mi0283.h"
#include "api/gpio_api.h"
#include "board_init.h"
//#include "board_properties.h"
//#include "driver/ebi.h"
//#include "driver/dma.h"
#include "api/lcd_def.h"
//#######################################################################################
extern unsigned long FCPU;
//#######################################################################################

enum {
	MI0283_ILI9341_CTRL,
	MI0283_ST7789V_CTRL
}mi0283_ctl_type;

//#######################################################################################
static void wr_cmd(tDisplay* LcdStruct, unsigned char cmd)
{
	volatile unsigned long cnt = FCPU / 5000000;
	while(cnt--);
	MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	unsigned int GPIORW = Struct->LCD_DISPLAY_PMWR->BaseAddr;
	unsigned int GPIODAT = Struct->LCD_DATA->BaseAddr;
	unsigned char PINRW = Struct->LCD_DISPLAY_PMWR->Pin;
	unsigned char PINDAT = Struct->LCD_DATA->Pin;
	gpio.out(Struct->LCD_DISPLAY_RS, 0);
	gpio.out(Struct->LCD_DISPLAY_CS, 0);
	GPIO_FAST_WRITE_MULTI_PIN(GPIODAT, PINDAT, cmd);
	GPIO_FAST_CLEAR_PIN(GPIORW, PINRW);
	GPIO_FAST_SET_PIN(GPIORW, PINRW);
	gpio.out(Struct->LCD_DISPLAY_RS, 1);
	cnt = FCPU / 5000000;
	while(cnt--);
}

static void write(tDisplay* LcdStruct, unsigned char data)
{
	MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	unsigned int GPIORW = Struct->LCD_DISPLAY_PMWR->BaseAddr;
	unsigned int GPIODAT = Struct->LCD_DATA->BaseAddr;
	unsigned char PINRW = Struct->LCD_DISPLAY_PMWR->Pin;
	unsigned char PINDAT = Struct->LCD_DATA->Pin;
	GPIO_FAST_WRITE_MULTI_PIN(GPIODAT, PINDAT, data);
	GPIO_FAST_CLEAR_PIN(GPIORW, PINRW);
	GPIO_FAST_SET_PIN(GPIORW, PINRW);
}

static unsigned int read(tDisplay* LcdStruct) {
	unsigned int *dataPort;

	MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	gpio.out(Struct->LCD_DISPLAY_RS, 1);
	gpio.out(Struct->LCD_DISPLAY_PMRD, 0);
	volatile unsigned long cnt = 5;
	while(cnt--);
	dataPort = (unsigned int*)gpio.in(Struct->LCD_DATA);
	gpio.out(Struct->LCD_DISPLAY_PMRD, 1);
	return *dataPort;
}

void mi0283_reset(void* LcdStruct)
{
	tDisplay* _LcdStruct = (tDisplay *) LcdStruct;
	MI0283_t *Struct = (MI0283_t *)_LcdStruct->UserData;
	gpio.out(Struct->LCD_DISPLAY_CS, 1);
	gpio.out(Struct->LCD_DISPLAY_RS, 1);
	gpio.out(Struct->LCD_DISPLAY_RST, 0);

	volatile unsigned long cnt = FCPU / 100000;
	while(cnt--);
	gpio.out(Struct->LCD_DISPLAY_RST, 1);
	cnt = FCPU / 1000000;
	while(cnt--);

    wr_cmd(LcdStruct, 0x01);                     // SW reset
 	gpio.out(Struct->LCD_DISPLAY_CS, 1);
	cnt = FCPU / 100000;
	while(cnt--);

	unsigned long id = 0;
	// read hw ID
	wr_cmd(LcdStruct, 0x04);
	gpio.function_set(Struct->LCD_DATA, GPIO_IN_PULL_UP);
	read(LcdStruct);
	id |= (unsigned long)(read(LcdStruct) & 0x00FF) << 8;
	id |= (unsigned long)(read(LcdStruct) & 0x00FF) << 16;
	id |= (unsigned long)(read(LcdStruct) & 0x00FF) << 24;
 	gpio.out(Struct->LCD_DISPLAY_CS, 1);
	gpio.function_set(Struct->LCD_DATA, GPIO_OUT_PUSH_PULL);
	if(id == 0x1c4c400)
		mi0283_ctl_type = MI0283_ST7789V_CTRL;
	else
		mi0283_ctl_type = MI0283_ILI9341_CTRL;


    wr_cmd(LcdStruct, 0x28);                     // display off
 	gpio.out(Struct->LCD_DISPLAY_CS, 1);

    wr_cmd(LcdStruct, 0x11);
 	gpio.out(Struct->LCD_DISPLAY_CS, 1);


    /* Start Initial Sequence ----------------------------------------------------*/
 	cnt = FCPU / 100;
 	while(cnt--);

 	if(mi0283_ctl_type == MI0283_ILI9341_CTRL)
 	{
		wr_cmd(LcdStruct, 0xCF);
		write(LcdStruct, 0x00);
		write(LcdStruct, 0x83);
		write(LcdStruct, 0x30);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xED);
		write(LcdStruct, 0x64);
		write(LcdStruct, 0x03);
		write(LcdStruct, 0x12);
		write(LcdStruct, 0x81);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xE8);
		write(LcdStruct, 0x85);
		write(LcdStruct, 0x01);
		write(LcdStruct, 0x79);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xCB);
		write(LcdStruct, 0x39);
		write(LcdStruct, 0x2C);
		write(LcdStruct, 0x00);
		write(LcdStruct, 0x34);
		write(LcdStruct, 0x02);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xF7);
		write(LcdStruct, 0x20);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xEA);
		write(LcdStruct, 0x00);
		write(LcdStruct, 0x00);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xC0);                     // POWER_CONTROL_1
		write(LcdStruct, 0x26);
		//write(LcdStruct, 0x06);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xC1);                     // POWER_CONTROL_2
		write(LcdStruct, 0x11);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xC5);                     // VCOM_CONTROL_1
		write(LcdStruct, 0x35);
		write(LcdStruct, 0x3E);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xC7);                     // VCOM_CONTROL_2
		write(LcdStruct, 0xBE);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0x36);                     // MEMORY_ACCESS_CONTROL
		write(LcdStruct, 0x48);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0x3A);                     // COLMOD_PIXEL_FORMAT_SET
		write(LcdStruct, 0x55);                 // 16 bit pixel
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		//wr_cmd(LcdStruct, 0x17);                     //0x0005=65k, 0x0006=262k
		//write(LcdStruct, 0x05);
		//gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xB1);                     // Frame Rate
		write(LcdStruct, 0x00);
		write(LcdStruct, 0x1B);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xF2);                     // Gamma Function Disable
		write(LcdStruct, 0x08);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0x26);
		write(LcdStruct, 0x01);                 // gamma set for curve 01/2/04/08
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xE0);                     // positive gamma correction
		write(LcdStruct, 0x1F);
		write(LcdStruct, 0x1A);
		write(LcdStruct, 0x18);
		write(LcdStruct, 0x0A);
		write(LcdStruct, 0x0F);
		write(LcdStruct, 0x06);
		write(LcdStruct, 0x45);
		write(LcdStruct, 0x87);
		write(LcdStruct, 0x32);
		write(LcdStruct, 0x0A);
		write(LcdStruct, 0x07);
		write(LcdStruct, 0x02);
		write(LcdStruct, 0x07);
		write(LcdStruct, 0x05);
		write(LcdStruct, 0x00);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xE1);                     // negativ gamma correction
		write(LcdStruct, 0x00);
		write(LcdStruct, 0x25);
		write(LcdStruct, 0x27);
		write(LcdStruct, 0x05);
		write(LcdStruct, 0x10);
		write(LcdStruct, 0x09);
		write(LcdStruct, 0x3A);
		write(LcdStruct, 0x78);
		write(LcdStruct, 0x4D);
		write(LcdStruct, 0x05);
		write(LcdStruct, 0x18);
		write(LcdStruct, 0x0D);
		write(LcdStruct, 0x38);
		write(LcdStruct, 0x3A);
		write(LcdStruct, 0x1F);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xB7);                       // entry mode
		write(LcdStruct, 0x07);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		wr_cmd(LcdStruct, 0xB6);                       // display function control
		write(LcdStruct, 0x0A);
		write(LcdStruct, 0x82);
		write(LcdStruct, 0x27);
		write(LcdStruct, 0x04);
		gpio.out(Struct->LCD_DISPLAY_CS, 1);

		mi0283_screen_set_area(LcdStruct, 0, 0, _LcdStruct->LcdTimings->X, _LcdStruct->LcdTimings->Y);
	 	wr_cmd(LcdStruct, 0x11);                     // sleep out
	 	gpio.out(Struct->LCD_DISPLAY_CS, 1);

	 	cnt = FCPU / 1000;
	 	while(cnt--);

	    wr_cmd(LcdStruct, 0x29);                     // display on
	 	gpio.out(Struct->LCD_DISPLAY_CS, 1);
 	}
 	else if(mi0283_ctl_type == MI0283_ST7789V_CTRL)
 	{
 		  wr_cmd(LcdStruct, 0x36);
 		  write(LcdStruct, 0x00);

 		  wr_cmd(LcdStruct, 0x3a);
 		  write(LcdStruct, 0x05);  //65k
 		  //--------------------------------ST7789V Frame rate setting----------------------------------//
 		  wr_cmd(LcdStruct, 0xb2);
 		  write(LcdStruct, 0x0c);
 		  write(LcdStruct, 0x0c);
 		  write(LcdStruct, 0x00);
 		  write(LcdStruct, 0x33);
 		  write(LcdStruct, 0x33);

 		  wr_cmd(LcdStruct, 0xb7);
 		  write(LcdStruct, 0x70);

 		  //---------------------------------ST7789V Power setting--------------------------------------//
 		  wr_cmd(LcdStruct, 0xbb);
 		  write(LcdStruct, 0x1B);//VCOM

 		  wr_cmd(LcdStruct, 0xc0);
 		  write(LcdStruct, 0x2c);

 		  wr_cmd(LcdStruct, 0xc2);
 		  write(LcdStruct, 0x01);

 		  wr_cmd(LcdStruct, 0xc3);
 		  write(LcdStruct, 0x0B);

 		  wr_cmd(LcdStruct, 0xc4);
 		  write(LcdStruct, 0x27);

 		  wr_cmd(LcdStruct, 0xc6);
 		  write(LcdStruct, 0x0f);

 		  wr_cmd(LcdStruct, 0xd0);
 		  write(LcdStruct, 0xa4);
 		  write(LcdStruct, 0xA1);
 		  //--------------------------------ST7789V gamma setting---------------------------------------//
 		  wr_cmd(LcdStruct, 0xe0);
 		  write(LcdStruct, 0xD0);
 		  write(LcdStruct, 0x06);
 		  write(LcdStruct, 0x0B);
 		  write(LcdStruct, 0x09);
 		  write(LcdStruct, 0x08);
 		  write(LcdStruct, 0x30);
 		  write(LcdStruct, 0x30);
 		  write(LcdStruct, 0x5B);
 		  write(LcdStruct, 0x4B);
 		  write(LcdStruct, 0x18);
 		  write(LcdStruct, 0x14);
 		  write(LcdStruct, 0x14);
 		  write(LcdStruct, 0x2C);
 		  write(LcdStruct, 0x32);

 		  wr_cmd(LcdStruct, 0xe1);
 		  write(LcdStruct, 0xD0);
 		  write(LcdStruct, 0x05);
 		  write(LcdStruct, 0x0A);
 		  write(LcdStruct, 0x0A);
 		  write(LcdStruct, 0x07);
 		  write(LcdStruct, 0x28);
 		  write(LcdStruct, 0x32);
 		  write(LcdStruct, 0x2C);
 		  write(LcdStruct, 0x49);
 		  write(LcdStruct, 0x18);
 		  write(LcdStruct, 0x13);
 		  write(LcdStruct, 0x13);
 		  write(LcdStruct, 0x2C);
 		  write(LcdStruct, 0x33);

 		wr_cmd(LcdStruct, 0x21); // Display inversion ON
 		gpio.out(Struct->LCD_DISPLAY_CS, 1);
 		mi0283_screen_set_area(LcdStruct, 0, 0, _LcdStruct->LcdTimings->X, _LcdStruct->LcdTimings->Y);

 		wr_cmd(LcdStruct, 0x29); //display on
 		gpio.out(Struct->LCD_DISPLAY_CS, 1);
 		wr_cmd(LcdStruct, 0x2c);
 		gpio.out(Struct->LCD_DISPLAY_CS, 1);
 	}

 	cnt = FCPU / 5000;
 	while(cnt--);
 	//wr_cmd(LcdStruct, 0xB8);                     // COLMOD_PIXEL_FORMAT_SET
    //write(LcdStruct, 0x07);                 // 16 bit pixel
 	//gpio.out(Struct->LCD_DISPLAY_CS, 1);

 }


//#######################################################################################
//#######################################################################################
#define ScreenArea_Integrated	1
void mi0283_screen_set_area(tDisplay* LcdStruct, signed int x0, signed int y0, signed int x1, signed int y1)
{
	MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	signed int x0_int = x0;
	signed int y0_int = y0;
	signed int x1_int = x1;
	signed int y1_int = y1;
    wr_cmd(LcdStruct, 0x2A);                       // entry mode
    write(LcdStruct, x0_int >> 8);
    write(LcdStruct, x0_int);
    write(LcdStruct, x1_int >> 8);
    write(LcdStruct, x1_int);
 	gpio.out(Struct->LCD_DISPLAY_CS, 1);
    wr_cmd(LcdStruct, 0x2B);                       // entry mode
    write(LcdStruct, y0_int >> 8);
    write(LcdStruct, y0_int);
    write(LcdStruct, y1_int >> 8);
    write(LcdStruct, y1_int);
 	gpio.out(Struct->LCD_DISPLAY_CS, 1);
}
//#######################################################################################
void mi0283_lcd_cursor(tDisplay* LcdStruct, signed int x, signed int y)
{
	mi0283_screen_set_area(LcdStruct, x, y, x, y);
}
//#######################################################################################
void mi0283_driver_register(void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.init = mi0283_open;
	LcdStruct->lcd_func.reset = mi0283_reset;
	//LcdStruct->lcd_func.enable = lcd_enable;
	//LcdStruct->lcd_func.disable = lcd_disable;
	//LcdStruct->lcd_func.backlight_on = backlight_on;
	//LcdStruct->lcd_func.backlight_off = backlight_off;
	//LcdStruct->lcd_func.copy = screen_copy;
	//LcdStruct->lcd_func.put_rgb_array_16 = put_rgb_array_16;
	//LcdStruct->lcd_func.put_rgb_array_24 = put_rgb_array_24;
	//LcdStruct->lcd_func.put_rgb_array_32 = put_rgb_array_32;
	LcdStruct->lcd_func.put_rectangle = mi0283_draw_rectangle;
	LcdStruct->lcd_func.put_pixel = mi0283_put_pixel16;
	LcdStruct->lcd_func.put_horizontal_line = mi0283_put_horizontal_line;
	LcdStruct->lcd_func.put_vertical_line = mi0283_put_vertical_line;
	LcdStruct->lcd_func.clear = mi0283_clear;
	//LcdStruct->lcd_func.box_cache_clean = box_cache_clean;
}
//#######################################################################################
bool mi0283_open(void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	gpio.out(Struct->LCD_DISPLAY_RST, 0);
	gpio.out(Struct->LCD_DISPLAY_RS, 1);
	gpio.out(Struct->LCD_BACKLIGHT, 1);
	gpio.out(Struct->LCD_DISPLAY_CS, 1);
	gpio.out(Struct->LCD_DISPLAY_PMRD, 1);
	gpio.out(Struct->LCD_DISPLAY_PMWR, 1);
	gpio.out(Struct->LCD_TOUCH_DRIVE_A, 0);
	gpio.out(Struct->LCD_TOUCH_DRIVE_B, 0);
	mi0283_reset(LcdStruct);
	mi0283_set_orientation(LcdStruct);
	LcdStruct->sClipRegion.sXMax = LcdStruct->LcdTimings->X;
	LcdStruct->sClipRegion.sYMax = LcdStruct->LcdTimings->Y;
	return true;
}
//#######################################################################################
void mi0283_close(void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	free(LcdStruct);
}
//#######################################################################################
void mi0283_cursor_reset(tDisplay* LcdStruct)
{

}
//#######################################################################################
bool mi0283_set_orientation(tDisplay* LcdStruct)
{
	MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	bool Tmp = false;
	wr_cmd(LcdStruct, 0x36);                     // MEMORY_ACCESS_CONTROL
 	if(mi0283_ctl_type == MI0283_ILI9341_CTRL)
 	{
		switch(LcdStruct->LcdTimings->orientation)
		{
			case LCD_ORIENTATION_PORTRAIT:
				write(LcdStruct, 0x48);
				//lcd_rw_cmd(LcdStruct, 0x36, 0x48); //MY=1 MX=0 MV=1 ML=0 BGR=1
				Tmp = true;
				break;

			case LCD_ORIENTATION_LANDSCAPE:
				write(LcdStruct, 0x28);
				//lcd_rw_cmd(LcdStruct, 0x36, 0x28); //MY=0 MX=0 MV=0 ML=0 BGR=1
				Tmp = true;
				break;

			case LCD_ORIENTATION_PORTRAIT_FLIP:
				write(LcdStruct, 0x88);
				//lcd_rw_cmd(LcdStruct, 0x36, 0x88); //MY=0 MX=1 MV=1 ML=0 BGR=1
				Tmp = true;
				break;

			case LCD_ORIENTATION_LANDSCAPE_FLIP:
				write(LcdStruct, 0xE8);
				//lcd_rw_cmd(LcdStruct, 0x36, 0xE8); //MY=1 MX=0 MV=1 ML=0 BGR=1
				Tmp = true;
				break;
		}
 	}
 	else if(mi0283_ctl_type == MI0283_ST7789V_CTRL)
 	{
		switch(LcdStruct->LcdTimings->orientation)
		{
			case LCD_ORIENTATION_PORTRAIT:
	 		    write(LcdStruct, 0x18);
				break;

			case LCD_ORIENTATION_LANDSCAPE:
	 		      write(LcdStruct, 0xB8);
				break;

			case LCD_ORIENTATION_PORTRAIT_FLIP:
	 		      write(LcdStruct, 0xC8);
				break;

			case LCD_ORIENTATION_LANDSCAPE_FLIP:
	 		      write(LcdStruct, 0x78);
				break;
		}
 	}

	gpio.out(Struct->LCD_DISPLAY_CS, 1);
	LcdStruct->sClipRegion.sXMax = LcdStruct->LcdTimings->X;
	LcdStruct->sClipRegion.sYMax = LcdStruct->LcdTimings->Y;
	mi0283_screen_set_area(LcdStruct, 0, 0, LcdStruct->LcdTimings->X,LcdStruct->LcdTimings->Y);
	return Tmp;
}
//#######################################################################################
void mi0283_send_pixels(tDisplay* LcdStruct, unsigned long PixelsNumber, unsigned int Color)
{
	MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	//if(PixelsNumber < 0) return;
	if(PixelsNumber > (unsigned long)(((unsigned long)LcdStruct->LcdTimings->Y * (unsigned long)LcdStruct->LcdTimings->X) << 1)) return;
	//unsigned long cnt = 0;
	unsigned int GPIORW = Struct->LCD_DISPLAY_PMWR->BaseAddr;
	unsigned int GPIODAT = Struct->LCD_DATA->BaseAddr;
	unsigned char PINRW = Struct->LCD_DISPLAY_PMWR->Pin;
	unsigned char PINDAT = Struct->LCD_DATA->Pin;
	while(PixelsNumber--)
	{
		GPIO_FAST_WRITE_MULTI_PIN(GPIODAT, PINDAT, Color >> 8);
		GPIO_FAST_CLEAR_PIN(GPIORW, PINRW);
		GPIO_FAST_SET_PIN(GPIORW, PINRW);
		GPIO_FAST_WRITE_MULTI_PIN(GPIODAT, PINDAT, Color);
		GPIO_FAST_CLEAR_PIN(GPIORW, PINRW);
		GPIO_FAST_SET_PIN(GPIORW, PINRW);
	}
 	gpio.out(Struct->LCD_DISPLAY_CS, 1);
}
//#######################################################################################
void mi0283_put_pixel16(void *pDisplay, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	//MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	//lcd_cursor(X_Coordonate,Y_Coordonate);
	if(X_Coordonate < LcdStruct->sClipRegion.sXMin || X_Coordonate >= LcdStruct->sClipRegion.sXMax || Y_Coordonate < LcdStruct->sClipRegion.sYMin || Y_Coordonate >= LcdStruct->sClipRegion.sYMax) return;
	if(X_Coordonate >= LcdStruct->LcdTimings->X || X_Coordonate < 0 || Y_Coordonate >= LcdStruct->LcdTimings->Y || Y_Coordonate < 0) return;
	
	mi0283_screen_set_area(LcdStruct, X_Coordonate, Y_Coordonate, X_Coordonate, Y_Coordonate);
	wr_cmd(LcdStruct, 0x2C);
	mi0283_send_pixels(LcdStruct, 1, Color16);
}
//#######################################################################################
unsigned short mi0283_get_pixel16(void *pDisplay, signed int X_Coordonate, signed int Y_Coordonate)
{
	//tDisplay* LcdStruct = (tDisplay *) pDisplay;
	//screen_set_area(LcdStruct, X_Coordonate, Y_Coordonate, X_Coordonate, Y_Coordonate);
	return 0;//lcd_rd_data(LcdStruct);
}
//#######################################################################################
void mi0283_put_pixel24(void *pDisplay, signed int X_Coordonate, signed int Y_Coordonate, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	//MI0283_t *Struct = (MI0283_t *)LcdStruct->UserData;
	if(X_Coordonate < LcdStruct->sClipRegion.sXMin || X_Coordonate >= LcdStruct->sClipRegion.sXMax || Y_Coordonate < LcdStruct->sClipRegion.sYMin || Y_Coordonate >= LcdStruct->sClipRegion.sYMax) return;
	if(X_Coordonate >= LcdStruct->LcdTimings->X || X_Coordonate < 0 || Y_Coordonate >= LcdStruct->LcdTimings->Y || Y_Coordonate < 0) return;
	mi0283_lcd_cursor(LcdStruct, X_Coordonate,Y_Coordonate);
	unsigned short TempColor = ((Red>>3)&0b11111) | ((Green<<3)&0b11111100000) | ((Blue<<8)&0b1111100000000000);
	wr_cmd(LcdStruct, 0x2C);
	mi0283_send_pixels(LcdStruct, 1, TempColor);
}
//#######################################################################################
void mi0283_draw_rectangle(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	signed int x_end = x_start + x_len ,y_end = y_start + y_len;
	if(x_start >= LcdStruct->sClipRegion.sXMax || y_start >= LcdStruct->sClipRegion.sYMax || x_end < LcdStruct->sClipRegion.sXMin || y_end < LcdStruct->sClipRegion.sYMin) return;
	register signed int LineCnt = y_start;
	if(fill)
	{
		if(LineCnt < LcdStruct->sClipRegion.sYMin)
			LineCnt = LcdStruct->sClipRegion.sYMin;
		signed int _x_start = x_start;
		if(_x_start < LcdStruct->sClipRegion.sXMin)
			_x_start = LcdStruct->sClipRegion.sXMin;
		signed int _x_end = x_end;
		if(_x_end > LcdStruct->sClipRegion.sXMax)
			_x_end = LcdStruct->sClipRegion.sXMax;
		if(y_end > LcdStruct->sClipRegion.sYMax)
			y_end = LcdStruct->sClipRegion.sYMax;
		mi0283_screen_set_area(LcdStruct, _x_start, LineCnt, _x_end - 1, y_end - 1);
		wr_cmd(LcdStruct, 0x2C);
		mi0283_send_pixels(LcdStruct, ((_x_end - 1 - _x_start) * (y_end - LineCnt)) + (y_end - LineCnt), color);
	}
	else
	{	
		register int _x_end = x_end;
		int _x_start = x_start;
		if(_x_end >= LcdStruct->sClipRegion.sXMax) _x_end = LcdStruct->sClipRegion.sXMax;
		if(_x_start < LcdStruct->sClipRegion.sXMin) _x_start = LcdStruct->sClipRegion.sXMin;
		if(y_start >= LcdStruct->sClipRegion.sYMin)
		{
			mi0283_screen_set_area(LcdStruct, _x_start, y_start, _x_end, y_start);
			wr_cmd(LcdStruct, 0x2C);
			mi0283_send_pixels(LcdStruct, (_x_end - _x_start) + 1, color);
		}

		if(y_end <= LcdStruct->sClipRegion.sYMax)
		{
			mi0283_screen_set_area(LcdStruct, _x_start, y_end - 1, _x_end, y_end - 1);
			wr_cmd(LcdStruct, 0x2C);
			mi0283_send_pixels(LcdStruct, (_x_end - _x_start) + 1, color);
		}

		int _y_end = y_end;
		if(_y_end >= LcdStruct->sClipRegion.sYMax) _y_end = LcdStruct->sClipRegion.sYMax;
		int _y_start = y_start;
		if(_y_start < LcdStruct->sClipRegion.sYMin) _y_start = LcdStruct->sClipRegion.sYMin;
		if(x_start >= LcdStruct->sClipRegion.sXMin)
		{
			mi0283_screen_set_area(LcdStruct, x_start,_y_start, x_start, y_end);
			wr_cmd(LcdStruct, 0x2C);
			mi0283_send_pixels(LcdStruct, (y_end - _y_start) + 1, color);
		}

		if(x_end <= LcdStruct->sClipRegion.sXMax)
		{
			mi0283_screen_set_area(LcdStruct, x_end - 1,_y_start, x_end - 1, y_end);
			wr_cmd(LcdStruct, 0x2C);
			mi0283_send_pixels(LcdStruct, (y_end - _y_start) + 1, color);
		}
	}
}
//#######################################################################################
void mi0283_put_horizontal_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	if(width == 1 && (Y < LcdStruct->sClipRegion.sYMin || Y >= LcdStruct->sClipRegion.sYMax))
		return;
	register int X1_Tmp = X1, X2_Tmp = X1 + X2;
	if(X1_Tmp <= (int)LcdStruct->sClipRegion.sXMin) X1_Tmp = (int)LcdStruct->sClipRegion.sXMin;
	if(X1_Tmp >= (int)LcdStruct->sClipRegion.sXMax) X1_Tmp = (int)LcdStruct->sClipRegion.sXMax;
	if(X2_Tmp <= (int)LcdStruct->sClipRegion.sXMin) X2_Tmp = (int)LcdStruct->sClipRegion.sXMin;
	if(X2_Tmp >= (int)LcdStruct->sClipRegion.sXMax) X2_Tmp = (int)LcdStruct->sClipRegion.sXMax;
	if(Y < (int)LcdStruct->sClipRegion.sYMin) Y = (int)LcdStruct->sClipRegion.sYMin;
	if(Y >= (int)LcdStruct->sClipRegion.sYMax) Y = (int)LcdStruct->sClipRegion.sYMax;
	int Half_width1 = (width>>1);
	int Half_width2 = width-Half_width1;

	if(X2_Tmp - X1_Tmp > 0)
	{
		//register int _Y_ = Y - Half_width1;
		mi0283_screen_set_area(LcdStruct, X1_Tmp, Y - Half_width1, X2_Tmp, (Y + Half_width2) - 1);
		wr_cmd(LcdStruct, 0x2C);
		mi0283_send_pixels(LcdStruct, (X2_Tmp - X1_Tmp) * width , color);
	}	
}
//#######################################################################################
void mi0283_put_vertical_line(void *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	if(width == 1 && (X < LcdStruct->sClipRegion.sXMin || X >= LcdStruct->sClipRegion.sXMax))
		return;
	register int Y1_Tmp = Y1, Y2_Tmp = Y1 + Y2;
	if(X <= (int)LcdStruct->sClipRegion.sXMin) X = (int)LcdStruct->sClipRegion.sXMin;
	if(X >= (int)LcdStruct->sClipRegion.sXMax) X = (int)LcdStruct->sClipRegion.sXMax;
	if(Y1_Tmp <= (int)LcdStruct->sClipRegion.sYMin) Y1_Tmp = (int)LcdStruct->sClipRegion.sYMin;
	if(Y1_Tmp >= (int)LcdStruct->sClipRegion.sYMax) Y1_Tmp = (int)LcdStruct->sClipRegion.sYMax;
	if(Y2_Tmp <= (int)LcdStruct->sClipRegion.sYMin) Y2_Tmp = (int)LcdStruct->sClipRegion.sYMin;
	if(Y2_Tmp >= (int)LcdStruct->sClipRegion.sYMax) Y2_Tmp = (int)LcdStruct->sClipRegion.sYMax;
	int Half_width1 = (width>>1);
	int Half_width2 = width - Half_width1;
	if(Y2_Tmp - Y1_Tmp > 0)
	{
		//register int _Y_ = Y - Half_width1;
		mi0283_screen_set_area(LcdStruct, X - Half_width1, Y1_Tmp, (X + Half_width2) - 1, Y2_Tmp);
		wr_cmd(LcdStruct, 0x2C);
		mi0283_send_pixels(LcdStruct, (Y2_Tmp - Y1_Tmp) * width , color);
	}	
}
//#######################################################################################
void  mi0283_clear(void *pDisplay, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	LcdStruct->lcd_func.put_rectangle(pDisplay, 0, 0, LcdStruct->LcdTimings->X, LcdStruct->LcdTimings->Y, true, color);
}
