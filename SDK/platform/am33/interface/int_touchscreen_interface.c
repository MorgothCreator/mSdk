/*
 * int_touchscreen_interface.c
 *
 *  Created on: Dec 11, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "api/touchscreen_def.h"
#include "int_touchscreen_interface.h"
#include "lib/gfx/controls_definition.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_types.h"
#include "../include/tsc_adc.h"
#include "../include/dmtimer.h"
#include "../clk/clk_tsc_adc.h"
#include "../aintc/aintc_tsc_adc.h"
#include "../pinmux/pin_mux_adc.h"
#include "lib/generic.h"
#include "api/lcd_api.h"
#include "../clk/clk_dmtimer.h"
#include "api/timer_api.h"
/*#####################################################*/
/******************************************************************************
**              INTERNAL MACRO DEFINITIONS
******************************************************************************/
#define SAMPLES       5
/*******************************************************************************
**                     INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static void StepConfigX(unsigned int);
static void StepConfigY(unsigned int);
/*******************************************************************************
**                     INTERNAL VARIALE DEFINITIONS
*******************************************************************************/
extern volatile unsigned int x_data[analog_touch_filter_level];
extern volatile unsigned int y_data[analog_touch_filter_level];
extern volatile unsigned int IsTSPress;
extern volatile unsigned int touchRelease;
extern volatile unsigned int dbidx;
extern volatile unsigned int ana_cnt_touch;
extern volatile signed int ignore_cnt;
extern volatile unsigned int ignored_touch;


extern volatile unsigned int xdata;
extern volatile unsigned int ydata;

/*******************************************************************************
**                     FUNCTION DEFINITIONS
*******************************************************************************/
/*
** Initializes the touch screen
*/
void InitTouchScreen(LcdTouch_t* structure)
{
	structure->TouchScreen_Type = TouchScreen_Type_Int;

	unsigned int i;
	/*	TSC clock config	*/
	TSCADCModuleClkConfig();
	/*	TSC pin mux	*/
	pin_mux_adc_all();

	TouchIntRegister();

	/*	Timer3 Clock config	*/
	//DMTimer3ModuleClkConfig(TimerClkSource_CLK_M_OSC);

    /* configures ADC to 3Mhz */
    TSCADCConfigureAFEClock(SOC_ADC_TSC_0_REGS, 24000000, 1000000);

    /* Enable Transistor bias */
    TSCADCTSTransistorConfig(SOC_ADC_TSC_0_REGS, TSCADC_TRANSISTOR_ENABLE);

    /* Map hardware event to Pen Touch IRQ */
    TSCADCHWEventMapSet(SOC_ADC_TSC_0_REGS, TSCADC_PEN_TOUCH);

    /* Set 4 Wire or 5 wire touch screen  mode */
    TSCADCTSModeConfig(SOC_ADC_TSC_0_REGS, TSCADC_FOUR_WIRE_MODE);

    TSCADCStepIDTagConfig(SOC_ADC_TSC_0_REGS, 1);

    /* Disable Write Protection of Step Configuration regs*/
    TSCADCStepConfigProtectionDisable(SOC_ADC_TSC_0_REGS);

    /* Configure ADC to Single ended operation mode */
    TSCADCIdleStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                    TSCADC_SINGLE_ENDED_OPER_MODE);

    /* Configure reference volatage and input to idlestep */
    TSCADCIdleStepConfig(SOC_ADC_TSC_0_REGS, TSCADC_NEGATIVE_REF_VSSA,
                         TSCADC_POSITIVE_INP_CHANNEL1, TSCADC_NEGATIVE_INP_ADCREFM,
                         TSCADC_POSITIVE_REF_VDDA);

    /* Configure the Analog Supply to Touch screen */
    TSCADCIdleStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                     TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF);

    /*
    **Configure the Analong Ground of Touch screen.
    */
    TSCADCIdleStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                     TSCADC_YPNSW_PIN_ON, TSCADC_YNNSW_PIN_ON,
                                     TSCADC_WPNSW_PIN_OFF);


    /* Configure ADC to Single ended operation mode */
    TSCADCChargeStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                         TSCADC_SINGLE_ENDED_OPER_MODE);

    /* Configure reference volatage and input to charge step*/
    TSCADCChargeStepConfig(SOC_ADC_TSC_0_REGS, TSCADC_NEGATIVE_REF_XNUR,
                           TSCADC_POSITIVE_INP_CHANNEL2, TSCADC_NEGATIVE_INP_CHANNEL2,
                           TSCADC_POSITIVE_REF_XPUL);

    /* Configure the Analog Supply to Touch screen */
    TSCADCChargeStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON,
                                       TSCADC_XNPSW_PIN_OFF, TSCADC_XPPSW_PIN_OFF);

    /* Configure the Analong Ground to Touch screen */
    TSCADCChargeStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                       TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_ON,
                                       TSCADC_WPNSW_PIN_OFF);

    TSCADCTSChargeStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 0x200);

    for(i = 0; i < SAMPLES; i++)
    {
         StepConfigX(i);

         TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 0x98);
    }

    for(i = SAMPLES; i < (2 * SAMPLES); i++)
    {
         StepConfigY(i);

         TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 0x98);
    }

    /* Configure FIFO1 threshold value */
    TSCADCFIFOIRQThresholdLevelConfig(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1, 5);

    IsTSPress = 0;
    touchRelease = 0;

	/* timer setup for touch screen */
	//IntPrioritySet(SYS_INT_TINT3, 0, AINTC_HOSTINT_ROUTE_IRQ);
    //IntSystemEnable(SYS_INT_TINT3);
    //IntRegister(SYS_INT_TINT3, Timer3Isr);

