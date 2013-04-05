/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief SPI bus example driver.
 *
 * This file defines a usefull set of functions for the SPI interface on
 * AVR32 devices. The driver handles normal polled usage and direct memory
 * access (PDC) usage.
 *
 * - Compiler:           IAR EWAAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a SPI module can be used.
 * - AppNote:            AVR32105 - Master and Slave SPI Driver
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
 *
 * $Revision: 508 $
 * $Date: 2007-05-21 20:06:51 +0200 (Mon, 21 May 2007) $
 *****************************************************************************/

/* Copyright (c) 2006, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _AVR32_SPI_H_
#define _AVR32_SPI_H_

#ifdef __GNUC__
#include <avr32/io.h>
#elif __ICCAVR32__
#include <avr32/ioap7000.h>
#else
#error No known compiler used
#endif


void  BSP_SPI_Init (unsigned char com, unsigned char master, unsigned char fdiv, unsigned char modfdis);
void  BSP_SPI_SelectionMode (unsigned char com, char ps, char pcsdec, unsigned char dly_bcs);
unsigned char  BSP_SPI_ChipSel (unsigned char com, unsigned char cs);
unsigned char  BSP_SPI_ChipSelSetup (unsigned char com, unsigned char reg, unsigned char mode, unsigned long baudrate, unsigned char bits, char active, unsigned char dly_start, unsigned char dly_trans);
void  BSP_SPI_En (unsigned char com);
void  BSP_SPI_Dis (unsigned char com);
unsigned char  BSP_SPI_Wr (unsigned char com, unsigned short data, unsigned char cs, unsigned char lastxfer);
unsigned char  SPI_Write ( unsigned short data, unsigned char cs, unsigned char lastxfer);
unsigned char  BSP_SPI_Rd (unsigned char com, unsigned short *data, unsigned char *cs);
unsigned char spi_readRegisterFullCheck(volatile avr32_spi_t * spi);
unsigned char  BSP_SPI_Read ();

#include "spi.c"

#endif /* #ifndef _AVR32_SPI_H_ */

