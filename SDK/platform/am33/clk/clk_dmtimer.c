/*
 * clk_config_dmtimer.c
 *
 *  Created on: Aug 18, 2012
 *      Author: XxXx
 */
/**********************************************/
#include "clk_dmtimer.h"
/**********************************************/
void DMTimer2ModuleClkConfig(int ClkSource)
{
    /* Select the clock source for the Timer2 instance. */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER2_CLK) &=
          ~(CM_DPLL_CLKSEL_TIMER2_CLK_CLKSEL);

    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER2_CLK) |=
    		ClkSource;

    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER2_CLK) &
           CM_DPLL_CLKSEL_TIMER2_CLK_CLKSEL) !=
        		   ClkSource);

    HWREG(SOC_CM_PER_REGS + CM_PER_TIMER2_CLKCTRL) |=
                             CM_PER_TIMER2_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER2_CLKCTRL) &
    CM_PER_TIMER2_CLKCTRL_MODULEMODE) != CM_PER_TIMER2_CLKCTRL_MODULEMODE_ENABLE);
}
/**********************************************/
void DMTimer3ModuleClkConfig(int ClkSource)
{
    /* Select the clock source for the Timer3 instance. */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER3_CLK) &=
          ~(CM_DPLL_CLKSEL_TIMER3_CLK_CLKSEL);

    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER3_CLK) |=
    		ClkSource;

    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER3_CLK) &
           CM_DPLL_CLKSEL_TIMER3_CLK_CLKSEL) !=
        		   ClkSource);

    HWREG(SOC_CM_PER_REGS + CM_PER_TIMER3_CLKCTRL) |=
                             CM_PER_TIMER3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER3_CLKCTRL) &
    CM_PER_TIMER3_CLKCTRL_MODULEMODE) != CM_PER_TIMER3_CLKCTRL_MODULEMODE_ENABLE);
}
/**********************************************/
void DMTimer4ModuleClkConfig(int ClkSource)
{
    /* Select the clock source for the Timer4 instance. */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER4_CLK) &=
          ~(CM_DPLL_CLKSEL_TIMER4_CLK_CLKSEL);

    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER4_CLK) |=
    		ClkSource;

    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER4_CLK) &
           CM_DPLL_CLKSEL_TIMER4_CLK_CLKSEL) !=
        		   ClkSource);

    HWREG(SOC_CM_PER_REGS + CM_PER_TIMER4_CLKCTRL) |=
                             CM_PER_TIMER4_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER4_CLKCTRL) &
    CM_PER_TIMER4_CLKCTRL_MODULEMODE) != CM_PER_TIMER4_CLKCTRL_MODULEMODE_ENABLE);
}
/**********************************************/
void DMTimer5ModuleClkConfig(int ClkSource)
{
    /* Select the clock source for the Timer5 instance. */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER5_CLK) &=
          ~(CM_DPLL_CLKSEL_TIMER5_CLK_CLKSEL);

    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER5_CLK) |=
    		ClkSource;

    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER5_CLK) &
           CM_DPLL_CLKSEL_TIMER5_CLK_CLKSEL) !=
        		   ClkSource);

    HWREG(SOC_CM_PER_REGS + CM_PER_TIMER5_CLKCTRL) |=
                             CM_PER_TIMER5_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER5_CLKCTRL) &
    CM_PER_TIMER5_CLKCTRL_MODULEMODE) != CM_PER_TIMER5_CLKCTRL_MODULEMODE_ENABLE);
}
/**********************************************/
void DMTimer6ModuleClkConfig(int ClkSource)
{
    /* Select the clock source for the Timer6 instance. */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER6_CLK) &=
          ~(CM_DPLL_CLKSEL_TIMER6_CLK_CLKSEL);

    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER6_CLK) |=
    		ClkSource;

    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER6_CLK) &
           CM_DPLL_CLKSEL_TIMER6_CLK_CLKSEL) !=
        		   ClkSource);

    HWREG(SOC_CM_PER_REGS + CM_PER_TIMER6_CLKCTRL) |=
                             CM_PER_TIMER6_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER6_CLKCTRL) &
    CM_PER_TIMER6_CLKCTRL_MODULEMODE) != CM_PER_TIMER6_CLKCTRL_MODULEMODE_ENABLE);
}
/**********************************************/
void DMTimer7ModuleClkConfig(int ClkSource)
{
    /* Select the clock source for the Timer7 instance. */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER7_CLK) &=
          ~(CM_DPLL_CLKSEL_TIMER7_CLK_CLKSEL);

    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER7_CLK) |=
    		ClkSource;

    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER7_CLK) &
           CM_DPLL_CLKSEL_TIMER7_CLK_CLKSEL) !=
        		   ClkSource);

    HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) |=
                             CM_PER_TIMER7_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) &
    CM_PER_TIMER7_CLKCTRL_MODULEMODE) != CM_PER_TIMER7_CLKCTRL_MODULEMODE_ENABLE);
}
/**********************************************/


