/*
 *  lib/device/mpl3115a2.c
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
#include <string.h>
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
	if(!twi.trx(TwiStruct, MPL3115A2_ADDR, &reg, 1, data, len))
		return false;
	return true;
}

bool mpl3115a2_reg_write(MPL3115A2_t *structure, unsigned char reg, unsigned char *data, unsigned char len)
{
	if(!structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPL3115A2_ADDR;
	unsigned char *tmp = malloc(len + 1);
	tmp[0] = reg;
	memcpy(tmp + 1, data, len);
	if(!twi.tx(TwiStruct, MPL3115A2_ADDR, tmp, 1 + len))
	{
		free(tmp);
		return false;
	}
	free(tmp);
	return true;
}

bool mpl3115a2_get_alt_temp(MPL3115A2_t *structure, unsigned char oversample, float *pression, float *altitude, float *temperature) {
	unsigned long out_p = 0;
	//signed short out_t = 0;
	unsigned char tmp_buff[5];
	unsigned char tmp;
	tmp = (1 << 7) | ((oversample & 0x07) << 3);
	if(!mpl3115a2_reg_write(structure, MPL3115A2_CTRL_REG1, &tmp, 1))
		return false;
	tmp = 0x07;
	if(!mpl3115a2_reg_write(structure, MPL3115A2_PT_DATA_CFG, &tmp, 1))
		return false;
	tmp = (1 << 7) | ((oversample & 0x07) << 3) | 1;
	if(!mpl3115a2_reg_write(structure, MPL3115A2_CTRL_REG1, &tmp, 1))
		return false;
	unsigned int timeout = 1000;
	do {
		if(!mpl3115a2_reg_read(structure, MPL3115A2_STATUS, &tmp, 1))
			return false;
		if(!timeout--)
			return false;
		sys_delay(2);
	}while(!(tmp & 0x08));
	if(!mpl3115a2_reg_read(structure, MPL3115A2_OUT_P_MSB, tmp_buff, 5))
		return false;
	out_p = ((tmp_buff[0] << 12) + (tmp_buff[1] << 4) + (tmp_buff[2] >> 4));
	//if(!mpl3115a2_reg_read(structure, MPL3115A2_OUT_T_MSB, tmp_buff, 2))
		//return false;
	signed char _tmp = tmp_buff[3];
	float temp = (float)_tmp;
	unsigned char temp_frac = tmp_buff[4] >> 4;
	if(!mpl3115a2_reg_read(structure, MPL3115A2_OFF_H, &tmp, 1))
		return false;
	*altitude = (44330.77 * (1.0 - ((float)out_p / (float)101326.0) * 0.1902632) + tmp) / 1000.0;
	*temperature = temp + ((1.0 * temp_frac) / 256.0);
	return true;
}

bool mpl3115a2_init(MPL3115A2_t *structure)
{
	return true;
}
