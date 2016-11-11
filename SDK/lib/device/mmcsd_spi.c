/*
 * mmcsd_spi.c
 *
 *  Created on: Feb 9, 2016
 *      Author: John Smith
 */


#include <stdbool.h>
//#include "board_properties.h"
#include "mmcsd_spi.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "lib/fs/fat.h"
#include "api/mcspi_api.h"
//#include "general/unions.h"
#include "sys/sysdelay.h"
//#######################################################################################
//new_dma_ch* DMA_MMCSD_TRANSMIT = NULL;
//new_dma_ch* DMA_MMCSD_RECEIVE = NULL;

DIR g_sDirObject;

/* MMC/SD command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define CMD2	(2)			/* SEND_CID */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */
#define CMD59	(59)		/* CRC_ON_OFF */

/* Card type flags (CardType) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK	0x08		/* Block addressing */


/*
 * EXT_CSD switch cmd macros
 */

#define EXT_CSD_FLUSH_CACHE             32      /* W */
#define EXT_CSD_CACHE_CTRL              33      /* R/W */
#define EXT_CSD_POWER_OFF_NOTIFICATION  34      /* R/W */
#define EXT_CSD_PACKED_FAILURE_INDEX    35      /* RO */
#define EXT_CSD_PACKED_CMD_STATUS       36      /* RO */
#define EXT_CSD_EXP_EVENTS_STATUS       54      /* RO, 2 bytes */
#define EXT_CSD_EXP_EVENTS_CTRL         56      /* R/W, 2 bytes */
#define EXT_CSD_DATA_SECTOR_SIZE        61      /* R */
#define EXT_CSD_GP_SIZE_MULT            143     /* R/W */
#define EXT_CSD_PARTITION_ATTRIBUTE     156     /* R/W */
#define EXT_CSD_PARTITION_SUPPORT       160     /* RO */
#define EXT_CSD_HPI_MGMT                161     /* R/W */
#define EXT_CSD_RST_N_FUNCTION          162     /* R/W */
#define EXT_CSD_BKOPS_EN                163     /* R/W */
#define EXT_CSD_BKOPS_START             164     /* W */
#define EXT_CSD_SANITIZE_START          165     /* W */
#define EXT_CSD_WR_REL_PARAM            166     /* RO */
#define EXT_CSD_RPMB_MULT               168     /* RO */
#define EXT_CSD_BOOT_WP                 173     /* R/W */
#define EXT_CSD_ERASE_GROUP_DEF         175     /* R/W */
#define EXT_CSD_PART_CONFIG             179     /* R/W */
#define EXT_CSD_ERASED_MEM_CONT         181     /* RO */
#define EXT_CSD_BUS_WIDTH               183     /* R/W */
#define EXT_CSD_HS_TIMING               185     /* R/W */
#define EXT_CSD_POWER_CLASS             187     /* R/W */
#define EXT_CSD_REV                     192     /* RO */
#define EXT_CSD_STRUCTURE               194     /* RO */
#define EXT_CSD_CARD_TYPE               196     /* RO */
#define EXT_CSD_OUT_OF_INTERRUPT_TIME   198     /* RO */
#define EXT_CSD_PART_SWITCH_TIME        199     /* RO */
#define EXT_CSD_PWR_CL_52_195           200     /* RO */
#define EXT_CSD_PWR_CL_26_195           201     /* RO */
#define EXT_CSD_PWR_CL_52_360           202     /* RO */
#define EXT_CSD_PWR_CL_26_360           203     /* RO */
#define EXT_CSD_SEC_CNT                 212     /* RO, 4 bytes */
#define EXT_CSD_S_A_TIMEOUT             217     /* RO */
#define EXT_CSD_REL_WR_SEC_C            222     /* RO */
#define EXT_CSD_HC_WP_GRP_SIZE          221     /* RO */
#define EXT_CSD_ERASE_TIMEOUT_MULT      223     /* RO */
#define EXT_CSD_HC_ERASE_GRP_SIZE       224     /* RO */
#define EXT_CSD_BOOT_MULT               226     /* RO */
#define EXT_CSD_SEC_TRIM_MULT           229     /* RO */
#define EXT_CSD_SEC_ERASE_MULT          230     /* RO */
#define EXT_CSD_SEC_FEATURE_SUPPORT     231     /* RO */
#define EXT_CSD_TRIM_MULT               232     /* RO */
#define EXT_CSD_PWR_CL_200_195          236     /* RO */
#define EXT_CSD_PWR_CL_200_360          237     /* RO */
#define EXT_CSD_PWR_CL_DDR_52_195       238     /* RO */
#define EXT_CSD_PWR_CL_DDR_52_360       239     /* RO */
#define EXT_CSD_BKOPS_STATUS            246     /* RO */
#define EXT_CSD_POWER_OFF_LONG_TIME     247     /* RO */
#define EXT_CSD_GENERIC_CMD6_TIME       248     /* RO */
#define EXT_CSD_CACHE_SIZE              249     /* RO, 4 bytes */
#define EXT_CSD_TAG_UNIT_SIZE           498     /* RO */
#define EXT_CSD_DATA_TAG_SUPPORT        499     /* RO */
#define EXT_CSD_MAX_PACKED_WRITES       500     /* RO */
#define EXT_CSD_MAX_PACKED_READS        501     /* RO */
#define EXT_CSD_BKOPS_SUPPORT           502     /* RO */
#define EXT_CSD_HPI_FEATURES            503     /* RO */

/*
 * EXT_CSD field definitions
 */

#define EXT_CSD_WR_REL_PARAM_EN         (1<<2)

#define EXT_CSD_BOOT_WP_B_PWR_WP_DIS    (0x40)
#define EXT_CSD_BOOT_WP_B_PERM_WP_DIS   (0x10)
#define EXT_CSD_BOOT_WP_B_PERM_WP_EN    (0x04)
#define EXT_CSD_BOOT_WP_B_PWR_WP_EN     (0x01)

#define EXT_CSD_PART_CONFIG_ACC_MASK    (0x7)
#define EXT_CSD_PART_CONFIG_ACC_BOOT0   (0x1)
#define EXT_CSD_PART_CONFIG_ACC_RPMB    (0x3)
#define EXT_CSD_PART_CONFIG_ACC_GP0     (0x4)

#define EXT_CSD_PART_SUPPORT_PART_EN    (0x1)

#define EXT_CSD_CMD_SET_NORMAL          (1<<0)
#define EXT_CSD_CMD_SET_SECURE          (1<<1)
#define EXT_CSD_CMD_SET_CPSECURE        (1<<2)

