/*
 * sst25vf.c
 *
 * Created: 3/16/2013 3:44:38 PM
 *  Author: XxXx
 */

#include <api/spi_api.h>
#include <api/spi_def.h>
#include "sst25vf.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "lib/fat_fs/inc/ff.h"

bool sst25vf_init(sst25vf_t *Settings)
{
	if(Settings->wp)
		gpio.out(Settings->wp, 1);
	if(Settings->hold)
		gpio.out(Settings->hold, 1);
	return true;
}

unsigned char sst25vf_read_status(sst25vf_t *Settings, unsigned char *Data)
{
	if(!Settings)
		return false;
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_RDSR);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 1);
	*Data = spi.trx_byte(Settings->F25xxx_Spi_Struct, 0xFF);
	return true;
}

unsigned char sst25vf_write_status(sst25vf_t *Settings, unsigned char Data)
{
	if(!Settings)
		return false;
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_EWSR);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_WRSR);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, Data);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 2, 0);
	return true;
}

bool sst25vf_enable_block_protection(sst25vf_t *Settings)
{
	unsigned char Response = 0;
	sst25vf_read_status(Settings, &Response);
	sst25vf_write_status(Settings, Response | SST25VF_SR_ALL_BLOCK_PROTECTION_bm);
	return false;
}

bool sst25vf_disable_block_protection(sst25vf_t *Settings)
{
	unsigned char Response = 0;
	sst25vf_read_status(Settings, &Response);
	sst25vf_write_status(Settings, Response & ~SST25VF_SR_ALL_BLOCK_PROTECTION_bm);
	return false;
}

bool sst25vf_read_busy(sst25vf_t *Settings)
{
	unsigned char Response = 0;
	if(!sst25vf_read_status(Settings, &Response))
		return false;
	if(Response & SST25VF_SR_BUSY_bm)
		return true;
	return false;
}

bool sst25vf_read_write_enable(sst25vf_t *Settings)
{
	unsigned char Response = 0;
	if(!sst25vf_read_status(Settings, &Response))
		return false;
	if(Response & SST25VF_SR_MEM_WR_EN_bm)
		return true;
	return false;
}

void sst25vf_wait_busy(sst25vf_t *Settings)
{
	while(sst25vf_read_busy(Settings));
}

void sst25vf_wait_write_enable(sst25vf_t *Settings)
{
	while(!sst25vf_read_write_enable(Settings));
}

bool sst25vf_write_enable(sst25vf_t *Settings)
{
	if(!Settings)
		return false;
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_WREN);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	return true;
}

bool sst25vf_write_disable(sst25vf_t *Settings)
{
	if(!Settings)
		return false;
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_WRDI);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	return true;
}

bool sst25vf_sector_erase(sst25vf_t *Settings, unsigned long addr, unsigned char sector_size, bool wait_erase_complete)
{
	if(!Settings)
		return false;
	sst25vf_wait_busy(Settings);
	sst25vf_disable_block_protection(Settings);
	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	spi.assert(Settings->F25xxx_Spi_Struct);
	switch (sector_size)
	{
	case 4:
		spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_4K_SectorErase);
		break;
	case 32:
		spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_32K_SectorErase);
		break;
	case 64:
		spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_64K_SectorErase);
		break;
	default:
		return false;
	}
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr >> 16);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr >> 8);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 4, 0);
	if(wait_erase_complete)
		sst25vf_wait_busy(Settings);
	sst25vf_enable_block_protection(Settings);
	return true;
}

bool sst25vf_chip_erase(sst25vf_t *Settings, bool wait_erase_complete)
{
	if(!Settings)
		return false;
	sst25vf_wait_busy(Settings);
	sst25vf_disable_block_protection(Settings);
	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_ChipErase);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	if(wait_erase_complete)
		sst25vf_wait_busy(Settings);
	sst25vf_enable_block_protection(Settings);
	return true;
}

bool sst25vf_write_word(sst25vf_t *Settings, unsigned long addr, unsigned char Chr1, unsigned char Chr2)
{
	sst25vf_wait_busy(Settings);

	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_WordProgram);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr >> 16);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr >> 8);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, Chr1);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, Chr2);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//mcspi_transfer(Settings->F25xxx_Spi_Struct, 6, 0);
	sst25vf_wait_busy(Settings);
	sst25vf_write_disable(Settings);
	sst25vf_wait_busy(Settings);

	return true;
}

bool sst25vf_write(sst25vf_t *Settings, unsigned long addr, unsigned int num_of_bytes, unsigned char *buff)
{
	if(!Settings)
		return false;
	volatile unsigned int CntBytes = num_of_bytes;
	volatile unsigned long CntAddr = addr;
	volatile unsigned char *Buff = buff;
	sst25vf_wait_busy(Settings);
	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	sst25vf_disable_block_protection(Settings);
	while(CntBytes > 1)
	{
		unsigned char Char1 = *Buff++;
		unsigned char Char2 = *Buff++;
		sst25vf_write_word(Settings, CntAddr, Char1, Char2);
		CntAddr += 2;
		CntBytes -= 2;
	}
	if(CntBytes)
	{
		sst25vf_write_enable(Settings);
		sst25vf_wait_write_enable(Settings);
		spi.assert(Settings->F25xxx_Spi_Struct);
		spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_ByteProgram);
		spi.trx_byte(Settings->F25xxx_Spi_Struct, CntAddr >> 16);
		spi.trx_byte(Settings->F25xxx_Spi_Struct, CntAddr >> 8);
		spi.trx_byte(Settings->F25xxx_Spi_Struct, CntAddr);
		spi.trx_byte(Settings->F25xxx_Spi_Struct, *Buff++);
		spi.deassert(Settings->F25xxx_Spi_Struct);
		//mcspi_transfer(Settings->F25xxx_Spi_Struct, 5, 0);
		sst25vf_wait_busy(Settings);
		sst25vf_write_disable(Settings);
		sst25vf_wait_busy(Settings);
	}
	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	sst25vf_enable_block_protection(Settings);
	return true;
}

bool sst25vf_read(sst25vf_t *Settings, unsigned long addr, unsigned int num_of_bytes, unsigned char *buff)/* the buffer must be minim num_of_bytes + 4 in size */
{
	if(!Settings)
		return false;
	sst25vf_wait_busy(Settings);
	//unsigned char *BackBuff = (unsigned char *)Settings->F25xxx_Spi_Struct->Buff;
	//Settings->F25xxx_Spi_Struct->Buff = buff;
	spi.assert(Settings->F25xxx_Spi_Struct);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, SST25VF_Read);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr >> 16);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr >> 8);
	spi.trx_byte(Settings->F25xxx_Spi_Struct, addr);
	spi.receive(Settings->F25xxx_Spi_Struct, buff, num_of_bytes);
	spi.deassert(Settings->F25xxx_Spi_Struct);
	//Settings->F25xxx_Spi_Struct->Buff = BackBuff;
	//unsigned int TmpCnt = 0;
	//for(; TmpCnt < num_of_bytes; TmpCnt++)
		//buff[TmpCnt] = buff[TmpCnt + 4];
	return true;
}

/*void sst25vf_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           *buffer = 1048576 / 512;
            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = 512;
            break;
        }
        default:
        {
            *buffer = 0;
            break;
        }
    }
}*/


