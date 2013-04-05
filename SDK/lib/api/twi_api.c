/*
 *  lib/api/twi_api.c
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
#include "twi_api.h"
#include "twi_def.h"
#include "interface/twi_interface.h"
/*#####################################################*/
bool SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	return _SetupI2CTransmit(TwiStruct, TransmitBytes);
}
/*#####################################################*/
bool SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	return _SetupI2CReception(TwiStruct, TransmitBytes, ReceiveBytes);
}
/*#####################################################*/
bool twi_open(new_twi* TwiStruct)
{
	return _twi_open(TwiStruct);
}
/*#####################################################*/
void twi_close(new_twi* TwiStruct)
{
	_twi_close(TwiStruct);
}
/*#####################################################*/
