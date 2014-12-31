/*
 * core_init.c
 *
 * Created: 2/8/2013 12:30:34 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include "core_init.h"
#include "sys/plat_properties.h"
#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "sys/system_stm32f10x.h"
#include "driver/stm32f10x_rcc.h"
//#include "driver/core.h"

unsigned long CoreFreq = 0;

void _core_init(void)
{
	SystemInit();
	//SystemCoreClockUpdate();
	//RCC_PCLK1Config(RCC_HCLK_Div4);
	//RCC_PCLK2Config(RCC_HCLK_Div2);
	gpio_init(IOA);
	gpio_init(IOB);
	gpio_init(IOC);
	gpio_init(IOD);
	gpio_init(IOE);
	gpio_init(IOF);
	gpio_init(IOG);
	gpio_init(IOH);
	gpio_init(IOI);
}
