/*
 *  lib/device/hih613x.h
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
#ifndef HIH613x_H_
#define HIH613x_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/twi_def.h"
/*#####################################################*/
#define HIH613x_ADDR					(0x27)

#define HIH613x_LOCKED_EEP				0x00
#define HIH613x_LOCKED_EEP_LEN			0x16

#define HIH613x_ALARM_HIGH_ON			0x18
#define HIH613x_ALARM_HIGH_OFF			0x19
#define HIH613x_ALARM_LOW_ON			0x1A
#define HIH613x_ALARM_LOW_OFF			0x1B
#define HIH613x_CUST_CONF				0x1C
#define HIH613x_CUST_ID					0x1F

#define HIH613x_CUST_CONF_I2C_ADDR_gp	0
#define HIH613x_CUST_CONF_I2C_ADDR_gm	(0x3F << HIH613x_CUST_CONF_I2C_ADDR_gp)

#define HIH613x_CUST_CONF_AL_LOW_PIN_gp	7
#define HIH613x_CUST_CONF_AL_LOW_PIN_gm	(0x03 << HIH613x_CUST_CONF_AL_LOW_PIN_gp)
#define HIH613x_CUST_CONF_AL_LOW_PIN_ACT_HI_PP	0
#define HIH613x_CUST_CONF_AL_LOW_PIN_ACT_HI_OD	1
#define HIH613x_CUST_CONF_AL_LOW_PIN_ACT_LO_PP	2
#define HIH613x_CUST_CONF_AL_LOW_PIN_ACT_LO_OD	3

#define HIH613x_CUST_CONF_AL_HI_PIN_gp	9
#define HIH613x_CUST_CONF_AL_HI_PIN_gm	(0x03 << HIH613x_CUST_CONF_AL_HI_PIN_gp)
#define HIH613x_CUST_CONF_AL_HI_PIN_ACT_HI_PP	0
#define HIH613x_CUST_CONF_AL_HI_PIN_ACT_HI_OD	1
#define HIH613x_CUST_CONF_AL_HI_PIN_ACT_LO_PP	2
#define HIH613x_CUST_CONF_AL_HI_PIN_ACT_LO_OD	3

#define HIH613x_CUST_CONF_FAST_ST_bp	13
#define HIH613x_CUST_CONF_FAST_ST_bm	(0x3F << HIH613x_CUST_CONF_FAST_ST_bp)
/*#####################################################*/
typedef struct HIH613x_s{
	//STimer_t Timeout_Timer;
	bool busy;
	Twi_t* TWI;
	unsigned char Stage;
}HIH613x_t;
/*#####################################################*/
bool hih613x_get_hum_temp(HIH613x_t *structure, unsigned char *status, unsigned short *hum, unsigned short *temp);
/*#####################################################*/
#define new_hih613x HIH613x_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_hih613x(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "hih6130.c"
#endif

/*#####################################################*/
#endif
/*#####################################################*/
