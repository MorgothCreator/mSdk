/**
 *  \file   dmtimer.h
 *
 *  \brief  DMTimer API prototypes and macros.
 *
 *   This file contains the driver API prototypes and macro definitions.
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

#ifndef _DMTIMER_H_
#define _DMTIMER_H_

#include "hw/hw_dmtimer.h"
#include "hw/hw_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*
** Values that can be passed to DMTimerPreScalerClkEnable as ptv so as to derive
** Pre-Scalar clock from timer clock.
*/
/* Value used to divide timer clock by 2 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_2      ((0 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 4 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_4	    ((1 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 8 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_8	    ((2 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 16 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_16	    ((3 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 32 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_32	    ((4 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 64 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_64	    ((5 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 128 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_128    ((6 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/* Value used to divide timer clock by 256 */
#define DMTIMER_PRESCALER_CLK_DIV_BY_256    ((7 << DMTIMER_TCLR_PTV_SHIFT) | \
                                              DMTIMER_TCLR_PRE)

/******************************************************************************/
/*
** Values that can be passed to DMTimerModeConfigure as timerMode.
*/
/* Value used to enable the timer in one-shot and compare mode */
#define DMTIMER_ONESHOT_CMP_ENABLE          (DMTIMER_TCLR_CE)

/* Value used to enable the timer only in one-shot mode */
#define DMTIMER_ONESHOT_NOCMP_ENABLE        (0x0000000)

/* Value used to enable the timer in auto-reload and compare mode */
#define DMTIMER_AUTORLD_CMP_ENABLE          (DMTIMER_TCLR_AR | DMTIMER_TCLR_CE)

/* Value used to enable the timer only in auto-reload mode */
#define DMTIMER_AUTORLD_NOCMP_ENABLE        (DMTIMER_TCLR_AR)

/******************************************************************************/
/*
** Values that can be passed to DMTimerGPOConfigure as gpoCfg.
*/
/* Value used to drive 0 on PORGPOCFG pin */
#define DMTIMER_GPO_CFG_0		    (DMTIMER_TCLR_GPO_CFG_DRIVE0)

/* Value used to drive 1 on PORGPOCFG pin */
#define DMTIMER_GPO_CFG_1		    (DMTIMER_TCLR_GPO_CFG_DRIVE1 << \
					     DMTIMER_TCLR_GPO_CFG_SHIFT)

/******************************************************************************/
/*
** Values that can be passed to DMTimerIntStatusClear/DMTimerIntRawStatusSet/
** as intFlags. Also these values can be used while checking the status got from 
** DMTimerIntRawStatusGet/DMTimerIntStatusGet.
** Any combination is also followed.
** Example- (DMTIMER_INT_TCAR_IT_FLAG | DMTIMER_INT_OVF_IT_FLAG)
*/
/* Value used for capture event of DMTimer */
#define DMTIMER_INT_TCAR_IT_FLAG             (DMTIMER_IRQSTATUS_RAW_TCAR_IT_FLAG)

/* Value used for overflow event of DMTimer */
#define DMTIMER_INT_OVF_IT_FLAG              (DMTIMER_IRQSTATUS_RAW_OVF_IT_FLAG) 

/* Value used for Match event of DMTimer */
#define DMTIMER_INT_MAT_IT_FLAG              (DMTIMER_IRQSTATUS_RAW_MAT_IT_FLAG)

/******************************************************************************/
/*
** Values that can be passed to DMTimerIntEnable/DMTimerIntDisable as intFlags.
** Also these values can be used while checking the status got from 
** DMTimerIntEnableGet.
** Any combination is also followed.
** Example- (DMTIMER_INT_TCAR_EN_FLAG | DMTIMER_INT_OVF_EN_FLAG)
*/
/* Value used for capture event of DMTimer */
#define DMTIMER_INT_TCAR_EN_FLAG             (DMTIMER_IRQENABLE_SET_TCAR_EN_FLAG)

