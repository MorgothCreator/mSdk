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

#include "stdbool.h"
#include "math.h"
#include "MPU60x0.h"
#include "board_init.h"
#include "../api/twi_api.h"
#include "../api/twi_def.h"
#include "../api/uart_api.h"
#include "../api/uart_def.h"
#include "sys/sysdelay.h"

bool mpu60x0_sample_rate_divider_set(Twi_t *TwiStruct, unsigned char IcNr, unsigned char Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_SMPLRT_DIV;
	TwiStruct->TxBuff[1] = Value;
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_who_am_i_get(Twi_t *TwiStruct, unsigned char IcNr)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_WHO_AM_I;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	if((TwiStruct->RxBuff[0] & 0x02 >> 1) == (IcNr & 0x01))
	{
		if((TwiStruct->RxBuff[0] >> 2) == (MPU60x0_ADDR >> 2)) return true;
	}
	return false;
}

bool mpu60x0_dlpf_cfg_set(Twi_t *TwiStruct, unsigned char IcNr, unsigned char value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_CONFIG_DLPF_CFG_gm) | (value & MPU60X0_CONFIG_DLPF_CFG_gm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_gyro_self_test_set(Twi_t *TwiStruct, unsigned char IcNr, bool X_test, bool Y_test, bool Z_test)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_GYRO_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_GYRO_CONFIG_ST_gm) | (X_test << MPU60X0_GYRO_CONFIG_XG_ST_bp) | (Y_test << MPU60X0_GYRO_CONFIG_YG_ST_bp) | (Z_test << MPU60X0_GYRO_CONFIG_ZG_ST_bp);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_gyro_full_scale_set(Twi_t *TwiStruct, unsigned char IcNr, unsigned int scale)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_GYRO_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	unsigned char tmp = 0;
	switch(scale)
	{
	case 250:
		tmp = MPU60X0_GYRO_CONFIG_FS_SEL_250 << MPU60X0_GYRO_CONFIG_FS_SEL_gp;
		break;
	case 500:
		tmp = MPU60X0_GYRO_CONFIG_FS_SEL_500 << MPU60X0_GYRO_CONFIG_FS_SEL_gp;
		break;
	case 1000:
		tmp = MPU60X0_GYRO_CONFIG_FS_SEL_1000 << MPU60X0_GYRO_CONFIG_FS_SEL_gp;
		break;
	case 2000:
		tmp = MPU60X0_GYRO_CONFIG_FS_SEL_2000 << MPU60X0_GYRO_CONFIG_FS_SEL_gp;
		break;
	default:
		return false;
	}
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_GYRO_CONFIG_FS_SEL_gm) | tmp;
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_accel_self_test_set(Twi_t *TwiStruct, unsigned char IcNr, bool X_test, bool Y_test, bool Z_test)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_ACCEL_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_ACCEL_CONFIG_ST_gm) | (X_test << MPU60X0_ACCEL_CONFIG_XA_ST_bp) | (Y_test << MPU60X0_ACCEL_CONFIG_YA_ST_bp) | (Z_test << MPU60X0_ACCEL_CONFIG_ZA_ST_bp);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_accel_full_scale_set(Twi_t *TwiStruct, unsigned char IcNr, unsigned int scale)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_ACCEL_CONFIG;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	unsigned char tmp = 0;
	switch(scale)
	{
	case 250:
		tmp = MPU60X0_ACCEL_CONFIG_AFS_SEL_2 << MPU60X0_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 500:
		tmp = MPU60X0_ACCEL_CONFIG_AFS_SEL_4 << MPU60X0_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 1000:
		tmp = MPU60X0_ACCEL_CONFIG_AFS_SEL_8 << MPU60X0_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	case 2000:
		tmp = MPU60X0_ACCEL_CONFIG_AFS_SEL_16 << MPU60X0_ACCEL_CONFIG_AFS_SEL_gp;
		break;
	default:
		return false;
	}
	TwiStruct->TxBuff[0] = (TwiStruct->RxBuff[0] & ~MPU60X0_ACCEL_CONFIG_AFS_SEL_gm) | tmp;
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_accel_data_get(Twi_t *TwiStruct, unsigned char IcNr, signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_ACCEL_XOUT_H;
	if(!SetupI2CReception(TwiStruct, 1, 6)) return false;
	*X_Axis = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	*Y_Axis = (TwiStruct->RxBuff[2] << 8) + TwiStruct->RxBuff[3];
	*Z_Axis = (TwiStruct->RxBuff[4] << 8) + TwiStruct->RxBuff[5];
	return true;
}

