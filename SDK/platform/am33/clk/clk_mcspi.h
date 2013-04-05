/*
 * clk_mcspi.h
 *
 *  Created on: Dec 12, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef CLK_MCSPI_H_
#define CLK_MCSPI_H_
/*#####################################################*/
void McSPIModuleClkConfig(unsigned char instance);
void McSPIModuleClkUnConfig(unsigned char instance);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "clk_mcspi.c"
#endif
/*#####################################################*/
#endif /* CLK_MCSPI_H_ */
/*#####################################################*/
