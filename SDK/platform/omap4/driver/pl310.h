/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 * Aneesh V <aneesh@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef _PL310_H_
#define _PL310_H_

#define CONFIG_SYS_PL310_BASE 0x48242000

/* Register bit fields */
#define PL310_AUX_CTRL_ASSOCIATIVITY_MASK	(1 << 16)

struct pl310_regs {
	unsigned int pl310_cache_id;
	unsigned int pl310_cache_type;
	unsigned int pad1[62];
	unsigned int pl310_ctrl;
	unsigned int pl310_aux_ctrl;
	unsigned int pl310_tag_latency_ctrl;
	unsigned int pl310_data_latency_ctrl;
	unsigned int pad2[60];
	unsigned int pl310_event_cnt_ctrl;
	unsigned int pl310_event_cnt1_cfg;
	unsigned int pl310_event_cnt0_cfg;
	unsigned int pl310_event_cnt1_val;
	unsigned int pl310_event_cnt0_val;
	unsigned int pl310_intr_mask;
	unsigned int pl310_masked_intr_stat;
	unsigned int pl310_raw_intr_stat;
	unsigned int pl310_intr_clear;
	unsigned int pad3[323];
	unsigned int pl310_cache_sync;
	unsigned int pad4[15];
	unsigned int pl310_inv_line_pa;
	unsigned int pad5[2];
	unsigned int pl310_inv_way;
	unsigned int pad6[12];
	unsigned int pl310_clean_line_pa;
	unsigned int pad7[1];
	unsigned int pl310_clean_line_idx;
	unsigned int pl310_clean_way;
	unsigned int pad8[12];
	unsigned int pl310_clean_inv_line_pa;
	unsigned int pad9[1];
	unsigned int pl310_clean_inv_line_idx;
	unsigned int pl310_clean_inv_way;
};

#define pl310_ctrl_cache_enable (1<<0)

#define pl310_aux_ctrl_parity_enable (1<<21)

#define pl310_pwr_ctrl_standby_mode_en (1<<0)

#define pl310_pwr_ctrl_dynamic_clk_gating_en (1<<1)




#define L2X0_CACHE_ID			0x000
#define L2X0_CACHE_TYPE			0x004
#define L2X0_CTRL			0x100
#define L2X0_AUX_CTRL			0x104
#define L2X0_TAG_LATENCY_CTRL		0x108
#define L2X0_DATA_LATENCY_CTRL		0x10C
#define L2X0_EVENT_CNT_CTRL		0x200
#define L2X0_EVENT_CNT1_CFG		0x204
#define L2X0_EVENT_CNT0_CFG		0x208
#define L2X0_EVENT_CNT1_VAL		0x20C
#define L2X0_EVENT_CNT0_VAL		0x210
#define L2X0_INTR_MASK			0x214
#define L2X0_MASKED_INTR_STAT		0x218
#define L2X0_RAW_INTR_STAT		0x21C
#define L2X0_INTR_CLEAR			0x220
#define L2X0_CACHE_SYNC			0x730
#define L2X0_DUMMY_REG			0x740
#define L2X0_INV_LINE_PA		0x770
#define L2X0_INV_WAY			0x77C
#define L2X0_CLEAN_LINE_PA		0x7B0
#define L2X0_CLEAN_LINE_IDX		0x7B8
#define L2X0_CLEAN_WAY			0x7BC
#define L2X0_CLEAN_INV_LINE_PA		0x7F0
#define L2X0_CLEAN_INV_LINE_IDX		0x7F8
#define L2X0_CLEAN_INV_WAY		0x7FC
/*
 * The lockdown registers repeat 8 times for L310, the L210 has only one
 * D and one I lockdown register at 0x0900 and 0x0904.
 */
#define L2X0_LOCKDOWN_WAY_D_BASE	0x900
#define L2X0_LOCKDOWN_WAY_I_BASE	0x904
#define L2X0_LOCKDOWN_STRIDE		0x08
#define L2X0_ADDR_FILTER_START		0xC00
#define L2X0_ADDR_FILTER_END		0xC04
#define L2X0_LOCKDOWN_WAY_D0		0x900
#define L2X0_LOCKDOWN_WAY_D1		0x908
#define L2X0_LOCKDOWN_WAY_I0		0x904
#define L2X0_LOCKDOWN_WAY_I1		0x90C

#define L2X0_TEST_OPERATION		0xF00
#define L2X0_LINE_DATA			0xF10
#define L2X0_LINE_TAG			0xF30
#define L2X0_DEBUG_CTRL			0xF40
#define L2X0_PREFETCH_CTRL		0xF60
#define L2X0_POWER_CTRL			0xF80
#define L2X0_DYNAMIC_CLK_GATING_EN	(1 << 1)
#define L2X0_STNDBY_MODE_EN		(1 << 0)

