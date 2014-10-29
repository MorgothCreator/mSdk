/*
 * board_init.h
 *
 *  Created on: Dec 4, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
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
/*#####################################################*/
//#define BOARD_MESSAGE "Beaglebone"
/*-----------------------------------------------------*/
#define E2PROM_ONBOARD TWI[0], 0x50
//#define E2PROM_BOARD1 TWI[2],0x54
//#define E2PROM_BOARD2 TWI[2],0x55
//#define E2PROM_BOARD3 TWI[2],0x56
//#define E2PROM_BOARD4 TWI[2],0x57
/*#####################################################*/
extern new_uart* DebugCom;
extern new_twi* TWI;
extern new_mcspi* MCSPI0;
//extern new_gpio* HARDBTN[];
//extern new_gpio* LED[];
/*-----------------------------------------------------*/
extern mmcsdCtrlInfo sdCtrl;
/*-----------------------------------------------------*/
/*extern unsigned char *OnBoardConfigData;
extern unsigned char *Board1ConfigData;
extern unsigned char *Board2ConfigData;
extern unsigned char *Board3ConfigData;
extern unsigned char *Board4ConfigData;*/
/*#####################################################*/
bool board_init();
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "board_init.c"
#endif
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
