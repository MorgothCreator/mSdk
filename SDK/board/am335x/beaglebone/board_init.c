/*
 * board_init.c
 *
 *  Created on: Dec 4, 2012
 *      Author: XxXx
 */
/*#####################################################*/
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
#include "api/gpio_api.h"
#include "api/touchscreen_def.h"
#include "api/usb_msc_host_api.h"
#include "device/24c.h"
#include "device/ADXL345.h"
#include "device/ft5x06.h"
#include "device/tps65910a.h"
#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"
#include "board_properties.h"
#include "api/mmcsd_api.h"
#include "interface/usb_interface.h"
#include "interface/rtc_interface.h"
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
unsigned int RtcVersionGet(void)
{
	return 2;
}

unsigned int EDMAVersionGet(void)
{
	return 2;
}
/********************************************************************/
static bool verify_if_is_beaglebone_board_extension(unsigned char*BoardInfo)
{
	if((unsigned int)(BoardInfo[0]<<24 | BoardInfo[1]<<16 | BoardInfo[2]<<8 | BoardInfo[3]<<0) == 0xAA5533EE) return true;
	else return false;
}
/********************************************************************/
static bool beaglebone_decode_board_info(unsigned char *BoardInfo)
{
	if(!memcmp(BoardInfo + 4, "A0BeagleBone BATTERY CAPE", 25))
	{
		UARTPuts(DebugCom, "BeagleBone battery cape detected.\n\r", -1);
		return true;
	}
	if(!memcmp(BoardInfo + 4, "A0BeagleBone LCD3 CAPE", 22))
	{
		UARTPuts(DebugCom, "BeagleBone LCD3 cape detected.\n\r", -1);
		return true;
	}
	return false;
}
/********************************************************************/
void beaglebone_detect_extension_boards()
{
	unsigned char TwiBuff[49];
	TwiBuff[48] = 0;

	if(E2promRead(E2PROM_BOARD1, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:1 unidentified.\n\r" , -1);
		}
		else
		{
			Board1ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board1ConfigData, TwiBuff, 48);
		}
	}else Board1ConfigData = NULL;

	if(E2promRead(E2PROM_BOARD2, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:2 unidentified.\n\r" , -1);
		}
		else
		{
			Board2ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board2ConfigData, TwiBuff, 48);
		}
	}else Board2ConfigData = NULL;

	if(E2promRead(E2PROM_BOARD3, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:3 unidentified.\n\r" , -1);
		}
		else
		{
			Board3ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board3ConfigData, TwiBuff, 48);
		}
	}else Board3ConfigData = NULL;

	if(E2promRead(E2PROM_BOARD4, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		if(!beaglebone_decode_board_info(TwiBuff))
		{
			UARTPuts(DebugCom, "Board ID:4 unidentified.\n\r" , -1);
		}
		else
		{
			Board4ConfigData = (unsigned char*)calloc(1, 49);
			memcpy(Board4ConfigData, TwiBuff, 48);
		}
	}else Board4ConfigData = NULL;
}
/********************************************************************/
static bool detect_if_is_beaglebone()
{
	UARTPuts(DebugCom, "Read BeagleBone data.\n\r" , -1);
	unsigned char TwiBuff[49];
	TwiBuff[48] = 0;
	if(E2promRead(E2PROM_ONBOARD, 0, TwiBuff, 48) && verify_if_is_beaglebone_board_extension(TwiBuff))
	{
		OnBoardConfigData = (unsigned char*)calloc(1, 49);
		memcpy(OnBoardConfigData, TwiBuff, 48);
		if(!memcmp(OnBoardConfigData + 4, "A335BONE", 8))
		{
			unsigned char tmpstr[3];
			memcpy(tmpstr, OnBoardConfigData + 14, 2);
			tmpstr[2] = NULL;
			UARTprintf(DebugCom, "Board detected: BeagleBone rev %s.\n\r", tmpstr);
			return true;
		}else
		{
			OnBoardConfigData = NULL;
		}
	}
	return false;
}
/*#####################################################*/
bool board_init()
{
	core_init();
	timer_init();
	RtcInit();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->TxPin = Uart0_Tx_PinMux_E16;
	DebugCom->RxPin = Uart0_Rx_PinMux_E15;
	DebugCom->BaudRate = 115200;
	DebugCom->RxBuffSize = 256;
	DebugCom->TxBuffSize = 256;
	DebugCom->rxFifoTrigLevel = 1;
	DebugCom->txFifoTrigLevel = 1;
	DebugCom->UartNr = 0;
	uart_open(DebugCom);
/*-----------------------------------------------------*/
	LED1 = gpio_assign(1, 21, GPIO_DIR_OUTPUT);
	LED2 = gpio_assign(1, 22, GPIO_DIR_OUTPUT);
	LED3 = gpio_assign(1, 23, GPIO_DIR_OUTPUT);
	LED4 = gpio_assign(1, 24, GPIO_DIR_OUTPUT);
/*-----------------------------------------------------*/
	/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
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
	if(!detect_if_is_beaglebone()) return false;
/*-----------------------------------------------------*/
/* Set up the Twi 1 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 1 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI1 = new_(new_twi);
	TWI1->SdaPin = Twi1_Sda_PinMux_B16;
	TWI1->SclPin = Twi1_Scl_PinMux_A16;
	TWI1->BaudRate = 100000;
	TWI1->TwiNr = 1;
	TWI1->Priority = 0;
	TWI1->WithInterrupt = TRUE;
	TWI1->RxBuffSize = 258;
	TWI1->TxBuffSize = 258;
	twi_open(TWI1);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the Twi 2 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 2 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI2 = new_(new_twi);
	TWI2->SdaPin = Twi2_Sda_PinMux_D18;
	TWI2->SclPin = Twi2_Scl_PinMux_D17;
	TWI2->BaudRate = 100000;
	TWI2->TwiNr = 2;
	TWI2->Priority = 0;
	TWI2->WithInterrupt = TRUE;
	TWI2->RxBuffSize = 258;
	TWI2->TxBuffSize = 258;
	twi_open(TWI2);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	beaglebone_detect_extension_boards();
/*-----------------------------------------------------*/
#if defined(USE_ADXL345)
	ADXL345_1 = new_(new_adxl345);
	ADXL345_1->MasterSlaveAddr = ADXL345_SLAVE_ADDR_ALT_0;
	ADXL345_1->TwiStruct = TWI2;
	ADXL345_1->Calibration_X = -14;
	ADXL345_1->Calibration_Y = 11;
	ADXL345_1->Calibration_Z = 24;
	ADXL345_1->FilterBuffSize = 4;
	if(adxl345_init(ADXL345_1))
	{
		UARTPuts(DebugCom, "ADXL345 detected on TWI2.\n\r" , -1);
	}
	else
	{
		free(ADXL345_1);
		UARTPuts(DebugCom, "ADXL345 not detected.\n\r" , -1);
	}
#endif
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Init MMCSD0 Host.......", -1);
	mmcsd_init(NULL, 0, 6, LED1);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Init USBMSC Host 1.......", -1);
	usb_host_init(1, LED2);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	TouchScreen = new_(new_touchscreen);
	TouchScreen->TwiStruct = TWI1;
	bool LCD3_Cape_Detected = false;
	if(Board1ConfigData) if(!memcmp(Board1ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(Board2ConfigData) if(!memcmp(Board2ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(Board3ConfigData) if(!memcmp(Board3ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(Board4ConfigData) if(!memcmp(Board4ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(LCD3_Cape_Detected)
	{
/*-----------------------------------------------------*/
		HARDBTN1 = gpio_assign(1, 16, GPIO_DIR_INPUT);
		HARDBTN2 = gpio_assign(1, 17, GPIO_DIR_INPUT);
		HARDBTN3 = gpio_assign(3, 19, GPIO_DIR_INPUT);
		HARDBTN4 = gpio_assign(1, 28, GPIO_DIR_INPUT);
		HARDBTN5 = gpio_assign(0, 7, GPIO_DIR_INPUT);
/*-----------------------------------------------------*/
		LED5 = gpio_assign(1, 28, GPIO_DIR_OUTPUT);
		LED6 = gpio_assign(1, 18, GPIO_DIR_OUTPUT);
/*-----------------------------------------------------*/
		ScreenBuff = new_(new_screen);
		ScreenBuff->LcdType = S035Q01;
		ScreenBuff->BackLightLevel = 80;
		ScreenBuff->PmicTwiModuleStruct = TWI0;
		screen_init(ScreenBuff);
		UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, 16 Bit/pixel.\n\r" , ScreenBuff->Width, ScreenBuff->Height);

		TouchScreen->screen_max_x = (double)ScreenBuff->Width;
		TouchScreen->screen_max_y = (double)ScreenBuff->Height;
		TouchScreen->pDisplay = ScreenBuff;
		InitTouchScreen(TouchScreen);
		UARTPuts(DebugCom, "Init calibration of LCD resistive touch screen....." , -1);
		TouchCalibrate(TouchScreen, ScreenBuff);
		UARTPuts(DebugCom, "OK.\n\r" , -1);
		put_rectangle(ScreenBuff, 0, 0, ScreenBuff->Width, ScreenBuff->Height, true, controls_color.Scren);
		box_cache_clean(ScreenBuff, 0, 0, ScreenBuff->Width, ScreenBuff->Height);
	}
	else
	{
		if(ft5x06_init(TouchScreen, 0, 30))
		{
			UARTPuts(DebugCom, "Capacitive touch screen detected and set up successful.\n\r" , -1);
			UARTPuts(DebugCom, "I suppose that is BeagleBone Multimedia cape from Chipsee.\n\r" , -1);
/*-----------------------------------------------------*/
			HARDBTN1 = gpio_assign(1, 19, GPIO_DIR_INPUT);
			HARDBTN2 = gpio_assign(1, 16, GPIO_DIR_INPUT);
			HARDBTN3 = gpio_assign(1, 17, GPIO_DIR_INPUT);
			HARDBTN4 = gpio_assign(0, 7, GPIO_DIR_INPUT);
			HARDBTN5 = gpio_assign(1, 3, GPIO_DIR_INPUT);
/*-----------------------------------------------------*/
			ScreenBuff = new_(new_screen);
			ScreenBuff->LcdType = AT070TN92;
			ScreenBuff->BackLightPort = 1;
			ScreenBuff->BackLightPin = 18;
			screen_init(ScreenBuff);
			TouchScreen->screen_max_x = ScreenBuff->Width;
			TouchScreen->screen_max_y = ScreenBuff->Height;
			TouchScreen->pDisplay = ScreenBuff;
			UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, 24 Bit/pixel.\n\r" , ScreenBuff->Width, ScreenBuff->Height);
			put_rectangle(ScreenBuff, 0, 0, ScreenBuff->Width, ScreenBuff->Height, true, controls_color.Scren);
			box_cache_clean(ScreenBuff, 0, 0, ScreenBuff->Width, ScreenBuff->Height);
			screen_backlight_on(ScreenBuff);
		}
		else
		{
			UARTPuts(DebugCom, "No display detected.\n\r" , -1);
			free_touchscreen(TouchScreen);
		}
	}
	return true;
}


