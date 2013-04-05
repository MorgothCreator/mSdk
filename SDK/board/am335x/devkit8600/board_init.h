/*
 *  lib/api/board_init.h
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

#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include <string.h>
#include "board_properties.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/pmic_api.h"
#include "api/touchscreen_def.h"
#include "api/mmcsd_api.h"
#include "device/24c.h"
#include "device/ADXL345.h"
#include "device/tps65910a.h"
#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"
#include "board_properties.h"
#include "interface/int_touchscreen_interface.h"
#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"
/*#####################################################*/
#define BOARD_MESSAGE "DevKit8600"
/*-----------------------------------------------------*/
#define E2PROM_ONBOARD TWI0, 0x50
#define E2PROM_BOARD1 TWI2,0x54
#define E2PROM_BOARD2 TWI2,0x55
#define E2PROM_BOARD3 TWI2,0x56
#define E2PROM_BOARD4 TWI2,0x57
/*#####################################################*/
extern new_uart* DebugCom;
extern new_uart* Uart1;
extern new_uart* Uart2;
extern new_uart* Uart3;
extern new_twi* TWI0;
extern new_twi* TWI1;
extern new_twi* TWI2;
extern new_mcspi* MCSPI0;
extern new_gpio* HARDBTN1;
extern new_gpio* HARDBTN2;
extern new_gpio* HARDBTN3;
extern new_gpio* HARDBTN4;
extern new_gpio* HARDBTN5;
extern new_gpio* LED1;
extern new_gpio* LED2;
extern new_gpio* LED3;
extern new_gpio* LED4;
extern new_gpio* LED5;
extern new_gpio* LED6;
extern new_adxl345* ADXL345_1;
extern new_touchscreen* TouchScreen;
/*-----------------------------------------------------*/
extern new_screen* ScreenBuff;
/*-----------------------------------------------------*/
extern FileInfo_t *FILE1;
/*-----------------------------------------------------*/
extern unsigned char *OnBoardConfigData;
extern unsigned char *Board1ConfigData;
extern unsigned char *Board2ConfigData;
extern unsigned char *Board3ConfigData;
extern unsigned char *Board4ConfigData;
/********************************************************************/
//extern mmcsdCtrlInfo  ctrlInfo;
/********************************************************************/
bool board_init();
/*#####################################################*/
//#include "board_init.c"
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
