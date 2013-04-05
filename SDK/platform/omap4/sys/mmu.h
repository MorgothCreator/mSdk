/*
 * mmu.h
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#ifndef MMU_H_
#define MMU_H_

#define CORTEXM3_L2MMU_BASE_ADDR 0x55082000
#define DSP_MMU_BASE_ADDR 0x4A066000


void MMUConfigAndEnable();

#ifdef HEADER_INCLUDE_C_FILES
#include "mmu.c"
#endif

#endif /* MMU_H_ */