/* Registers shifts and masks */
#define L2X0_CACHE_ID_REV_MASK		(0x3f)
#define L2X0_CACHE_ID_PART_MASK		(0xf << 6)
#define L2X0_CACHE_ID_PART_L210		(1 << 6)
#define L2X0_CACHE_ID_PART_L310		(3 << 6)
#define L2X0_CACHE_ID_RTL_MASK          0x3f
#define L2X0_CACHE_ID_RTL_R0P0          0x0
#define L2X0_CACHE_ID_RTL_R1P0          0x2
#define L2X0_CACHE_ID_RTL_R2P0          0x4
#define L2X0_CACHE_ID_RTL_R3P0          0x5
#define L2X0_CACHE_ID_RTL_R3P1          0x6
#define L2X0_CACHE_ID_RTL_R3P2          0x8

#define L2X0_AUX_CTRL_MASK			0xd0000fff
#define L2X0_AUX_CTRL_DATA_RD_LATENCY_SHIFT	4
#define L2X0_AUX_CTRL_DATA_RD_LATENCY_MASK	(0x7 << L2X0_AUX_CTRL_DATA_RD_LATENCY_SHIFT)
#define L2X0_AUX_CTRL_DATA_WR_LATENCY_SHIFT	8
#define L2X0_AUX_CTRL_DATA_WR_LATENCY_MASK	(0x7 << L2X0_AUX_CTRL_DATA_WR_LATENCY_SHIFT)
#define L2X0_AUX_CTRL_TAG_LATENCY_SHIFT		0
#define L2X0_AUX_CTRL_TAG_LATENCY_MASK		(0x7 << L2X0_AUX_CTRL_TAG_LATENCY_SHIFT)
#define L2X0_AUX_CTRL_DIRTY_LATENCY_SHIFT	9
#define L2X0_AUX_CTRL_DIRTY_LATENCY_MASK	(0x7 << 9)
#define L2X0_AUX_CTRL_ASSOCIATIVITY_SHIFT	16
#define L2X0_AUX_CTRL_WAY_SIZE_SHIFT		17
#define L2X0_AUX_CTRL_WAY_SIZE_MASK		(0x7 << 17)
#define L2X0_AUX_CTRL_WAY_SIZE_16KB			(1 << L2X0_AUX_CTRL_WAY_SIZE_SHIFT)
#define L2X0_AUX_CTRL_WAY_SIZE_32KB			(2 << L2X0_AUX_CTRL_WAY_SIZE_SHIFT)
#define L2X0_AUX_CTRL_WAY_SIZE_64KB			(3 << L2X0_AUX_CTRL_WAY_SIZE_SHIFT)
#define L2X0_AUX_CTRL_WAY_SIZE_128KB		(4 << L2X0_AUX_CTRL_WAY_SIZE_SHIFT)
#define L2X0_AUX_CTRL_WAY_SIZE_256KB		(5 << L2X0_AUX_CTRL_WAY_SIZE_SHIFT)
#define L2X0_AUX_CTRL_WAY_SIZE_512KB		(6 << L2X0_AUX_CTRL_WAY_SIZE_SHIFT)
#define L2X0_AUX_CTRL_SHARE_OVERRIDE_SHIFT	22
#define L2X0_AUX_CTRL_NS_LOCKDOWN_SHIFT		26
#define L2X0_AUX_CTRL_NS_INT_CTRL_SHIFT		27
#define L2X0_AUX_CTRL_DATA_PREFETCH_SHIFT	28
#define L2X0_AUX_CTRL_INSTR_PREFETCH_SHIFT	29
#define L2X0_AUX_CTRL_EARLY_BRESP_SHIFT		30

#define L2X0_LATENCY_CTRL_SETUP_SHIFT	0
#define L2X0_LATENCY_CTRL_RD_SHIFT	4
#define L2X0_LATENCY_CTRL_WR_SHIFT	8

#define L2X0_ADDR_FILTER_EN		1

#define L2X0_PREFETCH_DATA_PREFETCH_SHIFT	28
#define L2X0_PREFETCH_INTSTR_PREFETCH_SHIFT	29
#define L2X0_PREFETCH_DOUBLE_LINEFILL_SHIFT	30

#define REV_PL310_R2P0				4


void pl310_enable();

#ifdef HEADER_INCLUDE_C_FILES
#include "pl310.c"
#endif

#endif
