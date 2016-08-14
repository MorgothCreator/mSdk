/*
 * mmcsd_spi.h
 *
 *  Created on: Feb 9, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_MMCSD_SPI_H_
#define LIB_DEVICE_MMCSD_SPI_H_
/*#####################################################*/
//#include <avr/io.h>
#include <stdbool.h>
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/mcspi_def.h"
#include "lib/fs/fat.h"
#include "lib/fat_fs/inc/ff.h"
#include "interface/hs_mmcsd_interface.h"
/*#####################################################*/
#define _Disk_ErrInit					1
#define _Disk_ErrReset					2
#define _Disk_InitOk					3
#define _Disk_Busy						4
#define _Disk_DelayWrite				5
#define _Disk_DelayRead					6
#define _Disk_ReleaseError				7
#define _Disk_ReadWriteOk				8
#define _Disk_ReadInitError				9
#define _Disk_ErrVoltageRange			10
/*#####################################################*/
#define MMC_CMD_RESET                 	0
#define MMC_CMD_START_INITIALIZATION  	1
#define MMC_SEND_IF_COND		 		8
#define MMC_CMD_SEND_CSD              	9
#define MMC_CMD_SEND_CID             	10
#define MMC_CMD_STOP_TRANSMISSION    	12
#define MMC_CMD_SEND_STATUS          	13
#define MMC_CMD_SET_BLOCK_SIZE       	16
#define MMC_CMD_READ_SINGLE_BLOCK    	17
#define MMC_CMD_READ_MULTIPLE_BLOCKS 	18
#define MMC_CMD_WRITE_SINGLE_BLOCK   	24
#define MMC_CMD_WRITE_MULTIPLE_BLOCKS	25
#define MMC_CMD_PROGRAM_CSD          	27
#define MMC_CMD_SET_WRITE_PROT       	28
#define MMC_CMD_CLR_WRITE_PROT       	29
#define MMC_CMD_SEND_WRITE_PROT      	30
#define MMC_CMD_TAG_SECTOR_START     	32
#define MMC_CMD_TAG_SECTOR_END       	33
#define MMC_CMD_UNTAG_SECTOR         	34
#define MMC_CMD_TAG_ERASE_GROUP_START	35
#define MMC_CMD_TAG_ERASE_GROUP_END  	36
#define MMC_CMD_UNTAG_ERASE_GROUP    	37
#define MMC_CMD_ERASE                	38
#define MMC_CMD_SD_SEND_OP_COND			41
#define MMC_CMD_LOCK_UNLOCK          	42
#define MMC_CMD_APP_CMD					55
#define MMC_CMD_READ_OCR             	58
#define MMC_CMD_CRC_ON_OFF           	59
/*#####################################################*/
// SPI Response Flags
#define MMC_IN_IDLE_STATE				0x01
#define MMC_ERASE_RESET					0x02
#define MMC_ILLEGAL_COMMAND				0x04
#define MMC_COM_CRC_ERROR				0x08
#define MMC_ERASE_ERROR					0x10
#define MMC_ADRESS_ERROR				0x20
#define MMC_PARAMETER_ERROR				0x40
/*#####################################################*/
#define SD_RAW_SPEC_1 					0
#define SD_RAW_SPEC_2 					1
#define SD_RAW_SPEC_SDHC 				2
/*#####################################################*/
#define MMC_DATA_TOKEN              	0xFE
/*#####################################################*/
#define MMC_RESPONSE_OK             	0x05
#define MMC_RESPONSE_CRC_ERROR      	0x0B
#define MMC_RESPONSE_WRITE_ERROR    	0x0D
#define MMC_RESPONSE_MASK           	0x1F
/*#####################################################*/
#define DelayWriteCommand           	65536
#define DelayReadCommand            	1024
#define SdCard_NumberToTryResetCmd		100
#define SdCard_NumberToTryIfCondCmd		254
#define SdCard_NumberToTryInitCmdSd		30
#define SdCard_NumberToTryInitCmdSdHc	254
#define SdDriverDelaysConstant			20
#define NumberOfIdleBytes				65536
/*#####################################################*/
timer(sd_timer_delay);
/*#####################################################*/
typedef struct SD_Init_Return
{
	unsigned char SD_Init_OK;
	void* SD_Structure_Addr;
}SD_Init_Return_t;
/*#####################################################*/
typedef struct SD_Struct
{
	bool SD_Init_OK;
	//void* SD_Structure_Addr;
	//void* SD_SPI_Struct_Addr;
	bool connected;
	unsigned int initFlg;
	mmcsd_type_e SD_Hc;
	Gpio_t * CS_Port;
	Gpio_t * SD_Present;
	unsigned char CS_PinMask;
	unsigned char DriveNr;
	FATFS g_s_mmcFatFs;
	Mcspi_t *HardUnitStruct;
	bool (*HardUnitOpenFunc)(Mcspi_t *HardUnitStruct);
	void (*HardUnitCloseFunc)(Mcspi_t *HardUnitStruct);
	bool (*HardUnitSetBaudFunc)(Mcspi_t *HardUnitStruct, unsigned long baud);
	unsigned char (*HardUnitReadWriteCharFunc)(Mcspi_t *HardUnitStruct, unsigned char data);
}SD_Struct_t;
/*#####################################################*/
unsigned int MMCSD_SPI_ReadCmdSend(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks);
unsigned int MMCSD_SPI_WriteCmdSend(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks);
void mmcsd_spi_init(unsigned int unit_nr, new_gpio* Cs, new_gpio* StatusLed);
void mmcsd_spi_idle(unsigned int unit_nr);
void mmcsd_spi_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
/*#####################################################*/
#define new_mmcsd_spi SD_Struct_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_mmcsd_spi(address) free(address);

#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsd_interface.c"
#endif
/*#####################################################*/


#endif /* LIB_DEVICE_MMCSD_SPI_H_ */
