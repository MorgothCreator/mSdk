/*
 * n25qxxx.c
 *
 *  Created on: Nov 20, 2016
 *      Author: John Smith
 */

#include <string.h>
#include <stdbool.h>
#include "n25qxxx.h"
#include "api/qspi_def.h"
#include "api/qspi_api.h"
#include "interface/qspi_interface_def.h"
#include "lib/fat_fs/inc/diskio.h"

bool n25qxxx_block_modifyed = false;
unsigned long n25qxxx_block_in_buffer = -1;
unsigned char n25qxxx_block_buff[4096];

QSPI_Mem_Info_t n25q128a = {
		0x1000000,/*N25Q128A_FLASH_SIZE*/
		0x10000,/*N25Q128A_SECTOR_SIZE*/
		0x1000,/*N25Q128A_SUBSECTOR_SIZE*/
		0x100,/*N25Q128A_PAGE_SIZE*/
		8,/*N25Q128A_DUMMY_CYCLES_READ*/
		10,/*N25Q128A_DUMMY_CYCLES_READ_QUAD*/
		0,/*N25Q128A_DUMMY_CYCLES_READ_DTR*/
		0,/*N25Q128A_DUMMY_CYCLES_READ_QUAD_DTR*/
		250000,/*N25Q128A_BULK_ERASE_MAX_TIME*/
		3000,/*N25Q128A_SECTOR_ERASE_MAX_TIME*/
		800,/*N25Q128A_SUBSECTOR_ERASE_MAX_TIME*/
};

QSPI_Mem_Info_t n25q256a = {
		0x2000000,/*N25Q256A_FLASH_SIZE*/
		0x10000,/*N25Q256A_SECTOR_SIZE*/
		0x1000,/*N25Q256A_SUBSECTOR_SIZE*/
		0x100,/*N25Q256A_PAGE_SIZE*/
		8,/*N25Q256A_DUMMY_CYCLES_READ*/
		10,/*N25Q256A_DUMMY_CYCLES_READ_QUAD*/
		6,/*N25Q256A_DUMMY_CYCLES_READ_DTR*/
		8,/*N25Q256A_DUMMY_CYCLES_READ_QUAD_DTR*/
		480000,/*N25Q256A_BULK_ERASE_MAX_TIME*/
		3000,/*N25Q256A_SECTOR_ERASE_MAX_TIME*/
		800,/*N25Q256A_SUBSECTOR_ERASE_MAX_TIME*/
};

QSPI_Mem_Info_t n25q512a = {
		0x4000000,/*N25Q512A_FLASH_SIZE*/
		0x10000,/*N25Q512A_SECTOR_SIZE*/
		0x1000,/*N25Q512A_SUBSECTOR_SIZE*/
		0x100,/*N25Q512A_PAGE_SIZE*/
		8,/*N25Q512A_DUMMY_CYCLES_READ*/
		10,/*N25Q512A_DUMMY_CYCLES_READ_QUAD*/
		6,/*N25Q512A_DUMMY_CYCLES_READ_DTR*/
		8,/*N25Q512A_DUMMY_CYCLES_READ_QUAD_DTR*/
		480000,/*N25Q512A_BULK_ERASE_MAX_TIME*/
		3000,/*N25Q512A_SECTOR_ERASE_MAX_TIME*/
		800,/*N25Q512A_SUBSECTOR_ERASE_MAX_TIME*/
};

unsigned int n25qxxxa_block_read		(void *param, void* pData, unsigned long block, unsigned int nblks)
{
	Qspi_Mem_t *_param = (Qspi_Mem_t *)param;
	unsigned char status = 0;
	if(n25qxxx_block_in_buffer != (block >> 3))
	{
		if(n25qxxx_block_modifyed)
		{
			status = n25qxxx.write(_param, n25qxxx_block_buff, n25qxxx_block_in_buffer, 4096);
			n25qxxx_block_modifyed = false;
		}
		n25qxxx_block_in_buffer = block >> 3;
		status |= n25qxxx.read(_param, n25qxxx_block_buff, n25qxxx_block_in_buffer, 4096);
	}
	memcpy(pData, n25qxxx_block_buff + ((block & 0x07) << 9), 512);
	if(status)
		return 0;
	else
		return 1;
}
unsigned int n25qxxxa_block_write		(void *param, void* pData, unsigned long block, unsigned int nblks)
{
	Qspi_Mem_t *_param = (Qspi_Mem_t *)param;
	unsigned char status = 0;
	if(n25qxxx_block_in_buffer != (block >> 3))
	{
		if(n25qxxx_block_modifyed)
		{
			status = n25qxxx.write(_param, n25qxxx_block_buff, n25qxxx_block_in_buffer, 4096);
			n25qxxx_block_modifyed = false;
		}
		n25qxxx_block_in_buffer = block >> 3;
		status |= n25qxxx.read(_param, n25qxxx_block_buff, n25qxxx_block_in_buffer, 4096);
	}
	memcpy(n25qxxx_block_buff + ((block & 0x07) << 9), pData, 512);
	if(status)
		return 0;
	else
		return 1;
}

unsigned char n25qxxxa_block_flush		(void *param)
{
	Qspi_Mem_t *_param = (Qspi_Mem_t *)param;
	unsigned char status = 0;
	if(n25qxxx_block_modifyed)
	{
		status = n25qxxx.write(_param, n25qxxx_block_buff, n25qxxx_block_in_buffer, 4096);
		n25qxxx_block_modifyed = false;
	}
	return status;
}

void n25qxxxa_block_ioctl(void *param, unsigned int  command,  unsigned int *buffer)
{
	Qspi_Mem_t *_param = (Qspi_Mem_t *)param;
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           *buffer = _param->dev_info->FlashSize / 512;

            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = 512;

            break;
        }
		case CTRL_SYNC :		/* Make sure that no pending write process */
			if (!n25qxxxa_block_flush(param))
				*buffer = RES_OK;
			break;
       default:
        {
            *buffer = 0;
            break;
        }

    }
}
