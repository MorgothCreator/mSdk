/*
 * clk_lcd.c
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "clk_lcd.h"
#include "include/dss_cm2.h"
#include "include/dss_l3_l4per.h"

bool LCDModuleClkConfig(void)
{
	DSS_CM2.CM_DSS_DSS_CLKCTRL = DSS_CM2_MODULEMODE_EXPLICIT_ENABLE << DSS_CM2_MODULEMODE_gp;
	while((DSS_CM2.CM_DSS_DSS_CLKCTRL & DSS_CM2_MODULEMODE_gm) != (DSS_CM2_MODULEMODE_EXPLICIT_ENABLE << DSS_CM2_MODULEMODE_gp));
	DSS_CM2.CM_DSS_DSS_CLKCTRL |= DSS_CM2_OPTFCLKEN_DSSCLK_bm;
	DSS_CM2.CM_DSS_DSS_CLKCTRL |= DSS_CM2_OPTFCLKEN_48MHZ_CLK_bm;
	DSS_CM2.CM_DSS_DSS_CLKCTRL |= DSS_CM2_OPTFCLKEN_SYS_CLK_bm;
	while((DSS_CM2.CM_DSS_DSS_CLKCTRL & DSS_CM2_IDLEST_gm) != (DSS_CM2_IDLEST_FULL_FUNCTIONAL << DSS_CM2_IDLEST_gp));
	return true;
}

