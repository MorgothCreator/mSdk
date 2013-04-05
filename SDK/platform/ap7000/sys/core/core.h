/*
 * Core.h
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#ifndef CORE_H_
#define CORE_H_

#define     FOSC0                        (20000000)                     /* Frequency: Hz                                            */
#define     FOSC1                        (12000000)                     /* Frequency: Hz                                            */

#define     CLK_OSC0                         0                      	/* Clock Selection: OSC0                                    */
#define     CLK_PLL0                         1                      	/* Clock Selection: PLL0                                    */
#define     CPU_CLK_FREQ()             (15 * FOSC0 / 2)                 /* CPU operating frequency									*/

#define 	S80MHZ						8
#define 	S90MHZ						9
#define 	S100MHZ						10
#define 	S110MHZ						11
#define 	S120MHZ						12
#define 	S130MHZ						13
#define 	S140MHZ						14
#define 	S150MHZ						15
#define 	S160MHZ						16
#define 	S170MHZ						17
#define 	S180MHZ						18
#define 	S190MHZ						19
#define 	S200MHZ						20

#define     AVR32_INTC_ICR_reg            ((volatile unsigned long *)(AVR32_INTC_ADDRESS + 0x200))

typedef  struct Cpu_Freq_Sel{
  unsigned char cpudiv;
  unsigned char cpusel;
  unsigned char hsbdiv;
  unsigned char hsbsel;
  unsigned char pbadiv;
  unsigned char pbasel;
  unsigned char pbbdiv;
  unsigned char pbbsel;
}_Cpu_Freq_Sel;

void  PLL0Setup (unsigned long CoreFreq, unsigned char div);
unsigned long  Get_CPU_Clk (void);
void  GClkSetup (unsigned char pll);
void  ClkSelect (struct Cpu_Freq_Sel *FreqTable);
void  ClkSwitch (unsigned char clk);
unsigned long  Get_HSB_Clk (void);
unsigned long  Get_PBA_Clk (void);
unsigned long  Get_PBB_Clk (void);
void core_init(unsigned long clock_speed);

#include "core.c"

#endif /* CORE_H_ */
