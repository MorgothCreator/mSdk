/*
 * twi_interface.c
 *
 * Created: 2/8/2013 5:00:27 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#include "twi_interface.h"
#include "driver/twi.h"
//#include "int/int_twi.h"
//#####################################################
/*! \brief TWI write and/or read transaction.
 *
 *  This function is a TWI Master write and/or read transaction. The function
 *  can be used to both write and/or read bytes to/from the TWI Slave in one
 *  transaction.
 *
 *  \param twi            The TWI_Master_t struct instance.
 *  \param address        The slave address.
 *  \param writeData      Pointer to data to write.
 *  \param bytesToWrite   Number of bytes to write.
 *  \param bytesToRead    Number of bytes to read.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
uint8_t TWI_MasterWriteRead(Twi_t *twi, uint8_t bytesToWrite, uint8_t bytesToRead)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->BaseAddr;
	/*Parameter sanity check. */
	if (bytesToWrite > twi->TxBuffSize) 
	{
		return false;
	}
	if (bytesToRead > twi->RxBuffSize) 
	{
		return false;
	}

	/*Initiate transaction if bus is ready. */
	if (twi->status == TWIM_STATUS_READY) 
	{

		twi->status = TWIM_STATUS_BUSY;
		twi->result = TWIM_RESULT_UNKNOWN;

		//twi->DeviceAddress = DeviceAddress<<1;

		/* Fill write data buffer. */
		//memcpy(twi->, DataToWrite, bytesToWrite);
		//for (uint8_t bufferIndex=0; bufferIndex < bytesToWrite; bufferIndex++) 
		//{
		//	twi->WriteBuff[bufferIndex] = DataToWrite[bufferIndex];
		//}

		twi->bytesToWrite = bytesToWrite;
		twi->bytesToRead = bytesToRead;
		twi->tCount = 0;
		twi->rCount = 0;

		/* If write command, send the START condition + Address +
		 * 'R/_W = 0'
		 */
		if (twi->bytesToWrite > 0) {
			uint8_t writeAddress = (twi->MasterSlaveAddr<<1) & ~0x01;
			ModuleAddr->MASTER.ADDR = writeAddress;
		}

		/* If read command, send the START condition + Address +
		 * 'R/_W = 1'
		 */
		else if (twi->bytesToRead > 0) {
			uint8_t readAddress = (twi->MasterSlaveAddr<<1) | 0x01;
			ModuleAddr->MASTER.ADDR = readAddress;
		}

		unsigned int timeout = 1000;
		while(twi->status != TWIM_STATUS_READY && --timeout)
		{
			_delay_us(100);
			asm("WDR");
		}
		if(timeout == 0) 
			TWI_reset(twi);
	if(twi->result != TWIM_RESULT_OK || timeout == 0) return false;
	return true;
	} 
	else 
	{
		return false;
	}
}
/*#####################################################*/
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	return TWI_MasterWriteRead(TwiStruct, TransmitBytes, 0);
}
/*#####################################################*/
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	return TWI_MasterWriteRead(TwiStruct, TransmitBytes, ReceiveBytes);
}
/*#####################################################*/
bool _twi_open(new_twi* TwiStruct)
{
	return TWI_open(TwiStruct);
}
/*#####################################################*/
void _twi_close(new_twi* TwiStruct)
{
	TWI_close(TwiStruct);
}
/*#####################################################*/
