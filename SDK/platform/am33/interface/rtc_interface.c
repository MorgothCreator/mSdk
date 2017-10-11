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
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/soc_AM335x.h"
//#include "include/hw/evmAM335x.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_cm_rtc.h"
#include "../include/hw/hw_types.h"


#include "../include/hw/soc_AM335x.h"
#include "../include/interrupt.h"
//#include "../include/evmAM335x.h"
#include "api/uart_api.h"
#include "../include/rtc.h"
#include "rtc_interface.h"

/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
unsigned int rtcSetFlag = false;
volatile unsigned int rtcSecUpdate = 0;
extern new_uart* DebugCom;
volatile unsigned int __time = 0;
volatile unsigned int cal = 0;
/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define MASK_HOUR                        (0xFF000000u)
#define MASK_MIN                         (0x00FF0000u)
#define MASK_SEC                         (0x0000FF00u)
#define MASK_MERIDIEM                    (0x000000FFu)

#define SHIFT_HOUR                       (24u)
#define SHIFT_MIN                        (16u)
#define SHIFT_SEC                        (8u)

#define MASK_DAY                         (0xFF000000u)
#define MASK_MON                         (0x00FF0000u)
#define MASK_YEAR                        (0x0000FF00u)
#define MASK_DOTW                        (0x000000FFu)

#define SHIFT_DAY                        (24u)
#define SHIFT_MON                        (16u)
#define SHIFT_YEAR                       (8u)

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static void RTCIsr(void);

void _RTCModuleClkConfig(void)
{
    /* Configuring L3 Interface Clocks. */

    /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
          CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

    /* Checking fields for necessary values.  */

    /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
    while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) & CM_PER_L3_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
           CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
    ** attain the desired value.
    */
    while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
    ** register to attain the desired value.
    */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
          CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));


    /* Configuring registers related to Wake-Up region. */

    /* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
          CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_WKUP_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
          CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
          CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));

    /* Verifying if the other bits are set to required settings. */

    /*
    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
    ** to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
           CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK));

    /* Performing Clock configurations of RTC. */

    /* Writing to MODULEMODE field of CM_RTC_RTC_CLKCTRL register. */
    HWREG(SOC_CM_RTC_REGS + CM_RTC_RTC_CLKCTRL) |=
          CM_RTC_RTC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_RTC_RTC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_RTC_CLKCTRL) &
           CM_RTC_RTC_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_RTC_CLKSTCTRL register. */
    HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) |=
          CM_RTC_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_RTC_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) &
           CM_RTC_CLKSTCTRL_CLKTRCTRL));

    /* Checking the RTC registers for desired values being set. */

    /*
    ** Waiting for IDLEST field in CM_RTC_RTC_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_RTC_RTC_CLKCTRL_IDLEST_FUNC <<
           CM_RTC_RTC_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_RTC_REGS + CM_RTC_RTC_CLKCTRL) &
           CM_RTC_RTC_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_RTC_GCLK field in CM_RTC_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_RTC_CLKSTCTRL_CLKACTIVITY_L4_RTC_GCLK !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) &
           CM_RTC_CLKSTCTRL_CLKACTIVITY_L4_RTC_GCLK));

    /*
    ** Waiting for CLKACTIVITY_RTC_32KCLK field in CM_RTC_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_RTC_CLKSTCTRL_CLKACTIVITY_RTC_32KCLK !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) &
           CM_RTC_CLKSTCTRL_CLKACTIVITY_RTC_32KCLK));
}

/*
** This function configures the AINTC to receive RTC interrupts.
*/

//static void RTCAINTCConfigure(void)
//{
    /* Initializing the ARM Interrupt Controller. */
    //IntAINTCInit();

    /* Registering the Interrupt Service Routine(ISR). */
    //IntRegister(SYS_INT_RTCINT, RTCIsr);

    /* Setting the priority for the system interrupt in AINTC. */
    //IntPrioritySet(SYS_INT_RTCINT, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the system interrupt in AINTC. */
    //IntSystemEnable(SYS_INT_RTCINT);
