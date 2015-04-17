/*
 *  lib/device/mpl3115a2.c
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
#include "mpl3115a2.h"
#include "board_init.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/timer_api.h"

bool mpl3115a2_reg_read(MPL3115A2_t *structure, unsigned char reg, unsigned char *data, unsigned char len)
{
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPL3115A2_ADDR;
	TwiStruct->TxBuff[0] = reg;
	if(!SetupI2CReception(TwiStruct, 1, len))
		return false;
	memcpy(data, TwiStruct->RxBuff, len);
	return true;
}

bool mpl3115a2_reg_write(MPL3115A2_t *structure, unsigned char reg, unsigned char *data, unsigned char len)
{
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPL3115A2_ADDR;
	TwiStruct->TxBuff[0] = reg;
	memcpy(TwiStruct->TxBuff + 1, data, len);
	if(!SetupI2CTransmit(TwiStruct, 1 + len))
		return false;
	return true;
}

bool mpl3115a2_get_alt_temp(MPL3115A2_t *structure, float *altitude, float *temperature) {
	unsigned long out_p = 0;
	unsigned short out_t = 0;
	unsigned char tmp_buff[3];
	unsigned char tmp;
	tmp = 0xB8;
	if(!mpl3115a2_reg_write(structure, MPL3115A2_CTRL_REG1, &tmp, 1))
		return false;
	tmp = 0x07;
	if(!mpl3115a2_reg_write(structure, MPL3115A2_PT_DATA_CFG, &tmp, 1))
		return false;
	tmp = 0xB9;
	if(!mpl3115a2_reg_write(structure, MPL3115A2_CTRL_REG1, &tmp, 1))
		return false;
	unsigned int timeout = 1000;
	do {
		sys_delay(2);
		if(!mpl3115a2_reg_read(structure, MPL3115A2_STATUS, &tmp, 1))
			return false;
		if(!timeout--)
			return false;
	}while(!(tmp & 0x08));
	if(!mpl3115a2_reg_read(structure, MPL3115A2_OUT_P_MSB, tmp_buff, 3))
		return false;
	out_p = ((tmp_buff[0] << 16) + (tmp_buff[1] << 8) + tmp_buff[2]) >> 4;
	if(!mpl3115a2_reg_read(structure, MPL3115A2_OUT_T_MSB, tmp_buff, 2))
		return false;
	out_t = ((tmp_buff[0] << 8) + tmp_buff[1]) >> 4;
	*altitude = ((float)out_p) / 16.0;
	*temperature = ((float)out_t) / 16.0;
	return true;
}

bool mpl3115a2_init(MPL3115A2_t *structure)
{
	return true;
}
