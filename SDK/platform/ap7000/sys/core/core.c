/*
 * Core.c
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#include "avr32/ap7000.h"
#include "core.h"
#include "ram.h"
#include "delay.h"
#include "driver/intc.h"

#define     AVR32_PM_PBBMASK_reg         (*(volatile unsigned long *)(AVR32_PM_ADDRESS   + 0x014))

void  PLL0Setup (unsigned long CoreFreq, unsigned char div)
{
	unsigned long  pll;
    unsigned long  opt;
    unsigned char mul = CoreFreq/FOSC0;

    pll              =   0;
    pll             |= ((mul - 1) << AVR32_PM_PLL0_PLLMUL_OFFSET)   &
                        AVR32_PM_PLL0_PLLMUL_MASK;
    pll             |= ((div - 1) << AVR32_PM_PLL0_PLLDIV_OFFSET)   &
                        AVR32_PM_PLL0_PLLDIV_MASK;
    pll             |=  (0        << AVR32_PM_PLL0_PLLOSC_OFFSET)   &   /* Select OSC0                                              */
                        AVR32_PM_PLL0_PLLOSC_MASK;
    pll             |=  (16       << AVR32_PM_PLL0_PLLCOUNT_OFFSET) &   /* Slow clk cycles before ISR:LOCK is set, ~500us           */
                        AVR32_PM_PLL0_PLLCOUNT_MASK;

    if (CoreFreq >= 160000000) {
        opt          =  6;
    } else {
        opt          =  4;
    }

    pll             |=  (opt      << AVR32_PM_PLL0_PLLOPT_OFFSET)   &
                        AVR32_PM_PLL0_PLLOPT_MASK;
    pll             &= ~AVR32_PM_PLL0_PLLEN_MASK;                       /* Disable PLL to store the changes                         */

    AVR32_PM.pll0    =  pll;                                            /* Load new PLL settings                                    */

    pll             |=  AVR32_PM_PLL0_PLLEN_MASK;                       /* Enable PLL                                               */
    AVR32_PM.pll0    =  pll;

    while (AVR32_PM.ISR.lock0 == 0) {
        ;                                                               /* Wait until PLL0 is ready                                 */
    }
}



unsigned long  Get_CPU_Clk (void)
{
	unsigned char  mul;
	unsigned char  div;
	unsigned char  sel;
    unsigned long  clk;


    if (AVR32_PM.CKSEL.cpudiv == 0) {                                   /* Clock Division: main clock divided by 2^(CPUSEL + 1)     */
        sel =  0;
    } else {
        sel =  AVR32_PM.CKSEL.cpusel + 1;
    }

    if ((AVR32_PM.mcctrl & AVR32_PM_MCCTRL_PLLSEL_MASK) == 0) {         /* OSC0 is source for the main clock                        */
        clk =  FOSC0 >> sel;
    } else {                                                            /* PLL0 is source for the main clock                        */
        mul =  AVR32_PM.PLL0.pllmul + 1;
        div =  AVR32_PM.PLL0.plldiv + 1;
        clk = (mul * FOSC0 / div) >> sel;
    }

    return (clk);
}

void  GClkSetup (unsigned char pll)
{
	unsigned long  gclk;


    gclk                =  0;

    gclk               |= (0   << AVR32_PM_GCCTRL0_OSCSEL_OFFSET) &     /* Generic Clock source: OSC0/PLL0 or OSC1/PLL1             */
                           AVR32_PM_GCCTRL0_OSCSEL_MASK;
    gclk               |= (pll << AVR32_PM_GCCTRL0_PLLSEL_OFFSET) &     /* PLL Select: OSC (=0) or PLL (=1)                         */
                           AVR32_PM_GCCTRL0_PLLSEL_MASK;
    gclk               |= (0   << AVR32_PM_GCCTRL0_DIVEN_OFFSET)  &     /* Divide Enable                                            */
                           AVR32_PM_GCCTRL0_DIVEN_MASK;
    gclk               |= (0   << AVR32_PM_GCCTRL0_DIV_OFFSET)    &     /* Division factor: 8-bits                                  */
                           AVR32_PM_GCCTRL0_DIV_MASK;

    gclk               &= ~AVR32_PM_GCCTRL0_CEN_MASK;                   /* Disable Generic Clock 0 to store changes                 */
    AVR32_PM.gcctrl[0]  =  gclk;                                        /* Load new Generic Clock 0 settings                        */

    gclk               |=  AVR32_PM_GCCTRL0_CEN_MASK;                   /* Enable Generic Clock 0                                   */
    AVR32_PM.gcctrl[0]  =  gclk;
}


