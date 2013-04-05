/*
 * clk_edma.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */

#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"


/*
** This function enables the system L3 clocks.
** This also enables the clocks for EDMA instance.
*/

void _EDMAModuleClkConfig(void)
{
    /* Writing to MODULEMODE field of CM_PER_TPCC_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPCC_CLKCTRL) |=
          CM_PER_TPCC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPCC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPCC_CLKCTRL) &
          CM_PER_TPCC_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_TPTC0_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) |=
          CM_PER_TPTC0_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPTC0_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) &
           CM_PER_TPTC0_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_TPTC1_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) |=
          CM_PER_TPTC1_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPTC1_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) &
           CM_PER_TPTC1_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_TPTC2_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) |=
          CM_PER_TPTC2_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPTC2_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) &
           CM_PER_TPTC2_CLKCTRL_MODULEMODE));

	/*	DMA in non-idle mode	*/
	HWREG(0x49800010) = 0x00000028;
	HWREG(0x49900010) = 0x00000028;
	HWREG(0x49a00010) = 0x00000028;

    /*
    ** Waiting for IDLEST field in CM_PER_TPCC_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPCC_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPCC_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPCC_CLKCTRL) &
            CM_PER_TPCC_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_TPTC0_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC0_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPTC0_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) &
            CM_PER_TPTC0_CLKCTRL_IDLEST));

    /*
    ** Waiting for STBYST field in CM_PER_TPTC0_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC0_CLKCTRL_STBYST_FUNC <<
           CM_PER_TPTC0_CLKCTRL_STBYST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) &
            CM_PER_TPTC0_CLKCTRL_STBYST));

    /*
    ** Waiting for IDLEST field in CM_PER_TPTC1_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC1_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPTC1_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) &
            CM_PER_TPTC1_CLKCTRL_IDLEST));

    /*
    ** Waiting for STBYST field in CM_PER_TPTC1_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC1_CLKCTRL_STBYST_FUNC <<
           CM_PER_TPTC1_CLKCTRL_STBYST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) &
            CM_PER_TPTC1_CLKCTRL_STBYST));

    /*
    ** Waiting for IDLEST field in CM_PER_TPTC2_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC2_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPTC2_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) &
            CM_PER_TPTC2_CLKCTRL_IDLEST));

    /*
    ** Waiting for STBYST field in CM_PER_TPTC2_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC2_CLKCTRL_STBYST_FUNC <<
           CM_PER_TPTC2_CLKCTRL_STBYST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) &
            CM_PER_TPTC2_CLKCTRL_STBYST));
}

