/*
 * board_init.h
 *
 * Created: 2/8/2013 12:22:37 AM
 *  Author: XxXx
 */
/*#####################################################*/
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include <string.h>
#include "stm32f10x_conf.h"
#include "sys/system_stm32f10x.h"
#include "api/core_init_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
//#include "api/twi_def.h"
//#include "api/twi_api.h"
//#include "api/lcd_def.h"
//#include "api/lcd_api.h"
//#include "api/mmcsd_api.h"
//#include "device/ar1020.h"
//#include "driver/dma.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
/*#####################################################*/
#define BOARD_MESSAGE "STM32F407VGT6 Discovery"
/*#####################################################*/
extern new_uart* DebugCom;
extern new_gpio* HARDBTN1;
extern new_gpio* LED1;
extern new_gpio* LED2;
extern new_gpio* LED3;
extern new_gpio* LED4;
/*-----------------------------------------------------*/
//extern new_touchscreen* TouchScreen;
//extern new_screen* ScreenBuff;
/*-----------------------------------------------------*/
//extern SD_Struct_t SD_StructDisk1;
//extern SD_Struct_t SD_StructDisk2;
/*#####################################################*/
bool board_init();
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "board_init.c"
#endif
/*#####################################################*/
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
