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

#include "stdbool.h"
#include "math.h"
#include "ms5611.h"
#include "board_init.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/timer_api.h"
#include "sys/sysdelay.h"

bool ms5611_adc_get_cmd_send(MS5611_t *structure, unsigned long *data, unsigned char osr)
{
	timer_interval(&structure->Timeout_Timer, 15);
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_ADC_READ_CMD;
	while(!SetupI2CReception(TwiStruct, 1, 3)) {
		if(timer_tick(&structure->Timeout_Timer))
			return false;
	}
	if(TwiStruct->RxBuff[0] == 0 && TwiStruct->RxBuff[1] == 0 && TwiStruct->RxBuff[2] == 0)
		return false;
	*data = (TwiStruct->RxBuff[0] << 16) + (TwiStruct->RxBuff[1] << 8) + TwiStruct->RxBuff[2];
	return true;
}

bool ms5611_reset_cmd_send(MS5611_t *structure)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_RESET_CMD;
	if(!SetupI2CTransmit(TwiStruct, 1))
		return false;
	return true;
}

bool ms5611_d1_cmd_send(MS5611_t *structure, unsigned char osr, unsigned long *d1)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_CONVERT_D1_OSR_256_CMD | ((osr & 0x07) << 1);
	if(!SetupI2CTransmit(TwiStruct, 1))
		return false;
	if(!ms5611_adc_get_cmd_send(structure, d1, osr))
		return false;
	return true;
}
bool ms5611_d2_cmd_send(MS5611_t *structure, unsigned char osr, unsigned long *d2)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_CONVERT_D2_OSR_256_CMD | ((osr & 0x07) << 1);
	if(!SetupI2CTransmit(TwiStruct, 1))
		return false;
	if(!ms5611_adc_get_cmd_send(structure, d2, osr))
		return false;
	return true;
}

bool ms5611_read_prom_cmd_send(MS5611_t *structure)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MS5611_ADDR;
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C1_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C0 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C2_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C1 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C3_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C2 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C4_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C3 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C5_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C4 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C6_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C5 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C7_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C6 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	TwiStruct->TxBuff[0] = MS5611_CONVERT_PROM_READ_C8_CMD;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	structure->prom_data.C7 = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	return true;
}

bool ms5611_read(MS5611_t *structure, unsigned char osr, signed int *Pressure, signed int *Temperature)
{
	unsigned long D1 = 0, D2 = 0;
	signed long dT = 0, T2 = 0, TEMP = 0;
	signed long long OFF = 0, OFF2 = 0, SENS = 0, SENS2 = 0;
	if(!structure->prom_data_has_been_read) {
		if(!ms5611_reset_cmd_send(structure))
			return false;
		if(ms5611_read_prom_cmd_send(structure))
			structure->prom_data_has_been_read = true;
		else
			return false;
	}
	if(!ms5611_d1_cmd_send(structure, osr, &D1))
		return false;
	if(!ms5611_d2_cmd_send(structure, osr, &D2))
		return false;
	unsigned int tmp = structure->prom_data.C5 * ( 1<<8 );
	dT = D2 - tmp;
	TEMP = 2000 + dT * structure->prom_data.C6 /(1 << 23);
	OFF = ((signed long long)structure->prom_data.C2 * (signed long long)(1 << 16)) + (((signed long long)structure->prom_data.C4 * (signed long long)dT) / (signed long long)( 1<<7 ));
	SENS = ((signed long long)structure->prom_data.C1 * (signed long long)(1 << 15)) + (((signed long long)structure->prom_data.C3 * (signed long long)dT) / (signed long long)( 1<<8 ));

	if(TEMP >= 2000) {
		T2 = 0;
		OFF2 = 0;
		SENS2 = 0;
	} else {
		if(TEMP < 2000) {
			T2 = (dT * dT)/1 << 31;
			OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 1 << 1;
			SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 1 << 2;
		}
		if(TEMP < 1500) {
			OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
			SENS2 = SENS2 + 11 * ((TEMP + 1500) * (TEMP + 1500) / 1 << 1);
		}
	}

	TEMP = TEMP - T2;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;
	*Pressure = (signed int)((signed long long)(((((signed long long)D1 * (signed long long)SENS) / (signed long long)(1 << 21)) - (signed long long)OFF)) / (signed long long)(1 << 15));
	*Temperature = (signed int)TEMP;
	return true;
}

bool ms5611_display_pressure_result(MS5611_t *structure, unsigned char osr)
{
	signed int Pressure = 0;
	signed int Temperature = 0;
	//if(!ms5611_read_prom_cmd_send(prom_data, TwiStruct)) return false;
	if(!ms5611_read(structure, osr, &Pressure, &Temperature))
		return false;
#ifndef _TINY_PRINT_
	UARTprintf(DebugCom, "MS5611: P = %4.2f milibar, T = %2.2f gr celsius\n\r", ((float)Pressure) / 100.0, ((float)Temperature) / 100.0);
#else
	float PressureInt = 0;
	float PressureDec = modff(((float)Pressure)/100.0, &PressureInt);
	float TemperatureInt = 0;
	float TemperatureDec = modff(((float)Pressure)/100.0, &Temperature);
	UARTprintf(DebugCom, "MS5611: P = %d.%u milibar, T = %d.%u gr celsius\n\r", (signed int)PressureInt, (unsigned int)(PressureDec * 100.0), (signed int)TemperatureInt, (unsigned int)(TemperatureDec * 100.0));
#endif
	return true;
}


