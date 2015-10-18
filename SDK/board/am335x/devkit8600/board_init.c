/*
 *  lib/api/board_init.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <string.h>
#include "board_init.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/pmic_api.h"
#include "api/usb_api.h"
#include "api/touchscreen_def.h"
#include "api/mmcsd_api.h"
#include "device/24c.h"
#include "device/ADXL345.h"
#include "device/ft5x06.h"
#include "device/tps65910a.h"
#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"
#include "board_properties.h"
#include "interface/usb_interface.h"
#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_uart* Uart1 = NULL;
new_uart* Uart2 = NULL;
new_uart* Uart3 = NULL;
new_twi* TWI0 = NULL;
new_twi* TWI1 = NULL;
new_twi* TWI2 = NULL;
new_twi* TWI3 = NULL;
new_mcspi* MCSPI0 = NULL;
new_gpio* HARDBTN1 = NULL;
new_gpio* HARDBTN2 = NULL;
new_gpio* HARDBTN3 = NULL;
new_gpio* HARDBTN4 = NULL;
new_gpio* HARDBTN5 = NULL;
new_gpio* HARDBTN6 = NULL;
new_gpio* LED1 = NULL;
new_gpio* LED2 = NULL;
new_gpio* LED3 = NULL;
new_gpio* LED4 = NULL;
new_gpio* LED5 = NULL;
new_gpio* LED6 = NULL;
new_adxl345* ADXL345_1 = NULL;
new_touchscreen* TouchScreen = NULL;
/*-----------------------------------------------------*/
new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
FileInfo_t *FILE1 = NULL;
/*-----------------------------------------------------*/
unsigned char *OnBoardConfigData = NULL;
unsigned char *Board1ConfigData = NULL;
unsigned char *Board2ConfigData = NULL;
unsigned char *Board3ConfigData = NULL;
unsigned char *Board4ConfigData = NULL;
/********************************************************************/
//extern mmcsdCtrlInfo  ctrlInfo;
/********************************************************************/
unsigned int RtcVersionGet(void)
{
	return 2;
}

