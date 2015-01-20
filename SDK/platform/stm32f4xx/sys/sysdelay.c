/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include "sysdelay.h"
#include "sys/include/core_cm4.h"

extern unsigned int SystemCoreClock;
/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
volatile unsigned long long flagIsr = 1;
volatile unsigned long long STimerCnt;
//#####################################################
void SysDelayTimerSetup(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}
//#####################################################
void Sysdelay(unsigned int milliSec)
{
	flagIsr = STimerCnt + (unsigned long long)milliSec;
	while(1) {
		if(flagIsr < STimerCnt) break;
	}
}
//#####################################################
void TimerCnt_Isr_Increment(void)
{
	STimerCnt++;
}
