/*
 *  lib/api/mcspi_api.c
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
#include "mcspi_api.h"
//#include "aintc/aintc_mcspi.h"
//#include "clk/clk_mcspi.h"
#include "interface/mcspi_interface.h"
/*#####################################################*/
bool mcspi_open(new_mcspi *McspiStruct)
{
	return _mcspi_open(McspiStruct);
}
/*#####################################################*/
void mcspi_close(new_mcspi *McspiStruct)
{
	_mcspi_close(McspiStruct);
}
/*#####################################################*/
bool mcspi_transfer(Mcspi_t *McspiStruct, unsigned int NumOfBytesSend, unsigned int NumOfBytesReceive)
{
	McspiStruct->numOfBytes = NumOfBytesSend + NumOfBytesReceive;
	_mcspi_transfer(McspiStruct);
	return true;
}
/*#####################################################*/

