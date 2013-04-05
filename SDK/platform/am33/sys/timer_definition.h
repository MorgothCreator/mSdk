/*
 * timer_def.h
 *
 *  Created on: Jul 15, 2012
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */

#ifndef TIMER_DEFINITION_H_
#define TIMER_DEFINITION_H_

typedef volatile unsigned int TIMER_REGISTER;

typedef struct timer_s
{
	TIMER_REGISTER TIDR;			/* 0x0 Identification Register */
	TIMER_REGISTER RESERVED_0x04_TO_0xC[(0x0C - 0x4) / sizeof(TIMER_REGISTER)];
	TIMER_REGISTER TIOCP_CFG;		/* 0x10 Timer OCP Configuration Register */
	TIMER_REGISTER RESERVED_0x14_TO_0x20[(0x24 - 0x14) / sizeof(TIMER_REGISTER)];
	TIMER_REGISTER IRQSTATUS_RAW;	/* 0x24 Timer IRQSTATUS Raw Register */
	TIMER_REGISTER IRQSTATUS;		/* 0x28 Timer IRQSTATUS Register */
	TIMER_REGISTER IRQENABLE_SET;	/* 0x2C Timer IRQENABLE Set Register */
	TIMER_REGISTER IRQENABLE_CLR;	/* 0x30 Timer IRQENABLE Clear Register */
	TIMER_REGISTER IRQWAKEEN;		/* 0x34 Timer IRQ Wakeup Enable Register */
	TIMER_REGISTER TCLR;			/* 0x38 Timer Control Register */
	TIMER_REGISTER TCRR;			/* 0x3C Timer Counter Register */
	TIMER_REGISTER TLDR;			/* 0x40 Timer Load Register */
	TIMER_REGISTER TTGR;			/* 0x44 Timer Trigger Register */
	TIMER_REGISTER TWPS;			/* 0x48 Timer Write Posted Status Register */
	TIMER_REGISTER TMAR;			/* 0x4C Timer Match Register */
	TIMER_REGISTER TCAR1;			/* 0x50 Timer Capture Register */
	TIMER_REGISTER TSICR;			/* 0x54 Timer Synchronous Interface Control Register */
	TIMER_REGISTER TCAR2;			/* 0x58 Timer Capture Register */
}timer_t;

#define TIMER0_addr (0x44E05000)
#define TIMER0 (*(timer_t *) 0x44E05000)

#define TIMER1_addr (0x44E31000)
#define TIMER1 (*(timer_t *) 0x44E31000)

#define TIMER1_1MS_addr (0x44E31000)
#define TIMER1_1MS (*(timer_t *) 0x44E31000)

#define TIMER2_addr (0x48040000)
#define TIMER2 (*(timer_t *) 0x48040000)

#define TIMER3_addr (0x48042000)
#define TIMER3 (*(timer_t *) 0x48042000)

#define TIMER4_addr (0x48044000)
#define TIMER4 (*(timer_t *) 0x48044000)

#define TIMER5_addr (0x48046000)
#define TIMER5 (*(timer_t *) 0x48046000)

#define TIMER6_addr (0x48048000)
#define TIMER6 (*(timer_t *) 0x48048000)

#define TIMER7_addr (0x4804A000)
#define TIMER7 (*(timer_t *) 0x4804A000)

#endif /* TIMER_DEF_H_ */
