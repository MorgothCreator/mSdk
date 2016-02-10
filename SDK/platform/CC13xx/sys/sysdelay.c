/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */ 

#include "sysdelay.h"

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned long long flagIsr = 1;
static volatile unsigned long long STimerCnt;
//Rtc_t RtcStruct;

//#####################################################
void SysDelayTimerSetup(void)
{

}
//#####################################################
void Sysdelay(unsigned int milliSec)
{
	flagIsr = STimerCnt + milliSec;
	while(flagIsr < STimerCnt);
}
//#####################################################
/*ISR(RTC_OVF_vect)
{
	STimerCnt++;
}*/
