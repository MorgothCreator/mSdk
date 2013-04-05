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
#include "board_init.h"
#include "api/core_init_api.h"
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
#include "lib/fs/fat.h"
//#include "device/mi0283.h"
/*#####################################################*/
#define BOARD_MESSAGE "XmegaMedaiPlayer"
/*#####################################################*/
extern new_uart* DebugCom;
extern new_uart* Uart1;
extern new_uart* Uart2;
extern new_uart* Uart3;
extern new_uart* Uart4;
extern new_uart* Uart5;
extern new_uart* Uart6;
extern new_uart* Uart7;
extern new_twi* TWI0;
extern new_twi* TWI1;
extern new_twi* TWI2;
extern new_twi* TWI3;
extern new_gpio* HARDBTN1;
extern new_dma_gen* Dma;
/*-----------------------------------------------------*/
extern new_touchscreen* TouchScreen;
extern new_screen* ScreenBuff;
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
