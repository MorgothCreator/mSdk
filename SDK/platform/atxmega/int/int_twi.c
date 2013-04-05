/*
 * int_twi.c
 *
 * Created: 2/8/2013 5:13:49 PM
 *  Author: XxXx
 */ 

//#####################################################
#include <avr/io.h>
#include "int_twi.h"
#include "interface/twi_interface.h"
//-----------------------------------------------------
Twi_Int_Service_t TWI_ServiceAddresses;
//#####################################################
/*! \brief TWI master arbitration lost and bus error interrupt handler.
 *
 *  Handles TWI responses to lost arbitration and bus error.
 *
 *  \param twi  The TWI_Master_t struct instance.
 */
void TWI_MasterTransactionFinished(Twi_t *twi, unsigned char result)
{
	twi->result = result;
	twi->status = TWIM_STATUS_READY;
}

void TWI_MasterWriteHandler(Twi_t *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->BaseAddr;
	/* Local variables used in if tests to avoid compiler warning. */
	unsigned char bytesToWrite  = twi->bytesToWrite;
	unsigned char bytesToRead   = twi->bytesToRead;

	/* If NOT acknowledged (NACK) by slave cancel the transaction. */
	if (ModuleAddr->MASTER.STATUS & TWI_MASTER_RXACK_bm) {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		twi->result = TWIM_RESULT_NACK_RECEIVED;
		twi->status = TWIM_STATUS_READY;
	}

	/* If more bytes to write, send data. */
	else if (twi->tCount < bytesToWrite) {
		unsigned char data = twi->TxBuff[twi->tCount];
		ModuleAddr->MASTER.DATA = data;
		++twi->tCount;
	}

	/* If bytes to read, send repeated START condition + Address +
	 * 'R/_W = 1'
	 */
	else if (twi->rCount < bytesToRead) {
		unsigned char readAddress = (twi->MasterSlaveAddr<<1) | 0x01;
		ModuleAddr->MASTER.ADDR = readAddress;
	}

	/* If transaction finished, send STOP condition and set RESULT OK. */
	else {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_OK);
	}
}

/*! \brief TWI master read interrupt handler.
 *
 *  This is the master read interrupt handler that takes care of
 *  reading bytes from the TWI slave.
 *
 *  \param twi The TWI_Master_t struct instance.
 */
void TWI_MasterReadHandler(Twi_t *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->BaseAddr;
	/* Fetch data if bytes to be read. */
	if (twi->rCount < twi->RxBuffSize) {
		unsigned char data = ModuleAddr->MASTER.DATA;
		twi->RxBuff[twi->rCount] = data;
		twi->rCount++;
	}

	/* If buffer overflow, issue STOP and BUFFER_OVERFLOW condition. */
	else {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_BUFFER_OVERFLOW);
	}

	/* Local variable used in if test to avoid compiler warning. */
	unsigned char bytesToRead = twi->bytesToRead;

	/* If more bytes to read, issue ACK and start a byte read. */
	if (twi->rCount < bytesToRead) {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
	}

	/* If transaction finished, issue NACK and STOP condition. */
	else {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_ACKACT_bm |
		                               TWI_MASTER_CMD_STOP_gc;
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_OK);
	}
}

void TWI_MasterArbitrationLostBusErrorHandler(Twi_t *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->BaseAddr;
	unsigned char currentStatus = ModuleAddr->MASTER.STATUS;

	/* If bus error. */
	if (currentStatus & TWI_MASTER_BUSERR_bm) {
		twi->result = TWIM_RESULT_BUS_ERROR;
	}
	/* If arbitration lost. */
	else {
		twi->result = TWIM_RESULT_ARBITRATION_LOST;
	}

	/* Clear interrupt flag. */
	ModuleAddr->MASTER.STATUS = currentStatus | TWI_MASTER_ARBLOST_bm;

	twi->status = TWIM_STATUS_READY;
}

void TWI_MasterInterruptHandler(Twi_t *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->BaseAddr;
	unsigned char currentStatus = ModuleAddr->MASTER.STATUS;

	/* If arbitration lost or bus error. */
	if ((currentStatus & TWI_MASTER_ARBLOST_bm) ||
	    (currentStatus & TWI_MASTER_BUSERR_bm)) {

		TWI_MasterArbitrationLostBusErrorHandler(twi);
	}

	/* If master write interrupt. */
	else if (currentStatus & TWI_MASTER_WIF_bm) {
		TWI_MasterWriteHandler(twi);
	}

	/* If master read interrupt. */
	else if (currentStatus & TWI_MASTER_RIF_bm) {
		TWI_MasterReadHandler(twi);
	}

	/* If unexpected state. */
	else {
		TWI_MasterTransactionFinished(twi, TWIM_RESULT_FAIL);
	}
}
//#####################################################
//#####################################################
//############ Interrupt vectors insertion ############
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
#ifdef use_twi0
		ISR(TWIC_TWIM_vect)
		{
			TWI_MasterInterruptHandler(TWI_ServiceAddresses.Twi0);
		}
 		/*/*ISR(TWIC_TWIS_vect)
 		{
 			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI0RamServiceAddr);
 		}*/
#endif
//-----------------------------------------------------
#ifdef use_twi1
		ISR(TWID_TWIM_vect)
		{
			TWI_MasterInterruptHandler(TWI_ServiceAddresses.Twi1);
		}
		/*ISR(TWID_TWIS_vect)
		{
			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI1RamServiceAddr);
		}*/
#endif
//-----------------------------------------------------
#ifdef use_twi2
		ISR(TWIE_TWIM_vect)
		{
			TWI_MasterInterruptHandler(TWI_ServiceAddresses.Twi2);
		}
		/*ISR(TWIE_TWIS_vect)
		{
			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI2RamServiceAddr);
		}*/
#endif
//-----------------------------------------------------
#ifdef use_twi3
		ISR(TWIF_TWIM_vect)
		{
			TWI_MasterInterruptHandler(TWI_ServiceAddresses.Twi3);
		}
		/*ISR(TWIF_TWIS_vect)
		{
			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI3RamServiceAddr);
		}*/
#endif
//#####################################################
