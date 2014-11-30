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
#include "main.h"
#ifdef test1
#include <stdbool.h>
#include <string.h>
#include "sys/plat_properties.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/pmic_api.h"
#include "api/touchscreen_def.h"
#include "device/24c.h"
#include "device/ADXL345.h"
#include "device/tps65910a.h"
#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"
#include "sys/plat_properties.h"
#include "interface/int_touchscreen_interface.h"
#include "api/mmcsd_api.h"
#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"
#include "interface/mmcsdlib/mmcsd_proto.h"
/*#####################################################*/
#define USE_ADXL345 1
/*-----------------------------------------------------*/
#define BOARD_MESSAGE "Beaglebone"
/*-----------------------------------------------------*/
#define usb_host_msc_debug
#define mmcsd_debug
/*#####################################################*/
extern new_uart* Uart[];
extern new_uart* DebugCom;
extern new_twi* TWI[];
extern new_mcspi* MCSPI0;
extern new_gpio* HARDBTN[];
extern new_gpio* LED[];
extern new_adxl345* ADXL345_1;
extern new_touchscreen* TouchScreen;
/*-----------------------------------------------------*/
extern new_screen* ScreenBuff;
/*-----------------------------------------------------*/
extern FileInfo_t *FILE1;
extern mmcsdCtrlInfo sdCtrl[];
/*#####################################################*/
bool board_init();
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "board_init.c"
#endif
/*#####################################################*/
#endif
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
