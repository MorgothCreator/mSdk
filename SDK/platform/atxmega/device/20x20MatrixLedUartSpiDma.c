/*
 * _20x20MatrixLedUartSpiDma.c
 *
 * Created: 2/21/2013 9:23:40 AM
 *  Author: XxXx
 */ 
//#######################################################################################
#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include <util/delay.h>
#include <stdbool.h>
#include <util/atomic.h>
#include <string.h>
#include <stdlib.h>
#include "board_properties.h"
#include "driver/dma.h"
//#include "api/lcd_api.h"
#include "api/lcd_def.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "interface/uart_interface.h"
#include "driver/uart.h"
#include "driver/timer.h"
#include "20x20MatrixLedUartSpiDma.h"
#include "board_init.h"
extern new_uart* DebugCom;
//#######################################################################################
new_dma_ch* DMA_SCREEN = NULL;
new_timer *Timer0_Struct = NULL;
new_uart* UART_SCREEN = NULL;
static unsigned char LathBuffer[5];
static unsigned char randered_col = 0;
//#######################################################################################
const unsigned char screen_col_mask[80]  PROGMEM =
{
	0x80, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00,
	0x20, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00,
	0x02, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x00, 0x00,
	0x00, 0x40, 0x00, 0x00,
	0x00, 0x20, 0x00, 0x00,
	0x00, 0x10, 0x00, 0x00,
	0x00, 0x08, 0x00, 0x00,
	0x00, 0x04, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x00,
	0x00, 0x00, 0x40, 0x00,
	0x00, 0x00, 0x20, 0x00,
	0x00, 0x00, 0x10, 0x00
};
//#######################################################################################
void screen_end_transfer(void* Data)
{
	new_uart *Struct = (new_uart *)Data;
	dma_complete_int(DMA_SCREEN);
	PORT_t *CS_Port = (PORT_t *)Struct->CS_Port;
	CS_Port->OUTCLR = Struct->CS_PinMask;
	CS_Port->OUTSET = Struct->CS_PinMask;
	SCLR_Port.OUTCLR = SCLR_bm;
	SCLR_Port.OUTSET = SCLR_bm;
}
//#######################################################################################
void screen_rander(void* Data)
{
	tDisplay* Struct = (tDisplay* )Data;
	unsigned char *screen_buff = (unsigned char *)Struct->UserData;
	screen_buff += randered_col * 3;
	unsigned char *_LathBuffer = LathBuffer;
	*_LathBuffer++ = ~pgm_read_byte(&screen_col_mask[randered_col << 2]);
	*_LathBuffer++ = ~pgm_read_byte(&screen_col_mask[((randered_col << 2) + 1)]);
	*_LathBuffer++ = ((~pgm_read_byte(&screen_col_mask[((randered_col << 2)) + 2])) & 0xF0) | (screen_buff[2] & 0x0F);
	*_LathBuffer++ = screen_buff[1];
	*_LathBuffer = *screen_buff;
	randered_col++;
	if(randered_col >= 20) 
	{
		memcpy(Struct->UserData, (void *)Struct->DisplayData, 60);
		randered_col = 0;
	}		
	SCLR_Port.OUTCLR = SCLR_bm;
	PORT_t *CS_Port = (PORT_t *)UART_SCREEN->CS_Port;
	SCLR_Port.OUTSET = SCLR_bm;
	CS_Port->OUTCLR = UART_SCREEN->CS_PinMask;
	CS_Port->OUTSET = UART_SCREEN->CS_PinMask;
	dma_search_and_use(DMA_SCREEN, 7);
}
//#######################################################################################
void screen_black(void* Data)
{
	LathBuffer[0]=0xFF;
	LathBuffer[1]=0xFF;
	LathBuffer[2]=0xF0;
	LathBuffer[3]=0x00;
	LathBuffer[4]=0x00;
	SCLR_Port.OUTCLR = SCLR_bm;
	PORT_t *CS_Port = (PORT_t *)UART_SCREEN->CS_Port;
	SCLR_Port.OUTSET = SCLR_bm;
	CS_Port->OUTCLR = UART_SCREEN->CS_PinMask;
	CS_Port->OUTSET = UART_SCREEN->CS_PinMask;
	dma_search_and_use((new_dma_ch *)Data, 7);
}
//#######################################################################################
void screen_open(tDisplay* LcdStruct)
{
	
	LcdStruct->sClipRegion.sXMin = 0;
	LcdStruct->sClipRegion.sXMax = LcdStruct->raster_timings->X;
	LcdStruct->sClipRegion.sYMin = 0;
	LcdStruct->sClipRegion.sYMax = LcdStruct->raster_timings->Y;
	
	LcdStruct->DisplayData = (void*)malloc(60);
	LcdStruct->UserData = (void*)malloc(60);
	if(LcdStruct->DisplayData == 0 && LcdStruct->UserData == 0) return;
	
	SCLR_Port.DIRSET = SCLR_bm;
	SCLR_Port.OUTSET = SCLR_bm;
	
	//LcdStruct->DisplayData[0] = 0xFF;
	//LcdStruct->DisplayData[1] = 0xFF;
	memset((void*)LcdStruct->DisplayData, 0xFF, 60);
	
	UART_SCREEN = new_(new_uart);
	UART_SCREEN->UartNr = 3;
	UART_SCREEN->BaudRate = 4000000;
	UART_SCREEN->Mode = UsartCom_Mode_Spi;
	UART_SCREEN->Priority = Usart_Int_Disabled;
	UART_SCREEN->CS_Port = (void*)&SCS_Port;
	UART_SCREEN->CS_PinMask = SCS_bm;
	uart_open(UART_SCREEN);
	


	DMA_SCREEN = new_(new_dma_ch);
	DMA_SCREEN->Dma_BlockLength = 5;
	DMA_SCREEN->Dma_BurstMode = _Dma_BurstMode_1;
	DMA_SCREEN->Dma_CompleteIntLevel = _Dma_Int_High_Level;
	DMA_SCREEN->Dma_DestAddr = (unsigned int)(void*)&USARTD1.DATA;
	DMA_SCREEN->Dma_DestAddrMode = _Dma_DestAddressMode_Fixed;
	DMA_SCREEN->Dma_DestAddrReload = _Dma_DestAddressReload_None;
	DMA_SCREEN->Dma_ErrIntLevel = _Dma_Int_Disabled;
	DMA_SCREEN->Dma_RepeatMode = false;
	DMA_SCREEN->Dma_RepeatNr = 1;
	DMA_SCREEN->Dma_SingleShot = true;
	DMA_SCREEN->Dma_SrcAddr = (unsigned int)(void*)&LathBuffer;
	DMA_SCREEN->Dma_SrcAddrMode = _Dma_SourceAddressMode_Inc;
	DMA_SCREEN->Dma_SrcAddrReload = _Dma_SourceAddressReload_None;
	DMA_SCREEN->Dma_Trigers = _Dma_Triger_Usartd1_Dre;
	DMA_SCREEN->CallBackTransferEnd = screen_end_transfer;
	DMA_SCREEN->CallBackTransferEndData = UART_SCREEN;
	dma_search_and_use(DMA_SCREEN, 7);
	
	Timer0_Struct = new_(new_timer);
	Timer0_Struct->Timer_PeriodUpdate = CoreFreq / (100 * 20);
	Timer0_Struct->Timer_CompCapUpdateA = (Timer0_Struct->Timer_PeriodUpdate / 10) * 9 ;
	Timer0_Struct->Timer_CompCapUpdateB = 0;
	Timer0_Struct->Timer_CompCapUpdateC = 0;
	Timer0_Struct->Timer_CompCapUpdateD = 0;
	Timer0_Struct->Timer_Number = 1;
	Timer0_Struct->Timer_ClockSel = _Timer_ClkSel_Div1;
	Timer0_Struct->Timer_WaveformGenMode = _Timer_WaveformGenMode_Normal;
	Timer0_Struct->Timer_EvAction = _Timer_EventAction_Off;
	Timer0_Struct->Timer_EvDelay = 0;
	Timer0_Struct->Timer_EvSourceSelect = _Timer_EventSource_Off;
	Timer0_Struct->Timer_ByteModeSel = _Timer_ByteMode_16Bit;
	Timer0_Struct->Timer_ErrIntLevel = _Timer_Int_Disabled;
	Timer0_Struct->Timer_OvfIntLevel = _Timer_Int_High_Level;
	Timer0_Struct->Timer_Com = _Timer_Command_None;
	Timer0_Struct->Timer_LockUpdate = false;
	Timer0_Struct->Timer_Dir = _Timer_Increment;
	Timer0_Struct->Timer_PerBuffValid = false;
	Timer0_Struct->Timer_CompCaptureEnA = true;
	Timer0_Struct->Timer_CompCaptureEnB = false;
	Timer0_Struct->Timer_CompCaptureEnC = false;
	Timer0_Struct->Timer_CompCaptureEnD = false;
	Timer0_Struct->Timer_CompOutValueA = false;
	Timer0_Struct->Timer_CompOutValueB = false;
	Timer0_Struct->Timer_CompOutValueC = false;
	Timer0_Struct->Timer_CompOutValueD = false;
	Timer0_Struct->Timer_CompCapIntLevelA = _Timer_Int_High_Level;
	Timer0_Struct->Timer_CompCapIntLevelB = _Timer_Int_Disabled;
	Timer0_Struct->Timer_CompCapIntLevelC = _Timer_Int_Disabled;
	Timer0_Struct->Timer_CompCapIntLevelD = _Timer_Int_Disabled;
	Timer0_Struct->Timer_CompCapBuffValidA = false;
	Timer0_Struct->Timer_CompCapBuffValidB = false;
	Timer0_Struct->Timer_CompCapBuffValidC = false;
	Timer0_Struct->Timer_CompCapBuffValidD = false;
	Timer0_Struct->CallBackOvf = screen_rander;
	Timer0_Struct->CallBackOvfData = (void*)LcdStruct;
	Timer0_Struct->CallBackCompCaptureA = screen_black;
	Timer0_Struct->CallBackCompCaptureDataA = (void*)DMA_SCREEN;
	tc_init(Timer0_Struct,1);
	screen_clear(LcdStruct, 0x0000);
}
//#######################################################################################
void screen_put_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16)
{
	if (X_Coordonate >= LcdStruct->sClipRegion.sXMin && Y_Coordonate >= LcdStruct->sClipRegion.sYMin && X_Coordonate < LcdStruct->sClipRegion.sXMax && Y_Coordonate < LcdStruct->sClipRegion.sYMax)
	{
		unsigned char *screen_buff = (unsigned char *)LcdStruct->DisplayData;
		if(Color16 < 0x8F) screen_buff[(X_Coordonate * 3) + (Y_Coordonate >> 3)] &= ~(1 << (Y_Coordonate & 0x07));
		else screen_buff[(X_Coordonate * 3) + (Y_Coordonate >> 3)] |= (1 << (Y_Coordonate & 0x07));
	}
}
//#######################################################################################
void screen_fill_area(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color)
{
	int16_t _x1 = x1, _x2 = (x1 + x2), _y1 = y1, _y2 = (y1 + y2);
	if(_x1 < 0) _x1 = 0;
	if(_x1 > (int16_t)LcdStruct->raster_timings->X) _x1 = (int16_t)LcdStruct->raster_timings->X;
	if(_x2 < 0) _x2 = 0;
	if(_x2 > (int16_t)LcdStruct->raster_timings->X) _x2 = (int16_t)LcdStruct->raster_timings->X;
	if(_y1 < 0) _y1 = 0;
	if(_y1 > (int16_t)LcdStruct->raster_timings->Y) _y1 = (int16_t)LcdStruct->raster_timings->Y;
	if(_y2 < 0) _y2 = 0;
	if(_y2 > (int16_t)LcdStruct->raster_timings->Y) _y2 = (int16_t)LcdStruct->raster_timings->Y;
	unsigned char Y_Cnt = 0;
	for(Y_Cnt = _y1; Y_Cnt < _y2; Y_Cnt++)
	{
		unsigned char X_Cnt = 0;
		for(X_Cnt = _x1; X_Cnt < _x2; X_Cnt++)
		{
			screen_put_pixel16(LcdStruct, X_Cnt, Y_Cnt, color);
		}
	}
}
//#######################################################################################
void screen_clear(tDisplay* LcdStruct, unsigned int color)
{
	screen_fill_area(LcdStruct, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, color);
}
//#######################################################################################
void screen_draw_rectangle(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, bool fill, unsigned int color)
{
	if(fill)
	{
		screen_fill_area(LcdStruct,x1, y1, x2, y2, color);
	}
	else
	{
		int16_t _x1 = x1, _x2 = (x1 + x2)-1, _y1 = y1, _y2 = (y1 + y2)-1;
		if(_x1 < 0) _x1 = 0;
		if(_x1 > (int16_t)LcdStruct->raster_timings->X) _x1 = (int16_t)LcdStruct->raster_timings->X;
		if(_x2 < 0) _x2 = 0;
		if(_x2 > (int16_t)LcdStruct->raster_timings->X) _x2 = (int16_t)LcdStruct->raster_timings->X;
		if(_y1 < 0) _y1 = 0;
		if(_y1 > (int16_t)LcdStruct->raster_timings->Y) _y1 = (int16_t)LcdStruct->raster_timings->Y;
		if(_y2 < 0) _y2 = 0;
		if(_y2 > (int16_t)LcdStruct->raster_timings->Y) _y2 = (int16_t)LcdStruct->raster_timings->Y;
	}
}
//#######################################################################################
void screen_put_horizontal_line(tDisplay *LcdStruct, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	if(Y < 0) return;
	if(Y > (int16_t)LcdStruct->raster_timings->Y) return;
	if(X1 < 0 && X2 < 0) return;
	int16_t _x1 = X1, _x2 = (X1 + X2), _y = Y;
	if(_x1 < 0) _x1 = 0;
	if(_x1 > (int16_t)LcdStruct->raster_timings->X) _x1 = (int16_t)LcdStruct->raster_timings->X;
	if(_x2 < 0) _x2 = 0;
	if(_x2 > (int16_t)LcdStruct->raster_timings->X) _x2 = (int16_t)LcdStruct->raster_timings->X;
	signed short ScanX;
	signed short Half_width1 = (width>>1);
	signed short Half_width2 = width-Half_width1;
	for(ScanX = _x1; ScanX < _x2; ScanX++)
	{
		for (signed short ScanY = _y - Half_width1; ScanY <= _y + Half_width2-1; ScanY++)
		{
			screen_put_pixel16(LcdStruct, ScanX, ScanY, color);
		}
	}
}
//#######################################################################################
void screen_put_vertical_line(tDisplay *LcdStruct, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	if(X < 0) return;
	if(X > (int16_t)LcdStruct->raster_timings->X) return;
	if(Y1 < 0 && Y2 < 0) return;
	int16_t _y1 = Y1, _y2 = (Y1 + Y2), _x = X;
	if(_y1 < 0) _y1 = 0;
	if(_y1 > (int16_t)LcdStruct->raster_timings->Y) _y1 = (int16_t)LcdStruct->raster_timings->Y;
	if(_y2 < 0) _y2 = 0;
	if(_y2 > (int16_t)LcdStruct->raster_timings->Y) _y2 = (int16_t)LcdStruct->raster_timings->Y;
	signed short ScanY;
	signed short Half_width1 = (width>>1);
	signed short Half_width2 = width-Half_width1;
	for(ScanY = _y1; ScanY < _y2; ScanY++)
	{
		for (signed short ScanX = _x - Half_width1; ScanX <= _x + Half_width2-1; ScanX++)
		{
			screen_put_pixel16(LcdStruct, ScanX, ScanY, color);
		}
	}
}
//#######################################################################################
