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
#include "driver/core.h"

unsigned long CoreFreq = 0;

#ifndef F_CPU
#define F_CPU	32000000
#endif

#ifndef CoreSourceClock
#define CoreSourceClock		(CoreClk_Source_PLL)
#endif

#ifndef CoreClkDivider
#define CoreClkDivider	1
#endif

#ifndef CoreExternClk
#define CoreExternClk	false
#endif


void _core_init(void)
{
	CoreFreq = core_clk_set(F_CPU, CoreSourceClock, CoreClkDivider, CoreExternClk);
	int_lvl_enable(true, true, true);
}
