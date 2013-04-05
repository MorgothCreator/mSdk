/*
 * int_uart.c
 *
 * Created: 2/8/2013 1:34:22 PM
 *  Author: XxXx
 */ 

#include <avr/io.h>
#include "int_uart.h"
#include "api/uart_def.h"
#include "lib/buffers/ring_buff.h"
//#####################################################
Uart_Int_Service_t Uart_Int_Service;
//#####################################################
//#####################################################
//############## Rx interrupt service     #############
//#####################################################
//#####################################################
void usart_rx_int(Uart_t *UsartStruct)
{
	if(UsartStruct ==NULL) return;
	USART_t *ModuleAddress = (USART_t *)UsartStruct->BaseAddr;
	char Data = ModuleAddress->DATA;
	fifo_push((fifo_settings_t *)UsartStruct->RxBuff, Data);
	if (Data == '\r')
	{
		UsartStruct->NrOfPocketsInBuff++;
		//asm("SEI");
		if(UsartStruct->OnRxPocketReceived) UsartStruct->OnRxPocketReceived(UsartStruct->udata);
	}
}
//#####################################################
//#####################################################
//############## Tx interrupt service     #############
//#####################################################
//#####################################################
void usart_tx_int(Uart_t *UsartStruct)
{
	if(UsartStruct ==NULL) return;
	fifo_pop_return_t FifoReturn;
	USART_t *ModuleAddress = (USART_t *)UsartStruct->BaseAddr;
	FifoReturn = fifo_pop((fifo_settings_t *)UsartStruct->TxBuff);
	if(FifoReturn.status == false)
	{
		ModuleAddress->CTRLA &= (~USART_DREINTLVL_gm);
		if(UsartStruct->OnTxComplete != 0) UsartStruct->OnTxComplete(UsartStruct->udata);
	}
	else ModuleAddress->DATA = FifoReturn.character;
}
//#####################################################
//#####################################################
//#####################################################
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
