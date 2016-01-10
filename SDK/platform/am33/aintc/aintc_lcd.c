/*
 * aintc_lcd.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/**********************************************/
#include "aintc_lcd.h"
#include "api/lcd_def.h"
#include "../include/hw/hw_types.h"
#include "../include/raster.h"
#include "../include/interrupt.h"
#include "../include/hw/soc_AM335x.h"
/**********************************************/
extern tDisplay* ScreenRander;
/**
* \brief  This function Acknowledges interrupt.
*
* \param  baseAddr is the Memory address of the LCD instace used.
* \param  flag     is the value,which when written to EOI register
*                  acknowledges interrupt.
*
* \returns  None
*
* This API is only supported for AM335x raster.
*
**/
extern void RasterEndOfInterrupt(unsigned int baseAddr, unsigned int flag);
/**********************************************/
/*
** For each end of frame interrupt base and ceiling is reconfigured
*/
static void LCDIsr(void)
{
    unsigned int  status;

    status = RasterIntStatus(SOC_LCDC_0_REGS,RASTER_END_OF_FRAME0_INT_STAT |
                                             RASTER_END_OF_FRAME1_INT_STAT );

    status = RasterClearGetIntStatus(SOC_LCDC_0_REGS, status);

    if (status & RASTER_END_OF_FRAME0_INT_STAT)
    {
        RasterDMAFBConfig(SOC_LCDC_0_REGS,
                (unsigned int)ScreenRander->DisplayData,
                (unsigned int)ScreenRander->DisplayData + (ScreenRander->raster_timings->X * ScreenRander->raster_timings->Y * sizeof(ScreenRander->DisplayData[0])) - 2,
                          0);
    }

    if(status & RASTER_END_OF_FRAME1_INT_STAT)
    {
        RasterDMAFBConfig(SOC_LCDC_0_REGS,
                (unsigned int)ScreenRander->DisplayData,
                (unsigned int)ScreenRander->DisplayData + (ScreenRander->raster_timings->X * ScreenRander->raster_timings->Y * sizeof(ScreenRander->DisplayData[0])) - 2,
                          1);
    }

    //RasterEndOfInterrupt(SOC_LCDC_0_REGS, 0);
}
/**********************************************/
/*
** configures arm interrupt controller to generate raster interrupt
*/
void LCDAINTCConfigure(void)
{
	//IntProtectionDisable();
    /* Register the ISR in the Interrupt Vector Table.*/
    IntRegister(SYS_INT_LCDCINT, LCDIsr);

    IntPrioritySet(SYS_INT_LCDCINT, 0, AINTC_HOSTINT_ROUTE_IRQ );

    /* Enable the System Interrupts for AINTC.*/
    IntSystemEnable(SYS_INT_LCDCINT);
    //IntProtectionEnable();
}
/**********************************************/
/*
** configures arm interrupt controller to generate raster interrupt
*/
void LCDAINTCUnConfigure(void)
{
	//IntProtectionDisable();
    /* Enable the System Interrupts for AINTC.*/
    IntSystemDisable(SYS_INT_LCDCINT);

    /* Register the ISR in the Interrupt Vector Table.*/
    IntUnRegister(SYS_INT_LCDCINT);
    //IntProtectionEnable();
}
/**********************************************/



