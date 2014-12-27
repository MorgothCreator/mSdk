/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include "sysdelay.h"
#include "board_properties.h"
#include "sys/core_cm3.h"

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
volatile unsigned long long flagIsr = 1;
volatile unsigned long long STimerCnt;
//#####################################################
void SysDelayTimerSetup(void)
{
	SysTick_Config((CoreFreq_Mhz * 1000000) / 1000);
}
//#####################################################
void Sysdelay(unsigned int milliSec)
{
	flagIsr = STimerCnt + milliSec;
	while(flagIsr < STimerCnt);
}
//#####################################################
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	STimerCnt++;
}
//ISR(RTC_OVF_vect)
//{
//	STimerCnt++;
//}
