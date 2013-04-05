/*
 * uart_interface.c
 *
 * Created: 2/8/2013 2:06:44 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#include "uart_interface.h"
#include "driver/usart.h"
#include "api/uart_def.h"
#include "sys\clock\sysclk.h"
#include "driver/gpio.h"
//#include "int/int_uart.h"
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings)
{
	usart_options_t usart_options;
	usart_options.baudrate = UartSettings->BaudRate;
	usart_options.channelmode = USART_NORMAL_CHMODE;
	usart_options.charlength = 8;
	usart_options.paritytype = USART_NO_PARITY;
	usart_options.stopbits = USART_1_STOPBIT;
	gpio_map_t COMPORT_GPIO_MAP = {{ 0, 0 }, { 0, 0 }};

	switch (UartSettings->UartNr)
	{
		case 0:
			UartSettings->BaseAddr = (unsigned int)(void*)&AVR32_USART0;
			COMPORT_GPIO_MAP[0].pin = AVR32_USART0_RXD_0_PIN;
			COMPORT_GPIO_MAP[0].function = AVR32_USART0_RXD_0_FUNCTION;
			COMPORT_GPIO_MAP[1].pin = AVR32_USART0_TXD_0_PIN;
			COMPORT_GPIO_MAP[1].function = AVR32_USART0_TXD_0_FUNCTION;
			break;
		case 1:
			UartSettings->BaseAddr = (unsigned int)(void*)&AVR32_USART1;
			switch(UartSettings->RxPin)
			{
				case 0:
					COMPORT_GPIO_MAP[0].pin = AVR32_USART1_RXD_0_0_PIN;
					COMPORT_GPIO_MAP[0].function = AVR32_USART1_RXD_0_0_FUNCTION;
					break;
				case 1:
					COMPORT_GPIO_MAP[0].pin = AVR32_USART1_RXD_0_1_PIN;
					COMPORT_GPIO_MAP[0].function = AVR32_USART1_RXD_0_1_FUNCTION;
					break;
				case 2:
					COMPORT_GPIO_MAP[0].pin = AVR32_USART1_RXD_0_2_PIN;
					COMPORT_GPIO_MAP[0].function = AVR32_USART1_RXD_0_2_FUNCTION;
					break;
			}
			switch(UartSettings->TxPin)
			{
				case 0:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART1_TXD_0_0_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART1_TXD_0_0_FUNCTION;
					break;
				case 1:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART1_TXD_0_1_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART1_TXD_0_1_FUNCTION;
					break;
				case 2:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART1_TXD_0_2_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART1_TXD_0_2_FUNCTION;
					break;
			}
			break;
		case 2:
			UartSettings->BaseAddr = (unsigned int)(void*)&AVR32_USART2;
			switch(UartSettings->RxPin)
			{
				case 0:
					COMPORT_GPIO_MAP[0].pin = AVR32_USART2_RXD_0_0_PIN;
					COMPORT_GPIO_MAP[0].function = AVR32_USART2_RXD_0_0_FUNCTION;
					break;
				case 1:
					COMPORT_GPIO_MAP[0].pin = AVR32_USART2_RXD_0_1_PIN;
					COMPORT_GPIO_MAP[0].function = AVR32_USART2_RXD_0_1_FUNCTION;
					break;
			}
			switch(UartSettings->TxPin)
			{
				case 0:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART2_TXD_0_0_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART2_TXD_0_0_FUNCTION;
					break;
				case 1:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART2_TXD_0_1_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART2_TXD_0_1_FUNCTION;
					break;
				case 2:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART2_TXD_0_2_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART2_TXD_0_2_FUNCTION;
					break;
				case 3:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART2_TXD_0_3_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART2_TXD_0_3_FUNCTION;
					break;
			}
			break;
		case 3:
			UartSettings->BaseAddr = (unsigned int)(void*)&AVR32_USART3;
			switch(UartSettings->RxPin)
			{
				case 0:
					COMPORT_GPIO_MAP[0].pin = AVR32_USART3_RXD_0_0_PIN;
					COMPORT_GPIO_MAP[0].function = AVR32_USART3_RXD_0_0_FUNCTION;
					break;
				case 1:
					COMPORT_GPIO_MAP[0].pin = AVR32_USART3_RXD_0_1_PIN;
					COMPORT_GPIO_MAP[0].function = AVR32_USART3_RXD_0_1_FUNCTION;
					break;
			}
			switch(UartSettings->TxPin)
			{
				case 0:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART3_TXD_0_0_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART3_TXD_0_0_FUNCTION;
					break;
				case 1:
					COMPORT_GPIO_MAP[1].pin = AVR32_USART3_TXD_0_1_PIN;
					COMPORT_GPIO_MAP[1].function = AVR32_USART3_TXD_0_1_FUNCTION;
					break;
			}
			break;
		default:
			return false;
	}
	
	if(gpio_enable_module (COMPORT_GPIO_MAP, sizeof (COMPORT_GPIO_MAP) / sizeof (COMPORT_GPIO_MAP[0])) != GPIO_SUCCESS) return false;
	// Initialize USART in RS232 mode with the requested settings.
	sysclk_enable_peripheral_clock((avr32_usart_t*)UartSettings->BaseAddr);
	if(usart_init_rs232(UartSettings->BaseAddr, &usart_options, sysclk_get_pba_hz()) != USART_SUCCESS) return false;
	return true;
}
/*#####################################################*/
bool _uart_close(Uart_t *UartSettings)
{
	//UartClose(UartSettings);
	return true;
}
/*#####################################################*/
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate)
{
	usart_set_async_baudrate(BaseAddr, BaudRate, sysclk_get_pba_hz());
	//uart_set_baud_rate(UartSettings->BaseAddr, BaudRate);
}
/*#####################################################*/
void _UARTCharPut (unsigned int BaseAddr, unsigned char byteTx) 
{
	usart_putchar(BaseAddr, (int)byteTx);
}	
/*#####################################################*/
void _UARTCharPutNonBlocking(unsigned int BaseAddr,unsigned char byteTx)
{
	usart_write_char(BaseAddr, (int)byteTx);
}	
/*#####################################################*/
signed int _UARTCharGet(unsigned int BaseAddr) 
{
	return (signed int)usart_getchar(BaseAddr);
}	
/*#####################################################*/
signed char _UARTCharGetNonBlocking(unsigned int BaseAddr)
{
	 signed int C = -1;
	 if(usart_read_char(BaseAddr, &C) == USART_SUCCESS) return C;
	 return -1;
}	 
/*#####################################################*/
unsigned int _UARTRxErrorGet(unsigned int BaseAddr)
{
	return 0;
}
/*#####################################################*/



