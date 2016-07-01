/*
 * mcspi_interface.h
 *
 * Created: 3/7/2013 6:40:42 PM
 *  Author: XxXx
 */


#ifndef MCSPI_INTERFACE_H_
#define MCSPI_INTERFACE_H_

#include <stdbool.h>
#include "api/mcspi_def.h"

bool _mcspi_open(new_mcspi *McspiStruct);
void _mcspi_close(new_mcspi *McspiStruct);
void _mcspi_assert(Mcspi_t *McspiStruct);
void _mcspi_deassert(Mcspi_t *McspiStruct);
bool _mcspi_transfer(Mcspi_t *McspiStruct);

#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_interface.c"
#endif

#endif /* MCSPI_INTERFACE_H_ */
