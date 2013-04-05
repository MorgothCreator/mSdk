/*
 * pin_mux_spi.h
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */

#ifndef PIN_MUX_SPI_H_
#define PIN_MUX_SPI_H_

unsigned int pin_mux_spi_sclk(unsigned int PinFunction);
unsigned int pin_mux_spi_d0(unsigned int PinFunction);
unsigned int pin_mux_spi_d1(unsigned int PinFunction);
unsigned int pin_mux_spi_cs0(unsigned int PinFunction);
unsigned int pin_mux_spi_cs1(unsigned int PinFunction);
unsigned int pin_mux_spi_cs2(unsigned int PinFunction);
unsigned int pin_mux_spi_cs3(unsigned int PinFunction);

#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_spi.c"
#endif

#endif /* PIN_MUX_SPI_H_ */
