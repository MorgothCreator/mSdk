/**
 * \file    demoTimer.c
 *
 * \brief   This file contains Timer related functions.
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
#include <stdbool.h>
#include "api/uart_api.h"
//#include "include/rtc.h"
#include "rtc_interface.h"

/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
unsigned int rtcSetFlag = false;
volatile unsigned int rtcSecUpdate = 0;
extern new_uart* DebugCom;
volatile unsigned int time = 0;
volatile unsigned int cal = 0;

/*******************************************************************************
**                     FUNCTION DEFINITIONS
*******************************************************************************/
/*
** Initializes the RTC peripheral
*/
void RtcInit(void)
{
	
}

/*
** Sets the Time and Calender in the RTC. This is a blocking call. 
** The _time and date are entered through UART.
*/
void RtcTimeCalSet(void)
{
    
}

/*
** Displays the Time and Date on the UART console
*/
void RtcTimeCalDisplay(void)
{
	
}

/*
** Interrupt service routine for RTC
*/
void RTCIsr(void)
{
    rtcSecUpdate++;
}

/******************************** End of file **********************************/



