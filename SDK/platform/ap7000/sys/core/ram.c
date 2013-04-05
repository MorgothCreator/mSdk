/*
 * Ram.c
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */
#include "avr32/ap7000.h"
#include "ram.h"
#include "core.h"
#include "delay.h"
//#include <avr32\io.h>


void  SMC_PreConfig (void *paddr, BSP_SMC_HDR *settings, unsigned long hsb_clock)
{
	volatile  unsigned long  read;
	unsigned long  mul;
	unsigned long  setup;
	unsigned long  pulse;
	unsigned long  cycle;
	unsigned long  mode;
	unsigned long  addr;
	unsigned char  cs;


	    addr = (unsigned long)paddr;
	    switch ((addr >> 26) & 0x0f) {
	        case 0x0:                                                       /* Addr = 0x00000000,  64MB, EBI SRAM CS0.                  */
	             cs = 0;
	             break;

	        case 0x1:                                                       /* Addr = 0x04000000,  64MB, EBI SRAM CS4.                  */
	             cs = 4;
	             break;

	        case 0x2:                                                       /* Addr = 0x08000000,  64MB, EBI SRAM CS2.                  */
	             cs = 2;
	             break;

	        case 0x3:                                                       /* Addr = 0x0C000000,  64MB, EBI SRAM CS3.                  */
	             cs = 3;
	             break;

	        case 0x4:                                                       /* Addr = 0x10000000, 256MB, EBI SRAM/SDRAM CS1.            */
	        case 0x5:
	        case 0x6:
	        case 0x7:
	             cs = 1;
	             break;

	        case 0x8:                                                       /* Addr = 0x20000000,  64MB, EBI SRAM CS5.                  */
	             cs = 5;
	             break;

	        default:
	             return;
	    }

	    switch (settings->BusWidth) {
	        case 1:
	             mode = (0 & SMC_DBW_MASK) << SMC_DBW_OFFSET;
	             break;

	        case 2:
	             mode = (1 & SMC_DBW_MASK) << SMC_DBW_OFFSET;
	             break;

	        case 4:
	             mode = (2 & SMC_DBW_MASK) << SMC_DBW_OFFSET;
	             break;

	        default:
	             return;
		}

	    if (hsb_clock == 0) {
	        return;
	    }
	                                                                        /* Cycles = x / T = x * f = ...                             */
	                                                                        /* ... = ((x * 10^9) * ((f * 65536) / 10^9)) / 65536 = ...  */
	                                                                        /* ... = ((x * 10^9) * (((f / 10^4) * 2^16) / 10^5)) / 2^16 */
	    mul   = hsb_clock;
	    mul  /= 10000;
	    mul <<= 16;
	    mul  /= 160000;

	#define ns2cyc(x) ((((x) * mul) + 65535) >> 16)

	    setup = (((ns2cyc(settings->NWESetup)      & SMC_NWE_SETUP_MASK)    << SMC_NWE_SETUP_OFFSET)    |
	             ((ns2cyc(settings->NCSWriteSetup) & SMC_NCS_WR_SETUP_MASK) << SMC_NCS_WR_SETUP_OFFSET) |
	             ((ns2cyc(settings->NRDSetup)      & SMC_NRD_SETUP_MASK)    << SMC_NRD_SETUP_OFFSET)    |
	             ((ns2cyc(settings->NCSReadSetup)  & SMC_NCS_RD_SETUP_MASK) << SMC_NCS_RD_SETUP_OFFSET));
	    pulse = (((ns2cyc(settings->NWEPulse)      & SMC_NWE_PULSE_MASK)    << SMC_NWE_PULSE_OFFSET)    |
	             ((ns2cyc(settings->NCSWritePulse) & SMC_NCS_WR_PULSE_MASK) << SMC_NCS_WR_PULSE_OFFSET) |
	             ((ns2cyc(settings->NRDPulse)      & SMC_NRD_PULSE_MASK)    << SMC_NRD_PULSE_OFFSET)    |
	             ((ns2cyc(settings->NCSReadPulse)  & SMC_NCS_RD_PULSE_MASK) << SMC_NCS_RD_PULSE_OFFSET));
	    cycle = (((ns2cyc(settings->WriteCycle)    & SMC_NWE_CYCLE_MASK)    << SMC_NWE_CYCLE_OFFSET)    |
	             ((ns2cyc(settings->ReadCycle)     & SMC_NRD_CYCLE_MASK)    << SMC_NRD_CYCLE_OFFSET));

	    if (settings->NRDControlled > 0) {
	        mode |= SMC_READ_MODE_MASK << SMC_READ_MODE_OFFSET;
	    }
	    if (settings->NWEControlled > 0) {
	        mode |= SMC_WRITE_MODE_MASK << SMC_WRITE_MODE_OFFSET;
	    }
	    if (settings->ByteWrite > 0) {
	        mode |= SMC_BAT_MASK << SMC_BAT_OFFSET;
	    }
	    if (settings->TDFOptimization > 0) {
	        mode |= SMC_TDF_MODE_MASK << SMC_TDF_MODE_OFFSET;
	    }
	    if (settings->TDFCycles < 16) {
	        mode |= (settings->TDFCycles & SMC_TDF_CYCLES_MASK) << SMC_TDF_CYCLES_OFFSET;
	    }

	    AVR32_PM_HSBMASK_reg |= 1 <<  0;                                    /* Enable EBI HSB clock.                                    */
	    AVR32_PM_PBBMASK_reg |= 1 << 13;                                    /* Enable SMC PBB clock.                                    */

	    AVR32_SMC_MODE_reg(cs)  = mode;
	    AVR32_SMC_CYCLE_reg(cs) = cycle;
	    AVR32_SMC_PULSE_reg(cs) = pulse;
	    AVR32_SMC_SETUP_reg(cs) = setup;

	    read = AVR32_SMC_MODE_reg(cs);
}


