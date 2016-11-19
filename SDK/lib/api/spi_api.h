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
#include <api/spi_def.h>
#include <stdbool.h>
#include "interface/mcspi_interface.h"
/*#####################################################*/
extern const spi_t spi;

//void mcspi_assert(Mcspi_t *McspiStruct);
//void mcspi_deassert(Mcspi_t *McspiStruct);
//bool mcspi_transfer(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned char *buff_receive, unsigned int size);
//bool mcspi_receive(Mcspi_t *McspiStruct, unsigned char *buff_receive, unsigned int bytes_receive);
//bool mcspi_transmit(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned int bytes_send);
//bool mcspi_open(Mcspi_t *McspiStruct);
//void mcspi_close(Mcspi_t *McspiStruct);
//unsigned char mcspi_send_byte(Mcspi_t *McspiStruct, unsigned char byte);
//bool mcspi_set_baud(Mcspi_t *McspiStruct, unsigned long baud);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_api.c"
#endif
/*#####################################################*/
#endif /* MCSPI_API_H_ */
/*#####################################################*/
