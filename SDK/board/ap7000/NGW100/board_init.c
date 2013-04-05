/*
 * board_init.c
 *
 *  Created on: Jan 8, 2013
 *      Author: XxXx
 */

/*#####################################################*/
#include <stdbool.h>
#include <stdlib.h>
#include "board_init.h"
#include "api/timer_api.h"
#include "sys/core/core.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/mmcsd_api.h"
#include "api/gpio_api.h"
#include "lib/fs/fat.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_twi* TWI0 = NULL;
new_gpio* LED1 = NULL;
new_gpio* LED2 = NULL;
new_gpio* LED3 = NULL;
new_gpio* LED4 = NULL;
new_gpio* LED5 = NULL;
/*-----------------------------------------------------*/
new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
FileInfo_t *FILE1 = NULL;
/*#####################################################*/
bool board_init()
{
	core_init(180000000);
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->BaudRate = 115200;
	DebugCom->RxBuffSize = 256;
	DebugCom->TxBuffSize = 256;
	DebugCom->rxFifoTrigLevel = 1;
	DebugCom->txFifoTrigLevel = 1;
	DebugCom->UartNr = 0;
	uart_open(DebugCom);
/*-----------------------------------------------------*/
	/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 1 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI0 = new_(new_twi);
	TWI0->BaudRate = 100000;
	TWI0->RxBuffSize = 258;
	TWI0->TxBuffSize = 258;
	twi_open(TWI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	LED1 = gpio_assign(4, 19, dir_out);
	LED2 = gpio_assign(0, 16, dir_out);
	LED3 = gpio_assign(0, 19, dir_out);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Init MMCSD0 Host.......", -1);
	mmcsd_init(NULL, 2, 25, NULL);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	ScreenBuff = new_(new_screen);
	ScreenBuff->LcdType = AT070TN92;
	screen_init(ScreenBuff);
	UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, 24 Bit/pixel.\n\r" , ScreenBuff->Width, ScreenBuff->Height);
/*-----------------------------------------------------*/
	return true;
}
