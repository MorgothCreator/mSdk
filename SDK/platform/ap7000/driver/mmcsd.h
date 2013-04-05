/*
 * SdMmc.h
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#ifndef SDMMC_H_
#define SDMMC_H_

#define MMCI_CR					0x0000
#define MMCI_MR					0x0004
#define MMCI_DTOR				0x0008
#define MMCI_SDCR				0x000c
#define MMCI_ARGR				0x0010
#define MMCI_CMDR				0x0014
#define MMCI_RSPR				0x0020
#define MMCI_RSPR1				0x0024
#define MMCI_RSPR2				0x0028
#define MMCI_RSPR3				0x002c
#define MMCI_RDR				0x0030
#define MMCI_TDR				0x0034
#define MMCI_SR					0x0040
#define MMCI_IER				0x0044
#define MMCI_IDR				0x0048
#define MMCI_IMR				0x004c

/* Bitfields in CR */
#define MMCI_MCIEN_OFFSET			0
#define MMCI_MCIEN_SIZE				1
#define MMCI_MCIDIS_OFFSET			1
#define MMCI_MCIDIS_SIZE			1
#define MMCI_PWSEN_OFFSET			2
#define MMCI_PWSEN_SIZE				1
#define MMCI_PWSDIS_OFFSET			3
#define MMCI_PWSDIS_SIZE			1
#define MMCI_SWRST_OFFSET			7
#define MMCI_SWRST_SIZE				1

/* Bitfields in MR */
#define MMCI_CLKDIV_OFFSET			0
#define MMCI_CLKDIV_SIZE			8
#define MMCI_PWSDIV_OFFSET			8
#define MMCI_PWSDIV_SIZE			3
#define MMCI_RDPROOF_OFFSET			11
#define MMCI_RDPROOF_SIZE			1
#define MMCI_WRPROOF_OFFSET			12
#define MMCI_WRPROOF_SIZE			1
#define MMCI_PDCPADV_OFFSET			14
#define MMCI_PDCPADV_SIZE			1
#define MMCI_PDCMODE_OFFSET			15
#define MMCI_PDCMODE_SIZE			1
#define MMCI_BLKLEN_OFFSET			16
#define MMCI_BLKLEN_SIZE			16

/* Bitfields in DTOR */
#define MMCI_DTOCYC_OFFSET			0
#define MMCI_DTOCYC_SIZE			4
#define MMCI_DTOMUL_OFFSET			4
#define MMCI_DTOMUL_SIZE			3

/* Bitfields in SDCR */
#define MMCI_SCDSEL_OFFSET			0
#define MMCI_SCDSEL_SIZE			4
#define MMCI_SCDBUS_OFFSET			7
#define MMCI_SCDBUS_SIZE			1

/* Bitfields in ARGR */
#define MMCI_ARG_OFFSET				0
#define MMCI_ARG_SIZE				32

/* Bitfields in CMDR */
#define MMCI_CMDNB_OFFSET			0
#define MMCI_CMDNB_SIZE				6
#define MMCI_RSPTYP_OFFSET			6
#define MMCI_RSPTYP_SIZE			2
#define MMCI_SPCMD_OFFSET			8
#define MMCI_SPCMD_SIZE				3
#define MMCI_OPDCMD_OFFSET			11
#define MMCI_OPDCMD_SIZE			1
#define MMCI_MAXLAT_OFFSET			12
#define MMCI_MAXLAT_SIZE			1
#define MMCI_TRCMD_OFFSET			16
#define MMCI_TRCMD_SIZE				2
#define MMCI_TRDIR_OFFSET			18
#define MMCI_TRDIR_SIZE				1
#define MMCI_TRTYP_OFFSET			19
#define MMCI_TRTYP_SIZE				2

/* Bitfields in RSPRx */
#define MMCI_RSP_OFFSET				0
#define MMCI_RSP_SIZE				32

