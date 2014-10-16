
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
#include "../include/hw/soc_AM335x.h"
#include "../include/interrupt.h"
#include "../include/dmtimer.h"
//#include "../api/delay_api.h"
#include "../clk/clk_dmtimer.h"
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
#ifdef DELAY_USE_INTERRUPTS
static void DMTimerIsr(void);
#endif

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned int flagIsr = 1;
volatile unsigned long long STimerCnt;

/******************************************************************************
**                      FUNCTION DEFINITIONS
*******************************************************************************/

/**
 * This function configures the DMTimer7 instance. It also registers the
 * DMTimer Interrupt Service Routine in the Interrupt Controller, sets the
 * priority and enables the respective system interrupt.
 */

void SysDelayTimerSetup(void)
{   

#ifdef DELAY_USE_INTERRUPTS
    /* This function will enable clocks for the DMTimer7 instance */
    DMTimer2ModuleClkConfig(TimerClkSource_CLK_M_OSC);

	IntProtectionDisable();
    /* Registering DMTimerIsr */
    IntRegister(SYS_INT_TINT2, DMTimerIsr);

    /* Set the priority */
    IntPrioritySet(SYS_INT_TINT2, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enable the system interrupt */
    IntSystemEnable(SYS_INT_TINT2);

    IntProtectionEnable();
    DMTimerCounterSet(SOC_DMTIMER_2_REGS, TIMER_INITIAL_COUNT);

    /* Load the load register with the reload count value */
    DMTimerReloadSet(SOC_DMTIMER_2_REGS, TIMER_INITIAL_COUNT);

    /* Configure the DMTimer for Auto-reload and compare mode */
    DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);

    /* Enable the DMTimer interrupts */
    DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Start the DMTimer */
    DMTimerEnable(SOC_DMTIMER_2_REGS);
#else
    DMTimer7ModuleClkConfig();

    DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_ONESHOT_NOCMP_ENABLE);
#endif

}

/**
 * This function generates the requested amount of delay in milliseconds.
 */

void Sysdelay(unsigned int milliSec)
{
#ifdef DELAY_USE_INTERRUPTS
    //DMTimerCounterSet(SOC_DMTIMER_7_REGS, TIMER_INITIAL_COUNT);

    flagIsr = milliSec;

    /* Enable the DMTimer interrupts */
    //DMTimerIntEnable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Start the DMTimer */
    //DMTimerEnable(SOC_DMTIMER_7_REGS);
    //unsigned long long Cnt = milliSec * 250000;
    do{
		//WDR();
    }
    //while(Cnt--);
    while(flagIsr > 0) ;

    //DMTimerDisable(SOC_DMTIMER_7_REGS);

    /* Disable the DMTimer interrupts */
    //DMTimerIntDisable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

#else
    while(milliSec != 0)
    {
        DMTimerCounterSet(SOC_DMTIMER_2_REGS, 0);
        DMTimerEnable(SOC_DMTIMER_2_REGS);
        while(DMTimerCounterGet(SOC_DMTIMER_2_REGS) < 0x5DC0);
        DMTimerDisable(SOC_DMTIMER_2_REGS);
        milliSec--;
    }
 
#endif
}

#ifdef DELAY_USE_INTERRUPTS
/*
** DMTimer Interrupt Service Routine.
*/

static void DMTimerIsr(void)
{
    /* Disable the DMTimer interrupts */
    DMTimerIntDisable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Clear the status of the interrupt flags */
    DMTimerIntStatusClear(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Adding extra protection */
    if(flagIsr > 0)
    {
        flagIsr--;
    }
    STimerCnt++;

    /* Notify end of interrupt */
    //DMTimerEndOfInterrupt(SOC_DMTIMER_7_REGS);

    /* Enable the DMTimer interrupts */
    DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
}
#endif

