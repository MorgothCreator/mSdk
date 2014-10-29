/*
 * board_init.c
 *
 *  Created on: Dec 4, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "board_init.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/pmic_api.h"
#include "api/gpio_api.h"
//#include "api/usb_msc_host_api.h"
#include "api/mmcsd_api.h"
//#include "device/24c.h"
#include "device/tps65910a.h"
#include "board_properties.h"
//#include "interface/usb_interface.h"
#include "pinmux/pin_mux_uart.h"
//#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"
#include "interface/mmcsdlib/mmcsd_proto.h"
#include "lib/fs/fat.h"

#include "api/cache_api.h"
#include "api/gpio_api.h"
#include "include/interrupt.h"
#include "interface/mmu_interface.h"
#include "clk/clk_core.h"
#include "include/hw/hw_types.h"
#include "include/hw/hw_cm_per.h"
#include "include/hw/hw_cm_dpll.h"
#include "include/hw/hw_cm_wkup.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_control_AM335x.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_twi* TWI = NULL;
new_mcspi* MCSPI0 = NULL;
//new_gpio* HARDBTN[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
//new_gpio* LED[6] = {NULL,NULL,NULL,NULL,NULL,NULL};

mmcsdCtrlInfo sdCtrl;
/* SD Controller info structure */
//mmcsdCtrlInfo  ctrlInfo;
FileInfo_t *FILE1;
/*-----------------------------------------------------*/
/*unsigned char *OnBoardConfigData = NULL;
unsigned char *Board1ConfigData = NULL;
unsigned char *Board2ConfigData = NULL;
unsigned char *Board3ConfigData = NULL;
unsigned char *Board4ConfigData = NULL;*/
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
/*static bool verify_if_is_beaglebone_board_extension(unsigned char*BoardInfo)
{
	if((unsigned int)(BoardInfo[0]<<24 | BoardInfo[1]<<16 | BoardInfo[2]<<8 | BoardInfo[3]<<0) == 0xAA5533EE) return true;
	else return false;
}*/
/********************************************************************/
/*static bool beaglebone_decode_board_info(unsigned char *BoardInfo)
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
}*/
/********************************************************************/
/*void beaglebone_detect_extension_boards()
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
}*/
/********************************************************************/
/*static bool detect_if_is_beaglebone()
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
			tmpstr[2] = (int)NULL;
			UARTprintf(DebugCom, "Board detected: BeagleBone rev %s.\n\r", tmpstr);
			return true;
		}
		else if(!memcmp(OnBoardConfigData + 4, "A335BNLT", 8))
		{
			unsigned char tmpstr[3];
			memcpy(tmpstr, OnBoardConfigData + 14, 2);
			tmpstr[2] = (int)NULL;
			UARTprintf(DebugCom, "Board detected: BeagleBoneBlack rev %s.\n\r", tmpstr);
			return true;
		}
		else
		{
			OnBoardConfigData = NULL;
		}
	}
	return false;
}*/
/*#####################################################*/
bool board_init()
{
	//MMUConfigAndEnable();
	CacheEnable(CACHE_ICACHE);
	CacheEnable(CACHE_ALL);
	L3L4_init();
    /* Enable IRQ for ARM (in CPSR)*/
    IntMasterIRQEnable();
    IntAINTCInit();
	timer_init();
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
/*	LED[0] = gpio_assign(1, 21, GPIO_DIR_OUTPUT, false);
	LED[1] = gpio_assign(1, 22, GPIO_DIR_OUTPUT, false);
	LED[2] = gpio_assign(1, 23, GPIO_DIR_OUTPUT, false);
	LED[3] = gpio_assign(1, 24, GPIO_DIR_OUTPUT, false);*/
/*-----------------------------------------------------*/
	/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	//UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI = new_(new_twi);
	TWI->SdaPin = Twi0_Sda_PinMux_C17;
	TWI->SclPin = Twi0_Scl_PinMux_C16;
	TWI->BaudRate = 100000;
	TWI->TwiNr = 0;
	TWI->Priority = 0;
	TWI->WithInterrupt = TRUE;
	TWI->RxBuffSize = 258;
	TWI->TxBuffSize = 258;
	twi_open(TWI);
	//UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	//if(!detect_if_is_beaglebone()) return false;
/*-----------------------------------------------------*/
	//beaglebone_detect_extension_boards();
/*-----------------------------------------------------*/
	//UARTPuts(DebugCom, "Init MMCSD0 Host.......", -1);
	mmcsd_init(&sdCtrl, 0, 6, NULL /*LED[0]*/);
	//UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	//UARTPuts(DebugCom, "Init USBMSC1 Host.......", -1);
	//usb_host_init(1, LED2);
	//UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
	return true;
}
