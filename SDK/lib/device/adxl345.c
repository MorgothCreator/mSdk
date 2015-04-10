/*
 *  lib/device/ADXL345.c
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

#include "ADXL345.h"
#include "api/twi_api.h"
/*#####################################################*/
bool adxl345_bit_set(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char mask)
{
    new_twi* TwiStruct = adxl345_struct->TwiStruct;
    unsigned char* RxBuff = (unsigned char*)TwiStruct->RxBuff;
    unsigned char* TxBuff = (unsigned char*)TwiStruct->TxBuff;
	TwiStruct->MasterSlaveAddr = adxl345_struct->MasterSlaveAddr;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	TxBuff[0] = regOffset;
    if(!SetupI2CReception(TwiStruct, 1, 1))
    	return false;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	TxBuff[0] = regOffset;
	TxBuff[1] = RxBuff[0] | mask;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
    return true;
}
/*#####################################################*/
bool adxl345_bit_clr(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char mask)
{
    new_twi* TwiStruct = adxl345_struct->TwiStruct;
    unsigned char* RxBuff = (unsigned char*)TwiStruct->RxBuff;
    unsigned char* TxBuff = (unsigned char*)TwiStruct->TxBuff;
	TwiStruct->MasterSlaveAddr = adxl345_struct->MasterSlaveAddr;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	TxBuff[0] = regOffset;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	TxBuff[0] = regOffset;
	TxBuff[1] = RxBuff[0] & ~mask;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
    return true;
}
/*#####################################################*/
bool adxl345_write(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char data)
{
    new_twi* TwiStruct = adxl345_struct->TwiStruct;
    unsigned char* TxBuff = (unsigned char*)TwiStruct->TxBuff;
	TwiStruct->MasterSlaveAddr = adxl345_struct->MasterSlaveAddr;
	TxBuff[0] = regOffset;
	TxBuff[1] = data;
	TwiStruct->tCount = 0;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
    return true;
}
/*#####################################################*/
bool adxl345_write_multiple(new_adxl345* adxl345_struct, unsigned int regOffset, unsigned char *Buff, unsigned int DataLen)
{
    unsigned int i;
    new_twi* TwiStruct = adxl345_struct->TwiStruct;
    unsigned char* TxBuff = (unsigned char*)TwiStruct->TxBuff;
	TwiStruct->MasterSlaveAddr = adxl345_struct->MasterSlaveAddr;
	TxBuff[0] = regOffset;
	TwiStruct->tCount = 0;
    for (i = 0; i < DataLen; i++ ) {
    	TxBuff[i + 1] = Buff[i];
    }
    if(!SetupI2CTransmit(TwiStruct, DataLen + 1))
    	return false;
    return true;
}
/*#####################################################*/
bool adxl345_read(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char* data)
{
    new_twi* TwiStruct = adxl345_struct->TwiStruct;
    unsigned char* RxBuff = (unsigned char*)TwiStruct->RxBuff;
    unsigned char* TxBuff = (unsigned char*)TwiStruct->TxBuff;
	TwiStruct->MasterSlaveAddr = adxl345_struct->MasterSlaveAddr;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	TxBuff[0] = regOffset;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
    *data = RxBuff[0];
    return true;
}
/*#####################################################*/
bool adxl345_read_multiple(new_adxl345* adxl345_struct, unsigned int regOffset, unsigned char *Buff, unsigned int DataLen)
{
    unsigned int i;
    new_twi* TwiStruct = adxl345_struct->TwiStruct;
    unsigned char* RxBuff = (unsigned char*)TwiStruct->RxBuff;
    unsigned char* TxBuff = (unsigned char*)TwiStruct->TxBuff;
	TwiStruct->MasterSlaveAddr = adxl345_struct->MasterSlaveAddr;
	TwiStruct->tCount = 0;
	TwiStruct->rCount = 0;
	TxBuff[0] = regOffset;
	if(!SetupI2CReception(TwiStruct, 1, DataLen))
		return false;
    for (i = 0; i < DataLen; i++ ) {
    	Buff[i] = RxBuff[i];
    }
    return true;
}
/*#####################################################*/
bool adxl345_device_id_corect(new_adxl345* adxl345_struct)
{
	unsigned char data = 0;
	if(!adxl345_read(adxl345_struct, 0, &data))
		return false;
	if(data == 0xE5)
		return true;
	else
		return false;
}
/*#####################################################*/
/* ADXL345_REG_ACT_TAP_STATUS */
unsigned char adxl345_tap_status_read(new_adxl345* adxl345_struct)
{
	unsigned char Status = 0;
	adxl345_read(adxl345_struct, ADXL345_REG_ACT_TAP_STATUS, &Status);
	return Status;
}
/*#####################################################*/
/* ADXL345_REG_ACT_TAP_STATUS */
bool adxl345_x_read(new_adxl345* adxl345_struct, signed int *Data)
{
	unsigned short Status;
	if(!adxl345_read_multiple(adxl345_struct, ADXL345_REG_OUT_X_L_DATA, (unsigned char*)&Status, 0x2))
		return false;
	signed int XAxe = Status;
	if(XAxe & 0x8000) XAxe+= 0xFFFF0000;
	unsigned int CntMed = 0;
	adxl345_struct->OutputFilter.Buff_X[adxl345_struct->OutputFilter.Buff_X_Count] = XAxe;
	XAxe = 0;
	for(; CntMed < adxl345_struct->FilterBuffSize; CntMed++) {
		XAxe += adxl345_struct->OutputFilter.Buff_X[CntMed];
	}
	XAxe /= (signed int)adxl345_struct->FilterBuffSize;
	if(++adxl345_struct->OutputFilter.Buff_X_Count >= adxl345_struct->FilterBuffSize) adxl345_struct->OutputFilter.Buff_X_Count = 0;
	*Data = (signed int)XAxe * 4;
	return true;
}
/*#####################################################*/
/* ADXL345_REG_ACT_TAP_STATUS */
bool adxl345_y_read(new_adxl345* adxl345_struct, signed int *Data)
{
	unsigned short Status;
	if(!adxl345_read_multiple(adxl345_struct, ADXL345_REG_OUT_Y_L_DATA, (unsigned char*)&Status, 0x2))
		return false;
	signed int YAxe = Status;
	if(YAxe & 0x8000) YAxe+= 0xFFFF0000;
	unsigned int CntMed = 0;
	adxl345_struct->OutputFilter.Buff_Y[adxl345_struct->OutputFilter.Buff_Y_Count] = YAxe;
	YAxe = 0;
	for(; CntMed < adxl345_struct->FilterBuffSize; CntMed++) {
		YAxe += adxl345_struct->OutputFilter.Buff_Y[CntMed];
	}
	YAxe /= (signed int)adxl345_struct->FilterBuffSize;
	if(++adxl345_struct->OutputFilter.Buff_Y_Count >= adxl345_struct->FilterBuffSize) adxl345_struct->OutputFilter.Buff_Y_Count = 0;
	*Data = (signed int)YAxe * 4;
	return true;
}
/*#####################################################*/
/* ADXL345_REG_ACT_TAP_STATUS */
bool adxl345_z_read(new_adxl345* adxl345_struct, signed int *Data)
{
	unsigned short Status;
	if(!adxl345_read_multiple(adxl345_struct, ADXL345_REG_OUT_Z_L_DATA, (unsigned char*)&Status, 0x2))
		return false;
	signed int ZAxe = Status;
	if(ZAxe & 0x8000) ZAxe+= 0xFFFF0000;
	unsigned int CntMed = 0;
	adxl345_struct->OutputFilter.Buff_Z[adxl345_struct->OutputFilter.Buff_Z_Count] = ZAxe;
	ZAxe = 0;
	for(; CntMed < adxl345_struct->FilterBuffSize; CntMed++) {
		ZAxe += adxl345_struct->OutputFilter.Buff_Z[CntMed];
	}
	ZAxe /= (signed int)adxl345_struct->FilterBuffSize;
	if(++adxl345_struct->OutputFilter.Buff_Z_Count >= adxl345_struct->FilterBuffSize) adxl345_struct->OutputFilter.Buff_Z_Count = 0;
	*Data = (signed int)ZAxe * 4;
	return true;
}
/*#####################################################*/
/* ADXL345_REG_OFSX */
bool adxl345_x_offset_set(new_adxl345* adxl345_struct, signed char Offset)
{
	if(!adxl345_write(adxl345_struct, ADXL345_REG_OFSX, Offset / 3))
		return false;
	return true;
}
/*#####################################################*/
/* ADXL345_REG_OFSY */
bool adxl345_y_offset_set(new_adxl345* adxl345_struct, signed char Offset)
{
	if(!adxl345_write(adxl345_struct, ADXL345_REG_OFSY, Offset / 3))
		return false;
	return true;
}
/*#####################################################*/
/* ADXL345_REG_OFSZ */
bool adxl345_z_offset_set(new_adxl345* adxl345_struct, signed char Offset)
{
	if(!adxl345_write(adxl345_struct, ADXL345_REG_OFSZ, Offset / 3))
		return false;
	return true;
}
/*#####################################################*/
/* ADXL345_REG_POWER_CTL */
bool adxl345_power_ctl_set_wekup(new_adxl345* adxl345_struct, unsigned char Wekup)
{
	if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_POWER_CTL, Wekup))
		return false;
	if(!adxl345_bit_clr(adxl345_struct, ADXL345_REG_POWER_CTL, ~Wekup))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_power_ctl_set_link(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_AUTO_SLEEP))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_power_ctl_clear_link(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_clr(adxl345_struct, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_AUTO_SLEEP))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_power_ctl_set_autosleep(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_AUTO_SLEEP))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_power_ctl_clear_autosleep(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_clr(adxl345_struct, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_AUTO_SLEEP))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_power_ctl_set_sleep(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_Sleep))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_power_ctl_clear_sleep(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_clr(adxl345_struct, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_Sleep))
		return false;
	return true;
}
/*#####################################################*/
/* ADXL345_REG_DATA_FORMAT */
bool adxl345_data_format_set_full_resolution(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_DATA_FORMAT, ADXL345_DATA_FORMAT_FULL_RES))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_data_format_set_normal_resolution(new_adxl345* adxl345_struct)
{
	if(!adxl345_bit_clr(adxl345_struct, ADXL345_REG_DATA_FORMAT, ADXL345_DATA_FORMAT_FULL_RES))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_data_format_set(new_adxl345* adxl345_struct, unsigned int DataFormat)
{
	if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_DATA_FORMAT, DataFormat))
		return false;
	if(!adxl345_bit_clr(adxl345_struct, ADXL345_REG_DATA_FORMAT, ~DataFormat))
		return false;
	return true;
}
/*#####################################################*/
bool adxl345_bw_rate_output_rate_set(new_adxl345* adxl345_struct, unsigned char Rate)
{
	if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_BW_RATE, Rate))
		return false;
	if(!adxl345_bit_clr(adxl345_struct, ADXL345_REG_BW_RATE, ~Rate))
		return false;
	return true;
}
/*#####################################################*/
/* ADXL345 init */
bool adxl345_init(new_adxl345* adxl345_struct)
{
	if(!adxl345_struct)
		return false;
	if(adxl345_device_id_corect(adxl345_struct)) {
		adxl345_struct->OutputFilter.Buff_X = calloc(1, adxl345_struct->FilterBuffSize * sizeof(adxl345_struct->OutputFilter.Buff_X[0]));
		adxl345_struct->OutputFilter.Buff_Y = calloc(1, adxl345_struct->FilterBuffSize * sizeof(adxl345_struct->OutputFilter.Buff_Y[0]));
		adxl345_struct->OutputFilter.Buff_Z = calloc(1, adxl345_struct->FilterBuffSize * sizeof(adxl345_struct->OutputFilter.Buff_Z[0]));
		if(!adxl345_bit_set(adxl345_struct, ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_Measure))
			return false;
		if(!adxl345_data_format_set(adxl345_struct, ADXL345_DATA_FORMAT_Range_16g))
			return false;
		if(!adxl345_bw_rate_output_rate_set(adxl345_struct, ADXL345_BW_RATE_Rate_25Hz))
			return false;
		if(!adxl345_data_format_set_full_resolution(adxl345_struct))
			return false;
		if(!adxl345_x_offset_set(adxl345_struct, adxl345_struct->Calibration_X))
			return false;
		if(!adxl345_y_offset_set(adxl345_struct, adxl345_struct->Calibration_Y))
			return false;
		if(!adxl345_z_offset_set(adxl345_struct, adxl345_struct->Calibration_Z))
			return false;
		return true;
	} else {
		//if(adxl345_struct) free(adxl345_struct);
		return false;
	}
}
/*#####################################################*/
void adxl345_free(new_adxl345* adxl345_struct)
{
	if(adxl345_struct->OutputFilter.Buff_X) free(adxl345_struct->OutputFilter.Buff_X);
	if(adxl345_struct->OutputFilter.Buff_Y) free(adxl345_struct->OutputFilter.Buff_Y);
	if(adxl345_struct->OutputFilter.Buff_Z) free(adxl345_struct->OutputFilter.Buff_Z);
	if(adxl345_struct) free(adxl345_struct);
}
/*#####################################################*/



