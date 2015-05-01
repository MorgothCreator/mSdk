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
#include <util/delay.h>
#include <stdbool.h>
#include <util/atomic.h>
#include "general/unions.h"
#include "mi0283.h"
#include "board_properties.h"
#include "driver/ebi.h"
#include "driver/dma.h"
#include "api/lcd_def.h"
//#######################################################################################
new_dma_ch* DMA_LCD = NULL;
//#######################################################################################
void lcd_rw_cmd(unsigned char Cmd, unsigned char Data)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
		__far_mem_write(MI0283QT2_Addr, Cmd);
		MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
		__far_mem_write(MI0283QT2_Addr, Data);
	}	
}
//#######################################################################################
void lcd_rw_data(unsigned short Data)
{
	convert16to8 ByteToWrite_;
	ByteToWrite_.i16 = Data;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		__far_mem_write(MI0283QT2_Addr, ByteToWrite_.Byte1);
		__far_mem_write(MI0283QT2_Addr, ByteToWrite_.Byte0);
	}	
}
//#######################################################################################
unsigned short lcd_rd_data(void)
{
	convert8to16 ByteToWrite_;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
		__far_mem_write(MI0283QT2_Addr, 34);
		MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
		ByteToWrite_.Byte0 = __far_mem_read(MI0283QT2_Addr);
		ByteToWrite_.Byte1 = __far_mem_read(MI0283QT2_Addr);
	}	
	return ByteToWrite_.ShortReturn;
}
//#######################################################################################
#ifndef LcdCmdSend
#define LcdCmdSend
void lcd_cmd_send(void)
{
	const uint8_t* screen_setup_data = MI0283_screen_setup_data;
	uint8_t DataCount = 0;
	do
	{
		if(pgm_read_byte(&screen_setup_data[DataCount]) == 254)
		{
			_delay_ms(10);
			DataCount = DataCount +1;
		}
		else
		{
			lcd_rw_cmd(pgm_read_byte(&screen_setup_data[DataCount]),pgm_read_byte(&screen_setup_data[DataCount+1]));
			DataCount = DataCount +2;
		}
	} while (pgm_read_byte(&screen_setup_data[DataCount]) != 255);
}
#endif
//#######################################################################################
#define ScreenArea_Integrated	1
void screen_set_area(signed int x0, signed int y0, signed int x1, signed int y1)
{
	while(dma_ch_busy(DMA_LCD));
	convert16to8 SplitData;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		SplitData.i16 = x0;
		lcd_rw_cmd(0x03, SplitData.Byte0); //set x0
		lcd_rw_cmd(0x02, SplitData.Byte1); //set x0
		SplitData.i16 = x1;
		lcd_rw_cmd(0x05, SplitData.Byte0); //set x1
		lcd_rw_cmd(0x04, SplitData.Byte1); //set x1
		SplitData.i16 = y0;
		lcd_rw_cmd(0x07, SplitData.Byte0); //set y0
		lcd_rw_cmd(0x06, SplitData.Byte1); //set y0
		SplitData.i16 = y1;
		lcd_rw_cmd(0x09, SplitData.Byte0); //set y1
		lcd_rw_cmd(0x08, SplitData.Byte1); //set y1
	}	
}
//#######################################################################################
void lcd_cursor(signed int x, signed int y)
{
  screen_set_area(x, y, x, y);
}
//#######################################################################################
void lcd_drawstop(void)
{
}
//#######################################################################################
void screen_dma_sinalize_end_of_transfer(void* Struct)
{
	
}
//#######################################################################################
void screen_open(tDisplay* LcdStruct)
{
	MI0283QT2_RS_Port.DIRSET = MI0283QT2_RS_Pin_bm;
	MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
	_delay_ms(50);
	lcd_cmd_send();
	screen_set_orientation(LcdStruct);
	DMA_LCD = new_(new_dma_ch);
	DMA_LCD->Dma_BlockLength = 2;//DisplayStruct->NrOfLaths;
	DMA_LCD->Dma_BurstMode = _Dma_BurstMode_8;
	DMA_LCD->Dma_CompleteIntLevel = _Dma_Int_Disabled;
	DMA_LCD->Dma_DestAddr = MI0283QT2_Addr;
	DMA_LCD->Dma_DestAddrMode = _Dma_DestAddressMode_Fixed;
	DMA_LCD->Dma_DestAddrReload = _Dma_DestAddressReload_None;
	DMA_LCD->Dma_ErrIntLevel = _Dma_Int_Disabled;
	DMA_LCD->Dma_RepeatMode = true;
	DMA_LCD->Dma_RepeatNr = 1;
	DMA_LCD->Dma_SingleShot = false;
	DMA_LCD->Dma_SrcAddr = (unsigned int)((unsigned char*)&LcdStruct->InkColor) + 1;//(unsigned long)DisplayStruct->LathBuffer;
	DMA_LCD->Dma_SrcAddrMode = _Dma_SourceAddressMode_Dec;
	DMA_LCD->Dma_SrcAddrReload = _Dma_SourceAddressReload_Block;
	DMA_LCD->Dma_Trigers = _Dma_Triger_Off;
	dma_search_and_use(DMA_LCD, USED_LCD_DMA + 4);
}
//#######################################################################################
void screen_close(tDisplay* LcdStruct)
{
	free(LcdStruct);
}
//#######################################################################################
void screen_cursor_reset(tDisplay* LcdStruct)
{

}
//#######################################################################################
bool screen_set_orientation(tDisplay* LcdStruct)
{
	bool Tmp = false;
	switch(LcdStruct->Orientation)
	{
		case 0:
			LcdStruct->raster_timings->X  = 320;
			LcdStruct->raster_timings->Y = 240;
			lcd_rw_cmd(0x16, 0xA0); //MY=1 MX=0 MV=1 ML=0 BGR=1
			Tmp = true;
			break;

		case 90:
			LcdStruct->raster_timings->X  = 240;
			LcdStruct->raster_timings->Y = 320;
			lcd_rw_cmd(0x16, 0x00); //MY=0 MX=0 MV=0 ML=0 BGR=1
			Tmp = true;
			break;

		case 180:
			LcdStruct->raster_timings->X  = 320;
			LcdStruct->raster_timings->Y = 240;
			lcd_rw_cmd(0x16, 0x60); //MY=0 MX=1 MV=1 ML=0 BGR=1
			Tmp = true;
			break;

		case 270:
			LcdStruct->raster_timings->X  = 240;
			LcdStruct->raster_timings->Y = 320;
			lcd_rw_cmd(0x16, 0xC0); //MY=1 MX=0 MV=1 ML=0 BGR=1
			Tmp = true;
			break;
		default:
			LcdStruct->Orientation = 90;
			LcdStruct->raster_timings->X  = 240;
			LcdStruct->raster_timings->Y = 320;
			lcd_rw_cmd(0x16, 0x00); //MY=0 MX=0 MV=0 ML=0 BGR=1
			Tmp = true;
			break;
	}
	LcdStruct->sClipRegion.sXMax = LcdStruct->raster_timings->X;
	LcdStruct->sClipRegion.sYMax = LcdStruct->raster_timings->Y;
	screen_set_area(0, 0, LcdStruct->raster_timings->X,LcdStruct->raster_timings->Y);
	return Tmp;
}
//#######################################################################################
void screen_send_pixels_via_dma(tDisplay* LcdStruct, unsigned long PixelsNumber, unsigned int Color)
{
	//if(PixelsNumber < 0) return;
	if(PixelsNumber > (unsigned long)(((unsigned long)LcdStruct->raster_timings->Y * (unsigned long)LcdStruct->raster_timings->X) << 1)) return;
	LcdStruct->InkColor = Color;
	volatile unsigned long _PixelsNumber = PixelsNumber;
	while (_PixelsNumber & 0xFFFFFF00)
	{
		while(dma_ch_busy(DMA_LCD));
		dma_ch_repeat_enable(DMA_LCD);
		dma_repeat_nr_set(DMA_LCD, 0xFF);
		dma_transfer_request(DMA_LCD);
		_PixelsNumber -= 255;
	}		
	if(_PixelsNumber)
	{
		while(dma_ch_busy(DMA_LCD));
		dma_ch_repeat_enable(DMA_LCD);
		dma_repeat_nr_set(DMA_LCD, _PixelsNumber);
		dma_transfer_request(DMA_LCD);
	}	
}
//#######################################################################################
void screen_put_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16)
{
	//lcd_cursor(X_Coordonate,Y_Coordonate);
	if(X_Coordonate < LcdStruct->sClipRegion.sXMin || X_Coordonate >= LcdStruct->sClipRegion.sXMax || Y_Coordonate < LcdStruct->sClipRegion.sYMin || Y_Coordonate >= LcdStruct->sClipRegion.sYMax) return;
	if(X_Coordonate >= LcdStruct->raster_timings->X || X_Coordonate < 0 || Y_Coordonate >= LcdStruct->raster_timings->Y || Y_Coordonate < 0) return;
	
	screen_set_area(X_Coordonate, Y_Coordonate, X_Coordonate, Y_Coordonate);
	//lcd_drawstart();
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
		__far_mem_write(MI0283QT2_Addr, 0x22);
		MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
		convert16to8 ByteToWrite_;
		ByteToWrite_.i16 = Color16;
		__far_mem_write(MI0283QT2_Addr, ByteToWrite_.Byte1);
		__far_mem_write(MI0283QT2_Addr, ByteToWrite_.Byte0);
	}	
}
//#######################################################################################
uint16_t screen_get_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16)
{
	screen_set_area(X_Coordonate, Y_Coordonate, X_Coordonate, Y_Coordonate);
	return lcd_rd_data();
}
//#######################################################################################
void screen_put_pixel24(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned char Red, unsigned char Green, unsigned char Blue)
{
	if(X_Coordonate < LcdStruct->sClipRegion.sXMin || X_Coordonate >= LcdStruct->sClipRegion.sXMax || Y_Coordonate < LcdStruct->sClipRegion.sYMin || Y_Coordonate >= LcdStruct->sClipRegion.sYMax) return;
	if(X_Coordonate >= LcdStruct->raster_timings->X || X_Coordonate < 0 || Y_Coordonate >= LcdStruct->raster_timings->Y || Y_Coordonate < 0) return;
	lcd_cursor(X_Coordonate,Y_Coordonate);
	uint16_t TempColor = ((Red>>3)&0b11111) | ((Green<<3)&0b11111100000) | ((Blue<<8)&0b1111100000000000);
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
		__far_mem_write(MI0283QT2_Addr, 0x22);
		MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
		convert16to8 ByteToWrite_;
		ByteToWrite_.i16 = TempColor;
		__far_mem_write(MI0283QT2_Addr, ByteToWrite_.Byte1);
		__far_mem_write(MI0283QT2_Addr, ByteToWrite_.Byte0);
	}	
}
//#######################################################################################
void screen_clear(tDisplay* LcdStruct, unsigned int color)
{
	screen_draw_rectangle(LcdStruct, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, true, color);
}
//#######################################################################################
void screen_draw_rectangle(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, bool fill, unsigned int color)
{
	int16_t _x1 = x1, _x2 = (x1 + x2)-1, _y1 = y1, _y2 = (y1 + y2)-1;
	
	if(_x1 >= LcdStruct->sClipRegion.sXMax - 1 || _x2 < LcdStruct->sClipRegion.sXMin || _y1 >= LcdStruct->sClipRegion.sYMax - 1 || _y2 < LcdStruct->sClipRegion.sYMin) return;
	if(_x1 >= LcdStruct->raster_timings->X - 1 || _x2 < 0 || _y1 >= LcdStruct->raster_timings->Y - 1 || _y2 < 0) return;
	
	if(_x1 < LcdStruct->sClipRegion.sXMin) _x1 = LcdStruct->sClipRegion.sXMin;
	if(_x1 >= LcdStruct->sClipRegion.sXMax) _x1 = LcdStruct->sClipRegion.sXMax;
	if(_x2 < LcdStruct->sClipRegion.sXMin) _x2 = LcdStruct->sClipRegion.sXMin;
	if(_x2 >= LcdStruct->sClipRegion.sXMax) _x2 = LcdStruct->sClipRegion.sXMax;
	if(_y1 < LcdStruct->sClipRegion.sYMin) _y1 = LcdStruct->sClipRegion.sYMin;
	if(_y1 >= LcdStruct->sClipRegion.sYMax) _y1 = LcdStruct->sClipRegion.sYMax;
	if(_y2 < LcdStruct->sClipRegion.sYMin) _y2 = LcdStruct->sClipRegion.sYMin;
	if(_y2 >= LcdStruct->sClipRegion.sYMax) _y2 = LcdStruct->sClipRegion.sYMax;
	
	if(_x1 < 0) _x1 = 0;
	if(_x1 >= LcdStruct->raster_timings->X) _x1 = LcdStruct->raster_timings->X;
	if(_x2 < 0) _x2 = 0;
	if(_x2 >= LcdStruct->raster_timings->X) _x2 = LcdStruct->raster_timings->X;
	if(_y1 < 0) _y1 = 0;
	if(_y1 >= LcdStruct->raster_timings->Y) _y1 = LcdStruct->raster_timings->Y;
	if(_y2 < 0) _y2 = 0;
	if(_y2 >= LcdStruct->raster_timings->Y) _y2 = LcdStruct->raster_timings->Y;
	if(fill)
	{
		unsigned long size1 = 0;
		screen_set_area(_x1, _y1, _x2, _y2);
		MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			__far_mem_write(MI0283QT2_Addr, 0x22);
		}
		MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
		if(_y2-_y1 != 0) size1 = ((uint32_t)((_x2-_x1)+1)*(uint32_t)((_y2-_y1)+1));
		else size1 = (uint32_t)(_x2-_x1);
		if(_x2-_x1 != 0) size1 = ((uint32_t)((_x2-_x1)+1)*(uint32_t)((_y2-_y1)+1));
		else size1 = (uint32_t)(_y2-_y1);
		screen_send_pixels_via_dma(LcdStruct, size1, color);
	}
	else
	{	
		if(_x2 - _x1 > 0)
		{
			if(_y1 >= LcdStruct->sClipRegion.sYMin && _y1 < LcdStruct->sClipRegion.sYMax)
			{
				screen_set_area(_x1, _y1, _x2, _y1);
				MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					__far_mem_write(MI0283QT2_Addr, 0x22);
				}
				MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
				screen_send_pixels_via_dma(LcdStruct, (_x2 - _x1) + 1, color);
			}		
		
			if(_y2 >= LcdStruct->sClipRegion.sYMin && _y2 < LcdStruct->sClipRegion.sYMax)
			{
				screen_set_area(_x1, _y2, _x2, _y2);
				MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					__far_mem_write(MI0283QT2_Addr, 0x22);
				}		
				MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
				screen_send_pixels_via_dma(LcdStruct, (_x2 - _x1) + 1, color);
			}		
		}		
		
		if(_y2 - _y1 > 0)
		{
			if(_x1 >= LcdStruct->sClipRegion.sXMin && _x1 < LcdStruct->sClipRegion.sXMax)
			{
				screen_set_area(_x1, _y1, _x1, _y2);
				MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					__far_mem_write(MI0283QT2_Addr, 0x22);
				}
				MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
				screen_send_pixels_via_dma(LcdStruct, (_y2 - _y1) + 1, color);
			}		
		
			if(_x1 >= LcdStruct->sClipRegion.sXMin && _x1 < LcdStruct->sClipRegion.sXMax)
			{
				screen_set_area(_x2, _y1, _x2, _y2);
				MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					__far_mem_write(MI0283QT2_Addr, 0x22);
				}
				MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
				screen_send_pixels_via_dma(LcdStruct, (_y2 - _y1) + 1, color);
			}		
		}		
	}
}
//#######################################################################################
void _screen_put_horizontal_line_(tDisplay *LcdStruct, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	if(Y < LcdStruct->sClipRegion.sYMin) return;
	if(Y >= LcdStruct->sClipRegion.sYMax) return;
	//if(X1 < LcdStruct->sClipRegion.sXMin && X2 < LcdStruct->sClipRegion.sXMin) return;
	int16_t _x1 = X1, _x2 = (X1 + X2), _y = Y;
	if(_x1 >= LcdStruct->raster_timings->X || _x2 < 0) return;
	if(_x1 < LcdStruct->sClipRegion.sXMin) _x1 = LcdStruct->sClipRegion.sXMin;
	if(_x1 >= LcdStruct->sClipRegion.sXMax) _x1 = LcdStruct->sClipRegion.sXMax;
	if(_x2 < LcdStruct->sClipRegion.sXMin) _x2 = LcdStruct->sClipRegion.sXMin;
	if(_x2 >= LcdStruct->sClipRegion.sXMax) _x2 = LcdStruct->sClipRegion.sXMax;
	
	if(_x1 < 0) _x1 = 0;
	if(_x1 >= LcdStruct->raster_timings->X) _x1 = LcdStruct->raster_timings->X;
	if(_x2 < 0) _x2 = 0;
	if(_x2 >= LcdStruct->raster_timings->X) _x2 = LcdStruct->raster_timings->X;

	if(_x2 - _x1 > 0)
	{
		screen_set_area(_x1, _y, _x2, _y);
		MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			__far_mem_write(MI0283QT2_Addr, 0x22);
		}	
		MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
		screen_send_pixels_via_dma(LcdStruct, X2 * width, color);
	}	
}
//#######################################################################################
void _screen_put_vertical_line_(tDisplay *LcdStruct, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	if(X < LcdStruct->sClipRegion.sXMin) return;
	if(X >= LcdStruct->sClipRegion.sXMax) return;
	//if(Y1 < LcdStruct->sClipRegion.sYMin && Y2 < LcdStruct->sClipRegion.sYMin) return;
	int16_t _y1 = Y1, _y2 = (Y1 + Y2), _x = X;
	if(_y1 >= LcdStruct->raster_timings->Y || _y2 < 0) return;
	if(_y1 < LcdStruct->sClipRegion.sYMin) _y1 = LcdStruct->sClipRegion.sYMin;
	if(_y1 >= LcdStruct->sClipRegion.sYMax) _y1 = LcdStruct->sClipRegion.sYMax;
	if(_y2 < LcdStruct->sClipRegion.sYMin) _y2 = LcdStruct->sClipRegion.sYMin;
	if(_y2 >= LcdStruct->sClipRegion.sYMax) _y2 = LcdStruct->sClipRegion.sYMax;

	if(_y1 < 0) _y1 = 0;
	if(_y1 >= LcdStruct->raster_timings->Y) _y1 = LcdStruct->raster_timings->Y;
	if(_y2 < 0) _y2 = 0;
	if(_y2 >= LcdStruct->raster_timings->Y) _y2 = LcdStruct->raster_timings->Y;

	if(_y2 - _y1 > 0)
	{
		screen_set_area(_x, _y1, _x, _y2);
		MI0283QT2_RS_Port.OUTCLR = MI0283QT2_RS_Pin_bm;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			__far_mem_write(MI0283QT2_Addr, 0x22);
		}	
		MI0283QT2_RS_Port.OUTSET = MI0283QT2_RS_Pin_bm;
		screen_send_pixels_via_dma(LcdStruct, Y2 * width, color);
	}	
}
//#######################################################################################
