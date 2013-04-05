/*
 * pin_mux_spi.h
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */

#ifndef PIN_MUX_SPI_H_
#define PIN_MUX_SPI_H_
/**********************************************/
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/hw_types.h"
/**********************************************/
#define Spi0_Sclk_PinMux_A17	1

#define Spi0_D0_PinMux_B17	1

#define Spi0_D1_PinMux_B16	1

#define Spi0_CS0_PinMux_A16	1

#define Spi0_CS1_PinMux_C15	1
/**********************************************/
#define Spi1_Sclk_PinMux_H16	1
#define Spi1_Sclk_PinMux_A13	2
#define Spi1_Sclk_PinMux_C18	3

#define Spi1_D0_PinMux_H17	1
#define Spi1_D0_PinMux_B13	2
#define Spi1_D0_PinMux_E18	3

#define Spi1_D1_PinMux_J15	1
#define Spi1_D1_PinMux_D12	2
#define Spi1_D1_PinMux_E17	3

#define Spi1_CS0_PinMux_E15	1
#define Spi1_CS0_PinMux_E17	2
#define Spi1_CS0_PinMux_H18	3
#define Spi1_CS0_PinMux_D18	4
#define Spi1_CS0_PinMux_C12	5

#define Spi1_CS1_PinMux_E16	1
#define Spi1_CS1_PinMux_D17	2
#define Spi1_CS1_PinMux_C18	3
#define Spi1_CS1_PinMux_A15	4
/**********************************************/
unsigned int pin_mux_spi_sclk_beaglebone(unsigned int SpiNr);
unsigned int pin_mux_spi_d0_beaglebone(unsigned int SpiNr);
unsigned int pin_mux_spi_d1_beaglebone(unsigned int SpiNr);
unsigned int pin_mux_spi_cs0_beaglebone(unsigned int SpiNr);
unsigned int pin_mux_spi_cs1_beaglebone(unsigned int SpiNr);
/**********************************************/
unsigned int pin_mux_spi0_sclk(unsigned int PinNr);
unsigned int pin_mux_spi0_d0(unsigned int PinNr);
unsigned int pin_mux_spi0_d1(unsigned int PinNr);
unsigned int pin_mux_spi0_cs0(unsigned int PinNr);
unsigned int pin_mux_spi0_cs1(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_spi1_sclk(unsigned int PinNr);
unsigned int pin_mux_spi1_d0(unsigned int PinNr);
unsigned int pin_mux_spi1_d1(unsigned int PinNr);
unsigned int pin_mux_spi1_cs0(unsigned int PinNr);
unsigned int pin_mux_spi1_cs1(unsigned int PinNr);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_spi.c"
#endif
/**********************************************/
#endif /* PIN_MUX_SPI_H_ */
