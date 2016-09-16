/*
 * microe_touch.c
 *
 *  Created on: Aug 11, 2016
 *      Author: John Smith
 */

#include <stdlib.h>

#include "microe_touch.h"

#include "api/adc_def.h"
#include "api/adc_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/touchscreen_def.h"
#include "lib/generic.h"
#include "lib/gfx/controls_definition.h"

bool mikroe_touch_init(LcdTouch_t *Touch, Adc_t *Adc, Gpio_t *DriveA, Gpio_t *DriveB, unsigned int AdcChannel_X, unsigned int AdcChannel_Y)
{
	Touch->UsrData = calloc(1, sizeof(MicroeTouch_t));
	if(!Touch->UsrData || !Adc || !DriveA || !DriveB)
		return false;
	MicroeTouch_t *MikroeTouch = Touch->UsrData;
	MikroeTouch->Adc = Adc;
	MikroeTouch->DriveA = DriveA;
	MikroeTouch->DriveB = DriveB;
	MikroeTouch->AdcChannel_X = AdcChannel_X;
	MikroeTouch->AdcChannel_Y = AdcChannel_Y;
	return true;
}

static int TouchCoOrdGet(LcdTouch_t* Touch, int *pX, int *pY)
{
	unsigned int Cnt = 0;
	*pX = 0;
	*pY = 0;
	MicroeTouch_t *MikroeTouch = Touch->UsrData;
	for(Cnt = 0; Cnt < ANALOG_TOUCH_FILTER_LEVEL;Cnt++)
	{
		*pX += to_percentage(Touch->LcdTouch_L_Calibration_Value,Touch->LcdTouch_R_Calibration_Value,Touch->screen_max_x,MikroeTouch->x_data[Cnt & (ANALOG_TOUCH_FILTER_LEVEL - 1)]);
		*pY += to_percentage(Touch->LcdTouch_U_Calibration_Value,Touch->LcdTouch_D_Calibration_Value,Touch->screen_max_y,MikroeTouch->y_data[Cnt & (ANALOG_TOUCH_FILTER_LEVEL - 1)]);
	}
    *pX /= ANALOG_TOUCH_FILTER_LEVEL;
    *pY /= ANALOG_TOUCH_FILTER_LEVEL;
    return 1;
}

bool microe_touch_service(LcdTouch_t* Touch)
{
	MicroeTouch_t *MikroeTouch = Touch->UsrData;
	if(!Touch)
		return false;
	if(MikroeTouch->touch_conv_finish == true)
		return true;
	adc_start_conversion(MikroeTouch->Adc);
	while(!MikroeTouch->Adc->EndOfConversion){};
	switch(MikroeTouch->xy_state)
	{
	case 0:
		MikroeTouch->y_data[MikroeTouch->dbidx] = MikroeTouch->Adc->ConvResult[MikroeTouch->AdcChannel_Y];
		break;
	case 1:
		MikroeTouch->x_data[MikroeTouch->dbidx] = MikroeTouch->Adc->ConvResult[MikroeTouch->AdcChannel_X];
		break;
	case 2:
		if(MikroeTouch->Adc->ConvResult[MikroeTouch->AdcChannel_Y] > 1000)
		{
			MikroeTouch->IsTSPress = true;
		}
		MikroeTouch->touch_conv_finish = true;
		MikroeTouch->dbidx = ANALOG_TOUCH_FILTER_LEVEL - 1;
		break;
	default:
		MikroeTouch->xy_state = 0;
		break;
	}
	if(MikroeTouch->dbidx == ANALOG_TOUCH_FILTER_LEVEL - 1)
	{
		MikroeTouch->xy_state++;
		if(MikroeTouch->xy_state >= 3)
			MikroeTouch->xy_state = 0;
		MikroeTouch->dbidx = 0;
		switch(MikroeTouch->xy_state)
		{
		case 0:
			gpio_out(MikroeTouch->DriveB, 1);
			gpio_out(MikroeTouch->DriveA, 0);
			break;
		case 1:
			gpio_out(MikroeTouch->DriveA, 1);
			gpio_out(MikroeTouch->DriveB, 0);
			break;
		case 2:
			gpio_out(MikroeTouch->DriveA, 0);
			gpio_out(MikroeTouch->DriveB, 0);
			break;
		default:
			MikroeTouch->xy_state = 0;
			break;
		}

	}
	else
		MikroeTouch->dbidx++;
	return MikroeTouch->touch_conv_finish;
}

