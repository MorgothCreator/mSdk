/*
 *  lib/device/ar1020.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "ar1020.h"
#include "lib/gfx/controls_definition.h"
#include "lib/generic.h"
#include "api/touchscreen_def.h"
#include "api/timer_api.h"
#include "api/gpio_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"

bool ar1020_data_ready(new_touchscreen* structure)
{
	if(!structure)
		return false;
	if(!gpio.in(structure->IrqStruct))
		return false;
	else
		return true;
}

void ar1020_free(new_touchscreen* structure)
{
	//if(structure->TouchResponse) free(structure->TouchResponse);
	if(structure->IrqStruct) free(structure->IrqStruct);
	if(structure) free(structure);
}

bool ar1020_init(new_touchscreen* structure, unsigned char Port, unsigned char Pin)
{
	if(!structure)
		return false;
	structure->TouchScreen_Type = TouchScreen_Type_AR1020;
	structure->IrqStruct = gpio.assign(Port, Pin, GPIO_IN_FLOATING, false);
	if(!structure->IrqStruct)
		return false;
	if(ar1020_enable_touch(structure)) 
	{
		ar1020_disable_touch(structure);
		sys_delay(50);
		ar1020_pen_up_delay_set(structure);
		ar1020_enable_touch(structure);
		return true;
	}		
	ar1020_free(structure);
	return false;
}

//#####################################################
//#####################################################
bool ar1020_comand_send(new_touchscreen* structure, unsigned char command)
{
	unsigned char tmp[4];
	unsigned char result[4];
	tmp[0] = 0x0;
	tmp[1] = 0x55;
	tmp[2] = 0x01;
	tmp[3] = command;
	unsigned int TimeoutCnt = 10;
	bool ResponseOK = false;
	do
	{
		sys_delay(50);
		if(twi.trx(structure->TwiStruct, AR1020_TWI_DeviceAddr, tmp, 4, result, 4))
		{
			unsigned char Buff0 = result[0];
			unsigned char Buff1 = result[1];
			unsigned char Buff2 = result[2];
			unsigned char Buff3 = result[3];
			if(Buff0 == 0x55 && Buff1 == 0x02 && Buff2 == AR1020_Response_Success && (Buff3 == command)) ResponseOK = true;
		}
	} while (TimeoutCnt-- != 0 && ResponseOK == false);
	if(TimeoutCnt) return ResponseOK;

	return false;
}
//#####################################################
bool ar1020_enable_touch(new_touchscreen* structure)
{
	return ar1020_comand_send(structure, 0x12);
}
//#####################################################
bool ar1020_disable_touch(new_touchscreen* structure)
{
	return ar1020_comand_send(structure, 0x13);
}
//#####################################################
unsigned char ar1020_register_offset_read(new_touchscreen* structure)
{
	new_twi* twistruct = structure->TwiStruct;
	unsigned char tmp[4];
	unsigned char result[5];
	tmp[0] = 0x0;
	tmp[1] = 0x55;
	tmp[2] = 0x01;
	tmp[3] = 0x22;
	twi.trx(twistruct, AR1020_TWI_DeviceAddr, tmp, 4, result, 5);
	return result[4];
}
//#####################################################
bool ar1020_pen_up_delay_set(new_touchscreen* structure)
{
	new_twi* twistruct = structure->TwiStruct;
	unsigned char tmp[5];
	tmp[0] = 0x0;
	tmp[1] = 0x55;
	tmp[2] = 0x02;
	tmp[3] = 0x0B + ar1020_register_offset_read(structure);
	tmp[4] = 0x20;
	return twi.tx(twistruct, AR1020_TWI_DeviceAddr, tmp, 5);
}
//#####################################################
unsigned char ar1020_touch(new_touchscreen* structure)
{
	sys_delay(50);
	new_twi* twistruct = structure->TwiStruct;
	unsigned char result[4];
	if(twi.trx(twistruct, AR1020_TWI_DeviceAddr, NULL, 0, result, 4) == false) return false;
	if(result[0] != 0x55 || result[1] != 0x02 || result[2] != AR1020_Response_Success || result[3] != 0x14)
		return false;
	return true;
}
//#####################################################
void ar1020_read_coordonate(new_touchscreen* structure)
{
	structure->TouchResponse.touch_event1 = Cursor_NoAction;
	structure->TouchResponse.x1 = -1;
	structure->TouchResponse.y1 = -1;
	if(ar1020_data_ready(structure))
	{
		unsigned char result[5];
		if(twi.trx(structure->TwiStruct, AR1020_TWI_DeviceAddr, NULL, 0, result, 5))
		{
			//unsigned char* ReadBuffer = twistruct->RxBuff;
			unsigned char Tmp = result[0] & 0x01;
			signed int X = to_percentage(0, 4095, (signed int)(double)structure->pDisplay->LcdTimings->X, ((unsigned short)result[2]<<7) | (unsigned short)result[1]);
			signed int Y = to_percentage(0, 4095, (signed int)(double)structure->pDisplay->LcdTimings->Y, ((unsigned short)result[4]<<7) | (unsigned short)result[3]);
			switch (structure->pDisplay->LcdTimings->orientation)
			{
			case LCD_ORIENTATION_LANDSCAPE:
			case LCD_ORIENTATION_LANDSCAPE_FLIP:
				structure->TouchResponse.x1 = Y;
				structure->TouchResponse.y1 = X;
				break;
			case LCD_ORIENTATION_PORTRAIT:
			case LCD_ORIENTATION_PORTRAIT_FLIP:
				structure->TouchResponse.x1 = X;
				structure->TouchResponse.y1 = Y;
				break;
			default:
				structure->TouchResponse.x1 = X;
				structure->TouchResponse.y1 = Y;
				break;
			}
			if(Tmp == 1 && structure->LastState1 == 1) 
			{
				structure->TouchResponse.touch_event1 = Cursor_Move;
			}
			else if(Tmp == 1 && structure->LastState1 == 0) 
			{
				structure->TouchResponse.touch_event1 = Cursor_Down;
			}
			else if(Tmp == 0 && structure->LastState1 == 1) 
			{
				structure->TouchResponse.touch_event1 = Cursor_Up;
			}
			structure->LastState1 = Tmp;

		}
		//else AR1020_Struct->Pen = False;
	}
	//else AR1020_Struct->Pen = False;
}
//#####################################################
static bool ar1020_calibrate(new_touchscreen* structure, tDisplay *pDisplay)
{
	new_twi* twistruct = structure->TwiStruct;
	unsigned char tmp[5];
	unsigned char result[4];
	tmp[0] = 0x0;
	tmp[1] = 0x55;
	tmp[2] = 0x02;
	tmp[3] = 0x14;
	tmp[4] = 0x04;
	if(!twi.trx(twistruct, AR1020_TWI_DeviceAddr, tmp, 5, result, 4))
		return false;
	if(result[0] != 0x55 && result[0] != 0x02 && result[0] != AR1020_Response_Success && result[0] != 0x14)
		return false;
	
	TouchPaintPoint(pDisplay, (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), 0x0000);
	while(ar1020_touch(structure) == false);
	TouchPaintPoint(pDisplay, (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), controls_color.Scren);

	TouchPaintPoint(pDisplay, (double)pDisplay->LcdTimings->X - (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), 0x0000);
	while(ar1020_touch(structure) == false);
	TouchPaintPoint(pDisplay, (double)pDisplay->LcdTimings->X - (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), controls_color.Scren);

	TouchPaintPoint(pDisplay, (double)pDisplay->LcdTimings->X - (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (double)pDisplay->LcdTimings->Y - (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), 0x0000);
	while(ar1020_touch(structure) == false);
	TouchPaintPoint(pDisplay, (double)pDisplay->LcdTimings->X - (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (double)pDisplay->LcdTimings->Y - (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), controls_color.Scren);

	TouchPaintPoint(pDisplay,  (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (double)pDisplay->LcdTimings->Y - (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), 0x0000);
	while(ar1020_touch(structure) == false);
	TouchPaintPoint(pDisplay,  (((double)pDisplay->LcdTimings->X * (double)12.5) / (double)100), (double)pDisplay->LcdTimings->Y - (((double)pDisplay->LcdTimings->Y * (double)12.5) / (double)100), controls_color.Scren);
	return true;
}
//#####################################################
bool ar1020_calibration_start(new_touchscreen* structure, tDisplay *pDisplay)
{
	if(!ar1020_disable_touch(structure))
		return false;
	sys_delay(50);
	if(!ar1020_calibrate(structure, pDisplay))
		return false;
	if(!ar1020_enable_touch(structure))
		return false;
	return true;
}

