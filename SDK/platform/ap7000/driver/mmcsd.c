/*
 * SdMmc.c
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */
#include "avr32/ap7000.h"
#include <stdio.h>
#include <string.h>
#include "sys/core/core.h"
#include "gpio.h"
#include "mmcsd.h"
#include "sys/core/delay.h"
#include "api/uart_api.h"
#include "../nlao/nlao_io.h"

static unsigned int mmc_rca;
static int mmc_card_is_sd;
static block_dev_desc_t mmc_blkdev;
static unsigned char SdCardType;
extern new_uart* DebugCom;

void MMC_setio(void)
{


	IO_SetFnct(AVR32_MCI_CLK_0_PIN, AVR32_MCI_CLK_0_FUNCTION);
	IO_SetFnct(AVR32_MCI_CMD_0_PIN, AVR32_MCI_CMD_0_FUNCTION);
	IO_SetFnct(AVR32_MCI_CMD_1_PIN, AVR32_MCI_CMD_1_FUNCTION);
	IO_SetFnct(AVR32_MCI_DATA_0_PIN, AVR32_MCI_DATA_0_FUNCTION);
	IO_SetFnct(AVR32_MCI_DATA_1_PIN, AVR32_MCI_DATA_1_FUNCTION);
	IO_SetFnct(AVR32_MCI_DATA_2_PIN, AVR32_MCI_DATA_2_FUNCTION);
	IO_SetFnct(AVR32_MCI_DATA_3_PIN, AVR32_MCI_DATA_3_FUNCTION);

}


static void mci_set_mode(unsigned long hz, unsigned long blklen)
{
	unsigned long bus_hz;
	unsigned long clkdiv;

	bus_hz = Get_PBB_Clk();
	clkdiv = (bus_hz / hz) / 2 - 1;

	//print("mmc: setting clock %u Hz, block size %u\r\n",hz, blklen);

	if (clkdiv & ~255UL) {
		clkdiv = 255;
		//print("mmc: clock %u too low; setting CLKDIV to 255\r\n",hz);
	}

	blklen &= 0xfffc;
	mmci_writel(MR, (MMCI_BF(CLKDIV, clkdiv)| MMCI_BF(BLKLEN, blklen)| MMCI_BIT(RDPROOF)| MMCI_BIT(WRPROOF)));
}


static int mmc_cmd(unsigned long cmd, unsigned long arg,void *resp, unsigned long flags)
{
	unsigned long *response = resp;
	int i, response_words = 0;
	unsigned long error_flags;
	unsigned int status;

	//print("mmc: CMD%u 0x%x (flags 0x%x)\r\n",cmd, arg, flags);

	error_flags = ERROR_FLAGS;
	if (!(flags & RESP_NO_CRC))
		error_flags |= MMCI_BIT(RCRCE);

	flags &= ~MMCI_BF(CMDNB, ~0UL);

	if (MMCI_BFEXT(RSPTYP, flags) == MMCI_RSPTYP_48_BIT_RESP)
		response_words = 1;
	else if (MMCI_BFEXT(RSPTYP, flags) == MMCI_RSPTYP_136_BIT_RESP)
		response_words = 4;

	mmci_writel(ARGR, arg);
	mmci_writel(CMDR, cmd | flags);
	do {
		delay_us(40);
		status = mmci_readl(SR);
	} while (!(status & MMCI_BIT(CMDRDY)));

	//print("mmc: status 0x%08x\r\n", status);

	if (status & error_flags) {
		//print("mmc: command %u failed (status: 0x%08x)\n",cmd, status);
		return -1;
	}

	//if (response_words)
		//print("mmc: response:");

	for (i = 0; i < response_words; i++) {
		response[i] = mmci_readl(RSPR);
		//print(" %08x", response[i]);
	}
	//print("\r\n");

	return 0;
}

static int mmc_acmd(unsigned long cmd, unsigned long arg,void *resp, unsigned long flags)
{
	unsigned long aresp[4];
	int ret;

	/*
	 * Seems like the APP_CMD part of an ACMD has 64 cycles max
	 * latency even though the ACMD part doesn't. This isn't
	 * entirely clear in the SD Card spec, but some cards refuse
	 * to work if we attempt to use 5 cycles max latency here...
	 */
	ret = mmc_cmd(MMC_CMD_APP_CMD, 0, aresp,
		      R1 | NCR | (flags & OPEN_DRAIN));
	if (ret)
		return ret;
	if ((aresp[0] & (R1_ILLEGAL_COMMAND | R1_APP_CMD)) != R1_APP_CMD)
		return -1;

	ret = mmc_cmd(cmd, arg, resp, flags);
	return ret;
}


