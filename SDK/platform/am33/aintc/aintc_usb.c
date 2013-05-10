/*
 * aintc_usb.c
 *
 *  Created on: Dec 25, 2012
 *      Author: XxXx
 */

#include "aintc_usb.h"
#include "include/interrupt.h"
#include "include/hw/hw_control_AM335x.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_cm_wkup.h"
#include "include/hw/hw_cm_per.h"
#include "include/hw/hw_types.h"
#include "include/hw/hw_usbOtg_AM335x.h"
#include "interface/usblib/include/usblib.h"
#include "interface/usblib/include/usbhhid.h"
#include "interface/usblib/include/usbhost.h"
#include "interface/usblib/include/usbhid.h"
#include "interface/usblib/include/usbhhid.h"
#include "interface/usblib/include/usbhhidmouse.h"
#include "interface/usblib/include/usbhmsc.h"
#include "interface/usblib/include/usbmsc.h"
#include "interface/usblib/include/usblibpriv.h"
#include "interface/usblib/include/usbdevice.h"

#ifdef DMA_MODE
//
// \brief  This function confiugres the AINTC to receive UART interrupts.
//
void CPDMAAINTCConfigure(int usbInstance)
{
	if(usbInstance)
	{
    	IntProtectionDisable();
		/* Registering the Interrupt Service Routine(ISR). */
		IntRegister(SYS_INT_USBSSINT, USB1HostIntHandler);

		/* Setting the priority for the system interrupt in AINTC. */
		IntPrioritySet(SYS_INT_USBSSINT, 1, AINTC_HOSTINT_ROUTE_IRQ);

		/* Enabling the system interrupt in AINTC. */
		IntSystemEnable(SYS_INT_USBSSINT);
        IntProtectionEnable();
	}
	else
	{
    	IntProtectionDisable();
	    /* Registering the Interrupt Service Routine(ISR). */
	    IntRegister(SYS_INT_USBSSINT, USB0HostIntHandler);

	    /* Setting the priority for the system interrupt in AINTC. */
	    IntPrioritySet(SYS_INT_USBSSINT, 1, AINTC_HOSTINT_ROUTE_IRQ);

	    /* Enabling the system interrupt in AINTC. */
	    IntSystemEnable(SYS_INT_USBSSINT);
        IntProtectionEnable();
	}
}
#endif


static void USBAINTCConfigure(int usbInstance)
{
   if(usbInstance)
   {
	   IntProtectionDisable();
	   /* Registering the Interrupt Service Routine(ISR). */
	   IntRegister(SYS_INT_USB1, USB1HostIntHandler);

	   /* Setting the priority for the system interrupt in AINTC. */
	   IntPrioritySet(SYS_INT_USB1, 1, AINTC_HOSTINT_ROUTE_IRQ);

	   /* Enabling the system interrupt in AINTC. */
	   IntSystemEnable(SYS_INT_USB1);
       IntProtectionEnable();
   }
   else
   {
	   IntProtectionDisable();
	   /* Registering the Interrupt Service Routine(ISR). */
	   IntRegister(SYS_INT_USB0, USB0HostIntHandler);

	   /* Setting the priority for the system interrupt in AINTC. */
	   IntPrioritySet(SYS_INT_USB0, 1, AINTC_HOSTINT_ROUTE_IRQ);

	   /* Enabling the system interrupt in AINTC. */
	   IntSystemEnable(SYS_INT_USB0);
       IntProtectionEnable();
   }
}

void USBInterruptEnable(int usbInstance)
{
    /* Configuring AINTC to receive USB interrupts. */
    USBAINTCConfigure(usbInstance);
#ifdef DMA_MODE
	CPDMAAINTCConfigure(usbInstance);
#endif
}

/**
 * \brief  This API  enables the USB Interrupts through subsystem specific wrapper
 *	       registers
 * \param  Base address
 * \return None
 */
void USBEnableInt(unsigned int ulBase)
{
	HWREG(ulBase + USB_0_IRQ_ENABLE_SET_0) = 0xFFFFFFFF;
	HWREG(ulBase + USB_0_IRQ_ENABLE_SET_1) = 0xFFFFFFFF;
#ifdef DMA_MODE
	HWREG(USBSS_BASE + USBSS_IRQ_ENABLE_SET) = 0xFFFFFFFF;
#endif
}

/**
 * \brief  This API  Clear  the USB Interrupts through subsystem specific wrapper
 *	       registers
 * \param  Base address
 * \return None
 */
void USBClearInt(unsigned int ulBase)
{

}

