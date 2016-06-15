/*
 * hs_mmcsd_interface.h
 *
 * Created: 2/11/2013 3:10:12 PM
 *  Author: XxXx
 */
/*#####################################################*/
#ifndef HS_MMCSD_INTERFACE_H_
#define HS_MMCSD_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/timer_api.h"
#include "api/gpio_def.h"
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
#define IsNoCard						0
#define IsSd							1
#define IsSdhc							2
/*#####################################################*/
timer(sd_timer_delay);
/*#####################################################*/
void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
unsigned int MMCSDReadCmdSend(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks);
void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer);
void _mmcsd_init(unsigned int unit_nr, new_gpio* CardDetect, new_gpio* StatusLed);
void _mmcsd_idle(unsigned int unit_nr);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsd_interface.c"
#endif
/*#####################################################*/
#endif /* HS_MMCSD_INTERFACE_H_ */
/*#####################################################*/
