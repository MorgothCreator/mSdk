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
	if(!structure) return false;
	if(!gpio_in(structure->IrqStruct)) return false;
	else return true;
}

void ft5x06_free(new_touchscreen* structure)
{
	//if(structure->TouchResponse) free(structure->TouchResponse);
	if(structure->IrqStruct) free(structure->IrqStruct);
	if(structure) free(structure);
}

bool ft5x06_init(new_touchscreen* structure, unsigned char Port, unsigned char Pin)
{
	if(!structure) return false;
	structure->TouchScreen_Type = TouchScreen_Type_FT5x06;
	structure->touch_max_x = 1024;
	structure->touch_max_y = 600;
    gpio_init(0);
    structure->IrqStruct = gpio_assign(Port, Pin, GPIO_DIR_INPUT);
    if(ft5x06_data_ready(structure))
    {
		new_twi* twistruct = structure->TwiStruct;
		twistruct->MasterSlaveAddr = 0x38;
		twistruct->rCount = 0;
		twistruct->tCount = 0;
		twistruct->TxBuff[0] = 0xF9;
		if(SetupI2CReception(twistruct, 1, 38)) return true;
    }
    ft5x06_free(structure);
    return false;
}

#define ft5x06_TouchIdle_offset 8

bool ft5x06_TouchIdle(new_touchscreen* structure)
{
	if(!structure) return false;
	if(!gpio_in(structure->IrqStruct)) return false;
	new_twi* twistruct = structure->TwiStruct;
	twistruct->MasterSlaveAddr = 0x38;
	twistruct->rCount = 0;
	twistruct->tCount = 0;
	twistruct->TxBuff[0] = 0xF9;
	SetupI2CReception(twistruct, 1, 38);
	volatile unsigned char* Response = twistruct->RxBuff;
	//if(Response[0] != 0xAA || Response[1] != 0xAA) return 0;
	TouchScreen_Data_t* TouchReport = &structure->TouchResponse;
	TouchReport->touch_point = Response[ft5x06_TouchIdle_offset + 1] & 0x0F;
	unsigned char State1 = Gfx_IntTouch_MouseNoAction;
	unsigned char State2 = Gfx_IntTouch_MouseNoAction;
	unsigned char State3 = Gfx_IntTouch_MouseNoAction;
	unsigned char State4 = Gfx_IntTouch_MouseNoAction;
	unsigned char State5 = Gfx_IntTouch_MouseNoAction;
	switch(TouchReport->touch_point)
	{
	case 5:
		TouchReport->x5 = (structure->screen_max_x - 1) - to_percentage(0,structure->touch_max_x,structure->screen_max_x,((Response[ft5x06_TouchIdle_offset + 26]<<8) | Response[ft5x06_TouchIdle_offset + 27]) & 0x0FFF);
		TouchReport->y5 = to_percentage(0,structure->touch_max_y,structure->screen_max_y,((Response[ft5x06_TouchIdle_offset + 28]<<8) | Response[ft5x06_TouchIdle_offset + 29]) & 0x0FFF);
		TouchReport->touch_ID5=(unsigned short)(Response[ft5x06_TouchIdle_offset + 29] & 0xF0)>>4;
		//TouchReport->touch_event5 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 27] & 0xc0) >> 6);
		State5 = Gfx_IntTouch_MouseMove;
	case 4:
		TouchReport->x4 = (structure->screen_max_x - 1) - to_percentage(0,structure->touch_max_x,structure->screen_max_x,((Response[ft5x06_TouchIdle_offset + 20]<<8) | Response[ft5x06_TouchIdle_offset + 21]) & 0x0FFF);
		TouchReport->y4 = to_percentage(0,structure->touch_max_y,structure->screen_max_y,((Response[ft5x06_TouchIdle_offset + 22]<<8) | Response[ft5x06_TouchIdle_offset + 23]) & 0x0FFF);
		TouchReport->touch_ID4=(unsigned short)(Response[ft5x06_TouchIdle_offset + 23] & 0xF0)>>4;
		//TouchReport->touch_event4 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 21] & 0xc0) >> 6);
		State4 = Gfx_IntTouch_MouseMove;
	case 3:
		TouchReport->x3 = (structure->screen_max_x - 1) - to_percentage(0,structure->touch_max_x,structure->screen_max_x,((Response[ft5x06_TouchIdle_offset + 14]<<8) | Response[ft5x06_TouchIdle_offset + 15]) & 0x0FFF);
		TouchReport->y3 = to_percentage(0,structure->touch_max_y,structure->screen_max_y,((Response[ft5x06_TouchIdle_offset + 16]<<8) | Response[ft5x06_TouchIdle_offset + 17]) & 0x0FFF);
		TouchReport->touch_ID3=(unsigned short)(Response[ft5x06_TouchIdle_offset + 17] & 0xF0)>>4;
		//TouchReport->touch_event3 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 15] & 0xc0) >> 6);
		State3 = Gfx_IntTouch_MouseMove;
	case 2:
		TouchReport->x2 = (structure->screen_max_x - 1) - to_percentage(0,structure->touch_max_x,structure->screen_max_x,((Response[ft5x06_TouchIdle_offset + 8]<<8) | Response[ft5x06_TouchIdle_offset + 9]) & 0x0FFF);
		TouchReport->y2 = to_percentage(0,structure->touch_max_y,structure->screen_max_y,((Response[ft5x06_TouchIdle_offset + 10]<<8) | Response[ft5x06_TouchIdle_offset + 11]) & 0x0FFF);
		TouchReport->touch_ID2=(unsigned short)(Response[ft5x06_TouchIdle_offset + 11] & 0xF0)>>4;
		//TouchReport->touch_event2 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 9] & 0xc0) >> 6);
		State2 = Gfx_IntTouch_MouseMove;
	case 1:
		TouchReport->x1 = (structure->screen_max_x - 1) - to_percentage(0,structure->touch_max_x,structure->screen_max_x,((Response[ft5x06_TouchIdle_offset + 2]<<8) | Response[ft5x06_TouchIdle_offset + 3]) & 0x0FFF);
		TouchReport->y1 = to_percentage(0,structure->touch_max_y,structure->screen_max_y,((Response[ft5x06_TouchIdle_offset + 4]<<8) | Response[ft5x06_TouchIdle_offset + 5]) & 0x0FFF);
		TouchReport->touch_ID1=(unsigned short)(Response[ft5x06_TouchIdle_offset + 5] & 0xF0)>>4;
		//TouchReport->touch_event1 = (unsigned short)((Response[ft5x06_TouchIdle_offset + 3] & 0xc0) >> 6);
		State1 = Gfx_IntTouch_MouseMove;
	}

	//if(timer_tick(&structure->timer1) && structure->LastState1 == Gfx_MouseMove || structure->LastState1 == Gfx_MouseDn) TouchReport->touch_event1 = Gfx_MouseUp;

	if(structure->LastState1 == Gfx_IntTouch_MouseNoAction && State1 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState1 = Gfx_IntTouch_MouseDn;
		TouchReport->touch_event1 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState1 == Gfx_IntTouch_MouseDn || structure->LastState1 == Gfx_IntTouch_MouseMove) && State1 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState1 = Gfx_IntTouch_MouseMove;
		TouchReport->touch_event1 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState1 == Gfx_IntTouch_MouseDn || structure->LastState1 == Gfx_IntTouch_MouseMove) && State1 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState1 = Gfx_IntTouch_MouseUp;
		TouchReport->touch_event1 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState1 == Gfx_IntTouch_MouseUp && State1 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState1 = Gfx_IntTouch_MouseNoAction;
		TouchReport->touch_event1 = Gfx_IntTouch_MouseNoAction;
		TouchReport->x1 = -1;
		TouchReport->y1 = -1;
	}

	if(structure->LastState2 == Gfx_IntTouch_MouseNoAction && State2 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState2 = Gfx_IntTouch_MouseDn;
		TouchReport->touch_event2 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState2 == Gfx_IntTouch_MouseDn || structure->LastState2 == Gfx_IntTouch_MouseMove) && State2 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState2 = Gfx_IntTouch_MouseMove;
		TouchReport->touch_event2 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState2 == Gfx_IntTouch_MouseDn || structure->LastState2 == Gfx_IntTouch_MouseMove) && State2 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState2 = Gfx_IntTouch_MouseUp;
		TouchReport->touch_event2 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState2 == Gfx_IntTouch_MouseUp && State2 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState2 = Gfx_IntTouch_MouseNoAction;
		TouchReport->touch_event2 = Gfx_IntTouch_MouseNoAction;
		TouchReport->x2 = -1;
		TouchReport->y2 = -1;
	}

	if(structure->LastState3 == Gfx_IntTouch_MouseNoAction && State3 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState3 = Gfx_IntTouch_MouseDn;
		TouchReport->touch_event3 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState3 == Gfx_IntTouch_MouseDn || structure->LastState3 == Gfx_IntTouch_MouseMove) && State3 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState3 = Gfx_IntTouch_MouseMove;
		TouchReport->touch_event3 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState3 == Gfx_IntTouch_MouseDn || structure->LastState3 == Gfx_IntTouch_MouseMove) && State3 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState3 = Gfx_IntTouch_MouseUp;
		TouchReport->touch_event3 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState3 == Gfx_IntTouch_MouseUp && State3 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState3 = Gfx_IntTouch_MouseNoAction;
		TouchReport->touch_event3 = Gfx_IntTouch_MouseNoAction;
		TouchReport->x3 = -1;
		TouchReport->y3 = -1;
	}

	if(structure->LastState4 == Gfx_IntTouch_MouseNoAction && State4 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState4 = Gfx_IntTouch_MouseDn;
		TouchReport->touch_event4 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState4 == Gfx_IntTouch_MouseDn || structure->LastState4 == Gfx_IntTouch_MouseMove) && State4 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState4 = Gfx_IntTouch_MouseMove;
		TouchReport->touch_event4 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState4 == Gfx_IntTouch_MouseDn || structure->LastState4 == Gfx_IntTouch_MouseMove) && State4 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState4 = Gfx_IntTouch_MouseUp;
		TouchReport->touch_event4 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState4 == Gfx_IntTouch_MouseUp && State4 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState4 = Gfx_IntTouch_MouseNoAction;
		TouchReport->touch_event4 = Gfx_IntTouch_MouseNoAction;
		TouchReport->x4 = -1;
		TouchReport->y4 = -1;
	}

	if(structure->LastState5 == Gfx_IntTouch_MouseNoAction && State5 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState5 = Gfx_IntTouch_MouseDn;
		TouchReport->touch_event5 = Gfx_IntTouch_MouseDn;
	}
	else if((structure->LastState5 == Gfx_IntTouch_MouseDn || structure->LastState5 == Gfx_IntTouch_MouseMove) && State5 == Gfx_IntTouch_MouseMove)
	{
		structure->LastState5 = Gfx_IntTouch_MouseMove;
		TouchReport->touch_event5 = Gfx_IntTouch_MouseMove;
	}
	else if((structure->LastState5 == Gfx_IntTouch_MouseDn || structure->LastState5 == Gfx_IntTouch_MouseMove) && State5 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState5 = Gfx_IntTouch_MouseUp;
		TouchReport->touch_event5 = Gfx_IntTouch_MouseUp;
	}
	else if(structure->LastState5 == Gfx_IntTouch_MouseUp && State5 == Gfx_IntTouch_MouseNoAction)
	{
		structure->LastState5 = Gfx_IntTouch_MouseNoAction;
		TouchReport->touch_event5 = Gfx_IntTouch_MouseNoAction;
		TouchReport->x5 = -1;
		TouchReport->y5 = -1;
	}

	return true;
}