//    DMTimerModeConfigure(SOC_DMTIMER_3_REGS, DMTIMER_ONESHOT_CMP_ENABLE);
//    DMTimerReloadSet(SOC_DMTIMER_3_REGS, 0xffffffff);
//    DMTimerCompareSet(SOC_DMTIMER_3_REGS, 0xfffff);

//    DMTimerIntStatusClear(SOC_DMTIMER_3_REGS, DMTIMER_INT_MAT_EN_FLAG);

    /* Enable the DMTimer interrupts */
//    DMTimerIntEnable(SOC_DMTIMER_3_REGS, DMTIMER_INT_MAT_EN_FLAG);

	/*	Interrupt register	*/
	TouchIntEnable();
	/*	enable touch	*/
	TouchEnable();
}

/*
** Timer 3 ISR
*/
//static void Timer3Isr(void)
//{
    /* Clear the status of the interrupt flags */
//    DMTimerIntStatusClear(SOC_DMTIMER_3_REGS, DMTIMER_INT_MAT_EN_FLAG);

    /* Notify end of interrupt */
//    DMTimerEndOfInterrupt(SOC_DMTIMER_3_REGS);

//    DMTimerDisable(SOC_DMTIMER_3_REGS);
//    DMTimerCounterSet(SOC_DMTIMER_3_REGS, 0);

//    touchRelease = 1;
//}

/*
** This function resolves the coordinates of the location on the
** touch screen being touched.
*/
extern tDisplay* ScreenRander;