#define EXT_CSD_CARD_TYPE_26    (1<<0)  /* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52    (1<<1)  /* Card can run at 52MHz */
#define EXT_CSD_CARD_TYPE_MASK  0x3F    /* Mask out reserved bits */
#define EXT_CSD_CARD_TYPE_DDR_1_8V  (1<<2)   /* Card can run at 52MHz */
                                              /* DDR mode @1.8V or 3V I/O */
#define EXT_CSD_CARD_TYPE_DDR_1_2V  (1<<3)   /* Card can run at 52MHz */
                                              /* DDR mode @1.2V I/O */
#define EXT_CSD_CARD_TYPE_DDR_52       (EXT_CSD_CARD_TYPE_DDR_1_8V  \
                                         | EXT_CSD_CARD_TYPE_DDR_1_2V)
 #define EXT_CSD_CARD_TYPE_SDR_1_8V      (1<<4)  /* Card can run at 200MHz */
#define EXT_CSD_CARD_TYPE_SDR_1_2V      (1<<5)  /* Card can run at 200MHz */
                                                 /* SDR mode @1.2V I/O */

#define EXT_CSD_BUS_WIDTH_1     0       /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4     1       /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8     2       /* Card is in 8 bit mode */
#define EXT_CSD_DDR_BUS_WIDTH_4 5       /* Card is in 4 bit DDR mode */
#define EXT_CSD_DDR_BUS_WIDTH_8 6       /* Card is in 8 bit DDR mode */

#define EXT_CSD_SEC_ER_EN       BIT(0)
#define EXT_CSD_SEC_BD_BLK_EN   BIT(2)
#define EXT_CSD_SEC_GB_CL_EN    BIT(4)
#define EXT_CSD_SEC_SANITIZE    BIT(6)  /* v4.5 only */

#define EXT_CSD_RST_N_EN_MASK   0x3
#define EXT_CSD_RST_N_ENABLED   1       /* RST_n is enabled on card */

#define EXT_CSD_NO_POWER_NOTIFICATION   0
#define EXT_CSD_POWER_ON                1
#define EXT_CSD_POWER_OFF_SHORT         2
#define EXT_CSD_POWER_OFF_LONG          3

#define EXT_CSD_PWR_CL_8BIT_MASK        0xF0    /* 8 bit PWR CLS */
#define EXT_CSD_PWR_CL_4BIT_MASK        0x0F    /* 8 bit PWR CLS */
#define EXT_CSD_PWR_CL_8BIT_SHIFT       4
#define EXT_CSD_PWR_CL_4BIT_SHIFT       0

#define EXT_CSD_PACKED_EVENT_EN BIT(3)

/*
 * EXCEPTION_EVENT_STATUS field
 */
#define EXT_CSD_URGENT_BKOPS            BIT(0)
#define EXT_CSD_DYNCAP_NEEDED           BIT(1)
#define EXT_CSD_SYSPOOL_EXHAUSTED       BIT(2)
#define EXT_CSD_PACKED_FAILURE          BIT(3)

#define EXT_CSD_PACKED_GENERIC_ERROR    BIT(0)
#define EXT_CSD_PACKED_INDEXED_ERROR    BIT(1)


/*
 * MMC_SWITCH access modes
 *
 * The SWITCH command response is of type R1b, therefore, the host should read the card status, using
 * SEND_STATUS command, after the busy signal is de-asserted, to check the result of the SWITCH
 * operation.
 */
#define MMC_SWITCH_MODE_CMD_SET         0x00    /* The command set is changed according to the Cmd Set field of the argument */
#define MMC_SWITCH_MODE_SET_BITS        0x01    /* The bits in the pointed byte are set, according to the ‘1’ bits in the Value field. */
#define MMC_SWITCH_MODE_CLEAR_BITS      0x02    /* The bits in the pointed byte are cleared, according to the ‘1’ bits in the Value field. */
#define MMC_SWITCH_MODE_WRITE_BYTE      0x03    /* The Value field is written into the pointed byte. */


unsigned int mmcsd_spi_unstuffBits(unsigned int *resp,unsigned int start,unsigned int size)
{
	const int __size = size;
	const unsigned int __mask = (__size < 32 ? 1 << __size : 0) - 1;
	const int __off = 3 - ((start) / 32);
	const int __shft = (start) & 31;
	unsigned int __res;

	__res = resp[__off] >> __shft;
	if (__size + __shft > 32)
	{
		__res |= resp[__off-1] << ((32 - __shft) % 32);
	}

	return(__res & __mask);
}

//MMC_extCsd extCsd;





extern new_uart* DebugCom;
new_mmcsd_spi *MMCSD_SPI[1];














