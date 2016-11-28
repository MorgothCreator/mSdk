/*
 * n25qxxx.h
 *
 *  Created on: Nov 20, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_N25QXXX_H_
#define LIB_DEVICE_N25QXXX_H_
//#####################################################
#include "api/qspi_def.h"
//#####################################################
typedef  Qspi_Mem_t N25qxxx_t;

QSPI_Mem_Info_t n25q128a;
QSPI_Mem_Info_t n25q256a;
QSPI_Mem_Info_t n25q512a;
unsigned int 	n25qxxxa_block_read			(void *param, void* pData, unsigned long block, unsigned int nblks);
unsigned int 	n25qxxxa_block_write		(void *param, void* pData, unsigned long block, unsigned int nblks);
unsigned char 	n25qxxxa_block_flush		(void *param);
void 			n25qxxxa_block_ioctl		(void *param, unsigned int  command,  unsigned int *buffer);
//#####################################################
#define new_dev_n25qxxx Qspi_Mem_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_dev_n25qxxx_t(address) free(address);
//#####################################################
#endif /* LIB_DEVICE_N25QXXX_H_ */
