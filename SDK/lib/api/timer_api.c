/*
 *  lib/api/timer_api.c
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

#include <stdbool.h>
#include <stdint.h>
#include "sys/sysdelay.h"
#include "timer_api.h"
/*#####################################################*/
extern volatile unsigned long long STimerCnt;
//#####################################################
void timer_init()
{
	SysDelayTimerSetup();
}
//#####################################################
void timer_interval(STimer_t *S_Timer_Struct,unsigned long long _Value)
{
#ifdef USE_RTC_100_MS
	S_Timer_Struct->S_Timmer_Interval = (_Value & (unsigned long long)INT64_MAX) / 10;
#else
	S_Timer_Struct->S_Timmer_Interval = _Value & (unsigned long long)INT64_MAX;
#endif
}
//#####################################################
void timer_en(STimer_t *S_Timer_Struct,unsigned char Action)
{
	if (Action == 1)
	{
		S_Timer_Struct->S_Timmer_TickValue = (S_Timer_Struct->S_Timmer_Interval + STimerCnt)&(unsigned long long)INT64_MAX;
	}
	else
	{
		S_Timer_Struct->S_Timmer_TickValue = S_Timer_Struct->S_Timmer_TickValue | (unsigned long long)INT64_MIN;
	}
}
//#####################################################
void timer_enable(STimer_t *S_Timer_Struct)
{
	S_Timer_Struct->S_Timmer_TickValue = (S_Timer_Struct->S_Timmer_Interval + STimerCnt)&(unsigned long long)INT64_MAX;
}
void timer_disable(STimer_t *S_Timer_Struct)
{
	S_Timer_Struct->S_Timmer_TickValue = S_Timer_Struct->S_Timmer_TickValue | (unsigned long long)INT64_MIN;
}
//#####################################################
bool timer_tick(STimer_t *S_Timer_Struct)
{
	if(S_Timer_Struct->S_Timmer_TickValue & (unsigned long long)INT64_MIN);
	else
	{
		unsigned long long Temp1 = S_Timer_Struct->S_Timmer_TickValue & (unsigned long long)INT64_MAX;
		unsigned long long Temp2 = STimerCnt & (unsigned long long)INT64_MAX;
		unsigned long long Temp = (Temp1 - Temp2 - 1)& (unsigned long long)INT64_MIN;
		if(Temp)
		{
			timer_en(S_Timer_Struct,1);
			return true;
		}
	}
	return false;
}
//#####################################################
bool timer_enabled(STimer_t *S_Timer_Struct)
{
	if(S_Timer_Struct->S_Timmer_TickValue & (unsigned long long)INT64_MIN) return false;
	else return true;
}
//#####################################################
