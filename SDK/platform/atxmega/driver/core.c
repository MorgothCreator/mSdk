/*
 * core.c
 *
 * Created: 2/8/2013 1:46:40 AM
 *  Author: XxXx
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "core.h"

#ifdef OSC_RC32MCREF_gm
#define OSC_RC32MCREF_bm OSC_RC32MCREF_gm
#endif
//###########################################################################
unsigned long core_clk_set(unsigned long CoreFrequency, unsigned char ClkSource, unsigned short Prescaller, char ExternalOsc)
{
	#ifdef CPU_RAMPX
	CPU_RAMPX = 0;
	#endif
	#ifdef CPU_RAMPY
	CPU_RAMPY = 0;
	#endif
	#ifdef CPU_RAMPZ
	CPU_RAMPZ = 0;
	#endif
	volatile unsigned char Tmp = 0;
	//	unsigned char Tmp2 = 0;
	//OSC_XOSCCTRL = OSC_XOSCSEL1_bm;// | OSC_X32KLPM_bm;
	/*if(ExternalOsc == true)
	{
		OSC_CTRL= OSC_XOSCEN_bm | OSC_XOSCSEL1_bm;
		while(~OSC_STATUS & OSC_XOSCRDY_bm);
	}*/
	switch(ExternalOsc)
	{
		case CoreClk_ExtOscType_ExtClk:
			//OSC_CTRL= OSC_XOSCEN_bm | OSC_XOSCSEL1_bm;
			break
		case CoreClk_ExtOscType_32KHz:
			OSC_CTRL= OSC_XOSCEN_bm | OSC_XOSCSEL_32KHz_gc;
			break
		case CoreClk_ExtOscType_XTAL_256CLK:
			OSC_CTRL= OSC_XOSCEN_bm | OSC_XOSCSEL_XTAL_256CLK_gc;
			break
		case CoreClk_ExtOscType_XTAL_1KCLK:
			OSC_CTRL= OSC_XOSCEN_bm | OSC_XOSCSEL_XTAL_1KCLK_gc;
			break
		case CoreClk_ExtOscType_XTAL_16KCLK:
			OSC_CTRL= OSC_XOSCEN_bm | OSC_XOSCSEL_XTAL_16KCLK_gc;
			break
	}
	while(~OSC_STATUS & OSC_XOSCRDY_bm);
	//Setup DFLL
	//OSC_CTRL = OSC_XOSCEN_bm;
	//Set osc
	switch (ClkSource)
	{
		case CoreClk_Source_PLL:
			if(ExternalOsc == true)
			{
				OSC_DFLLCTRL = OSC_RC2MCREF_bm;
			}
			else OSC_DFLLCTRL = 0;
			DFLLRC2M_CTRL |= DFLL_ENABLE_bm;
			OSC_PLLCTRL = (OSC_PLLSRC_RC2M_gc) | (CoreFrequency/2000000);
			OSC_CTRL|= OSC_PLLEN_bm;
			while(~OSC_STATUS & OSC_PLLRDY_bm);
			break;
		case CoreClk_Source_RC2M:
			if(ExternalOsc == true) OSC_DFLLCTRL = OSC_RC2MCREF_bm;
			else OSC_DFLLCTRL = 0;
			DFLLRC2M_CTRL = DFLL_ENABLE_bm;
			OSC_CTRL|= OSC_RC2MEN_bm;
			while(~OSC_STATUS & OSC_RC2MRDY_bm);
			break;
		case CoreClk_Source_RC32M:
			OSC_DFLLCTRL = OSC_RC32MCREF_bm;
			if(ExternalOsc == true) OSC_DFLLCTRL = OSC_RC32MCREF_bm;
			else OSC_DFLLCTRL = 0;
			DFLLRC32M_CTRL = DFLL_ENABLE_bm;
			OSC_CTRL|= OSC_RC32MEN_bm;
			while(~OSC_STATUS & OSC_RC32MRDY_bm);
			break;
		case CoreClk_Source_RC32K:
			//OSC_DFLLCTRL = OSC_RC32MCREF_bm;
			//DFLLRC32M_CTRL = DFLL_ENABLE_bm;
			OSC_CTRL|= OSC_RC32KEN_bm;
			while(~OSC_STATUS & OSC_RC32KEN_bm);
		break;
	}
	// //Security Signature to modify clock
	switch (Prescaller)
	{
		case 512:
		Tmp = CLK_PSADIV_512_gc;
		break;
		case 256:
		Tmp = CLK_PSADIV_256_gc;
		break;
		case 128:
		Tmp = CLK_PSADIV_128_gc;
		break;
		case 64:
		Tmp = CLK_PSADIV_64_gc;
		break;
		case 32:
		Tmp = CLK_PSADIV_32_gc;
		break;
		case 16:
		Tmp = CLK_PSADIV_16_gc;
		break;
		case 8:
		Tmp = CLK_PSADIV_8_gc;
		break;
		case 4:
		Tmp = CLK_PSADIV_4_gc;
		break;
		case 2:
		Tmp = CLK_PSADIV_2_gc;
		break;
		case 1:
		Tmp = CLK_PSADIV_1_gc;
		break;
	};
	Tmp &= CLK_PSADIV_gm;
	CPU_CCP = CCP_IOREG_gc;
	CLK_PSCTRL = Tmp;
	//Select system clock source PLL
	// //Security Signature to modify clock
	Tmp = CLK_SCLKSEL_RC2M_gc;
	volatile unsigned long ReturnedCoreFreq = 0;
	switch (ClkSource)
	{
		case CoreClk_Source_RC2M:
		Tmp = CLK_SCLKSEL_RC2M_gc;
		ReturnedCoreFreq = 2000000;
		break;
		case CoreClk_Source_RC32M:
		Tmp = CLK_SCLKSEL_RC32M_gc;
		ReturnedCoreFreq = 32000000;
		break;
		case CoreClk_Source_RC32K:
		Tmp = CLK_SCLKSEL_RC32K_gc;
		ReturnedCoreFreq = 32768;
		break;
		case CoreClk_Source_XOSC:
		Tmp = CLK_SCLKSEL_XOSC_gc;
		ReturnedCoreFreq = CoreFrequency;
		break;
		case CoreClk_Source_PLL:
		Tmp = CLK_SCLKSEL_PLL_gc;
		ReturnedCoreFreq = 2000000 * (OSC_PLLCTRL&0x1F);
		break;
	};
	CPU_CCP = CCP_IOREG_gc;
	CLK_CTRL = Tmp;
	if(ClkSource == CoreClk_Source_XOSC || ClkSource == CoreClk_Source_RC32K || ClkSource == CoreClk_Source_RC32M)
	{
		OSC_CTRL &= ~OSC_RC2MEN_bm;
		while(~OSC_STATUS & OSC_RC2MRDY_bm);
	}
	volatile unsigned char PrescallerDivider = 0;
	switch (CLK_PSCTRL & 0xFC)
	{
		case CLK_PSADIV_2_gc:
		PrescallerDivider = 1;
		break;
		case CLK_PSADIV_4_gc:
		PrescallerDivider = 2;
		break;
		case CLK_PSADIV_8_gc:
		PrescallerDivider = 3;
		break;
		case CLK_PSADIV_16_gc:
		PrescallerDivider = 4;
		break;
		case CLK_PSADIV_32_gc:
		PrescallerDivider = 5;
		break;
		case CLK_PSADIV_64_gc:
		PrescallerDivider = 6;
		break;
		case CLK_PSADIV_128_gc:
		PrescallerDivider = 7;
		break;
		case CLK_PSADIV_256_gc:
		PrescallerDivider = 8;
		break;
		case CLK_PSADIV_512_gc:
		PrescallerDivider = 9;
		break;
	};
	ReturnedCoreFreq = ReturnedCoreFreq >> PrescallerDivider;
	return ReturnedCoreFreq;
}
//###########################################################################
void int_lvl_enable(bool LowLevel_En, bool MedLevel_En, bool HighLevel_En)
{
	if (LowLevel_En == true) PMIC_CTRL |= LowLevel_En << PMIC_LOLVLEN_bp;
	if (MedLevel_En == true) PMIC_CTRL |= LowLevel_En << PMIC_MEDLVLEN_bp;
	if (HighLevel_En == true) PMIC_CTRL |= LowLevel_En << PMIC_HILVLEN_bp;
	asm("SEI");
}
//###########################################################################
void int_lvl_disable(bool LowLevel_En, bool MedLevel_En, bool HighLevel_En)
{
	if (LowLevel_En == true) PMIC_CTRL &= ~(LowLevel_En << PMIC_LOLVLEN_bp);
	if (MedLevel_En == true) PMIC_CTRL &= ~(LowLevel_En << PMIC_MEDLVLEN_bp);
	if (HighLevel_En == true) PMIC_CTRL &= ~(LowLevel_En << PMIC_HILVLEN_bp);

	if(LowLevel_En  == true|| MedLevel_En  == true|| HighLevel_En == true) 	asm("CLI");
}
//###########################################################################
// Reverses the order of bits in a byte.
// I.e. MSB is swapped with LSB, etc.
unsigned char Bit_Reverse( unsigned char x )
{
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}
//###########################################################################
