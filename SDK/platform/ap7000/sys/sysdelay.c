
/*
 * \file   sysdelay.c
 *
 * \brief  This file contains functions that configures a DMTimer instance
 *         for operation and to generate a requested amount of delay.
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
/*#####################################################*/
#include "sysdelay.h"
#include "driver/intc.h"
/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define TIMER_INITIAL_COUNT             (0xFFFFA23Fu)

/* If delay using interrupts is desire define this. If polling is desired,
   undefine this */
#define DELAY_USE_INTERRUPTS            1

/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
/******************************************************************************
**                      FUNCTION DEFINITIONS
*******************************************************************************/

#include "avr32/io.h"
//#include "sys/interrupts.h"
#include "nlao/nlao_exceptions.h"
#include "nlao/nlao_io.h"

#include "nlao/nlao_interrupts.h"
#include "nlao/nlao_cpu.h"
#include "nlao/nlao_usart.h"

#include "sysdelay.h"
#include "interface/rtc_interface.h"

volatile unsigned int flagIsr = 1;
volatile unsigned long long STimerCnt = 0;
volatile unsigned short SecCnt = 0;

volatile struct avr32_tc_t *tc=&AVR32_TC0;
volatile struct avr32_rtc_t *rtc=&AVR32_RTC;
volatile struct avr32_pio_t *pioa=&AVR32_PIOA;

extern volatile unsigned long long STimerCnt;

void delay(int t);
//unsigned long cnt0=25;
extern void RTCIsr(void);

//__int_handler *timer_int_handler()
#if __GNUC__
__attribute__((__interrupt__))
#elif __ICCAVR32__
__interrupt
#endif
__int_handler *timer_int_handler()
{
	//tick++;
	rtc->icr = 	0x00000001;
	STimerCnt++;
    if(flagIsr > 0)
    {
        flagIsr--;
    }
	/*if(SecCnt++)
	{
		SecCnt = 0;
		void RTCIsr(void);
	}*/
	return (void *) 0;
}

void SysDelayTimerSetup(void)
{   
	Disable_global_interrupt();
	INTC_register_interrupt((__int_handler *)timer_int_handler, AVR32_RTC_IRQ, INT0);
	Enable_global_interrupt();

	//tick=0;
	//rtc->ctrl=	0x00000205;		// prescalar 8 rtc enabled,prescalar clear=0,topen=1(enales wrap up at TOP
	rtc->ctrl= AVR32_RTC_CTRL_EN_MASK | AVR32_RTC_CTRL_TOPEN_MASK;

	rtc->val=	0x00000000;		//initial val=0
	rtc->top=	0x00000010;		//top =1000 decimal
	rtc->icr = 	0x00000001;
	rtc->ier=	0x00000001;		//interrupt enabled
}

/**
 * This function generates the requested amount of delay in milliseconds.
 */

void Sysdelay(unsigned int milliSec)
{
    flagIsr = milliSec;
    do{
		//WDR();
    }
    while(flagIsr > 0) ;
}

