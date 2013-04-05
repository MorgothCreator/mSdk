/*
 * core.h
 *
 * Created: 2/8/2013 1:46:29 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef CORE_H_
#define CORE_H_
/*#####################################################*/
#include <stdbool.h>
/*#####################################################*/
//-Wl,--defsym=__heap_start=0x803000,--defsym=__heap_end=0x80ffff
/*#####################################################*/
typedef enum
{
	CoreClk_Source_RC2M = 0,
	CoreClk_Source_RC32M,
	CoreClk_Source_RC32K,
	CoreClk_Source_XOSC,
	CoreClk_Source_PLL,
}CoreClk_Source;

typedef enum
{
	CoreClk_Prescaller_1 = 0,
	CoreClk_Prescaller_2,
	CoreClk_Prescaller_4,
	CoreClk_Prescaller_8,
	CoreClk_Prescaller_16,
	CoreClk_Prescaller_32,
	CoreClk_Prescaller_64,
	CoreClk_Prescaller_128,
	CoreClk_Prescaller_256,
	CoreClk_Prescaller_512,
}CoreClk_Prescaller;
/*#####################################################*/
unsigned long core_clk_set(unsigned long CoreFrequency, unsigned char ClkSource, unsigned short Prescaller, bool ExternalOsc);
void int_lvl_enable(bool LowLevel_En, bool MedLevel_En, bool HighLevel_En);
void int_lvl_disable(bool LowLevel_En, bool MedLevel_En, bool HighLevel_En);
unsigned char Bit_Reverse( unsigned char x );
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "core.c"
#endif
/*#####################################################*/
#endif /* CORE_H_ */
/*#####################################################*/
