/*
 * lcd_interface.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/**********************************************/
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hw/hw_lcdc.h"
#include "lcd_interface.h"
#include "../include/raster.h"
#include "../include/edma.h"
#include "../clk/clk_lcd.h"
#include "../aintc/aintc_lcd.h"
#include "../pinmux/pin_mux_lcd.h"
#include "api/lcd_def.h"
#include "api/twi_def.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/pmic_api.h"
#include "api/uart_api.h"
#include "lib/gfx/gfx_util.h"
#include "../sys/cache.h"
#include "sys/platform.h"

#define EDMA_INST_BASE                 (SOC_EDMA30CC_0_REGS)/* EDMA3 Event queue number. */
#define EVT_QUEUE_NUM                  (0)
#define LCD_DMA_TRANSFER_CHANNEL		12
extern void (*cb_Fxn[EDMA3_NUM_TCC]) (unsigned int tcc, unsigned int status);
extern new_uart* DebugCom;

/**********************************************/
new_screen* ScreenRander;
/**********************************************/
//#define LCD_USE_DMA
unsigned int palete_raster_len = 8;
volatile bool LcdTransfer_callbackOccured = false;
/**
 * \brief  This API returns a unique number which identifies itself
 *         with the LCDC IP in AM335x SoC.
 * \param  None
 * \return This returns a number '2' which is unique to LCDC IP in AM335x.
 */
