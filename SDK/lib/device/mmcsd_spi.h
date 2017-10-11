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
#include <api/spi_def.h>
#include <interface/mmcsd_interface.h>
#include <stdbool.h>
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/mmcsd_api.h"
#include "lib/fs/fat.h"
#include "lib/fat_fs/inc/ff.h"


/*
 * EXT_CSD struct
 */

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
	struct cardCSD_s
	{
		char csd_structure;
		char spec_vers;
		char taac;
		char nsac;
		char tran_speed;
		unsigned short ccc;
		char read_bl_len;
		char read_bl_partial;
		char write_blk_misalign;
		char read_blk_misalign;
		char dsr_imp;
		unsigned short c_size;
		char vdd_r_curr_min;
		char vdd_r_curr_max;
		char vdd_w_curr_min;
		char vdd_w_curr_max;
		char c_size_mult;
		char erase_grp_size;
		char erase_grp_mult;
		char wp_grp_size;
		char wp_grp_enable;
		char default_ecc;
		char r2w_factor;
		char write_bl_len;
		char write_bl_partial;
		char content_prot_app;
		char file_format_grp;
		char copy;
		char perm_write_protect;
		char tmp_write_protect;
		char file_format;
		char ecc;
		char crc;
	} CSDInfo;


	struct cardCid_s {
		char mid; 					/* Manufacturer ID */
		char cbx;					/* Device/BGA */
		char oid;					/* OEM/Application ID */
		unsigned long int pnm;		/* Product name */
		char prv;					/* Product revision  */
		unsigned long psn;			/* Product serial number */
		char mdt;					/* Manufacturing date */
		char crc;					/* crc7 checksum */
	}cardCid;

	struct extCsd_s {
		// [511:505] Reserved
		char s_cmd_set;		 					// [504] Supported Command Sets
		char hpi_features;						// [503] HPI features
		char bkops_support; 					// [502] Background operations support
		// [501:247] Reserved
		char bkops_status;						// [246] Background operations status
		unsigned int correctly_prg_sectors_num;	// [245:242] Number of correctly programmed sectors
		char ini_timeout_ap;					// [241] 1st initialization time after partitioning
		// [240] Reserved
		char pwr_cl_ddr_52_360;					// [239] Power class for 52MHz, DDR at 3.6V
		char pwr_cl_ddr_52_195;					// [238] Power class for 52MHz, DDR at 1.95V
		// [237:236]
		char min_perf_ddr_w_8_52;				// [235] Minimum Write Performance for 8bit at 52MHz in DDR mode
		char min_perf_ddr_r_8_52;				// [234] Minimum Read Performance for 8bit at 52MHz in DDR mode
		// [233]
		char trim_mult;							// [232] TRIM Multiplier
		char sec_feature_support;				// [231] Secure Feature support
		char sec_erase_mult;					// [230] Secure Erase Multiplier
		char sec_trim_mult;						// [229] Secure TRIM Multiplier
		char boot_info;							// [228] Boot information
		// [227] Reserved
		char boot_size_multi;					// [226] Boot partition size
		char acc_size;							// [225] Access size;
		char hc_erase_grp_size;					// [224] High-capacity erase unit size
		char erase_timeout_mult;				// [223] High-capacity erase timeout
		char rel_wr_sec_c; 						// [222] Reliable write sector count
		char hc_wp_grp_size;					// [221] High-capacity write protect group size
		char s_c_vcc;							// [220] Sleep current (VCC)
		char s_c_vccq;							// [219] Sleep current (VCCQ)
		// [218] Reserved
		char s_a_timeout;						// [217] Sleep/awake timeout
		// [216] Reserved
		unsigned int sec_count;					// [215:212] Sector Count
		// [211] Reserved
		char min_perf_w_8_52;					// [210] Minimum Write Performance for 8bit at 52MHz
		char min_perf_r_8_52;					// [209] Minimum Read Performance for 8bit at 52MHz
		char min_perf_w_8_26_4_52;				// [208] Minimum Write Performance for 8bit at 26MHz, for 4bit at 52MHz
		char min_perf_r_8_26_4_52;				// [207] Minimum Read Performance for 8bit at 26MHz, for 4bit at 52MHz
		char min_perf_w_4_26;					// [206] Minimum Write Performance for 4bit at 26MHz
		char min_perf_r_4_26;					// [205] Minimum Read Performance for 4bit at 26MHz
		// [211] Reserved
		char pwr_cl_26_360;						// [203] Power class for 26MHz at 3.6V
		char pwr_cl_52_360;						// [202] Power class for 52MHz at 3.6V
		char pwr_cl_26_195;						// [201] Power class for 26MHz at 1.95V
		char pwr_cl_52_195;						// [200] Power class for 52MHz at 1.95V
		char partition_switch_time;				// [199] Partition switching timing
		char out_of_interrupt_time;				// [198] Out-of-interrupt busy timing
		// [197] Reserved
		char card_type;							// [196] Card type
		// [195] Reserved
		char csd_structure;						// [194] CSD structure version
		// [193] Reserved
		char ext_csd_rev;						// [192] Extended CSD revision
		char cmd_set;							// [191] Command set
		// [190] Reserved
		char cmd_set_rev;						// [189] Command set revision
		// [188] Reserved
		char power_class;						// [187] Power class
		// [186] Reserved
		char hs_timing;							// [185] High-speed interface timing
		// [184] Reserved
		char bus_width;							// [183] Bus width mode
		// [182] Reserved
		char erased_mem_cont;					// [181] Erased memory content
		// [180] Reserved
		char partition_config;					// [179] Partition configuration
		char boot_config_prot;					// [178] Boot config protection
		char boot_bus_width;					// [177] Boot bus width1
		// [176] Reserved
		char erase_group_def;					// [175] High-density erase group definition
		// [174] Reserved;
		char boot_wp;							// [173] Boot area write protection register
		// [172] Reserved;
		char user_wp;							// [171] User area write protection register
		// [170] Reserved;
		char fw_config;							// [169] FW configuration
		char rpmb_size_mult;					// [168] RPMB Size
		char wr_rel_set; 						// [167] Write reliability setting register
		char wr_rel_param;						// [166] Write reliability parameter register
		// [165] Reserved;
		char bkops_start;						// [164] Manually start background operations
		char bkops_en;							// [163] Enable background operations handshake
		char rst_n_function;					// [162] H/W reset function
		char hpi_mgmt;							// [161] HPI management
		char partitioning_support;				// [160] Partitioning Support
		unsigned char max_enh_size_mult; 		// [159:157] Max Enhanced Area Size
		char partitions_attribute;				// [156] Partitions attribute
		char partition_setting_completed; 		// [155] Paritioning Setting
		unsigned long gp_size_mult[4];			// [154:143] General Purpose Partition Size
		unsigned long enh_size_mult; 			// [142:140] Enhanced User Data Area Size
		unsigned long enh_start_addr;			// [139:136] Enhanced User Data Start Address
		// [135] Reserved;
		char sec_bad_blk_mgmnt;			// [134] Bad Block Management mode
		// [133:0] Reserved
	}extCsd;

	unsigned long raw_scr[2];
	unsigned long raw_csd[4];
	unsigned long raw_cid[4];
	unsigned long blkLen;
	unsigned long nBlks;
	unsigned long long size;
	unsigned char tranSpeed;
	bool SD_Init_OK;
	//void* SD_Structure_Addr;
	//void* SD_SPI_Struct_Addr;
	bool connected;
	unsigned int initFlg;
	mmcsd_type_e SD_Hc;
	Gpio_t * SD_Present;
	unsigned char SpiInstance;
	FATFS g_s_mmcFatFs;
	Mcspi_t *HardUnitStruct;
	bool fs_mounted;
	bool (*HardUnitOpenFunc)(Mcspi_t *HardUnitStruct);
	void (*HardUnitCloseFunc)(Mcspi_t *HardUnitStruct);
	bool (*HardUnitSetBaudFunc)(Mcspi_t *HardUnitStruct, unsigned long baud);
	unsigned char (*HardUnitReadWriteCharFunc)(Mcspi_t *HardUnitStruct, unsigned char data);
	bool (*HardUnitReadWriteBuffFunc)(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned char *buff_receive, unsigned int size);

}SD_Struct_t;
/*#####################################################*/
/* SD Commands enumeration */
#define SD_CMD(x)   (x)