/*
** This function tells if a touch is detected.
*/
bool microe_touch_idle(LcdTouch_t* Touch)
{
	if(!Touch)
		return false;
	MicroeTouch_t *MikroeTouch = Touch->UsrData;
#ifdef UseWatchdog
		WDR();
#endif

	bool Return = false;
	if(MikroeTouch->touch_conv_finish)
	{
		MikroeTouch->touch_conv_finish = false;
		unsigned char State1 = Cursor_NoAction;
		signed int TouchX = -1, TouchY = -1;
		if(MikroeTouch->IsTSPress)
		{
			if(MikroeTouch->ignore_cnt > 1)
			{
				MikroeTouch->IsTSPress = 0;
				State1 = Cursor_Move;
				TouchCoOrdGet(Touch, &TouchX, &TouchY);
				Touch->TouchResponse.x1 = TouchX;
				Touch->TouchResponse.y1 = TouchY;
				Return = true;
			}else
			{
				MikroeTouch->ignore_cnt++;
				MikroeTouch->IsTSPress = 0;
			}
		}else
			MikroeTouch->ignore_cnt = 0;
		if(Touch->LastState1 == Cursor_NoAction && State1 == Cursor_Move)
		{
			Touch->LastState1 = Cursor_Down;
			Touch->TouchResponse.touch_event1 = Cursor_Down;
		}
		else if((Touch->LastState1 == Cursor_Down || Touch->LastState1 == Cursor_Move) && State1 == Cursor_Move)
		{
			Touch->LastState1 = Cursor_Move;
			Touch->TouchResponse.touch_event1 = Cursor_Move;
		}
		else if((Touch->LastState1 == Cursor_Down || Touch->LastState1 == Cursor_Move) && State1 == Cursor_NoAction)
		{
			Touch->LastState1 = Cursor_Up;
			Touch->TouchResponse.touch_event1 = Cursor_Up;
			MikroeTouch->ignored_touch = ANALOG_TOUCH_FILTER_LEVEL;
			MikroeTouch->dbidx = 0;
		}
		else if(Touch->LastState1 == Cursor_Up && State1 == Cursor_NoAction)
		{
			Touch->LastState1 = Cursor_NoAction;
			Touch->TouchResponse.touch_event1 = Cursor_NoAction;
			MikroeTouch->ignored_touch = ANALOG_TOUCH_FILTER_LEVEL;
			MikroeTouch->dbidx = 0;
			Touch->TouchResponse.x1 = -1;
			Touch->TouchResponse.y1 = -1;
		}
	}
	return Return;
}


timer(TimerTouchCalibrate);

#define FilterTouchCalibrateSize	(32)