unsigned int mmc_read_status()
{
	unsigned int status;
	return status = mmci_readl(SR);

}



unsigned long sd_read_page(void *buffer, unsigned long start, lbaint_t blkcnt)
{
	int ret, i = 0;
	unsigned long resp[4];
	unsigned long card_status, data;
	unsigned long wordcount;
	unsigned int *p = buffer;
	unsigned int status;

	if (blkcnt == 0)
		return 0;

//print("mmc_bread: dev %d, start %x, blkcnt %x\r\n",0, start, blkcnt);

	irqflags_t int_reg_backup = cpu_irq_save();
	/* Put the device into Transfer state */
	ret = mmc_cmd(MMC_CMD_SELECT_CARD, mmc_rca << 16, resp, R1 | NCR);
	if (ret) goto out;

	/* Set block length */
	ret = mmc_cmd(MMC_CMD_SET_BLOCKLEN, mmc_blkdev.blksz, resp, R1 | NCR);
	if (ret) goto out;

	//print("MCI_DTOR = %08x\r\n", mmci_readl(DTOR));

	for (i = 0; i < blkcnt; i++, start++) {
		if(SdCardType == IsSD)
		{
			ret = mmc_cmd(MMC_CMD_READ_SINGLE_BLOCK, start * mmc_blkdev.blksz, resp, (R1 | NCR | TRCMD_START | TRDIR_READ | TRTYP_BLOCK));
		}
		else
		{
			ret = mmc_cmd(MMC_CMD_READ_SINGLE_BLOCK, start, resp, (R1 | NCR | TRCMD_START | TRDIR_READ | TRTYP_BLOCK));
		}
		if (ret) goto out;

		ret = -1;
		wordcount = 0;
		do {
			do {
				status = mmci_readl(SR);
				if (status & (ERROR_FLAGS | MMCI_BIT(OVRE)))
					goto read_error;
			} while (!(status & MMCI_BIT(RXRDY)));

			if (status & MMCI_BIT(RXRDY)) {
				data = mmci_readl(RDR);
				/* pr_debug("%x\n", data); */
				*p++ = data;
				wordcount++;
			}
		} while(wordcount < (mmc_blkdev.blksz / 4));

		//print("mmc: read %u words, waiting for BLKE\r\n", wordcount);

		do {
			status = mmci_readl(SR);
		} while (!(status & MMCI_BIT(BLKE)));

		//print("Done\r\n");
	}

out:
	/* Put the device back into Standby state */
	mmc_cmd(MMC_CMD_SELECT_CARD, 0, resp, NCR);
	cpu_irq_restore(int_reg_backup);
	return i;

read_error:
	mmc_cmd(MMC_CMD_SEND_STATUS, mmc_rca << 16, &card_status, R1 | NCR);
	//print("mmc: bread failed, status = %08x, card status = %08x\r\n",status, card_status);
	cpu_irq_restore(int_reg_backup);
	goto out;
}

