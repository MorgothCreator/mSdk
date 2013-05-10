/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include <string.h>
#include "stm32f10x_conf.h"
#include "sys/system_stm32f10x.h"
#include "board_init.h"
#include "api/core_init_api.h"
//#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
//#include "api/twi_def.h"
//#include "api/twi_api.h"
//#include "api/lcd_def.h"
//#include "api/lcd_api.h"
//#include "api/mmcsd_api.h"
//#include "lib/gfx/controls_definition.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_gpio* HARDBTN1 = NULL;
new_gpio* LED1 = NULL;
new_gpio* LED2 = NULL;
new_gpio* LED3 = NULL;
new_gpio* LED4 = NULL;
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
	//timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->BaudRate = 115200;
	DebugCom->RxBuffSize = 20;
	DebugCom->TxBuffSize = 10;
	//DebugCom->Mode = UsartCom_Mode_Asynchronus;
	DebugCom->Priority = 0;
	DebugCom->UartNr = 5;
	uart_open(DebugCom);
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
	HARDBTN1 = gpio_assign(0, 1, GPIO_DIR_INPUT);
	gpio_up_dn(HARDBTN1, 1);
/*-----------------------------------------------------*/
	LED1 = gpio_assign(3, 12, GPIO_DIR_OUTPUT);
	LED2 = gpio_assign(3, 13, GPIO_DIR_OUTPUT);
	LED3 = gpio_assign(3, 14, GPIO_DIR_OUTPUT);
	LED4 = gpio_assign(3, 15, GPIO_DIR_OUTPUT);
/*-----------------------------------------------------*/
	return true;
}
