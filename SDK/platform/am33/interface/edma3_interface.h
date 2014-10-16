/*
 * edma3_interface.h
 *
 *  Created on: Dec 13, 2012
 *      Author: XxXx
 */

#ifndef EDMA3_INTERFACE_H_
#define EDMA3_INTERFACE_H_

#include "mmcsdlib/hs_mmcsdlib.h"
#include "mmcsdlib/mmcsd_proto.h"

void EDMA3Initialize(int SdNr);
void HSMMCSDEdmaInit(int SdNr);

#ifdef HEADER_INCLUDE_C_FILES
#include "edma3_interface.c"
#endif

#endif /* EDMA3_INTERFACE_H_ */