unsigned long sd_write_page(unsigned long start, lbaint_t blkcnt,void *buffer)
{
	int ret, i = 0;
	unsigned long resp[4];
	unsigned long card_status, data;
	unsigned long wordcount;
	unsigned int *p = buffer;
	unsigned int status;

	if (blkcnt == 0)
		return 0;

	//print("mmc_bwrite: dev %d, start %x, blkcnt %x\r\n",dev, start, blkcnt);

	irqflags_t int_reg_backup = cpu_irq_save();
	/* Put the device into Transfer state */
	ret = mmc_cmd(MMC_CMD_SELECT_CARD, mmc_rca << 16, resp, R1 | NCR);
	if (ret) goto out;

	/* Set block length */
	ret = mmc_cmd(MMC_CMD_SET_BLOCKLEN, mmc_blkdev.blksz, resp, R1 | NCR);
	if (ret) goto out;

	//print("MCI_DTOR = %08x\r\n", mmci_readl(DTOR));

	for (i = 0; i < blkcnt; i++, start++) {
		if(SdCardType == IsSD)
		{
			ret = mmc_cmd(MMC_CMD_WRITE_BLOCK, start * mmc_blkdev.blksz, resp, (R1 | NCR | TRCMD_START | TRDIR_WRITE | TRTYP_BLOCK));
		}
		else
		{
			ret = mmc_cmd(MMC_CMD_WRITE_BLOCK, start, resp, (R1 | NCR | TRCMD_START | TRDIR_WRITE | TRTYP_BLOCK));
		}
		if (ret) goto out;

		ret = -1;
		wordcount = 0;
		do {
			do {
				status = mmci_readl(SR);
				if (status & (ERROR_FLAGS | MMCI_BIT(OVRE)))
					goto read_error;
			} while (!(status & MMCI_BIT(TXRDY)));

			if (status & MMCI_BIT(TXRDY)) {
				data=*p++;
				mmci_writel(TDR,data);
				//data = mmci_readl(RDR);
				 //print("%08x ", data);
				wordcount++;
			}
			//print("\r\n\n");
		} while(wordcount < (mmc_blkdev.blksz / 4));

		//print("mmc: write %u words, waiting for BLKE\r\n", wordcount);

		do {
			status = mmci_readl(SR);
		} while (!(status & MMCI_BIT(BLKE)));

		//print("Done\r\n");
	}

out:
	/* Put the device back into Standby state */
	mmc_cmd(MMC_CMD_SELECT_CARD, 0, resp, NCR);
	//print("mmc: brite i=%d\r\n",i);
	cpu_irq_restore(int_reg_backup);
	return i;

read_error:
	mmc_cmd(MMC_CMD_SEND_STATUS, mmc_rca << 16, &card_status, R1 | NCR);
	//print("mmc: bwrite failed, status = %08x, card status = %08x\r\n",status, card_status);
	cpu_irq_restore(int_reg_backup);
	goto out;
}






static void mmc_parse_cid(struct mmc_cid *cid, unsigned long *resp)
{
	cid->mid = resp[0] >> 24;
	cid->oid = (resp[0] >> 8) & 0xffff;
	cid->pnm[0] = resp[0];
	cid->pnm[1] = resp[1] >> 24;
	cid->pnm[2] = resp[1] >> 16;
	cid->pnm[3] = resp[1] >> 8;
	cid->pnm[4] = resp[1];
	cid->pnm[5] = resp[2] >> 24;
	cid->pnm[6] = 0;
	cid->prv = resp[2] >> 16;
	cid->psn = (resp[2] << 16) | (resp[3] >> 16);
	cid->mdt = resp[3] >> 8;
}

static void sd_parse_cid(struct mmc_cid *cid, unsigned long *resp)
{
	cid->mid = resp[0] >> 24;
	cid->oid = (resp[0] >> 8) & 0xffff;
	cid->pnm[0] = resp[0];
	cid->pnm[1] = resp[1] >> 24;
	cid->pnm[2] = resp[1] >> 16;
	cid->pnm[3] = resp[1] >> 8;
	cid->pnm[4] = resp[1];
	cid->pnm[5] = 0;
	cid->pnm[6] = 0;
	cid->prv = resp[2] >> 24;
	cid->psn = (resp[2] << 8) | (resp[3] >> 24);
	cid->mdt = (resp[3] >> 8) & 0x0fff;
}

static void mmc_dump_cid(const struct mmc_cid *cid)
{
	UARTprintf(DebugCom, "Manufacturer ID:         %02X\n", cid->mid);
	UARTprintf(DebugCom, "OEM/Application ID:      %04X\n", cid->oid);
	UARTprintf(DebugCom, "Product name:            %s\n", cid->pnm);
	UARTprintf(DebugCom, "Product Revision:        %u.%u\n", cid->prv >> 4, cid->prv & 0x0f);
	UARTprintf(DebugCom, "Product Serial Number:   %u\n", cid->psn);
	UARTprintf(DebugCom, "Manufacturing Date:      %02u/%02u\n",cid->mdt >> 4, cid->mdt & 0x0f);
}