unsigned int LCDVersionGet(void)
{
    return 2;
}
/**
* \brief This function configures DMA present inside LCD controller.
*
* \param  baseAddr is the Memory address of LCD module.
*
* \param  frmMode  is the value which detemines either to use single frame
*                  or double frame buffer.\n
*
*                 frmMode can take following values.\n
*
*                 RASTER_SINGLE_FRAME_BUFFER - single frame buffer.\n
*                 RASTER_DOUBLE_FRAME_BUFFER - double frame buffer.\n
*
* \param bustSz   is the value which detemines burst size of DMA.\n
*
*                 bustSz can take following values.\n
*
*                 RASTER_BURST_SIZE_1   -  burst size of DMA is one.\n
*                 RASTER_BURST_SIZE_2   -  burst size of DMA is two.\n
*                 RASTER_BURST_SIZE_4   -  burst size of DMA is four.\n
*                 RASTER_BURST_SIZE_16  -  burst size of DMA is sixteen.\n
*
* \param fifoTh   is the value which detemines when the input FIFO can be
*                 read by raster controller.\n
*
*                 fifoTh can take following values.\n
*
*                 RASTER_FIFO_THRESHOLD_8    -   DMA FIFO Threshold is eight.\n
*                 RASTER_FIFO_THRESHOLD_16   -   DMA FIFO Threshold is sixteen.\n
*                 RASTER_FIFO_THRESHOLD_32   -   DMA FIFO Threshold is thirtytwo.\n
*                 RASTER_FIFO_THRESHOLD_64   -   DMA FIFO Threshold is sixtyfour.\n
*                 RASTER_FIFO_THRESHOLD_128  -   DMA FIFO Threshold is one twenty
*                                                eight.\n
*                 RASTER_FIFO_THRESHOLD_256  -   DMA FIFO Threshold is two
*                                                fifty six.\n
*                 RASTER_FIFO_THRESHOLD_512  -   DMA FIFO Threshold is five
*                                                twelve.\n
*
* \param endian   is value determines whether to use big endian for data
*                 reordering or not.\n
*
*                 endian can take following values.\n
*
*                 RASTER_BIG_ENDIAN_ENABLE - big endian enabled.\n
*                 RASTER_BIG_ENDIAN_ENABLE - big endian disabled.\n
*
* \return None
**/
void _RasterDMAConfig(unsigned int baseAddr, unsigned int frmMode,
                     unsigned int bustSz, unsigned int fifoTh,
                     unsigned int endian, unsigned int byteswap)
{
    HWREG(baseAddr + LCDC_LCDDMA_CTRL) = frmMode | bustSz | fifoTh | endian | byteswap;
}
/**********************************************/
/*
** This function is used as a callback from EDMA3 Completion Handler.
*/
void callbackLcdTransfer(unsigned int tccNum, unsigned int status)
{
	LcdTransfer_callbackOccured = true;
    EDMA3DisableTransfer(EDMA_INST_BASE, tccNum, EDMA3_TRIG_MODE_MANUAL);
}
/**********************************************/
/*
** Configures raster to display image
*/
bool SetUpLCD(tDisplay* LcdStruct)
{
	ScreenRander = LcdStruct;

	LcdStruct->sClipRegion.sXMax = LcdStruct->raster_timings->X;
	LcdStruct->sClipRegion.sYMax = LcdStruct->raster_timings->Y;
#ifdef gcc
	LcdStruct->DisplayData = (volatile unsigned int *)malloc((LcdStruct->raster_timings->X * LcdStruct->raster_timings->Y * sizeof(LcdStruct->DisplayData[0])) + (LcdStruct->raster_timings->palete_len * sizeof(LcdStruct->DisplayData[0])));
#else
	LcdStruct->DisplayData = (volatile unsigned int *)memalign(sizeof(LcdStruct->DisplayData[0]) << 3, (LcdStruct->Width * LcdStruct->Height * sizeof(LcdStruct->DisplayData[0])) +  + (palete_raster_len * sizeof(LcdStruct->DisplayData[0])));
#endif
    LCDAINTCConfigure();

    DisplayPLLInit(LcdStruct->raster_timings->ref_clk, LcdStruct->raster_timings->pll_m, LcdStruct->raster_timings->pll_n, LcdStruct->raster_timings->pll_m2);
    /* Enable clock for LCD Module */
    LCDModuleClkConfig();

    LCDPinMuxSetup();

    /*
    **Clock for DMA,LIDD and for Core(which encompasses
    ** Raster Active Matrix and Passive Matrix logic)
    ** enabled.
    */
    RasterClocksEnable(SOC_LCDC_0_REGS);

    /* Disable raster */
    RasterDisable(SOC_LCDC_0_REGS);

    /* Configure the pclk */
    RasterClkConfig(SOC_LCDC_0_REGS, LcdStruct->raster_timings->raster_clk, ((LcdStruct->raster_timings->ref_clk / LcdStruct->raster_timings->pll_n) * LcdStruct->raster_timings->pll_m) * LcdStruct->raster_timings->pll_m2);

    /* Configuring DMA of LCD controller */
    _RasterDMAConfig(SOC_LCDC_0_REGS, RASTER_DOUBLE_FRAME_BUFFER,
                    RASTER_BURST_SIZE_16, RASTER_FIFO_THRESHOLD_8,
                    RASTER_BIG_ENDIAN_ENABLE, RASTER_BYTE_SWAP_DISABLE);

    /* Configuring modes(ex:tft or stn,color or monochrome etc) for raster controller */
    RasterModeConfig(SOC_LCDC_0_REGS, RASTER_DISPLAY_MODE_TFT_UNPACKED,
                     RASTER_PALETTE_DATA, RASTER_COLOR, RASTER_RIGHT_ALIGNED);

    //RasterMSBDataOrderSelect(SOC_LCDC_0_REGS);


     /* Configuring the polarity of timing parameters of raster controller */
    RasterTiming2Configure(SOC_LCDC_0_REGS, RASTER_FRAME_CLOCK_LOW |
                                            RASTER_LINE_CLOCK_LOW  |
                                            RASTER_PIXEL_CLOCK_HIGH|
                                            RASTER_SYNC_EDGE_RISING|
                                            RASTER_SYNC_CTRL_ACTIVE|
                                            RASTER_AC_BIAS_HIGH     , 0, 255);

	/* Configuring horizontal timing parameter */
			RasterHparamConfig(SOC_LCDC_0_REGS, LcdStruct->raster_timings->X, LcdStruct->raster_timings->hsw, LcdStruct->raster_timings->hfp, LcdStruct->raster_timings->hbp);

	/* Configuring vertical timing parameters */
			RasterVparamConfig(SOC_LCDC_0_REGS, LcdStruct->raster_timings->Y, LcdStruct->raster_timings->vsw, LcdStruct->raster_timings->vfp, LcdStruct->raster_timings->vbp);

    RasterFIFODMADelayConfig(SOC_LCDC_0_REGS, 128);

    /* Configuring the base ceiling */
    RasterDMAFBConfig(SOC_LCDC_0_REGS,
                      (unsigned int)LcdStruct->DisplayData,
                      (unsigned int)LcdStruct->DisplayData + (LcdStruct->raster_timings->X * LcdStruct->raster_timings->Y * sizeof(LcdStruct->DisplayData[0])) + (LcdStruct->raster_timings->palete_len * sizeof(LcdStruct->DisplayData[0])) - 2,
                      0);

    RasterDMAFBConfig(SOC_LCDC_0_REGS,
                      (unsigned int)LcdStruct->DisplayData,
                      (unsigned int)LcdStruct->DisplayData + (LcdStruct->raster_timings->X * LcdStruct->raster_timings->Y * sizeof(LcdStruct->DisplayData[0])) + (LcdStruct->raster_timings->palete_len * sizeof(LcdStruct->DisplayData[0])) - 2,
                      1);

    /* Enable End of frame0/frame1 interrupt */
    RasterIntEnable(SOC_LCDC_0_REGS, RASTER_END_OF_FRAME0_INT |
                                     RASTER_END_OF_FRAME1_INT);

    /* Enable raster */
    RasterEnable(SOC_LCDC_0_REGS);
#if 0
    unsigned int retVal = 0u;
	/* Request DMA Channel and TCC for MMCSD Transmit*/
    retVal = EDMA3RequestChannel(EDMA_INST_BASE, EDMA3_CHANNEL_TYPE_DMA,
			LCD_DMA_TRANSFER_CHANNEL, LCD_DMA_TRANSFER_CHANNEL,
						EVT_QUEUE_NUM);

	/* Registering Callback Function for TX*/
	cb_Fxn[LCD_DMA_TRANSFER_CHANNEL] = &callbackLcdTransfer;

	if(!retVal) UARTPuts(DebugCom, "Failed to register LCD DMA Transfer.\n\r" , -1);
#endif

    return true;
}
/**********************************************/
void _lcd_enable()
{
	RasterEnable(SOC_LCDC_0_REGS);
}
/**********************************************/
void _lcd_disable()
{
    RasterDisable(SOC_LCDC_0_REGS);
}
//#######################################################################################
void _screen_backlight_on(tDisplay *pDisplay)
{
	if(pDisplay->pmic_back_light)
	{
		pmic_backlight_enable(pDisplay->PmicTwiModuleStruct);
		pmic_backlight_level(pDisplay->PmicTwiModuleStruct, pDisplay->BackLightLevel);
	}
	else
	{
		if(pDisplay->invert_backlight) gpio_out(pDisplay->BackLight, 0);
		else gpio_out(pDisplay->BackLight, 1);
	}
}
//#######################################################################################
void _screen_backlight_off(tDisplay *pDisplay)
{
	if(pDisplay->pmic_back_light)
	{
		pmic_backlight_enable(pDisplay->PmicTwiModuleStruct);
		pmic_backlight_level(pDisplay->PmicTwiModuleStruct, 0);
	}
	else
	{
		if(pDisplay->invert_backlight) gpio_out(pDisplay->BackLight, 1);
		else gpio_out(pDisplay->BackLight, 0);
	}
}
//#######################################################################################
bool _screen_copy(tDisplay *pDisplayTo, tDisplay *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{
	if(pDisplayTo->raster_timings->X != pDisplayFrom->raster_timings->X || pDisplayTo->raster_timings->Y != pDisplayFrom->raster_timings->Y) return false;
	CacheDataCleanBuff((unsigned int)pDisplayFrom->DisplayData, (pDisplayFrom->raster_timings->X * pDisplayFrom->raster_timings->Y * sizeof(pDisplayFrom->DisplayData[0])) + (pDisplayFrom->raster_timings->palete_len * sizeof(pDisplayFrom->DisplayData[0])));
	signed int LineCnt = 0;
	volatile unsigned int* ScreenBuff = pDisplayTo->DisplayData + pDisplayTo->raster_timings->palete_len;
	volatile unsigned int* _ScreenBuff = pDisplayFrom->DisplayData + pDisplayTo->raster_timings->palete_len;

#if 1
	for(; LineCnt < pDisplayTo->raster_timings->Y; LineCnt ++)
	{
		memcpy((void *)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (void *)(_ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X));
		if(put_cursor == true && LineCnt >= Y && LineCnt <= Y + 2)
		{
			unsigned int cnt_x = X;
			for(;cnt_x < X + 2; cnt_x++) _put_pixel(pDisplayTo, cnt_x, LineCnt, color);
		}
		CacheDataCleanInvalidateBuff((unsigned int)((unsigned int*)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt))), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X) + 64);
	}
