/*
 * sys/plat_properties.h
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef BOARD_PROPERTIES_H_
#define BOARD_PROPERTIES_H_
/*#####################################################*/
#include "sys/mmu.h"

#define NO_I_TO_A_IN_STD_LIB

#define DEFAULT_FONT g_sFontCmss14b
//#define USE_FONT_CMSS14B
#define USE_ALL_FONTS
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
static volatile unsigned int pageTable[MMU_PAGETABLE_NUM_ENTRY]
            __attribute__ ((aligned(MMU_PAGETABLE_ALIGN_SIZE)));
#endif
/*#####################################################*/
#endif /* BOARD_PROPERTIES_H_ */
/*#####################################################*/
