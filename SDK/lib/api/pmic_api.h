/*
 *  lib/api/pmic_api.h
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

#ifndef PMIC_API_H_
#define PMIC_API_H_
/*#####################################################*/
#include "twi_api.h"
/*#####################################################*/
void pmic_setup(new_twi* TwiStruct);
void pmic_read(new_twi* TwiStruct,unsigned char regOffset, unsigned char* dest);
void pmic_write(new_twi* TwiStruct,unsigned char port_level, unsigned char regOffset,unsigned char dest_val, unsigned char mask);
void pmic_backlight_enable(new_twi* TwiStruct);
void pmic_backlight_level(new_twi* TwiStruct, unsigned char level);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "pmic_api.c"
#endif
/*#####################################################*/
#endif /* PMIC_API_H_ */
/*#####################################################*/