void mmcsd_spi_decodeExtCsd(SD_Struct_t *ctrl, unsigned char *buffer)
{
	ctrl->extCsd.s_cmd_set		 			= buffer[504];
	ctrl->extCsd.hpi_features					= buffer[503];// [503] HPI features
	ctrl->extCsd.bkops_support 				= buffer[502];// [502] Background operations support
	// [501:247] Reserved
	ctrl->extCsd.bkops_status					= buffer[246]; // [246] Background operations status
	ctrl->extCsd.correctly_prg_sectors_num	=(buffer[245] << 24) | (buffer[244] << 16) | (buffer[243] << 8) | buffer[242];	// [245:242] Number of correctly programmed sectors
	ctrl->extCsd.ini_timeout_ap				= buffer[241]; // [241] 1st initialization time after partitioning
	// [240] Reserved
	ctrl->extCsd.pwr_cl_ddr_52_360			= buffer[239];	// [239] Power class for 52MHz, DDR at 3.6V
	ctrl->extCsd.pwr_cl_ddr_52_195			= buffer[238];	// [238] Power class for 52MHz, DDR at 1.95V
	// [237:236]
	ctrl->extCsd.min_perf_ddr_w_8_52			= buffer[235];// [235] Minimum Write Performance for 8bit at 52MHz in DDR mode
	ctrl->extCsd.min_perf_ddr_r_8_52			= buffer[234];// [234] Minimum Read Performance for 8bit at 52MHz in DDR mode
	// [233]
	ctrl->extCsd.trim_mult					= buffer[232];	// [232] TRIM Multiplier
	ctrl->extCsd.sec_feature_support			= buffer[231];  // [231] Secure Feature support
	ctrl->extCsd.sec_erase_mult				= buffer[230];// [230] Secure Erase Multiplier
	ctrl->extCsd.sec_trim_mult				= buffer[229];	// [229] Secure TRIM Multiplier
	ctrl->extCsd.boot_info					= buffer[228];	// [228] Boot information
	// [227] Reserved
	ctrl->extCsd.boot_size_multi				= buffer[226];// [226] Boot partition size
	ctrl->extCsd.acc_size						= buffer[225];// [225] Access size;
	ctrl->extCsd.hc_erase_grp_size			= buffer[224];	// [224] High-capacity erase unit size
	ctrl->extCsd.erase_timeout_mult			= buffer[223];// [223] High-capacity erase timeout
	ctrl->extCsd.rel_wr_sec_c 				= buffer[222];	// [222] Reliable write sector count
	ctrl->extCsd.hc_wp_grp_size				= buffer[221];// [221] High-capacity write protect group size
	ctrl->extCsd.s_c_vcc						= buffer[220];// [220] Sleep current (VCC)
	ctrl->extCsd.s_c_vccq						= buffer[219];// [219] Sleep current (VCCQ)
	// [218] Reserved
	ctrl->extCsd.s_a_timeout					= buffer[217];// [217] Sleep/awake timeout
	// [216] Reserved
	ctrl->extCsd.sec_count					= (buffer[215] << 24) | (buffer[214] << 16) | (buffer[213] << 8) | buffer[212];	// [215:212] Sector Count
	// [211] Reserved
	ctrl->extCsd.min_perf_w_8_52				= buffer[210];// [210] Minimum Write Performance for 8bit at 52MHz
	ctrl->extCsd.min_perf_r_8_52				= buffer[209];// [209] Minimum Read Performance for 8bit at 52MHz
	ctrl->extCsd.min_perf_w_8_26_4_52			= buffer[208];// [208] Minimum Write Performance for 8bit at 26MHz, for 4bit at 52MHz
	ctrl->extCsd.min_perf_r_8_26_4_52			= buffer[207];// [207] Minimum Read Performance for 8bit at 26MHz, for 4bit at 52MHz
	ctrl->extCsd.min_perf_w_4_26				= buffer[206];// [206] Minimum Write Performance for 4bit at 26MHz
	ctrl->extCsd.min_perf_r_4_26				= buffer[205];// [205] Minimum Read Performance for 4bit at 26MHz
	// [211] Reserved
	ctrl->extCsd.pwr_cl_26_360				= buffer[203];	// [203] Power class for 26MHz at 3.6V
	ctrl->extCsd.pwr_cl_52_360				= buffer[202];	// [202] Power class for 52MHz at 3.6V
	ctrl->extCsd.pwr_cl_26_195				= buffer[201];	// [201] Power class for 26MHz at 1.95V
	ctrl->extCsd.pwr_cl_52_195				= buffer[200];	// [200] Power class for 52MHz at 1.95V
	ctrl->extCsd.partition_switch_time		= buffer[199];	// [199] Partition switching timing
	ctrl->extCsd.out_of_interrupt_time		= buffer[198];	// [198] Out-of-interrupt busy timing
	// [197] Reserved
	ctrl->extCsd.card_type					= buffer[196];	// [196] Card type
	// [195] Reserved
	ctrl->extCsd.csd_structure				= buffer[194];	// [194] CSD structure version
	// [193] Reserved
	ctrl->extCsd.ext_csd_rev					= buffer[192];// [192] Extended CSD revision
	ctrl->extCsd.cmd_set						= buffer[191];// [191] Command set
	// [190] Reserved
	ctrl->extCsd.cmd_set_rev					= buffer[189];// [189] Command set revision
	// [188] Reserved
	ctrl->extCsd.power_class					= buffer[187];// [187] Power class
	// [186] Reserved
	ctrl->extCsd.hs_timing					= buffer[185];	// [185] High-speed interface timing
	// [184] Reserved
	ctrl->extCsd.bus_width					= buffer[183];	// [183] Bus width mode
	// [182] Reserved
	ctrl->extCsd.erased_mem_cont				= buffer[181];// [181] Erased memory content
	// [180] Reserved
	ctrl->extCsd.partition_config				= buffer[179];// [179] Partition configuration
	ctrl->extCsd.boot_config_prot				= buffer[178];// [178] Boot config protection
	ctrl->extCsd.boot_bus_width				= buffer[177];// [177] Boot bus width1
	// [176] Reserved
	ctrl->extCsd.erase_group_def				= buffer[175];// [175] High-density erase group definition
	// [174] Reserved
	ctrl->extCsd.boot_wp						= buffer[173];// [173] Boot area write protection register
	// [172] Reserved
	ctrl->extCsd.user_wp						= buffer[171];// [171] User area write protection register
	// [170] Reserved
	ctrl->extCsd.fw_config					= buffer[169];	// [169] FW configuration
	ctrl->extCsd.rpmb_size_mult				= buffer[168];// [168] RPMB Size
	ctrl->extCsd.wr_rel_set 					= buffer[167];// [167] Write reliability setting register
	ctrl->extCsd.wr_rel_param					= buffer[166];// [166] Write reliability parameter register
	// [165] Reserved
	ctrl->extCsd.bkops_start					= buffer[164];// [164] Manually start background operations
	ctrl->extCsd.bkops_en						= buffer[163];// [163] Enable background operations handshake
	ctrl->extCsd.rst_n_function				= buffer[162];// [162] H/W reset function
	ctrl->extCsd.hpi_mgmt						= buffer[161];// [161] HPI management
	ctrl->extCsd.partitioning_support			= buffer[160];// [160] Partitioning Support
	ctrl->extCsd.max_enh_size_mult 			= (buffer[159] << 16) | (buffer[158] << 8) | buffer[157]; // [159:157] Max Enhanced Area Size
	ctrl->extCsd.partitions_attribute			= buffer[156];// [156] Partitions attribute
	ctrl->extCsd.partition_setting_completed 	= buffer[155];// [155] Paritioning Setting
	ctrl->extCsd.gp_size_mult[0]				= (buffer[154] << 24) | (buffer[153] << 16) | (buffer[152] << 8) | buffer[151];// [154:143] General Purpose Partition Size
	ctrl->extCsd.gp_size_mult[1]				= (buffer[150] << 24) | (buffer[149] << 16) | (buffer[148] << 8) | buffer[147];// [154:143] General Purpose Partition Size
	ctrl->extCsd.gp_size_mult[2]				= (buffer[146] << 24) | (buffer[145] << 16) | (buffer[144] << 8) | buffer[143];// [154:143] General Purpose Partition Size
	ctrl->extCsd.enh_size_mult 				= (buffer[142] << 16) | (buffer[141] << 8) | buffer[140];// [142:140] Enhanced User Data Area Size
	ctrl->extCsd.enh_start_addr				= (buffer[139] << 24) | (buffer[138] << 16) | (buffer[137] << 8) | buffer[136];// [139:136] Enhanced User Data Start Address
	// [135] Reserved;
	ctrl->extCsd.sec_bad_blk_mgmnt			= buffer[134];	// [134] Bad Block Management mode
	// [133:0] Reserved

	ctrl->tranSpeed = SD_CARD0_TRANSPEED(ctrl);
	ctrl->blkLen = 1 << (SD_CARD0_RDBLKLEN(ctrl));
	ctrl->nBlks = ctrl->extCsd.sec_count;
	ctrl->size = ctrl->nBlks * ctrl->blkLen;

}

