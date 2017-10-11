/*
 * qspi_interface.h
 *
 *  Created on: Oct 11, 2017
 *      Author: Eu
 */

#ifndef AM33_INTERFACE_QSPI_INTERFACE_H_
#define AM33_INTERFACE_QSPI_INTERFACE_H_

#include <api/qspi_def.h>


unsigned char QSPI_N24Q_Init       (Qspi_Mem_t *param);
unsigned char QSPI_N24Q_DeInit     (Qspi_Mem_t *param);
unsigned char QSPI_N24Q_Read       (Qspi_Mem_t *param, unsigned char* pData, unsigned long ReadAddr, unsigned long Size);
unsigned char QSPI_N24Q_Write      (Qspi_Mem_t *param, unsigned char* pData, unsigned long WriteAddr, unsigned long Size);
unsigned char QSPI_N24Q_Erase_Block(Qspi_Mem_t *param, unsigned long BlockAddress);
unsigned char QSPI_N24Q_Erase_Chip (Qspi_Mem_t *param);
unsigned char QSPI_N24Q_GetStatus  (Qspi_Mem_t *param);
//unsigned char QSPI_N24Q_GetInfo    (Qspi_Mem_t *param, QSPI_InfoTypeDef* pInfo);
unsigned char QSPI_N24Q_MemoryMappedMode(Qspi_Mem_t *param);

/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void QSPI_N24Q_MspInit(void *Params);
void QSPI_N24Q_MspDeInit(void *Params);


#endif /* AM33_INTERFACE_QSPI_INTERFACE_H_ */