void  ClkSelect (struct Cpu_Freq_Sel *FreqTable)
{
  unsigned int  clksel;


    AVR32_PM.icr    =  AVR32_PM_ICR_CKRDY_MASK;

    clksel          =  0;

    clksel         |= (FreqTable->cpudiv << AVR32_PM_CKSEL_CPUDIV_OFFSET) &        /* CPU Division enable                                      */
                       AVR32_PM_CKSEL_CPUDIV_MASK;
    clksel         |= (FreqTable->cpusel << AVR32_PM_CKSEL_CPUSEL_OFFSET) &        /* CPU Clock select: clk divided by 2^(SEL+1)  SEL[0:2]     */
                       AVR32_PM_CKSEL_CPUSEL_MASK;
    clksel         |= (FreqTable->hsbdiv << AVR32_PM_CKSEL_HSBDIV_OFFSET) &
                       AVR32_PM_CKSEL_HSBDIV_MASK;
    clksel         |= (FreqTable->hsbsel << AVR32_PM_CKSEL_HSBSEL_OFFSET) &
                       AVR32_PM_CKSEL_HSBSEL_MASK;
    clksel         |= (FreqTable->pbadiv << AVR32_PM_CKSEL_PBADIV_OFFSET) &
                       AVR32_PM_CKSEL_PBADIV_MASK;
    clksel         |= (FreqTable->pbasel << AVR32_PM_CKSEL_PBASEL_OFFSET) &
                       AVR32_PM_CKSEL_PBASEL_MASK;
    clksel         |= (FreqTable->pbbdiv << AVR32_PM_CKSEL_PBBDIV_OFFSET) &
                       AVR32_PM_CKSEL_PBBDIV_MASK;
    clksel         |= (FreqTable->pbbsel << AVR32_PM_CKSEL_PBBSEL_OFFSET) &
                       AVR32_PM_CKSEL_PBBSEL_MASK;

    AVR32_PM.cksel  =  clksel;

    while (AVR32_PM.ISR.ckrdy == 0) {
        ;                                                               /* Wait until new clock settings become effective           */
    }
}



void  ClkSwitch (unsigned char clk)
{
	unsigned long  mcctrl;

    mcctrl          =  AVR32_PM.mcctrl;                                 /* Load current value of MCCTRL register                    */
    mcctrl         &= ~AVR32_PM_MCCTRL_PLLSEL_MASK;                     /* Clear MCSEL options                                      */
    mcctrl         |= (clk << AVR32_PM_MCCTRL_PLLSEL_OFFSET) &          /* Select main clock source                                 */
                       AVR32_PM_MCCTRL_PLLSEL_MASK;

    AVR32_PM.mcctrl =  mcctrl;
}

unsigned long  Get_HSB_Clk (void)
{
	unsigned char  mul;
	unsigned char  div;
	unsigned char  sel;
	unsigned long  clk;


    if (AVR32_PM.CKSEL.hsbdiv == 0) {                                   /* Clock Division: main clock divided by 2^(HSBSEL + 1)     */
        sel =  0;
    } else {
        sel =  AVR32_PM.CKSEL.hsbsel + 1;
    }

    if ((AVR32_PM.mcctrl & AVR32_PM_MCCTRL_PLLSEL_MASK) == 0) {         /* OSC0 is source for the main clock                        */
        clk =  FOSC0 >> sel;
    } else {                                                            /* PLL0 is source for the main clock                        */
        mul =  AVR32_PM.PLL0.pllmul + 1;
        div =  AVR32_PM.PLL0.plldiv + 1;
        clk = (mul * FOSC0 / div) >> sel;
    }

    return (clk);
}



