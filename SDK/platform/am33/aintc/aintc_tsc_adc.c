/*
 * aintc_tsc_adc.c
 *
 *  Created on: Dec 11, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "aintc_tsc_adc.h"
#include "api/lcd_def.h"
#include "../interface/int_touchscreen_interface.h"
#include "../include/tsc_adc.h"
#include "../include/interrupt.h"
#include "../include/hw/hw_types.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
/*#####################################################*/
volatile unsigned int x_data[analog_touch_filter_level];
volatile unsigned int y_data[analog_touch_filter_level];
volatile unsigned int IsTSPress = 0;
volatile unsigned int touchRelease = 0;
volatile unsigned int dbidx = 0;
volatile unsigned int ana_cnt_touch = 0;
volatile signed int ignore_cnt = 0;
volatile unsigned int xdata = 0;
volatile unsigned int ydata = 0;
volatile unsigned int ignored_touch = analog_touch_filter_level;
/*#####################################################*/
static void TouchScreenIsr(void);

extern new_screen* ScreenRander;
/*#####################################################*/
/*
** Enables Touch Screen Interrupt
*/
void TouchIntEnable(void)
{
   /* Enable the FIFO Threshold interrupt */
   TSCADCEventInterruptEnable(SOC_ADC_TSC_0_REGS, TSCADC_FIFO1_THRESHOLD_INT);
}
/*#####################################################*/
/*
** Registers Touch Screen Interrupt
*/
void TouchIntRegister(void)
{
	IntProtectionDisable();
	IntRegister(SYS_INT_ADC_TSC_GENINT, TouchScreenIsr);

    IntPrioritySet(SYS_INT_ADC_TSC_GENINT, 0, AINTC_HOSTINT_ROUTE_IRQ);

    IntSystemEnable(SYS_INT_ADC_TSC_GENINT);
    IntProtectionEnable();
}
/*#####################################################*/
/*
** ISR for Touch Screen
*/
static void TouchScreenIsr(void)
{
    volatile unsigned int wordsLeft = 0;
    unsigned int status;
    unsigned int i = 0;
    unsigned int prevVal_x = 0xFFFFFFFF;
    unsigned int prevVal_y = 0xFFFFFFFF;
    unsigned int currDiff_x;
    unsigned int currDiff_y;
    unsigned int prevDiff_x = 0xFFFFFFFF;
    unsigned int prevDiff_y = 0xFFFFFFFF;
    unsigned int readx1;
    unsigned int ready1;

    status = TSCADCIntStatus(SOC_ADC_TSC_0_REGS);

	wordsLeft = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);

    if(status & TSCADC_FIFO1_THRESHOLD_INT)
    {
         for (i = 0; i < wordsLeft; i++)
         {
              readx1 = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
              readx1 = readx1 & 0xfff;

              if (readx1 > prevVal_x)
              {
                   currDiff_x = readx1 - prevVal_x;
              }
              else
              {
                   currDiff_x = prevVal_x - readx1;
              }
              if (currDiff_x < prevDiff_x)
              {
                   prevDiff_x = currDiff_x;
                   xdata = readx1;
              }
              prevVal_x = readx1;

              ready1 = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
              ready1 &= 0xfff;

              if (ready1 > prevVal_y)
              {
                  currDiff_y = ready1 - prevVal_y;
              }
              else
              {
                  currDiff_y = prevVal_y - ready1;
              }
              if (currDiff_y < prevDiff_y)
              {
                  prevDiff_y = currDiff_y;
                  ydata = ready1;
              }
              prevVal_y = ready1;
              wordsLeft = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
         }
         if(ScreenRander->touch_invert_x)
         {
			 x_data[dbidx] = 4095 - xdata;
         }
         else
         {
			 x_data[dbidx] = xdata;
         }
         if(ScreenRander->touch_invert_y)
         {
			 y_data[dbidx] = 4095 - ydata;
         }
         else
         {
			 y_data[dbidx] = ydata;
         }
         dbidx = (dbidx + 1) & (analog_touch_filter_level - 1);
    }

    /* Restart the timer counter */
    //DMTimerCounterSet(SOC_DMTIMER_3_REGS, 0);
    //DMTimerEnable(SOC_DMTIMER_3_REGS);

    //TSCADCEOIControl(SOC_ADC_TSC_0_REGS, 0);
    //TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS,  TSCADC_FIFO1_THRESHOLD_INT);
	TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS,   TSCADC_FIFO1_THRESHOLD_INT |
										   TSCADC_ASYNC_HW_PEN_EVENT_INT |
										   TSCADC_SYNC_PEN_EVENT_INT |
										   TSCADC_FIFO0_UNDER_FLOW_INT |
										   TSCADC_FIFO1_UNDER_FLOW_INT |
										   TSCADC_END_OF_SEQUENCE_INT |
										   TSCADC_FIFO0_THRESHOLD_INT |
										   TSCADC_FIFO0_OVER_RUN_INT |
										   TSCADC_FIFO1_OVER_RUN_INT |
										   TSCADC_OUT_OF_RANGE_INT |
										   TSCADC_PEN_UP_EVENT_INT);

	if(!ignored_touch) IsTSPress = 1;
	else ignored_touch--;
    touchRelease = 0;

    StepEnable();
}
/*#####################################################*/


