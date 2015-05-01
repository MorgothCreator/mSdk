/**
 * \file   pio.c
 *
 * \brief  This file contains functions which performs the platform specific
 *         configurations of GPIO.
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


#include "include/hw/hw_control_AM335x.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_cm_wkup.h"
#include "include/hw/hw_cm_per.h"
#include "include/hw/hw_types.h"
#include "include/gpio_v2.h"


/**
 * \brief  This function does the pin multiplexing for any GPIO Pin.
 *
 * \param  offsetAddr   This is the offset address of the Pad Control Register
 *                      corresponding to the GPIO pin. These addresses are
 *                      offsets with respect to the base address of the
 *                      Control Module.
 * \param  padConfValue This is the value to be written to the Pad Control
 *                      register whose offset address is given by 'offsetAddr'.
 *
 * The 'offsetAddr' and 'padConfValue' can be obtained from macros defined
 * in the file 'include/armv7a/am335x/pin_mux.h'.\n
 *
 * \return  None.
 */
void GpioPinMuxSetup(unsigned int offsetAddr, unsigned int padConfValue)
{
    HWREG(SOC_CONTROL_REGS + offsetAddr) = (padConfValue);
}

/****************************** End of file *********************************/
