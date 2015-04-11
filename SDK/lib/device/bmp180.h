/*
 *  lib/device/bmp180.h
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

/*#####################################################*/
#ifndef BMP180_H_
#define BMP180_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/twi_def.h"
/*#####################################################*/
#define BMP180_ADDR					(0x77)

#define BMP180_CALIB				0xAA
#define BMP180_ID					0xD0
#define BMP180_SOFT_RESET			0xE0
#define BMP180_CTRL_MEAS			0xF4
#define BMP180_OUT_MSB				0xF6
#define BMP180_OUT_LSB				0xF7
#define BMP180_OUT_XLSB				0xF8

/*
 * CTRL_MEAS reg
 */
#define BMP180_CTRL_MEAS_OSS_gp		6
#define BMP180_CTRL_MEAS_OSS_gm		(0x03 << BMP180_CTRL_MEAS_OSS_gp)
#define BMP180_CTRL_MEAS_OSS_1		(0)
#define BMP180_CTRL_MEAS_OSS_2		(1)
#define BMP180_CTRL_MEAS_OSS_4		(2)
#define BMP180_CTRL_MEAS_OSS_8		(3)

#define BMP180_CTRL_MEAS_SCO_bp		5
#define BMP180_CTRL_MEAS_SCO_bm		(0x01 << BMP180_CTRL_MEAS_SCO_bp)

/*
 * SOFT_RESET reg
 */
#define BMP180_SOFT_RESET_gp		0
#define BMP180_SOFT_RESET_gm		(0xFF << BMP180_CTRL_MEAS_SCO_bp)
#define BMP180_SOFT_RESET_value		0xB6

/*
 * CALIB reg's
 */
#define BMP180_CALIB_LEN			22

/*#####################################################*/
typedef struct BMP180_s{
	//STimer_t Timeout_Timer;
	bool busy;
	Twi_t* TWI;
	unsigned char IcNr;
	unsigned char Stage;
	struct {
		signed short AC1;
		signed short AC2;
		signed short AC3;
		unsigned short AC4;
		unsigned short AC5;
		unsigned short AC6;
		signed short B1;
		signed short B2;
		signed short MB;
		signed short MC;
		signed short MD;
	}cal_data;
}BMP180_t;

bool bmp180_read_calibration(BMP180_t *structure);
bool bmp180_read_temp(BMP180_t *structure, signed long *temp);
bool bmp180_read_pressure(BMP180_t *structure, signed long *pressure, unsigned char oss);
bool bmp180_display_result(BMP180_t *structure, unsigned char oss);
bool bmp180_get_temp(BMP180_t *structure, float *temp);
bool bmp180_get_pressure(BMP180_t *structure, float *pressure, unsigned char oss);
bool bmp180_get_altitude(BMP180_t *structure, float *altitude, unsigned char oss);

/*#####################################################*/
#define new_bmp180 BMP180_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_bmp180(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "bmp180.c"
#endif

/*#####################################################*/
#endif
/*#####################################################*/