static void mmc_dump_csd(const struct mmc_csd *csd)
{
	unsigned long *csd_raw = (unsigned long *)csd;
	UARTprintf(DebugCom, "CSD data:                %08lx %08lx %08lx %08x\n",
	       csd_raw[0], csd_raw[1], csd_raw[2], csd_raw[3]);
	UARTprintf(DebugCom, "CSD structure version:   1.%u\n", csd->csd_structure);
	UARTprintf(DebugCom, "MMC System Spec version: %u\n", csd->spec_vers);
	UARTprintf(DebugCom, "Card command classes:    %03x\n", csd->ccc);
	UARTprintf(DebugCom, "Read block length:       %u\n", 1 << csd->read_bl_len);
	if (csd->read_bl_partial)
		UARTprintf(DebugCom, "Supports partial reads\n");
	else
		UARTprintf(DebugCom, "Does not support partial reads\n");
	UARTprintf(DebugCom, "Write block length:      %u\n", 1 << csd->write_bl_len);
	if (csd->write_bl_partial)
		UARTprintf(DebugCom, "Supports partial writes\n");
	else
		UARTprintf(DebugCom, "Does not support partial writes\n");
	if (csd->wp_grp_enable)
		UARTprintf(DebugCom, "Supports group WP:       %u\n", csd->wp_grp_size + 1);
	else
		UARTprintf(DebugCom, "Does not support group WP\n");
	UARTprintf(DebugCom, "Card capacity:		 %u bytes\n", (csd->c_size + 1) * (1 << (csd->c_size_mult + 2)) * (1 << csd->read_bl_len));
	UARTprintf(DebugCom, "File format:             %u/%u\n", csd->file_format_grp, csd->file_format);
	UARTprintf(DebugCom, "Write protection:        ");
	if (csd->perm_write_protect)
		UARTprintf(DebugCom, " permanent");
	if (csd->tmp_write_protect)
		UARTprintf(DebugCom, " temporary");
	UARTprintf(DebugCom, "\r\n");
}

static int mmc_idle_cards(void)
{
	int ret;

	/* Reset and initialize all cards */
	ret = mmc_cmd(MMC_CMD_GO_IDLE_STATE, 0, NULL, 0);
	if (ret)
		return ret;

	/* Keep the bus idle for 74 clock cycles */
	return mmc_cmd(0, 0, NULL, INIT_CMD);
}

static int sd_init_card(struct mmc_cid *cid, int verbose)
{
	unsigned long resp[4];
	int i, ret = 0;

	mmc_idle_cards();
	for (i = 0; i < 1000; i++) {
		ret = mmc_acmd(SD_CMD_APP_SEND_OP_COND, CFG_MMC_OP_COND,
			       resp, R3 | NID);
		if (ret || (resp[0] & 0x80000000))
			break;
		ret = -2;
	}
	SdCardType = IsSD;
	if (ret)
	{
		SdCardType = IsSDHC;
		return ret;
	}

	ret = mmc_cmd(MMC_CMD_ALL_SEND_CID, 0, resp, R2 | NID);
	if (ret)
		return ret;
	sd_parse_cid(cid, resp);
	if (verbose)
		mmc_dump_cid(cid);

	/* Get RCA of the card that responded */
	ret = mmc_cmd(SD_CMD_SEND_RELATIVE_ADDR, 0, resp, R6 | NCR);
	if (ret)
		return ret;

	mmc_rca = resp[0] >> 16;
	if (verbose)
		UARTprintf(DebugCom, "SD Card detected         (RCA %u)\n", mmc_rca);
	mmc_card_is_sd = 1;
	return 0;
}

static int mmc_init_card(struct mmc_cid *cid, int verbose)
{
	unsigned long resp[4];
	int i, ret = 0;

	mmc_idle_cards();
	for (i = 0; i < 1000; i++) {
		ret = mmc_cmd(MMC_CMD_SEND_OP_COND, CFG_MMC_OP_COND, resp,
			      R3 | NID | OPEN_DRAIN);
		if (ret || (resp[0] & 0x80000000))
			break;
		ret = -2;
	}

	if (ret)
		return ret;

	/* Get CID of all cards. FIXME: Support more than one card */
	ret = mmc_cmd(MMC_CMD_ALL_SEND_CID, 0, resp, R2 | NID | OPEN_DRAIN);
	if (ret)
		return ret;
	mmc_parse_cid(cid, resp);
	if (verbose)
		mmc_dump_cid(cid);

	/* Set Relative Address of the card that responded */
	ret = mmc_cmd(MMC_CMD_SET_RELATIVE_ADDR, mmc_rca << 16, resp,
		      R1 | NCR | OPEN_DRAIN);
	return ret;
}

