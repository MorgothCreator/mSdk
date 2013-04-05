/*
 * clk_usb.c
 *
 *  Created on: Dec 25, 2012
 *      Author: XxXx
 */

#include "clk_usb.h"

#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"

/**
 * \brief   This function enables USB clocks
 *
 * \param   None
 *
 * \return  None.
 *
 */
void USB0ModuleClkConfig(void)
{
	HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKDCOLDO_DPLL_PER) |=
		(CM_WKUP_CM_CLKDCOLDO_DPLL_PER_DPLL_CLKDCOLDO_GATE_CTRL |
					CM_WKUP_CM_CLKDCOLDO_DPLL_PER_ST_DPLL_CLKDCOLDO);


	HWREG(SOC_PRCM_REGS + CM_PER_USB0_CLKCTRL) |=
                             CM_PER_USB0_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_USB0_CLKCTRL) &
      CM_PER_USB0_CLKCTRL_MODULEMODE) != CM_PER_USB0_CLKCTRL_MODULEMODE_ENABLE);


    /*
    ** Waiting for IDLEST field in CM_PER_USB0_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_USB0_CLKCTRL_IDLEST_FUNC <<
           CM_PER_USB0_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_USB0_CLKCTRL) &
           CM_PER_USB0_CLKCTRL_IDLEST));

}
/**
 * \brief  This API  enables the USB Module clock
 *	       registers
 * \param  Base address
 * \return None
 */
void USBModuleClkEnable(unsigned int ulIndex, unsigned int ulBase)
{
	//
	//Disable the module clock
	//
	USB0ModuleClkConfig();
}
 /**
  * \brief  This API Disables the module clock
  *	       registers
  * \param  Base address
  * \return None
  */
void USBModuleClkDisable(unsigned int ulIndex, unsigned int ulBase)
{
 	//
 	//Disables the  module clock
 	//
 	HWREG(SOC_PRCM_REGS + CM_PER_USB0_CLKCTRL) |=
                              CM_PER_USB0_CLKCTRL_MODULEMODE_DISABLE;
}