static int TouchCoOrdGet(LcdTouch_t* structure, int *pX, int *pY)
{
	unsigned int Cnt = 0;
	*pX = 0;
	*pY = 0;
	for(Cnt = 0; Cnt < analog_touch_filter_level;Cnt++)
	{
		*pX += to_percentage(structure->LcdTouch_L_Calibration_Value,structure->LcdTouch_R_Calibration_Value,structure->screen_max_x,x_data[Cnt & (analog_touch_filter_level - 1)]);
		*pY += to_percentage(structure->LcdTouch_U_Calibration_Value,structure->LcdTouch_D_Calibration_Value,structure->screen_max_y,y_data[Cnt & (analog_touch_filter_level - 1)]);
	}
    *pX /= analog_touch_filter_level;
    *pY /= analog_touch_filter_level;
    return 1;
}
/*
** This function tells if a touch is detected.
*/
bool TouchIdle(LcdTouch_t* structure)
{
#ifdef UseWatchdog
		WDR();
#endif
	if(!structure) return false;
	//TouchScreen_Data_t* TouchReport = &structure->TouchResponse;
	unsigned char State1 = Gfx_ft5x06_Touch_MouseNoAction;
	bool Return = false;
    signed int TouchX = -1, TouchY = -1;
    if(IsTSPress)
    {
    	if(ignore_cnt > 1)
    	{
			IsTSPress = 0;
			State1 = Gfx_ft5x06_MouseMove;
			TouchCoOrdGet(structure, &TouchX, &TouchY);
			structure->TouchResponse.x1 = TouchX;
			structure->TouchResponse.y1 = TouchY;
			Return = true;
    	}else
    	{
    		ignore_cnt++;
    		IsTSPress = 0;
    	}
    }else ignore_cnt = 0;
	if(structure->LastState1 == Gfx_ft5x06_Touch_MouseNoAction && State1 == Gfx_ft5x06_MouseMove)
	{
		structure->LastState1 = Gfx_ft5x06_MouseDn;
		structure->TouchResponse.touch_event1 = Gfx_ft5x06_MouseDn;
	}
	else if((structure->LastState1 == Gfx_ft5x06_MouseDn || structure->LastState1 == Gfx_ft5x06_MouseMove) && State1 == Gfx_ft5x06_MouseMove)
	{
		structure->LastState1 = Gfx_ft5x06_MouseMove;
		structure->TouchResponse.touch_event1 = Gfx_ft5x06_MouseMove;
	}
	else if((structure->LastState1 == Gfx_ft5x06_MouseDn || structure->LastState1 == Gfx_ft5x06_MouseMove) && State1 == Gfx_ft5x06_Touch_MouseNoAction)
	{
		structure->LastState1 = Gfx_ft5x06_MouseUp;
		structure->TouchResponse.touch_event1 = Gfx_ft5x06_MouseUp;
		ignored_touch = analog_touch_filter_level;
		dbidx = 0;
	}
	else if(structure->LastState1 == Gfx_ft5x06_MouseUp && State1 == Gfx_ft5x06_Touch_MouseNoAction)
	{
		structure->LastState1 = Gfx_ft5x06_Touch_MouseNoAction;
		structure->TouchResponse.touch_event1 = Gfx_ft5x06_Touch_MouseNoAction;
		ignored_touch = analog_touch_filter_level;
		dbidx = 0;
		structure->TouchResponse.x1 = -1;
		structure->TouchResponse.y1 = -1;
	}
	return Return;
}

/*     Function: TouchCalibrate()
 *
 *     Description: Ask the user to touch the predefined coordinates
 *                  and read the correspoding touch screen driver values.
 *                  Collect 3 sets of values and pass to the
 *                  setCalibrationMatrix() function.
 */
timer(TimerTouchCalibrate);

#define FilterTouchCalibrateSize	(32)