static void mci_set_data_timeout(struct mmc_csd *csd)
{
	static const unsigned int dtomul_to_shift[] = {
		0, 4, 7, 8, 10, 12, 16, 20,
	};
	static const unsigned int taac_exp[] = {
		1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
	};
	static const unsigned int taac_mant[] = {
		0,  10, 12, 13, 15, 60, 25, 30,
		35, 40, 45, 50, 55, 60, 70, 80,
	};
	unsigned int timeout_ns, timeout_clks;
	unsigned int e, m;
	unsigned int dtocyc, dtomul;
	unsigned int shift;
	unsigned int dtor;


	e = csd->taac & 0x07;
	m = (csd->taac >> 3) & 0x0f;

	timeout_ns = (taac_exp[e] * taac_mant[m] + 9) / 10;
	timeout_clks = csd->nsac * 100;

	timeout_clks += (((timeout_ns + 9) / 10)
			 * ((CFG_MMC_CLK_PP + 99999) / 100000) + 9999) / 10000;
	if (!mmc_card_is_sd)
		timeout_clks *= 10;
	else
		timeout_clks *= 100;

	dtocyc = timeout_clks;
	dtomul = 0;
	shift = 0;
	while (dtocyc > 15 && dtomul < 8) {
		dtomul++;
		shift = dtomul_to_shift[dtomul];
		dtocyc = (timeout_clks + (1 << shift) - 1) >> shift;
	}

	if (dtomul >= 8) {
		dtomul = 7;
		dtocyc = 15;
		UARTprintf(DebugCom, "Warning: Using maximum data timeout\n");
	}

	dtor = (MMCI_BF(DTOMUL, dtomul)
		| MMCI_BF(DTOCYC, dtocyc));
	mmci_writel(DTOR, dtor);

	//UARTprintf(DebugCom, "mmc: Using %u cycles data timeout (DTOR=0x%x)\n",dtocyc << shift, dtor);
}









void mmc_controller_init()
{
	MMC_setio();
}

int mmc_card_init(int verbose)
{
	struct mmc_cid cid;
	struct mmc_csd csd;
	unsigned int max_blksz;
	int ret;

	/* Initialize controller */
	mmci_writel(CR, MMCI_BIT(SWRST));
	mmci_writel(CR, MMCI_BIT(MCIEN));
	mmci_writel(DTOR, 0x5f);
	mmci_writel(IDR, ~0UL);
	mci_set_mode(CFG_MMC_CLK_OD, MMC_DEFAULT_BLKLEN);

	mmc_card_is_sd = 0;

	ret = sd_init_card(&cid, verbose);
	if (ret) {
		mmc_rca = MMC_DEFAULT_RCA;
		ret = mmc_init_card(&cid, verbose);
	}
	if (ret)
		return ret;

	/* Get CSD from the card */
	ret = mmc_cmd(MMC_CMD_SEND_CSD, mmc_rca << 16, &csd, R2 | NCR);
	if (ret)
		return ret;
	if (verbose)
		mmc_dump_csd(&csd);

	mci_set_data_timeout(&csd);

	/* Initialize the blockdev structure */
	mmc_blkdev.if_type = IF_TYPE_MMC;
	mmc_blkdev.part_type = PART_TYPE_DOS;
	//mmc_blkdev.block_read = sd_read_page;
	//mmc_blkdev.block_write = mmc_bwrite;
	UARTprintf(DebugCom, (char *)mmc_blkdev.vendor,"Man %02x%04x Snr %08x",cid.mid, cid.oid, cid.psn);
	strncpy((char *)mmc_blkdev.product, cid.pnm, sizeof(mmc_blkdev.product));
	//UARTprintf(DebugCom, (char *)mmc_blkdev.revision, "%x %x", cid.prv >> 4, cid.prv & 0x0f);

	/*
	 * If we can't use 512 byte blocks, refuse to deal with the
	 * card. Tons of code elsewhere seems to depend on this.
	 */
	max_blksz = 1 << csd.read_bl_len;
	if (max_blksz < 512 || (max_blksz > 512 && !csd.read_bl_partial)) {
		//UARTprintf(DebugCom, "Card does not support 512 byte reads, aborting.\n");
		return -1;
	}
	mmc_blkdev.blksz = 512;
	mmc_blkdev.lba = (csd.c_size + 1) * (1 << (csd.c_size_mult + 2));

	mci_set_mode(CFG_MMC_CLK_PP, mmc_blkdev.blksz);
	return 1;
}