/* Value used for overflow event of DMTimer */
#define DMTIMER_INT_OVF_EN_FLAG              (DMTIMER_IRQENABLE_SET_OVF_EN_FLAG)

/* Value used for Match event of DMTimer */
#define DMTIMER_INT_MAT_EN_FLAG              (DMTIMER_IRQENABLE_SET_MAT_EN_FLAG)

/******************************************************************************/
/*
** Values that can be passed to DMTimerResetConfigure as rstOption.
*/
/* Value used to enable software reset for DMTimer */
#define DMTIMER_SFT_RESET_ENABLE             (DMTIMER_TSICR_SFT_RESETENABLE)

/* Value used to disable software reset for DMTimer */
#define DMTIMER_SFT_RESET_DISABLE            (DMTIMER_TSICR_SFT)

/******************************************************************************/
/*
** Values that can be used while checking the status received from 
** DMTimerIsResetDone.
*/
/* Value used to check whether reset is done */
#define DMTIMER_IS_RESET_DONE                (DMTIMER_TIOCP_CFG_SOFTRESET_DONE)

/* Value used to check whether reset is ongoing */
#define DMTIMER_IS_RESET_ONGOING             (DMTIMER_TIOCP_CFG_SOFTRESET_ONGOING)

/******************************************************************************/
/*
** Structure to store the DM timer context
*/
typedef struct dmtimerContext{
    unsigned int tldr;
    unsigned int tmar;
    unsigned int irqenableset;
    unsigned int tcrr;
    unsigned int tclr;
}DMTIMERCONTEXT;

/*
** Prototype of the APIs
*/
void DMTimerEnable(unsigned int baseAdd);
void DMTimerDisable(unsigned int baseAdd);
void DMTimerModeConfigure(unsigned int baseAdd, unsigned int timerMode);
void DMTimerPreScalerClkEnable(unsigned int baseAdd, unsigned int ptv);
void DMTimerPreScalerClkDisable(unsigned int baseAdd);
void DMTimerCounterSet(unsigned int baseAdd, unsigned int counter);
unsigned int DMTimerCounterGet(unsigned int baseAdd);
void DMTimerReloadSet(unsigned int baseAdd, unsigned int reload);
unsigned int DMTimerReloadGet(unsigned int baseAdd);
void DMTimerGPOConfigure(unsigned int baseAdd, unsigned int gpoCfg);
void DMTimerCompareSet(unsigned int baseAdd, unsigned int compareVal);
unsigned int DMTimerCompareGet(unsigned int baseAdd);
void DMTimerEmulationModeConfigure(unsigned int baseAdd, unsigned int emuMode);
void DMTimerIntRawStatusSet(unsigned int baseAdd, unsigned int intFlags);
unsigned int DMTimerIntRawStatusGet(unsigned int baseAdd);
unsigned int DMTimerIntStatusGet(unsigned int baseAdd);
void DMTimerIntStatusClear(unsigned int baseAdd, unsigned int intFlags);
void DMTimerIntEnable(unsigned int baseAdd, unsigned int intFlags);
void DMTimerIntDisable(unsigned int baseAdd, unsigned int intFlags);
void DMTimerTriggerSet(unsigned int baseAdd);
unsigned int DMTimerIntEnableGet(unsigned int baseAdd);
void DMTimerResetConfigure(unsigned int baseAdd, unsigned int rstOption);
void DMTimerReset(unsigned int baseAdd);
void DMTimerContextSave(unsigned int baseAdd, DMTIMERCONTEXT *contextPtr);
void DMTimerContextRestore(unsigned int baseAdd, DMTIMERCONTEXT *contextPtr);

#ifdef HEADER_INCLUDE_C_FILES
#include "driver/dmtimer.c"
#endif

#ifdef __cplusplus
}
#endif

#endif

/******************************** END OF FILE *********************************/