/* Bitfields in SR/IER/IDR/IMR */
#define MMCI_CMDRDY_OFFSET			0
#define MMCI_CMDRDY_SIZE			1
#define MMCI_RXRDY_OFFSET			1
#define MMCI_RXRDY_SIZE				1
#define MMCI_TXRDY_OFFSET			2
#define MMCI_TXRDY_SIZE				1
#define MMCI_BLKE_OFFSET			3
#define MMCI_BLKE_SIZE				1
#define MMCI_DTIP_OFFSET			4
#define MMCI_DTIP_SIZE				1
#define MMCI_NOTBUSY_OFFSET			5
#define MMCI_NOTBUSY_SIZE			1
#define MMCI_ENDRX_OFFSET			6
#define MMCI_ENDRX_SIZE				1
#define MMCI_ENDTX_OFFSET			7
#define MMCI_ENDTX_SIZE				1
#define MMCI_RXBUFF_OFFSET			14
#define MMCI_RXBUFF_SIZE			1
#define MMCI_TXBUFE_OFFSET			15
#define MMCI_TXBUFE_SIZE			1
#define MMCI_RINDE_OFFSET			16
#define MMCI_RINDE_SIZE				1
#define MMCI_RDIRE_OFFSET			17
#define MMCI_RDIRE_SIZE				1
#define MMCI_RCRCE_OFFSET			18
#define MMCI_RCRCE_SIZE				1
#define MMCI_RENDE_OFFSET			19
#define MMCI_RENDE_SIZE				1
#define MMCI_RTOE_OFFSET			20
#define MMCI_RTOE_SIZE				1
#define MMCI_DCRCE_OFFSET			21
#define MMCI_DCRCE_SIZE				1
#define MMCI_DTOE_OFFSET			22
#define MMCI_DTOE_SIZE				1
#define MMCI_OVRE_OFFSET			30
#define MMCI_OVRE_SIZE				1
#define MMCI_UNRE_OFFSET			31
#define MMCI_UNRE_SIZE				1

/* Constants for DTOMUL */
#define MMCI_DTOMUL_1_CYCLE			0
#define MMCI_DTOMUL_16_CYCLES			1
#define MMCI_DTOMUL_128_CYCLES			2
#define MMCI_DTOMUL_256_CYCLES			3
#define MMCI_DTOMUL_1024_CYCLES			4
#define MMCI_DTOMUL_4096_CYCLES			5
#define MMCI_DTOMUL_65536_CYCLES		6
#define MMCI_DTOMUL_1048576_CYCLES		7

/* Constants for RSPTYP */
#define MMCI_RSPTYP_NO_RESP			0
#define MMCI_RSPTYP_48_BIT_RESP			1
#define MMCI_RSPTYP_136_BIT_RESP		2

/* Constants for SPCMD */
#define MMCI_SPCMD_NO_SPEC_CMD			0
#define MMCI_SPCMD_INIT_CMD			1
#define MMCI_SPCMD_SYNC_CMD			2
#define MMCI_SPCMD_INT_CMD			4
#define MMCI_SPCMD_INT_RESP			5

/* Constants for TRCMD */
#define MMCI_TRCMD_NO_TRANS			0
#define MMCI_TRCMD_START_TRANS			1
#define MMCI_TRCMD_STOP_TRANS			2

/* Constants for TRTYP */
#define MMCI_TRTYP_BLOCK			0
#define MMCI_TRTYP_MULTI_BLOCK			1
#define MMCI_TRTYP_STREAM			2




/* MMC command numbers */
#define MMC_CMD_GO_IDLE_STATE		0
#define MMC_CMD_SEND_OP_COND		1
#define MMC_CMD_ALL_SEND_CID		2
#define MMC_CMD_SET_RELATIVE_ADDR	3
#define MMC_CMD_SET_DSR			4
#define MMC_CMD_SELECT_CARD		7
#define MMC_SEND_IF_COND		8
#define MMC_CMD_SEND_CSD		9
#define MMC_CMD_SEND_CID		10
#define MMC_CMD_SEND_STATUS		13
#define MMC_CMD_SET_BLOCKLEN		16
#define MMC_CMD_READ_SINGLE_BLOCK	17
#define MMC_CMD_READ_MULTIPLE_BLOCK	18
#define MMC_CMD_WRITE_BLOCK		24
#define MMC_CMD_APP_CMD			55
#define MMC_CMD_READ_OCR		58

