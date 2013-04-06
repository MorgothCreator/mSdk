/*
 * twi_interface.c
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include <stdbool.h>
#include "twi_interface.h"
#include "api/twi_def.h"
#include "driver/twi.h"
#include "driver/gpio.h"
/*#####################################################*/
/*
** Transmits data over I2C bus
*/
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	twi_package_t packet;
	// TWI chip address to communicate with
	packet.chip = TwiStruct->MasterSlaveAddr;
	// TWI address/commands to issue to the other chip (node)
	packet.addr = 0;
	// Length of the TWI data address segment (1-3 bytes)
	packet.addr_length = 0;
	// Where to find the data to be written
	packet.buffer = TwiStruct->TxBuff;
	// How many bytes do we want to write
	packet.length = TransmitBytes;
	// perform a write access
	if(twi_master_write(&AVR32_TWI, &packet) != TWI_SUCCESS) return false;

	return true;
}
/*#####################################################*/
/*
** Receives data over I2C bus
*/
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	twi_package_t packet_received;
	// TWI chip address to communicate with
	packet_received.chip = TwiStruct->MasterSlaveAddr ;
	// Length of the TWI data address segment (1-3 bytes)
	packet_received.addr_length = TransmitBytes;
	// How many bytes do we want to write
	packet_received.length = ReceiveBytes;
	// TWI address/commands to issue to the other chip (node)
	switch(TransmitBytes)
	{
	case 0:
		packet_received.addr = 0;
		break;
	case 1:
		packet_received.addr = TwiStruct->TxBuff[0];
		break;
	case 2:
		packet_received.addr = TwiStruct->TxBuff[0] | (TwiStruct->TxBuff[1] << 8);
		break;
	case 3:
		packet_received.addr = TwiStruct->TxBuff[0] | (TwiStruct->TxBuff[1] << 8) | (TwiStruct->TxBuff[2] << 16);
		break;
	default:
		return false;
	}
	// Where to find the data to be written
	packet_received.buffer = TwiStruct->RxBuff;

	// perform a read access
	if(twi_master_read(&AVR32_TWI, &packet_received) != TWI_SUCCESS) return false;
	return true;
}
/*#####################################################*/
bool _twi_open(new_twi* TwiStruct)
{
	unsigned char *RxBuff = NULL;
	unsigned char *TxBuff = NULL;
	if(TwiStruct->RxBuffSize)
	{
		RxBuff = calloc(1, TwiStruct->RxBuffSize);
		if(RxBuff == NULL) return false;
		TwiStruct->RxBuff = RxBuff;
	}
	if(TwiStruct->TxBuffSize)
	{
		TxBuff = calloc(1, TwiStruct->TxBuffSize);
		if(TxBuff == NULL )
		{
			if(RxBuff) free(RxBuff);
			return false;
		}
		TwiStruct->TxBuff = TxBuff;
	}

	IO_SetFnct(AVR32_TWI_SDA_0_PIN, AVR32_TWI_SDA_0_FUNCTION);
	IO_SetFnct(AVR32_TWI_SCL_0_PIN, AVR32_TWI_SCL_0_FUNCTION);

    twi_options_t opt;
    opt.pba_hz = Get_PBA_Clk();
    opt.speed = TwiStruct->BaudRate;
    opt.chip = TwiStruct->MasterSlaveAddr;

    // initialize TWI driver with options
    int status = twi_master_init(&AVR32_TWI, &opt);
    // check init result
    if (status != TWI_SUCCESS)
    {
		if(RxBuff) free(RxBuff);
		if(TxBuff) free(TxBuff);
		return false;
    }
	return true;
}
/*#####################################################*/
void _twi_close(new_twi* TwiStruct)
{
}
/*#####################################################*/


