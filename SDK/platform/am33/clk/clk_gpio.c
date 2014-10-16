/*
 * clk_gpio.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"

void GPIOModuleClkConfig(unsigned int GpioModuleNr)
{
	/* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
	HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
		  CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

	/* Waiting for MODULEMODE field to reflect the written value. */
	while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
		   CM_PER_L3_CLKCTRL_MODULEMODE));

	/* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
	HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
		  CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

	/* Waiting for MODULEMODE field to reflect the written value. */
	while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
		   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

	/* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
	HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
		  CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

	/* Waiting for CLKTRCTRL field to reflect the written value. */
	while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
		   CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

	/* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
	HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
		  CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

	/*Waiting for CLKTRCTRL field to reflect the written value. */
	while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
		   CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

	/* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
	HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
		  CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

	/*Waiting for CLKTRCTRL field to reflect the written value. */
	while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
		   CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

	/* Checking fields for necessary values.  */

	/* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
	while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
		   CM_PER_L3_CLKCTRL_IDLEST));

	/*
	** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
	** desired value.
	*/
	while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
		   CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
		   CM_PER_L3_INSTR_CLKCTRL_IDLEST));

	/*
	** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
	** attain the desired value.
	*/
	while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
		   CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

	/*
	** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
	** register to attain the desired value.
	*/
	while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
		   CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

	/*
	** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
	** to attain the desired value.
	*/
	while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
		  (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
		  CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));


	/* Configuring registers related to Wake-Up region. */

	/* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
	HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
		  CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

	/* Waiting for MODULEMODE field to reflect the written value. */
	while(CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
		   CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));

	/* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
	HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
		  CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

	/*Waiting for CLKTRCTRL field to reflect the written value. */
	while(CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
		   CM_WKUP_CLKSTCTRL_CLKTRCTRL));

	/* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
	HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
		  CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

	/*Waiting for CLKTRCTRL field to reflect the written value. */
	while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
		   CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));

switch (GpioModuleNr)
{
case 0:
	HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) |=
			CM_WKUP_GPIO0_CLKCTRL_MODULEMODE_ENABLE;

	/* Waiting for MODULEMODE field to reflect the written value. */
	while(CM_WKUP_GPIO0_CLKCTRL_MODULEMODE_ENABLE !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) &
				  CM_WKUP_GPIO0_CLKCTRL_MODULEMODE));

	HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) |=
			CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK_FCLK_EN<<CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK_SHIFT;

	while(CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK_FCLK_EN<<CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK_SHIFT !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) &
				  CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK));
	break;
case 1:
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |=
			CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK_FCLK_EN<<CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK_SHIFT;

	while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) & CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK) !=
			CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK_FCLK_EN<<CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK_SHIFT);

	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |= CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

	while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) & CM_PER_GPIO1_CLKCTRL_MODULEMODE) !=
			CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE);
	break;
case 2:
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |=
				CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK_FCLK_EN<<CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK_SHIFT;

	while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) & CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK) !=
				CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK_FCLK_EN<<CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK_SHIFT);

	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |= CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE;

	while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) & CM_PER_GPIO2_CLKCTRL_MODULEMODE) !=
			CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE);
	break;
case 3:
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO3_CLKCTRL) |=
				CM_PER_GPIO3_CLKCTRL_OPTFCLKEN_GPIO_3_GDBCLK_FCLK_EN<<CM_PER_GPIO3_CLKCTRL_OPTFCLKEN_GPIO_3_GDBCLK_SHIFT;

	while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO3_CLKCTRL) & CM_PER_GPIO3_CLKCTRL_OPTFCLKEN_GPIO_3_GDBCLK) !=
				CM_PER_GPIO3_CLKCTRL_OPTFCLKEN_GPIO_3_GDBCLK_FCLK_EN<<CM_PER_GPIO3_CLKCTRL_OPTFCLKEN_GPIO_3_GDBCLK_SHIFT);

	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO3_CLKCTRL) |= CM_PER_GPIO3_CLKCTRL_MODULEMODE_ENABLE;

	while((HWREG(SOC_CM_PER_REGS + CM_PER_GPIO3_CLKCTRL) & CM_PER_GPIO3_CLKCTRL_MODULEMODE) !=
			CM_PER_GPIO3_CLKCTRL_MODULEMODE_ENABLE);
	break;
}


	while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
		   CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
		   CM_WKUP_CONTROL_CLKCTRL_IDLEST));

	/*
	** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
	** register to attain desired value.
	*/
	while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
		   CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));

	/*
	** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
	** desired value.
	*/
	while((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
		   CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
		   CM_WKUP_L4WKUP_CLKCTRL_IDLEST));

	/*
	** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
	** to attain desired value.
	*/
	while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
		   CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));

	/*
	** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
	** register to attain desired value.
	*/
	while(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
		   CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK));

	/*
	** Waiting for CLKACTIVITY_UART0_GFCLK field in CM_WKUP_CLKSTCTRL
	** register to attain desired value.
	*/
	while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_UART0_GFCLK !=
		  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
		   CM_WKUP_CLKSTCTRL_CLKACTIVITY_UART0_GFCLK));

	/*
	** Waiting for IDLEST field in CM_WKUP_UART0_CLKCTRL register to attain
	** desired value.
	*/

switch (GpioModuleNr)
{
case 0:
    while((CM_WKUP_GPIO0_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_GPIO0_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) &
           CM_WKUP_GPIO0_CLKCTRL_IDLEST));
	break;
case 1:
	while((CM_PER_GPIO1_CLKCTRL_IDLEST_FUNC<<CM_PER_GPIO1_CLKCTRL_IDLEST_SHIFT) !=
			(CM_PER_GPIO1_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL)));
	break;
case 2:
	while((CM_PER_GPIO2_CLKCTRL_IDLEST_FUNC<<CM_PER_GPIO2_CLKCTRL_IDLEST_SHIFT) !=
			(CM_PER_GPIO2_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL)));
	break;
case 3:
	while((CM_PER_GPIO3_CLKCTRL_IDLEST_FUNC<<CM_PER_GPIO3_CLKCTRL_IDLEST_SHIFT) !=
			(CM_PER_GPIO3_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_GPIO3_CLKCTRL)));
	break;
}
}



