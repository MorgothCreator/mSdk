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
Rtc_t RtcStruct;

//#####################################################
void rtc_en(Rtc_t *RtcStruct)
{
	unsigned char tmp = (RtcStruct->Rtc_ClkSource<<CLK_RTCSRC_gp) |  CLK_RTCEN_bm;
	CPU_CCP = CCP_IOREG_gc;
	CLK_RTCCTRL = tmp;
	RTC_PER = RtcStruct->Rtc_PeriodValue;
	RTC_COMP = RtcStruct->Rtc_CompareValue;
	RTC_INTCTRL = (RtcStruct->Rtc_Overflow_IntLevel<<RTC_OVFINTLVL_gp) | (RtcStruct->Rtc_Compare_IntLevel<<RTC_COMPINTLVL_gp);
	RTC_CTRL = RtcStruct->Rtc_Prescaller;
}
//#####################################################
#ifndef _Rtc_Clk_Source_RCOSC32_gc
#define _Rtc_Clk_Source_RCOSC32_gc _Rtc_Clk_Source_RCOSC32_bc
#endif
void rtc_default_set(Rtc_t *RtcStruct)
{
	RtcStruct->Rtc_Compare_IntLevel			= _Rtc_Int_Disabled;
	RtcStruct->Rtc_Overflow_IntLevel		= _Rtc_Int_High_Level;
	RtcStruct->Rtc_CompareValue				= 0;
	if(RtcStruct->Rtc_ClkSource == _Rtc_Clk_Source_TOSC32_gc)
	{
		if (~OSC_CTRL & OSC_RC32KEN_bm)
		{
			OSC_CTRL|= OSC_RC32KEN_bm;
			while(~OSC_STATUS & OSC_RC32KEN_bm);
		}
		RtcStruct->Rtc_PeriodValue	= 31;
	}
	else
	{
			RtcStruct->Rtc_PeriodValue	= 10;
	}
	RtcStruct->Rtc_Prescaller = _Rtc_Prescaller_1;
	rtc_en(RtcStruct);
}
//#####################################################
void SysDelayTimerSetup(void)
{
	rtc_default_set(&RtcStruct);
}
//#####################################################
void Sysdelay(unsigned int milliSec)
{
	flagIsr = STimerCnt + milliSec;
	while(flagIsr < STimerCnt);
}
//#####################################################
ISR(RTC_OVF_vect)
{
	STimerCnt++;
}