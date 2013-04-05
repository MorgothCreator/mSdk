/*
 * clk_tsc_adc.c
 *
 *  Created on: Dec 11, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "clk_tsc_adc.h"
#include "../include/hw/hw_types.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_cm_wkup.h"
/*#####################################################*/
/*
** This function enables the system L3 and system L4_WKUP clocks.
** This also enables the clocks for TSC instance.
*/

void TSCADCModuleClkConfig(void)
{
    /* Writing to MODULEMODE field of CM_WKUP_TSC_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) |=
          CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE));

    /*
    ** Waiting for CLKACTIVITY_ADC_FCLK field in CM_WKUP_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_ADC_TSC_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST));
}
/*#####################################################*/