void mmcsd_spi_GetCSDParameters(SD_Struct_t *ctrl)
{
	//unsigned int blockNr = 0;
	//unsigned int mult = 0;

	/* Describes the version of the CSD structure. */
	ctrl->CSDInfo.csd_structure = ((ctrl->raw_csd[3] & 0xC0000000) >> 30); // [127:126]
	ctrl->CSDInfo.spec_vers = ((ctrl->raw_csd[3] & 0x3C000000) >> 26); // [125:122] everything above 4 is reserved
	// [121:120] reserved
	ctrl->CSDInfo.taac = ((ctrl->raw_csd[3] & 0x00FF0000) >> 16); // [119:112] Data read access-time 1
	ctrl->CSDInfo.nsac = ((ctrl->raw_csd[3] & 0x0000FF00) >> 8); // [111:104] Data read access-time 2 in CLK cycles (NSAC*100)
	ctrl->CSDInfo.tran_speed = (ctrl->raw_csd[3] & 0x000000FF); // [103:96] Max. bus clock frequency

	ctrl->CSDInfo.ccc = ((ctrl->raw_csd[2] & 0xFF000000) >> 20) | ((ctrl->raw_csd[2] & 0x00F00000) >> 20); // [95:84] Card command classes
	ctrl->CSDInfo.read_bl_len = ((ctrl->raw_csd[2] & 0x000F0000) >> 16); // [83:80] Max. read data block length
	ctrl->CSDInfo.read_bl_partial = ((ctrl->raw_csd[2] & 0x00008000) >> 15); // [79:79] Partial blocks for read allowed
	ctrl->CSDInfo.write_blk_misalign = ((ctrl->raw_csd[2] & 0x00004000) >> 14); // [78:78] WRITE_BLK_MISALIGN
	ctrl->CSDInfo.read_blk_misalign = ((ctrl->raw_csd[2] & 0x00002000) >> 13); // [77:77] READ_BLK_MISALIGN
	ctrl->CSDInfo.dsr_imp = ((ctrl->raw_csd[2] & 0x00001000) >> 12); // [76:76] DSR implemented
	// [75:74] reserved
	ctrl->CSDInfo.c_size = ((ctrl->raw_csd[2] & 0x000003FF) << 2) | ((ctrl->raw_csd[1] & 0xC0000000) >> 30); // [73:62] Device size
	ctrl->CSDInfo.vdd_r_curr_min = ((ctrl->raw_csd[1] & 0x38000000) >> 27); // [61:59] Max. read current @ VDD min
	ctrl->CSDInfo.vdd_r_curr_max = ((ctrl->raw_csd[1] & 0x07000000) >> 24); // [58:56] Max. read current @ VDD max
	ctrl->CSDInfo.vdd_w_curr_min = ((ctrl->raw_csd[1] & 0x00E00000) >> 21); // [55:53] Max. write current @ VDD min
	ctrl->CSDInfo.vdd_w_curr_max = ((ctrl->raw_csd[1] & 0x001C0000) >> 18); // [52:50] Max. write current @ VDD max
	ctrl->CSDInfo.c_size_mult = ((ctrl->raw_csd[1] & 0x00038000) >> 15); // [49:47] Device size multiplier
	ctrl->CSDInfo.erase_grp_size = ((ctrl->raw_csd[1] & 0x00007C00) >> 10); // [46:42] Erase group size
	ctrl->CSDInfo.erase_grp_mult = ((ctrl->raw_csd[1] & 0x000003E0) >> 5); // [41:37] Erase group size multiplier
	ctrl->CSDInfo.wp_grp_size = (ctrl->raw_csd[1] & 0x0000001F); // [36:32] Write protect group size
	ctrl->CSDInfo.wp_grp_enable = ((ctrl->raw_csd[0] & 0x80000000) >> 31); // [31:31] WP_GRP_ENABLE
	ctrl->CSDInfo.default_ecc = ((ctrl->raw_csd[0] & 0x60000000) >> 29); // [30:29] Manufacturer default ECC
	ctrl->CSDInfo.r2w_factor = ((ctrl->raw_csd[0] & 0x1C000000) >> 26); // [28:26] Write speed factor
	ctrl->CSDInfo.write_bl_len = ((ctrl->raw_csd[0] & 0x03C00000) >> 22); // [25:22] Max. write data block length
	ctrl->CSDInfo.write_bl_partial = ((ctrl->raw_csd[0] & 0x00200000) >> 21); // [21:21] Partial blocks for write allowed
	// [20:17]
	ctrl->CSDInfo.content_prot_app = ((ctrl->raw_csd[0] & 0x00010000) >> 16); // [16:16] Content protection application
	ctrl->CSDInfo.file_format_grp = ((ctrl->raw_csd[0] & 0x00008000) >> 13); // [15:15] File format group
	ctrl->CSDInfo.copy = ((ctrl->raw_csd[0] & 0x00004000) >> 12); // [14:14] Copy flag (OTP)
	ctrl->CSDInfo.perm_write_protect = ((ctrl->raw_csd[0] & 0x00002000) >> 11); // [13:13] Permanent write protection
	ctrl->CSDInfo.tmp_write_protect = ((ctrl->raw_csd[0] & 0x00001000) >> 10); // [12:12] Temporary write protection
	ctrl->CSDInfo.file_format = ((ctrl->raw_csd[0] & 0x00000C00) >> 10); // [11:10] File format
	ctrl->CSDInfo.ecc = ((ctrl->raw_csd[0] & 0x00000300) >> 8); // [9:8] ECC code
	ctrl->CSDInfo.crc = (ctrl->raw_csd[0] & 0x000000FE); // [7:1] CRC
	// [0:0] Not used, always’1’


	/*mult = 2^(ctrl->CSDInfo.c_size + 2);
	blockNr = (ctrl->CSDInfo.c_size + 1) * mult;
	ctrl->size = blockNr * mult;


	//ctrl->card->blkLen = 1 << (CSDInfo.read_bl_len);*/
	//ctrl->blkLen = 1 << (ctrl->CSDInfo.read_bl_len - 1); // Set it to 512 /////////////////////////////////////////////////////////

	//ctrl->nBlks = ctrl->size/ctrl->blkLen;

	//ctrl->tranSpeed = ctrl->CSDInfo.tran_speed ;
}


