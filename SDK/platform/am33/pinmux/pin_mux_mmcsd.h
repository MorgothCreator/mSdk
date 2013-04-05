/*
 * pin_mux_mmcsd.h
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */

#ifndef PIN_MUX_MMCSD_H_
#define PIN_MUX_MMCSD_H_
/**********************************************/
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/hw_types.h"
/**********************************************/
#define Mmcsd0_Clk_PinMux_G17	0

#define Mmcsd0_Cmd_PinMux_G18	0

#define Mmcsd0_Dat0_PinMux_G16	0

#define Mmcsd0_Dat1_PinMux_G15	0

#define Mmcsd0_Dat2_PinMux_F18	0

#define Mmcsd0_Dat3_PinMux_F17	0

#define Mmcsd0_Dat4_PinMux_L16	0

#define Mmcsd0_Dat5_PinMux_L17	0

#define Mmcsd0_Dat6_PinMux_L18	0

#define Mmcsd0_Dat7_PinMux_K18	0
/**********************************************/
#define Mmcsd1_Clk_PinMux_U9	0
#define Mmcsd1_Clk_PinMux_K17	1
#define Mmcsd1_Clk_PinMux_M18	2

#define Mmcsd1_Cmd_PinMux_V9	0
#define Mmcsd1_Cmd_PinMux_K16	1
#define Mmcsd1_Cmd_PinMux_M17	2

#define Mmcsd1_Dat0_PinMux_U10	0
#define Mmcsd1_Dat0_PinMux_K18	1
#define Mmcsd1_Dat0_PinMux_U7	2

#define Mmcsd1_Dat1_PinMux_T10	0
#define Mmcsd1_Dat1_PinMux_L18	1
#define Mmcsd1_Dat1_PinMux_V7	2

#define Mmcsd1_Dat2_PinMux_T11	0
#define Mmcsd1_Dat2_PinMux_L17	1
#define Mmcsd1_Dat2_PinMux_R8	2

#define Mmcsd1_Dat3_PinMux_U12	0
#define Mmcsd1_Dat3_PinMux_L16	1
#define Mmcsd1_Dat3_PinMux_T8	2

#define Mmcsd1_Dat4_PinMux_T12	0
#define Mmcsd1_Dat4_PinMux_U8	2

#define Mmcsd1_Dat5_PinMux_R12	0
#define Mmcsd1_Dat5_PinMux_V8	2

#define Mmcsd1_Dat6_PinMux_V13	0
#define Mmcsd1_Dat6_PinMux_R9	2

#define Mmcsd1_Dat7_PinMux_U13	0
#define Mmcsd1_Dat7_PinMux_T9	2
/**********************************************/
#define Mmcsd2_Clk_PinMux_L15	0
#define Mmcsd2_Clk_PinMux_M18	1
#define Mmcsd2_Clk_PinMux_V12	2

#define Mmcsd2_Cmd_PinMux_J16	0
#define Mmcsd2_Cmd_PinMux_M17	1
#define Mmcsd2_Cmd_PinMux_T13	2

#define Mmcsd2_Dat0_PinMux_T12	0
#define Mmcsd2_Dat0_PinMux_V14	1
#define Mmcsd2_Dat0_PinMux_J17	2

#define Mmcsd2_Dat1_PinMux_R12	0
#define Mmcsd2_Dat1_PinMux_U14	1
#define Mmcsd2_Dat1_PinMux_J18	2

#define Mmcsd2_Dat2_PinMux_V13	0
#define Mmcsd2_Dat2_PinMux_T14	1
#define Mmcsd2_Dat2_PinMux_K15	2

#define Mmcsd2_Dat3_PinMux_U18	0
#define Mmcsd2_Dat3_PinMux_U13	1
#define Mmcsd2_Dat3_PinMux_H16	2

#define Mmcsd2_Dat4_PinMux_U10	0
#define Mmcsd2_Dat4_PinMux_U15	2

#define Mmcsd2_Dat5_PinMux_T10	0
#define Mmcsd2_Dat5_PinMux_T15	2

#define Mmcsd2_Dat6_PinMux_T11	0
#define Mmcsd2_Dat6_PinMux_V16	2

#define Mmcsd2_Dat7_PinMux_U12	0
#define Mmcsd2_Dat7_PinMux_U16	2
/**********************************************/
void pin_mux_mmcsd_beaglebone(unsigned int MmcsdNr);
/**********************************************/
unsigned int pin_mux_mmcsd0_clk(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_cmd(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat0(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat1(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat2(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat3(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat4(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat5(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat6(unsigned int PinNr);
unsigned int pin_mux_mmcsd0_dat7(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_mmcsd1_clk(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_cmd(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat0(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat1(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat2(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat3(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat4(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat5(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat6(unsigned int PinNr);
unsigned int pin_mux_mmcsd1_dat7(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_mmcsd2_clk(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_cmd(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat0(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat1(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat2(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat3(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat4(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat5(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat6(unsigned int PinNr);
unsigned int pin_mux_mmcsd2_dat7(unsigned int PinNr);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_mmcsd.c"
#endif
/**********************************************/
#endif /* PIN_MUX_MMCSD_H_ */
