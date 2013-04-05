/*
 *  lib/api/timer_api.h
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

#ifndef SYS_TIMER_H_
#define SYS_TIMER_H_
/*#####################################################*/
#include <stdbool.h>
#include "sys/sysdelay.h"
/*#####################################################*/
typedef struct STimer_Struct
{
	unsigned long long S_Timmer_Interval;				//Here, user will store the address of buffer used to receive data
	unsigned long long S_Timmer_TickValue;				//Here, user will store the address of buffer used to receive data
}STimer_t;
/*#####################################################*/
void timer_init();
void timer_interval(STimer_t *S_Timer_Struct,unsigned long long _Value);
void timer_en(STimer_t *S_Timer_Struct,unsigned char Action);
void timer_enable(STimer_t *S_Timer_Struct);
void timer_disable(STimer_t *S_Timer_Struct);
bool timer_tick(STimer_t *S_Timer_Struct);
bool timer_enabled(STimer_t *S_Timer_Struct);
/*#####################################################*/
#define timer(Name)  STimer_t Name
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "timer_api.c"
#endif
/*#####################################################*/
#endif /* SYS_TIMER_H_ */
