/*
 *  lib/api/mcspi_api.h
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

#ifndef MCSPI_API_H_
#define MCSPI_API_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/mcspi_def.h"
//#include "aintc/aintc_mcspi.h"
//#include "clk/clk_mcspi.h"
#include "interface/mcspi_interface.h"
/*#####################################################*/
bool mcspi_transfer(Mcspi_t *McspiStruct, unsigned int NumOfBytesSend, unsigned int NumOfBytesReceive);
bool mcspi_open(new_mcspi *McspiStruct);
void mcspi_close(new_mcspi *McspiStruct);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_api.c"
#endif
/*#####################################################*/
#endif /* MCSPI_API_H_ */
/*#####################################################*/
