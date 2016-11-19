/*
 *  lib/device/ft5x06.c
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
#include "../api/touchscreen_def.h"
#include "../device/ft5x06.h"
#include "../api/twi_api.h"
#include "../api/gpio_api.h"
#include "../lib/generic.h"

bool ft5x06_data_ready(new_touchscreen* structure)
{
	if(!structure)
		return false;
	if(!gpio.in(structure->IrqStruct))
		return false;
	else
		return true;
}

void ft5x06_free(new_touchscreen* structure)
{
	//if(structure->TouchResponse) free(structure->TouchResponse);
	if(structure->IrqStruct) free(structure->IrqStruct);
	if(structure) free(structure);
}

bool ft5x06_init(new_touchscreen* structure, unsigned char Port, unsigned char Pin)
{
	if(!structure)
		return false;
	structure->TouchScreen_Type = TouchScreen_Type_FT5x06;
    gpio.init(Port);
    structure->IrqStruct = gpio.assign(Port, Pin, GPIO_IN_FLOATING, false);
    if(ft5x06_data_ready(structure))
    {
		new_twi* twistruct = structure->TwiStruct;
		unsigned char reg = 0xF9;
		unsigned char result[38];
		if(twi.trx(twistruct, FT5X0X_TWI_ADDR2, &reg, 1, result, 38))
		{
			structure->twi_addr = FT5X0X_TWI_ADDR2;
			structure->touch_max_x = 1024;
			structure->touch_max_y = 600;
			//(double)structure->pDisplay->LcdTimings->X = (double)structure->pDisplay->LcdTimings->X;
			//(double)structure->pDisplay->LcdTimings->Y = (double)structure->pDisplay->LcdTimings->Y;
			return true;
		}
		twistruct->MasterSlaveAddr = FT5X0X_TWI_ADDR1;
		twistruct->rCount = 0;
		twistruct->tCount = 0;
		twistruct->TxBuff[0] = 0xF9;
		if(twi.trx(twistruct, FT5X0X_TWI_ADDR1, &reg, 1, result, 38))
		{
			structure->twi_addr = FT5X0X_TWI_ADDR1;
			structure->touch_max_x = 480;
			structure->touch_max_y = 800;
			//(double)structure->pDisplay->LcdTimings->X = (double)structure->pDisplay->LcdTimings->X;
			//(double)structure->pDisplay->LcdTimings->Y = (double)structure->pDisplay->LcdTimings->Y;
			return true;
		}
    }
    //ft5x06_free(structure);
    return false;
}

#define ft5x06_TouchIdle_offset 8

bool ft5x06_TouchIdle(new_touchscreen* structure)
{
	if(!structure)
		return false;
	if(!gpio.in(structure->IrqStruct))
		return false;
	new_twi* twistruct = structure->TwiStruct;
	unsigned char reg = 0xF9;
	unsigned char result[38];
	if(!twi.trx(twistruct, structure->twi_addr, &reg, 1, result, 38))
		return false;
	volatile unsigned char* Response = twistruct->RxBuff;
	//if(Response[0] != 0xAA || Response[1] != 0xAA) return 0;
	//TouchScreen_Data_t* structure->TouchResponse = &structure->TouchResponse;
	structure->TouchResponse.touch_point = Response[ft5x06_TouchIdle_offset + 1] & 0x0F;
	unsigned char State1 = Gfx_IntTouch_MouseNoAction;
	unsigned char State2 = Gfx_IntTouch_MouseNoAction;
	unsigned char State3 = Gfx_IntTouch_MouseNoAction;
	unsigned char State4 = Gfx_IntTouch_MouseNoAction;
	unsigned char State5 = Gfx_IntTouch_MouseNoAction;
	double X =(double)structure->pDisplay->LcdTimings->X;
	double Y =(double)structure->pDisplay->LcdTimings->Y;
	if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
	{
		X =(double)structure->pDisplay->LcdTimings->Y;
		Y =(double)structure->pDisplay->LcdTimings->X;
	}
	switch(structure->TouchResponse.touch_point)
	{
	case 5:
		structure->TouchResponse.x5 = (X - 1) - to_percentage(0,structure->touch_max_x,X,((Response[ft5x06_TouchIdle_offset + 26]<<8) | Response[ft5x06_TouchIdle_offset + 27]) & 0x0FFF);
		structure->TouchResponse.y5 = to_percentage(0,structure->touch_max_y,Y,((Response[ft5x06_TouchIdle_offset + 28]<<8) | Response[ft5x06_TouchIdle_offset + 29]) & 0x0FFF);
		structure->TouchResponse.touch_ID5=(unsigned short)(Response[ft5x06_TouchIdle_offset + 29] & 0xF0)>>4;
		//structure->TouchResponse->touch_event5 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 27] & 0xc0) >> 6);
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x5_Back = structure->TouchResponse.x5;
			structure->TouchResponse.x5 = structure->TouchResponse.y5;
			structure->TouchResponse.y5 = x5_Back;
		}
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			structure->TouchResponse.x5 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x5;
			structure->TouchResponse.y5 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y5;
		}
		if(!structure->flip_x)
			structure->TouchResponse.x5 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x5;
		if(structure->flip_y)
			structure->TouchResponse.y5 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y5;
		State5 = Gfx_IntTouch_MouseMove;
	case 4:
		structure->TouchResponse.x4 = (X - 1) - to_percentage(0,structure->touch_max_x,X,((Response[ft5x06_TouchIdle_offset + 20]<<8) | Response[ft5x06_TouchIdle_offset + 21]) & 0x0FFF);
		structure->TouchResponse.y4 = to_percentage(0,structure->touch_max_y,Y,((Response[ft5x06_TouchIdle_offset + 22]<<8) | Response[ft5x06_TouchIdle_offset + 23]) & 0x0FFF);
		structure->TouchResponse.touch_ID4=(unsigned short)(Response[ft5x06_TouchIdle_offset + 23] & 0xF0)>>4;
		//structure->TouchResponse->touch_event4 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 21] & 0xc0) >> 6);
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x4_Back = structure->TouchResponse.x4;
			structure->TouchResponse.x4 = structure->TouchResponse.y4;
			structure->TouchResponse.y4 = x4_Back;
		}
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			structure->TouchResponse.x4 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x4;
			structure->TouchResponse.y4 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y4;
		}
		if(!structure->flip_x)
			structure->TouchResponse.x4 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x4;
		if(structure->flip_y)
			structure->TouchResponse.y4 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y4;
		State4 = Gfx_IntTouch_MouseMove;
	case 3:
		structure->TouchResponse.x3 = (X - 1) - to_percentage(0,structure->touch_max_x,X,((Response[ft5x06_TouchIdle_offset + 14]<<8) | Response[ft5x06_TouchIdle_offset + 15]) & 0x0FFF);
		structure->TouchResponse.y3 = to_percentage(0,structure->touch_max_y,Y,((Response[ft5x06_TouchIdle_offset + 16]<<8) | Response[ft5x06_TouchIdle_offset + 17]) & 0x0FFF);
		structure->TouchResponse.touch_ID3=(unsigned short)(Response[ft5x06_TouchIdle_offset + 17] & 0xF0)>>4;
		//structure->TouchResponse->touch_event3 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 15] & 0xc0) >> 6);
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x3_Back = structure->TouchResponse.x3;
			structure->TouchResponse.x3 = structure->TouchResponse.y3;
			structure->TouchResponse.y3 = x3_Back;
		}
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			structure->TouchResponse.x3 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x3;
			structure->TouchResponse.y3 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y3;
		}
		if(!structure->flip_x)
			structure->TouchResponse.x3 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x3;
		if(structure->flip_y)
			structure->TouchResponse.y3 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y3;
		State3 = Gfx_IntTouch_MouseMove;
	case 2:
		structure->TouchResponse.x2 = (X - 1) - to_percentage(0,structure->touch_max_x,X,((Response[ft5x06_TouchIdle_offset + 8]<<8) | Response[ft5x06_TouchIdle_offset + 9]) & 0x0FFF);
		structure->TouchResponse.y2 = to_percentage(0,structure->touch_max_y,Y,((Response[ft5x06_TouchIdle_offset + 10]<<8) | Response[ft5x06_TouchIdle_offset + 11]) & 0x0FFF);
		structure->TouchResponse.touch_ID2=(unsigned short)(Response[ft5x06_TouchIdle_offset + 11] & 0xF0)>>4;
		//structure->TouchResponse->touch_event2 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 9] & 0xc0) >> 6);
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			signed int	x2_Back = structure->TouchResponse.x2;
			structure->TouchResponse.x2 = structure->TouchResponse.y2;
			structure->TouchResponse.y2 = x2_Back;
		}
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			structure->TouchResponse.x2 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x2;
			structure->TouchResponse.y2 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y2;
		}
		if(!structure->flip_x)
			structure->TouchResponse.x2 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x2;
		if(structure->flip_y)
			structure->TouchResponse.y2 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y2;
		State2 = Gfx_IntTouch_MouseMove;
	case 1:
		structure->TouchResponse.x1 = (X - 1) - to_percentage(0,structure->touch_max_x,X,((Response[ft5x06_TouchIdle_offset + 2]<<8) | Response[ft5x06_TouchIdle_offset + 3]) & 0x0FFF);
		structure->TouchResponse.y1 = to_percentage(0,structure->touch_max_y,Y,((Response[ft5x06_TouchIdle_offset + 4]<<8) | Response[ft5x06_TouchIdle_offset + 5]) & 0x0FFF);
		structure->TouchResponse.touch_ID1=(unsigned short)(Response[ft5x06_TouchIdle_offset + 5] & 0xF0)>>4;
		//structure->TouchResponse->touch_event1 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 3] & 0xc0) >> 6);
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			structure->TouchResponse.y1 = (double)structure->pDisplay->LcdTimings->X - structure->TouchResponse.y1;
			signed int	x1_Back = structure->TouchResponse.x1;
			structure->TouchResponse.x1 = structure->TouchResponse.y1;
			structure->TouchResponse.y1 = x1_Back;
		}
		if(structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_PORTRAIT_FLIP || structure->pDisplay->LcdTimings->orientation == LCD_ORIENTATION_LANDSCAPE_FLIP)
		{
			structure->TouchResponse.x1 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x1;
			structure->TouchResponse.y1 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y1;
		}
		if(!structure->flip_x)
			structure->TouchResponse.x1 = structure->pDisplay->LcdTimings->X - structure->TouchResponse.x1;
		if(structure->flip_y)
			structure->TouchResponse.y1 = structure->pDisplay->LcdTimings->Y - structure->TouchResponse.y1;
		State1 = Gfx_IntTouch_MouseMove;
	}

	//if(timer_tick(&structure->timer1) && structure->LastState1 == Gfx_MouseMove || structure->LastState1 == Gfx_MouseDn) structure->TouchResponse->touch_event1 = Gfx_MouseUp;

	if(structure->LastState1 == Gfx_IntTouch_MouseNoAction && State1 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState1 = Gfx_IntTouch_MouseDn;
		structure->TouchResponse.touch_event1 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState1 == Gfx_IntTouch_MouseDn || structure->LastState1 == Gfx_IntTouch_MouseMove) && State1 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState1 = Gfx_IntTouch_MouseMove;
		structure->TouchResponse.touch_event1 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState1 == Gfx_IntTouch_MouseDn || structure->LastState1 == Gfx_IntTouch_MouseMove) && State1 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState1 = Gfx_IntTouch_MouseUp;
		structure->TouchResponse.touch_event1 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState1 == Gfx_IntTouch_MouseUp && State1 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState1 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.touch_event1 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.x1 = -1;
		structure->TouchResponse.y1 = -1;
	}

	if(structure->LastState2 == Gfx_IntTouch_MouseNoAction && State2 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState2 = Gfx_IntTouch_MouseDn;
		structure->TouchResponse.touch_event2 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState2 == Gfx_IntTouch_MouseDn || structure->LastState2 == Gfx_IntTouch_MouseMove) && State2 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState2 = Gfx_IntTouch_MouseMove;
		structure->TouchResponse.touch_event2 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState2 == Gfx_IntTouch_MouseDn || structure->LastState2 == Gfx_IntTouch_MouseMove) && State2 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState2 = Gfx_IntTouch_MouseUp;
		structure->TouchResponse.touch_event2 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState2 == Gfx_IntTouch_MouseUp && State2 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState2 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.touch_event2 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.x2 = -1;
		structure->TouchResponse.y2 = -1;
	}

	if(structure->LastState3 == Gfx_IntTouch_MouseNoAction && State3 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState3 = Gfx_IntTouch_MouseDn;
		structure->TouchResponse.touch_event3 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState3 == Gfx_IntTouch_MouseDn || structure->LastState3 == Gfx_IntTouch_MouseMove) && State3 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState3 = Gfx_IntTouch_MouseMove;
		structure->TouchResponse.touch_event3 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState3 == Gfx_IntTouch_MouseDn || structure->LastState3 == Gfx_IntTouch_MouseMove) && State3 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState3 = Gfx_IntTouch_MouseUp;
		structure->TouchResponse.touch_event3 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState3 == Gfx_IntTouch_MouseUp && State3 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState3 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.touch_event3 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.x3 = -1;
		structure->TouchResponse.y3 = -1;
	}

	if(structure->LastState4 == Gfx_IntTouch_MouseNoAction && State4 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState4 = Gfx_IntTouch_MouseDn;
		structure->TouchResponse.touch_event4 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState4 == Gfx_IntTouch_MouseDn || structure->LastState4 == Gfx_IntTouch_MouseMove) && State4 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState4 = Gfx_IntTouch_MouseMove;
		structure->TouchResponse.touch_event4 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState4 == Gfx_IntTouch_MouseDn || structure->LastState4 == Gfx_IntTouch_MouseMove) && State4 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState4 = Gfx_IntTouch_MouseUp;
		structure->TouchResponse.touch_event4 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState4 == Gfx_IntTouch_MouseUp && State4 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState4 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.touch_event4 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.x4 = -1;
		structure->TouchResponse.y4 = -1;
	}

	if(structure->LastState5 == Gfx_IntTouch_MouseNoAction && State5 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState5 = Gfx_IntTouch_MouseDn;
		structure->TouchResponse.touch_event5 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState5 == Gfx_IntTouch_MouseDn || structure->LastState5 == Gfx_IntTouch_MouseMove) && State5 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState5 = Gfx_IntTouch_MouseMove;
		structure->TouchResponse.touch_event5 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState5 == Gfx_IntTouch_MouseDn || structure->LastState5 == Gfx_IntTouch_MouseMove) && State5 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState5 = Gfx_IntTouch_MouseUp;
		structure->TouchResponse.touch_event5 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState5 == Gfx_IntTouch_MouseUp && State5 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState5 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.touch_event5 = Gfx_IntTouch_MouseNoAction;
		structure->TouchResponse.x5 = -1;
		structure->TouchResponse.y5 = -1;
	}

	return true;
}
