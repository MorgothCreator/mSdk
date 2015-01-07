/*
 *  lib/device/mhc5883.h
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
/*#####################################################*/
#ifndef MHC5883_H_
#define MHC5883_H_
/*#####################################################*/
#include "stdbool.h"
#include "api/twi_def.h"
#include "api/timer_api.h"
/*#####################################################*/
#define MHC5883_ADDR								(0x1E)

#define MHC5883_ID_A								(0x48)
#define MHC5883_ID_B								(0x34)
#define MHC5883_ID_C								(0x33)

#define MHC5883_CONFIG_REG_A						(0)
#define MHC5883_CONFIG_REG_B						(1)
#define MHC5883_MODE_REG							(2)
#define MHC5883_DATA_OUT_X_H						(3)
#define MHC5883_DATA_OUT_X_L						(4)
#define MHC5883_DATA_OUT_Z_H						(5)
#define MHC5883_DATA_OUT_Z_L						(6)
#define MHC5883_DATA_OUT_Y_H						(7)
#define MHC5883_DATA_OUT_Y_L						(8)
#define MHC5883_STATUS_REG							(9)
#define MHC5883_IDENT_REG_A							(10)
#define MHC5883_IDENT_REG_B							(11)
#define MHC5883_IDENT_REG_C							(12)

#define MHC5883_CONFIG_REG_A_MA_gp					(5)
#define MHC5883_CONFIG_REG_A_MA_gm					(0x03 << MHC5883_CONFIG_REG_A_MA_gp)
#define MHC5883_CONFIG_REG_A_MA_1					(0)
#define MHC5883_CONFIG_REG_A_MA_2					(1)
#define MHC5883_CONFIG_REG_A_MA_4					(2)
#define MHC5883_CONFIG_REG_A_MA_8					(3)

#define MHC5883_CONFIG_REG_A_DO_gp					(2)
#define MHC5883_CONFIG_REG_A_DO_gm					(0x07 << MHC5883_CONFIG_REG_A_DO_gp)
#define MHC5883_CONFIG_REG_A_DO_0_75				(0)
#define MHC5883_CONFIG_REG_A_DO_1_5					(1)
#define MHC5883_CONFIG_REG_A_DO_3					(2)
#define MHC5883_CONFIG_REG_A_DO_7_5					(3)
#define MHC5883_CONFIG_REG_A_DO_15					(4)
#define MHC5883_CONFIG_REG_A_DO_30					(5)
#define MHC5883_CONFIG_REG_A_DO_75					(6)

#define MHC5883_CONFIG_REG_A_MS_gp					(0)
#define MHC5883_CONFIG_REG_A_MS_gm					(0x03 << MHC5883_CONFIG_REG_A_MS_gp)
#define MHC5883_CONFIG_REG_A_MS_NORMAL				(0)
#define MHC5883_CONFIG_REG_A_MS_POSITIVE_BIAS		(1)
#define MHC5883_CONFIG_REG_A_MS_NEGATIVE_BIAS		(2)

#define MHC5883_CONFIG_REG_B_GN_gp					(5)
#define MHC5883_CONFIG_REG_B_GN_gm					(0x07 << MHC5883_CONFIG_REG_B_GN_gp)
#define MHC5883_CONFIG_REG_B_GN_1370				(0)
#define MHC5883_CONFIG_REG_B_GN_1090				(1)
#define MHC5883_CONFIG_REG_B_GN_820					(2)
#define MHC5883_CONFIG_REG_B_GN_660					(3)
#define MHC5883_CONFIG_REG_B_GN_440					(4)
#define MHC5883_CONFIG_REG_B_GN_390					(5)
#define MHC5883_CONFIG_REG_B_GN_330					(6)
#define MHC5883_CONFIG_REG_B_GN_230					(7)

#define MHC5883_MODE_REG_MD_gp						(0)
#define MHC5883_MODE_REG_MD_gm						(0x03 << MHC5883_MODE_REG_MD_gp)
#define MHC5883_MODE_REG_MD_CONTINUOUS				(0)
#define MHC5883_MODE_REG_MD_SINGLE					(1)
#define MHC5883_MODE_REG_MD_IDLE					(2)

#define MHC5883_STATUS_REG_RDY_bp					(0)
#define MHC5883_STATUS_REG_RDY_bm					(0x01 << MHC5883_STATUS_REG_RDY_bp)

#define MHC5883_STATUS_REG_LOCK_bp					(1)
#define MHC5883_STATUS_REG_LOCK_bm					(0x01 << MHC5883_STATUS_REG_LOCK_bp)
/*#####################################################*/
typedef struct MHC5883_s{
	STimer_t Timeout_Timer;
	//MHC5883_COMMANDS reg_inst;
	bool busy;
	Twi_t* TWI;
}MHC5883_t;
/*#####################################################*/
bool mhc5883_sample_nr_set(MHC5883_t *structure, unsigned char Value);
bool mhc5883_data_output_rate_set(MHC5883_t *structure, unsigned char Value);
bool mhc5883_measurament_mode_set(MHC5883_t *structure, unsigned char Value);
bool mhc5883_gain_set(MHC5883_t *structure, unsigned char Value);
bool mhc5883_mode_set(MHC5883_t *structure, unsigned char Value);
bool mhc5883_data_get(MHC5883_t *structure, signed short *X_Axis, signed short *Y_Axis, signed short *Z_Axis);
bool mhc5883_display_result(MHC5883_t *structure);
bool mhc5883_display_positive_test_result(MHC5883_t *structure);
bool mhc5883_display_negative_test_result(MHC5883_t *structure);
bool mhc5883_init(MHC5883_t *structure);
/*#####################################################*/
#define new_mhc5883 MHC5883_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_mhc5883(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mhc5883.c"
#endif
/*#####################################################*/
#endif /* MHC5883_H_ */
/*#####################################################*/
