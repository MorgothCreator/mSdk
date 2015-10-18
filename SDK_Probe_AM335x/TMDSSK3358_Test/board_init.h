/*
 * board_init.h
 *
 *  Created on: Oct 17, 2015
 *      Author: John Smith
 */

#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include <string.h>
#include "api/uart_api.h"

extern new_uart* DebugCom;

inline bool board_init();

/*#####################################################*/
#endif /* BOARD_INIT_H_ */
