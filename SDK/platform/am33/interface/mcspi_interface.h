/*
 * mcspi_interface.h
 *
 *  Created on: Dec 13, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef MCSPI_INTERFACE_H_
#define MCSPI_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/mcspi_def.h"
/*#####################################################*/
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