void  SMC_Config (void *paddr, BSP_SMC_HDR *settings)
{
	unsigned long  hsb_clock;
    hsb_clock = Get_HSB_Clk();
    SMC_PreConfig(paddr, settings, hsb_clock);
}

void  SDRAMC_RefreshRateUpdate (void)
{
    unsigned long  hsb_freq;
    hsb_freq         =  Get_HSB_Clk();
    hsb_freq        /=  1000;
    AVR32_SDRAMC.tr  = (15625 * hsb_freq) / 1000000;                    /* write SDRAM refresh time for 15.625us                    */
}


void  SDRAMC_Init (void)
{
    volatile  unsigned long  *sdram;
    volatile  unsigned long   read;
    unsigned long   i;

    AVR32_HMATRIX.sfr[AVR32_SDRAMC_HMATRIX_NR] |=  0x0002;              /* Enable SDRAM mode for CS1 in BAMBI mux                   */
    AVR32_HMATRIX.sfr[AVR32_SDRAMC_HMATRIX_NR] |=  0x0100;
                                                                        /* Parameters for MT481C2M16B2TG SDRAM                      */
    AVR32_SDRAMC.cr  = (( 8 -  8) << AVR32_SDRAMC_CR_NC)                /* number of cols  =  256 ( 8 bits)                         */
                     | ((11 - 11) << AVR32_SDRAMC_CR_NR)                /* number of rows  = 2048 (11 bits)                         */
                     | (( 2 -  1) << AVR32_SDRAMC_CR_NB)                /* number of banks =    4 ( 2 bits)                         */
                     | (       2  << AVR32_SDRAMC_CR_CAS)               /* CAS latency     =    3                                   */
                     | (       2  << AVR32_SDRAMC_CR_TWR)               /* write recovery time  = min 2 clk                         */
                     | (       7  << AVR32_SDRAMC_CR_TRC)               /* active to active command period = min 55ns               */
                     | (       2  << AVR32_SDRAMC_CR_TRP)               /* precharge command period        = min 15ns               */
                     | (       2  << AVR32_SDRAMC_CR_TRCD)              /* active to read or write delay   = min 15ns               */
                     | (       5  << AVR32_SDRAMC_CR_TRAS)              /* active to precharge command     = min 42ns               */
                     | (       5  << AVR32_SDRAMC_CR_TXSR)              /* exit self refresh to active command = min 55ns           */
    				 | (	   1  << AVR32_SDRAMC_CR_DBW_OFFSET);

    delay_us(400);                                                       /* Delay while SDRAMC is initialized                        */
    sdram            = (unsigned long *)BSP_EBI_CS1_ADDRESS;               		/* load SDRAM address (0x10000000)                          */

    AVR32_SDRAMC.mr  =  MODE_PRECHARGE;                                 /* issue PRECHARGE ALL command to SDRAM                     */
    read             =  AVR32_SDRAMC.mr;                                /* dummy read                                               */
    sdram[0]         =  0;

    AVR32_SDRAMC.mr  =  MODE_AUTOREFRESH;                               /* issue eight AUTO-REFRESH cycles on SDRAM                 */
    read             =  AVR32_SDRAMC.mr;                                /* dummy read                                               */
    for (i = 0; i < 8; i++) {
        sdram[0]     =  0;
    }

    AVR32_SDRAMC.mr  =  MODE_LOAD_MR;                                   /* issue mode register set cycle to program SDRAM           */
    read             =  AVR32_SDRAMC.mr;                                /* dummy read                                               */
    sdram[0x020]     =  0;

                                                                        /* issue NORMAL command to SDRAM, 3 clk after tMRD          */
    read             =  AVR32_SDRAMC.mr;                                /* dummy read                                               */
    AVR32_SDRAMC.mr  =  MODE_NORMAL;
    read             =  AVR32_SDRAMC.mr;                                /* dummy read                                               */
    sdram[0]         =  0;
    SDRAMC_RefreshRateUpdate();
    for (i = 0; i < (32 * 1024 * 1024) / 4; i++)
    {                       /* erase 32MB of SDRAM in 32-bits access                     */
    	sdram[i]     =  0;
    }
}