unsigned int EDMAVersionGet(void)
{
	return 2;
}
/*#####################################################*/
bool board_init()
{
	core_init();
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->TxPin = Uart0_Tx_PinMux_E16;
	DebugCom->RxPin = Uart0_Rx_PinMux_E15;
	DebugCom->BaudRate = 115200;
	//DebugCom->RxBuffSize = 256;
	//DebugCom->TxBuffSize = 256;
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
	HARDBTN1 = gpio_assign(0, 22, GPIO_DIR_INPUT, false);
	HARDBTN2 = gpio_assign(1, 30, GPIO_DIR_INPUT, false);
	HARDBTN3 = gpio_assign(1, 31, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
	LED1 = gpio_assign(1, 26, GPIO_DIR_OUTPUT, false);
	LED2 = gpio_assign(1, 27, GPIO_DIR_OUTPUT, false);
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258 at 100000b/s....." , -1);
	TWI0 = new_(new_twi);
	TWI0->SdaPin = Twi0_Sda_PinMux_C17;
	TWI0->SclPin = Twi0_Scl_PinMux_C16;
	TWI0->BaudRate = 100000;
	TWI0->TwiNr = 0;
	TWI0->Priority = 0;
	TWI0->WithInterrupt = TRUE;
	TWI0->RxBuffSize = 258;
	TWI0->TxBuffSize = 258;
	twi_open(TWI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
#ifdef USE_TWI1_ON_UART1
/* Set up the Twi 1 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI1 with RxBuff = 258, TxBuff = 258 at 100000b/s instead of Uart1 on J5 connector....." , -1);
	TWI1 = new_(new_twi);
	TWI1->SdaPin = Twi1_Sda_PinMux_D16;
	TWI1->SclPin = Twi1_Scl_PinMux_D15;
	TWI1->BaudRate = 100000;
	TWI1->TwiNr = 1;
	TWI1->Priority = 0;
	TWI1->WithInterrupt = TRUE;
	TWI1->RxBuffSize = 258;
	TWI1->TxBuffSize = 258;
	twi_open(TWI1);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
#else
/* Set up the Uart 1 with RxBuff = 256, TxBuff = 256, 115200b/s*/
	UARTPuts(DebugCom, "Setup Uart1 with RxBuff = 256, TxBuff = 256 at 115200b/s on J5 connector....." , -1);
	Uart1 = new_(new_uart);
	Uart1->TxPin = Uart1_Tx_PinMux_D15;
	Uart1->RxPin = Uart1_Rx_PinMux_D16;
	Uart1->BaudRate = 115200;
	Uart1->RxBuffSize = 256;
	Uart1->TxBuffSize = 256;
	Uart1->rxFifoTrigLevel = 1;
	Uart1->txFifoTrigLevel = 1;
	Uart1->UartNr = 1;
	uart_open(Uart1);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
#endif
/*-----------------------------------------------------*/
#ifdef USE_TWI1_ON_UART2
/* Set up the Twi 1 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI1 with RxBuff = 258, TxBuff = 258 instead of Uart2 on J6 connector....." , -1);
	TWI1 = new_(new_twi);
	TWI1->SdaPin = Twi1_Sda_PinMux_H17;
	TWI1->SclPin = Twi1_Scl_PinMux_J15;
	TWI1->BaudRate = 100000;
	TWI1->TwiNr = 1;
	TWI1->Priority = 0;
	TWI1->WithInterrupt = TRUE;
	TWI1->RxBuffSize = 258;
	TWI1->TxBuffSize = 258;
	twi_open(TWI1);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
#else
/* Set up the Uart 2 with RxBuff = 256, TxBuff = 256, 115200b/s*/
	UARTPuts(DebugCom, "Setup Uart2 with RxBuff = 256, TxBuff = 256 at 115200b/s on J6 connector....." , -1);
	Uart2 = new_(new_uart);
	Uart2->TxPin = Uart2_Tx_PinMux_J15;
	Uart2->RxPin = Uart2_Rx_PinMux_H17;
	Uart2->BaudRate = 9600;
	Uart2->RxBuffSize = 256;
	Uart2->TxBuffSize = 256;
	Uart2->rxFifoTrigLevel = 1;
	Uart2->txFifoTrigLevel = 1;
	Uart2->UartNr = 2;
	uart_open(Uart2);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
#endif
/*-----------------------------------------------------*/
/* Set up the Uart 3 with RxBuff = 256, TxBuff = 256, 115200b/s*/
	UARTPuts(DebugCom, "Setup Uart3 with RxBuff = 256, TxBuff = 256 at 115200b/s on J5 connector....." , -1);
	Uart3 = new_(new_uart);
	Uart3->TxPin = Uart3_Tx_PinMux_C18;
	Uart3->RxPin = Uart3_Rx_PinMux_C15;
	Uart3->BaudRate = 115200;
	Uart3->RxBuffSize = 256;
	Uart3->TxBuffSize = 256;
	Uart3->rxFifoTrigLevel = 1;
	Uart3->txFifoTrigLevel = 1;
	Uart3->UartNr = 3;
	uart_open(Uart3);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Setup McSpi0 only CS0 with Buff = 260 at 24Mb/s on J8 connector....." , -1);
	MCSPI0 = new_(new_mcspi);
	MCSPI0->BaudRate = 24000000;
	MCSPI0->BuffSize = 260;
	MCSPI0->Channel = 0;
	MCSPI0->McspiNr = 0;
	MCSPI0->Priority = 0;
	MCSPI0->D0Pin = Spi0_D0_PinMux_B17;
	MCSPI0->D1Pin = Spi0_D1_PinMux_B16;
	MCSPI0->ClkPin = Spi0_Sclk_PinMux_A17;
	MCSPI0->Cs0Pin = Spi0_CS0_PinMux_A16;
	MCSPI0->Cs1Pin = 0;
	mcspi_open(MCSPI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Init MMCSD0 Host.......", -1);
	mmcsd_init(NULL, 3, 19, LED1);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Init USBMSC Host 0.......", -1);
	usb_host_init(0, LED2);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Init USBMSC Host 1.......", -1);
	usb_host_init(1, LED2);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	TouchScreen = new_(new_touchscreen);
	ScreenBuff = new_(new_screen);
	ScreenBuff->LcdType = TFT43AB_OMAP35x;
	ScreenBuff->BackLightPort = 3;
	ScreenBuff->BackLightPin = 17;
	screen_init(ScreenBuff);
	UARTprintf(DebugCom, "LCD display initialized successful for %dx%d resolution, 16 Bit/pixel.\n\r" , ScreenBuff->Width, ScreenBuff->Height);
	screen_backlight_on(ScreenBuff);
/*-----------------------------------------------------*/
	TouchScreen->screen_max_x = (double)ScreenBuff->Width;
	TouchScreen->screen_max_y = (double)ScreenBuff->Height;
	TouchScreen->pDisplay = ScreenBuff;
	InitTouchScreen(TouchScreen);
	UARTPuts(DebugCom, "Init calibration of LCD resistive touch screen....." , -1);
	TouchCalibrate(TouchScreen, ScreenBuff);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
	put_rectangle(ScreenBuff, 0, 0, ScreenBuff->Width, ScreenBuff->Height, true, controls_color.Scren);
	box_cache_clean(ScreenBuff, 0, 0, ScreenBuff->Width, ScreenBuff->Height);
/*-----------------------------------------------------*/
	return true;
}


