/*
 *  lib/device/mpu60x0.c
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
#include "board_init.h"
#include "mpu60x0_9150.h"
#include "board_init.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/timer_api.h"

extern new_uart* DebugCom;

bool mpu60x0_9150_sample_rate_divider_set(MPU60x0_9150_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_SMPLRT_DIV;
	TwiStruct->TxBuff[1] = Value;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_who_am_i_get(MPU60x0_9150_t *structure)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_WHO_AM_I;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	if((TwiStruct->RxBuff[0] & 0x02 >> 1) == (structure->IcNr & 0x01))
	{
		if((TwiStruct->RxBuff[0] >> 2) == (MPU60x0_9150_ADDR >> 2))
			return true;
	}
	return false;
}

bool mpu60x0_9150_dlpf_cfg_set(MPU60x0_9150_t *structure, unsigned char value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_9150_CONFIG_DLPF_CFG_gm) | (value & MPU60X0_9150_CONFIG_DLPF_CFG_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_gyro_self_test_set(MPU60x0_9150_t *structure, bool X_test, bool Y_test, bool Z_test)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_GYRO_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_9150_GYRO_CONFIG_ST_gm) | (X_test << MPU60X0_9150_GYRO_CONFIG_XG_ST_bp) | (Y_test << MPU60X0_9150_GYRO_CONFIG_YG_ST_bp) | (Z_test << MPU60X0_9150_GYRO_CONFIG_ZG_ST_bp);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_gyro_full_scale_set(MPU60x0_9150_t *structure, unsigned int scale)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_GYRO_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	unsigned char tmp = 0;
	switch(scale)
	{
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_250:
		tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_250 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_500:
		tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_500 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_1000:
		tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_1000 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	case MPU60X0_9150_GYRO_CONFIG_FS_SEL_2000:
		tmp = MPU60X0_9150_GYRO_CONFIG_FS_SEL_2000 << MPU60X0_9150_GYRO_CONFIG_FS_SEL_gp;
		break;
	default:
		return false;
	}
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_9150_GYRO_CONFIG_FS_SEL_gm) | tmp;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_accel_self_test_set(MPU60x0_9150_t *structure, bool X_test, bool Y_test, bool Z_test)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_ACCEL_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_9150_ACCEL_CONFIG_ST_gm) | (X_test << MPU60X0_9150_ACCEL_CONFIG_XA_ST_bp) | (Y_test << MPU60X0_9150_ACCEL_CONFIG_YA_ST_bp) | (Z_test << MPU60X0_9150_ACCEL_CONFIG_ZA_ST_bp);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_accel_full_scale_set(MPU60x0_9150_t *structure, unsigned int scale)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_ACCEL_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	unsigned char tmp = 0;
	switch(scale)
	{
	case 250:
		tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_2 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 500:
		tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_4 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 1000:
		tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_8 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 2000:
		tmp = MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_16 << MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	default:
		return false;
	}
	TwiStruct->TxBuff[0] = (TwiStruct->RxBuff[0] & ~MPU60X0_9150_ACCEL_CONFIG_AFS_SEL_gm) | tmp;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_accel_data_get(MPU60x0_9150_t *structure, signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_ACCEL_XOUT_H;
	if(!SetupI2CReception(TwiStruct, 1, 6))
		return false;
	*X_Axis = ((TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1]) + structure->XA_Cal;
	*Y_Axis = ((TwiStruct->RxBuff[2] << 8) + TwiStruct->RxBuff[3]) + structure->YA_Cal;
	*Z_Axis = ((TwiStruct->RxBuff[4] << 8) + TwiStruct->RxBuff[5]) + structure->ZA_Cal;
	return true;
}

bool mpu60x0_9150_gyro_data_get(MPU60x0_9150_t *structure, signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_GYRO_XOUT_H;
	if(!SetupI2CReception(TwiStruct, 1, 6))
		return false;
	*X_Axis = ((TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1]) + structure->XG_Cal;
	*Y_Axis = ((TwiStruct->RxBuff[2] << 8) + TwiStruct->RxBuff[3]) + structure->YG_Cal;
	*Z_Axis = ((TwiStruct->RxBuff[4] << 8) + TwiStruct->RxBuff[5]) + structure->ZG_Cal;
	return true;
}

bool mpu60x0_9150_temp_data_get(MPU60x0_9150_t *structure, float *Temperature)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_TEMP_OUT_H;
	if(!SetupI2CReception(TwiStruct, 1, 2))
		return false;
	*Temperature = (((float)(((signed short)(TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1])))/340.0) + 36.53;
	return true;
}

bool mpu60x0_9150_clock_sel_set(MPU60x0_9150_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_9150_PWR_MGMT_1_CLKSEL_gm) | (Value & MPU60X0_9150_PWR_MGMT_1_CLKSEL_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_temp_dis_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bp) & MPU60X0_9150_PWR_MGMT_1_TEMP_DIS_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_cycle_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_1_CYCLE_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_CYCLE_bp) & MPU60X0_9150_PWR_MGMT_1_CYCLE_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_sleep_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_1_SLEEP_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_SLEEP_bp) & MPU60X0_9150_PWR_MGMT_1_SLEEP_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_device_reset_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bp) & MPU60X0_9150_PWR_MGMT_1_DEVICE_RESET_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_stby_zg_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_ZG_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_stby_yg_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_2_STBY_YG_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_YG_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_YG_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_stby_xg_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_2_STBY_XG_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_XG_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_XG_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_stby_za_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_ZA_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_stby_ya_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_2_STBY_YA_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_YA_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_YA_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_stby_xa_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_2_STBY_XA_bm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_STBY_XA_bp) & MPU60X0_9150_PWR_MGMT_2_STBY_XA_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_lp_wake_ctrl_set(MPU60x0_9150_t *structure, unsigned char Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gm)) | ((Value << MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gp) & MPU60X0_9150_PWR_MGMT_2_LP_WAKE_CTRL_gm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_i2c_bypass_enable_set(MPU60x0_9150_t *structure, bool Value)
{
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_INT_PIN_CFG;
	if(!SetupI2CReception(TwiStruct, 1, 1))
		return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bm)) | ((Value << MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bp) & MPU60X0_9150_INT_PIN_CFG_I2C_BYPASS_EN_bm);
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;
	return true;
}

bool mpu60x0_9150_gyroscope_test(MPU60x0_9150_t *structure, double *XtestResult, double *YtestResult, double *ZtestResult)
{
	Twi_t *TwiStruct = structure->TWI;
	signed short Xdata = 0;
	signed short Ydata = 0;
	signed short Zdata = 0;
	signed short _Xdata = 0;
	signed short _Ydata = 0;
	signed short _Zdata = 0;
	if(!mpu60x0_9150_gyro_data_get(structure, &Xdata, &Ydata, &Zdata))
		return false;
	if(!mpu60x0_9150_gyro_self_test_set(structure, true, true, true))
		return false;
	sys_delay(100);
	if(!mpu60x0_9150_gyro_data_get(structure, &_Xdata, &_Ydata, &_Zdata))
		return false;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_SELF_TEST_X;
	if(!SetupI2CReception(TwiStruct, 1, 3))
		return false;
	double XG_TEST = (double)(TwiStruct->RxBuff[0] & 0x1F);
	double YG_TEST = (double)(TwiStruct->RxBuff[1] & 0x1F);
	double ZG_TEST = (double)(TwiStruct->RxBuff[2] & 0x1F);
	if(!mpu60x0_9150_gyro_self_test_set(structure, false, false, false))
		return false;
	*XtestResult = 0;
	*YtestResult = 0;
	*ZtestResult = 0;
	if(XG_TEST != 0.0) *XtestResult = 25.0 * 131.0 * pow(1.046, XG_TEST - 1.0);
	if(YG_TEST != 0.0) *YtestResult = -25.0 * 131.0 * pow(1.046, YG_TEST - 1.0);
	if(ZG_TEST != 0.0) *ZtestResult = 25.0 * 131.0 * pow(1.046, ZG_TEST - 1.0);
	*XtestResult = (((double)_Xdata - (double)Xdata) - *XtestResult) / *XtestResult;
	*YtestResult = (((double)_Ydata - (double)Ydata) - *YtestResult) / *YtestResult;
	*ZtestResult = (((double)_Zdata - (double)Zdata) - *ZtestResult) / *ZtestResult;
	return true;
}

bool mpu60x0_9150_accelerometer_test(MPU60x0_9150_t *structure, double *XtestResult, double *YtestResult, double *ZtestResult)
{
	Twi_t *TwiStruct = structure->TWI;
	signed short Xdata = 0;
	signed short Ydata = 0;
	signed short Zdata = 0;
	signed short _Xdata = 0;
	signed short _Ydata = 0;
	signed short _Zdata = 0;
	if(!mpu60x0_9150_accel_data_get(structure, &Xdata, &Ydata, &Zdata))
		return false;
	if(!mpu60x0_9150_accel_self_test_set(structure, true, true, true))
		return false;
	sys_delay(100);
	if(!mpu60x0_9150_accel_data_get(structure, &_Xdata, &_Ydata, &_Zdata))
		return false;
	TwiStruct->MasterSlaveAddr = MPU60x0_9150_ADDR | (structure->IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_9150_SELF_TEST_X;
	if(!SetupI2CReception(TwiStruct, 1, 4))
		return false;
	double XA_TEST = (double)(((TwiStruct->RxBuff[0] >> 4) & 0x0C) | ((TwiStruct->RxBuff[3] >> 4) & 0x03));
	double YA_TEST = (double)(((TwiStruct->RxBuff[1] >> 4) & 0x0C) | ((TwiStruct->RxBuff[3] >> 2) & 0x03));
	double ZA_TEST = (double)(((TwiStruct->RxBuff[2] >> 4) & 0x0C) | ((TwiStruct->RxBuff[3] >> 0) & 0x03));

	if(!mpu60x0_9150_accel_self_test_set(structure, false, false, false))
		return false;
	*XtestResult = 0;
	*YtestResult = 0;
	*ZtestResult = 0;
	if(XA_TEST != 0.0) *XtestResult = 4096.0 * 0.34 * (pow(0.92, (XA_TEST - 1.0) / ((1<<5) - 2)) / 0.34);
	if(YA_TEST != 0.0) *YtestResult = 4096.0 * 0.34 * (pow(0.92, (YA_TEST - 1.0) / ((1<<5) - 2)) / 0.34);
	if(ZA_TEST != 0.0) *ZtestResult = 4096.0 * 0.34 * (pow(0.92, (ZA_TEST - 1.0) / ((1<<5) - 2)) / 0.34);
	*XtestResult = (((double)_Xdata - (double)Xdata) - *XtestResult) / *XtestResult;
	*YtestResult = (((double)_Ydata - (double)Ydata) - *YtestResult) / *YtestResult;
	*ZtestResult = (((double)_Zdata - (double)Zdata) - *ZtestResult) / *ZtestResult;
	return true;
}

bool mpu60x0_9150_gyroscope_test_display_result(MPU60x0_9150_t *structure)
{
	double XtestResult = 0;
	double YtestResult = 0;
	double ZtestResult = 0;
	if(!mpu60x0_9150_gyroscope_test(structure , &XtestResult, &YtestResult, &ZtestResult))
		return false;
	double XtestResultInt = 0;
	double XtestResultDec = modf(((double)XtestResult), &XtestResultInt);
	double YtestResultInt = 0;
	double YtestResultDec = modf(((double)YtestResult), &YtestResultInt);
	double ZtestResultInt = 0;
	double ZtestResultDec = modf(((double)ZtestResult), &ZtestResultInt);
	UARTprintf(DebugCom, "MPU60x0: Giroscope calibration value: Xg = %d.%u, Yg = %d.%u, Zg = %d.%u\n\r", (signed int)XtestResultInt, (unsigned int)(XtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(YtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(ZtestResultDec * 10000));
	return true;
}

bool mpu60x0_9150_accelerometer_test_display_result(MPU60x0_9150_t *structure)
{
	double XtestResult = 0;
	double YtestResult = 0;
	double ZtestResult = 0;
	if(!mpu60x0_9150_accelerometer_test(structure , &XtestResult, &YtestResult, &ZtestResult))
		return false;
	double XtestResultInt = 0;
	double XtestResultDec = modf(((double)XtestResult), &XtestResultInt);
	double YtestResultInt = 0;
	double YtestResultDec = modf(((double)YtestResult), &YtestResultInt);
	double ZtestResultInt = 0;
	double ZtestResultDec = modf(((double)ZtestResult), &ZtestResultInt);
	UARTprintf(DebugCom, "MPU60x0: Accelerometer calibration value: Xa = %d.%u, Ya = %d.%u, Za = %d.%u\n\r", (signed int)XtestResultInt, (unsigned int)(XtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(YtestResultDec * 10000), (signed int)ZtestResultInt, (unsigned int)(ZtestResultDec * 10000));
	return true;
}

bool mpu60x0_9150_temperature_display_result(MPU60x0_9150_t *structure)
{
	float Temperature = 0;
	if(!mpu60x0_9150_temp_data_get(structure , &Temperature))
		return false;
#ifndef _TINY_PRINT_
	UARTprintf(DebugCom, "MPU60x0: Temperature: %2.2f Gr Celsius\n\r", Temperature);
#else
	float GrCelsius = 0;
	float GrCelsiusMod = modff(Temperature, &GrCelsius);
	UARTprintf(DebugCom, "MPU60x0: Temperature: %d.%u Gr Celsius\n\r", (unsigned int)GrCelsius, (unsigned int)(GrCelsiusMod*1000));
#endif
	return true;
}

bool mpu60x0_9150_giroscope_display_result(MPU60x0_9150_t *structure)
{
	signed short Xg = 0;
	signed short Yg = 0;
	signed short Zg = 0;
	if(!mpu60x0_9150_gyro_data_get(structure, &Xg, &Yg, &Zg))
		return false;
	UARTprintf(DebugCom, "MPU60x0: Giroscope: Xg = %d, Yg = %d, Zg = %d\n\r", Xg, Yg, Zg);
	return true;
}

bool mpu60x0_9150_accelerometer_display_result(MPU60x0_9150_t *structure)
{
	signed short Xa = 0;
	signed short Ya = 0;
	signed short Za = 0;
	if(!mpu60x0_9150_accel_data_get(structure, &Xa, &Ya, &Za))
		return false;
	UARTprintf(DebugCom, "MPU60x0: Accelerometer: Xa = %d, Ya = %d, Za = %d\n\r", Xa, Ya, Za);
	return true;
}

bool mpu60x0_9150_init(MPU60x0_9150_t *structure)
{
	if(!mpu60x0_9150_who_am_i_get(structure))
		return false;
	if(!mpu60x0_9150_sleep_set(structure, false))
		return false;
	if(!mpu60x0_9150_clock_sel_set(structure, MPU60X0_9150_PWR_MGMT_1_CLKSEL_PLL_X_AX))
		return false;
	if(!mpu60x0_9150_gyro_full_scale_set(structure, MPU60X0_9150_GYRO_CONFIG_FS_SEL_2000))
		return false;
	if(!mpu60x0_9150_i2c_bypass_enable_set(structure, true))
		return false;
	return true;
}
