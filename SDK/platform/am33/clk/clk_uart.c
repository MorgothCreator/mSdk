/*
 * clk_uart.c
 *
 *  Created on: Dec 1, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "clk_uart.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"
/*#####################################################*/
/*
** This function enables the system L3 and system L4_WKUP clocks.
** This also enables the clocks for UART0 instance.
*/

void UARTModuleClkConfig(unsigned char instanceNum)
{
    /* Configuring L3 Interface Clocks. */

    	switch (instanceNum)
    	{
    	case 0:
			/* Writing to MODULEMODE field of CM_WKUP_UART0_CLKCTRL register. */
			HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) |=
				  CM_WKUP_UART0_CLKCTRL_MODULEMODE_ENABLE;

			/* Waiting for MODULEMODE field to reflect the written value. */
			while(CM_WKUP_UART0_CLKCTRL_MODULEMODE_ENABLE !=
				  (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) &
				   CM_WKUP_UART0_CLKCTRL_MODULEMODE));
		    /*
		    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
		    ** desired value.
		    */
		    while((CM_WKUP_UART0_CLKCTRL_IDLEST_FUNC <<
		           CM_WKUP_UART0_CLKCTRL_IDLEST_SHIFT) !=
		          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) &
		           CM_WKUP_UART0_CLKCTRL_IDLEST));
			break;
    	case 1:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART1_CLKCTRL) |= CM_PER_UART1_CLKCTRL_MODULEMODE_ENABLE;

    		while((HWREG(SOC_CM_PER_REGS + CM_PER_UART1_CLKCTRL) & CM_PER_UART1_CLKCTRL_MODULEMODE) !=
    				CM_PER_UART1_CLKCTRL_MODULEMODE_ENABLE);
    		    /*
    		    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    		    ** desired value.
    		    */
    			while((CM_PER_UART1_CLKCTRL_IDLEST_FUNC<<CM_PER_UART1_CLKCTRL_IDLEST_SHIFT) !=
    					(CM_PER_UART1_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_UART1_CLKCTRL)));
    		break;
    	case 2:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART2_CLKCTRL) |= CM_PER_UART2_CLKCTRL_MODULEMODE_ENABLE;

    		while((HWREG(SOC_CM_PER_REGS + CM_PER_UART2_CLKCTRL) & CM_PER_UART2_CLKCTRL_MODULEMODE) !=
    				CM_PER_UART2_CLKCTRL_MODULEMODE_ENABLE);
    		    /*
    		    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    		    ** desired value.
    		    */
    			while((CM_PER_UART2_CLKCTRL_IDLEST_FUNC<<CM_PER_UART2_CLKCTRL_IDLEST_SHIFT) !=
    					(CM_PER_UART2_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_UART2_CLKCTRL)));
    		break;
    	case 3:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART3_CLKCTRL) |= CM_PER_UART3_CLKCTRL_MODULEMODE_ENABLE;

    		while((HWREG(SOC_CM_PER_REGS + CM_PER_UART3_CLKCTRL) & CM_PER_UART3_CLKCTRL_MODULEMODE) !=
    				CM_PER_UART3_CLKCTRL_MODULEMODE_ENABLE);
    		    /*
    		    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    		    ** desired value.
    		    */
    			while((CM_PER_UART3_CLKCTRL_IDLEST_FUNC<<CM_PER_UART3_CLKCTRL_IDLEST_SHIFT) !=
    					(CM_PER_UART3_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_UART3_CLKCTRL)));
    		break;
    	case 4:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART4_CLKCTRL) |= CM_PER_UART4_CLKCTRL_MODULEMODE_ENABLE;

    		while((HWREG(SOC_CM_PER_REGS + CM_PER_UART4_CLKCTRL) & CM_PER_UART4_CLKCTRL_MODULEMODE) !=
    				CM_PER_UART4_CLKCTRL_MODULEMODE_ENABLE);
    		    /*
    		    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    		    ** desired value.
    		    */
    			while((CM_PER_UART4_CLKCTRL_IDLEST_FUNC<<CM_PER_UART4_CLKCTRL_IDLEST_SHIFT) !=
    					(CM_PER_UART4_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_UART4_CLKCTRL)));
    		break;
    	case 5:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART5_CLKCTRL) |= CM_PER_UART5_CLKCTRL_MODULEMODE_ENABLE;

    		while((HWREG(SOC_CM_PER_REGS + CM_PER_UART5_CLKCTRL) & CM_PER_UART5_CLKCTRL_MODULEMODE) !=
    				CM_PER_UART5_CLKCTRL_MODULEMODE_ENABLE);
    		    /*
    		    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    		    ** desired value.
    		    */
    			while((CM_PER_UART5_CLKCTRL_IDLEST_FUNC<<CM_PER_UART5_CLKCTRL_IDLEST_SHIFT) !=
    					(CM_PER_UART5_CLKCTRL_IDLEST & HWREG(SOC_CM_PER_REGS + CM_PER_UART5_CLKCTRL)));
    		break;
    	}
}
/*#####################################################*/
/*
** This function enables the system L3 and system L4_WKUP clocks.
** This also enables the clocks for UART0 instance.
*/

void UARTModuleClkUnConfig(unsigned char instanceNum)
{
    /* Configuring L3 Interface Clocks. */

    	switch (instanceNum)
    	{
    	case 0:
			/* Writing to MODULEMODE field of CM_WKUP_UART0_CLKCTRL register. */
			HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) &= ~CM_WKUP_UART0_CLKCTRL_MODULEMODE_DISABLED;
			break;
    	case 1:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART1_CLKCTRL) = CM_PER_UART1_CLKCTRL_MODULEMODE_DISABLED;
    		break;
    	case 2:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART2_CLKCTRL) = CM_PER_UART2_CLKCTRL_MODULEMODE_DISABLED;
    		break;
    	case 3:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART3_CLKCTRL) = CM_PER_UART3_CLKCTRL_MODULEMODE_DISABLED;
    		break;
    	case 4:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART4_CLKCTRL) = CM_PER_UART4_CLKCTRL_MODULEMODE_DISABLE;
    		break;
    	case 5:
    		HWREG(SOC_CM_PER_REGS + CM_PER_UART5_CLKCTRL) = CM_PER_UART5_CLKCTRL_MODULEMODE_DISABLED;
    		break;
    	}
}

/*#####################################################*/



