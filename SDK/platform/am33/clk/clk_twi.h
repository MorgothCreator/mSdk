/*
 * clk_twi.h
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef CLK_TWI_H_
#define CLK_TWI_H_
/*#####################################################*/
#include "api/twi_def.h"
/*#####################################################*/
void TWIModuleClkConfig(new_twi* TwiStruct);
void TWIModuleClkUnConfig(new_twi* TwiStruct);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "clk_twi.c"
#endif
/*#####################################################*/
#endif /* CLK_TWI_H_ */
/*#####################################################*/
