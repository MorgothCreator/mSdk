/*
 * core_init.c
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#include "core_init.h"

//#include "mmu.h"
//#include "cache.h"

#include "api/gpio_def.h"
#include "api/gpio_api.h"
//#include "driver/hlos.h"
#include "driver/pl310.h"
#include "include/hw.h"

#include "driver/smc.h"
#include "driver/cpu.h"
#include "driver/interrupt.h"
#include "driver/cpu.h"
#include "interface/gpio_interface.h"
#include "clk/clk_gpio.h"

void _core_init(void)
{
	//CPUSwitchToPrivilegedMode();
	//omap_smc_rom(ROM_SERVICE_PL310_POR_SVC, PL310_POR);
	//omap_smc_ppa(PPA_SERVICE_PL310_POR, 0, 0x7, 1, PL310_POR);
	/* Enable L2 data prefetch */
	//omap_smc_rom(ROM_SERVICE_PL310_AUXCR_SVC, HWREG(OMAP44XX_PL310_AUX_CTRL) | 0x10000000);

	//CPUSwitchToUserMode();



	//pl310_enable();
	//MMUConfigAndEnable();
	//CacheEnable(CACHE_ICACHE);
	//CacheEnable(CACHE_ALL);

	//L3L4_init();

    /* Enable IRQ for ARM (in CPSR)*/
    //IntMasterIRQEnable();
    IntAINTCInit();
    CPUirqe();
    CPUfiqe();
    //STimer_Init();

	GPIOModuleClkConfig(0);
	GPIOModuleClkConfig(1);
	GPIOModuleClkConfig(2);
	GPIOModuleClkConfig(3);
	GPIOModuleClkConfig(4);
	GPIOModuleClkConfig(5);
}

