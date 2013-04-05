/*
 * board_init.c
 *
 * Created: 2/16/2013 9:33:48 PM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "board_init.h"
#include "board_properties.h"
#include "sys/core_init.h"
#include "api/timer_api.h"
#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_uart* Uart1 = NULL;
new_uart* Uart2 = NULL;
new_uart* Uart3 = NULL;
new_twi* TWI0 = NULL;
new_gpio* LED1 = NULL;
new_gpio* LED2 = NULL;
new_gpio* LED3 = NULL;
new_gpio* LED4 = NULL;
new_gpio* LED5 = NULL;
/*#####################################################*/
bool board_init(void)
{
	core_init();
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 1 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->BaudRate = 115200;
	DebugCom->RxPin = 1;
	DebugCom->TxPin = 1;
	DebugCom->rxFifoTrigLevel = 1;
	DebugCom->txFifoTrigLevel = 1;
	DebugCom->UartNr = 2;
	uart_open(DebugCom);
/*-----------------------------------------------------*/
/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 1 to communicate with PMIC and the Onboard serial EEprom memory */
	/*UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI0 = new_(new_twi);
	TWI0->SdaPin = 1;
	TWI0->SclPin = 3;
	TWI0->BaudRate = 100000;
	TWI0->TwiNr = 1;
	TWI0->Priority = 0;
	//TWI0->WithInterrupt = true;
	TWI0->RxBuffSize = 258;
	TWI0->TxBuffSize = 258;
	twi_open(TWI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);*/
/*-----------------------------------------------------*/
	LED1 = gpio_assign(0, 11, GPIO_DIR_OUTPUT);
	LED2 = gpio_assign(1,  4, GPIO_DIR_OUTPUT);
	LED3 = gpio_assign(0, 13, GPIO_DIR_OUTPUT);
	LED4 = gpio_assign(0, 17, GPIO_DIR_OUTPUT);
	LED5 = gpio_assign(0, 20, GPIO_DIR_OUTPUT);
	
	return true;
}	
