/*
 *  lib/device/ms5611.h
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
#ifndef MS5611_H_
#define MS5611_H_
/*#####################################################*/
#include "api/twi_def.h"
#include "api/timer_api.h"
/*#####################################################*/
#define MS5611_ADDR								(0x77)

#define MS5611_ADC_READ_CMD						(0x00)
#define MS5611_RESET_CMD						(0x1E)
#define MS5611_CONVERT_D1_OSR_256_CMD			(0x40)
#define MS5611_CONVERT_D1_OSR_512_CMD			(0x42)
#define MS5611_CONVERT_D1_OSR_1024_CMD			(0x44)
#define MS5611_CONVERT_D1_OSR_2048_CMD			(0x46)
#define MS5611_CONVERT_D1_OSR_4096_CMD			(0x48)
#define MS5611_CONVERT_D2_OSR_256_CMD			(0x50)
#define MS5611_CONVERT_D2_OSR_512_CMD			(0x52)
#define MS5611_CONVERT_D2_OSR_1024_CMD			(0x54)
#define MS5611_CONVERT_D2_OSR_2048_CMD			(0x56)
#define MS5611_CONVERT_D2_OSR_4096_CMD			(0x58)
#define MS5611_CONVERT_PROM_READ_C1_CMD			(0xA0)
#define MS5611_CONVERT_PROM_READ_C2_CMD			(0xA2)
#define MS5611_CONVERT_PROM_READ_C3_CMD			(0xA4)
#define MS5611_CONVERT_PROM_READ_C4_CMD			(0xA6)
#define MS5611_CONVERT_PROM_READ_C5_CMD			(0xA8)
#define MS5611_CONVERT_PROM_READ_C6_CMD			(0xAA)
#define MS5611_CONVERT_PROM_READ_C7_CMD			(0xAC)
#define MS5611_CONVERT_PROM_READ_C8_CMD			(0xAE)

#define MS5611_CONVERT_OSR_256					(0x0)
#define MS5611_CONVERT_OSR_512					(0x1)
#define MS5611_CONVERT_OSR_1024					(0x2)
#define MS5611_CONVERT_OSR_2048					(0x3)
#define MS5611_CONVERT_OSR_4096					(0x4)
/*#####################################################*/
typedef struct
{
	unsigned short C0;
	unsigned short C1;
	unsigned short C2;
	unsigned short C3;
	unsigned short C4;
	unsigned short C5;
	unsigned short C6;
	unsigned short C7;
}ms5611_prom_data;
/*#####################################################*/
typedef struct MS5611_s{
	STimer_t Timeout_Timer;
	//MS5611_COMMANDS reg_inst;
	bool prom_data_has_been_read;
	ms5611_prom_data prom_data;
	bool busy;
	Twi_t* TWI;
}MS5611_t;
/*#####################################################*/
bool ms5611_read(MS5611_t *structure, unsigned char osr, signed int *Preasure, signed int *Temperature);
bool ms5611_display_pressure_result(MS5611_t *structure, unsigned char osr);
/*#####################################################*/
#define new_ms5611 MS5611_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_ms5611(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "ms5611.c"
#endif
/*#####################################################*/
#endif /* MS5611_H_ */
/*#####################################################*/
