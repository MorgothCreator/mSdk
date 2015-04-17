/*
 *  lib/device/hih613x.c
 *
 *  Copyright (C) 2015  Iulian Gheorghiu <morgoth.creator@gmail.com>
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
#include "hih6130.h"
#include "api/twi_def.h"
#include "api/timer_api.h"

bool hih613x_get_hum_temp(HIH613x_t *structure, unsigned char *status, float *hum, float *temp) {
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = HIH613x_ADDR;
	if(!SetupI2CTransmit(TwiStruct, 0))
		return false;
	//Sysdelay(100);
	unsigned int timeout = 100;
	structure->TWI->NoSendWriteOnRead = true;
	do {
		sys_delay(2);
		if(!SetupI2CReception(TwiStruct, 0, 4)) {
			structure->TWI->NoSendWriteOnRead = false;
			return false;
		}
		if(!timeout--)
			return false;
	}while(((TwiStruct->RxBuff[0] >> 6) & 0x03) == 0x01);
	structure->TWI->NoSendWriteOnRead = false;
	*status = (TwiStruct->RxBuff[0] >> 6) & 0x03;
	TwiStruct->RxBuff[0] = TwiStruct->RxBuff[0] & 0x3F;
	*hum = (float)((((unsigned short)TwiStruct->RxBuff[0]) << 8) | TwiStruct->RxBuff[1]) * 6.10e-3;
	*temp = (float)(((((unsigned short)TwiStruct->RxBuff[2]) << 8) | TwiStruct->RxBuff[3]) >> 2) * 1.007e-2 - 40.0;
	return true;
}