#else

#if 1
	EDMA3CCPaRAMEntry paramSet;

    paramSet.srcBIdx    = (pDisplayFrom->raster_timings->X * sizeof(pDisplayFrom->DisplayData[0]));
    paramSet.srcCIdx    = 1;
    paramSet.destBIdx   = (pDisplayFrom->raster_timings->X * sizeof(pDisplayFrom->DisplayData[0]));
    paramSet.destCIdx   = 1;
    paramSet.aCnt       = (pDisplayFrom->raster_timings->X * sizeof(pDisplayFrom->DisplayData[0]));
    paramSet.bCnt       = 1;
    paramSet.cCnt       = 1;
    paramSet.bCntReload = 0x0;
    paramSet.linkAddr   = 0xffff;
    paramSet.opt        = 0x00000000;
    /* 4.  AB-Sync mode */
    int dma_channel = LCD_DMA_TRANSFER_CHANNEL;

    paramSet.opt |= (1 << 2) | (1 << 3) | (1 << EDMA3CC_OPT_TCINTEN_SHIFT) | ((dma_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);
	for(; LineCnt < pDisplayTo->raster_timings->Y; LineCnt ++)
	{
		//memcpy((void *)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (void *)(_ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X));
	    paramSet.srcAddr    = (unsigned int)(_ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt));
	    paramSet.destAddr   = (unsigned int)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt));
	    /* configure PaRAM Set */
	    EDMA3SetPaRAM(EDMA_INST_BASE, dma_channel, &paramSet);
	    /* Enable the transfer */
	    LcdTransfer_callbackOccured = false;
	    EDMA3EnableTransfer(EDMA_INST_BASE, dma_channel, EDMA3_TRIG_MODE_MANUAL);

	    /* Wait for transfer complete*/
	    while(!LcdTransfer_callbackOccured);
	    /* Paint cursor on screen*/
		if(put_cursor == true && LineCnt >= Y && LineCnt <= Y + 2)
		{
			unsigned int cnt_x = X;
			for(;cnt_x < X + 2; cnt_x++) _put_pixel(pDisplayTo, cnt_x, LineCnt, color);
			CacheDataCleanInvalidateBuff((unsigned int)((unsigned int*)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt))), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X) + 64);
		}
	}
