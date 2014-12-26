/*
 *  lib/device/ms5611.c
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
#include <math.h>
#include "ms5611.h"
#include "board_init.h"
#include "../api/twi_api.h"
#include "../api/twi_def.h"
#include "../api/uart_api.h"
#include "../api/uart_def.h"
#include "sys/sysdelay.h"

bool ms5611_adc_get_cmd_send(Twi_t *TwiStruct, unsigned long *data, unsigned char osr)
{
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_ADC_READ_CMD;
	switch(osr)
	{
	case MS5611_CONVERT_OSR_256:
	case MS5611_CONVERT_OSR_512:
		Sysdelay(2);
		break;
	case MS5611_CONVERT_OSR_1024:
		Sysdelay(4);
		break;
	case MS5611_CONVERT_OSR_2048:
		Sysdelay(6);
		break;
	case MS5611_CONVERT_OSR_4096:
		Sysdelay(10);
		break;
	}
	SetupI2CReception(TwiStruct, 1, 3);
	if(TwiStruct->RxBuff[0] == 0 && TwiStruct->RxBuff[1] == 0 && TwiStruct->RxBuff[2] == 0) return false;
	*data = (TwiStruct->RxBuff[0] << 16) + (TwiStruct->RxBuff[1] << 8) + TwiStruct->RxBuff[2];
	return true;
}

bool ms5611_reset_cmd_send(Twi_t *TwiStruct)
{
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_RESET_CMD;
	if(!SetupI2CTransmit(TwiStruct, 1)) return false;
	return true;
}

bool ms5611_d1_cmd_send(Twi_t *TwiStruct, unsigned char osr, unsigned long *d1)
{
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_CONVERT_D1_OSR_256_CMD | ((osr & 0x07) << 1);
	if(!SetupI2CTransmit(TwiStruct, 1)) return false;
	if(!ms5611_adc_get_cmd_send(TwiStruct, d1, osr)) return false;
	return true;
}
bool ms5611_d2_cmd_send(Twi_t *TwiStruct, unsigned char osr, unsigned long *d2)
{
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_CONVERT_D2_OSR_256_CMD | ((osr & 0x07) << 1);
	if(!SetupI2CTransmit(TwiStruct, 1)) return false;
	if(!ms5611_adc_get_cmd_send(TwiStruct, d2, osr)) return false;
	return true;
}

bool ms5611_read_prom_cmd_send(ms5611_prom_data *prom_data, Twi_t *TwiStruct)
{
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C1_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C0 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C2_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C1 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C3_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C2 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C4_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C3 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C5_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C4 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C6_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C5 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C7_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C6 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C8_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	prom_data->C7 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];

	return true;
}

bool ms5611_read(ms5611_prom_data *prom_data, Twi_t *TwiStruct, unsigned char osr, signed int *Preasure, signed int *Temperature)
{
	unsigned long D1 = 0, D2 = 0;
	signed long dT = 0, TEMP = 0;
	signed long long OFF = 0, SENS = 0;

	if(!ms5611_d1_cmd_send(TwiStruct, osr, &D1)) return false;
	if(!ms5611_d2_cmd_send(TwiStruct, osr, &D2)) return false;
	unsigned int tmp = prom_data->C5 * ( 1<<8 );
	dT = D2 - tmp;
	TEMP = 2000 + dT * prom_data->C6 /(1 << 23);
	OFF = ((signed long long)prom_data->C2 * (signed long long)(1 << 16)) + (((signed long long)prom_data->C4 * (signed long long)dT) / (signed long long)( 1<<7 ));
	SENS = ((signed long long)prom_data->C1 * (signed long long)(1 << 15)) + (((signed long long)prom_data->C3 * (signed long long)dT) / (signed long long)( 1<<8 ));
	*Preasure = (signed int)((signed long long)(((((signed long long)D1 * (signed long long)SENS) / (signed long long)(1 << 21)) - (signed long long)OFF)) / (signed long long)(1 << 15));
	*Temperature = (signed int)TEMP;
	return true;
}

bool ms5611_display_preasure_result(ms5611_prom_data *prom_data, Twi_t *TwiStruct, unsigned char osr)
{
	signed int Preasure = 0;
	signed int Temperature = 0;
	if(!ms5611_read_prom_cmd_send(prom_data, TwiStruct)) return false;
	if(!ms5611_read(prom_data, TwiStruct, osr, &Preasure, &Temperature)) return false;
	float PreasureInt = 0;
	float PreasureDec = modff(((float)Preasure)/100.0, &PreasureInt);
	UARTprintf(DebugCom, "Preasure = %d.%u\n\r", (signed int)PreasureInt, (unsigned int)(PreasureDec * 100.0));
	return true;
}

bool ms5611_init(ms5611_prom_data *prom_data, Twi_t *TwiStruct)
{
	if(!ms5611_read_prom_cmd_send(prom_data, TwiStruct)) return false;
	return true;
}


