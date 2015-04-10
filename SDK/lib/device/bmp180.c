/*
 *  lib/device/bmp180.c
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
#include "board_init.h"
#include "bmp180.h"
#include "api/twi_def.h"
#include "api/timer_api.h"


bool bmp180_read_calibration(BMP180_t *structure) {
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = BMP180_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = BMP180_CALIB;
	if(!SetupI2CReception(TwiStruct, 1, BMP180_CALIB_LEN))
		return false;
	structure->cal_data.AC1 = (signed short)((TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1]);
	structure->cal_data.AC2 = (signed short)((TwiStruct->RxBuff[2] << 8) + TwiStruct->RxBuff[3]);
	structure->cal_data.AC3 = (signed short)((TwiStruct->RxBuff[4] << 8) + TwiStruct->RxBuff[5]);
	structure->cal_data.AC4 = (unsigned short)((TwiStruct->RxBuff[6] << 8) + TwiStruct->RxBuff[7]);
	structure->cal_data.AC5 = (unsigned short)((TwiStruct->RxBuff[8] << 8) + TwiStruct->RxBuff[9]);
	structure->cal_data.AC6 = (unsigned short)((TwiStruct->RxBuff[10] << 8) + TwiStruct->RxBuff[11]);
	structure->cal_data.B1 = (unsigned short)((TwiStruct->RxBuff[12] << 8) + TwiStruct->RxBuff[13]);
	structure->cal_data.B2 = (unsigned short)((TwiStruct->RxBuff[14] << 8) + TwiStruct->RxBuff[15]);
	structure->cal_data.MB = (unsigned short)((TwiStruct->RxBuff[16] << 8) + TwiStruct->RxBuff[17]);
	structure->cal_data.MC = (unsigned short)((TwiStruct->RxBuff[18] << 8) + TwiStruct->RxBuff[19]);
	structure->cal_data.MD = (unsigned short)((TwiStruct->RxBuff[20] << 8) + TwiStruct->RxBuff[21]);
	return true;
}

bool bmp180_read_temp(BMP180_t *structure, signed long *temp) {
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = BMP180_ADDR | (structure->IcNr & 0x03);
	TwiStruct->TxBuff[0] = BMP180_CTRL_MEAS;
	TwiStruct->TxBuff[1] = 0x2E;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	Sysdelay(6);
	if(!structure->TWI)
		return false;
	TwiStruct->TxBuff[0] = BMP180_OUT_MSB;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	*temp = (signed long)((TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1]);
	return true;
}

bool bmp180_read_pressure(BMP180_t *structure, signed long *pressure, unsigned char oss) {
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = BMP180_ADDR | (structure->IcNr & 0x03);
	TwiStruct->TxBuff[0] = BMP180_CTRL_MEAS;
	TwiStruct->TxBuff[1] = 0x34 + (oss << BMP180_CTRL_MEAS_OSS_gp);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	Sysdelay(6 * (1 << oss));
	TwiStruct->TxBuff[0] = BMP180_OUT_MSB;
	if(!SetupI2CReception(TwiStruct, 1, 3))
		return false;
	*pressure = (signed long)(((TwiStruct->RxBuff[0] << 16) + (TwiStruct->RxBuff[1] << 8) + TwiStruct->RxBuff[2]) >> (8 - oss));
	return true;
}

bool bmp180_display_result(BMP180_t *structure, unsigned char oss) {
	if(!structure->TWI)
		return false;
	if(!bmp180_read_calibration(structure))
		return false;
	signed long UT = 0, UP = 0;
	if(!bmp180_read_temp(structure, &UT))
		return false;
	signed long X1 = (UT - structure->cal_data.AC6) * structure->cal_data.AC5 / 32768;
	signed long X2 = structure->cal_data.MC * 2048 / (X1 + structure->cal_data.MD);
	signed long B5 = X1 + X2;
	signed long T = (B5 + 8) / 16;

	if(!bmp180_read_pressure(structure, &UP, oss))
		return false;
	signed long B6 = B5 - 4000;
	X1 = (structure->cal_data.B2 * (B6 * B6 / 4096)) / 2048;
	X2 = structure->cal_data.AC2 * B6 / 2048;
	signed long X3 = X1 + X2;
	signed long B3 = (((structure->cal_data.AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = structure->cal_data.AC3 * B6 / 8192;
	X2 = (structure->cal_data.B1 * (B6 * B6 / 4096)) / 65536;
	X3 = ((X1 + X2) + 2) / 4;
	unsigned long B4 = structure->cal_data.AC4 * (unsigned long)(X3 + 32768) / 32768;
	unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	signed long p = 0;
	if(B7 < 80000000) {
		p = (B7 * 2) / B4;
	} else {
		p = (B7 / B4) * 2;
	}
	X1 = (p / 256) * (p / 256);
	X1 = (X1 * 3038) / 65536;
	X2 = (-7357 * p) / 65536;
	p = p + (X1 + X2 + 3791) / 16;

	float altitude = 44330.0 * ((1.0 - (((float)p / 100.0) / 1013.25)) * (1.0 / 5.255));
	UARTprintf(DebugCom, "BMP180: T = %2.1f, P = %4.2f, Alt = %4.2f\n\r", (float)(T / 10.0), (float)p / 100.0, altitude);
	return true;
}

bool bmp180_get_temp(BMP180_t *structure, float *temp) {
	if(!structure->TWI)
		return false;
	if(!bmp180_read_calibration(structure))
		return false;
	signed long UT = 0;
	if(!bmp180_read_temp(structure, &UT))
		return false;
	signed long X1 = (UT - structure->cal_data.AC6) * structure->cal_data.AC5 / 32768;
	signed long X2 = structure->cal_data.MC * 2048 / (X1 + structure->cal_data.MD);
	signed long B5 = X1 + X2;
	signed long T = (B5 + 8) / 16;
	*temp = (float)(T / 10.0);
	return true;
}

bool bmp180_get_pressure(BMP180_t *structure, float *pressure, unsigned char oss) {
	if(!bmp180_read_calibration(structure))
		return false;
	signed long UT = 0, UP = 0;
	if(!bmp180_read_temp(structure, &UT))
		return false;
	signed long X1 = (UT - structure->cal_data.AC6) * structure->cal_data.AC5 / 32768;
	signed long X2 = structure->cal_data.MC * 2048 / (X1 + structure->cal_data.MD);
	signed long B5 = X1 + X2;
	//signed long T = (B5 + 8) / 16;

	if(!bmp180_read_pressure(structure, &UP, oss))
		return false;
	signed long B6 = B5 - 4000;
	X1 = (structure->cal_data.B2 * (B6 * B6 / 4096)) / 2048;
	X2 = structure->cal_data.AC2 * B6 / 2048;
	signed long X3 = X1 + X2;
	signed long B3 = (((structure->cal_data.AC1 * 4 + X3) << oss) + 2) / 4;
	X1 = structure->cal_data.AC3 * B6 / 8192;
	X2 = (structure->cal_data.B1 * (B6 * B6 / 4096)) / 65536;
	X3 = ((X1 + X2) + 2) / 4;
	unsigned long B4 = structure->cal_data.AC4 * (unsigned long)(X3 + 32768) / 32768;
	unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> oss);
	signed long p = 0;
	if(B7 < 80000000) {
		p = (B7 * 2) / B4;
	} else {
		p = (B7 / B4) * 2;
	}
	X1 = (p / 256) * (p / 256);
	X1 = (X1 * 3038) / 65536;
	X2 = (-7357 * p) / 65536;
	p = p + (X1 + X2 + 3791) / 16;
	*pressure = (float)p / 100.0;
	return true;
}

bool bmp180_get_altitude(BMP180_t *structure, float *altitude, unsigned char oss) {
	float pressure = 0.0;
	if(!bmp180_get_pressure(structure, &pressure, oss))
		return false;
	*altitude = 44330.0 * ((1.0 - (pressure / 1013.25)) * (1.0 / 5.255));
	return true;
}

