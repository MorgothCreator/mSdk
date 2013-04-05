/*
 *  lib/api/board_properties.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOARD_PROPERTIES_H_
#define BOARD_PROPERTIES_H_
/*#####################################################*/
#define USE_TPS65910A

#define board_type_devkit8600

#define USE_FFT_TABLES
#define USE_FFT_BUFFERS	double

#define USE_SELECTED_FONTS
#define USE_G_SFONT_CMSS_14B
//#define USE_TWI1_ON_UART1
//#define USE_TWI1_ON_UART2
/*#####################################################*/
#define ASCII_NUM_IDX                      (48u)
#define START_ADDR_DDR                     (0x80000000)
#define START_ADDR_DEV                     (0x44000000)
#define START_ADDR_OCMC                    (0x40300000)
#define NUM_SECTIONS_DDR                   (512)
#define NUM_SECTIONS_DEV                   (960)
#define NUM_SECTIONS_OCMC                  (1)
#define MMU_PAGETABLE_ALIGN_SIZE		   (16384)
/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
/* page tables start must be aligned in 16K boundary */
#ifdef __TMS470__
#pragma DATA_ALIGN(pageTable, 16384);
static volatile unsigned int pageTable[4*1024];
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=16384
static volatile unsigned int pageTable[4*1024];
#else
static volatile unsigned int pageTable[4*1024] __attribute__((aligned(16*1024)));
#endif
/*#####################################################*/
#endif /* BOARD_PROPERTIES_H_ */
/*#####################################################*/
