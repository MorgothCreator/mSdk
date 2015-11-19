/* This header file is part of the ATMEL AVR-UC3-SoftwareFramework-1.7.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief JPEG decoder.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ***************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */
#ifndef _JPEG_DECODER_H_
#define _JPEG_DECODER_H_

#include <stdint.h>

int jpeg_lib_init(void);
void jpeg_lib_exit(void);
int jpeg_lib_decode(int _offset);
void *jpeg_lib_decode_ex(int _offset, unsigned short *width, unsigned short *height);

#ifdef HEADER_INCLUDE_C_FILES
#include "jdatasrc.c"
#include "IJG/jcapistd.c"
#include "IJG/jccoefct.c"
#include "IJG/jccolor.c"
#include "IJG/jcdctmgr.c"
#include "IJG/jchuff.c"
#include "IJG/jcinit.c"
#include "IJG/jcmainct.c"
#include "IJG/jcmarker.c"
#include "IJG/jcmaster.c"
#include "IJG/jcomapi.c"
#include "IJG/jcparam.c"
#include "IJG/jcphuff.c"
#include "IJG/jcprepct.c"
#include "IJG/jcsample.c"
#include "IJG/jctrans.c"
#include "IJG/jdapimin.c"
#include "IJG/jdapistd.c"
#include "IJG/jdcoefct.c"
#include "IJG/jdcolor.c"
#include "IJG/jddctmgr.c"
#include "IJG/jdhuff.c"
#include "IJG/jdinput.c"
#include "IJG/jdmainct.c"
#include "IJG/jdmarker.c"
#include "IJG/jdmaster.c"
#include "IJG/jdmerge.c"
#include "IJG/jdphuff.c"
#include "IJG/jdpostct.c"
#include "IJG/jdsample.c"
#include "IJG/jdtrans.c"
#include "IJG/jerror.c"
#include "IJG/jfdctflt.c"
#include "IJG/jfdctfst.c"
#include "IJG/jfdctint.c"
#include "IJG/jidctflt.c"
#include "IJG/jidctfst.c"
#include "IJG/jidctint.c"
#include "IJG/jidctred.c"
#include "IJG/jmemmgr.c"
#include "IJG/jmemnobs.c"
#include "IJG/jquant1.c"
#include "IJG/jquant2.c"
#include "IJG/jutils.c"*/
#endif
#endif // _JPEG_DECODER_H_
