/*
 * hlos.c
 *
 *  Created on: Jan 15, 2013
 *      Author: XxXx
 */

#include "hlos.h"

//R0 Debug register value to set
/*void hlos_cache_write_debug_reg()
{
	asm("	push	{R1-R12,	LR}\n\t"
		"	ldr	r12,	#0x100\n\t"
		"	smc	0x1\n\t"
		"	pop	{R1-R12, PC}");
}*/


/*__asm("    .sect \".text:hlos_cache_clean_invalidate_range\"\n"
	"    .clink\n"
	"    .global hlos_cache_clean_invalidate_range\n"
	"hlos_cache_clean_invalidate_range:\n"
	"	PUSH {R1-R12, LR}\n"
	"	LDR r12, #0x101\n"
	"	SMC 0x1\n"
	"	PUSH {R1-R12, PC}\n"
	"    bx      lr");*/


//Input R0 Physical Start Address of the range
//		R1 Size of the range to invalidate
/*void hlos_cache_clean_invalidate_range()
{
	asm("	PUSH {R1-R12, LR}\n\t"
		"	LDR r12, #0x101\n\t"
		"	SMC 0x1\n\t"
		"	PUSH {R1-R12, PC}");
}*/
//Input R0 Control register value to set
/*void hlos_cache_enable_disable()
{
	asm("	push {R1-R12, LR}\n\t"
		"	ldr r12, #0x102\n\t"
		"	smc 0x1\n\t"
		"	pop {R1-R12, PC}");
}*/
//Input R0 Value to set in the register
/*void hlos_cache_write_auxiliary_reg()
{
	asm("	push {R1-R12, LR}\n\t"
		"	ldr r12, #0x109\n\t"
		"	smc 0x1\n\t"
		"	pop {R1-R12, PC}");
}*/
//Input R0 Tag RAM latency to set
//		R1 Data RAM latency to set
/*void hlos_cache_write_tag_and_latency_ram_reg()
{
	asm("	push {R1-R12, LR}\n\t"
		"	ldr r12, #0x112\n\t"
		"	smc 0x1\n\t"
		"	pop {R1-R12, PC}");
}

//Input R0 Value to set in the registe
void hlos_cache_write_prefetch_control_ram_reg()
{
	asm("	push {R1-R12, LR}\n\t"
		"	ldr r12, #0x113\n\t"
		"	smc 0x1\n\t"
		"	pop {R1-R12, PC}");
}*/






