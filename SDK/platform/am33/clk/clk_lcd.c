/*
 * clk_lcd.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/**********************************************/
#include "clk_lcd.h"
#include "../include/hw/hw_types.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_dpll.h"
/**********************************************/
/**
 * \brief   This function will configure the required clocks for LCDC instance.
 *
 * \return  None.
 *
 */
void LCDModuleClkConfig(void)
{
    /* lcd pixel clock is derived from peripheral pll */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_LCDC_PIXEL_CLK) =
                             CM_DPLL_CLKSEL_LCDC_PIXEL_CLK_CLKSEL_SEL3;

    HWREG(SOC_PRCM_REGS + CM_PER_LCDC_CLKCTRL) |=
                             CM_PER_LCDC_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_LCDC_CLKCTRL) &
      CM_PER_LCDC_CLKCTRL_MODULEMODE) != CM_PER_LCDC_CLKCTRL_MODULEMODE_ENABLE);
}
/**********************************************/