bool mpu60x0_gyro_data_get(Twi_t *TwiStruct, unsigned char IcNr, signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_GYRO_XOUT_H;
	if(!SetupI2CReception(TwiStruct, 1, 6)) return false;
	*X_Axis = (TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1];
	*Y_Axis = (TwiStruct->RxBuff[2] << 8) + TwiStruct->RxBuff[3];
	*Z_Axis = (TwiStruct->RxBuff[4] << 8) + TwiStruct->RxBuff[5];
	return true;
}

bool mpu60x0_temp_data_get(Twi_t *TwiStruct, unsigned char IcNr, float *Temperature)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_TEMP_OUT_H;
	if(!SetupI2CReception(TwiStruct, 1, 2)) return false;
	*Temperature = (((float)(((signed short)(TwiStruct->RxBuff[0] << 8) + TwiStruct->RxBuff[1])))/340.0) + 36.53;
	return true;
}

bool mpu60x0_clock_sel_set(Twi_t *TwiStruct, unsigned char IcNr, unsigned char Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~MPU60X0_PWR_MGMT_1_CLKSEL_gm) | (Value & MPU60X0_PWR_MGMT_1_CLKSEL_gm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_temp_dis_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_1_TEMP_DIS_bm)) | ((Value << MPU60X0_PWR_MGMT_1_TEMP_DIS_bp) & MPU60X0_PWR_MGMT_1_TEMP_DIS_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_cycle_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_1_CYCLE_bm)) | ((Value << MPU60X0_PWR_MGMT_1_CYCLE_bp) & MPU60X0_PWR_MGMT_1_CYCLE_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_sleep_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_1_SLEEP_bm)) | ((Value << MPU60X0_PWR_MGMT_1_SLEEP_bp) & MPU60X0_PWR_MGMT_1_SLEEP_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_device_reset_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_1;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_1_DEVICE_RESET_bm)) | ((Value << MPU60X0_PWR_MGMT_1_DEVICE_RESET_bp) & MPU60X0_PWR_MGMT_1_DEVICE_RESET_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_stby_zg_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_2_STBY_ZG_bm)) | ((Value << MPU60X0_PWR_MGMT_2_STBY_ZG_bp) & MPU60X0_PWR_MGMT_2_STBY_ZG_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_stby_yg_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_2_STBY_YG_bm)) | ((Value << MPU60X0_PWR_MGMT_2_STBY_YG_bp) & MPU60X0_PWR_MGMT_2_STBY_YG_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_stby_xg_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_2_STBY_XG_bm)) | ((Value << MPU60X0_PWR_MGMT_2_STBY_XG_bp) & MPU60X0_PWR_MGMT_2_STBY_XG_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_stby_za_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_2_STBY_ZA_bm)) | ((Value << MPU60X0_PWR_MGMT_2_STBY_ZA_bp) & MPU60X0_PWR_MGMT_2_STBY_ZA_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_stby_ya_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_2_STBY_YA_bm)) | ((Value << MPU60X0_PWR_MGMT_2_STBY_YA_bp) & MPU60X0_PWR_MGMT_2_STBY_YA_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_stby_xa_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_2_STBY_XA_bm)) | ((Value << MPU60X0_PWR_MGMT_2_STBY_XA_bp) & MPU60X0_PWR_MGMT_2_STBY_XA_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_lp_wake_ctrl_set(Twi_t *TwiStruct, unsigned char IcNr, unsigned char Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_PWR_MGMT_2;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_PWR_MGMT_2_LP_WAKE_CTRL_gm)) | ((Value << MPU60X0_PWR_MGMT_2_LP_WAKE_CTRL_gp) & MPU60X0_PWR_MGMT_2_LP_WAKE_CTRL_gm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_i2c_bypass_enable_set(Twi_t *TwiStruct, unsigned char IcNr, bool Value)
{
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_INT_PIN_CFG;
	if(!SetupI2CReception(TwiStruct, 1, 1)) return false;
	TwiStruct->TxBuff[1] = (TwiStruct->RxBuff[0] & ~(MPU60X0_INT_PIN_CFG_I2C_BYPASS_EN_bm)) | ((Value << MPU60X0_INT_PIN_CFG_I2C_BYPASS_EN_bp) & MPU60X0_INT_PIN_CFG_I2C_BYPASS_EN_bm);
	if(!SetupI2CTransmit(TwiStruct, 2)) return false;
	return true;
}

bool mpu60x0_gyroscope_test(Twi_t *TwiStruct, unsigned char IcNr, double *XtestResult, double *YtestResult, double *ZtestResult)
{
	signed short Xdata = 0;
	signed short Ydata = 0;
	signed short Zdata = 0;
	signed short _Xdata = 0;
	signed short _Ydata = 0;
	signed short _Zdata = 0;
	mpu60x0_gyro_data_get(TwiStruct, IcNr, &Xdata, &Ydata, &Zdata);
	mpu60x0_gyro_self_test_set(TwiStruct, IcNr, true, true, true);
	Sysdelay(100);
	mpu60x0_gyro_data_get(TwiStruct, IcNr, &_Xdata, &_Ydata, &_Zdata);
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_SELF_TEST_X;
	if(!SetupI2CReception(TwiStruct, 1, 3)) return false;
	double XG_TEST = (double)(TwiStruct->RxBuff[0] & 0x1F);
	double YG_TEST = (double)(TwiStruct->RxBuff[1] & 0x1F);
	double ZG_TEST = (double)(TwiStruct->RxBuff[2] & 0x1F);
	mpu60x0_gyro_self_test_set(TwiStruct, IcNr, false, false, false);
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

bool mpu60x0_accelerometer_test(Twi_t *TwiStruct, unsigned char IcNr, double *XtestResult, double *YtestResult, double *ZtestResult)
{
	signed short Xdata = 0;
	signed short Ydata = 0;
	signed short Zdata = 0;
	signed short _Xdata = 0;
	signed short _Ydata = 0;
	signed short _Zdata = 0;
	mpu60x0_accel_data_get(TwiStruct, IcNr, &Xdata, &Ydata, &Zdata);
	mpu60x0_accel_self_test_set(TwiStruct, IcNr, true, true, true);
	Sysdelay(100);
	mpu60x0_accel_data_get(TwiStruct, IcNr, &_Xdata, &_Ydata, &_Zdata);
	TwiStruct->MasterSlaveAddr = MPU60x0_ADDR | (IcNr & 0x01);
	TwiStruct->TxBuff[0] = MPU60X0_SELF_TEST_X;
	if(!SetupI2CReception(TwiStruct, 1, 4)) return false;
	double XA_TEST = (double)(((TwiStruct->RxBuff[0] >> 4) & 0x0C) | ((TwiStruct->RxBuff[3] >> 4) & 0x03));
	double YA_TEST = (double)(((TwiStruct->RxBuff[1] >> 4) & 0x0C) | ((TwiStruct->RxBuff[3] >> 2) & 0x03));
	double ZA_TEST = (double)(((TwiStruct->RxBuff[2] >> 4) & 0x0C) | ((TwiStruct->RxBuff[3] >> 0) & 0x03));

	mpu60x0_accel_self_test_set(TwiStruct, IcNr, false, false, false);
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

bool mpu60x0_gyroscope_test_display_result(Twi_t *TwiStruct, unsigned char IcNr)
{
	double XtestResult = 0;
	double YtestResult = 0;
	double ZtestResult = 0;
	if(!mpu60x0_gyroscope_test(TwiStruct, IcNr , &XtestResult, &YtestResult, &ZtestResult)) return false;
	double XtestResultInt = 0;
	double XtestResultDec = modf(((double)XtestResult), &XtestResultInt);
	double YtestResultInt = 0;
	double YtestResultDec = modf(((double)YtestResult), &YtestResultInt);
	double ZtestResultInt = 0;
	double ZtestResultDec = modf(((double)ZtestResult), &ZtestResultInt);
	UARTprintf(DebugCom, "Giroscope calibration value:\n\rXg = %d.%u, Yg = %d.%u, Zg = %d.%u\n\r", (signed int)XtestResultInt, (unsigned int)(XtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(YtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(ZtestResultDec * 10000));
	return true;
}

bool mpu60x0_accelerometer_test_display_result(Twi_t *TwiStruct, unsigned char IcNr)
{
	double XtestResult = 0;
	double YtestResult = 0;
	double ZtestResult = 0;
	if(!mpu60x0_accelerometer_test(TwiStruct, IcNr , &XtestResult, &YtestResult, &ZtestResult)) return false;
	double XtestResultInt = 0;
	double XtestResultDec = modf(((double)XtestResult), &XtestResultInt);
	double YtestResultInt = 0;
	double YtestResultDec = modf(((double)YtestResult), &YtestResultInt);
	double ZtestResultInt = 0;
	double ZtestResultDec = modf(((double)ZtestResult), &ZtestResultInt);
	UARTprintf(DebugCom, "Accelerometer calibration value:\n\rXa = %d.%u, Ya = %d.%u, Za = %d.%u\n\r", (signed int)XtestResultInt, (unsigned int)(XtestResultDec * 10000), (signed int)YtestResultInt, (unsigned int)(YtestResultDec * 10000), (signed int)ZtestResultInt, (unsigned int)(ZtestResultDec * 10000));
	return true;
}

bool mpu60x0_temperature_display_result(Twi_t *TwiStruct, unsigned char IcNr)
{
	float Temperature = 0;
	if(!mpu60x0_temp_data_get(TwiStruct, IcNr , &Temperature)) return false;
#ifndef _TINY_PRINT_
	UARTprintf(DebugCom, "Temperature:\n\r%f Gr Celsius\n\r", Temperature);
#else
	float GrCelsius = 0;
	float GrCelsiusMod = modff(Temperature, &GrCelsius);
	UARTprintf(DebugCom, "Temperature:\n\r%d.%u Gr Celsius\n\r", (unsigned int)GrCelsius, (unsigned int)(GrCelsiusMod*1000));
#endif
	return true;
}

bool mpu60x0_giroscope_display_result(Twi_t *TwiStruct, unsigned char IcNr)
{
	signed short Xg = 0;
	signed short Yg = 0;
	signed short Zg = 0;
	if(!mpu60x0_gyro_data_get(TwiStruct, IcNr, &Xg, &Yg, &Zg)) return false;
	UARTprintf(DebugCom, "Giroscope:\n\rXg = %d, Yg = %d, Zg = %d\n\r", Xg, Yg, Zg);
	return true;
}

bool mpu60x0_accelerometer_display_result(Twi_t *TwiStruct, unsigned char IcNr)
{
	signed short Xa = 0;
	signed short Ya = 0;
	signed short Za = 0;
	if(!mpu60x0_accel_data_get(TwiStruct, IcNr, &Xa, &Ya, &Za)) return false;
	UARTprintf(DebugCom, "Accelerometer:\n\rXa = %d, Ya = %d, Za = %d\n\r", Xa, Ya, Za);
	return true;
}

bool mpu60x0_init(Twi_t *TwiStruct, unsigned char IcNr)
{
	if(!mpu60x0_who_am_i_get(TwiStruct, IcNr)) return false;
	mpu60x0_sleep_set(TwiStruct, IcNr, false);
	mpu60x0_clock_sel_set(TwiStruct, IcNr, MPU60X0_PWR_MGMT_1_CLKSEL_PLL_X_AX);
	mpu60x0_gyro_full_scale_set(TwiStruct, IcNr, MPU60X0_GYRO_CONFIG_FS_SEL_2000);
	mpu60x0_i2c_bypass_enable_set(TwiStruct, IcNr, true);
	return true;
}
