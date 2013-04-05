/*
 * clk_twi.h
 *
 *  Created on: Jan 16, 2013
 *      Author: XxXx
 */

#ifndef CLK_TWI_H_
#define CLK_TWI_H_

#include <stdbool.h>

bool TWIModuleClkConfig(unsigned char instanceNum);
bool TWIModuleClkUnConfig(unsigned char instanceNum);

#ifdef HEADER_INCLUDE_C_FILES
#include "clk_twi.c"
#endif

#endif /* CLK_TWI_H_ */
