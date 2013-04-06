/*
 * clk_usart.c
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "clk_uart.h"
#include "include/l4per_cm2.h"

bool UARTModuleClkConfig(unsigned char instanceNum)
{
    switch (instanceNum)
    {
    case 0:
    	L4PER_CM2.CM_L4PER_UART1_CLKCTRL = CM_L4PER_MODULEMODE_EXPLICIT_ENABLE << CM_L4PER_MODULEMODE_gp;
    	while((L4PER_CM2.CM_L4PER_UART1_CLKCTRL & CM_L4PER_MODULEMODE_gm) != (CM_L4PER_MODULEMODE_EXPLICIT_ENABLE << CM_L4PER_MODULEMODE_gp));
		while((L4PER_CM2.CM_L4PER_UART1_CLKCTRL & CM_L4PER_IDLEST_gm) != (CM_L4PER_IDLEST_FULL_FUNCTIONAL << CM_L4PER_IDLEST_gp));
    	return true;
    case 1:
    	L4PER_CM2.CM_L4PER_UART2_CLKCTRL = CM_L4PER_MODULEMODE_EXPLICIT_ENABLE << CM_L4PER_MODULEMODE_gp;
    	while((L4PER_CM2.CM_L4PER_UART2_CLKCTRL & CM_L4PER_MODULEMODE_gm) != (CM_L4PER_MODULEMODE_EXPLICIT_ENABLE << CM_L4PER_MODULEMODE_gp));
		while((L4PER_CM2.CM_L4PER_UART2_CLKCTRL & CM_L4PER_IDLEST_gm) != (CM_L4PER_IDLEST_FULL_FUNCTIONAL << CM_L4PER_IDLEST_gp));
    	return true;
    case 2:
    	L4PER_CM2.CM_L4PER_UART3_CLKCTRL = CM_L4PER_MODULEMODE_EXPLICIT_ENABLE << CM_L4PER_MODULEMODE_gp;
    	while((L4PER_CM2.CM_L4PER_UART3_CLKCTRL & CM_L4PER_MODULEMODE_gm) != (CM_L4PER_MODULEMODE_EXPLICIT_ENABLE << CM_L4PER_MODULEMODE_gp));
		while((L4PER_CM2.CM_L4PER_UART3_CLKCTRL & CM_L4PER_IDLEST_gm) != (CM_L4PER_IDLEST_FULL_FUNCTIONAL << CM_L4PER_IDLEST_gp));
    	return true;
    case 3:
    	L4PER_CM2.CM_L4PER_UART4_CLKCTRL = CM_L4PER_MODULEMODE_AUTOMATIC << CM_L4PER_MODULEMODE_gp;
    	while((L4PER_CM2.CM_L4PER_UART4_CLKCTRL & CM_L4PER_MODULEMODE_gm) != (CM_L4PER_MODULEMODE_EXPLICIT_ENABLE << CM_L4PER_MODULEMODE_gp));
		while((L4PER_CM2.CM_L4PER_UART4_CLKCTRL & CM_L4PER_IDLEST_gm) != (CM_L4PER_IDLEST_FULL_FUNCTIONAL << CM_L4PER_IDLEST_gp));
    	return true;
    }
    return false;
}

bool UARTModuleClkUnConfig(unsigned char instanceNum)
{
    switch (instanceNum)
    {
    case 0:
    	L4PER_CM2.CM_L4PER_UART1_CLKCTRL &= ~CM_L4PER_MODULEMODE_gm;
    	while(L4PER_CM2.CM_L4PER_UART1_CLKCTRL & CM_L4PER_MODULEMODE_gm);
    	return true;
    case 1:
    	L4PER_CM2.CM_L4PER_UART2_CLKCTRL &= ~CM_L4PER_MODULEMODE_gm;
    	while(L4PER_CM2.CM_L4PER_UART2_CLKCTRL & CM_L4PER_MODULEMODE_gm);
    	return true;
    case 2:
    	L4PER_CM2.CM_L4PER_UART3_CLKCTRL &= ~CM_L4PER_MODULEMODE_gm;
    	while(L4PER_CM2.CM_L4PER_UART3_CLKCTRL & CM_L4PER_MODULEMODE_gm);
    	return true;
    case 3:
    	L4PER_CM2.CM_L4PER_UART4_CLKCTRL &= ~CM_L4PER_MODULEMODE_gm;
    	while(L4PER_CM2.CM_L4PER_UART4_CLKCTRL & CM_L4PER_MODULEMODE_gm);
    	return true;
    }
    return false;
}
