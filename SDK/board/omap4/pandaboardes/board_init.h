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
#include <string.h>
#include "board_init.h"
#include "sys/core_init.h"
#include "clk/clk_core.h"
#include "driver/control_module.h"
//#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "device/ft5x06.h"
/*#####################################################*/
#define BOARD_MESSAGE "PandaBoardES"
/*-----------------------------------------------------*/
#define E2PROM_ONBOARD TWI0, 0x50
/*#####################################################*/
extern new_uart* DebugCom;
extern new_uart* Uart1;
extern new_uart* Uart2;
extern new_uart* Uart3;
extern new_twi* TWI0;
extern new_twi* TWI1;
extern new_twi* TWI2;
extern new_twi* TWI3;
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
/*-----------------------------------------------------*/
extern new_touchscreen* TouchScreen;
extern new_screen* ScreenBuff;
/*#####################################################*/
/*#####################################################*/
bool board_init();
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "board_init.c"
#endif
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