#else

	EDMA3CCPaRAMEntry paramSet;

    paramSet.srcAddr    = (unsigned int)_ScreenBuff;
    paramSet.destAddr   = (unsigned int)ScreenBuff;
    paramSet.srcBIdx    = (pDisplayFrom->raster_timings->X * sizeof(pDisplayFrom->DisplayData[0]));
    paramSet.srcCIdx    = pDisplayFrom->raster_timings->Y;
    paramSet.destBIdx   = (pDisplayFrom->raster_timings->X * sizeof(pDisplayFrom->DisplayData[0]));
    paramSet.destCIdx   = pDisplayFrom->raster_timings->Y;
    paramSet.aCnt       = (pDisplayFrom->raster_timings->X * sizeof(pDisplayFrom->DisplayData[0]));
    paramSet.bCnt       = pDisplayFrom->raster_timings->Y;
    paramSet.cCnt       = 1;
    paramSet.bCntReload = 0x0;
    paramSet.linkAddr   = 0xffff;
    paramSet.opt        = 0x00000000;
    /* 4.  AB-Sync mode */
    int dma_channel = LCD_DMA_TRANSFER_CHANNEL;

    paramSet.opt |= (1 << 2) | (1 << 3) | (1 << EDMA3CC_OPT_TCINTEN_SHIFT) | ((dma_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

    /* configure PaRAM Set */
    EDMA3SetPaRAM(EDMA_INST_BASE, dma_channel, &paramSet);
    /* Enable the transfer */
    LcdTransfer_callbackOccured = false;
    EDMA3EnableTransfer(EDMA_INST_BASE, dma_channel, EDMA3_TRIG_MODE_MANUAL);

    /* Wait for transfer complete*/
    while(!LcdTransfer_callbackOccured);
    /* Paint cursor on screen*/
    LineCnt = Y;
	for(; LineCnt < Y + 2; LineCnt ++)
	{
		unsigned int cnt_x = X;
		for(;cnt_x < X + 2; cnt_x++) _put_pixel(pDisplayTo, cnt_x, LineCnt, color);
		CacheDataCleanInvalidateBuff((unsigned int)((unsigned int*)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt))), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X) + 64);
	}
