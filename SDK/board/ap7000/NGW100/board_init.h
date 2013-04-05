/*
 * board_init.h
 *
 *  Created on: Jan 8, 2013
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include <stdlib.h>
#include "board_init.h"
#include "api/timer_api.h"
#include "sys/core/core.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/mmcsd_api.h"
#include "api/gpio_api.h"
#include "lib/fs/fat.h"
/*#####################################################*/
#define BOARD_MESSAGE "AVR32 AP7000 NGW100"
/*#####################################################*/
extern new_uart* DebugCom;
extern new_twi* TWI0;
extern new_gpio* LED1;
extern new_gpio* LED2;
extern new_gpio* LED3;
extern new_gpio* LED4;
extern new_gpio* LED5;
/*-----------------------------------------------------*/
extern new_screen* ScreenBuff;
/*-----------------------------------------------------*/
extern FileInfo_t *FILE1;
/*-----------------------------------------------------*/
bool board_init();
/*#####################################################*/
#include "board_init.c"
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