unsigned long  Get_PBA_Clk (void)
{
	unsigned char  mul;
	unsigned char  div;
	unsigned char  sel;
	unsigned long  clk;


    if (AVR32_PM.CKSEL.pbadiv == 0) {                                   /* Clock Division: main clock divided by 2^(PBASEL + 1)     */
        sel =  0;
    } else {
        sel =  AVR32_PM.CKSEL.pbasel + 1;
    }

    if ((AVR32_PM.mcctrl & AVR32_PM_MCCTRL_PLLSEL_MASK) == 0) {         /* OSC0 is source for the main clock                        */
        clk       =  FOSC0 >> sel;
    } else {                                                            /* PLL0 is source for the main clock                        */
        mul       =  AVR32_PM.PLL0.pllmul + 1;
        div       =  AVR32_PM.PLL0.plldiv + 1;
        clk       = (mul * FOSC0 / div) >> sel;
    }

    return (clk);
}


unsigned long  Get_PBB_Clk (void)
{
	unsigned char  mul;
	unsigned char  div;
	unsigned char  sel;
	unsigned long  clk;


    if (AVR32_PM.CKSEL.pbbdiv == 0) {                                   /* Clock Division: main clock divided by 2^(PBBSEL + 1)     */
        sel =  0;
    } else {
        sel =  AVR32_PM.CKSEL.pbbsel + 1;
    }

    if ((AVR32_PM.mcctrl & AVR32_PM_MCCTRL_PLLSEL_MASK) == 0) {         /* OSC0 is source for the main clock                        */
        clk       =  FOSC0 >> sel;
    } else {                                                            /* PLL0 is source for the main clock                        */
        mul       =  AVR32_PM.PLL0.pllmul + 1;
        div       =  AVR32_PM.PLL0.plldiv + 1;
        clk       = (mul * FOSC0 / div) >> sel;
    }

    return (clk);
}


void core_init(unsigned long clock_speed)
{
	BSP_SMC_HDR  config;

	//PM_ClkSwitch(CLK_OSC0);

	config.NCSReadSetup    =   0;                                       /* Delay from address valid to assertion of given strobe.   */
	config.NRDSetup        =  13;
	config.NCSWriteSetup   =   0;
	config.NWESetup        =  26;
	config.NCSReadPulse    = 146;                                       /* Pulse length of given strobe.                            */
	config.NRDPulse        = 133;
	config.NCSWritePulse   = 120;
	config.NWEPulse        =  80;
	config.ReadCycle       = 160;                                       /* Total cycle length of given operation.                   */
	config.WriteCycle      = 173;
	config.BusWidth        =   2;                                       /* Bus width in bytes.                                      */
	config.NRDControlled   =   1;                                       /* Data is sampled on rising edge of {0}:NCS {1}:NRD.       */
	config.NWEControlled   =   1;                                       /* Data is driven on falling edge of {0}:NCS {1}:NWR.       */
	config.ByteWrite       =   1;                                       /* Byte {0}:select {1}:write access type.                   */
	config.TDFOptimization =   0;
	config.TDFCycles       =   3;

	SMC_PreConfig((void *)0x00000000, &config, clock_speed);        /* Pre-config SMC CS0 for higher freq (HSB == MAIN-CLK/2).  */
	PLL0Setup(clock_speed, 1);                                            	/* Setup PLL0 to 180MHz 					                */
	GClkSetup(1);                                                			/* Activate Generic clock using PLL0 as source              */
	_Cpu_Freq_Sel FreqTable;
	FreqTable.cpudiv = 0;
	FreqTable.cpusel = 0;
	FreqTable.hsbdiv = 1;
	FreqTable.hsbsel = 0;
	FreqTable.pbadiv = 1;
	FreqTable.pbasel = 1;
	FreqTable.pbbdiv = 1;
	FreqTable.pbbsel = 0;
	ClkSelect(&FreqTable);                           			/* Configure HSB MAIN-CLK/2, PBA MAIN-CLK/4, PBB MAIN-CLK/2 */
	ClkSwitch(CLK_PLL0);
	SDRAMC_Init();
	INTC_init_interrupts();
	//Enable_global_interrupt();
}

//-T"C:\SDK\platform\ap7000\nlao\linker.lds"
