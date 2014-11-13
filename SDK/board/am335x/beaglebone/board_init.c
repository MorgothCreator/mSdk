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
#include "board/boards.h"
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
#include "api/usb_host_api.h"
#include "api/mmcsd_api.h"
#include "device/24c.h"
#include "device/ADXL345.h"
#include "device/ft5x06.h"
#include "device/tps65910a.h"
#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"
#include "sys/plat_properties.h"
#include "sys/sysdelay.h"
#include "interface/usb_host_msc_interface.h"
#include "interface/usb_host_mouse_interface.h"
#include "interface/rtc_interface.h"
#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"
#include "interface/mmcsdlib/mmcsd_proto.h"

/*#####################################################*/
new_uart* Uart[4] = {NULL,NULL,NULL,NULL};
new_uart* DebugCom = NULL;
new_twi* TWI[3] = {NULL,NULL,NULL};
new_mcspi* MCSPI0 = NULL;
new_gpio* HARDBTN[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
new_gpio* LED[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
new_adxl345* ADXL345_1 = NULL;
new_touchscreen* TouchScreen = NULL;
new_gpio* eMMC_Res = NULL;
/*-----------------------------------------------------*/
new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
FileInfo_t *FILE1 = NULL;

mmcsdCtrlInfo sdCtrl[2];
/* SD Controller info structure */
//mmcsdCtrlInfo  ctrlInfo;

/*#####################################################*/
bool board_init()
{
	core_init();
	timer_init();
	RtcInit();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	Uart[0] = new_(new_uart);
	Uart[0]->TxPin = Uart0_Tx_PinMux_E16;
	Uart[0]->RxPin = Uart0_Rx_PinMux_E15;
	Uart[0]->BaudRate = 115200;
	Uart[0]->RxBuffSize = 256;
	Uart[0]->TxBuffSize = 256;
	Uart[0]->rxFifoTrigLevel = 1;
	Uart[0]->txFifoTrigLevel = 1;
	Uart[0]->UartNr = 0;
	uart_open(Uart[0]);
	DebugCom = Uart[0];
/*-----------------------------------------------------*/
	LED[0] = gpio_assign(1, 21, GPIO_DIR_OUTPUT, false);
	LED[1] = gpio_assign(1, 22, GPIO_DIR_OUTPUT, false);
	LED[2] = gpio_assign(1, 23, GPIO_DIR_OUTPUT, false);
	LED[3] = gpio_assign(1, 24, GPIO_DIR_OUTPUT, false);
/*-----------------------------------------------------*/
	/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI[0] = new_(new_twi);
	TWI[0]->SdaPin = Twi0_Sda_PinMux_C17;
	TWI[0]->SclPin = Twi0_Scl_PinMux_C16;
	TWI[0]->BaudRate = 100000;
	TWI[0]->TwiNr = 0;
	TWI[0]->Priority = 0;
	TWI[0]->WithInterrupt = TRUE;
	TWI[0]->RxBuffSize = 258;
	TWI[0]->TxBuffSize = 258;
	twi_open(TWI[0]);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	if(!is_beaglebone()) return false;
/*-----------------------------------------------------*/
/* Set up the Twi 1 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 1 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI[1] = new_(new_twi);
	TWI[1]->SdaPin = Twi1_Sda_PinMux_B16;
	TWI[1]->SclPin = Twi1_Scl_PinMux_A16;
	TWI[1]->BaudRate = 100000;
	TWI[1]->TwiNr = 1;
	TWI[1]->Priority = 0;
	TWI[1]->WithInterrupt = TRUE;
	TWI[1]->RxBuffSize = 258;
	TWI[1]->TxBuffSize = 258;
	twi_open(TWI[1]);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the Twi 2 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 2 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI[2] = new_(new_twi);
	TWI[2]->SdaPin = Twi2_Sda_PinMux_D18;
	TWI[2]->SclPin = Twi2_Scl_PinMux_D17;
	TWI[2]->BaudRate = 100000;
	TWI[2]->TwiNr = 2;
	TWI[2]->Priority = 0;
	TWI[2]->WithInterrupt = TRUE;
	TWI[2]->RxBuffSize = 258;
	TWI[2]->TxBuffSize = 258;
	twi_open(TWI[2]);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	beaglebone_detect_extension_boards();
/*-----------------------------------------------------*/
#if USE_ADXL345
	ADXL345_1 = new_(new_adxl345);
	ADXL345_1->MasterSlaveAddr = ADXL345_SLAVE_ADDR_ALT_0;
	ADXL345_1->TwiStruct = TWI[2];
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
		if(ADXL345_1) free(ADXL345_1);
		UARTPuts(DebugCom, "ADXL345 not detected.\n\r" , -1);
	}
#endif
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Init MMCSD0 Host.......", -1);
	sdCtrl[1].SdNr = 0;
	mmcsd_init(&sdCtrl[0], 0, 6, LED[0]);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	eMMC_Res = gpio_assign(1, 20, GPIO_DIR_OUTPUT, false);
	gpio_out(eMMC_Res, 0);
	//Sysdelay(10);
	gpio_out(eMMC_Res, 1);
	UARTPuts(DebugCom, "Init MMCSD1 Host.......", -1);
	sdCtrl[1].SdNr = 1;
	mmcsd_init(&sdCtrl[1], -1, -1, LED[0]);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
#ifdef usb_1_msc
	UARTPuts(DebugCom, "Init USBMSC1 Host.......", -1);
	usb_msc_host_init(1, LED[2]);
	UARTPuts(DebugCom, "OK.\n\r", -1);
#endif
#ifdef usb_1_mouse
	UARTPuts(DebugCom, "Init USBMOUSE1 Host.......", -1);
	usb_mouse_host_init(1);
	UARTPuts(DebugCom, "OK.\n\r", -1);
#endif
/*-----------------------------------------------------*/
#ifdef lcd
	TouchScreen = new_(new_touchscreen);
	TouchScreen->TwiStruct = TWI[1];
#endif
	bool LCD3_Cape_Detected = false;
	if(Board1ConfigData) if(!memcmp(Board1ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(Board2ConfigData) if(!memcmp(Board2ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(Board3ConfigData) if(!memcmp(Board3ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(Board4ConfigData) if(!memcmp(Board4ConfigData + 4, "A0BeagleBone LCD3 CAPE", 22)) LCD3_Cape_Detected = true;
	if(LCD3_Cape_Detected)
	{
/*-----------------------------------------------------*/
		HARDBTN[0] = gpio_assign(1, 16, GPIO_DIR_INPUT, false);
		HARDBTN[1] = gpio_assign(1, 17, GPIO_DIR_INPUT, false);
		HARDBTN[2] = gpio_assign(3, 19, GPIO_DIR_INPUT, false);
		HARDBTN[3] = gpio_assign(1, 28, GPIO_DIR_INPUT, false);
		HARDBTN[4] = gpio_assign(0, 7, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
		LED[4] = gpio_assign(1, 28, GPIO_DIR_OUTPUT, false);
		LED[5] = gpio_assign(1, 18, GPIO_DIR_OUTPUT, false);
/*-----------------------------------------------------*/
#ifdef lcd
		ScreenBuff = new_(new_screen);
		ScreenBuff->raster_timings = &lcd_S035Q01_beaglebone_exp;
		ScreenBuff->BackLightLevel = 60;
		ScreenBuff->PmicTwiModuleStruct = TWI[0];
		screen_init(ScreenBuff);
		UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, ScreenBuff->raster_timings->bus_size);

		TouchScreen->screen_max_x = (double)ScreenBuff->raster_timings->X;
		TouchScreen->screen_max_y = (double)ScreenBuff->raster_timings->Y;
		TouchScreen->pDisplay = ScreenBuff;
		InitTouchScreen(TouchScreen);
		UARTPuts(DebugCom, "Init calibration of LCD resistive touch screen....." , -1);
		TouchCalibrate(TouchScreen, ScreenBuff);
		UARTPuts(DebugCom, "OK.\n\r" , -1);
		put_rectangle(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, true, controls_color.Scren);
		box_cache_clean(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y);
#endif
	}
	else
	{
#ifdef lcd
		if(ft5x06_init(TouchScreen, 0, 30))
		{
			UARTPuts(DebugCom, "Capacitive touch screen detected and set up successful.\n\r" , -1);
#endif
			UARTPuts(DebugCom, "I suppose that is BeagleBone Multimedia cape from Chipsee.\n\r" , -1);
/*-----------------------------------------------------*/
			HARDBTN[0] = gpio_assign(1, 19, GPIO_DIR_INPUT, false);
			HARDBTN[1] = gpio_assign(1, 16, GPIO_DIR_INPUT, false);
			HARDBTN[2] = gpio_assign(1, 17, GPIO_DIR_INPUT, false);
			HARDBTN[3] = gpio_assign(0, 7, GPIO_DIR_INPUT, false);
			//HARDBTN[4] = gpio_assign(1, 3, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
#ifdef lcd
			ScreenBuff = new_(new_screen);
			ScreenBuff->raster_timings = &lcd_AT070TN92_beaglebone_exp;
			ScreenBuff->BackLightPort = 1;
			ScreenBuff->BackLightPin = 18;
			screen_init(ScreenBuff);
			TouchScreen->screen_max_x = ScreenBuff->raster_timings->X;
			TouchScreen->screen_max_y = ScreenBuff->raster_timings->Y;
			TouchScreen->pDisplay = ScreenBuff;
			UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, ScreenBuff->raster_timings->bus_size);
			put_rectangle(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, true, controls_color.Scren);
			box_cache_clean(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y);
			backlight_on(ScreenBuff);
		}
		else
		{
			//UARTPuts(DebugCom, "No display detected.\n\r" , -1);
			//free_touchscreen(TouchScreen);
			UARTPuts(DebugCom, "I suppose that is BeagleBone Multimedia cape from Chipsee.\n\r" , -1);
/*-----------------------------------------------------*/
			HARDBTN[0] = gpio_assign(1, 19, GPIO_DIR_INPUT, false);
			HARDBTN[1] = gpio_assign(1, 16, GPIO_DIR_INPUT, false);
			HARDBTN[2] = gpio_assign(1, 17, GPIO_DIR_INPUT, false);
			HARDBTN[3] = gpio_assign(0, 7, GPIO_DIR_INPUT, false);
			HARDBTN[4] = gpio_assign(1, 3, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
			ScreenBuff = new_(new_screen);
			ScreenBuff->raster_timings = &lcd_720p_50hz_beaglebone_exp;
			ScreenBuff->BackLightPort = 1;
			ScreenBuff->BackLightPin = 18;
			screen_init(ScreenBuff);
#ifdef touch
			TouchScreen->screen_max_x = ScreenBuff->raster_timings->X;
			TouchScreen->screen_max_y = ScreenBuff->raster_timings->Y;
			TouchScreen->pDisplay = ScreenBuff;
			InitTouchScreen(TouchScreen);
			UARTPuts(DebugCom, "Init calibration of LCD resistive touch screen....." , -1);
			TouchCalibrate(TouchScreen, ScreenBuff);
			UARTPuts(DebugCom, "OK.\n\r" , -1);
#endif
			UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, ScreenBuff->raster_timings->bus_size);
			put_rectangle(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, true, controls_color.Scren);
			box_cache_clean(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y);
			backlight_on(ScreenBuff);
		}
#endif
	}
	return true;
}