//}
/*******************************************************************************
**                     FUNCTION DEFINITIONS
*******************************************************************************/
/*
** Registers RTC interrupts
*/
void RtcIntRegister(void)
{
	//IntProtectionDisable();
	/* Registering the Interrupt Service Routine(ISR). */
    IntRegister(SYS_INT_RTCINT, RTCIsr);

    /* Setting the priority for the system interrupt in AINTC. */
    IntPrioritySet(SYS_INT_RTCINT, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the system interrupt in AINTC. */
    IntSystemEnable(SYS_INT_RTCINT);

    //IntProtectionEnable();
}

/*
** Enables RTC seconds interrupt
*/
void RtcSecIntEnable(void)
{
    /* Enable interrupts to be generated on every second.*/
    RTCIntTimerEnable(SOC_RTC_0_REGS, RTC_INT_EVERY_SECOND);
}

/*
** Initializes the RTC peripheral
*/
void RtcInit(void)
{
	/* Performing the System Clock configuration for RTC. */
	_RTCModuleClkConfig();

	/* Disabling Write Protection for RTC registers.*/
    RTCWriteProtectDisable(SOC_RTC_0_REGS);

    RtcIntRegister();

    /* Selecting Internal Clock source for RTC. */
    RTC32KClkSourceSelect(SOC_RTC_0_REGS, RTC_INTERNAL_CLK_SRC_SELECT);

    /* Enabling RTC to receive the Clock inputs. */
    RTC32KClkClockControl(SOC_RTC_0_REGS, RTC_32KCLK_ENABLE);

    RTCEnable(SOC_RTC_0_REGS);

    /* Set the 32KHz counter to run. */
    RTCRun(SOC_RTC_0_REGS);

    RtcSecIntEnable();

    /* Run the RTC. The seconds tick from now on.*/
    RTCRun(SOC_RTC_0_REGS);

    /* Disabling Write Protection for RTC registers.*/
    RTCWriteProtectEnable(SOC_RTC_0_REGS);
}

/*
** Sets the Time and Calender in the RTC. This is a blocking call. 
** The _time and date are entered through UART.
*/
void RtcTimeCalSet(void)
{
    //unsigned int _time = 0;
    //unsigned int _cal = 0;
    //unsigned int temp = 0;
 
    /*uart.puts(DebugCom, "\n\rEnter Hours (0 to 23):", -1);
    temp = UARTGetNum(DebugCom);

    while(temp > 23)
    {
        uart.puts(DebugCom, "\n\rValue entered is invalid. Enter value:", -1);
        temp = UARTGetNum(DebugCom);
    }

    _time = (((temp / 10) << 4) << SHIFT_HOUR)
            | ((temp % 10) << SHIFT_HOUR);

    uart.puts(DebugCom, "\n\rEnter Minutes (0 to 59):", -1);
    temp = UARTGetNum(DebugCom);

    while(temp > 59)
    {
        uart.puts(DebugCom, "\n\rValue entered is invalid. Enter value:", -1);
        temp = UARTGetNum(DebugCom);
    }

    _time |= (((temp / 10) << 4) << SHIFT_MIN)
            | ((temp % 10) << SHIFT_MIN);
 
    uart.puts(DebugCom, "\n\rEnter Seconds (0 to 59):", -1);
    temp = UARTGetNum(DebugCom);

    while(temp > 59)
    {
        uart.puts(DebugCom, "\n\rValue entered is invalid. Enter value:", -1);
        temp = UARTGetNum(DebugCom);
    }

    _time |= (((temp / 10) << 4) << SHIFT_SEC)
             | ((temp % 10) << SHIFT_SEC);

    uart.puts(DebugCom, "\n\rEnter Date (1 to 31):", -1);
    temp = UARTGetNum(DebugCom);

    while((temp > 31) || (0 == temp))
    {
        uart.puts(DebugCom, "\n\rValue entered is invalid. Enter value:", -1);
        temp = UARTGetNum(DebugCom);
    }

    _cal = (((temp / 10) << 4) << SHIFT_DAY)
           | ((temp % 10) << SHIFT_DAY);

    uart.puts(DebugCom, "\n\rEnter Month (1 to 12):", -1);
    temp = UARTGetNum(DebugCom);

    while((temp > 12) || (0 == temp))
    {
        uart.puts(DebugCom, "\n\rValue entered is invalid. Enter value:", -1);
        temp = UARTGetNum(DebugCom);
    }

    _cal |= (((temp / 10) << 4) << SHIFT_MON)
            | ((temp % 10) << SHIFT_MON);

    uart.puts(DebugCom, "\n\rEnter Year (0 to 99):", -1);
    temp = UARTGetNum(DebugCom);
    while(temp > 99)
    {
        uart.puts(DebugCom, "\n\rValue entered is invalid. Enter value:", -1);
        temp = UARTGetNum(DebugCom);
    }

    _cal |= (((temp / 10) << 4) << SHIFT_YEAR)
            | ((temp % 10) << SHIFT_YEAR);

    uart.puts(DebugCom, "\n\rEnter Day Of the week (0 for Sunday...6 for Saturday):", -1);
    temp = UARTGetNum(DebugCom);

    while(temp > 6)
    {
        uart.puts(DebugCom, "\n\rValue entered is invalid. Enter value:", -1);
        temp = UARTGetNum(DebugCom);
    }

    _cal |= (((temp / 10) << 4)) | ((temp % 10));
 */
	/* Disabling Write Protection for RTC registers.*/
    //RTCWriteProtectDisable(SOC_RTC_0_REGS);

    /* Set the calendar registers of RTC with received calendar information.*/
    //RTCCalendarSet(SOC_RTC_0_REGS, _cal);

    /* Set the _time registers of RTC with the received _time information.*/
    //RTCTimeSet(SOC_RTC_0_REGS, _time);

    /* Disabling Write Protection for RTC registers.*/
    //RTCWriteProtectEnable(SOC_RTC_0_REGS);
 
    //uart.puts(DebugCom, "\n\rThe Time and Date are set successfully! \n\n\r", -1);

    //rtcSetFlag = true;
}

/*
** Displays the Time and Date on the UART console
*/
void RtcTimeCalDisplay(void)
{
	unsigned int _time = __time;
	unsigned int _cal = cal;
    unsigned int temp;
 
    //uart.puts(DebugCom, "\r", -1);

    uart.puts(DebugCom, "Current Time And Date: ", -1);
 
    temp = (_time & MASK_HOUR) >> SHIFT_HOUR;
    uart.putc(DebugCom, ((temp >> 4) & 0x0F) + 48);
    uart.putc(DebugCom, (temp & 0x0F) + 48);
  
    uart.putc(DebugCom, ':');
 
    temp = (_time & MASK_MIN) >> SHIFT_MIN;
    uart.putc(DebugCom, ((temp >> 4) & 0x0F) + 48);
    uart.putc(DebugCom, (temp & 0x0F) + 48);

    uart.putc(DebugCom, ':');

    temp = (_time & MASK_SEC) >> SHIFT_SEC;
    uart.putc(DebugCom, ((temp >> 4) & 0x0F) + 48);
    uart.putc(DebugCom, (temp & 0x0F) + 48);

    uart.puts(DebugCom, ", ", -1);


    temp = (_cal & MASK_DAY) >> SHIFT_DAY;
    uart.putc(DebugCom, ((temp >> 4) & 0x0F) + 48);
    uart.putc(DebugCom, (temp & 0x0F) + 48);

    uart.putc(DebugCom, '-');

    temp = (_cal & MASK_MON) >> SHIFT_MON;
    uart.putc(DebugCom, ((temp >> 4) & 0x0F) + 48);
    uart.putc(DebugCom, (temp & 0x0F) + 48);

    uart.putc(DebugCom, '-');

    temp = (_cal & MASK_YEAR) >> SHIFT_YEAR;
    uart.putc(DebugCom, ((temp >> 4) & 0x0F) + 48);
    uart.putc(DebugCom, (temp & 0x0F) + 48);

    uart.puts(DebugCom, ", ", -1);

    switch(_cal & MASK_DOTW)
    {
        case 0x00:
             uart.puts(DebugCom, "Sunday", -1);
        break;

        case 0x01:
             uart.puts(DebugCom, "Monday", -1);
        break;

        case 0x02:
             uart.puts(DebugCom, "Tuesday", -1);
        break;

        case 0x03:
             uart.puts(DebugCom, "Wednesday", -1);
        break;

        case 0x04:
             uart.puts(DebugCom, "Thursday", -1);
        break;

        case 0x05:
             uart.puts(DebugCom, "Friday", -1);
        break;

        case 0x06:
             uart.puts(DebugCom, "Saturday", -1);

        default:
        break;

    }
    uart.puts(DebugCom, "\n\r", -1);
}

/*
** Interrupt service routine for RTC
*/
static void RTCIsr(void)
{
//	RTCIntTimerDisable(SOC_RTC_0_REGS);
	//unsigned int CntTimeout = 1000;
	//while(RTCBusyStatusGet(SOC_RTC_0_REGS) == RTC_BUSY || CntTimeout--);
	__time = RTCTimeGet(SOC_RTC_0_REGS);
	//CntTimeout = 1000;
	//while(RTCBusyStatusGet(SOC_RTC_0_REGS) == RTC_BUSY || CntTimeout--);
    cal = RTCCalendarGet(SOC_RTC_0_REGS);
    rtcSecUpdate++;
   // RTCIntTimerEnable(SOC_RTC_0_REGS, RTC_INT_EVERY_SECOND);
}

/******************************** End of file **********************************/



