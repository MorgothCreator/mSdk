/*
 * clk_cpsw.c
 *
 *  Created on: Jan 1, 2013
 *      Author: XxXx
 */

#include "clk_cpsw.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"
#include "sys/plat_properties.h"

#if (defined beaglebone) || (defined evmAM335x) || (defined evmskAM335x)

/**
 * \brief   Enables CPSW clocks
 *
 * \param   None
 *
 * \return  None.
 */
void CPSWClkEnable(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL) =
                      CM_PER_CPGMAC0_CLKCTRL_MODULEMODE_ENABLE;

    while(0 != (HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL)
                & CM_PER_CPGMAC0_CLKCTRL_IDLEST));

    HWREG(SOC_PRCM_REGS + CM_PER_CPSW_CLKSTCTRL) =
                      CM_PER_CPSW_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while(0 == (HWREG(SOC_PRCM_REGS + CM_PER_CPSW_CLKSTCTRL)
                & CM_PER_CPSW_CLKSTCTRL_CLKACTIVITY_CPSW_125MHZ_GCLK));
}

#endif

#if (defined devkit8600)
/**
 * \brief   Enables CPSW clocks
 *
 * \param   None
 *
 * \return  None.
 */
void CPSWClkEnable(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL) =
                      CM_PER_CPGMAC0_CLKCTRL_MODULEMODE_ENABLE;

    while(0 != (HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL)
                & CM_PER_CPGMAC0_CLKCTRL_IDLEST));
}
#endif

