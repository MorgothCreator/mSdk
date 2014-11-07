/*
 *  lib/device/ds1307.h
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

#ifndef __DS1307_H__
#define __DS1307_H__
//#####################################################
#include <stdbool.h>
#include "../api/twi_def.h"
//#####################################################
#define DS1307_Rtc_DeviceAddr					(0x68)
#define DS1307_Sram_DeviceAddr					(0x68)
//-----------------------------------------------------
#define DS1307_Address_Length					(1)
//-----------------------------------------------------
#define DS1307_Sram_Page_Size					(0x00)
#define DS1307_Sram_Start						(0x08)
#define DS1307_Sram_Size						(0x38)
#define DS1307_Sram_End							(0x3F)
//#####################################################
typedef enum DS1307_Enum
{
	//RTCC MEMORY MAP
	DS1307_RTC_SecReg	= 0x00,
	DS1307_RTC_MinReg,
	DS1307_RTC_HourReg,
	DS1307_RTC_DayReg,
	DS1307_RTC_DateReg,
	DS1307_RTC_MonthReg,
	DS1307_RTC_YearReg,
	DS1307_RTC_Control
}DS1307_t;
//#####################################################
//RTCC MEMORY MAP Bit description
//-----------------------------------------------------
//SECONDS
typedef enum DS1307_Seconds_Enum
{
	DS1307_Seconds_gp	= 0,
	DS1307_10Seconds_gp = 4,
	DS1307_Ch_bp = 7,
	DS1307_Seconds_gm = 0x0F<<0,
	DS1307_10Seconds_gm = 0x07<<4,
	DS1307_Ch_bm = 0x01<<7
}DS1307_Seconds_t;
//-----------------------------------------------------
//MINUTES
typedef enum DS1307_Minutes_Enum
{
	DS1307_Minutes_gp	= 0,
	DS1307_10Minutes_gp = 4,
	DS1307_Minutes_gm	= 0x0F<<0,
	DS1307_10Minutes_gm = 0x07<<4
}DS1307_Minutes_t;
//-----------------------------------------------------
//HOURS
typedef enum DS1307_Hours_Enum
{
	DS1307_Hours_gp = 0,
	DS1307_10Hours_gp	= 4,
	DS1307_12_24_bp = 6,
	DS1307_Hours_gm = 0x0F<<0,
	DS1307_10Hours_gm	= 0x03<<4,
	DS1307_12_24_bm = 0x01<<6
}DS1307_Hours_t;
//-----------------------------------------------------
//DAYS
typedef enum DS1307_Days_Enum
{
	DS1307_Day_gp = 0,
	DS1307_Day_gm = 0x07<<0
}DS1307_Days_t;
//-----------------------------------------------------
//DATES
typedef enum DS1307_Dates_Enum
{
	DS1307_Date_gp = 0,
	DS1307_10Date_gp	= 4,
	DS1307_Date_gm = 0x0F<<0,
	DS1307_10Date_gm	= 0x03<<4
}DS1307_Dates_t;
//-----------------------------------------------------
//MONTHS
typedef enum DS1307_Months_Enum
{
	DS1307_Month_gp = 0,
	DS1307_10Month_bp	= 4,
	DS1307_Month_gm = 0x0F<<0,
	DS1307_10Month_bm	= 0x01<<4
}DS1307_Months_t;
//-----------------------------------------------------
//YEARS
typedef enum DS1307_Years_Enum
{
	DS1307_Year_gp = 0,
	DS1307_10Year_gp	= 4,
	DS1307_Year_gm = 0x0F<<0,
	DS1307_10Year_gm	= 0x0F<<4
}DS1307_Years_t;
//-----------------------------------------------------
//CONTROL
typedef enum DS1307_Control_Enum
{
	DS1307_RS_1H = 0,
	DS1307_RS_4096H,
	DS1307_RS_8192H,
	DS1307_RS_32768H,
	DS1307_RS_gp = 0,
	DS1307_RS_gm = 3,
	DS1307_SQWE_bp = 4,
	DS1307_SQWE_bm = 0x10,
	DS1307_OUT_bp = 7,
	DS1307_OUT_bm = 0x80
}DS1307_Control_t;
//#####################################################
bool DS1307_Setup(new_twi* TwiStruct);
int DS1307_Read_Reg(new_twi* TwiStruct, unsigned char Address);
bool DS1307_Write_Reg(new_twi* TwiStruct, unsigned char Address, unsigned char Data);
int DS1307_Read_Ram(new_twi* TwiStruct, unsigned char Address);
bool DS1307_Write_Ram(new_twi* TwiStruct, unsigned char Address, unsigned char Data);
bool DS1307_Set12Hours(new_twi* TwiStruct);
bool DS1307_Set24Hours(new_twi* TwiStruct);
bool DS1307_ClrOut(new_twi* TwiStruct);
bool DS1307_SetOut(new_twi* TwiStruct);
bool DS1307_ClrSqwe(new_twi* TwiStruct);
bool DS1307_SetSqwe(new_twi* TwiStruct);
bool DS1307_SetRs(new_twi* TwiStruct, unsigned char Rs);
bool DS1307_ReadTime_Str(new_twi* TwiStruct, char* Buff);
void DS1307_Write_RTC(new_twi* TwiStruct, unsigned char Reg, unsigned char Data);
unsigned char DS1307_Read_RTC(new_twi* TwiStruct, unsigned char Reg);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ds1307.c"
#endif
//#####################################################
#endif
