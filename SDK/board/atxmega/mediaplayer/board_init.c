/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include <string.h>
#include "board_init.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/mmcsd_api.h"
#include "device/ar1020.h"
#include "driver/dma.h"
#include "driver/uart.h"
#include "driver/twi.h"
#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"
//#include "device/mi0283.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_uart* Uart1 = NULL;
new_uart* Uart2 = NULL;
new_uart* Uart3 = NULL;
new_uart* Uart4 = NULL;
new_uart* Uart5 = NULL;
new_uart* Uart6 = NULL;
new_uart* Uart7 = NULL;
new_twi* TWI0 = NULL;
new_twi* TWI1 = NULL;
new_twi* TWI2 = NULL;
new_twi* TWI3 = NULL;
new_gpio* HARDBTN1 = NULL;
/*-----------------------------------------------------*/
new_dma_gen* Dma = NULL;
/*-----------------------------------------------------*/
new_touchscreen* TouchScreen = NULL;
new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
SD_Struct_t sd_struct[2];
/*#####################################################*/
bool board_init()
{
	RtcStruct.Rtc_ClkSource = _Rtc_Clk_Source_RCOSC_gc;
	core_init();
	timer_init();
	set_Board
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->BaudRate = 115200;
	DebugCom->RxBuffSize = 20;
	DebugCom->TxBuffSize = 2;
	DebugCom->Mode = UsartCom_Mode_Asynchronus;
	DebugCom->Priority = Usart_Int_Med_Level;
	DebugCom->UartNr = 4;
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
	HARDBTN1 = gpio_assign(0, 1, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN1, 1);
/*-----------------------------------------------------*/
	Dma = new_(new_dma_gen);
	dma_enable(Dma);
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258 at 100000b/s....." , -1);
	TWI3 = new_(new_twi);
	TWI3->BaudRate = 100000;
	TWI3->TwiNr = 3;
	TWI3->Priority = TWI_Int_High_Level;
	TWI3->RxBuffSize = 20;
	TWI3->TxBuffSize = 20;
	twi_open(TWI3);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	//TouchScreen = new_(new_touchscreen);
	TouchScreen = new_(new_touchscreen);
	ScreenBuff = new_(new_screen);
	ScreenBuff->BackLightPort = MI0283QT2_Light_Port_Nr;
	ScreenBuff->BackLightPin = MI0283QT2_Light_Pin;
	ScreenBuff->Orientation = 90;
	screen_init(ScreenBuff);
	//UARTprintf(DebugCom, "LCD display initialize successful for %ux%u resolution, 16 Bit/pixel.\n\r" , (unsigned long)LcdStruct->raster_timings->X, (unsigned long)LcdStruct->raster_timings->Y);
	screen_clear(ScreenBuff, 0);
	screen_clear(ScreenBuff, controls_color.Scren);
	//screen_fill_area(ScreenBuff, 2, 2, ScreenBuff->Width, ScreenBuff->Height, controls_color.Scren);
	backlight_on(ScreenBuff);
	//TouchScreen->screen_max_x = (double)LcdStruct->raster_timings->X;
	//TouchScreen->screen_max_y = (double)LcdStruct->raster_timings->Y;
	TouchScreen->TwiStruct = TWI3;
	TouchScreen->pDisplay = ScreenBuff;
	ar1020_init(TouchScreen, 3, 1);
	//UARTPuts(DebugCom, "Init calibration of LCD resistive touch screen....." , -1);
	//ar1020_calibration_start(TouchScreen, ScreenBuff);
	//UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	UARTPuts(DebugCom, "\n\r" , -1);
/*-----------------------------------------------------*/
	sd_struct[0].CS_Port = &uSD_CS1_Port;
	sd_struct[0].CS_PinMask = uSD_CS1_Pin_pm;
	sd_struct[0].DriveNr = 0;
	mmcsd_init(0, NULL, NULL);
	mmcsd_idle(0);
	/*if(mmcsd_idle(&SD_StructDisk1)) 
	{
		if(SD_StructDisk1.SD_Hc == IsSd) UARTPuts(DebugCom, "uSD disk 1 is SD type.\n\r" , -1);
		else if(SD_StructDisk1.SD_Hc == IsSdhc)UARTPuts(DebugCom, "uSD disk 1 is SDHC type.\n\r" , -1);
		mmcsd_idle(&SD_StructDisk1);
		UARTPuts(DebugCom, "\n\r" , -1);
	}		
	else  UARTPuts(DebugCom, "uSD disk 1 not detected.\n\r" , -1);*/
/*-----------------------------------------------------*/
	sd_struct[1].CS_Port = &uSD_CS2_Port;
	sd_struct[1].CS_PinMask = uSD_CS2_Pin_pm;
	sd_struct[1].DriveNr = 4;
	mmcsd_init(1, NULL, NULL);
	mmcsd_idle(1);
	/*if(mmcsd_idle(&SD_StructDisk2))
	{
		if(SD_StructDisk2.SD_Hc == IsSd) UARTPuts(DebugCom, "uSD disk 2 is SD type.\n\r" , -1);
		else if(SD_StructDisk2.SD_Hc == IsSdhc)UARTPuts(DebugCom, "uSD disk 2 is SDHC type.\n\r" , -1);
		mmcsd_idle(&SD_StructDisk2);
		UARTPuts(DebugCom, "\n\r" , -1);
	}		
	else  UARTPuts(DebugCom, "uSD disk 2 not detected.\n\r" , -1);*/
/*-----------------------------------------------------*/

	return true;
}