/* SD Card command numbers */
#define SD_CMD_SEND_RELATIVE_ADDR	3
#define SD_CMD_SWITCH			6
#define SD_CMD_SEND_IF_COND		8

#define SD_CMD_APP_SET_BUS_WIDTH	6
#define SD_CMD_APP_SEND_OP_COND		41


#ifndef CFG_MMC_CLK_OD
#define CFG_MMC_CLK_OD		400000
#endif

#ifndef CFG_MMC_CLK_PP
#define CFG_MMC_CLK_PP		20000000
#endif

#ifndef CFG_MMC_OP_COND
#define CFG_MMC_OP_COND		0x00100000
#endif

#define MMC_DEFAULT_BLKLEN	512
#define MMC_DEFAULT_RCA		1

#define IsSD				0
#define IsSDHC				1

#define RESP_NO_CRC	1
#define R1		MMCI_BF(RSPTYP, 1)
#define R2		MMCI_BF(RSPTYP, 2)
#define R3		(R1 | RESP_NO_CRC)
#define R6		R1
#define NID		MMCI_BF(MAXLAT, 0)
#define NCR		MMCI_BF(MAXLAT, 1)
#define TRCMD_START	MMCI_BF(TRCMD, 1)
#define TRDIR_READ	MMCI_BF(TRDIR, 1)
#define TRDIR_WRITE	MMCI_BF(TRDIR, 0)
#define TRTYP_BLOCK	MMCI_BF(TRTYP, 0)
#define INIT_CMD	MMCI_BF(SPCMD, 1)
#define OPEN_DRAIN	MMCI_BF(OPDCMD, 1)

#define ERROR_FLAGS	(MMCI_BIT(DTOE)			\
			 | MMCI_BIT(RDIRE)		\
			 | MMCI_BIT(RENDE)		\
			 | MMCI_BIT(RINDE)		\
			 | MMCI_BIT(RTOE))


struct mmc_cid {
	unsigned long psn;
	unsigned short oid;
	unsigned char mid;
	unsigned char prv;
	unsigned char mdt;
	char pnm[7];
};

struct mmc_csd
{
	unsigned char	csd_structure:2,
		spec_vers:4,
		rsvd1:2;
	unsigned char	taac;
	unsigned char	nsac;
	unsigned char	tran_speed;
	unsigned short	ccc:12,
		read_bl_len:4;
	unsigned long long	read_bl_partial:1,
		write_blk_misalign:1,
		read_blk_misalign:1,
		dsr_imp:1,
		rsvd2:2,
		c_size:12,
		vdd_r_curr_min:3,
		vdd_r_curr_max:3,
		vdd_w_curr_min:3,
		vdd_w_curr_max:3,
		c_size_mult:3,
		sector_size:5,
		erase_grp_size:5,
		wp_grp_size:5,
		wp_grp_enable:1,
		default_ecc:2,
		r2w_factor:3,
		write_bl_len:4,
		write_bl_partial:1,
		rsvd3:5;
	unsigned char	file_format_grp:1,
		copy:1,
		perm_write_protect:1,
		tmp_write_protect:1,
		file_format:2,
		ecc:2;
	unsigned char	crc:7;
	unsigned char	one:1;
};

#ifdef CFG_64BIT_LBA
typedef unsigned long long lbaint_t;
#else
typedef unsigned long lbaint_t;
#endif

typedef struct block_dev_desc {
	int				if_type;	/* type of the interface */
	int				dev;		/* device number */
	unsigned char	part_type;	/* partition type */
	unsigned char	target;		/* target SCSI ID */
	unsigned char	lun;		/* target LUN */
	unsigned char	type;		/* device type */
	unsigned char	removable;	/* removable device */
	unsigned char	lba48;		/* device can use 48bit addr (ATA/ATAPI v7) */
	lbaint_t		lba;		/* number of blocks */
	unsigned long	blksz;		/* block size */
	char			vendor [40+1];	/* IDE model, SCSI Vendor */
	char			product[20+1];	/* IDE Serial no, SCSI product */
	char			revision[8+1];	/* firmware revision */
	unsigned long	(*block_read) (unsigned long start,lbaint_t blkcnt,void *buffer);
	unsigned long	(*block_write)(unsigned long start,lbaint_t blkcnt,void *buffer);
	void			*priv;		/* driver private struct pointer */
}block_dev_desc_t;

