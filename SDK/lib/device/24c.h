/*
 *  lib/device/24c.h
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

#ifndef TWI_EEP_API_H_
#define TWI_EEP_API_H_

#include <stdbool.h>
#include "../api/twi_api.h"


bool E2promRead(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned long Addr, unsigned char *Buff, unsigned int DataLen);
bool E2promWrite(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned long Addr, unsigned char *Buff, unsigned int DataLen);
bool E2promRead_max16kb(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned int Addr, unsigned char *Buff, unsigned int DataLen);
bool E2promWrite_max16kb(new_twi* TwiStruct, unsigned char MasterSlaveAddr, unsigned int Addr, unsigned char *Buff, unsigned int DataLen);

#ifdef HEADER_INCLUDE_C_FILES
#include "24c.c"
#endif
#endif /* TWI_EEP_API_H_ */
