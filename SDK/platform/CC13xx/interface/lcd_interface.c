/*
 * lcd_interface.c
 *
 * Created: 2/8/2013 10:26:43 PM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "main.h"
//#include "board_properties.h"
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

bool SetUpLCD(void* LcdStruct)
{
	//screen_open(LcdStruct);
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
void _screen_backlight_on(void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	if(LcdStruct->pmic_back_light)
	{
#ifdef pmic_backlight_enable
		pmic_backlight_enable(pDisplay->PmicTwiModuleStruct);
#endif
#ifdef pmic_backlight_level
		pmic_backlight_level(pDisplay->PmicTwiModuleStruct, pDisplay->BackLightLevel);
#endif
	}
	else
	{
		if(LcdStruct->invert_backlight) gpio_out(LcdStruct->BackLight, 0);
		else gpio_out(LcdStruct->BackLight, 1);
	}
}
//#######################################################################################
void _screen_backlight_off(void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	if(LcdStruct->pmic_back_light)
	{
#ifdef pmic_backlight_enable
		pmic_backlight_enable(pDisplay->PmicTwiModuleStruct);
#endif
#ifdef pmic_backlight_level
		pmic_backlight_level(pDisplay->PmicTwiModuleStruct, 0);
#endif
	}
	else
	{
		if(LcdStruct->invert_backlight) gpio_out(LcdStruct->BackLight, 1);
		else gpio_out(LcdStruct->BackLight, 0);
	}
}
//#######################################################################################
void _box_cache_clean(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{
	
}
//#######################################################################################
bool _screen_copy(void *pDisplayTo, void *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{
	tDisplay* LcdStructTo = (tDisplay *) pDisplayTo;
	tDisplay* LcdStructFrom = (tDisplay *) pDisplayFrom;
	if(LcdStructTo->raster_timings->X != LcdStructFrom->raster_timings->X || LcdStructTo->raster_timings->Y != LcdStructFrom->raster_timings->Y)
	return false;
	CacheDataCleanBuff((unsigned int)LcdStructFrom->DisplayData, (LcdStructFrom->raster_timings->X * LcdStructFrom->raster_timings->Y * sizeof(LcdStructFrom->DisplayData[0])) + (LcdStructFrom->raster_timings->palete_len * sizeof(LcdStructFrom->DisplayData[0])));
	signed int LineCnt = 0;
	volatile unsigned int* ScreenBuff = LcdStructTo->DisplayData + LcdStructTo->raster_timings->palete_len;
	volatile unsigned int* _ScreenBuff = LcdStructFrom->DisplayData + LcdStructTo->raster_timings->palete_len;

	for(; LineCnt < LcdStructTo->raster_timings->Y; LineCnt ++)
	{
		memcpy((void *)(ScreenBuff + (LcdStructFrom->raster_timings->X * LineCnt)), (void *)(_ScreenBuff + (LcdStructFrom->raster_timings->X * LineCnt)), (sizeof(ScreenBuff[0]) * LcdStructTo->raster_timings->X));
		if(put_cursor == true && LineCnt >= Y && LineCnt <= Y + 2)
		{
			unsigned int cnt_x = X;
			for(;cnt_x < X + 2; cnt_x++)
			_put_pixel(pDisplayTo, cnt_x, LineCnt, color);
		}
	}
	return true;
}
//#######################################################################################
void _put_rectangle(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	//screen_draw_rectangle(pDisplay, x_start, y_start, x_len, y_len, fill, color);
	return;
}
//#######################################################################################
void _put_pixel(void *pDisplay, signed int X, signed int Y, unsigned int color)
{
	//screen_put_pixel16(pDisplay, X, Y, color);
}
//#######################################################################################
void _screen_put_horizontal_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	//screen_put_horizontal_line(pDisplay, X1, X2, Y, width, color);
}
//#######################################################################################
void _screen_put_vertical_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	//screen_put_vertical_line(pDisplay, X1, X2, Y, width, color);
}
//#######################################################################################
void _screen_put_rgb_array_16(void *pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	//tDisplay* LcdStruct = (tDisplay *) pDisplay;
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
void _screen_put_rgb_array_24(void *pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{
	//tDisplay* LcdStruct = (tDisplay *) pDisplay;
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
void _screen_put_rgb_array_32(void *pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	//tDisplay* LcdStruct = (tDisplay *) pDisplay;
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
void _screen_clear(void *pDisplay, unsigned int color)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
	_put_rectangle(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, true, color);
}