#define MMCI_BASE				0xFFF02400
#define R1_ILLEGAL_COMMAND		(1 << 22)
#define R1_APP_CMD			(1 << 5)




/* Interface types: */
#define IF_TYPE_UNKNOWN		0
#define IF_TYPE_IDE		1
#define IF_TYPE_SCSI		2
#define IF_TYPE_ATAPI		3
#define IF_TYPE_USB		4
#define IF_TYPE_DOC		5
#define IF_TYPE_MMC		6
#define IF_TYPE_SD		7
#define IF_TYPE_SATA		8

/* Part types */
#define PART_TYPE_UNKNOWN	0x00
#define PART_TYPE_MAC		0x01
#define PART_TYPE_DOS		0x02
#define PART_TYPE_ISO		0x03
#define PART_TYPE_AMIGA		0x04




/* Bit manipulation macros */
#define MMCI_BIT(name)					\
	(1 << MMCI_##name##_OFFSET)
#define MMCI_BF(name,value)				\
	(((value) & ((1 << MMCI_##name##_SIZE) - 1))	\
	 << MMCI_##name##_OFFSET)
#define MMCI_BFEXT(name,value)				\
	(((value) >> MMCI_##name##_OFFSET)\
	 & ((1 << MMCI_##name##_SIZE) - 1))
#define MMCI_BFINS(name,value,old)			\
	(((old) & ~(((1 << MMCI_##name##_SIZE) - 1)	\
		    << MMCI_##name##_OFFSET))		\
	 | MMCI_BF(name,value))




/* Register access macros */
#define mmci_readl(reg)			readl((void *)MMCI_BASE + MMCI_##reg)
#define mmci_writel(reg,value)	writel((value), (void *)MMCI_BASE + MMCI_##reg)



// DOS stuff

#define DEFAULT_SECTOR_SIZE					512
#define DOS_PART_TBL_OFFSET					0x1be
#define DOS_PART_MAGIC_OFFSET				0x1fe
#define DOS_PBR_FSTYPE_OFFSET				0x36
#define DOS_PBR_MEDIA_TYPE_OFFSET			0x15
#define DOS_MBR	0
#define DOS_PBR	1

typedef struct dos_partition {
	unsigned char boot_ind;		/* 0x80 - active			*/
	unsigned char head;		/* starting head			*/
	unsigned char sector;		/* starting sector			*/
	unsigned char cyl;		/* starting cylinder			*/
	unsigned char sys_ind;		/* What partition type			*/
	unsigned char end_head;		/* end head				*/
	unsigned char end_sector;	/* end sector				*/
	unsigned char end_cyl;		/* end cylinder				*/
	unsigned char start4[4];	/* starting sector counting from 0	*/
	unsigned char size4[4];		/* nr of sectors in partition		*/
} dos_partition_t;

void MMC_setio(void);
//static void mci_set_mode(unsigned long hz, unsigned long blklen);
//static int mmc_cmd(unsigned long cmd, unsigned long arg,void *resp, unsigned long flags);
//static int mmc_acmd(unsigned long cmd, unsigned long arg,void *resp, unsigned long flags);
unsigned int mmc_read_status();
unsigned long sd_read_page(void *buffer, unsigned long start, lbaint_t blkcnt);
unsigned long sd_write_page(unsigned long start, lbaint_t blkcnt,void *buffer);
//static void mmc_parse_cid(struct mmc_cid *cid, unsigned long *resp);
//static void sd_parse_cid(struct mmc_cid *cid, unsigned long *resp);
//static int mmc_idle_cards(void);
//static int sd_init_card(struct mmc_cid *cid, int verbose);
//static int mmc_init_card(struct mmc_cid *cid, int verbose);
//static void mci_set_data_timeout(struct mmc_csd *csd);
void mmc_controller_init();
int mmc_card_init(int verbose);


#include "mmcsd.c"

#endif /* SDMMC_H_ */
