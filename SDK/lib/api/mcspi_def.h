/*
 *  lib/api/mcspi_def.h
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

#ifndef MCSPI_DEF_H_
#define MCSPI_DEF_H_
/*#####################################################*/
#include <stdbool.h>
/*#####################################################*/
typedef struct
{
	unsigned int D0Pin;
	unsigned int D1Pin;
	unsigned int ClkPin;
	unsigned int Cs0Pin;
	unsigned int Cs1Pin;
	unsigned int Cs2Pin;
	unsigned int Cs3Pin;
	volatile bool interrupted;
	volatile unsigned char Channel;
	volatile unsigned int BaseAddr;
	volatile unsigned long BaudRate;
	volatile unsigned int BuffSize;
	volatile unsigned int numOfBytes;
	volatile unsigned char McspiNr;
	volatile unsigned char flag;
	volatile unsigned char Priority;
	volatile unsigned char Mode;//_SpiModeRisingSample, _SpiModeRisingSetup, _SpiModeFallingSample, _SpiModeFallingSetup
	volatile unsigned int BytesCnt;
	void *UserData;
	volatile unsigned char* Buff;
	volatile unsigned char* BuffTmp;
}Mcspi_t;
/*#####################################################*/
#define new_mcspi Mcspi_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_mcspi(address) free(address);
/*#####################################################*/
#endif /* MCSPI_DEF_H_ */
/*#####################################################*/
