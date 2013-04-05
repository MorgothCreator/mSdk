/*
 * clk_config_dmtimer.h
 *
 *  Created on: Aug 18, 2012
 *      Author: XxXx
 */
/**********************************************/
#ifndef CLK_CONFIG_DMTIMER_H_
#define CLK_CONFIG_DMTIMER_H_
/**********************************************/
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_cm_dpll.h"
#include "../include/hw/hw_types.h"
/**********************************************/
#define TimerClkSource_TCLKIN		0u
#define TimerClkSource_CLK_M_OSC	1u
#define TimerClkSource_CLK_32KHZ	2u
/**********************************************/
void DMTimer2ModuleClkConfig(int ClkSource);
void DMTimer3ModuleClkConfig(int ClkSource);
void DMTimer4ModuleClkConfig(int ClkSource);
void DMTimer5ModuleClkConfig(int ClkSource);
void DMTimer6ModuleClkConfig(int ClkSource);
void DMTimer7ModuleClkConfig(int ClkSource);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "clk_dmtimer.c"
#endif
/**********************************************/
#endif /* CLK_CONFIG_DMTIMER_H_ */
