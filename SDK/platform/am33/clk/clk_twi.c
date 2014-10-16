/*
 * clk_twi.c
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "clk_twi.h"
#include "../include/hw/hw_types.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_cm_wkup.h"
#include "api/twi_def.h"
/*#####################################################*/
void TWIModuleClkConfig(new_twi* TwiStruct)
{
    switch(TwiStruct->TwiNr)
    {
    case 0:
		/* Writing to MODULEMODE field of CM_WKUP_UART0_CLKCTRL register. */
		HWREG(SOC_CM_WKUP_REGS + CM_WKUP_I2C0_CLKCTRL) |=
				CM_WKUP_I2C0_CLKCTRL_MODULEMODE_ENABLE;

		/* Waiting for MODULEMODE field to reflect the written value. */
		while(CM_WKUP_I2C0_CLKCTRL_MODULEMODE_ENABLE !=
			  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_I2C0_CLKCTRL) &
					  CM_WKUP_I2C0_CLKCTRL_MODULEMODE));
	    while((CM_WKUP_I2C0_CLKCTRL_IDLEST_FUNC <<
	           CM_WKUP_I2C0_CLKCTRL_IDLEST_SHIFT) !=
	          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_I2C0_CLKCTRL) &
	           CM_WKUP_I2C0_CLKCTRL_IDLEST));
    	break;
    case 1:
		HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) |= CM_PER_I2C1_CLKCTRL_MODULEMODE_ENABLE;

		while((HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) & CM_PER_I2C1_CLKCTRL_MODULEMODE) !=
				CM_PER_I2C1_CLKCTRL_MODULEMODE_ENABLE);
		while((CM_PER_I2C1_CLKCTRL_IDLEST_FUNC<<CM_PER_I2C1_CLKCTRL_IDLEST_SHIFT) !=
				(CM_PER_I2C1_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL)));
    	break;
    case 2:
		HWREG(SOC_CM_PER_REGS + CM_PER_I2C2_CLKCTRL) |= CM_PER_I2C2_CLKCTRL_MODULEMODE_ENABLE;

		while((HWREG(SOC_CM_PER_REGS + CM_PER_I2C2_CLKCTRL) & CM_PER_I2C2_CLKCTRL_MODULEMODE) !=
				CM_PER_I2C2_CLKCTRL_MODULEMODE_ENABLE);
		while((CM_PER_I2C2_CLKCTRL_IDLEST_FUNC<<CM_PER_I2C2_CLKCTRL_IDLEST_SHIFT) !=
				(CM_PER_I2C2_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_I2C2_CLKCTRL)));
    	break;
    }
}
/*#####################################################*/
void TWIModuleClkUnConfig(new_twi* TwiStruct)
{
    switch(TwiStruct->TwiNr)
    {
    case 0:
		/* Writing to MODULEMODE field of CM_WKUP_UART0_CLKCTRL register. */
		HWREG(SOC_CM_WKUP_REGS + CM_WKUP_I2C0_CLKCTRL) |= CM_WKUP_I2C0_CLKCTRL_MODULEMODE_DISABLE;
    	break;
    case 1:
		HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) |= CM_PER_I2C1_CLKCTRL_MODULEMODE_DISABLED;
    	break;
    case 2:
		HWREG(SOC_CM_PER_REGS + CM_PER_I2C2_CLKCTRL) |= CM_PER_I2C2_CLKCTRL_MODULEMODE_DISABLED;
    	break;
    }
}
/*#####################################################*/



