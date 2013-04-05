/*
 * \file   hs_mmcsdlib.h
 *
 * \brief  MMC/SD Library API prototypes
 *
*/

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __MMCSD_LIB_H__
#define __MMCSD_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcsd_proto.h"
#include "../../include/hs_mmcsd.h"

/* Prototype declarations */
void HSMMCSDBusWidthConfig(mmcsdCtrlInfo *ctrl, unsigned int busWidth);
int HSMMCSDBusFreqConfig(mmcsdCtrlInfo *ctrl, unsigned int busFreq);
unsigned int HSMMCSDCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c);
unsigned int HSMMCSDControllerInit(mmcsdCtrlInfo *ctrl);
unsigned int HSMMCSDCardPresent(mmcsdCtrlInfo *ctrl);
void HSMMCSDIntEnable(mmcsdCtrlInfo *ctrl);
#ifdef __cplusplus
}
#endif

#ifdef HEADER_INCLUDE_C_FILES
#include "hs_mmcsdlib.c"
#endif

#endif