#endif
#endif
	return true;
}
//#######################################################################################
void _box_cache_clean(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{
	signed int x_end = x_start + x_len ,y_end = y_start + y_len;
	if(x_start >= pDisplay->sClipRegion.sXMax || y_start >= pDisplay->sClipRegion.sYMax || x_end < pDisplay->sClipRegion.sXMin || y_end < pDisplay->sClipRegion.sYMin) return;
	register signed int LineCnt = y_start;
	if(LineCnt < pDisplay->sClipRegion.sYMin) LineCnt = pDisplay->sClipRegion.sYMin;
	signed int _x_start = x_start;
	if(_x_start < pDisplay->sClipRegion.sXMin) _x_start = pDisplay->sClipRegion.sXMin;
	signed int _x_end = x_end;
	if(_x_end > pDisplay->sClipRegion.sXMax) _x_end = pDisplay->sClipRegion.sXMax;
	unsigned int width_to_refresh = (_x_end - _x_start)+ 64;
	if((width_to_refresh + _x_start) > pDisplay->sClipRegion.sXMax) width_to_refresh = (pDisplay->sClipRegion.sXMax - _x_start) + 64;
	width_to_refresh *= sizeof(unsigned int);
	volatile unsigned int* ScreenBuff = pDisplay->DisplayData + palete_raster_len + _x_start;
	for(; LineCnt < y_end; LineCnt++)
	{
		if(LineCnt >= pDisplay->sClipRegion.sYMax) return;
		CacheDataCleanInvalidateBuff((unsigned int)((unsigned int*)(ScreenBuff + (pDisplay->raster_timings->X * LineCnt))), width_to_refresh);
	}
}
//#######################################################################################
void _put_rectangle(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	signed int x_end = x_start + x_len ,y_end = y_start + y_len;
	if(x_start >= pDisplay->sClipRegion.sXMax || y_start >= pDisplay->sClipRegion.sYMax || x_end < pDisplay->sClipRegion.sXMin || y_end < pDisplay->sClipRegion.sYMin) return;
	register signed int LineCnt = y_start;
	volatile unsigned int* ScreenBuff = pDisplay->DisplayData + pDisplay->raster_timings->palete_len;
	unsigned int _color = color<<8;
	if(fill)
	{
		if(LineCnt < pDisplay->sClipRegion.sYMin) LineCnt = pDisplay->sClipRegion.sYMin;
		signed int _x_start = x_start;
		if(_x_start < pDisplay->sClipRegion.sXMin) _x_start = pDisplay->sClipRegion.sXMin;
		register signed int _x_end = x_end;
		if(_x_end > pDisplay->sClipRegion.sXMax) _x_end = pDisplay->sClipRegion.sXMax;
		unsigned int width_to_refresh = (_x_end - _x_start)+ 64;
		if((width_to_refresh + _x_start) > pDisplay->sClipRegion.sXMax) width_to_refresh = (pDisplay->sClipRegion.sXMax - _x_start) + 64;
#if 1
		for( ; LineCnt < y_end; LineCnt++)
		{
			if(LineCnt >= pDisplay->sClipRegion.sYMax) return;
			register int x = _x_start;
			for( ; x < _x_end ; x++)
			{
				ScreenBuff[x + (pDisplay->raster_timings->X * LineCnt)] = _color;
			}
		}
#else
#if 0
	    EDMA3CCPaRAMEntry paramSet;
	    //unsigned int _color = color << 8;
	    CacheDataCleanBuff((unsigned int)&_color, sizeof(_color));

	    paramSet.srcAddr    = (unsigned int)&_color;
	    paramSet.destAddr   = (unsigned int)ScreenBuff + (_x_start * sizeof(pDisplay->DisplayData[0])) + (pDisplay->raster_timings->X * LineCnt * sizeof(pDisplay->DisplayData[0]));
	    paramSet.srcBIdx    = 0;
	    paramSet.srcCIdx    = 0;
	    paramSet.destBIdx   = pDisplay->raster_timings->X * sizeof(pDisplay->DisplayData[0]);
	    paramSet.destCIdx   = 0;
	    paramSet.aCnt       = sizeof(pDisplay->DisplayData[0]) * (_x_end - _x_start);
	    paramSet.bCnt       = y_end - LineCnt;
	    paramSet.cCnt       = 1;//y_end - LineCnt;
	    paramSet.bCntReload = 0;
	    paramSet.linkAddr   = 0xffff;
	    paramSet.opt        = 0x00000000;
	    int dma_channel = LCD_DMA_TRANSFER_CHANNEL;
	    /* 4.  AB-Sync mode */
	    paramSet.opt |= /*(1 << 0) | (2 << 8) | */(1 << 2) | (1 << 3) | (1 << EDMA3CC_OPT_TCINTEN_SHIFT) | ((dma_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);
	    /* 2. Read FIFO : SRC Constant addr mode */
	    //paramSet.opt |= (1 << 0);
	    /* 3. DST FIFO width is 32 bit */
	    //paramSet.opt |= (2 << 8);
	    /* 1. Transmission complition interrupt enable */
	    //paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

	    /* configure PaRAM Set */
	    EDMA3SetPaRAM(EDMA_INST_BASE, dma_channel, &paramSet);
	    /* Enable the transfer */
	    LcdTransfer_callbackOccured = false;
    	EDMA3EnableTransfer(EDMA_INST_BASE, dma_channel, EDMA3_TRIG_MODE_MANUAL);
	    while(!LcdTransfer_callbackOccured);
#else
	    EDMA3CCPaRAMEntry paramSet;
	    //unsigned int _color = color << 8;
	    CacheDataCleanBuff((unsigned int)&_color, sizeof(_color));

	    paramSet.srcAddr    = (unsigned int)&_color;
	    paramSet.srcBIdx    = 0;
	    paramSet.srcCIdx    = 0;
	    paramSet.destBIdx   = sizeof(pDisplay->DisplayData[0]);
	    paramSet.destCIdx   = pDisplay->raster_timings->X * sizeof(pDisplay->DisplayData[0]);
	    paramSet.aCnt       = sizeof(pDisplay->DisplayData[0]);
	    paramSet.bCnt       = _x_end - _x_start;
	    paramSet.cCnt       = 1;//y_end - LineCnt;
	    paramSet.bCntReload = 0;
	    paramSet.linkAddr   = 0xffff;
	    paramSet.opt        = 0x00000000;
	    int dma_channel = LCD_DMA_TRANSFER_CHANNEL;
	    /* 4.  AB-Sync mode */
	    paramSet.opt |= /*(1 << 0) | (2 << 8) | */(1 << 2) | (1 << 3) | (1 << EDMA3CC_OPT_TCINTEN_SHIFT) | ((dma_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

	    for( ; LineCnt < y_end; LineCnt++)
		{
			if(LineCnt >= pDisplay->sClipRegion.sYMax) return;
		    paramSet.destAddr   = (unsigned int)ScreenBuff + ((_x_start - 1) * sizeof(pDisplay->DisplayData[0])) + (pDisplay->raster_timings->X * LineCnt * sizeof(pDisplay->DisplayData[0]));
		    /* configure PaRAM Set */
		    EDMA3SetPaRAM(EDMA_INST_BASE, dma_channel, &paramSet);
		    /* Enable the transfer */
		    LcdTransfer_callbackOccured = false;
	    	EDMA3EnableTransfer(EDMA_INST_BASE, dma_channel, EDMA3_TRIG_MODE_MANUAL);
		    while(!LcdTransfer_callbackOccured);
		    CacheDataCleanInvalidateBuff((unsigned int)paramSet.destAddr - 64, ((_x_end - _x_start) * sizeof(pDisplay->DisplayData[0])) + 128);
	    }
#endif
#endif
		return;
	}

	register int _x_end = x_end;
	int _x_start = x_start;
	if(_x_end >= pDisplay->sClipRegion.sXMax) _x_end = pDisplay->sClipRegion.sXMax;
	if(_x_start < pDisplay->sClipRegion.sXMin) _x_start = pDisplay->sClipRegion.sXMin;
	if(y_start >= pDisplay->sClipRegion.sYMin)
	{
		for(LineCnt = _x_start ; LineCnt < _x_end ; LineCnt++)
		{
			ScreenBuff[LineCnt + (pDisplay->raster_timings->X * y_start)] = _color;
		}
	}

	if(y_end <= pDisplay->sClipRegion.sYMax)
	{
		for(LineCnt = _x_start ; LineCnt < _x_end ; LineCnt++)
		{
			ScreenBuff[LineCnt + (pDisplay->raster_timings->X * (y_end - 1))] = _color;
		}
	}

	int _y_end = y_end;
	if(_y_end >= pDisplay->sClipRegion.sYMax) _y_end = pDisplay->sClipRegion.sYMax;
	int _y_start = y_start;
	if(_y_start < pDisplay->sClipRegion.sYMin) _y_start = pDisplay->sClipRegion.sYMin;
	if(x_start >= pDisplay->sClipRegion.sXMin)
	{
		for(LineCnt = _y_start ; LineCnt < _y_end ; LineCnt++)
		{
			ScreenBuff[x_start + (pDisplay->raster_timings->X * LineCnt)] = _color;
		}
	}

	if(x_end <= pDisplay->sClipRegion.sXMax)
	{
		for(LineCnt = _y_start ; LineCnt < _y_end ; LineCnt++)
		{
			ScreenBuff[(x_end - 1) + (pDisplay->raster_timings->X * LineCnt)] = _color;
		}
	}

	return;
}
//#######################################################################################
void _put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color)
{
	if(X >= pDisplay->sClipRegion.sXMin && Y >= pDisplay->sClipRegion.sYMin && X < pDisplay->sClipRegion.sXMax && Y < pDisplay->sClipRegion.sYMax)
	{
		pDisplay->DisplayData[X + pDisplay->raster_timings->palete_len + (pDisplay->raster_timings->X * Y)] = color;
	}
}
//#######################################################################################
void _screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	///Write rgb array to video memory
	unsigned char *Buff = (unsigned char *)rgb_buffer;
	//unsigned long Length = (width*height)<<1;

	register int   x, y;
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
			_put_pixel(pDisplay, x, y, color);
		}
		CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + pDisplay->raster_timings->palete_len + x1 + (pDisplay->raster_timings->X * y), width * 4);
	}
	//lcd.dblbuf = dblbuf;
}
//#######################################################################################
void _screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	register unsigned char *Buff = rgb_buffer;
	register int   y;
	for(y =y1; y<(height+y1);y++)
	{
		if(y < pDisplay->sClipRegion.sYMin || y > pDisplay->sClipRegion.sYMax);
		else
		{
			register unsigned int *DisplayStartLine = (unsigned int *)pDisplay->DisplayData + pDisplay->raster_timings->palete_len + x1 + (pDisplay->raster_timings->X * y);
			register unsigned int *DisplayEndLine = DisplayStartLine + width;
			while(DisplayStartLine < DisplayEndLine)
			{
				*DisplayStartLine++= ((*Buff)<<24) | ((*(Buff+1))<<16) | ((*(Buff+2))<<8);
				Buff+=3;
			}
			CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + pDisplay->raster_timings->palete_len + x1 + (pDisplay->raster_timings->X * y * 4), width * 4);
		}
	}
}
//#######################################################################################
void _screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	register int   y;
	register int _y = 0;
	signed int _width = width;
	if(x1 + _width > pDisplay->sClipRegion.sXMax) _width = pDisplay->sClipRegion.sXMax - x1;
	_width *= 4;
	for(y =y1; y<(height+y1);y++)
	{
		if(y < pDisplay->sClipRegion.sYMin || y > pDisplay->sClipRegion.sYMax);
		else
		{
			unsigned int *DisplayStartLine = (unsigned int *)pDisplay->DisplayData + pDisplay->raster_timings->palete_len + x1 + (pDisplay->raster_timings->X * y);
			unsigned char *Buff = rgb_buffer + (width * _y * 4) - 1;
			_y++;
			memcpy((void*)(DisplayStartLine), (void*)(Buff), width * 4);
			CacheDataCleanBuff((unsigned int)((unsigned int*)(pDisplay->DisplayData + pDisplay->raster_timings->palete_len + x1 + (pDisplay->raster_timings->X * y))), _width);
		}
	}
}
//#######################################################################################
void _screen_put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	if(width == 1 && (Y < pDisplay->sClipRegion.sYMin || Y >= pDisplay->sClipRegion.sYMax))
		return;
	register int X1_Tmp = X1, X2_Tmp = X1 + X2;
	if(X1_Tmp <= (int)pDisplay->sClipRegion.sXMin) X1_Tmp = (int)pDisplay->sClipRegion.sXMin;
	if(X1_Tmp >= (int)pDisplay->sClipRegion.sXMax) X1_Tmp = (int)pDisplay->sClipRegion.sXMax;
	if(X2_Tmp <= (int)pDisplay->sClipRegion.sXMin) X2_Tmp = (int)pDisplay->sClipRegion.sXMin;
	if(X2_Tmp >= (int)pDisplay->sClipRegion.sXMax) X2_Tmp = (int)pDisplay->sClipRegion.sXMax;
	if(Y < (int)pDisplay->sClipRegion.sYMin) Y = (int)pDisplay->sClipRegion.sYMin;
	if(Y >= (int)pDisplay->sClipRegion.sYMax) Y = (int)pDisplay->sClipRegion.sYMax;
	int Half_width1 = (width>>1);
	int Half_width2 = width-Half_width1;
	#ifdef USE_16_BIT_COLOR_DEPTH
	unsigned int _color = color;
	#else
	unsigned int _color = color<<8;
	#endif
	for(;X1_Tmp < X2_Tmp; X1_Tmp++)
	{
		register int _Y_ = Y - Half_width1;
		for(; _Y_ < Y + Half_width2; _Y_++) _put_pixel(pDisplay,(X1_Tmp),(_Y_), _color);
	}
}
//#######################################################################################
void _screen_put_vertical_line(tDisplay *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	if(width == 1 && (X < pDisplay->sClipRegion.sXMin || X >= pDisplay->sClipRegion.sXMax))
		return;
	register int Y1_Tmp = Y1, Y2_Tmp = Y1 + Y2;
	if(X <= (int)pDisplay->sClipRegion.sXMin) X = (int)pDisplay->sClipRegion.sXMin;
	if(X >= (int)pDisplay->sClipRegion.sXMax) X = (int)pDisplay->sClipRegion.sXMax;
	if(Y1_Tmp <= (int)pDisplay->sClipRegion.sYMin) Y1_Tmp = (int)pDisplay->sClipRegion.sYMin;
	if(Y1_Tmp >= (int)pDisplay->sClipRegion.sYMax) Y1_Tmp = (int)pDisplay->sClipRegion.sYMax;
	if(Y2_Tmp <= (int)pDisplay->sClipRegion.sYMin) Y2_Tmp = (int)pDisplay->sClipRegion.sYMin;
	if(Y2_Tmp >= (int)pDisplay->sClipRegion.sYMax) Y2_Tmp = (int)pDisplay->sClipRegion.sYMax;
	int Half_width1 = (width>>1);
	int Half_width2 = width-Half_width1;
	#ifdef USE_16_BIT_COLOR_DEPTH
	unsigned int _color = color;
	#else
	unsigned int _color = color<<8;
	#endif
	for(;Y1_Tmp < Y2_Tmp; Y1_Tmp++)
	{
		register int _X_ = X - Half_width1;
		for(; _X_ < X + Half_width2; _X_++) _put_pixel(pDisplay,(_X_),(Y1_Tmp), _color);
	}
}
//#######################################################################################
void _screen_clear(tDisplay *pDisplay, unsigned int color)
{
	_put_rectangle(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, color);
	_box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
}
