/*
 * clk_mcspi.c
 *
 *  Created on: Dec 12, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"
#include "clk_mcspi.h"
/*#####################################################*/
/**
 * \brief   This function will configure the required clocks for McSPI0 instance.
 *
 * \return  None.
 *
 */
void McSPIModuleClkConfig(unsigned char instance)
{
	switch(instance)
	{
		case 0:
			HWREG(SOC_CM_PER_REGS + CM_PER_SPI0_CLKCTRL) &= ~CM_PER_SPI0_CLKCTRL_MODULEMODE;

			HWREG(SOC_CM_PER_REGS + CM_PER_SPI0_CLKCTRL) |= CM_PER_SPI0_CLKCTRL_MODULEMODE_ENABLE;

			while((HWREG(SOC_CM_PER_REGS + CM_PER_SPI0_CLKCTRL) &
			  CM_PER_SPI0_CLKCTRL_MODULEMODE) != CM_PER_SPI0_CLKCTRL_MODULEMODE_ENABLE);
			break;
		case 1:
			HWREG(SOC_CM_PER_REGS + CM_PER_SPI1_CLKCTRL) &= ~CM_PER_SPI1_CLKCTRL_MODULEMODE;

			HWREG(SOC_CM_PER_REGS + CM_PER_SPI1_CLKCTRL) |= CM_PER_SPI1_CLKCTRL_MODULEMODE_ENABLE;

			while((HWREG(SOC_CM_PER_REGS + CM_PER_SPI1_CLKCTRL) &
			  CM_PER_SPI1_CLKCTRL_MODULEMODE) != CM_PER_SPI1_CLKCTRL_MODULEMODE_ENABLE);
			break;
	}
}
/*#####################################################*/
/**
 * \brief   This function will configure the required clocks for McSPI0 instance.
 *
 * \return  None.
 *
 */
void McSPIModuleClkUnConfig(unsigned char instance)
{
	switch(instance)
	{
		case 0:
			HWREG(SOC_CM_PER_REGS + CM_PER_SPI0_CLKCTRL) = CM_PER_SPI0_CLKCTRL_MODULEMODE_DISABLED;
			break;
		case 1:
			HWREG(SOC_CM_PER_REGS + CM_PER_SPI1_CLKCTRL) = CM_PER_SPI1_CLKCTRL_MODULEMODE_DISABLED;
			break;
	}
}
/*#####################################################*/



