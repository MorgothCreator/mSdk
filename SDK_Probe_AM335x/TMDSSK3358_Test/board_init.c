/*
 * board_init.c
 *
 *  Created on: Oct 17, 2015
 *      Author: John Smith
 */




#include "board_init.h"

#include "api/core_init_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/touchscreen_def.h"

#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"

#include "device/ADXL345.h"

#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"

#include "interface/int_touchscreen_interface.h"
#include "interface/rtc_interface.h"

new_uart* DebugCom = NULL;
new_twi* TWI0 = NULL;
new_mcspi* MCSPI0 = NULL;
new_touchscreen* TouchScreen = NULL;
new_screen* ScreenBuff = NULL;

inline bool board_init()
{
	core_init();
	timer_init();
	RtcInit();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->TxPort = IOE;
	DebugCom->RxPort = IOE;
	DebugCom->TxPin = 16;
	DebugCom->RxPin = 15;
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
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258 at 100000b/s....." , -1);
	TWI0 = new_(new_twi);
	TWI0->SdaPort = IOC;
	TWI0->SclPort = IOC;
	TWI0->SdaPin = 17;
	TWI0->SclPin = 16;
	TWI0->BaudRate = 100000;
	TWI0->TwiNr = 0;
	TWI0->Priority = 0;
	TWI0->UseInterrupt = true;
	TWI0->RxBuffSize = 258;
	TWI0->TxBuffSize = 258;
	twi_open(TWI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "Setup McSpi0 only CS0 with Buff = 260 at 24Mb/s on J8 connector....." , -1);
	MCSPI0 = new_(new_mcspi);
	MCSPI0->BaudRate = 24000000;
	MCSPI0->BuffSize = 260;
	MCSPI0->Channel = 0;
	MCSPI0->McspiNr = 0;
	MCSPI0->Priority = 0;
	MCSPI0->MosiPort = IOB;
	MCSPI0->MisoPort = IOB;
	MCSPI0->SckPort = IOA;
	MCSPI0->Cs0Port = IOA;
	MCSPI0->MosiPin = 17;
	MCSPI0->MisoPin = 16;
	MCSPI0->SckPin = 17;
	MCSPI0->Cs0Pin = 16;
	mcspi_open(MCSPI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	ScreenBuff = new_(new_screen);
	ScreenBuff->raster_timings = &lcd_TFT43_TMDSSK3358;
	ScreenBuff->BackLightLevel = 60;
	ScreenBuff->PmicTwiModuleStruct = TWI0;
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
	return true;
}
