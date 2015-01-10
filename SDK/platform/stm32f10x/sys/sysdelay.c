/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */

#include "sysdelay.h"
#include "plat_properties.h"
#include "sys/core_cm3.h"

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
volatile unsigned long long flagIsr = 1;
volatile unsigned long long STimerCnt;

struct {
	void *func[8];
}sys_service;
//#####################################################
void SysDelayTimerSetup(void)
{
	SysTick_Config((CoreFreq_Mhz * 1000000) / 1000);
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
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	STimerCnt++;
	int service_cnt = 0;
	void (*appEntry)();
	while(sys_service.func[service_cnt] && service_cnt < 8) {
		appEntry = (void (*)(void)) sys_service.func[service_cnt];
		(*appEntry)();
		service_cnt++;
	}
}
//ISR(RTC_OVF_vect)
//{
//	STimerCnt++;
//}
