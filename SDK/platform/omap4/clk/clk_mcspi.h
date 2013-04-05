/*
 * clk_mcspi.h
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef CLK_MCSPI_H_
#define CLK_MCSPI_H_
/*#####################################################*/
#include <stdbool.h>
/*#####################################################*/
bool McSPIModuleClkConfig(unsigned char instance);
bool McSPIModuleClkUnConfig(unsigned char instance);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "clk_mcspi.c"
#endif
/*#####################################################*/
#endif /* CLK_MCSPI_H_ */
/*#####################################################*/
