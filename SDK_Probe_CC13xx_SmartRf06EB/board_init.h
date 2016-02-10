/*
 * board_init.h
 *
 *  Created on: Feb 8, 2016
 *      Author: John Smith
 */

#ifndef SDK_PROBE_CC13XX_SMARTRF06EB_BOARD_INIT_H_
#define SDK_PROBE_CC13XX_SMARTRF06EB_BOARD_INIT_H_

#include "board_init.h"
#include "api/gpio_def.h"
#include "api/uart_def.h"
#include "api/mcspi_def.h"

extern new_uart* Uart[];
extern new_uart* DebugCom;
extern new_gpio* LED[];
extern new_gpio* PUSHBTN[];
extern new_mcspi* SPI[2];

bool board_init();

#endif /* SDK_PROBE_CC13XX_SMARTRF06EB_BOARD_INIT_H_ */
