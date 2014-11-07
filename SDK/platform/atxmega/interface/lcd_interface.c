/*
 * lcd_interface.c
 *
 * Created: 2/8/2013 10:26:43 PM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "board_properties.h"
#include "lcd_interface.h"
#include "sys/cache.h"
#include "lib/gfx/controls_definition.h"
#include "api/gpio_api.h"

#ifdef use_mi0283
#include "device/mi0283.h"
#endif
#ifdef use_20x20MatrixLedUsartSpiDma
#include "device/20x20MatrixLedUartSpiDma.h"
#endif

bool SetUpLCD(tDisplay* LcdStruct)
{
	screen_open(LcdStruct);
	return true;
}

/**********************************************/
void _lcd_enable()
{
	
}
/**********************************************/
void _lcd_disable()
{
	
}
//#######################################################################################
void _screen_backlight_on(tDisplay *pDisplay)
{
	switch(pDisplay->LcdType)
	{
	case MI0283:
			gpio_out(pDisplay->BackLight, 1);
			return;
	}
}
//#######################################################################################
void _screen_backlight_off(tDisplay *pDisplay)
{
	switch(pDisplay->LcdType)
	{
	case MI0283:
		gpio_out(pDisplay->BackLight, 0);
		return;
	}
}
//#######################################################################################
void _box_cache_clean(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{
	
}
//#######################################################################################
void _put_rectangle(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	screen_draw_rectangle(pDisplay, x_start, y_start, x_len, y_len, fill, color);
	return;
}
//#######################################################################################
void _put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color)
{
	screen_put_pixel16(pDisplay, X, Y, color);
}
//#######################################################################################
void _screen_put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	screen_put_horizontal_line(pDisplay, X1, X2, Y, width, color);
}
//#######################################################################################
void _screen_put_vertical_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	screen_put_vertical_line(pDisplay, X1, X2, Y, width, color);
}
//#######################################################################################
void _screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	/*tDisplay *pDisplay = (tDisplay *)_pDisplay;
	///Write rgb array to video memory
	unsigned char *Buff = (unsigned char *)rgb_buffer;
	//unsigned long Length = (width*height)<<1;

	int   x, y;
	volatile unsigned char Tmp1;
	volatile unsigned char Tmp2;
	unsigned int color;
	for(y =y1; y<(height+y1);y++)
	{
		for(x = x1; x < width+x1; x++)
		{
			Tmp2 = *Buff++;
			Tmp1 = *Buff++;
			//color.blue = Tmp1 & 0b11111000;
			//color.green = ((Tmp1<<5) | (Tmp2>>3)) & 0b11111100;
			//color.red = Tmp2<<3;
			color = RGB_TO_UINT((Tmp2<<3) & 0xF8, ((Tmp1<<5) | (Tmp2>>3)) & 0xFC, Tmp1 & 0xF8);
			put_pixel(pDisplay, x, y, color);
		}
		CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y), width * 4);
	}
	//lcd.dblbuf = dblbuf;*/
}
//#######################################################################################
void _screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{
	/*tDisplay *pDisplay = (tDisplay *)_pDisplay;
	unsigned char *Buff = rgb_buffer;
	int   y;
	for(y =y1; y<(height+y1);y++)
	{
		if(y < pDisplay->sClipRegion.sYMin || y > pDisplay->sClipRegion.sYMax);
		else
		{
			unsigned int *DisplayStartLine = (unsigned int *)pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y);
			unsigned int *DisplayEndLine = DisplayStartLine + width;
			while(DisplayStartLine < DisplayEndLine)
			{
				*DisplayStartLine++= ((*Buff)<<24) | ((*(Buff+1))<<16) | ((*(Buff+2))<<8);
				Buff+=3;
			}
			CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y * 4), width * 4);
		}
	}*/
}
//#######################################################################################
void _screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	/*tDisplay *pDisplay = (tDisplay *)_pDisplay;
	int   y;
	int _y = 0;
	signed int _width = width;
	if(x1 + _width > pDisplay->sClipRegion.sXMax) _width = pDisplay->sClipRegion.sXMax - x1;
	_width *= 4;
	for(y =y1; y<(height+y1);y++)
	{
		if(y < pDisplay->sClipRegion.sYMin || y > pDisplay->sClipRegion.sYMax);
		else
		{
			unsigned int *DisplayStartLine = (unsigned int *)pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y);
			unsigned char *Buff = rgb_buffer + (width * _y * 4) - 1;
			_y++;
			memcpy((void*)(DisplayStartLine), (void*)(Buff), width * 4);
			CacheDataCleanBuff((unsigned int)((unsigned int*)(pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y))), _width);
		}
	}*/
}
//#######################################################################################
