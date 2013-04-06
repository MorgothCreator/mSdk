/*
 * uart_interface.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */


/*#####################################################*/
#include <stdbool.h>
#include "api/uart_def.h"
#include "driver/uart.h"
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings)
{
	uart_init(UartSettings->BaudRate);
	return true;
}

/*#####################################################*/
bool _uart_close(Uart_t *UartSettings)
{
/*    UARTModuleReset(UartSettings->BaseAddr);
    UARTModuleClkUnConfig(UartSettings->UartNr);
    if(UartSettings->RxBuff) free((void*)UartSettings->RxBuff);
    if(UartSettings->TxBuff) free((void*)UartSettings->TxBuff);
	switch(UartSettings->UartNr)
	{
	case 0:
		Uart_Int_Service.Uart0 = NULL;
		break;
	case 1:
		Uart_Int_Service.Uart1 = NULL;
		break;
	case 2:
		Uart_Int_Service.Uart2 = NULL;
		break;
	case 3:
		Uart_Int_Service.Uart3 = NULL;
		break;
	case 4:
		Uart_Int_Service.Uart4 = NULL;
		break;
	case 5:
		Uart_Int_Service.Uart5 = NULL;
		break;
	}
	if(UartSettings) free(UartSettings);*/
	return true;
}
/*#####################################################*/
void _UARTBaudSetRate(Uart_t* UartSettings, unsigned long BaudRate)
{
	//uart_set_baud_rate(UartSettings->BaseAddr, BaudRate);
}
/*#####################################################*/
void  _UARTCharPut(unsigned int BaseAddr, char b)
{
    volatile  avr32_usart_t  *usart;
    usart  = &AVR32_USART1;
	if(b=='\n')
	{
		while ((usart->csr & AVR32_USART_CSR_TXRDY_MASK) == 0);
		usart->thr ='\r';
	}
    while ((usart->csr & AVR32_USART_CSR_TXRDY_MASK) == 0){             /*  Wait for room in the transmit register.                 */
        ;
    }
    usart->thr = b;
}


unsigned char _UARTCharGet(unsigned int BaseAddr)
{
    volatile  avr32_usart_t  *usart;
    unsigned char      b;
    usart  = &AVR32_USART1;
    while ((usart->csr & AVR32_USART_CSR_RXRDY_MASK) == 0) {            /*  Wait for a byte to show up.                             */
       // OSTimeDly(2);
    }
    b = (unsigned char)(usart->rhr & 0x00FF);                              /* Read the character.                                      */
    return (b);
}

void  _UARTCharPutNonBlocking(unsigned int BaseAddr, char b)
{

}


signed char _UARTCharGetNonBlocking(unsigned int BaseAddr)
{
	return -1;
}

char _UARTRxErrorGet(unsigned int BaseAddr)
{
	return 0;
}