/* Command/Response flags for notifying some information to controller */
#define SD_CMDRSP_NONE			BIT(0)
#define SD_CMDRSP_STOP			BIT(1)
#define SD_CMDRSP_FS			BIT(2)
#define SD_CMDRSP_ABORT			BIT(3)
#define SD_CMDRSP_BUSY			BIT(4)
#define SD_CMDRSP_136BITS		BIT(5)
#define SD_CMDRSP_DATA			BIT(6)
#define SD_CMDRSP_READ			BIT(7)
#define SD_CMDRSP_WRITE			BIT(8)


#define SD_CMDRSP_R1			(0)
#define SD_CMDRSP_R1b			(0 | SD_CMDRSP_BUSY)
#define SD_CMDRSP_R2			(SD_CMDRSP_136BITS)
#define SD_CMDRSP_R3			(SD_CMDRSP_R1)



/* SD voltage enumeration as per VHS field of the interface command */
#define SD_VOLT_2P7_3P6                 (0x000100u)

/* SD OCR register definitions */
/* High capacity */
#define SD_OCR_HIGH_CAPACITY    	BIT(30)
/* Voltage */
#define SD_OCR_VDD_2P7_2P8		BIT(15)
#define SD_OCR_VDD_2P8_2P9		BIT(16)
#define SD_OCR_VDD_2P9_3P0		BIT(17)
#define SD_OCR_VDD_3P0_3P1		BIT(18)
#define SD_OCR_VDD_3P1_3P2		BIT(19)
#define SD_OCR_VDD_3P2_3P3		BIT(20)
#define SD_OCR_VDD_3P3_3P4		BIT(21)
#define SD_OCR_VDD_3P4_3P5		BIT(22)
#define SD_OCR_VDD_3P5_3P6		BIT(23)
/* This is for convenience only. Sets all the VDD fields */
#define SD_OCR_VDD_WILDCARD		(0x1FF << 15)