void microe_touch_calibrate(LcdTouch_t* structure, void *pDisplay)
{
	tDisplay* LcdStruct = (tDisplay *) pDisplay;
    timer_interval(&TimerTouchCalibrate, 25);
    LcdStruct->lcd_func.put_rectangle(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, true, ClrBlack);
    LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
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

	TouchPaintPoint(pDisplay, (((double)LcdStruct->raster_timings->X * (double)12.5) / (double)100), (((double)LcdStruct->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	double Xbuffer[FilterTouchCalibrateSize];
	double Ybuffer[FilterTouchCalibrateSize];
	unsigned int Cnt = 0;
	unsigned int Cnt1 = 0;
	while(1)
	{
		Sysdelay(2);
		if(microe_touch_service(structure))
		{
			if(timer_tick(&TimerTouchCalibrate))
			{
				microe_touch_idle(structure);
				if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Cursor_Move)
				{
					Xbuffer[Cnt] = structure->TouchResponse.x1;
					Ybuffer[Cnt] = structure->TouchResponse.y1;
					Cnt++;
				}
				else if(structure->TouchResponse.touch_event1 == Cursor_Up)
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
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_L_Calibration_Value = (double)TouchX;
	LcdTouch_U_Calibration_Value = (double)TouchY;
	LcdStruct->lcd_func.put_rectangle(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, true, ClrBlack);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	//delay(500);

	TouchPaintPoint(pDisplay, (double)LcdStruct->raster_timings->X - (((double)LcdStruct->raster_timings->X * (double)12.5) / (double)100), (((double)LcdStruct->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	Cnt = 0;
	Cnt1 = 0;
	while(1)
	{
		Sysdelay(2);
		if(microe_touch_service(structure))
		{
			if(timer_tick(&TimerTouchCalibrate))
			{
				microe_touch_idle(structure);
				if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Cursor_Move)
				{
					Xbuffer[Cnt] = structure->TouchResponse.x1;
					Ybuffer[Cnt] = structure->TouchResponse.y1;
					Cnt++;
				}
				else if(structure->TouchResponse.touch_event1 == Cursor_Up)
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
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_R_Calibration_Value = (double)TouchX;
	LcdTouch_U_Calibration_Value += (double)TouchY;
	LcdStruct->lcd_func.put_rectangle(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, true, ClrBlack);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	//delay(500);

	TouchPaintPoint(pDisplay, (double)LcdStruct->raster_timings->X - (((double)LcdStruct->raster_timings->X * (double)12.5) / (double)100), (double)LcdStruct->raster_timings->Y - (((double)LcdStruct->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	Cnt = 0;
	Cnt1 = 0;
	while(1)
	{
		Sysdelay(2);
		if(microe_touch_service(structure))
		{
			if(timer_tick(&TimerTouchCalibrate))
			{
				microe_touch_idle(structure);
				if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Cursor_Move)
				{
					Xbuffer[Cnt] = structure->TouchResponse.x1;
					Ybuffer[Cnt] = structure->TouchResponse.y1;
					Cnt++;
				}
				else if(structure->TouchResponse.touch_event1 == Cursor_Up)
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
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_R_Calibration_Value += (double)TouchX;
	LcdTouch_D_Calibration_Value = (double)TouchY;
	LcdStruct->lcd_func.put_rectangle(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, true, ClrBlack);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	//delay(500);

	TouchPaintPoint(pDisplay,  (((double)LcdStruct->raster_timings->X * (double)12.5) / (double)100), (double)LcdStruct->raster_timings->Y - (((double)LcdStruct->raster_timings->Y * (double)12.5) / (double)100), ClrRed);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
	//CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 32, pDisplay->Width * pDisplay->Height * 4);
	Cnt = 0;
	Cnt1 = 0;
	while(1)
	{
		Sysdelay(2);
		if(microe_touch_service(structure))
		{
			if(timer_tick(&TimerTouchCalibrate))
			{
				microe_touch_idle(structure);
				if(Cnt < FilterTouchCalibrateSize && structure->TouchResponse.touch_event1 == Cursor_Move)
				{
					Xbuffer[Cnt] = structure->TouchResponse.x1;
					Ybuffer[Cnt] = structure->TouchResponse.y1;
					Cnt++;
				}
				else if(structure->TouchResponse.touch_event1 == Cursor_Up)
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
	}
	//TouchX = structure->TouchResponse.x1;
	//TouchY = structure->TouchResponse.y1;
	LcdTouch_L_Calibration_Value += (double)TouchX;
	LcdTouch_D_Calibration_Value += (double)TouchY;
	LcdStruct->lcd_func.put_rectangle(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y, true, ClrBlack);
	LcdStruct->lcd_func.box_cache_clean(pDisplay, 0, 0, LcdStruct->raster_timings->X, LcdStruct->raster_timings->Y);
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




