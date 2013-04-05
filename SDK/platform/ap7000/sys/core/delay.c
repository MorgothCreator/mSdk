/*
 * Delay.c
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */
#include "avr32/ap7000.h"
#include "sys/sysregs.h"
#include "core.h"

void  delay_us (unsigned long usec)
{
	unsigned long start;
	unsigned long ticks;
	unsigned long stop;
    //asm volatile(".global  Get_Count;Get_Count:;MFSR    R12, 0x00000108;MOV     PC, LR ");

    start = __builtin_mfsr(SYSREG_COUNT);
    ticks = usec * (Get_CPU_Clk() / 1000000);
    stop  = start + ticks;

    if (start > stop) {                                                 /* wait until Count reaches start when wrapping around      */
        while (__builtin_mfsr(SYSREG_COUNT) > start) {
            ;
        }
    }

    while (__builtin_mfsr(SYSREG_COUNT) < stop) {                             /* wait until Count reaches stop to complete delay          */
        ;
    }
}




void  delay_ms (unsigned long msec)
{
	delay_us (1000*msec);
}