void mmcsd_spi_getCID(SD_Struct_t *ctrl)
{
	unsigned int temp[4];
	int i;

	// UNSTUFF_BITS() read in a reverse order so use a temp buffer
	for(i=0; i<4; i++)
	{
		temp[3-i] = ctrl->raw_cid[i];
	}

	unsigned int *resp = temp;

	ctrl->cardCid.mid	= (char)mmcsd_spi_unstuffBits(resp,120,8);
	ctrl->cardCid.cbx	= (char)mmcsd_spi_unstuffBits(resp,112,2);
	ctrl->cardCid.oid	= (char)mmcsd_spi_unstuffBits(resp,102,8);
	ctrl->cardCid.pnm	= mmcsd_spi_unstuffBits(resp,56,48); // This value is not correct!
	ctrl->cardCid.prv	= (char)mmcsd_spi_unstuffBits(resp,48,8);
	ctrl->cardCid.psn	= mmcsd_spi_unstuffBits(resp,16,32);
	ctrl->cardCid.mdt	= (char)mmcsd_spi_unstuffBits(resp,8,8);
	ctrl->cardCid.crc	= (char)mmcsd_spi_unstuffBits(resp,1,7);
}


//#######################################################################################
uint8_t sd_io_data(SD_Struct_t *SD_Struct, uint8_t Value)
{
	SD_Struct->HardUnitStruct->CsSelect = SD_Struct->SpiInstance;
	return SD_Struct->HardUnitReadWriteCharFunc(SD_Struct->HardUnitStruct, Value);//usart_spi_data((Uart_t *)Struct, Value);
}
//#######################################################################################
/*static void sd_delay(uint16_t DelayMs)
{
	unsigned int cnt = 0;
	for(cnt = 0; cnt < 1000; cnt++);
	//Sysdelay(DelayMs);
}*/
//#######################################################################################
void sd_cs_assert(SD_Struct_t *SD_Struct)
{
	SD_Struct->HardUnitStruct->DisableCsHandle = true;
	SD_Struct->HardUnitStruct->CsSelect = SD_Struct->SpiInstance;
	mcspi_assert(SD_Struct->HardUnitStruct);
	//gpio_out(SD_Struct->CS_Port, 0);
}
//#######################################################################################
void sd_cs_deassert(SD_Struct_t *SD_Struct)
{
	SD_Struct->HardUnitStruct->CsSelect = SD_Struct->SpiInstance;
	mcspi_deassert(SD_Struct->HardUnitStruct);
	//gpio_out(SD_Struct->CS_Port, 1);
}
//#######################################################################################
/*uint8_t sd_cs_readstate(SD_Struct_t *SD_Struct)
{
	return gpio_in(SD_Struct->CS_Port);;
}*/
//#######################################################################################
/*static uint8_t sd_read_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	uint8_t Temp = 255;
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		Temp = sd_io_data(SD_Struct,255);
		if(Temp != 255)
			break;
	}
	 return Temp;
}
//#######################################################################################
static void sd_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		sd_io_data(SD_Struct,255);
	}
}*/
//#######################################################################################
/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/
int wait_ready (	/* 1:Ready, 0:Timeout */
	SD_Struct_t *SD_Struct,
	UINT wt			/* Timeout [ms] */
)
{
	BYTE d;

	unsigned int Timer2 = wt;
	do {
		d = sd_io_data(SD_Struct, 0xFF);
		/* This loop takes a time. Insert rot_rdq() here for multitask envilonment. */
		//sd_delay(1);
	} while (d != 0xFF && Timer2--);	/* Wait for card goes ready or timeout */

	return (d == 0xFF) ? 1 : 0;
}

/*-----------------------------------------------------------------------*/
/* Deselect card and release SPI                                         */
/*-----------------------------------------------------------------------*/

void deselect (SD_Struct_t *SD_Struct)
{
	sd_cs_deassert(SD_Struct);		/* CS = H */
	sd_io_data(SD_Struct, 0xFF);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}



/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/

int _select (SD_Struct_t *SD_Struct)	/* 1:OK, 0:Timeout */
{
	sd_cs_assert(SD_Struct);
	sd_io_data(SD_Struct, 0xFF);	/* Dummy clock (force DO enabled) */

	if (wait_ready(SD_Struct, 50000))
		return 1;	/* OK */
	deselect(SD_Struct);
	return 0;	/* Timeout */
}

unsigned char mmcsd_crc7(char *data,uint8_t length)
{
   uint8_t i, ibit, c, crc;

   crc = 0x00;                                                                // Set initial value

   for (i = 0; i < length; i++, data++)
   {
      c = *data;

      for (ibit = 0; ibit < 8; ibit++)
      {
         crc = crc << 1;
         if ((c ^ crc) & 0x80) crc = crc ^ 0x09;                              // ^ is XOR
         c = c << 1;
      }

       crc = crc & 0x7F;
   }

   //shift_left(&crc, 1, 1);                                                    // MMC card stores the result in the top 7 bits so shift them left 1
                                                                              // Should shift in a 1 not a 0 as one of the cards I have won't work otherwise
   return (crc << 1) | 1;
}

uint16_t mmcsd_crc16(char *data, uint8_t length)
{
   uint8_t i, ibit, c;

   uint16_t crc;

   crc = 0x0000;                                                                // Set initial value

   for (i = 0; i < length; i++, data++)
   {
      c = *data;

      for (ibit = 0; ibit < 8; ibit++)
      {
         crc = crc << 1;
         if ((c ^ crc) & 0x8000) crc = crc ^ 0x1021;                              // ^ is XOR
         c = c << 1;
      }

       crc = crc & 0x7FFF;
   }

   //shift_left(&crc, 2, 1);                                                    // MMC card stores the result in the top 7 bits so shift them left 1
                                                                              // Should shift in a 1 not a 0 as one of the cards I have won't work otherwise
   return (crc << 1) | 1;
}

/*-----------------------------------------------------------------------*/
/* Send a command packet to the MMC                                      */
/*-----------------------------------------------------------------------*/

