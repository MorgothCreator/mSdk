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
#include "gpio_def.h"
/*#####################################################*/
typedef struct
{
	unsigned char OldCsSelect;
	unsigned char CsSelect;
	unsigned char MosiPin;
	unsigned char MisoPin;
	unsigned char SckPin;
	unsigned char CsPin[4];
	unsigned int ClkDiv[4];
	gpio_port_enum MosiPort;
	gpio_port_enum MisoPort;
	gpio_port_enum SckPort;
	gpio_port_enum CsPort[4];
	Gpio_t *Gpio_Mosi;
	Gpio_t *Gpio_Miso;
	Gpio_t *Gpio_Sck;
	Gpio_t *Gpio_Cs[4];
	volatile bool interrupted;
	//volatile unsigned char Channel;
	volatile unsigned int BaseAddr;
	//volatile unsigned long BaudRate;
	//volatile unsigned int BuffSize;
	//volatile unsigned int numOfBytes;
	volatile unsigned char McspiNr;
	volatile unsigned char flag;
	volatile unsigned char Priority;
	volatile unsigned char Mode;//_SpiModeRisingSample, _SpiModeRisingSetup, _SpiModeFallingSample, _SpiModeFallingSetup
	volatile unsigned int BytesCnt;
	void *UserData;
	//volatile unsigned char* Buff;
	//volatile unsigned char* BuffTmp;
	bool DisableCsHandle;
	bool Cpol;
	bool Cpha;
	bool LsbFirst;
	unsigned char WordSize;
	bool Slave;
}Mcspi_t;
/*#####################################################*/
typedef struct spi_s
{
	bool (*open)(Mcspi_t *McspiStruct);
	void (*close)(Mcspi_t *McspiStruct);
	void (*assert)(Mcspi_t *McspiStruct);
	void (*deassert)(Mcspi_t *McspiStruct);
	bool (*set_baud)(Mcspi_t *McspiStruct, unsigned long baud);
	unsigned char (*trx_byte)(Mcspi_t *McspiStruct, unsigned char byte);
	bool (*transfer)(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned char *buff_receive, unsigned int size);
	bool (*receive)(Mcspi_t *McspiStruct, unsigned char *buff_receive, unsigned int bytes_receive);
	bool (*transmit)(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned int bytes_send);
}spi_t;
/*#####################################################*/
#define new_mcspi Mcspi_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_mcspi(address) free(address);
/*#####################################################*/
#endif /* MCSPI_DEF_H_ */
/*#####################################################*/
