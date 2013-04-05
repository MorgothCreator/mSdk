/*
 * uart.c
 *
 * Created: 2/8/2013 2:29:44 AM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "uart.h"
#include "api/uart_def.h"
#include "general/unions.h"
#include "lib/buffers/ring_buff.h"
#include "driver/core.h"
#include "int/int_uart.h"
//#####################################################
extern Uart_Int_Service_t Uart_Int_Service;
extern unsigned long CoreFreq;
//#####################################################
//#####################################################
//#####################################################
//######    Init function  ############################
//#####################################################
//#####################################################
//#####################################################
USART_t* Usart_GetSetBaseAddress(Uart_t *UsartStruct)
{
	if (UsartStruct->Mode == UsartCom_Mode_Spi && UsartStruct->CS_Port != 0)
	{
		PORT_t *Cs_Port = (PORT_t *)UsartStruct->CS_Port;
		Cs_Port->DIRSET = UsartStruct->CS_PinMask;
		Cs_Port->OUTSET = UsartStruct->CS_PinMask;
	}
	USART_t* Temp = 0;
	switch(UsartStruct->UartNr)
	{
#ifdef use_usart0
		case(0):
			PORTC.DIRSET = 1<<3;
			PORTC.OUTSET = 1<<3;
			PORTC.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTC.DIRSET = 1<<1;
				PORTC.OUTSET = 1<<1;
				USARTC0.CTRLC &= ~(1<<1);
			}
			PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart0 = UsartStruct;
			Temp = &USARTC0;
			break;
#endif
#ifdef use_usart1
		case(1):
			PORTC.DIRSET = 1<<7;
			PORTC.OUTSET = 1<<7;
			PORTC.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTC.DIRSET = 1<<5;
				PORTC.OUTSET = 1<<5;
				USARTC1.CTRLC &= ~(1<<1);
			}
			PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart1 = UsartStruct;
			Temp = &USARTC1;
			break;
#endif
#ifdef use_usart2
		case(2):
			PORTD.DIRSET = 1<<3;
			PORTD.OUTSET = 1<<3;
			PORTD.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTD.DIRSET = 1<<1;
				PORTD.OUTSET = 1<<1;
				USARTD0.CTRLC &= ~(1<<1);
			}
			PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart2 = UsartStruct;
			Temp = &USARTD0;
			break;
#endif
#ifdef use_usart3
		case(3):
			PORTD.DIRSET = 1<<7;
			PORTD.OUTSET = 1<<7;
			PORTD.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTD.DIRSET = 1<<5;
				PORTD.OUTSET = 1<<5;
				USARTD1.CTRLC &= ~(1<<1);
			}
			PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart3 = UsartStruct;
			Temp = &USARTD1;
			break;
#endif
#ifdef PORTE
#ifdef use_usart4
		case(4):
			PORTE.DIRSET = 1<<3;
			PORTE.OUTSET = 1<<3;
			PORTE.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTE.DIRSET = 1<<1;
				PORTE.OUTSET = 1<<1;
				USARTE0.CTRLC &= ~(1<<1);
			}
			PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart4 = UsartStruct;
			Temp = &USARTE0;
			break;
#endif
#ifdef use_usart5
		case(5):
			PORTE.DIRSET = 1<<7;
			PORTE.OUTSET = 1<<7;
			PORTE.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTE.DIRSET = 1<<5;
				PORTE.OUTSET = 1<<5;
				USARTE1.CTRLC &= ~(1<<1);
			}
			PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart5 = UsartStruct;
			Temp = &USARTE1;
			Return.SettingsReg = USARTE1_CTRLB;
			break;
#endif
#endif
#ifdef PORTF
#ifdef use_usart6
		case(6):
			PORTF.DIRSET = 1<<3;
			PORTF.OUTSET = 1<<3;
			PORTF.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTF.DIRSET = 1<<1;
				PORTF.OUTSET = 1<<1;
				USARTF0.CTRLC &= ~(1<<1);
			}
			PORTF.PIN2CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart6 = UsartStruct;
			Temp = &USARTF0;
			break;
#endif
#ifdef use_usart7
		case(7):
			PORTF.DIRSET = 1<<7;
			PORTF.OUTSET = 1<<7;
			PORTF.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTF.DIRSET = 1<<5;
				PORTF.OUTSET = 1<<5;
				USARTF1.CTRLC &= ~(1<<1);
			}
			PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;
			Uart_Int_Service.Uart7 = UsartStruct;
			Temp = &USARTF1;
			break;
#endif
#endif
	}
	UsartStruct->BaseAddr = (unsigned int)(void*)Temp;
	return Temp;
}
//#####################################################
bool UartOpen(Uart_t* UsartStruct)
{
	//struct Usart_Struct* UsartStruct = (struct Usart_Struct*)calloc(1, sizeof(struct Usart_Struct));
	UsartStruct->SelfFile = UsartStruct;
	USART_t* Return = Usart_GetSetBaseAddress(UsartStruct);
	if((uint16_t)UsartStruct != 0)
	{
		//struct Usart_Struct *UsartStruct = stream->udata;
		UsartStruct->Status = 0;
		uint16_t BaudCtrl = (CoreFreq/(8*UsartStruct->BaudRate))-1;
		convert16to8 _BaudRate;
		_BaudRate.i16 = BaudCtrl;
		Return->BAUDCTRLA = _BaudRate.Byte0;
		Return->BAUDCTRLB = _BaudRate.Byte1;
		//UsartStruct->BaudRate;
		uint8_t Temp = Return->CTRLA;
		Temp = (Temp&(~USART_RXCINTLVL_gm))|(((unsigned char)UsartStruct->Priority)<<USART_RXCINTLVL_gp);
		Temp = (Temp&(~USART_TXCINTLVL_gm))|(Usart_Int_Disabled<<USART_TXCINTLVL_gp);
		Temp = (Temp&(~USART_DREINTLVL_gm))|(Usart_Int_Disabled<<USART_DREINTLVL_gp);
		Return->CTRLA = Temp;
		Temp = Return->CTRLC;
		Temp = (Temp&(~USART_CMODE_gm))|(UsartStruct->Mode<<USART_CMODE_gp);
		Temp = (Temp&(~USART_PMODE_gm))|(Usart_Parity_No<<USART_PMODE_gp);
		Temp = (Temp&(~USART_SBMODE_bm))|(Usart_StopBit_1<<USART_SBMODE_bp);
		if (UsartStruct->Mode != UsartCom_Mode_Spi) Temp = (Temp&(~USART_CHSIZE_gm))|(Usart_CharSize_8<<USART_CHSIZE_gp);
		Return->CTRLC = Temp;
		Temp = Return->CTRLB;
		Temp = (Temp|(1<<USART_CLK2X_bp));
		Temp = (Temp|(0<<USART_MPCM_bp));
		Temp = (Temp|(1<<USART_RXEN_bp));
		Temp = (Temp|(1<<USART_TXEN_bp));
		Return->CTRLB = Temp;

		//ModuleAddress[3] = (ModuleAddress[3]&(~USART_RXCINTLVL_gm))|(_UsartCommand_Int_Med_Level<<USART_RXCINTLVL_gp);
		UsartStruct->Status = UsartStruct->Status&(~(1<<UsartCom_Rx_Buff_OVF|1<<UsartCom_Rx_Pocket));
		
		UsartStruct->NrOfPocketsInBuff = 0;
		if(UsartStruct->RxBuffSize) UsartStruct->RxBuff = (unsigned char*)fifo_open(UsartStruct->RxBuffSize);
		if(UsartStruct->TxBuffSize) UsartStruct->TxBuff = (unsigned char*)fifo_open(UsartStruct->TxBuffSize);
		if(((UsartStruct->RxBuff == 0) && (UsartStruct->RxBuffSize))  || ((UsartStruct->TxBuff == 0) && (UsartStruct->TxBuffSize)))
		{
			if(UsartStruct->RxBuffSize) if(UsartStruct->RxBuff) free(UsartStruct->RxBuff);
			if(UsartStruct->TxBuffSize) if(UsartStruct->TxBuff) free(UsartStruct->TxBuff);
			if(UsartStruct) free(UsartStruct);
			return false;
		}
		bool Lowlevel = false;
		bool Medlevel = false;
		bool Highlevel = false;
		if(UsartStruct->Priority == Usart_Int_Low_Level) Lowlevel = true;
		if(UsartStruct->Priority == Usart_Int_Med_Level) Medlevel = true;
		if(UsartStruct->Priority == Usart_Int_High_Level) Highlevel = true;
		
		int_lvl_enable(Lowlevel, Medlevel, Highlevel);
		if(UsartStruct->Priority != Usart_Int_Disabled) asm("SEI");
		return true;
	}
	else
	{
		free(UsartStruct);
		return false;
	}
}
//#####################################################
void UartClose(Uart_t* UsartStruct)
{
	if(UsartStruct == NULL) return;
	USART_t *UartPort = (USART_t *)UsartStruct->BaseAddr;
	UartPort->CTRLB &= ~USART_RXEN_bm;
	UartPort->CTRLB &= ~USART_TXEN_bm;
	switch(UsartStruct->UartNr)
	{
#ifdef use_usart0
		case(0):
			Uart_Int_Service.Uart0 = NULL;
			PORTC.DIRCLR = 1<<3;
			PORTC.OUTCLR = 1<<3;
			PORTC.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTC.DIRCLR = 1<<1;
				PORTC.OUTCLR = 1<<1;
			}
			break;
#endif
#ifdef use_usart1
		case(1):
			Uart_Int_Service.Uart1 = NULL;
			PORTC.DIRCLR = 1<<7;
			PORTC.OUTCLR = 1<<7;
			PORTC.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTC.DIRCLR = 1<<5;
				PORTC.OUTCLR = 1<<5;
			}
			break;
#endif
#ifdef use_usart2
		case(2):
			Uart_Int_Service.Uart2 = NULL;
			PORTD.DIRCLR = 1<<3;
			PORTD.OUTCLR = 1<<3;
			PORTD.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTD.DIRCLR = 1<<1;
				PORTD.OUTCLR = 1<<1;
			}
			break;
#endif
#ifdef use_usart3
		case(3):
			Uart_Int_Service.Uart3 = NULL;
			PORTD.DIRCLR = 1<<7;
			PORTD.OUTCLR = 1<<7;
			PORTD.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTD.DIRCLR = 1<<5;
				PORTD.OUTCLR = 1<<5;
			}
			break;
#endif
#ifdef PORTE
#ifdef use_usart4
		case(4):
			Uart_Int_Service.Uart4 = NULL;
			PORTE.DIRCLR = 1<<3;
			PORTE.OUTCLR = 1<<3;
			PORTE.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTE.DIRCLR = 1<<1;
				PORTE.OUTCLR = 1<<1;
			}
			break;
#endif
#ifdef use_usart5
		case(5):
			Uart_Int_Service.Uart5 = NULL;
			PORTE.DIRCLR = 1<<7;
			PORTE.OUTCLR = 1<<7;
			PORTE.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTE.DIRCLR = 1<<5;
				PORTE.OUTCLR = 1<<5;
			}
			break;
#endif
#endif
#ifdef PORTF
#ifdef use_usart6
		case(6):
			Uart_Int_Service.Uart6 = NULL;
			PORTF.DIRCLR = 1<<3;
			PORTF.OUTCLR = 1<<3;
			PORTF.DIRCLR = 1<<2;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTF.DIRCLR = 1<<1;
				PORTF.OUTCLR = 1<<1;
			}
			break;
#endif
#ifdef use_usart7
		case(7):
			Uart_Int_Service.Uart7 = NULL;
			PORTF.DIRCLR = 1<<7;
			PORTF.OUTCLR = 1<<7;
			PORTF.DIRCLR = 1<<6;
			if (UsartStruct->Mode == UsartCom_Mode_Spi)
			{
				PORTF.DIRCLR = 1<<5;
				PORTF.OUTCLR = 1<<5;
			}
			break;
#endif
#endif
	}
	if(UsartStruct->RxBuff) free(UsartStruct->RxBuff);
	if(UsartStruct->TxBuff) free(UsartStruct->TxBuff);
	free(UsartStruct);
}
//#####################################################
Uart_t * usart_get_struct_base_addr(unsigned int port_base_addr)
{
	switch (port_base_addr)
	{
#ifdef use_usart0
	case (unsigned int)(void*)&USARTC0:
		return Uart_Int_Service.Uart0;
#endif
#ifdef use_usart1
	case (unsigned int)(void*)&USARTC1:
		return Uart_Int_Service.Uart1;
#endif
#ifdef use_usart2
	case (unsigned int)(void*)&USARTD0:
		return Uart_Int_Service.Uart2;
#endif
#ifdef use_usart3
	case (unsigned int)(void*)&USARTD1:
		return Uart_Int_Service.Uart3;
#endif
#ifdef use_usart4
	case (unsigned int)(void*)&USARTE0:
		return Uart_Int_Service.Uart4;
#endif
#ifdef use_usart5
#ifdef USARTE1
	case (unsigned int)(void*)&USARTE1:
		return Uart_Int_Service.Uart5;
#endif
#endif
#ifdef use_usart6
#ifdef USARTF0
	case (unsigned int)(void*)&USARTF0:
		return Uart_Int_Service.Uart6;
#endif
#endif
#ifdef use_usart7
#ifdef USARTF1
	case (unsigned int)(void*)&USARTF1:
		return Uart_Int_Service.Uart7;
#endif
#endif
	default:
		return NULL;
	}
}
//#####################################################
void uart_set_baud_rate(unsigned int port_base_addr, unsigned long baud_rate)
{
	USART_t *ModuleAddress = (USART_t *)port_base_addr;
	Uart_t *UsartStruct = usart_get_struct_base_addr(port_base_addr);
	uint16_t BaudCtrl = (CoreFreq/(8*UsartStruct->BaudRate))-1;
	convert16to8 _BaudRate;
	_BaudRate.i16 = BaudCtrl;
	ModuleAddress->BAUDCTRLA = _BaudRate.Byte0;
	ModuleAddress->BAUDCTRLB = _BaudRate.Byte1;
}
//#####################################################
int usart_check_received_pocket(Uart_t *UsartStruct)
{
	if(UsartStruct == NULL) return false;
	if(UsartStruct->NrOfPocketsInBuff) return true;
	return false;
}
//#####################################################
int usart_get_char_pocket(Uart_t *UsartStruct)
{
	if(UsartStruct == NULL) return EOF;
	fifo_pop_return_t Return;
	Return.character = 0;
	if(UsartStruct->NrOfPocketsInBuff != 0)
	{
		Return = fifo_pop((fifo_settings_t *)UsartStruct->RxBuff);
		if(Return.character == '\r' || Return.status == false)
		{
			UsartStruct->NrOfPocketsInBuff--;
			Return.character = '\n';
		}
		return (int)Return.character;
	}
	else return EOF;
}
//#####################################################
signed char usart_get_char_nonblocking(unsigned int baseAdd)
{
	if(baseAdd == 0) return EOF;
	Uart_t *UsartStruct = usart_get_struct_base_addr(baseAdd);
	fifo_pop_return_t Return;
	Return.character = 0;
	Return = fifo_pop((fifo_settings_t *)UsartStruct->RxBuff);
	if(!Return.status) return -1;
	return (signed char)Return.character;
}
//#####################################################
signed char usart_get_char(unsigned int baseAdd)
{
	if(baseAdd == 0) return EOF;
	Uart_t *UsartStruct = usart_get_struct_base_addr(baseAdd);
	fifo_pop_return_t Return;
	do 
	{
		Return = fifo_pop((fifo_settings_t *)UsartStruct->RxBuff);
	} while (Return.status == false);
	return (signed char)Return.character;
}
//#####################################################
//#####################################################
//##############    Transmit routines     #############
//#####################################################
//#####################################################
int usart_put_char(char Char, unsigned int baseAdd)
{
	if(baseAdd == 0) return 0;
	Uart_t *UsartStruct = usart_get_struct_base_addr(baseAdd);
	while(fifo_push((fifo_settings_t *)UsartStruct->TxBuff, Char) == false);
	USART_t *ModuleAddress = (USART_t *)UsartStruct->BaseAddr;
	ModuleAddress->CTRLA = (ModuleAddress->CTRLA&(~USART_DREINTLVL_gm))|(UsartStruct->Priority<<USART_DREINTLVL_gp);
	return 0;
}
//#####################################################
int usart_put_char_nonblocking(char Char, unsigned int baseAdd)
{
	if(baseAdd == 0) return 0;
	Uart_t *UsartStruct = usart_get_struct_base_addr(baseAdd);
	if(fifo_push((fifo_settings_t *)UsartStruct->TxBuff, Char) == false) return 0;
	USART_t *ModuleAddress = (USART_t *)UsartStruct->BaseAddr;
	ModuleAddress->CTRLA = (ModuleAddress->CTRLA&(~USART_DREINTLVL_gm))|(UsartStruct->Priority<<USART_DREINTLVL_gp);
	return 1;
}
//#####################################################
bool usart_put_char_no_transmit(char Char, Uart_t *UsartStruct)
{
	if(UsartStruct == NULL) return false;
	if(fifo_push((fifo_settings_t *)UsartStruct->TxBuff, Char) == false) return false;
	return true;
}
//#####################################################
int usart_transmit(Uart_t *UsartStruct)
{
	if(UsartStruct == NULL) return EOF;
	USART_t *ModuleAddress = (USART_t *)UsartStruct->BaseAddr;
	ModuleAddress->CTRLA = (ModuleAddress->CTRLA&(~USART_DREINTLVL_gm))|(UsartStruct->Priority<<USART_DREINTLVL_gp);
	return 0;
}
//#####################################################
//#####################################################
//##############		In SPI Mode		  #############
//#####################################################
//#####################################################
uint8_t usart_spi_data(Uart_t *UsartStruct, uint8_t Data)
{
	if(UsartStruct == NULL) return EOF;
	USART_t *ModuleAddress = (USART_t *)UsartStruct->BaseAddr;
	ModuleAddress->DATA = Data;
	while((~ModuleAddress->STATUS) & USART_RXCIF_bm);
	return ModuleAddress->DATA;
}
//#####################################################
unsigned int UARTCharPutNonBlocking(unsigned int baseAdd, unsigned char byteWrite)
{
    return usart_put_char_nonblocking((char)byteWrite, baseAdd);
}

signed char UARTCharGetNonBlocking(unsigned int baseAdd)
{
    return usart_get_char_nonblocking(baseAdd);
}

signed char UARTCharGet(unsigned int baseAdd)
{
    return usart_get_char(baseAdd);
}

void UARTCharPut(unsigned int baseAdd, unsigned char byteTx)
{
    usart_put_char((char)byteTx, baseAdd);
}

unsigned int UARTRxErrorGet(unsigned int baseAdd)
{
	return 0;
}



