/*
 * sst25vf.h
 *
 * Created: 3/16/2013 3:44:48 PM
 *  Author: XxXx
 */ 
//#####################################################
#ifndef SST25VF_H_
#define SST25VF_H_
//#####################################################
#include "api/mcspi_def.h"
#include "api/gpio_def.h"
//#####################################################
#define SST25VF_Read				0x03
#define SST25VF_HighSpeedRead		0x0B
#define SST25VF_4K_SectorErase		0x20
#define SST25VF_32K_SectorErase		0x52
#define SST25VF_64K_SectorErase		0xD8
#define SST25VF_ChipErase			0x60
#define SST25VF_ByteProgram			0x02
#define SST25VF_WordProgram			0xAD
#define SST25VF_RDSR				0x05
#define SST25VF_EWSR				0x50
#define SST25VF_WRSR				0x01
#define SST25VF_WREN				0x06
#define SST25VF_WRDI				0x04
#define SST25VF_RDID				0x90
#define SST25VF_JdecID				0x9F
#define SST25VF_EBSY				0x70
#define SST25VF_DBSY				0x80
//-----------------------------------------------------
//RDSR reg bits description
#define SST25VF_SR_BUSY_bp			0
#define SST25VF_SR_BUSY_bm			(1 << SST25VF_SR_BUSY_bp)
#define SST25VF_SR_MEM_WR_EN_bp		1
#define SST25VF_SR_MEM_WR_EN_bm		(1 << SST25VF_SR_MEM_WR_EN_bp)
#define SST25VF_SR_AAI_bp			6
#define SST25VF_SR_AAI_bm			(1 << SST25VF_SR_AAI_bp)
#define SST25VF_SR_ALL_BLOCK_PROTECTION_bp			2
#define SST25VF_SR_ALL_BLOCK_PROTECTION_bm			(0x0F << SST25VF_SR_ALL_BLOCK_PROTECTION_bp)
//#####################################################
typedef struct F25xxx_Struct
{
	Mcspi_t* F25xxx_Spi_Struct;
	Gpio_t *WP_Port;
	Gpio_t *Hold_Port;
	unsigned char F25xxx_Id[5];
}sst25vf_t;
//#####################################################
#define new_sst25vf sst25vf_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_sst25vf(address) free(address);
//#####################################################
bool sst25vf_init(sst25vf_t *Settings, unsigned char Wp_PortNr, unsigned char Wp_PinNr, unsigned char Hold_PortNr, unsigned char Hold_PinNr);
unsigned char sst25vf_read_status(sst25vf_t *Settings, unsigned char *Data);
unsigned char sst25vf_write_status(sst25vf_t *Settings, unsigned char Data);
bool sst25vf_enable_block_protection(sst25vf_t *Settings);
bool sst25vf_disable_block_protection(sst25vf_t *Settings);
bool sst25vf_read_busy(sst25vf_t *Settings);
bool sst25vf_read_write_enable(sst25vf_t *Settings);
void sst25vf_wait_busy(sst25vf_t *Settings);
void sst25vf_wait_write_enable(sst25vf_t *Settings);
bool sst25vf_write_enable(sst25vf_t *Settings);
bool sst25vf_write_disable(sst25vf_t *Settings);
bool sst25vf_sector_erase(sst25vf_t *Settings, unsigned long addr, unsigned char sector_size, bool wait_erase_complete);
bool sst25vf_chip_erase(sst25vf_t *Settings, bool wait_erase_complete);
bool sst25vf_write_word(sst25vf_t *Settings, unsigned long addr, unsigned char Chr1, unsigned char Chr2);
bool sst25vf_write(sst25vf_t *Settings, unsigned long addr, unsigned int num_of_bytes, unsigned char *buff);
bool sst25vf_read(sst25vf_t *Settings, unsigned long addr, unsigned int num_of_bytes, unsigned char *buff);/* the buffer must be minim num_of_bytes + 4 in size */
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "sst25vf.c"
#endif
//#####################################################
#endif /* SST25VF_H_ */
//#####################################################
