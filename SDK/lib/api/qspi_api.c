/*
 * qspi_api.c
 *
 *  Created on: Nov 20, 2016
 *      Author: John Smith
 */

#include "qspi_def.h"
#include "qspi_api.h"
#include "interface/qspi_interface.h"
#include "interface/qspi_interface_def.h"

extern unsigned int 	n25qxxxa_block_read			(void *param, void* pData, unsigned long block, unsigned int nblks);
extern unsigned int 	n25qxxxa_block_write		(void *param, void* pData, unsigned long block, unsigned int nblks);
extern unsigned char 	n25qxxxa_block_flush		(void *param);
extern void 			n25qxxxa_block_ioctl		(void *param, unsigned int  command,  unsigned int *buffer);

const n25qxxx_t n25qxxx = {
		QSPI_N24Q_Init,
		QSPI_N24Q_DeInit,
		QSPI_N24Q_Read,
		QSPI_N24Q_Write,
		n25qxxxa_block_read,
		n25qxxxa_block_write,
		n25qxxxa_block_flush,
		n25qxxxa_block_ioctl,
		QSPI_N24Q_Erase_Block,
		QSPI_N24Q_Erase_Chip,
		QSPI_N24Q_GetStatus,
		QSPI_N24Q_MemoryMappedMode
};
