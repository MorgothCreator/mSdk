/*
 * uart.h
 *
 * Created: 2/8/2013 2:29:34 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef UART_H_
#define UART_H_
//#####################################################
#include <avr/io.h>
#include <stdio.h>
#include "api/uart_def.h"
//#####################################################
#ifndef Usart_Int_Disabled
#define Usart_Int_Disabled			(0)
#endif
#ifndef Usart_Int_Low_Level
#define Usart_Int_Low_Level			(1)
#endif
#ifndef Usart_Int_Med_Level
#define Usart_Int_Med_Level			(2)
#endif
#ifndef Usart_Int_High_Level
#define Usart_Int_High_Level		(3)
#endif
//-----------------------------------------------------
#define Usart_Clk2X_False			(0)
#define Usart_Clk2X_True			(1)
//-----------------------------------------------------
#define Usart_Mode_Async			(0)
#define Usart_Mode_Sync				(1)
#define Usart_Mode_Irda				(2)
#define Usart_Mode_Spi				(3)
//-----------------------------------------------------
#define Usart_Parity_No				(0)
#define Usart_Parity_Even			(2)
#define Usart_Parity_Odd			(3)
//-----------------------------------------------------
#define Usart_StopBit_1				(0)
#define Usart_StopBit_2				(1)
//-----------------------------------------------------
#define Usart_CharSize_5			(0)
#define Usart_CharSize_6			(1)
#define Usart_CharSize_7			(2)
#define Usart_CharSize_8			(3)
#define Usart_CharSize_9			(7)
//#####################################################
#define Usart_Rx_BuffOvf			(0)
#define Usart_Tx_Complete			(1)
#define UsartDefaultBautRate		(19200)
#define UsartDefaultTimeout			(100)
//-----------------------------------------------------
#define Usart_Init					(0)
#define Usart_Rx_Int				(1)
#define Usart_Dre_Int				(2)
#define Usart_SendBuffer			(3)
#define Usart_ReceiveAndCompare_En	(4)
#define Usart_ReceiveBuff_Clear		(5)
//#####################################################
/*#####################################################*/
bool UartOpen(Uart_t* UsartStruct);
void UartClose(Uart_t* UsartStruct);
void uart_set_baud_rate(unsigned int port_base_addr, unsigned long baud_rate);
unsigned int UARTOperatingModeSelect(unsigned int baseAdd, unsigned int modeFlag);
unsigned int UARTRegConfigModeEnable(unsigned int baseAdd, unsigned int modeFlag);
void UARTLineCharacConfig(unsigned int baseAdd, unsigned int wLenStbFlag, unsigned int parityFlag);
void UARTParityModeSet(unsigned int baseAdd, unsigned int parityFlag);
unsigned int UARTCharPutNonBlocking(unsigned int baseAdd, unsigned char byteWrite);
signed char UARTCharGetNonBlocking(unsigned int baseAdd);
signed char UARTCharGet(unsigned int baseAdd);
void UARTCharPut(unsigned int baseAdd, unsigned char byteTx);
unsigned int UARTRxErrorGet(unsigned int baseAdd);
unsigned char usart_spi_data(Uart_t *UsartStruct, unsigned char Data);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "uart.c"
#endif
/*#####################################################*/
#endif /* UART_H_ */
/*#####################################################*/
