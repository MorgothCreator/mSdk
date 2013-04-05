/*
 * mcspi_interface.h
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef MCSPI_INTERFACE_H_
#define MCSPI_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/mcspi_def.h"
/*#####################################################*/

#define SOC_SPI_0_REGS	0x48098000
#define SOC_SPI_1_REGS	0x4809A000
#define SOC_SPI_2_REGS	0x480B8000
#define SOC_SPI_3_REGS	0x480BA000

void _mcspi_transfer(Mcspi_t *McspiStruct);
bool _mcspi_open(Mcspi_t *McspiStruct);
void _mcspi_close(Mcspi_t *McspiStruct);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_interface.c"
#endif
/*#####################################################*/
#endif /* MCSPI_INTERFACE_H_ */
/*#####################################################*/
