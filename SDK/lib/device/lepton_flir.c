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
#include <string.h>
#include "board_init.h"
#include "lepton_flir.h"
#include "api/gpio_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "api/mcspi_def.h"
#include "api/mcspi_api.h"
#include "api/timer_api.h"
#include "interface/mcspi_interface.h"

const unsigned short ccitt_16Table[] = {
   0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
   0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
   0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
   0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
   0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
   0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
   0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
   0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
   0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
   0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
   0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
   0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
   0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
   0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
   0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
   0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
   0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
   0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
   0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
   0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
   0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
   0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
   0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
   0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
   0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
   0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
   0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
   0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
   0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
   0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
   0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
   0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/*
 *  ===== ByteCRC16 =====
 *      Calculate (update) the CRC16 for a single 8-bit byte
 */
int ByteCRC16(int value, int crcin)
{
	return (unsigned short)((crcin << 8) ^  ccitt_16Table[((crcin >> 8) ^ (value)) & 255]);
}

/*
 *  ===== CalcCRC16Words =====
 *      Calculate the CRC for a buffer of 16-bit words.  Supports both
 *  Little and Big Endian formats using conditional compilation.
 *      Note: minimum count is 1 (0 case not handled)
 */
unsigned short CalcCRC16Words(unsigned int count, short *buffer) {

    int crc = 0;

    do {

        int value = *buffer++;
#ifdef _BIG_ENDIAN
        crc = ByteCRC16(value >> 8, crc);
        crc = ByteCRC16(value, crc);
#else
        crc = ByteCRC16(value, crc);
        crc = ByteCRC16(value >> 8, crc);
#endif
    }
	while (--count);
    return (unsigned short) crc;
}

/*
 *  ===== CalcCRC16Bytes =====
 *      Calculate the CRC for a buffer of 8-bit words.
 *      Note: minimum count is 1 (0 case not handled)
 */
unsigned short CalcCRC16Bytes(unsigned int count, char *buffer) {

    int crc = 0;

    do {

        int value = *buffer++;
        crc = ByteCRC16(value, crc);
    }
	while (--count);
    return crc;
}

bool lepton_run_command(LEPTON_FLIR_t *structure, unsigned short commandID) {
	if(structure == NULL || !structure->TWI)
		return false;
    unsigned short statusReg;
    signed short statusCode;
    bool done;
    unsigned short timeoutCount = LEPTON_I2C_COMMAND_BUSY_WAIT_COUNT;

	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = LEPTON_FLIR_ADDR;
	/*
	 * Implement the Lepton TWI WRITE Protocol
	 */
	/*
	 * First wait until the Camera is ready to receive a new
	 * command by polling the STATUS REGISTER BUSY Bit until it
	 * reports NOT BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_STATUS_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_STATUS_REG;
		if(!SetupI2CReception(TwiStruct, 2, 2))
			return false;
		statusReg = (TwiStruct->RxBuff[0] << 8) | TwiStruct->RxBuff[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		sys_delay(2);
	}while( !done );
	/*
	 * Set the Lepton's DATA LENGTH REGISTER first to inform the
	 * Lepton Camera no 16-bit DATA words being transferred.
	 */
	TwiStruct->TxBuff[0] = LEP_I2C_DATA_LENGTH_REG >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_LENGTH_REG;
	if(!SetupI2CTransmit(TwiStruct, 2))
		return false;

	/*
	 * Now issue the Run Command
	 */
	TwiStruct->TxBuff[0] = LEP_I2C_COMMAND_REG >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_COMMAND_REG;
	TwiStruct->TxBuff[2] = commandID >> 8;
	TwiStruct->TxBuff[3] = (unsigned char)commandID;
	if(!SetupI2CTransmit(TwiStruct, 4))
		return false;
	/*
	 * Now wait until the Camera has completed this command by
	 * polling the statusReg REGISTER BUSY Bit until it reports NOT
	 * BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_STATUS_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_STATUS_REG;
		if(!SetupI2CReception(TwiStruct, 2, 2))
			return false;
		statusReg = (TwiStruct->RxBuff[0] << 8) | TwiStruct->RxBuff[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		sys_delay(2);
	}while( !done );
    statusCode = (statusReg >> 8) ? ((statusReg >> 8) | 0xFF00) : 0;
    if(statusCode)
    {
      return false;
    }
    return true;
}


bool lepton_reg_write(LEPTON_FLIR_t *structure, unsigned short commandID, unsigned short *attributePtr, unsigned short attributeWordLength) {
	if(structure == NULL || !structure->TWI)
		return false;
    unsigned short statusReg;
    signed short statusCode;
    bool done;
    unsigned short timeoutCount = LEPTON_I2C_COMMAND_BUSY_WAIT_COUNT;

	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = LEPTON_FLIR_ADDR;
	/*
	 * Implement the Lepton TWI WRITE Protocol
	 */
	/*
	 * First wait until the Camera is ready to receive a new
	 * command by polling the STATUS REGISTER BUSY Bit until it
	 * reports NOT BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_STATUS_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_STATUS_REG;
		if(!SetupI2CReception(TwiStruct, 2, 2))
			return false;
		statusReg = (TwiStruct->RxBuff[0] << 8) | TwiStruct->RxBuff[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		sys_delay(2);
	}while( !done );

	/*
	 * Now WRITE the DATA to the DATA REGISTER(s)
	 */
	memcpy(TwiStruct->TxBuff + 2, attributePtr, attributeWordLength * 2);
	if( attributeWordLength <= 16 )
	{
		/*
		 * WRITE to the DATA Registers - always start from DATA 0
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_DATA_0_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_0_REG;
		if(!SetupI2CTransmit(TwiStruct, (attributeWordLength * 2) + 2))
			return false;
	}
	else if( attributeWordLength <= 1024 )
	{
		/*
		 * WRITE to the DATA Block Buffer
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_DATA_BUFFER_0 >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_BUFFER_0;
		if(!SetupI2CTransmit(TwiStruct, (attributeWordLength * 2) + 2))
			return false;
	}
	else {
		return false;
	}
    /*
     * Set the Lepton's DATA LENGTH REGISTER first to inform the
     * Lepton Camera how many 16-bit DATA words we want to read.
     */
	TwiStruct->TxBuff[0] = LEP_I2C_DATA_LENGTH_REG >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_LENGTH_REG;
	TwiStruct->TxBuff[2] = attributeWordLength >> 8;
	TwiStruct->TxBuff[3] = (unsigned char)attributeWordLength;
	if(!SetupI2CTransmit(TwiStruct, 4))
		return false;
	/*
	 * Now issue the SET Attribute Command
	 */
	TwiStruct->TxBuff[0] = LEP_I2C_COMMAND_REG >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_COMMAND_REG;
	TwiStruct->TxBuff[2] = commandID >> 8;
	TwiStruct->TxBuff[3] = (unsigned char)commandID;
	if(!SetupI2CTransmit(TwiStruct, 4))
		return false;
	/*
	 * Now wait until the Camera has completed this command by
	 * polling the statusReg REGISTER BUSY Bit until it reports NOT
	 * BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_STATUS_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_STATUS_REG;
		if(!SetupI2CReception(TwiStruct, 2, 2))
			return false;
		statusReg = (TwiStruct->RxBuff[0] << 8) | TwiStruct->RxBuff[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		sys_delay(2);
	}while( !done );
	/*
	 * Check statusReg word for Errors?
	 */
	statusCode = (statusReg >> 8) ? ((statusReg >> 8) | 0xFF00) : 0;
	if(statusCode)
		return false;

	return true;

}

bool lepton_reg_read(LEPTON_FLIR_t *structure, unsigned short commandID, unsigned short *attributePtr, unsigned short attributeWordLength) {
	if(structure == NULL || !structure->TWI)
		return false;
    unsigned short statusReg;
    signed short statusCode;
    bool done;
    unsigned short crcExpected, crcActual;
    unsigned short timeoutCount = LEPTON_I2C_COMMAND_BUSY_WAIT_COUNT;

	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = LEPTON_FLIR_ADDR;


	/*
	 * Implement the Lepton TWI WRITE Protocol
	 */
	/*
	 * First wait until the Camera is ready to receive a new
	 * command by polling the STATUS REGISTER BUSY Bit until it
	 * reports NOT BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_STATUS_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_STATUS_REG;
		if(!SetupI2CReception(TwiStruct, 2, 2))
			return false;
		statusReg = (TwiStruct->RxBuff[0] << 8) | TwiStruct->RxBuff[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		sys_delay(2);
	}while( !done );
    /*
     * Set the Lepton's DATA LENGTH REGISTER first to inform the
     * Lepton Camera how many 16-bit DATA words we want to read.
     */
	TwiStruct->TxBuff[0] = LEP_I2C_DATA_LENGTH_REG >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_LENGTH_REG;
	TwiStruct->TxBuff[2] = attributeWordLength >> 8;
	TwiStruct->TxBuff[3] = attributeWordLength;
	if(!SetupI2CTransmit(TwiStruct, 3))
		return false;
    /*
     * Now issue the GET Attribute Command
     */
	TwiStruct->TxBuff[0] = LEP_I2C_COMMAND_REG >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_COMMAND_REG;
	TwiStruct->TxBuff[2] = commandID >> 8;
	TwiStruct->TxBuff[3] = commandID;
	if(!SetupI2CTransmit(TwiStruct, 4))
		return false;
	/*
	 * Now wait until the Camera has completed this command by
	 * polling the statusReg REGISTER BUSY Bit until it reports NOT
	 * BUSY.
	 */
	do
	{
		/*
		 * Read the Status REGISTER and peek at the BUSY Bit
		 */
		TwiStruct->TxBuff[0] = LEP_I2C_STATUS_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_STATUS_REG;
		if(!SetupI2CReception(TwiStruct, 2, 2))
			return false;
		statusReg = (TwiStruct->RxBuff[0] << 8) | TwiStruct->RxBuff[1];
		done = (statusReg & LEP_I2C_STATUS_BUSY_BIT_MASK)? false : true;
		/*
		 * Add timout check
		 */
		if( timeoutCount-- == 0 )
		{
		/*
		 * Timed out waiting for command busy to go away
		 */
		return false;
		}
		sys_delay(2);
	}while( !done );
	/*
	 * Check statusReg word for Errors?
	 */
	statusCode = (statusReg >> 8) ? ((statusReg >> 8) | 0xFF00) : 0;
	if(statusCode)
	{
		return false;
	}
    /*
     * If NO Errors then READ the DATA from the DATA REGISTER(s)
    */
    if( attributeWordLength <= 16 )
    {
    	/*
    	 * Read from the DATA Registers - always start from DATA 0
    	 * Little Endean
    	 */
    	TwiStruct->TxBuff[0] = LEP_I2C_DATA_0_REG >> 8;
    	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_0_REG;
		if(!SetupI2CReception(TwiStruct, 2, attributeWordLength * 2))
			return false;
    }
    else if( attributeWordLength <= 1024 )
    {
    	/*
    	 * Read from the DATA Block Buffer
    	 */
    	TwiStruct->TxBuff[0] = LEP_I2C_DATA_BUFFER_0 >> 8;
    	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_BUFFER_0;
    	if(!SetupI2CReception(TwiStruct, 2, attributeWordLength * 2))
			return false;
    }
	memcpy(attributePtr, (void *)TwiStruct->RxBuff, attributeWordLength * 2);
    if(attributeWordLength > 0)
    {
    	/*
    	 * Check CRC
    	 */
		TwiStruct->TxBuff[0] = LEP_I2C_DATA_CRC_REG >> 8;
		TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_CRC_REG;
		if(!SetupI2CReception(TwiStruct, 1, 2))
			return false;
		crcExpected = (TwiStruct->RxBuff[0] << 8) | TwiStruct->RxBuff[1];
		crcActual = (unsigned short)CalcCRC16Words(attributeWordLength, (short*)attributePtr);

		/*
		 * Check for 0 in the register in case the camera does not support CRC check
         */
		if(crcExpected != 0 && crcExpected != crcActual)
		{
			return false;
		}
    }
    return true;
}

bool lepton_flir_get_image(LEPTON_FLIR_t *structure, unsigned short *image) {
	if(structure == NULL || structure->SPI == NULL || image == NULL)
		return false;
	Sysdelay(200);
	unsigned char line_buff[LEPTON_FLIR_LINE_SIZE];
	unsigned char checkByte = 0x0F;
	unsigned int packet_nr = 0;
	structure->SPI->Buff = line_buff;
	// loop while discard packets
	while((checkByte & 0x0F) == 0x0f) {
		structure->SPI->CsSelect = structure->spi_instance;
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
		structure->SPI->CsSelect = structure->spi_instance;
		mcspi_transfer(structure->SPI, 0, LEPTON_FLIR_LINE_SIZE);
		packet_nr = structure->SPI->Buff[1];
	}

	return true;
}

bool lepton_direct_write_buff(LEPTON_FLIR_t *structure, unsigned short *attributePtr, unsigned short attributeWordLength) {
	if(!structure || !structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = LEPTON_FLIR_ADDR;
	/*
	 * Read from the DATA Block Buffer
	 */
	TwiStruct->TxBuff[0] = LEP_I2C_DATA_BUFFER_0 >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)LEP_I2C_DATA_BUFFER_0;
	memcpy(TwiStruct->TxBuff + 2, attributePtr, attributeWordLength * 2);
	if(!SetupI2CTransmit(TwiStruct, (attributeWordLength * 2) + 2))
		return false;
	return true;
}

bool lepton_direct_write_reg(LEPTON_FLIR_t *structure, unsigned short regAddress, unsigned short regValue) {
	if(!structure || !structure->TWI)
		return false;
	Twi_t *TwiStruct = structure->TWI;
	TwiStruct->MasterSlaveAddr = LEPTON_FLIR_ADDR;
	/*
	 * Read from the DATA Block Buffer
	 */
	TwiStruct->TxBuff[0] = regAddress >> 8;
	TwiStruct->TxBuff[1] = (unsigned char)regAddress;
	TwiStruct->TxBuff[2] = regValue >> 8;
	TwiStruct->TxBuff[3] = (unsigned char)regValue;
	if(!SetupI2CTransmit(TwiStruct, 4))
		return false;
	return true;
}

bool lepton_flir_init(LEPTON_FLIR_t *structure) {
	return true;
}