BYTE send_cmd (		/* Return value: R1 resp (bit7==1:Failed to send) */
	SD_Struct_t *SD_Struct,
	BYTE cmd,		/* Command index */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80)
	{	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(SD_Struct, CMD55, 0);
		if (res > 1)
			return res;
	}

	/* Select card */
	if (cmd != CMD12)
	{
		deselect(SD_Struct);
		if (!_select(SD_Struct))
			return 0xFF;
	}

	//if (cmd == CMD17)
		//send_cmd(SD_Struct, CMD59, 0);
	/* Send command packet */
	sd_io_data(SD_Struct, 0x40 | cmd);				/* Start + command index */
	sd_io_data(SD_Struct, (BYTE)(arg >> 24));		/* Argument[31..24] */
	sd_io_data(SD_Struct, (BYTE)(arg >> 16));		/* Argument[23..16] */
	sd_io_data(SD_Struct, (BYTE)(arg >> 8));			/* Argument[15..8] */
	sd_io_data(SD_Struct, (BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0)
		n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8)
		n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	sd_io_data(SD_Struct, n);

	/* Receive command resp */
	if (cmd == CMD12)
		sd_io_data(SD_Struct, 0xFF);	/* Diacard following one byte when CMD12 */
	n = 255;								/* Wait for response (10 bytes max) */
	do
		res = sd_io_data(SD_Struct, 0xFF);
	while ((res & 0x80) && --n);

	return res;
	/* Return received response */
}

bool _mmcsd_spi_init(unsigned int unit_nr)
{
	SD_Struct_t *SD_Struct = MMCSD_SPI[unit_nr];
	SD_Struct->HardUnitStruct->CsSelect = SD_Struct->SpiInstance;
	SD_Struct->HardUnitStruct->ClkDiv[SD_Struct->SpiInstance] = 7;
	//SD_Struct->HardUnitSetBaudFunc((void *)SD_Struct->HardUnitStruct, 7);			/* Set fast clock */
	sd_cs_deassert(SD_Struct);
	BYTE cmd, ty, ocr[4], csd[16];
	unsigned short  n;
	SD_Struct->SD_Hc = IsSd;
	for (n = 10; n; n--)
		sd_io_data(SD_Struct, 0xFF);	/* Send 80 dummy clocks */

	ty = 0;
	if (send_cmd(SD_Struct, CMD0, 0) == 1)
	{			/* Put the card SPI/Idle state */
		unsigned int Timer1 = 1000;						/* Initialization timeout = 1 sec */
		if (send_cmd(SD_Struct, CMD8, 0x1AA) == 1)
		{	/* SDv2? */
			for (n = 0; n < 4; n++) ocr[n] = sd_io_data(SD_Struct, 0xFF);	/* Get 32 bit return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA)
			{				/* Is the card supports vcc of 2.7-3.6V? */
				while (Timer1-- && send_cmd(SD_Struct, ACMD41, 1UL << 30)) ;	/* Wait for end of initialization with ACMD41(HCS) */
				if (Timer1 && send_cmd(SD_Struct, CMD58, 0) == 0)
				{		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++)
						ocr[n] = sd_io_data(SD_Struct, 0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Card id SDv2 */
					if(ty & CT_BLOCK)
						SD_Struct->SD_Hc = IsSdhc;
				}
			}
		} else {	/* Not SDv2 card */
			if (send_cmd(SD_Struct, ACMD41, 0) <= 1)
			{	/* SDv1 or MMC? */
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
			} else
			{
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 (CMD1(0)) */
			}
			while (Timer1-- && send_cmd(SD_Struct, cmd, 0)) ;		/* Wait for end of initialization */
			if (!Timer1 || send_cmd(SD_Struct, CMD16, 512) != 0)	/* Set block length: 512 */
				ty = 0;
		}
	}
	if (send_cmd(SD_Struct, CMD59, 0) == 0)
	{

	}

	/*if (send_cmd(SD_Struct, CMD10, 0) == 0)
	{
		for (n = 0; n < 32; n++) cid[n] = sd_io_data(SD_Struct, 0xFF);
        memcpy(SD_Struct->raw_cid, cid, 16);
	}*/
	if (ty)
	{			/* OK */
		SD_Struct->HardUnitStruct->CsSelect = SD_Struct->SpiInstance;
		SD_Struct->HardUnitStruct->ClkDiv[SD_Struct->SpiInstance] = 2;
		//SD_Struct->HardUnitSetBaudFunc((void *)SD_Struct->HardUnitStruct, 1);			/* Set fast clock */
		//send_cmd(SD_Struct, MMC_CMD_CRC_ON_OFF,0x00000001); // CMD59
		if (send_cmd(SD_Struct, CMD9, 0) == 0)
		{
			unsigned int wait = 0, response = 0;
			while (++wait <= 0x1ff && response != 0xfe)
				response = sd_io_data(SD_Struct, 0xff);
			if (wait >= 0x1ff)
			{
				SD_Struct->SD_Init_OK = false;
				deselect(SD_Struct);
				return SD_Struct->SD_Init_OK;
			}
			for (n = 0; n < 16; n++)
				csd[n] = sd_io_data(SD_Struct, 0xFF);
			sd_io_data(SD_Struct, 0xFF);
			sd_io_data(SD_Struct, 0xFF);
			SD_Struct->raw_csd[0] = csd[15] + (csd[14] << 8) +  (csd[13] << 16) +  (csd[12] << 24);
			SD_Struct->raw_csd[1] = csd[11] + (csd[10] << 8) +  (csd[9] << 16) +  (csd[8] << 24);
			SD_Struct->raw_csd[2] = csd[7] + (csd[6] << 8) +  (csd[5] << 16) +  (csd[4] << 24);
			SD_Struct->raw_csd[3] = csd[3] + (csd[2] << 8) +  (csd[1] << 16) +  (csd[0] << 24);
	        //memcpy(SD_Struct->raw_csd, csd, 16);
		}
	    if (SD_Struct->SD_Hc == IsSdhc)
	    {
	    	SD_Struct->tranSpeed = SD_CARD1_TRANSPEED(SD_Struct);
	    	SD_Struct->blkLen = 1 << (SD_CARD1_RDBLKLEN(SD_Struct));
	    	SD_Struct->size = SD_CARD1_SIZE(SD_Struct);
	    	SD_Struct->nBlks = SD_Struct->size / SD_Struct->blkLen;
	    }
	    else
	    {
	    	SD_Struct->tranSpeed = SD_CARD0_TRANSPEED(SD_Struct);
	    	SD_Struct->blkLen = 1 << (SD_CARD0_RDBLKLEN(SD_Struct));
	    	SD_Struct->nBlks = SD_CARD0_NUMBLK(SD_Struct);
	    	SD_Struct->size = SD_CARD0_SIZE(SD_Struct);
	    }
		SD_Struct->SD_Init_OK = true;
	} else
	{			/* Failed */
		//power_off();
		SD_Struct->SD_Init_OK = false;
	}

	deselect(SD_Struct);
	return SD_Struct->SD_Init_OK;
}

