/*
 *  lib/api/pmic_api.c
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

#include "pmic_api.h"
/*#####################################################*/
#include "board_properties.h"
#include "api/twi_api.h"
#ifdef USE_TPS65217
#include "device/tps65217.h"
#endif
#ifdef USE_TPS65910A
#include "device/tps65910a.h"
#endif
/*#####################################################*/
void pmic_setup(new_twi* TwiStruct)
{
#if defined(USE_TPS65217) || defined(USE_TPS65910A)
	_pmic_config(TwiStruct);
#endif
}
/*#####################################################*/
void pmic_read(new_twi* TwiStruct,unsigned char regOffset, unsigned char* dest)
{
#if defined(USE_TPS65217) || defined(USE_TPS65910A)
	_pmic_reg_read(TwiStruct, regOffset, dest);
#endif
}
/*#####################################################*/
void pmic_write(new_twi* TwiStruct,unsigned char port_level, unsigned char regOffset,unsigned char dest_val, unsigned char mask)
{
#if defined(USE_TPS65217) || defined(USE_TPS65910A)
	_pmic_reg_write(TwiStruct, port_level, regOffset, dest_val, mask);
#endif
}
/*#####################################################*/
void pmic_backlight_enable(new_twi* TwiStruct)
{
#if defined(USE_TPS65217) || defined(USE_TPS65910A)
	_pmic_wled_enable(TwiStruct);
#endif
}
/*#####################################################*/
void pmic_backlight_level(new_twi* TwiStruct, unsigned char level)
{
#if defined(USE_TPS65217) || defined(USE_TPS65910A)
	_pmic_wled_level(TwiStruct, level);
#endif
}
/*#####################################################*/


