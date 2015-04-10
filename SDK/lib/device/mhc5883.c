/*
 *  lib/device/mhc5883.c
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
#include "mhc5883.h"
#include "board_init.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/timer_api.h"

bool mhc5883_sample_nr_set(MHC5883_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MHC5883_ADDR;
	TwiStruct->TxBuff[0] = MHC5883_CONFIG_REG_A;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MHC5883_CONFIG_REG_A_MA_gm) | ((Value << MHC5883_CONFIG_REG_A_MA_gp) & MHC5883_CONFIG_REG_A_MA_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mhc5883_data_output_rate_set(MHC5883_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MHC5883_ADDR;
	TwiStruct->TxBuff[0] = MHC5883_CONFIG_REG_A;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MHC5883_CONFIG_REG_A_DO_gm) | ((Value << MHC5883_CONFIG_REG_A_DO_gp) & MHC5883_CONFIG_REG_A_DO_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mhc5883_measurament_mode_set(MHC5883_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MHC5883_ADDR;
	TwiStruct->TxBuff[0] = MHC5883_CONFIG_REG_A;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MHC5883_CONFIG_REG_A_MS_gm) | ((Value << MHC5883_CONFIG_REG_A_MS_gp) & MHC5883_CONFIG_REG_A_MS_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mhc5883_gain_set(MHC5883_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MHC5883_ADDR;
	TwiStruct->TxBuff[0] = MHC5883_CONFIG_REG_B;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MHC5883_CONFIG_REG_B_GN_gm) | ((Value << MHC5883_CONFIG_REG_B_GN_gp) & MHC5883_CONFIG_REG_B_GN_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mhc5883_mode_set(MHC5883_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MHC5883_ADDR;
	TwiStruct->TxBuff[0] = MHC5883_MODE_REG;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MHC5883_MODE_REG_MD_gm) | ((Value << MHC5883_MODE_REG_MD_gp) & MHC5883_MODE_REG_MD_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mhc5883_data_get(MHC5883_t *structure, signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MHC5883_ADDR;
	TwiStruct->TxBuff[0] = MHC5883_DATA_OUT_X_H;
	if(!SetupI2CReception(TwiStruct, 1, 6))
		return false;
	*X_Axis = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	*Z_Axis = (TwiStruct->RxBuff[2] << 8) + TwiStruct->RxBuff[3];
	*Y_Axis = (TwiStruct->RxBuff[4] << 8) + TwiStruct->RxBuff[5];
	return true;
}

bool mhc5883_display_result(MHC5883_t *structure)
{
	signed short Xc = 0;
	signed short Yc = 0;
	signed short Zc = 0;
	if(!mhc5883_data_get(structure, &Xc, &Yc, &Zc))
		return false;
#ifndef _TINY_PRINT_
	UARTprintf(DebugCom, "MHC5883: Compass: Xc = %d, Yc = %d, Zc = %d\n\r", Xc, Yc, Zc);
#else
	UARTprintf(DebugCom, "MHC5883: Compass: Xc = %d, Yc = %d, Zc = %d\n\r", Xc, Yc, Zc);
#endif

	return true;
}

bool mhc5883_display_positive_test_result(MHC5883_t *structure)
{
	if(!mhc5883_measurament_mode_set(structure, MHC5883_CONFIG_REG_A_MS_POSITIVE_BIAS))
		return false;
	sys_delay(100);
	signed short Xc = 0;
	signed short Yc = 0;
	signed short Zc = 0;
	if(!mhc5883_data_get(structure, &Xc, &Yc, &Zc))
		return false;
	UARTprintf(DebugCom, "Compass positive test: Xc = %d, Yc = %d, Zc = %d\n\r", Xc, Yc, Zc);
	if(!mhc5883_measurament_mode_set(structure, MHC5883_CONFIG_REG_A_MS_NORMAL))
		return false;
	sys_delay(100);
	return true;
}

bool mhc5883_display_negative_test_result(MHC5883_t *structure)
{
	if(!mhc5883_measurament_mode_set(structure, MHC5883_CONFIG_REG_A_MS_NEGATIVE_BIAS)) return false;
	sys_delay(100);
	signed short Xc = 0;
	signed short Yc = 0;
	signed short Zc = 0;
	if(!mhc5883_data_get(structure, &Xc, &Yc, &Zc))
		return false;
	UARTprintf(DebugCom, "Compass negative test: Xc = %d, Yc = %d, Zc = %d\n\r", Xc, Yc, Zc);
	if(!mhc5883_measurament_mode_set(structure, MHC5883_CONFIG_REG_A_MS_NORMAL)) return false;
	sys_delay(100);
	return true;
}

bool mhc5883_init(MHC5883_t *structure)
{
	if(!mhc5883_mode_set(structure, MHC5883_MODE_REG_MD_CONTINUOUS))
		return false;
	return true;
}
