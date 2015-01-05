/*
 * sst25vf.c
 *
 * Created: 3/16/2013 3:44:38 PM
 *  Author: XxXx
 */

#include "sst25vf.h"
#include "api/mcspi_def.h"
#include "api/mcspi_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"

bool sst25vf_init(sst25vf_t *Settings, unsigned char Wp_PortNr, unsigned char Wp_PinNr, unsigned char Hold_PortNr, unsigned char Hold_PinNr)
{
	Settings->WP_Port = gpio_assign(Wp_PortNr, Wp_PinNr, GPIO_OUT_PUSH_PULL, false);
	Settings->Hold_Port = gpio_assign(Hold_PortNr, Hold_PinNr, GPIO_OUT_PUSH_PULL, false);
	if(!Settings->WP_Port && !Settings->Hold_Port)
	{
		gpio_free(Settings->WP_Port);
		gpio_free(Settings->Hold_Port);
		return false;
	}
	gpio_out(Settings->WP_Port, 1);
	gpio_out(Settings->Hold_Port, 1);
	return true;
}

unsigned char sst25vf_read_status(sst25vf_t *Settings, unsigned char *Data)
{
	if(!Settings) return false;
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_RDSR;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 1);
	*Data = Settings->F25xxx_Spi_Struct->Buff[1];
	return true;
}

unsigned char sst25vf_write_status(sst25vf_t *Settings, unsigned char Data)
{
	if(!Settings) return false;
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_EWSR;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_WRSR;
	Settings->F25xxx_Spi_Struct->Buff[1] = Data;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 2, 0);
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
	if(!sst25vf_read_status(Settings, &Response)) return false;
	if(Response & SST25VF_SR_BUSY_bm) return true;
	return false;
}

bool sst25vf_read_write_enable(sst25vf_t *Settings)
{
	unsigned char Response = 0;
	if(!sst25vf_read_status(Settings, &Response)) return false;
	if(Response & SST25VF_SR_MEM_WR_EN_bm) return true;
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
	if(!Settings) return false;
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_WREN;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	return true;
}

bool sst25vf_write_disable(sst25vf_t *Settings)
{
	if(!Settings) return false;
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_WRDI;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	return true;
}

bool sst25vf_sector_erase(sst25vf_t *Settings, unsigned long addr, unsigned char sector_size, bool wait_erase_complete)
{
	if(!Settings) return false;
	sst25vf_wait_busy(Settings);
	sst25vf_disable_block_protection(Settings);
	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	switch (sector_size)
	{
	case 4:
		Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_4K_SectorErase;
		break;
	case 32:
		Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_32K_SectorErase;
		break;
	case 64:
		Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_64K_SectorErase;
		break;
	default:
		return false;
	}
	Settings->F25xxx_Spi_Struct->Buff[1] = addr >> 16;
	Settings->F25xxx_Spi_Struct->Buff[2] = addr >> 8;
	Settings->F25xxx_Spi_Struct->Buff[3] = addr;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 4, 0);
	if(wait_erase_complete) sst25vf_wait_busy(Settings);
	sst25vf_enable_block_protection(Settings);
	return true;
}

bool sst25vf_chip_erase(sst25vf_t *Settings, bool wait_erase_complete)
{
	if(!Settings) return false;
	sst25vf_wait_busy(Settings);
	sst25vf_disable_block_protection(Settings);
	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_ChipErase;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 1, 0);
	if(wait_erase_complete) sst25vf_wait_busy(Settings);
	sst25vf_enable_block_protection(Settings);
	return true;
}

bool sst25vf_write_word(sst25vf_t *Settings, unsigned long addr, unsigned char Chr1, unsigned char Chr2)
{
	sst25vf_wait_busy(Settings);

	sst25vf_write_enable(Settings);
	sst25vf_wait_write_enable(Settings);
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_WordProgram;
	Settings->F25xxx_Spi_Struct->Buff[1] = addr >> 16;
	Settings->F25xxx_Spi_Struct->Buff[2] = addr >> 8;
	Settings->F25xxx_Spi_Struct->Buff[3] = addr;
	Settings->F25xxx_Spi_Struct->Buff[4] = Chr1;
	Settings->F25xxx_Spi_Struct->Buff[5] = Chr2;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 6, 0);
	sst25vf_wait_busy(Settings);
	sst25vf_write_disable(Settings);
	sst25vf_wait_busy(Settings);

	return true;
}

bool sst25vf_write(sst25vf_t *Settings, unsigned long addr, unsigned int num_of_bytes, unsigned char *buff)
{
	if(!Settings) return false;
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
		Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_ByteProgram;
		Settings->F25xxx_Spi_Struct->Buff[1] = CntAddr >> 16;
		Settings->F25xxx_Spi_Struct->Buff[2] = CntAddr >> 8;
		Settings->F25xxx_Spi_Struct->Buff[3] = CntAddr;
		Settings->F25xxx_Spi_Struct->Buff[4] = *Buff++;
		mcspi_transfer(Settings->F25xxx_Spi_Struct, 5, 0);
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
	if(!Settings) return false;
	sst25vf_wait_busy(Settings);
	unsigned char *BackBuff = (unsigned char *)Settings->F25xxx_Spi_Struct->Buff;
	Settings->F25xxx_Spi_Struct->Buff = buff;
	Settings->F25xxx_Spi_Struct->Buff[0] = SST25VF_Read;
	Settings->F25xxx_Spi_Struct->Buff[1] = addr >> 16;
	Settings->F25xxx_Spi_Struct->Buff[2] = addr >> 8;
	Settings->F25xxx_Spi_Struct->Buff[3] = addr;
	mcspi_transfer(Settings->F25xxx_Spi_Struct, 4, num_of_bytes);
	Settings->F25xxx_Spi_Struct->Buff = BackBuff;
	unsigned int TmpCnt = 0;
	for(; TmpCnt < num_of_bytes; TmpCnt++) buff[TmpCnt] = buff[TmpCnt + 4];
	return true;
}


