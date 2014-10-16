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
//#include "pl310.h"
//#include "driver/cpu.h"
//#include "include/cache-l2x0.h"

//struct pl310_regs *const pl310 = (struct pl310_regs *)CONFIG_SYS_PL310_BASE;


/*void enable_cache()
{
	asm("	push {r0-r12, lr}	\n\t"
	"	mov		r12, #258	\n\t"
	"	mov 	r0, 1	\n\t"
	"	smc	#0	\n\t"
	"	pop {r0-r12, pc}"
);
}*/

/*void pl310_enable()
{
	//CPUSwitchToSecureMode();

    asm("    MSR   cpsr_c, #0x13|0xC0");


	//pl310->pl310_aux_ctrl &= ~L2X0_AUX_CTRL_WAY_SIZE_MASK;
	//pl310->pl310_aux_ctrl |= L2X0_AUX_CTRL_WAY_SIZE_64KB;
	pl310->pl310_clean_inv_way = 0xFFFF;
	//pl310->pl310_data_latency_ctrl = (0x07 << L2X0_AUX_CTRL_DATA_RD_LATENCY_SHIFT) | (0x07 << L2X0_AUX_CTRL_DATA_WR_LATENCY_SHIFT) | (0x07 << L2X0_AUX_CTRL_TAG_LATENCY_SHIFT);


	pl310->pl310_intr_clear = 0xFFFF;
	pl310->pl310_ctrl |= pl310_ctrl_cache_enable;
	CPUSwitchToUserMode();
}*/
