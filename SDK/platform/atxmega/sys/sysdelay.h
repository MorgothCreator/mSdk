/*
 * sysdelay.h
 *
 * Created: 2/8/2013 10:53:25 PM
 *  Author: XxXx
 */ 
//#####################################################
#ifndef SYSDELAY_H_
#define SYSDELAY_H_
//#####################################################
#include <avr/io.h>
#include <avr/interrupt.h>
//#####################################################
#define _Rtc_Int_Disabled					(0)
#define _Rtc_Int_Low_Level					(1)
#define _Rtc_Int_Med_Level					(2)
#define _Rtc_Int_High_Level					(3)
//-----------------------------------------------------
#define _Rtc_Prescaller_Off					(0)
#define _Rtc_Prescaller_1					(RTC_PRESCALER_DIV1_gc)
#define _Rtc_Prescaller_2					(RTC_PRESCALER_DIV2_gc)
#define _Rtc_Prescaller_8					(RTC_PRESCALER_DIV8_gc)
#define _Rtc_Prescaller_16					(RTC_PRESCALER_DIV16_gc)
#define _Rtc_Prescaller_64					(RTC_PRESCALER_DIV64_gc)
#define _Rtc_Prescaller_256					(RTC_PRESCALER_DIV256_gc)
#define _Rtc_Prescaller_1024				(RTC_PRESCALER_DIV1024_gc)

//#ifndef CLK_RTCSRC_RCOSC32_gc
//#define CLK_RTCSRC_RCOSC32_gc CLK_RTCSRC_TOSC32_gc
//#endif


#define _Rtc_Clk_Source_Ulp					(CLK_RTCSRC_ULP_gc>>1)  /* 1.024 kHz from internal 32kHz ULP */
#define _Rtc_Clk_Source_TOSC_gc				(CLK_RTCSRC_TOSC_gc>>1)  /* 1.024 kHz from 32.768 kHz crystal oscillator on TOSC */
#define _Rtc_Clk_Source_RCOSC_gc			(CLK_RTCSRC_RCOSC_gc>>1)  /* 1.024 kHz from internal 32.768 kHz RC oscillator */
#define _Rtc_Clk_Source_TOSC32_gc			(CLK_RTCSRC_TOSC32_gc>>1)  /* 32.768 kHz from 32.768 kHz crystal oscillator on TOSC */
#define _Rtc_Clk_Source_RCOSC32_gc			(CLK_RTCSRC_RCOSC32_gc>>1)  /* 32.768 kHz from internal 32.768 kHz RC oscillator */
#ifdef CLK_RTCSRC_EXTCLK_gc
#define _Rtc_Clk_Source_EXTCLK_gc			(CLK_RTCSRC_EXTCLK_gc>>1)  /* External Clock from TOSC1 */
#endif

//#####################################################
typedef struct Rtc_Struct
{
	unsigned short Rtc_PeriodValue;			//
	unsigned short Rtc_CompareValue;			//
	unsigned char Rtc_ClkSource;				//
	unsigned char Rtc_Prescaller;				//
	unsigned char Rtc_Overflow_IntLevel;		//
	unsigned char Rtc_Compare_IntLevel;		//
	//uint32_t Rtc_Ret_Value;
} Rtc_t;
//#####################################################
extern Rtc_t RtcStruct;
//#####################################################
void SysDelayTimerSetup(void);
void Sysdelay(unsigned int milliSec);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "sysdelay.c"
#endif
//#####################################################
#endif /* SYSDELAY_H_ */
//#####################################################
