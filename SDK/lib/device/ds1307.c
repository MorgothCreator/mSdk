/*
 *  lib/device/ds1307.c
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
#include <stdlib.h>
#include "ds1307.h"
#include "sys/plat_properties.h"
#include "../api/twi_api.h"
#include "../api/twi_def.h"
//#####################################################
bool DS1307_Setup(new_twi* TwiStruct)
{
	TwiStruct->MasterSlaveAddr = DS1307_Rtc_DeviceAddr;
	TwiStruct->TxBuff[0] = DS1307_RTC_SecReg;
	if(!SetupI2CReception(TwiStruct, DS1307_Address_Length, 1))
		return false;
	else {
		if((TwiStruct->RxBuff[0] & DS1307_Ch_bm) != 0) {
			unsigned char *RxBuff = TwiStruct->RxBuff;
			unsigned char *TxBuff = TwiStruct->TxBuff;
			*TxBuff++ = DS1307_RTC_SecReg;
			*TxBuff++ = *RxBuff++ & !DS1307_Ch_bm;
			*TxBuff++ = *RxBuff++;
			*TxBuff++ = *RxBuff | DS1307_12_24_bm;
			if(!SetupI2CTransmit(TwiStruct, DS1307_Address_Length + 4))
				return false;
		}
	}
	return true;
}
//#####################################################
int DS1307_Read_Reg(new_twi* TwiStruct, unsigned char Address)
{
	TwiStruct->MasterSlaveAddr = DS1307_Rtc_DeviceAddr;
	*TwiStruct->TxBuff = Address;
	if(SetupI2CReception(TwiStruct, DS1307_Address_Length, 1))
	{
		return (int)*TwiStruct->RxBuff;
	}
	return -1;
}
//-----------------------------------------------------
bool DS1307_Write_Reg(new_twi* TwiStruct, unsigned char Address, unsigned char Data)
{
	unsigned char *TxBuff = TwiStruct->TxBuff;
	*TxBuff++ = Address;
	*TxBuff = Data;
	TwiStruct->MasterSlaveAddr = DS1307_Rtc_DeviceAddr;
	return SetupI2CTransmit(TwiStruct, DS1307_Address_Length + 1);
}
//#####################################################
int DS1307_Read_Ram(new_twi* TwiStruct, unsigned char Address)
{
	if(Address >= DS1307_Sram_Size) return -1;
	TwiStruct->MasterSlaveAddr = DS1307_Rtc_DeviceAddr;
	*TwiStruct->TxBuff = DS1307_Sram_Start + Address;
	if(SetupI2CReception(TwiStruct, DS1307_Address_Length, 1))
	{
		return (int)*TwiStruct->RxBuff;
	}
	return -1;
}
//-----------------------------------------------------
bool DS1307_Write_Ram(new_twi* TwiStruct, unsigned char Address, unsigned char Data)
{
	if(Address >= DS1307_Sram_Size) return false;
	unsigned char *TxBuff = TwiStruct->TxBuff;
	*TxBuff++ = DS1307_Sram_Start + Address;
	*TxBuff = Data;
	TwiStruct->MasterSlaveAddr = DS1307_Rtc_DeviceAddr;
	return SetupI2CTransmit(TwiStruct, DS1307_Address_Length + 1);
}
//#####################################################
bool DS1307_Set12Hours(new_twi* TwiStruct)
{
	return DS1307_Write_Reg(TwiStruct, DS1307_RTC_HourReg, DS1307_Read_Reg(TwiStruct, DS1307_RTC_HourReg) & !DS1307_12_24_bm);
}
//#####################################################
bool DS1307_Set24Hours(new_twi* TwiStruct)
{
	return DS1307_Write_Reg(TwiStruct, DS1307_RTC_HourReg, DS1307_Read_Reg(TwiStruct, DS1307_RTC_HourReg) | DS1307_12_24_bm);
}
//#####################################################
bool DS1307_ClrOut(new_twi* TwiStruct)
{
	return DS1307_Write_Reg(TwiStruct, DS1307_RTC_Control, DS1307_Read_Reg(TwiStruct, DS1307_RTC_Control) & !DS1307_OUT_bm);
}
//#####################################################
bool DS1307_SetOut(new_twi* TwiStruct)
{
	return DS1307_Write_Reg(TwiStruct, DS1307_RTC_Control, DS1307_Read_Reg(TwiStruct, DS1307_RTC_Control) | DS1307_OUT_bm);
}
//#####################################################
bool DS1307_ClrSqwe(new_twi* TwiStruct)
{
	return DS1307_Write_Reg(TwiStruct, DS1307_RTC_Control, DS1307_Read_Reg(TwiStruct, DS1307_RTC_Control) & !DS1307_SQWE_bm);
}
//#####################################################
bool DS1307_SetSqwe(new_twi* TwiStruct)
{
	return DS1307_Write_Reg(TwiStruct, DS1307_RTC_Control, DS1307_Read_Reg(TwiStruct, DS1307_RTC_Control) | DS1307_SQWE_bm);
}
//#####################################################
bool DS1307_SetRs(new_twi* TwiStruct, unsigned char Rs)
{
	return DS1307_Write_Reg(TwiStruct, DS1307_RTC_Control, (DS1307_Read_Reg(TwiStruct, DS1307_RTC_Control) & !DS1307_RS_gm) | (Rs & DS1307_RS_gm));
}
//#####################################################
//#####################################################
//#####################################################
bool DS1307_ReadTime_Str(new_twi* TwiStruct, char* Buff)
{
	TwiStruct->MasterSlaveAddr = DS1307_Rtc_DeviceAddr;
	*TwiStruct->TxBuff = DS1307_RTC_SecReg;
	if(!SetupI2CReception(TwiStruct, DS1307_Address_Length, 7))
		return false;
	else {
#if defined(_AVR_IOXXX_H_)
		unsigned char *ReadBuffer = TwiStruct->RxBuff;
		utoa((unsigned short)((ReadBuffer[6] & DS1307_10Year_gm) >> DS1307_10Year_gp), (char *)Buff, 10);
		utoa((unsigned short)((ReadBuffer[6] & DS1307_Year_gm) >> DS1307_Year_gp), (char *)Buff+1, 10);
		Buff[2] = '/';
		utoa((unsigned short)((ReadBuffer[5] & DS1307_10Month_bm) >> DS1307_10Month_bp), (char *)Buff+ 3, 10);
		utoa((unsigned short)((ReadBuffer[5] & DS1307_Month_gm) >> DS1307_Month_gp), (char *)Buff+ 4, 10);
		Buff[5] = '/';
		utoa((unsigned short)((ReadBuffer[4] & DS1307_10Date_gm) >> DS1307_10Date_gp), (char *)Buff+ 6, 10);
		utoa((unsigned short)((ReadBuffer[4] & DS1307_Date_gm) >> DS1307_Date_gp), (char *)Buff+ 7, 10);
		Buff[8] = '-';
		utoa((unsigned short)((ReadBuffer[2] & DS1307_10Hours_gm) >> DS1307_10Hours_gp), (char *)Buff+ 9, 10);
		utoa((unsigned short)((ReadBuffer[2] & DS1307_Hours_gm) >> DS1307_Hours_gp), (char *)Buff+ 10, 10);
		Buff[11] = ':';
		utoa((unsigned short)((ReadBuffer[1] & DS1307_10Minutes_gm) >> DS1307_10Minutes_gp), (char *)Buff+ 12, 10);
		utoa((unsigned short)((ReadBuffer[1] & DS1307_Minutes_gm) >> DS1307_Minutes_gp), (char *)Buff+ 13, 10);
		Buff[14] = ':';
		utoa((unsigned short)((ReadBuffer[0] & DS1307_10Seconds_gm) >> DS1307_10Seconds_gp), (char *)Buff+ 15, 10);
		utoa((unsigned short)((ReadBuffer[0] & DS1307_Seconds_gm) >> DS1307_Seconds_gp), (char *)Buff+ 16, 10);
#else
	#ifndef NO_I_TO_A_IN_STD_LIB
		unsigned char *ReadBuffer = TwiStruct->RxBuff;
		ltoa((unsigned short)((ReadBuffer[6] & DS1307_10Year_gm) >> DS1307_10Year_gp), (char *)Buff);
		ltoa((unsigned short)((ReadBuffer[6] & DS1307_Year_gm) >> DS1307_Year_gp), (char *)Buff+1);
		Buff[2] = '/';
		ltoa((unsigned short)((ReadBuffer[5] & DS1307_10Month_bm) >> DS1307_10Month_bp), (char *)Buff+ 3);
		ltoa((unsigned short)((ReadBuffer[5] & DS1307_Month_gm) >> DS1307_Month_gp), (char *)Buff+ 4);
		Buff[5] = '/';
		ltoa((unsigned short)((ReadBuffer[4] & DS1307_10Date_gm) >> DS1307_10Date_gp), (char *)Buff+ 6);
		ltoa((unsigned short)((ReadBuffer[4] & DS1307_Date_gm) >> DS1307_Date_gp), (char *)Buff+ 7);
		Buff[8] = '-';
		ltoa((unsigned short)((ReadBuffer[2] & DS1307_10Hours_gm) >> DS1307_10Hours_gp), (char *)Buff+ 9);
		ltoa((unsigned short)((ReadBuffer[2] & DS1307_Hours_gm) >> DS1307_Hours_gp), (char *)Buff+ 10);
		Buff[11] = ':';
		ltoa((unsigned short)((ReadBuffer[1] & DS1307_10Minutes_gm) >> DS1307_10Minutes_gp), (char *)Buff+ 12);
		ltoa((unsigned short)((ReadBuffer[1] & DS1307_Minutes_gm) >> DS1307_Minutes_gp), (char *)Buff+ 13);
		Buff[14] = ':';
		ltoa((unsigned short)((ReadBuffer[0] & DS1307_10Seconds_gm) >> DS1307_10Seconds_gp), (char *)Buff+ 15);
		ltoa((unsigned short)((ReadBuffer[0] & DS1307_Seconds_gm) >> DS1307_Seconds_gp), (char *)Buff+ 16);
	#endif
#endif
		Buff[17] = 0;
	}
	return true;
}
//#####################################################
unsigned char DS1307_Byte_To_Bcd(unsigned char Byte)
{
	unsigned char Tmp = 0;
	while(Byte > 9) Tmp++;
	return ((Tmp<<4) | Byte);
}
//#####################################################
unsigned char DS1307_Bcd_To_Byte(unsigned char Bcd)
{
	return ((((Bcd>>4) & 0x0F) * 10) | (Bcd & 0x0F));
}
//#####################################################
void DS1307_Write_RTC(new_twi* TwiStruct, unsigned char Reg, unsigned char Data)
{
	unsigned char TmpReg = DS1307_Read_Reg(TwiStruct, Reg);
	switch (Reg)
	{
		case DS1307_RTC_SecReg:
			TmpReg = (TmpReg & ~(DS1307_Seconds_gm | DS1307_10Seconds_gm)) | (DS1307_Byte_To_Bcd(Data) & (DS1307_Seconds_gm | DS1307_10Seconds_gm));
			break;
		case DS1307_RTC_MinReg:
			TmpReg = (TmpReg & ~(DS1307_Minutes_gm | DS1307_10Minutes_gm)) | (DS1307_Byte_To_Bcd(Data) & (DS1307_Minutes_gm | DS1307_10Minutes_gm));
			break;
		case DS1307_RTC_HourReg:
			TmpReg = (TmpReg & ~(DS1307_Hours_gm | DS1307_10Hours_gm)) | (DS1307_Byte_To_Bcd(Data) & (DS1307_Hours_gm | DS1307_10Hours_gm));
			break;
		case DS1307_RTC_DateReg:
			TmpReg = (TmpReg & ~(DS1307_Date_gm | DS1307_10Date_gm)) | (DS1307_Byte_To_Bcd(Data) & (DS1307_Date_gm | DS1307_10Date_gm));
			break;
		case DS1307_RTC_MonthReg:
			TmpReg = (TmpReg & ~(DS1307_Month_gm | DS1307_10Month_bm)) | (DS1307_Byte_To_Bcd(Data) & (DS1307_Month_gm | DS1307_10Month_bm));
			break;
		case DS1307_RTC_YearReg:
			TmpReg = (TmpReg & ~(DS1307_Year_gm | DS1307_10Year_gm)) | (DS1307_Byte_To_Bcd(Data) & (DS1307_Year_gm | DS1307_10Year_gm));
			break;
	}
	DS1307_Write_Reg(TwiStruct, Reg, TmpReg);
}
//#####################################################
unsigned char DS1307_Read_RTC(new_twi* TwiStruct, unsigned char Reg)
{
	unsigned char TmpReg = DS1307_Read_Reg(TwiStruct, Reg);
	switch (Reg)
	{
		case DS1307_RTC_SecReg:
			return DS1307_Bcd_To_Byte(TmpReg) & (DS1307_Seconds_gm | DS1307_10Seconds_gm);
		case DS1307_RTC_MinReg:
			return DS1307_Bcd_To_Byte(TmpReg) & (DS1307_Minutes_gm | DS1307_10Minutes_gm);
		case DS1307_RTC_HourReg:
			return DS1307_Bcd_To_Byte(TmpReg) & (DS1307_Hours_gm | DS1307_10Hours_gm);
		case DS1307_RTC_DateReg:
			return DS1307_Bcd_To_Byte(TmpReg) & (DS1307_Date_gm | DS1307_10Date_gm);
		case DS1307_RTC_MonthReg:
			return DS1307_Bcd_To_Byte(TmpReg) & (DS1307_Month_gm | DS1307_10Month_bm);
		case DS1307_RTC_YearReg:
			return DS1307_Bcd_To_Byte(TmpReg) & (DS1307_Year_gm | DS1307_10Year_gm);
	}
	return 0;
}
//#####################################################
