/*
 *  lib/device/tps65217.h
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

#ifndef TPS65217_H_
#define TPS65217_H_
/*#####################################################*/
#include "sys/plat_properties.h"
#include "../api/twi_api.h"
#if (defined USE_TPS65217) || (defined beaglebone)
/*#####################################################*/
#define WLED_CURENT_SYNK_ENABLE_BITMASK	0x8
#define WLED_ISET_BITMASK				0x4
#define WLED_PWM_DIMMING_FREQ_100		0x0
#define WLED_PWM_DIMMING_FREQ_200		0x1
#define WLED_PWM_DIMMING_FREQ_500		0x2
#define WLED_PWM_DIMMING_FREQ_1000		0x3
/*#####################################################*/
void _pmic_reg_read(new_twi* TwiStruct,unsigned char regOffset, unsigned char* dest);
void _pmic_reg_write(new_twi* TwiStruct,unsigned char port_level, unsigned char regOffset,
                        unsigned char dest_val, unsigned char mask);
void pmic_voltage_update(new_twi* TwiStruct,unsigned char dc_cntrl_reg, unsigned char volt_sel);
void _pmic_config(new_twi* TwiStruct);
void _pmic_wled_enable(new_twi* TwiStruct);
void _pmic_wled_level(new_twi* TwiStruct, unsigned char level);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "tps65217.c"
#endif
/*#####################################################*/
#endif
#endif /* TPS65217_H_ */
/*#####################################################*/
