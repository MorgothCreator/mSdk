/*
 *  lib/device/lepton_flir.c
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

#include <stdbool.h>
#include <stdlib.h>
#include "board_init.h"
#include "lepton_flir.h"
#include "gpio_interface.h"
#include "api/twi_def.h"
#include "api/mcspi_def.h"
#include "api/mcspi_api.h"
#include "api/timer_api.h"
#include "interface/mcspi_interface.h"

#include "board_init.h"

bool lepton_flir_init(LEPTON_FLIR_t *structure) {
	return true;
}

bool lepton_flir_get_image(LEPTON_FLIR_t *structure, unsigned short *image) {
	Sysdelay(200);
	unsigned char line_buff[LEPTON_FLIR_LINE_SIZE];
	unsigned char checkByte = 0x0F;
	unsigned int packet_nr;
	structure->SPI->Buff = line_buff;
	// loop while discard packets
	while((checkByte & 0x0F) == 0x0f) {
		mcspi_transfer(structure->SPI, 0, LEPTON_FLIR_LINE_SIZE);
		checkByte = structure->SPI->Buff[0];
		packet_nr = structure->SPI->Buff[1];
	}

	// sync done, first packet is ready, store packets
	while(packet_nr < 60)
	{
		// ignore discard packets
		if((structure->SPI->Buff[0] & 0x0f) != 0x0f){
			memcpy(&image[packet_nr * 80], (void *)&structure->SPI->Buff[4], 160);
		}
		// read next packet
		mcspi_transfer(structure->SPI, 0, LEPTON_FLIR_LINE_SIZE);
		packet_nr = structure->SPI->Buff[1];
	}

	return true;
}

