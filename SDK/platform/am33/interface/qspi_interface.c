/*
 * qspi_interface.c
 *
 *  Created on: Oct 11, 2017
 *      Author: Eu
 */

#include <inttypes.h>
#include <api/qspi_def.h>
#include "qspi_interface.h"
#include "qspi_interface_def.h"

/**
  * @brief  Initializes the QSPI interface.
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_Init(Qspi_Mem_t *param)
{
  return -1;
}

/**
  * @brief  De-Initializes the QSPI interface.
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_DeInit(Qspi_Mem_t *param)
{
  return -1;
}

/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_Read(Qspi_Mem_t *param, uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  return -1;
}

/**
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_Write(Qspi_Mem_t *param, uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  return -1;
}

/**
  * @brief  Erases the specified block of the QSPI memory.
  * @param  BlockAddress: Block address to erase
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_Erase_Block(Qspi_Mem_t *param, uint32_t BlockAddress)
{
  return -1;
}

/**
  * @brief  Erases the entire QSPI memory.
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_Erase_Chip(Qspi_Mem_t *param)
{
  return -1;
}

/**
  * @brief  Reads current status of the QSPI memory.
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_GetStatus(Qspi_Mem_t *param)
{
    return -1;
}
#if 0
/**
  * @brief  Reads the configuration of the memory and fills QspiInfo struct
  * @param  pInfo pointer to Info structure
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_GetInfo(QSPI_InfoTypeDef* pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize          = FLASH_SIZE;
  pInfo->EraseSectorSize    = SUBSECTOR_SIZE;
  pInfo->EraseSectorsNumber = (FLASH_SIZE/SUBSECTOR_SIZE);
  pInfo->ProgPageSize       = PAGE_SIZE;
  pInfo->ProgPagesNumber    = (FLASH_SIZE/PAGE_SIZE);

  return QSPI_OK;
}
#endif
/**
  * @brief  Configure the QSPI in memory-mapped mode
  * @param  None
  * @retval QSPI memory status
  */
uint8_t QSPI_N24Q_MemoryMappedMode(Qspi_Mem_t *param)
{
  return -1;
}



