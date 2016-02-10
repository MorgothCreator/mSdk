/*
 * board_init.c
 *
 *  Created on: Feb 8, 2016
 *      Author: John Smith
 */

#include "board_init.h"
#include "driver/setup.h"
#include "api/core_init_api.h"

#include "api/init_api_def.h"

#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/mcspi_api.h"
#include "api/mcspi_def.h"

#include "device/mmcsd_spi.h"

new_uart* Uart[1];
new_uart* DebugCom = NULL;
new_gpio* LED[4];
new_gpio* PUSHBTN[5];
new_mcspi* SPI[2];

new_gpio* LV_3_3V_EN = NULL;;
new_mmcsd_spi *MMCSD_SPI[1];

/*#####################################################*/
bool board_init()
{
	core_init();
/*-----------------------------------------------------*/
	INIT_UART(0, 115200, IOA, 3, IOA, 2);
	DebugCom = Uart[0];
	UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , 100, 100, 16);
/*-----------------------------------------------------*/
	LV_3_3V_EN = gpio_assign(0, 13, GPIO_OUT_PUSH_PULL, false);
	gpio_out(LV_3_3V_EN, 1);
/*-----------------------------------------------------*/
	/* Initialize SPI unit for MmcSd interface*/
	INIT_SPI(0, 400000, 8, IOA, 10, IOA, 9, IOA, 8, IOA, 30);
	MMCSD_SPI[0] = new_(new_mmcsd_spi);
	MMCSD_SPI[0]->HardUnitStruct = SPI[0];
	MMCSD_SPI[0]->HardUnitOpenFunc = mcspi_open;
	MMCSD_SPI[0]->HardUnitCloseFunc = mcspi_close;
	MMCSD_SPI[0]->HardUnitReadWriteCharFunc = mcspi_send_byte;
	MMCSD_SPI[0]->HardUnitSetBaudFunc = mcspi_set_baud;
	mmcsd_spi_init(0, gpio_assign(0, 30, GPIO_OUT_PUSH_PULL, false), NULL);
	mmcsd_spi_idle(0);
/*-----------------------------------------------------*/
	//SysCtrlPowerEverything();
	gpio_init(0);
	LED[0] = gpio_assign(0, 25, GPIO_OUT_PUSH_PULL, false);
	LED[1] = gpio_assign(0, 27, GPIO_OUT_PUSH_PULL, false);
	LED[2] = gpio_assign(0, 7, GPIO_OUT_PUSH_PULL, false);
	LED[3] = gpio_assign(0, 6, GPIO_OUT_PUSH_PULL, false);

	PUSHBTN[0] = gpio_assign(0, 19, GPIO_IN_PULL_UP, false);
	PUSHBTN[1] = gpio_assign(0, 12, GPIO_IN_PULL_UP, false);
	PUSHBTN[2] = gpio_assign(0, 15, GPIO_IN_PULL_UP, false);
	PUSHBTN[3] = gpio_assign(0, 18, GPIO_IN_PULL_UP, false);
	PUSHBTN[4] = gpio_assign(0, 11, GPIO_IN_PULL_UP, false);
//core_init();
	//timer_init();
	//RtcInit();
/*-----------------------------------------------------*/
	return true;
}