/* SD CSD register definitions */
#define SD_TRANSPEED_25MBPS		(0x32u)
#define SD_TRANSPEED_50MBPS		(0x5Au)

#define SD_CARD_CSD_VERSION(crd) (((crd)->raw_csd[3] & 0xC0000000) >> 30)

#define SD_CSD0_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x000003FF) << 2) | ((csd1 & 0xC0000000) >> 30))
#define SD_CSD0_MULT(csd3, csd2, csd1, csd0) ((csd1 & 0x00038000) >> 15)
#define SD_CSD0_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD0_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD0_DEV_SIZE(crd) SD_CSD0_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_MULT(crd) SD_CSD0_MULT((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_RDBLKLEN(crd) SD_CSD0_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_TRANSPEED(crd) SD_CSD0_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD0_NUMBLK(crd) ((SD_CARD0_DEV_SIZE((crd)) + 1) * (1 << (SD_CARD0_MULT((crd)) + 2)))
#define SD_CARD0_SIZE(crd) ((SD_CARD0_NUMBLK((crd))) * (1 << (SD_CARD0_RDBLKLEN(crd))))

#define SD_CSD1_DEV_SIZE(csd3, csd2, csd1, csd0) (((csd2 & 0x0000003F) << 16) | ((csd1 & 0xFFFF0000) >> 16))
#define SD_CSD1_RDBLKLEN(csd3, csd2, csd1, csd0) ((csd2 & 0x000F0000) >> 16)
#define SD_CSD1_WRBLKLEN(csd3, csd2, csd1, csd0) ((csd0 & 0x03C00000) >> 22)
#define SD_CSD1_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD1_DEV_SIZE(crd) SD_CSD1_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_RDBLKLEN(crd) SD_CSD1_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_WRBLKLEN(crd) SD_CSD1_WRBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_TRANSPEED(crd) SD_CSD1_TRANSPEED((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_SIZE(crd) ((unsigned long long)(SD_CARD1_DEV_SIZE((crd)) + 1) * (unsigned long long)(512 * 1024))


/* Check RCA/status */
#define SD_RCA_ADDR(rca)             ((rca & 0xFFFF0000) >> 16)
#define SD_RCA_STAT(rca)             (rca & 0x0xFFFF)

/* Check pattern that can be used for card response validation */
#define SD_CHECK_PATTERN   0xAA

/* SD SCR related macros */
#define SD_VERSION_1P0		0
#define SD_VERSION_1P1		1
#define SD_VERSION_2P0		2
#define SD_BUS_WIDTH_1BIT	1
#define SD_BUS_WIDTH_4BIT	4
#define SD_BUS_WIDTH_8BIT	8

/* Helper macros */
/* Note card registers are big endian */
#define SD_CARD_VERSION(sdcard)		((sdcard)->raw_scr[0] & 0xF)
#define SD_CARD_BUSWIDTH(sdcard)	(((sdcard)->raw_scr[0] & 0xF00) >> 8)
#define GET_SD_CARD_BUSWIDTH(sdcard)  ((((sdcard.busWidth) & 0x0F) == 0x01) ? \
                                      0x1 : ((((sdcard).busWidth & 0x04) == \
                                      0x04) ? 0x04 : 0xFF))
#define GET_SD_CARD_FRE(sdcard)	      (((sdcard.tranSpeed) == 0x5A) ? 50 : \
                                      (((sdcard.tranSpeed) == 0x32) ? 25 : 0))

/* Cacheline size */
#ifndef SOC_CACHELINE_SIZE
#define SOC_CACHELINE_SIZE         128
#endif

/* CM6 Swith mode arguments for High Speed */
#define SD_SWITCH_MODE        0x80FFFFFF
#define SD_CMD6_GRP1_SEL      0xFFFFFFF0
#define SD_CMD6_GRP1_HS       0x1
/*#####################################################*/
unsigned int mmcsd_spi_read(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks);
unsigned int mmcsd_spi_write(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks);
void mmcsd_spi_init(unsigned int unit_nr, new_gpio* StatusLed);
void mmcsd_spi_idle(unsigned int unit_nr);
void mmcsd_spi_ioctl(void *_SD_Struct, unsigned int  command,  unsigned int *buffer);
/*#####################################################*/
#define new_mmcsd_spi SD_Struct_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_mmcsd_spi(address) free(address);
/*#####################################################*/
extern new_mmcsd_spi *MMCSD_SPI[];

#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsd_interface.c"
#endif
/*#####################################################*/


#endif /* LIB_DEVICE_MMCSD_SPI_H_ */