//#######################################################################################
static inline bool rcvr_datablock(void *_SD_Struct, unsigned char *buff, unsigned int bytes_to_read)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	unsigned char token;
	unsigned long Timer1 = 50000;
	do
	{							/* Wait for data packet in timeout of 100ms */
		token = sd_io_data(SD_Struct,255);
	} while ((token == 0xFF) && Timer1--);

	if(token != MMC_DATA_TOKEN)
		return false;	/* If not valid data token, return with error */

	do
	{							// Receive the data block into buffer
		*buff++ = sd_io_data(SD_Struct,255);
	} while (--bytes_to_read);
	/*SD_Struct->HardUnitStruct->CsSelect = SD_Struct->SpiInstance;
	memset(buff, 0xFF, bytes_to_read);
	SD_Struct->HardUnitStruct->Buff = buff;
	if(!SD_Struct->HardUnitReadWriteBuffFunc(SD_Struct->HardUnitStruct, 0, bytes_to_read))
		return false;*/


	sd_io_data(SD_Struct,255);						/* Discard CRC */
	sd_io_data(SD_Struct,255);

	return true;					/* Return with success */
}
//#######################################################################################
unsigned int _sd_read_page(void *_SD_Struct, void* _Buffer, unsigned long block, unsigned int nblks)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	if (/*drv || */!nblks)
		return false;		/* Check parameter */
	if (SD_Struct->SD_Init_OK == false)
		return false;	/* Check if drive is ready */

	if (SD_Struct->SD_Hc == IsSd)
		block *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (nblks == 1)
	{	/* Single sector read */
		if ((send_cmd(SD_Struct, CMD17, block) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(SD_Struct, _Buffer, 512))
			nblks = 0;
	}
	else
	{				/* Multiple sector read */
		unsigned char* Buffer = (unsigned char*)_Buffer;
		if (send_cmd(SD_Struct, CMD18, block) == 0)
		{	/* READ_MULTIPLE_BLOCK */
			do
			{
				if (!rcvr_datablock(SD_Struct, Buffer, 512))
					break;
				Buffer += 512;
			} while (--nblks);
			send_cmd(SD_Struct, CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect(SD_Struct);
	return nblks ? false : true;	/* Return result */
}
//#######################################################################################
unsigned int MMCSD_SPI_ReadCmdSend(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	//if(SD_Struct->SD_Hc == IsSdhc)
	//{
	//	if(!_sd_read_page(_SD_Struct, Buffer, block, nblks))
	//		return false;
	//}
	//else
	{
		do
		{
			if(!_sd_read_page(_SD_Struct, Buffer, block++, 1))
				return false;
			Buffer += 512;
		} while (--nblks);
	}
	return true;
}
//#######################################################################################
/* Send multiple byte */
static
void xmit_spi_multi (
	SD_Struct_t *SD_Struct,
	const BYTE *buff,	/* Pointer to the data */
	UINT btx			/* Number of bytes to send (512) */
)
{
	do {						// Transmit data block
		sd_io_data(SD_Struct, *buff++);
	} while (btx -= 1);
	/*SD_Struct->HardUnitStruct->CsSelect = SD_Struct->SpiInstance;
	SD_Struct->HardUnitStruct->DisableCsHandle = true;
	SD_Struct->HardUnitStruct->Buff = (unsigned char *)buff;
	SD_Struct->HardUnitReadWriteBuffFunc(SD_Struct->HardUnitStruct, btx, 0);*/

}
/*-----------------------------------------------------------------------*/
/* Send a data packet to the MMC                                         */
/*-----------------------------------------------------------------------*/

static
int xmit_datablock (	/* 1:OK, 0:Failed */
	SD_Struct_t *SD_Struct,
	const BYTE *buff,	/* Ponter to 512 byte data to be sent */
	BYTE token			/* Token */
)
{
	BYTE resp;


	if (!wait_ready(SD_Struct, 50000))
		return 0;		/* Wait for card ready */

	sd_io_data(SD_Struct, token);					/* Send token */
	if (token != 0xFD) {				/* Send data if token is other than StopTran */
		xmit_spi_multi(SD_Struct, buff, 512);		/* Data */
		//unsigned short crc = mmcsd_crc16(buff, 512);
		sd_io_data(SD_Struct, 0xFF);
		sd_io_data(SD_Struct, 0xFF);	/* Dummy CRC */

		resp = sd_io_data(SD_Struct, 0xFF);				/* Receive data resp */
		if ((resp & 0x1F) != 0x05)		/* Function fails if the data packet was not accepted */
			return 0;
	}
	return 1;
}


unsigned int  _sd_write_page(void *_SD_Struct, void* _Buffer, unsigned long block, unsigned int nblks)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	if (/*drv || */!nblks)
		return false;		/* Check parameter */
	if (SD_Struct->SD_Init_OK == false)
		return false;	/* Check if drive is ready */

	if (SD_Struct->SD_Hc == IsSd)
		block *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (nblks == 1)
	{	/* Single sector write */
		if ((send_cmd(SD_Struct, CMD24, block) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(SD_Struct, _Buffer, 0xFE))
			nblks = 0;
	}
	else {				/* Multiple sector write */
		if (SD_Struct->SD_Hc == IsSd)
			send_cmd(SD_Struct, ACMD23, nblks);	/* Predefine number of sectors */
		if (send_cmd(SD_Struct, CMD25, block) == 0)
		{	/* WRITE_MULTIPLE_BLOCK */
			unsigned char* Buffer = (unsigned char*)_Buffer;
			do
			{
				if (!xmit_datablock(SD_Struct, Buffer, 0xFC))
					break;
				Buffer += 512;
			} while (--nblks);
			if (!xmit_datablock(SD_Struct, 0, 0xFD))	/* STOP_TRAN token */
				nblks = 1;
		}
	}
	deselect(SD_Struct);
	return nblks ? false : true;	/* Return result */
}
//#######################################################################################
unsigned int MMCSD_SPI_WriteCmdSend(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	//if(SD_Struct->SD_Hc == IsSdhc)
	//{
	//	if(!_sd_write_page(_SD_Struct, Buffer, block, nblks))
	//		return false;
	//}
	//else
	{
		do
		{
			if(!_sd_write_page(_SD_Struct, Buffer, block++, 1))
				return false;
			Buffer += 512;
		} while (--nblks);
	}
	return true;
}
//#######################################################################################
void mmcsd_spi_init(unsigned int unit_nr, new_gpio* StatusLed)
{
	SD_Struct_t *SD_Struct = MMCSD_SPI[unit_nr];
	SD_Struct->initFlg = 1;
	_mmcsd_spi_init(unit_nr);
	return;
}
//#######################################################################################
void mmcsd_spi_idle(unsigned int unit_nr)
{
	SD_Struct_t *SD_StructDisk = MMCSD_SPI[unit_nr];
    if(gpio_in(SD_StructDisk->SD_Present) == 0 || (int)(SD_StructDisk->SD_Present) == 0 || (int)(SD_StructDisk->SD_Present) == -1)
    {
        if(SD_StructDisk->initFlg)
        {
        	SD_StructDisk->initFlg = 0;
        	sys_delay(400);
        	if(_mmcsd_spi_init(unit_nr))
        	{
        		SD_StructDisk->connected = true;
        		SD_StructDisk->g_s_mmcFatFs.drv_rw_func.DriveStruct = SD_StructDisk;
        		SD_StructDisk->g_s_mmcFatFs.drv_rw_func.drv_r_func = MMCSD_SPI_ReadCmdSend;
        		SD_StructDisk->g_s_mmcFatFs.drv_rw_func.drv_w_func = MMCSD_SPI_WriteCmdSend;
        		SD_StructDisk->g_s_mmcFatFs.drv_rw_func.drv_ioctl_func = mmcsd_spi_ioctl;
#if (_FFCONF == 82786)
        		char drv_name_buff[4];
        		if(!f_mount(3 + unit_nr, &SD_StructDisk->g_s_mmcFatFs))
#else
                char drv_name_buff[9];
                drv_name_buff[0] = 'S';
                drv_name_buff[1] = 'P';
                drv_name_buff[2] = 'I';
                drv_name_buff[3] = 'S';
                drv_name_buff[4] = 'D';
                drv_name_buff[5] = '1' + unit_nr;
                drv_name_buff[6] = ':';
                drv_name_buff[7] = '\0';
                if(!f_mount(&SD_StructDisk->g_s_mmcFatFs, drv_name_buff, 1))
#endif
                {
#if (_FFCONF == 82786)
                    drv_name_buff[0] = '0' + 3 + unit_nr;
                    drv_name_buff[1] = ':';
                    drv_name_buff[2] = '/';
                    drv_name_buff[3] = '\0';
#else
                    drv_name_buff[7] = '/';
                    drv_name_buff[8] = '\0';
#endif
                   if(f_opendir(&g_sDirObject, drv_name_buff) == FR_OK)
                    {
                    	SD_StructDisk->fs_mounted = true;
#ifdef MMCSD_DEBUG_EN
						if(DebugCom)
						{
																				UARTprintf(DebugCom,   "MMCSD%d drive %d mounted\n\r" , unit_nr + 3 , unit_nr + 3);
																				UARTprintf(DebugCom,   "MMCSD%d Fat fs detected\n\r" , unit_nr + 3);
																				UARTprintf(DebugCom, "MMCSD%d Fs type:                 " , unit_nr + 3);
							if(SD_StructDisk->g_s_mmcFatFs.fs_type == FS_FAT12)	{ 				UARTprintf(DebugCom, "Fat12");}
							else if(SD_StructDisk->g_s_mmcFatFs.fs_type == FS_FAT16){ 				UARTprintf(DebugCom, "Fat16");}
							else if(SD_StructDisk->g_s_mmcFatFs.fs_type == FS_FAT32){ 				UARTprintf(DebugCom, "Fat32");}
							else if(SD_StructDisk->g_s_mmcFatFs.fs_type == FS_EXFAT){ 				UARTprintf(DebugCom, "exFat");}
							else								{ 				UARTprintf(DebugCom, "None");}
																				UARTprintf(DebugCom, "\n\r");
																				//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
																				UARTprintf(DebugCom, "MMCSD%d BytesPerSector:          %d \n\r",unit_nr + 3, /*(int)g_sFatFs.s_size*/512);
																				UARTprintf(DebugCom, "MMCSD%d SectorsPerCluster:       %d \n\r",unit_nr + 3, (int)SD_StructDisk->g_s_mmcFatFs.csize);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																				UARTprintf(DebugCom, "MMCSD%d NumberOfFats:            %d \n\r",unit_nr + 3, (int)SD_StructDisk->g_s_mmcFatFs.n_fats);
																				//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																				UARTprintf(DebugCom, "MMCSD%d DataSectionBegin:        %d \n\r",unit_nr + 3, (int)SD_StructDisk->g_s_mmcFatFs.fatbase);
																				unsigned long tmp = (unsigned long long)((unsigned long long)SD_StructDisk->g_s_mmcFatFs.n_fatent * (unsigned long long)/*g_sFatFs.s_size*/512 *(unsigned long long)SD_StructDisk->g_s_mmcFatFs.csize) >> 20/* / 1000000*/;
																				UARTprintf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r",unit_nr + 3, tmp);
						}
#endif
                    } else  if(DebugCom)										UARTprintf(DebugCom,   "MMCSD%d ERROR oppening path\n\r" , unit_nr + 3);
                }
                else if(DebugCom)												UARTprintf(DebugCom,   "MMCSD%d ERROR mounting disk\n\r" , unit_nr + 3);
        	}
        	else if(DebugCom)												UARTprintf(DebugCom,   "MMCSD%d not detected\n\r" , unit_nr + 3);
        }
    }
    else
    {
        Sysdelay(1);
        if(SD_StructDisk->initFlg != 1)
        {
        	SD_StructDisk->fs_mounted = false;
        	SD_StructDisk->connected = false;
        	SD_StructDisk->initFlg = 1;
#ifdef MMCSD_DEBUG_EN
        	UARTprintf(DebugCom,   "MMCSD%d Disconnected\n\r" , unit_nr + 3);
#endif
        }
    }

}
//#######################################################################################
void mmcsd_spi_ioctl(void *_SD_Struct, unsigned int  command,  unsigned int *buffer)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           //if(_SdCtrlStruct->CardType == MMCSD_CARD_SD) *buffer = _SdCtrlStruct->card->nBlks;
           //else if(_SdCtrlStruct->card->cardType == MMCSD_CARD_MMC) *buffer = extCsd.sec_count;
           //else *buffer = 0;
           *buffer = SD_Struct->nBlks;

            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = SD_Struct->blkLen;

            break;
        }
		case CTRL_SYNC :		/* Make sure that no pending write process */
			_select(SD_Struct);
			if (wait_ready(SD_Struct, 5000) == 0xFF)
				*buffer = RES_OK;
			break;
       default:
        {
            *buffer = 0;
            break;
        }

    }
}


