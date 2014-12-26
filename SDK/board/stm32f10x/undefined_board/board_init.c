/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x_conf.h"
#include "sys/system_stm32f10x.h"
#include "board_init.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
//#include "api/lcd_def.h"
//#include "api/lcd_api.h"
//#include "api/mmcsd_api.h"
//#include "lib/gfx/controls_definition.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
/*#####################################################*/
new_uart* Uart[3] = {NULL,NULL,NULL};
new_uart* DebugCom = NULL;
new_twi* TWI[2] = {NULL,NULL};
new_gpio* LED1 = NULL;
/*-----------------------------------------------------*/
//new_touchscreen* TouchScreen = NULL;
//new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
//SD_Struct_t SD_StructDisk1;
//SD_Struct_t SD_StructDisk2;
/*#####################################################*/
bool board_init()
{
	//RtcStruct.Rtc_ClkSource = _Rtc_Clk_Source_RCOSC_gc;
	core_init();
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	Uart[0] = new_(new_uart);
	Uart[0]->BaudRate = 115200;
	Uart[0]->RxBuffSize = 20;
	Uart[0]->TxBuffSize = 10;
	//Uart[0]->Mode = UsartCom_Mode_Asynchronus;
	Uart[0]->Priority = 0;
	Uart[0]->UartNr = 0;
	uart_open(Uart[0]);
	DebugCom = Uart[0];
/*-----------------------------------------------------*/
/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTPutc(DebugCom, 0xFF);
	UARTPutc(DebugCom, 0xFF);
	UARTPutc(DebugCom, '\n');
	UARTPutc(DebugCom, '\r');
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI[0] = new_(new_twi);
	TWI[0]->BaudRate = 100000;
	TWI[0]->TwiNr = 0;
	TWI[0]->Priority = 0;
	//TWI[0]->WithInterrupt = TRUE;
	TWI[0]->RxBuffSize = 258;
	TWI[0]->TxBuffSize = 258;
	twi_open(TWI[0]);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	LED1 = gpio_assign(2, 13, GPIO_DIR_OUTPUT, false);
/*-----------------------------------------------------*/
	return true;
}
