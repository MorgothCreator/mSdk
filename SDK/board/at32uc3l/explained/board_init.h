/*
 * board_init.h
 *
 * Created: 2/16/2013 9:33:57 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
/*#####################################################*/
#define BOARD_MESSAGE "UC3-L0 Xplained"
/*#####################################################*/
extern new_uart* DebugCom;
extern new_uart* Uart1;
extern new_uart* Uart2;
extern new_uart* Uart3;
extern new_twi* TWI0;
extern new_gpio* LED1;
extern new_gpio* LED2;
extern new_gpio* LED3;
extern new_gpio* LED4;
extern new_gpio* LED5;
/*#####################################################*/
bool board_init(void);
/*#####################################################*/
#include "board_init.c"
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
