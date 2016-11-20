/*
 * mcspi_interface.h
 *
 * Created: 3/7/2013 6:40:42 PM
 *  Author: XxXx
 */


#ifndef MCSPI_INTERFACE_H_
#define MCSPI_INTERFACE_H_

#include <api/spi_def.h>
#include <stdbool.h>
#include "mcspi_interface.h"
/*#####################################################*/
void _mcspi_assert(Mcspi_t *McspiStruct);
void _mcspi_deassert(Mcspi_t *McspiStruct);
bool _mcspi_open(new_mcspi *McspiStruct);
void _mcspi_close(new_mcspi *McspiStruct);
unsigned char _mcspi_SendByte(Mcspi_t *McspiStruct, unsigned char byte);
bool _mcspi_transfer(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned char *buff_receive, unsigned int size);
bool _mcspi_receive(Mcspi_t *McspiStruct, unsigned char *buff_receive, unsigned int bytes_receive);
bool _mcspi_transmit(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned int bytes_send);
bool _mcspi_set_baud(Mcspi_t *McspiStruct, unsigned long baud);

#ifdef HEADER_INCLUDE_C_FILES
#include "mcspi_interface.c"
#endif

#endif /* MCSPI_INTERFACE_H_ */
