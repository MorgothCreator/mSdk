/*
 * uart_interface.c
 *
 * Created: 2/8/2013 2:06:44 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#include "uart_interface.h"
#include "driver/uart.h"
#include "api/uart_def.h"
#include "sys/core_init.h"
#include "driver/ioc.h"
#include "driver/uart.h"
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings)
{
	IOCPortConfigureSet(UartSettings->TxPin, IOC_PORT_MCU_UART0_TX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
	HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= (1 << UartSettings->TxPin);
	IOCPortConfigureSet(UartSettings->RxPin, IOC_PORT_MCU_UART0_RX, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE);
	HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << UartSettings->RxPin);
	UARTConfigSetExpClk(UART0_BASE, CoreFreq, UartSettings->BaudRate, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
	//UARTParityModeSet(UART0_BASE, UART_CONFIG_PAR_NONE);
	UARTEnable(UART0_BASE);
	UartSettings->BaseAddr = UART0_BASE;
	return false;
}
/*#####################################################*/
bool _uart_close(Uart_t *UartSettings)
{
	UARTDisable(UART0_BASE);
	return true;
}
/*#####################################################*/
void _UARTBaudSetRate(Uart_t *UartSettings, unsigned long BaudRate)
{
	UARTConfigSetExpClk(UART0_BASE, CoreFreq, BaudRate, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
	UartSettings->BaudRate = BaudRate;
}
/*#####################################################*/
void _UARTCharPut(unsigned int BaseAddr, unsigned char byteTx)
{
	UARTCharPut(BaseAddr, byteTx);
}
/*#####################################################*/
unsigned char _UARTCharGet(unsigned int BaseAddr)
{
	return (signed char)UARTCharGet(BaseAddr);
}
/*#####################################################*/
bool _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx)
{
	return UARTCharPutNonBlocking(BaseAddr, byteTx);
}
/*#####################################################*/
signed short _UARTCharGetNonBlocking(unsigned int BaseAddr)
{
	return UARTCharGetNonBlocking(BaseAddr);
}
/*#####################################################*/
unsigned int _UARTRxErrorGet(unsigned int BaseAddr)
{
	return -1;//UARTRxErrorGet(BaseAddr);
}
/*#####################################################*/
