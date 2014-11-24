/*
 * \file	mmcsd_proto.h
 *
 * \brief	MMC/SD definitions
 *
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __MMCSD_PROTO_H__
#define __MMCSD_PROTO_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#define BIT(x) (1 << x)

/**
 * SD Card information structure
 */
#define MMCSD_CARD_SD		       (0u)
#define MMCSD_CARD_MMC		       (1u)

struct _mmcsdCtrlInfo;

/*
 * EXT_CSD struct
 */
typedef struct _extCsd {
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
	unsigned int gp_size_mult[4];			// [154:143] General Purpose Partition Size
	unsigned int enh_size_mult; 			// [142:140] Enhanced User Data Area Size
	unsigned int enh_start_addr;			// [139:136] Enhanced User Data Start Address
	// [135] Reserved;
	char sec_bad_blk_mgmnt;			// [134] Bad Block Management mode
	// [133:0] Reserved
}MMC_extCsd;

/* Structure for SD Card information */
typedef struct _mmcsdCardInfo {
    struct _mmcsdCtrlInfo *ctrl;
	unsigned int cardType;
	unsigned int rca;
	unsigned int raw_scr[2];
	unsigned int raw_csd[4];
	unsigned int raw_cid[4];
	unsigned int ocr;
	unsigned char sd_ver;
	unsigned char busWidth;
	unsigned char tranSpeed;
	unsigned char highCap;
	unsigned int blkLen;
	unsigned int nBlks;
	unsigned long long size;
}mmcsdCardInfo;

/* Structure for command */
typedef struct _mmcsdCmd {
	unsigned int idx;
	unsigned int flags;
	unsigned int arg;
	signed char *data;
	unsigned int nblks;
	unsigned int rsp[4];
}mmcsdCmd;

/* Structure for controller information */
typedef struct _mmcsdCtrlInfo {
	unsigned int memBase;
	unsigned int ipClk;
	unsigned int opClk;
	unsigned int (*ctrlInit) (struct _mmcsdCtrlInfo *ctrl);
	unsigned int (*cmdSend) (struct _mmcsdCtrlInfo *ctrl, mmcsdCmd *c);
    void (*busWidthConfig) (struct _mmcsdCtrlInfo *ctrl, unsigned int busWidth);
    int (*busFreqConfig) (struct _mmcsdCtrlInfo *ctrl, unsigned int busFreq);
	unsigned int (*cmdStatusGet) (struct _mmcsdCtrlInfo *ctrl);
	unsigned int (*xferStatusGet) (struct _mmcsdCtrlInfo *ctrl);
	void (*xferSetup) (struct _mmcsdCtrlInfo *ctrl, unsigned char rwFlag,
					       void *ptr, unsigned int blkSize, unsigned int nBlks);
    unsigned int (*cardPresent) (struct _mmcsdCtrlInfo *ctrl);
    void (*intrEnable) (struct _mmcsdCtrlInfo *ctrl);
    unsigned int intrMask;
	unsigned int dmaEnable;
	unsigned int busWidth;
	unsigned int highspeed;
	unsigned int ocr;
        unsigned int cdPinNum;
        unsigned int wpPinNum;
	mmcsdCardInfo *card;
	unsigned int SdNr;
	bool connected;
}mmcsdCtrlInfo;

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
#define SD_CSD1_TRANSPEED(csd3, csd2, csd1, csd0) ((csd3 & 0x000000FF) >> 0)

#define SD_CARD1_DEV_SIZE(crd) SD_CSD1_DEV_SIZE((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
#define SD_CARD1_RDBLKLEN(crd) SD_CSD1_RDBLKLEN((crd)->raw_csd[3], (crd)->raw_csd[2], (crd)->raw_csd[1], (crd)->raw_csd[0])
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

/*
 * Function prototypes
 */

unsigned int MMCSDReadCmdSend(void *_ctrl, void *ptr, unsigned long block,
				                     unsigned int blks);
unsigned int MMCSDWriteCmdSend(void *_ctrl, void *ptr, unsigned long block,
				                       unsigned int blks);
unsigned int MMCSDAppCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c);
unsigned int MMCSDCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c);
unsigned int MMCSDTranSpeedSet(mmcsdCtrlInfo *ctrl);
unsigned int MMCSDBusWidthSet(mmcsdCtrlInfo *ctrl);
unsigned int MMCSDStopCmdSend(mmcsdCtrlInfo *ctrl);
unsigned int MMCSDCardPresent(mmcsdCtrlInfo *ctrl);
unsigned int MMCSDCardReset(mmcsdCtrlInfo *ctrl);
unsigned int MMCSDCardInit(mmcsdCtrlInfo *ctrl);
unsigned int MMCSDCtrlInit(mmcsdCtrlInfo *ctrl);
void MMCSDIntEnable(mmcsdCtrlInfo *ctrl);
#ifdef __cplusplus
}
#endif

#ifdef HEADER_INCLUDE_C_FILES
#include "mmcsd_proto.c"
#endif

#endif
