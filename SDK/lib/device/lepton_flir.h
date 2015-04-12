/*
 *  lib/device/lepton_flir.h
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
#ifndef LEPTON_FLIR_H_
#define LEPTON_FLIR_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/twi_def.h"
#include "api/mcspi_def.h"
#include "api/mcspi_api.h"
#include "interface/mcspi_interface.h"
/*#####################################################*/
#define LEPTON_FLIR_ADDR					(0x2A)

#define LEPTON_FLIR_LINE_SIZE				164
#define LEPTON_FLIR_LINES_NR				60
/*#####################################################*/
typedef struct LEPTON_FLIR_s{
	//STimer_t Timeout_Timer;
	bool busy;
	Mcspi_t* SPI;
	unsigned char CsNr;
	//unsigned char Stage;
}LEPTON_FLIR_t;
/*#####################################################*/
bool lepton_flir_init(LEPTON_FLIR_t *structure);
bool lepton_flir_get_image(LEPTON_FLIR_t *structure, unsigned short *image);
/*#####################################################*/
#define new_lepton_flir LEPTON_FLIR_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_lepton_flir(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "lepton_flir.c"
#endif

/*#####################################################*/
#endif
/*#####################################################*/
