/*
 * core_init.c
 *
 * Created: 2/8/2013 12:30:34 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include "core_init.h"
#include "board_properties.h"
#include "api/gpio_api.h"
#include "sys/system_stm32f4xx.h"
#include "driver/stm32f4xx_hal_rcc.h"
//#include "driver/core.h"

void _core_init(void)
{
	//SystemInit();
	SystemCoreClockUpdate();
	//RCC_PCLK1Config(RCC_HCLK_Div4);
	//RCC_PCLK2Config(RCC_HCLK_Div2);
	gpio_init(0);
	gpio_init(1);
	gpio_init(2);
	gpio_init(3);
	gpio_init(4);
	gpio_init(5);
	gpio_init(6);
	gpio_init(7);
	gpio_init(8);
}