void TouchCalibrate(LcdTouch_t* structure, tDisplay *pDisplay)
{
    timer_interval(&TimerTouchCalibrate, 25);
	put_rectangle(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, ClrBlack);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	double TouchX = 0, TouchY = 0;
	double screen_max_x = structure->screen_max_x;
	double screen_max_y = structure->screen_max_y;
	structure->screen_max_x = 4095;
	structure->screen_max_y = 4095;
	structure->LcdTouch_L_Calibration_Value = 0;
	structure->LcdTouch_R_Calibration_Value = 4095;
	structure->LcdTouch_U_Calibration_Value = 0;
	structure->LcdTouch_D_Calibration_Value = 4095;
	double LcdTouch_L_Calibration_Value;
	double LcdTouch_R_Calibration_Value;
	double LcdTouch_U_Calibration_Value;
	double LcdTouch_D_Calibration_Value;

	TouchPaintPoint(pDisplay, (((double)pDisplay->raster_timings->X * (double)12.5) / (double)100), (((double)pDisplay->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	double Xbuffer[FilterTouchCalibrateSize];
	double Ybuffer[FilterTouchCalibrateSize];
	unsigned int Cnt = 0;
	unsigned int Cnt1 = 0;
	while(1)
	{
		if(timer_tick(&TimerTouchCalibrate))
		{
			TouchIdle(structure);
			if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseMove)
			{
				Xbuffer[Cnt] = structure->TouchResponse.x1;
				Ybuffer[Cnt] = structure->TouchResponse.y1;
				Cnt++;
			}
			else if(structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseUp)
			{
				for(Cnt1 = 1; Cnt1 < Cnt; Cnt1++)
				{
					Xbuffer[0] += Xbuffer[Cnt1];
					Ybuffer[0] += Ybuffer[Cnt1];
				}
				Xbuffer[0] /= Cnt;
				Ybuffer[0] /= Cnt;
				TouchX = Xbuffer[0];
				TouchY = Ybuffer[0];
				break;
			}
		}
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_L_Calibration_Value = (double)TouchX;
	LcdTouch_U_Calibration_Value = (double)TouchY;
	put_rectangle(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, ClrBlack);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	//delay(500);

	TouchPaintPoint(pDisplay, (double)pDisplay->raster_timings->X - (((double)pDisplay->raster_timings->X * (double)12.5) / (double)100), (((double)pDisplay->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	Cnt = 0;
	Cnt1 = 0;
	while(1)
	{
		if(timer_tick(&TimerTouchCalibrate))
		{
			TouchIdle(structure);
			if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseMove)
			{
				Xbuffer[Cnt] = structure->TouchResponse.x1;
				Ybuffer[Cnt] = structure->TouchResponse.y1;
				Cnt++;
			}
			else if(structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseUp)
			{
				for(Cnt1 = 1; Cnt1 < Cnt; Cnt1++)
				{
					Xbuffer[0] += Xbuffer[Cnt1];
					Ybuffer[0] += Ybuffer[Cnt1];
				}
				Xbuffer[0] /= Cnt;
				Ybuffer[0] /= Cnt;
				TouchX = Xbuffer[0];
				TouchY = Ybuffer[0];
				break;
			}
		}
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_R_Calibration_Value = (double)TouchX;
	LcdTouch_U_Calibration_Value += (double)TouchY;
	put_rectangle(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, ClrBlack);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	//delay(500);

	TouchPaintPoint(pDisplay, (double)pDisplay->raster_timings->X - (((double)pDisplay->raster_timings->X * (double)12.5) / (double)100), (double)pDisplay->raster_timings->Y - (((double)pDisplay->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	Cnt = 0;
	Cnt1 = 0;
	while(1)
	{
		if(timer_tick(&TimerTouchCalibrate))
		{
			TouchIdle(structure);
			if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseMove)
			{
				Xbuffer[Cnt] = structure->TouchResponse.x1;
				Ybuffer[Cnt] = structure->TouchResponse.y1;
				Cnt++;
			}
			else if(structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseUp)
			{
				for(Cnt1 = 1; Cnt1 < Cnt; Cnt1++)
				{
					Xbuffer[0] += Xbuffer[Cnt1];
					Ybuffer[0] += Ybuffer[Cnt1];
				}
				Xbuffer[0] /= Cnt;
				Ybuffer[0] /= Cnt;
				TouchX = Xbuffer[0];
				TouchY = Ybuffer[0];
				break;
			}
		}
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_R_Calibration_Value += (double)TouchX;
	LcdTouch_D_Calibration_Value = (double)TouchY;
	put_rectangle(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, ClrBlack);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	//delay(500);

	TouchPaintPoint(pDisplay,  (((double)pDisplay->raster_timings->X * (double)12.5) / (double)100), (double)pDisplay->raster_timings->Y - (((double)pDisplay->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	Cnt = 0;
	Cnt1 = 0;
	while(1)
	{
		if(timer_tick(&TimerTouchCalibrate))
		{
			TouchIdle(structure);
			if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseMove)
			{
				Xbuffer[Cnt] = structure->TouchResponse.x1;
				Ybuffer[Cnt] = structure->TouchResponse.y1;
				Cnt++;
			}
			else if(structure->TouchResponse.touch_event1 == Gfx_ft5x06_MouseUp)
			{
				for(Cnt1 = 1; Cnt1 < Cnt; Cnt1++)
				{
					Xbuffer[0] += Xbuffer[Cnt1];
					Ybuffer[0] += Ybuffer[Cnt1];
				}
				Xbuffer[0] /= Cnt;
				Ybuffer[0] /= Cnt;
				TouchX = Xbuffer[0];
				TouchY = Ybuffer[0];
				break;
			}
		}
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_L_Calibration_Value += (double)TouchX;
	LcdTouch_D_Calibration_Value += (double)TouchY;
	put_rectangle(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, ClrBlack);
	box_cache_clean(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);

	LcdTouch_L_Calibration_Value /= 2;
	LcdTouch_R_Calibration_Value /= 2;
	LcdTouch_U_Calibration_Value /= 2;
	LcdTouch_D_Calibration_Value /= 2;

	double X_middle = LcdTouch_L_Calibration_Value + ((LcdTouch_R_Calibration_Value - LcdTouch_L_Calibration_Value) / 2);
	LcdTouch_L_Calibration_Value = X_middle - ((X_middle - LcdTouch_L_Calibration_Value) * 1.33);
	LcdTouch_R_Calibration_Value = X_middle + ((LcdTouch_R_Calibration_Value - X_middle) * 1.33);
	double Y_middle = LcdTouch_U_Calibration_Value + ((LcdTouch_D_Calibration_Value - LcdTouch_U_Calibration_Value) / 2);
	LcdTouch_U_Calibration_Value = Y_middle - ((Y_middle - LcdTouch_U_Calibration_Value) * 1.33);
	LcdTouch_D_Calibration_Value = Y_middle + ((LcdTouch_D_Calibration_Value - Y_middle) * 1.33);

	structure->screen_max_x = screen_max_x;
	structure->screen_max_y = screen_max_y;
	structure->LcdTouch_L_Calibration_Value = LcdTouch_L_Calibration_Value;
	structure->LcdTouch_R_Calibration_Value = LcdTouch_R_Calibration_Value;
	structure->LcdTouch_U_Calibration_Value = LcdTouch_U_Calibration_Value;
	structure->LcdTouch_D_Calibration_Value = LcdTouch_D_Calibration_Value;
}


/* Enables Touch Screen */
void TouchEnable(void)
{
    /* The Sequencer will start in IDLE state */
    TSCADCModuleStateSet(SOC_ADC_TSC_0_REGS, TSCADC_MODULE_ENABLE);
    StepEnable();
}

/* Enables step */
void StepEnable(void)
{
    unsigned int i = 0;

    for(i = 0; i < 11; i++)
    {
         TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i, 1);
    }
}

/* Disables step */
void StepDisable(void)
{
    unsigned int i = 0;

    for(i = 0; i < 11; i++)
    {
         TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i, 0);
    }
}

static void StepConfigX(unsigned int stepSelc)
{
    /* Configure ADC to Single ended operation mode */
    TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                    TSCADC_SINGLE_ENDED_OPER_MODE, stepSelc);

    /* Configure reference volatage and input to charge step*/
    TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSelc,TSCADC_NEGATIVE_REF_VSSA,
                       TSCADC_POSITIVE_INP_CHANNEL3,TSCADC_NEGATIVE_INP_CHANNEL1,
                       TSCADC_POSITIVE_REF_VDDA);

    /* Configure the Analog Supply to Touch screen */
    TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON,
                                   TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF,
                                   stepSelc);

    /* Configure the Analong Ground to Touch screen */
    TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_ON,
                                   TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_OFF,
                                   TSCADC_WPNSW_PIN_OFF, stepSelc);

    /* select fifo 0 */
    TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_FIFO_0);

    /* Configure in One short hardware sync mode */
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_ONE_SHOT_HARDWARE_SYNC);

    TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG);
}

static void StepConfigY(unsigned int stepSelc)
{

    /* Configure ADC to Single ended operation mode */
    TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                     TSCADC_SINGLE_ENDED_OPER_MODE, stepSelc);

    /* Configure reference volatage and input to charge step*/
    TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_NEGATIVE_REF_VSSA,
                       TSCADC_POSITIVE_INP_CHANNEL1, TSCADC_NEGATIVE_INP_ADCREFM,
                       TSCADC_POSITIVE_REF_VDDA);

    /* Configure the Analog Supply to Touch screen */
    TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                   TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_ON, stepSelc);

    /* Configure the Analong Ground to Touch screen */
    TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                   TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_ON,
                                   TSCADC_WPNSW_PIN_OFF, stepSelc);

    /* select fifo 0 */
    TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_FIFO_1);

    /* Configure in One short hardware sync mode */
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_ONE_SHOT_HARDWARE_SYNC);

    TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG);
}
/*#####################################################*/

