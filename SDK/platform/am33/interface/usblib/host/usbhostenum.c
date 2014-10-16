//*****************************************************************************
//
// usbhostenum.c - Device enumeration code for the USB host library.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of AM1808 StarterWare USB Library, modified resused from revision 6288 of the 
// stellaris USB Library.
//
//*****************************************************************************

#include "include/hw/hw_usb.h"
#include "include/hw/hw_types.h"
#include "include/debug.h"
#include "include/interrupt.h"
#include "include/usb.h"
#include "../include/usblib.h"
#include "../include/usblibpriv.h"
#include "../include/usbhost.h"
#include "sys/sysdelay.h"
#include "include/cppi41dma.h"
#include "api/timer_api.h"
#include <string.h>


#ifdef DMA_MODE
unsigned char *rxBuffer;
#endif

timer(UsbPipeWriteTimer[2]);
timer(UsbPipeReadTimer[2]);
timer(UsbControlTransferTimer[2]);

//*****************************************************************************
//
//! \addtogroup usblib_hcd
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// External prototypes.
//
//*****************************************************************************
//extern tUSBMode g_eUSBMode;
static tUSBMode g_eUSBMode = USB_MODE_HOST;

//*****************************************************************************
//
// USB instance Object
//
//*****************************************************************************
extern tUSBInstanceObject g_USBInstance[];

//*****************************************************************************
//
// Internal function prototypes.
//
//*****************************************************************************
static void USBHCDEP0StateTx(unsigned int ulIndex);
static void USBHCDEnumHandler(unsigned int ulIndex);
static void USBHCDClearFeature(unsigned int ulIndex,
                               unsigned int ulDevAddress,
                               unsigned int ulEndpoint,
                               unsigned int ulFeature);
static unsigned int USBHCDTxAbort(unsigned int ulIndex, unsigned int endPoint) ;
static unsigned int USBHCDRxAbort(unsigned int ulIndex,  unsigned int endPoint);
static unsigned int USBHCDRetryConnect(unsigned int ulIndex);
//*****************************************************************************
//
// Automatic power enable.
//
//*****************************************************************************
#define USB_HOST_PWREN_AUTO     0x00000002

//*****************************************************************************
//
// Flags used to signal between the interrupt handler and USBHCDMain().
//
//*****************************************************************************
#define INT_EVENT_VBUS_ERR      0x01
#define INT_EVENT_CONNECT       0x02
#define INT_EVENT_DISCONNECT    0x04
#define INT_EVENT_POWER_FAULT   0x08
#define INT_EVENT_BABBLE_FAULT   0x10

//volatile unsigned int g_ulUSBHIntEvents;
unsigned int g_ulConnectRetry[USB_NUM_INSTANCE];
//*****************************************************************************
//
// Flags used to indicate that a uDMA transfer is pending on a pipe.
//
//*****************************************************************************
#define DMA_PEND_TRANSMIT_FLAG  0x10000
#define DMA_PEND_RECEIVE_FLAG   0x1

//volatile unsigned int g_ulDMAPending = 0;

//*****************************************************************************
//
// This holds the current power configuration that is used when USBHCDInit()
// is called.
//
//*****************************************************************************
//static unsigned int g_ulPowerConfig = USBHCD_VBUS_AUTO_HIGH;

//*****************************************************************************
//
// The states for endpoint 0 during enumeration.
//
//*****************************************************************************
typedef enum
{
    //
    // The USB device is waiting on a request from the host controller on
    // endpoint 0.
    //
    EP0_STATE_IDLE,

    //
    // Setup packet is expecting data IN.
    //
    EP0_STATE_SETUP_IN,

    //
    // Setup packet is sending data OUT.
    //
    EP0_STATE_SETUP_OUT,

    //
    // The USB device is receiving data from the device due to an SETUP IN
    // request.
    //
    EP0_STATE_RX,

    //
    // The USB device has completed the IN or OUT request and is now waiting
    // for the host to acknowledge the end of the IN/OUT transaction.  This
    // is the status phase for a USB control transaction.
    //
    EP0_STATE_STATUS,

    //
    // This state is for when a response only has a status phase and no
    // data phase.
    //
    EP0_STATE_STATUS_IN,

    //
    // This endpoint has signaled a stall condition and is waiting for the
    // stall to be acknowledged by the host controller.
    //
    EP0_STATE_STALL,

    //
    // An error has occurred on endpoint 0.
    //
    EP0_STATE_ERROR
}
tEP0State;

//*****************************************************************************
//
// This structure holds the full state for the device enumeration.
//
//*****************************************************************************
typedef struct
{
    //
    // This is the pointer to the current data being sent out or received
    // on endpoint 0.
    //
    unsigned char *pData;

    //
    // This is the number of bytes that remain to be sent from or received
    // into the g_DeviceState.pEP0Data data buffer.
    //
    volatile unsigned int ulBytesRemaining;

    //
    // The amount of data being sent/received due to a request.
    //
    unsigned int ulDataSize;

    //
    // This is the current device address in use by endpoint 0.
    //
    unsigned int ulDevAddress;

    //
    // The maximum packet size for the device responding to the setup packet.
    //
    unsigned int ulMaxPacketSize;

    //
    // The host controller's state.
    //
    tEP0State eState;
}
tHostState;

//*****************************************************************************
//
// This variable holds the current state of endpoint 0.
//
//*****************************************************************************
static volatile tHostState g_sUSBHEP0State[USB_NUM_INSTANCE] =
{
    {
        0,                          // pData
        0,                          // ulBytesRemaining
        0,                          // ulDataSize
        0,                          // ulDevAddress
        0,                          // ulMaxPacketSize
        EP0_STATE_IDLE              // eState
    }
#if (USB_NUM_INSTANCE == 2)
    ,{
        0,                          // pData
        0,                          // ulBytesRemaining
        0,                          // ulDataSize
        0,                          // ulDevAddress
        0,                          // ulMaxPacketSize
        EP0_STATE_IDLE              // eState
    }
#endif
};

//*****************************************************************************
//
// The global tick counter
//
//*****************************************************************************
//static volatile unsigned int g_ulCurrentTick = 0;

//*****************************************************************************
//
// The current active driver.
//
//*****************************************************************************
//static int g_iUSBHActiveDriver = -1;
//static void *g_pvDriverInstance = 0;

//*****************************************************************************
//
// These definitions are used to manipulate the values returned as allocated
// USB pipes.
//
//*****************************************************************************
#define EP_PIPE_USE_UDMA        0x01000000
#define EP_PIPE_TYPE_ISOC       0x00800000
#define EP_PIPE_TYPE_INTR       0x00400000
#define EP_PIPE_TYPE_BULK       0x00200000
#define EP_PIPE_TYPE_CONTROL    0x00100000
#define EP_PIPE_TYPE_IN         0x00020000
#define EP_PIPE_TYPE_OUT        0x00010000
#define EP_PIPE_IDX_M           0x0000ffff

//*****************************************************************************
//
// This creates a USB pipe handle from an index.
//
//*****************************************************************************
#define OUT_PIPE_HANDLE(ulIndex, ulIdx)  (g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].ulType | ulIdx)
#define IN_PIPE_HANDLE(ulIndex, ulIdx)  (g_sUSBHCD[ulIndex].USBINPipes[ulIdx].ulType | ulIdx)

//*****************************************************************************
//
// Converts from an endpoint specifier to the offset of the endpoint's
// control/status registers.
//
//*****************************************************************************
#define EP_OFFSET(Endpoint)     (Endpoint - 0x10)

//*****************************************************************************
//
// The global to hold all of the state information for a given host controller.
//
//*****************************************************************************
tUSBHCD g_sUSBHCD[USB_NUM_INSTANCE];

//*****************************************************************************
//
// If there is an event driver this function will send out a generic connection
// event USB_EVENT_CONNECTED indicating that an unknown connection event has
// occurred.
//
//*****************************************************************************
static void
SendUnknownConnect(unsigned int ulIndex, unsigned int ulClass, unsigned int ulInstance)
{
     
    if((g_sUSBHCD[ulIndex].iEventDriver != -1) &&
           (g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex]
               .iEventDriver]->pfnIntHandler))
    {
        //
        // Send the generic connected event.
        //
        g_sUSBHCD[ulIndex].EventInfo.ulEvent = USB_EVENT_CONNECTED;

        //
        // Save the class for later incase and application needs it.
        //
        g_sUSBHCD[ulIndex].ulClass = ulClass;
        g_sUSBHCD[ulIndex].EventInfo.ulInstance = ulInstance;

        g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex]
                .iEventDriver]->pfnIntHandler(&g_sUSBHCD[ulIndex].EventInfo);
    }
}

//*****************************************************************************
//
// Internal memory allocation space is two unsigned int values where each
// bit represents a 64 byte block in the FIFO.  This requires 64 bits for
// the 4096 bytes of FIFO available.
//
//*****************************************************************************
static unsigned int g_ulAlloc[2];

//*****************************************************************************
//
// This function handles freeing FIFO memory that has been allocated using the
// FIFOAlloc() function.
//
//*****************************************************************************
static void
FIFOFree(tUSBHCDPipe *pUSBPipe)
{
    unsigned int ulMask;

    //
    // Calculate the mask value to use to clear off the allocated blocks used
    // by the USB pipe specified by pUSBPipe.
    //
    ulMask = (1 << (pUSBPipe->ucFIFOSize - 2)) - 1;
    ulMask = ulMask << pUSBPipe->ucFIFOBitOffset;

    //
    // Determine which 32 bit word to access based on the size.
    //
    if(pUSBPipe->ucFIFOSize > USB_FIFO_SZ_64)
    {
        //
        // If the FIFO size is greater than 64 then use the upper 32 bits.
        //
        g_ulAlloc[1] &= ~ulMask;
    }
    else
    {
        //
        // If the FIFO size is less than or equal to 64 then use the lower
        // 32 bits.
        //
        g_ulAlloc[0] &= ~ulMask;
    }
}

//*****************************************************************************
//
// This function is used to allocate FIFO memory to a given USB pipe.
//
// \param pUSBPipe is the USB pipe that needs FIFO memory allocated.
// \param ulSize is the minimum size in bytes of the FIFO to allocate.
//
// This function will allocate \e ulSize bytes to the USB pipe in the
// \e pUSBPipe parameter.  The function will fill the pUSBPipe structure
// members ucFIFOSize and ucFIFOAddr with values that can be used with the
// USBFIFOConfigSet() API.  This allocation uses a first fit algorithm.
//
// \return This function returns the size of the block allocated.
//
//*****************************************************************************
static unsigned int
FIFOAlloc(tUSBHCDPipe *pUSBPipe, unsigned int ulSize)
{
    unsigned int ulBlocks, ulStart, ulBlockSize;
    unsigned short usFIFOAddr;
    unsigned int ulTemp, ulIndex;

    //
    // Save which 32 bit value to access, the upper is for blocks greater
    // than 64 and the lower is for block 64 or less.
    //
    if(ulSize > 64)
    {
        ulIndex = 1;
    }
    else
    {
        ulIndex = 0;
    }

    //
    // Initial FIFO address is 0.
    //
    usFIFOAddr = 0;

    //
    // Initialize the bit pattern and bit location.
    //
    ulBlocks = 1;
    ulStart = 0;

    //
    // The initial block size is always the minimum size of 64 bytes.
    //
    ulBlockSize = 64;

    //
    // The initial size and offset are 64 and 0.
    //
    pUSBPipe->ucFIFOBitOffset = 0;
    pUSBPipe->ucFIFOSize = 3;

    //
    // Scan through 32 bits looking for a memory block large enough to fill
    // the request.
    //
    while(usFIFOAddr <= 32)
    {
        //
        // If the pattern is zero then it is a possible match.
        //
        if((g_ulAlloc[ulIndex] & ulBlocks) == 0)
        {
            //
            // If the size is large enough then save it and break out of the
            // loop.
            //
            if(ulBlockSize >= ulSize)
            {
                //
                // Mark the memory as allocated.
                //
                g_ulAlloc[ulIndex] |= ulBlocks;

                break;
            }

            //
            // Increment the size of the FIFO block.
            //
            pUSBPipe->ucFIFOSize++;

            //
            // Add in a new bit to the size of the allocation.
            //
            ulBlocks = ulBlocks | (ulBlocks << 1) ;

            //
            // Double the current size.
            //
            ulBlockSize <<= 1;

        }
        else
        {
            //
            // Need to start over looking because the last allocation match
            // failed, so reset the bit offset to the current location and the
            // size to 64 bytes.
            //
            pUSBPipe->ucFIFOBitOffset = usFIFOAddr;
            pUSBPipe->ucFIFOSize = 3;

            //
            // Reset the block size to the minimum (64 bytes).
            //
            ulBlockSize = 64;

            //
            // Store the current starting bit location and set the block mask
            // to this value.
            //
            ulStart = 1 << usFIFOAddr;
            ulBlocks = ulStart;
        }

        //
        // Increase the address of the FIFO offset.
        //
        usFIFOAddr++;
    }

    //
    // If there was no block large enough then fail this call.
    //
    if(usFIFOAddr > 32)
    {
        ulBlockSize = 0;
        pUSBPipe->usFIFOAddr = 0;
        pUSBPipe->ucFIFOBitOffset = 0;
        pUSBPipe->ucFIFOSize = 0;
    }
    else
    {
        //
        // Calculate the offset in the FIFO.
        //
        ulTemp = pUSBPipe->ucFIFOBitOffset * 64;

        //
        // Sizes greater than 64 are allocated in the second half of the FIFO
        // memory space.
        //
        if(ulSize > 64)
        {
            ulTemp += 2048;
        }

        //
        // Convert this to the value that can be set in the USB controller.
        //
        pUSBPipe->usFIFOAddr = (unsigned short)ulTemp;
    }
    return(ulBlockSize);
}

//*****************************************************************************
//
//! This function is used to allocate a USB HCD pipe.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ulEndpointType is the type of endpoint that this pipe will be
//! communicating with.
//! \param ulDevAddr is the device address to use for this endpoint.
//! \param ulSize is the size of the FIFO in bytes.
//! \param pfnCallback is the function that will be called when events occur on
//! this USB Pipe.
//!
//! Since there are a limited number of USB HCD pipes that can be used in the
//! host controller, this function is used to temporarily or permanently
//! acquire one of the endpoints.  Unlike the USBHCDPipeAlloc() function this
//! function allows the caller to specify the size of the FIFO allocated to
//! this endpoint in the \e ulSize parameter.  This function also provides a
//! method to register a callback for status changes on this endpoint.  If no
//! callbacks are desired then the \e pfnCallback function should be set to 0.
//! The callback should be used when using the USBHCDPipeSchedule() function
//! so that the caller is notified when the action is complete.
//!
//! \return This function returns a value indicating which pipe was reserved.
//! If the value is 0 then there were no pipes currently available.  This value
//! should be passed to any USBHCDPipe APIs to indicate which pipe is being
//! accessed.
//
//*****************************************************************************
unsigned int
USBHCDPipeAllocSize(unsigned int ulIndex, unsigned int ulEndpointType,
                    unsigned int ulDevAddr, unsigned int ulSize,
                    tHCDPipeCallback pfnCallback)
{
    int iIdx;
    
    ASSERT(ulIndex == 0);

    //
    // Find a USB pipe that is free.
    //
    for(iIdx = 0; iIdx < MAX_NUM_PIPES; iIdx++)
    {
        //
        // Handle OUT Pipes.
        //
        if(ulEndpointType & EP_PIPE_TYPE_OUT)
        {
            //
            // A zero address indicates free.
            //
            if(g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].ulDevAddr == 0)
            {
 
                //
                // Save the endpoint type and device address and callback
                // function.
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].ulType = ulEndpointType;
                g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].ulDevAddr = ulDevAddr;
                g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].pfnCallback = pfnCallback;
                g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].ulEpMaxPacketSize = ulSize;

                //
                // Initialize the endpoint as idle.
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].eState = PIPE_IDLE;

                //
                // Allocate space in the FIFO for this endpoint.
                //
                if(FIFOAlloc(&g_sUSBHCD[ulIndex].USBOUTPipes[iIdx], ulSize) != 0)
                {
                    //
                    // Configure the FIFO.
                    //
                    USBFIFOConfigSet(g_USBInstance[ulIndex].uiBaseAddr , 
                                        INDEX_TO_USB_EP(iIdx + 1),
                                             g_sUSBHCD[ulIndex].USBOUTPipes[iIdx]
                                             .usFIFOAddr, g_sUSBHCD[ulIndex]
                                                 .USBOUTPipes[iIdx].ucFIFOSize,
                                                     USB_EP_HOST_OUT);
                }

                //
                // Set the function address for this endpoint.
                //
                USBHostAddrSet(g_USBInstance[ulIndex].uiBaseAddr, 
                                INDEX_TO_USB_EP(iIdx + 1), ulDevAddr,
                                       USB_EP_HOST_OUT);

                break;
            }
        }
        //
        // Handle IN Pipes.
        //
        else if(ulEndpointType & EP_PIPE_TYPE_IN)
        {
            //
            // A zero address indicates free.
            //
            if(g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulDevAddr == 0)
            {     
                                  
                //
                // Save the endpoint type and device address and callback
                // function.
                //
                g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulType = ulEndpointType;
                g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulDevAddr = ulDevAddr;
                g_sUSBHCD[ulIndex].USBINPipes[iIdx].pfnCallback = pfnCallback;
                g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulEpMaxPacketSize = ulSize;

                //
                // Allocate space in the FIFO for this endpoint.
                //
                if(FIFOAlloc(&g_sUSBHCD[ulIndex].USBINPipes[iIdx], ulSize) != 0)
                {
                    //
                    // Configure the FIFO.
                    //
                    USBFIFOConfigSet(g_USBInstance[ulIndex].uiBaseAddr , 
                                        INDEX_TO_USB_EP(iIdx + 1),
                                             g_sUSBHCD[ulIndex].USBINPipes[iIdx]
                                                 .usFIFOAddr, g_sUSBHCD[ulIndex]
                                                     .USBINPipes[iIdx].ucFIFOSize,
                                                         USB_EP_HOST_IN);
                }

                //
                // Set the function address for this endpoint.
                //
                USBHostAddrSet(g_USBInstance[ulIndex].uiBaseAddr , 
                                INDEX_TO_USB_EP(iIdx + 1), ulDevAddr,
                                       USB_EP_HOST_IN);

                //
                // Reset the state of the pipe to idle.
                //
                g_sUSBHCD[ulIndex].USBINPipes[iIdx].eState = PIPE_IDLE;

                break;
            }
        }
    }

    //
    // Did not find a free pipe.
    //
    if(iIdx == MAX_NUM_PIPES)
    {
        return(0);
    }

    //
    // Return the pipe index and type that was allocated.
    //
    return(ulEndpointType | iIdx);
}

//*****************************************************************************
//
//! This function is used to allocate a USB HCD pipe.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ulEndpointType is the type of endpoint that this pipe will be
//! communicating with.
//! \param ulDevAddr is the device address to use for this endpoint.
//! \param pfnCallback is the function that will be called when events occur on
//! this USB Pipe.
//!
//! Since there are a limited number of USB HCD pipes that can be used in the
//! host controller, this function is used to temporarily or permanently
//! acquire one of the endpoints.  It also provides a method to register a
//! callback for status changes on this endpoint.  If no callbacks are desired
//! then the \e pfnCallback function should be set to 0.  The callback should
//! be used when using the USBHCDPipeSchedule() function so that the caller is
//! notified when the action is complete.
//!
//! \return This function returns a value indicating which pipe was reserved.
//! If the value is 0 then there were no pipes currently available.  This value
//! should be passed to any USBHCDPipe APIs to indicate which pipe is being
//! accessed.
//
//*****************************************************************************
unsigned int
USBHCDPipeAlloc(unsigned int ulIndex, unsigned int ulEndpointType,
                unsigned int ulDevAddr, tHCDPipeCallback pfnCallback)
{
    //
    // The old API allocated only 64 bytes to each endpoint.
    //
    return(USBHCDPipeAllocSize(ulIndex, ulEndpointType, ulDevAddr, 64,
                               pfnCallback));
}

//*****************************************************************************
//
//! This function is used to configure a USB HCD pipe.
//!
//! This should be called after allocating a USB pipe with a call to
//! USBHCDPipeAlloc().  It is used to set the configuration associated with an
//! endpoint like the max payload and target endpoint.  The \e ulMaxPayload
//! parameter is typically read directly from the devices endpoint descriptor
//! and is expressed in bytes.
//!
//! Setting the \e ulInterval parameter depends on the type of endpoint being
//! configured.  For endpoints that do not need to use the \e ulInterval
//! parameter \e ulInterval should be set to 0.  For Bulk \e ulInterval is a
//! value from 2-16 and will set the NAK timeout value as 2^(\e ulInterval-1)
//! frames.  For interrupt endpoints \e ulInterval is a value from 1-255 and
//! is the count in frames between polling the endpoint.  For isochronous
//! endpoints \e ulInterval ranges from 1-16 and is the polling interval in
//! frames represented as 2^(\e ulInterval-1) frames.
//!
//! \param ulPipe is the allocated endpoint to modify.
//! \param ulMaxPayload is maximum data that can be handled per transaction.
//! \param ulInterval is the polling interval for data transfers expressed in
//! frames.
//! \param ulTargetEndpoint is the target endpoint on the device to communicate
//! with.
//!
//! \return If the call was successful, this function returns zero any other
//! value indicates an error.
//
//*****************************************************************************
unsigned int
USBHCDPipeConfig(unsigned int devIndex, unsigned int ulPipe, 
                       unsigned int ulMaxPayload, unsigned int ulInterval, 
                       unsigned int ulTargetEndpoint)
{
    unsigned int ulFlags;
    unsigned int ulIndex;    

    //
    // Get the index number from the allocated pipe.
    //
    ulIndex = (ulPipe & EP_PIPE_IDX_M);

    //
    // Set the direction.
    //
    if(ulPipe & EP_PIPE_TYPE_OUT)
    {
        //
        // Set the mode for this endpoint.
        //
        if(g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ulType & EP_PIPE_TYPE_BULK)
        {
            ulFlags = USB_EP_MODE_BULK;
        }
        else if(g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ulType & EP_PIPE_TYPE_INTR)
        {
            ulFlags = USB_EP_MODE_INT;
        }
        else if(g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ulType & EP_PIPE_TYPE_ISOC)
        {
            ulFlags = USB_EP_MODE_ISOC;
        }
        else
        {
            ulFlags = USB_EP_MODE_CTRL;
        }

        ulFlags |= USB_EP_HOST_OUT;

        g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ucEPNumber =
            (unsigned char)ulTargetEndpoint;

        //
        // Save the interval and the next tick to trigger a scheduler event.
        //
        g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ulInterval = ulInterval;
        g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ulNextEventTick =
            ulInterval + g_sUSBHCD[devIndex].ulCurrentTick;
       
        //
        // Get pipe speed
        //
        g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ulPipeSpeed = 
        g_sUSBHCD[devIndex].USBDevice[ulIndex].ulDeviceSpeed;

        // 
        // Speed configuration in type0 register 
        //
        switch(g_sUSBHCD[devIndex].USBDevice[ulIndex].ulDeviceSpeed)
        {
        case USB_HIGH_SPEED:
            ulFlags |= USB_EP_SPEED_HIGH;     
            break;

        case USB_FULL_SPEED:
            ulFlags |= USB_EP_SPEED_FULL;
            break;
       
        default:
            ulFlags |= USB_EP_SPEED_LOW;     
            break;
        }
    }
    else
    {
        //
        // Set the mode for this endpoint.
        //
        if(g_sUSBHCD[devIndex].USBINPipes[ulIndex].ulType & EP_PIPE_TYPE_BULK)
        {
            ulFlags = USB_EP_MODE_BULK;
        }
        else if(g_sUSBHCD[devIndex].USBINPipes[ulIndex].ulType & EP_PIPE_TYPE_INTR)
        {
            ulFlags = USB_EP_MODE_INT;
        }
        else if(g_sUSBHCD[devIndex].USBINPipes[ulIndex].ulType & EP_PIPE_TYPE_ISOC)
        {
            ulFlags = USB_EP_MODE_ISOC;
        }
        else
        {
            ulFlags = USB_EP_MODE_CTRL;
        }
        ulFlags |= USB_EP_HOST_IN;

        g_sUSBHCD[devIndex].USBINPipes[ulIndex].ucEPNumber =
            (unsigned char)ulTargetEndpoint;

        //
        // Save the interval and the next tick to trigger a scheduler event.
        //
        g_sUSBHCD[devIndex].USBINPipes[ulIndex].ulInterval = ulInterval;
        g_sUSBHCD[devIndex].USBINPipes[ulIndex].ulNextEventTick =
            ulInterval + g_sUSBHCD[devIndex].ulCurrentTick;
	//
	// Get pipe speed
	//
	g_sUSBHCD[devIndex].USBINPipes[ulIndex].ulPipeSpeed = 
	g_sUSBHCD[devIndex].USBDevice[ulIndex].ulDeviceSpeed;	
	

        // 
        // Speed configuration in type0 register 
        //
        switch(g_sUSBHCD[devIndex].USBDevice[ulIndex].ulDeviceSpeed)
        {
        case USB_HIGH_SPEED:
            ulFlags |= USB_EP_SPEED_HIGH;     
            break;
 
        case USB_FULL_SPEED: 
            ulFlags |= USB_EP_SPEED_FULL;
            break;

        default:
            ulFlags |= USB_EP_SPEED_LOW;     
            break;
        }
   }

    //
    // Configure the endpoint according to the flags determined above.
    //
    USBHostEndpointConfig(g_USBInstance[devIndex].uiBaseAddr ,
                          INDEX_TO_USB_EP((ulPipe & EP_PIPE_IDX_M) + 1),
                          ulMaxPayload, ulInterval, ulTargetEndpoint,
                          ulFlags);

    return(0);
}

//*****************************************************************************
//
//! This function is used to return the current status of a USB HCD pipe.
//!
//! This function will return the current status for a given USB pipe.  If
//! there is no status to report this call will simply return
//! \b USBHCD_PIPE_NO_CHANGE.
//!
//! \param ulPipe is the USB pipe for this status request.
//!
//! \return This function returns the current status for the given endpoint.
//! This will be one of the \b USBHCD_PIPE_* values.
//
//*****************************************************************************
unsigned int
USBHCDPipeStatus(unsigned int ulPipe)
{
    return(0);
}

//*****************************************************************************
//
//! This function is used to write data to a USB HCD pipe.
//!
//! \param ulPipe is the USB pipe to put data into.
//! \param pucData is a pointer to the data to send.
//! \param ulSize is the amount of data to send.
//!
//! This function will block until it has sent as much data as was
//! requested using the USB pipe's FIFO.  The caller should have registered a
//! callback with the USBHCDPipeAlloc() call in order to be informed when the
//! data has been transmitted.  The value returned by this function can be less
//! than the \e ulSize requested if the USB pipe has less space available than
//! this request is making.
//!
//! \return This function returns the number of bytes that were scheduled to
//! be sent on the given USB pipe.
//
//*****************************************************************************
unsigned int
USBHCDPipeWrite(unsigned int ulIndex, unsigned int ulPipe, 
                      unsigned char *pucData, unsigned int ulSize)
{
    unsigned int ulEndpoint;
    unsigned int ulRemainingBytes;
    unsigned int ulByteToSend;
    unsigned int ulPipeIdx;
    unsigned int ulEPStatus;
    unsigned int ulConnectRetry;
#ifdef DMA_MODE
    unsigned int txBuffer;
    unsigned int nBlocks;
#endif
    unsigned int ulTimer = 0;

    //
    // Determine which endpoint interface that this pipe is using.
    //
    ulEndpoint = INDEX_TO_USB_EP((EP_PIPE_IDX_M & ulPipe) + 1);

    //
    // Get index used for looking up pipe data
    //
    ulPipeIdx = ulPipe & EP_PIPE_IDX_M;

    //
    // Set the total number of bytes to send out.
    //
    ulRemainingBytes = ulSize;

    //
    // Send all of the requested data.
    //
    while(ulRemainingBytes != 0)
    {
        //
        // Start a write request.
        //
        g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState = PIPE_WRITING;
        
        //
        //Calculate the Number of blocks to Transmit
        //
        if(ulRemainingBytes <=
              (g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].ulEpMaxPacketSize))
        {
            ulByteToSend = ulRemainingBytes;
        }
        else
        {
            ulByteToSend =
           g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].ulEpMaxPacketSize;
        }

#ifdef DMA_MODE
        nBlocks = 1;

        //
        //Allocate enough buffer
        //
        txBuffer=(unsigned int)cppiDmaAllocnBuffer(nBlocks);
        ASSERT(NULL!=txBuffer);
        
        //
        // Set pending transmit DMA flag
        //
        g_sUSBHCD[ulIndex].ulDMAPending |= DMA_PEND_TRANSMIT_FLAG << ulPipeIdx;

        //
        //Copy the data to the TX buffer
        //
        memcpy((unsigned char *)txBuffer, pucData, ulByteToSend); 
        //
        //Load the DMA queue with the data buffer
        //
        doDmaTxTransfer(ulIndex, (unsigned char *)txBuffer, 
            ulByteToSend, ulEndpoint);
        //
        //Enable the DMA for TX operation
        //
        enableCoreTxDMA(ulIndex, ulEndpoint);     

#else

        //
        // Put the data in the buffer.
        //
        USBEndpointDataPut(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint, 
                            pucData, ulByteToSend);

        //
        // Schedule the data to be sent.
        //
        USBEndpointDataSend(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint, 
                            USB_TRANS_OUT);
#endif
       
        //
        // Wait for a status change.
        //
        if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
        {
           ulTimer = g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0;
           //StartTimer(ulTimer);
           timer_interval(&UsbPipeWriteTimer[ulIndex], ulTimer);
           timer_enable(&UsbPipeWriteTimer[ulIndex]);
        }
        while(g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState == PIPE_WRITING)
        {
            //
            // Exit the loop for any of the event(s): Disconnect.
            //
            if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_DISCONNECT)
            {
                //
                // Update the Pipe status
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState = PIPE_ERROR;
                continue;
            }
            else if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_BABBLE_FAULT)
            {
                //
                // Update the Pipe status
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState = PIPE_ERROR;
                continue;
            }

            //
            // Read endpoint status.
            //
            ulEPStatus = USBEndpointStatus(g_USBInstance[ulIndex].uiBaseAddr,
                                               ulEndpoint);

            //
            // Check for stall condition occurence.
            //
            if(ulEPStatus & USB_HOST_OUT_STALL)
            {
                g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState = PIPE_STALLED;
                continue;
            }
            else if(ulEPStatus & USB_HOST_OUT_ERROR)
            {
                g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState = PIPE_ERROR;
                continue;
            }
            if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
            {
                if(timer_tick(&UsbPipeWriteTimer[ulIndex])/*IsTimerElapsed()*/)
                {
                    ulTimer = 0;
                    g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState = PIPE_ERROR;
                    g_sUSBHCD[ulIndex].USBHTimeOut.Status.slNonEP0 = (1<<(ulPipeIdx + 1));
                    break;
                }
            }
        }
        if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
        {
            //StopTimer();
        	timer_disable(&UsbPipeWriteTimer[ulIndex]);
            ulTimer = 0;	
        }

        //
        // If the data was successfully sent then decrement the count and
        // continue.
        //
        if(g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState == PIPE_DATA_SENT)
        {

#ifdef DMA_MODE
            disableCoreTxDMA(g_USBInstance[ulIndex].uiUSBInstance, ulEndpoint);
             
            //
            //Free the TX buffer
            //
            cppiDmaFreenBuffer((unsigned int *)txBuffer);
#endif

            //
            // Decrement the remaining data and advance the pointer.
            //
            ulRemainingBytes -= ulByteToSend;
            pucData += ulByteToSend;
        }
        else if(g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState == PIPE_STALLED)
        {
            //
            // Zero out the size so that the caller knows that no data was
            // written.
            //
            ulSize = 0;
            USBHCDTxAbort(ulIndex, ulEndpoint);

            //
            // This is the actual endpoint number.
            //
            USBHCDClearFeature(ulIndex, 1, ulPipe, USB_FEATURE_EP_HALT);

            //
            // If there was a stall, then no more data is coming so break out.
            //
            break;
        }
        else if(g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState == PIPE_ERROR)
        {
            // Clean-up the EP FIFOs and CPPIDMA (if applicable)
            USBHCDTxAbort(ulIndex, ulEndpoint);

            if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
            {
                if(!ulTimer)
                {
                    // Try re-establising the connection with device
                    ulConnectRetry = g_sUSBHCD[ulIndex].ulConnectRetry;
                    for(;ulConnectRetry;--ulConnectRetry)
                    {
                       if(USBHCDRetryConnect(ulIndex))
                       {
                          break;
                       }
                    }
                    // Abort the transfer if re-establising the connection with device failed.
                    if(!ulConnectRetry)
                    {
                       //
                       // Set the size to 0 to indicate Error.
                       //
                       ulSize = 0;
                       break;
                    }
                }
            }
            else
            {
               //
               // Set the size to 0 to indicate Error.
               //
               ulSize = 0;
               break;
            }
        }

        //
        // If there are less than 64 bytes to send then this is the last
        // of the data to go out.
        //
        if(ulRemainingBytes < 64)
        {
            ulByteToSend = ulRemainingBytes;
        }
    }

    //
    // Go Idle once this state has been reached.
    //
    if(!ulRemainingBytes)
    {
        g_sUSBHCD[ulIndex].USBOUTPipes[ulPipeIdx].eState = PIPE_IDLE;
    }

    return(ulSize);
}

//*****************************************************************************
//
//! This function is used to schedule and IN transaction on a USB HCD pipe.
//!
//! \param ulPipe is the USB pipe to read data from.
//! \param pucData is a pointer to store the data that is received.
//! \param ulSize is the size in bytes of the buffer pointed to by pucData.
//!
//! This function will not block depending on the type of pipe passed in will
//! schedule either a send of data to the device or a read of data from the
//! device.  In either case the amount of data will be limited to what will
//! fit in the FIFO for a given endpoint.
//!
//! \return This function returns the number of bytes that sent in the case
//! of a transfer of data or it will return 0 for a request on a USB IN pipe.
//
//*****************************************************************************
unsigned int
USBHCDPipeSchedule(unsigned int ulIndex, unsigned int ulPipe, 
                          unsigned char *pucData, unsigned int ulSize)
{
    unsigned int ulEndpoint;

    //
    // Determine which endpoint interface that this pipe is using.
    //
    ulEndpoint = INDEX_TO_USB_EP((EP_PIPE_IDX_M & ulPipe) + 1);

    if(ulPipe & EP_PIPE_TYPE_OUT)
    {
        //
        // Start a write request.
        //
        g_sUSBHCD[ulIndex].USBOUTPipes[EP_PIPE_IDX_M & ulPipe].eState = PIPE_WRITING;

        //
        // Put the data in the buffer.
        //
        USBEndpointDataPut(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint, pucData, ulSize);

        //
        // Schedule the data to be sent.
        //
        USBEndpointDataSend(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint, USB_TRANS_OUT); 

    }
    else
    {
        //
        // Start a read request.
        //
        g_sUSBHCD[ulIndex].USBINPipes[EP_PIPE_IDX_M & ulPipe].eState = PIPE_READING;
       
        //
        // Trigger a request for data from the device.
        //
        USBHostRequestIN(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint);

        //
        // No data was put into or read from the buffer.
        //
        ulSize = 0;
    }
    return(ulSize);
}

//*****************************************************************************
//
//! This function is used to read data from a USB HCD pipe.
//!
//! \param ulPipe is the USB pipe to read data from.
//! \param pucData is a pointer to store the data that is received.
//! \param ulSize is the size in bytes of the buffer pointed to by pucData.
//!
//! This function will not block and will only read as much data as requested
//! or as much data is currently available from the USB pipe.  The caller
//! should have registered a callback with the USBHCDPipeAlloc() call in order
//! to be informed when the data has been received.  The value returned by this
//! function can be less than the \e ulSize requested if the USB pipe has less
//! data available than was requested.
//!
//! \return This function returns the number of bytes that were returned in the
//! \e pucData buffer.
//
//*****************************************************************************
unsigned int
USBHCDPipeReadNonBlocking(unsigned int ulIndex, unsigned int ulPipe, 
                                    unsigned char *pucData, unsigned int ulSize)
{
    unsigned int ulEndpoint;

    //
    // Determine which endpoint interface that this pipe is using.
    //
    ulEndpoint = INDEX_TO_USB_EP((EP_PIPE_IDX_M & ulPipe) + 1);

    //
    // Read the data out of the USB endpoint interface.
    //
    USBEndpointDataGet(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint, pucData, &ulSize);

    //
    // Acknowledge that the data was read from the endpoint.
    //
    USBHostEndpointDataAck(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint);

    //
    // Go Idle once this state has been reached.
    //
    g_sUSBHCD[ulIndex].USBINPipes[EP_PIPE_IDX_M & ulPipe].eState = PIPE_IDLE;

    return(ulSize);
}

//*****************************************************************************
//
//! This function is used to read data from a USB HCD pipe.
//!
//! \param ulPipe is the USB pipe to read data from.
//! \param pucData is a pointer to store the data that is received.
//! \param ulSize is the size in bytes of the buffer pointed to by pucData.
//!
//! This function will block and will only return when it has read as much data
//! as requested from the USB pipe.  The caller should have registered a
//! callback with the USBHCDPipeAlloc() call in order to be informed when the
//! data has been received.  The value returned by this function can be less
//! than the \e ulSize requested if the USB pipe has less data available than
//! was requested.
//!
//! \return This function returns the number of bytes that were returned in the
//! \e pucData buffer.
//
//*****************************************************************************
unsigned int
USBHCDPipeRead(unsigned int ulIndex, unsigned int ulPipe, 
                      unsigned char *pucData, unsigned int ulSize)
{
    unsigned int ulEndpoint;
    unsigned int ulRemainingBytes;
    unsigned int ulPipeIdx;
    unsigned int ulEPStatus;
    unsigned int ulConnectRetry;
#ifdef DMA_MODE    
    unsigned int ulLength;
#else
    unsigned int ulBytesRead = 0;
#endif
    unsigned int ulTimer = 0;
    //
    // Get index used for looking up pipe data
    //
    ulPipeIdx = ulPipe & EP_PIPE_IDX_M;

    
    //
    // Determine which endpoint interface that this pipe is using.
    //
    ulEndpoint = INDEX_TO_USB_EP(ulPipeIdx + 1);

    //
    // Set the remaining bytes to received.
    //
    ulRemainingBytes = ulSize;

    //
    // Continue until all data requested has been received.
    //

#ifdef DMA_MODE
    //
    //Calculate the Number of blocks requested
    //
    if(ulSize <= 
       (g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].ulEpMaxPacketSize))
    {
        ulLength = ulSize;
    }
    else
    {
        ulLength = 
         (g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].ulEpMaxPacketSize);
    }
    
#endif      

    while(ulRemainingBytes != 0)
    {
#ifdef DMA_MODE
        //
        // Give enough buffer to DMA
        //
        g_sUSBHCD[ulIndex].rxBuffer = (unsigned char *)cppiDmaAllocBuffer();
        ASSERT(NULL!=g_sUSBHCD[ulIndex].rxBuffer);
        doDmaRxTransfer(ulIndex, ulLength, g_sUSBHCD[ulIndex].rxBuffer, 
                                    ulEndpoint);        
        //
        //Enable the DMA
        //
        enableCoreRxDMA(g_USBInstance[ulIndex].uiUSBInstance, ulEndpoint);
#endif      
        //
        // Start a read request.
        //
        g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_READING;

        //
        // Set pending DMA flag
        //
        g_sUSBHCD[ulIndex].ulDMAPending |= DMA_PEND_RECEIVE_FLAG << ulPipeIdx;

        //
        // Trigger a request for data from the device.
        //
        USBHostRequestIN(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint);

        //
        // Wait for a status change.
        //
        if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
        {
           ulTimer = g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0;
           //StartTimer(ulTimer);
           timer_interval(&UsbPipeReadTimer[ulIndex], ulTimer);
           timer_enable(&UsbPipeReadTimer[ulIndex]);

        }
        while(g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState == PIPE_READING)
        {
            //
            // Exit the loop for any of the event(s): Disconnect.
            //
            if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_DISCONNECT)
            {
                //
                // Update the Pipe status
                //
                g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_ERROR;
                continue;
            }
            else if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_BABBLE_FAULT)
            {
                //
                // Update the Pipe status
                //
                g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_ERROR;
                continue;
            }

            //
            // Read endpoint status.
            //
            ulEPStatus = USBEndpointStatus(g_USBInstance[ulIndex].uiBaseAddr, 
                                   ulEndpoint);

            //
            // Check for stall condition occurence.
            //
            if(ulEPStatus & USB_HOST_IN_STALL)
            {
                g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_STALLED;
                continue;
            }
            else if(ulEPStatus & USB_HOST_IN_ERROR)
            {
                g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_ERROR;
                continue;
            }

            if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
            {
                if(timer_tick(&UsbPipeReadTimer[ulIndex])/*IsTimerElapsed()*/)
                {
                    ulTimer = 0;
                    g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_ERROR;
                    g_sUSBHCD[ulIndex].USBHTimeOut.Status.slNonEP0 = (1<<((ulPipeIdx + 1)+16));
                    break;
                }
            }
        }
        if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
        {
            //StopTimer();
        	timer_disable(&UsbPipeReadTimer[ulIndex]);
            ulTimer = 0; 
        }
        //
        // If data is ready then return it.
        //
        if(g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState == PIPE_DATA_READY)
        {


#ifdef DMA_MODE            
            disableCoreRxDMA(g_USBInstance[ulIndex].uiUSBInstance, ulEndpoint);

            //
            //Copy the data from DMA buffer to application buffer
            //
            //memset (pucData, 0, ulLength);
            memcpy(pucData, g_sUSBHCD[ulIndex].rxBuffer, ulLength);
            
            //
            //Free the DMA Buffer
            //
            cppiDmaFreeBuffer((unsigned int * )g_sUSBHCD[ulIndex].rxBuffer);

            //
            //Claculate the packet length
            //
            if(ulRemainingBytes <=
                (g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].ulEpMaxPacketSize))
            {
                ulRemainingBytes = ulRemainingBytes - ulLength;
            }
            else
            {
                ulRemainingBytes = ulRemainingBytes - ulLength;
                pucData += ulLength;
            }

            //
            //Update the Pipe status
            //
            if(ulRemainingBytes != 0)
            {
                g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_READING;
            }

#else
            //
            // Request all of the remaining bytes.
            //
            ulBytesRead = ulRemainingBytes;

            //
            // Read the data out of the USB endpoint interface.
            //
            USBEndpointDataGet(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint, pucData,
                               &ulBytesRead);

            //
            // Acknowledge that the data was read from the endpoint.
            //
            USBHostEndpointDataAck(g_USBInstance[ulIndex].uiBaseAddr , ulEndpoint); 

            //
            // Subtract the number of bytes read from the bytes remaining.
            //
            ulRemainingBytes -= ulBytesRead;

            //
            // If there were less than 64 bytes read, then this was a short
            // packet and no more data will be returned.
            //
            if(ulBytesRead < g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].ulEpMaxPacketSize)
            {
                //
                // Subtract off the bytes that were not received and exit the
                // loop.
                //
                ulSize = ulSize - ulRemainingBytes;
                break;
            }
            else
            {
                //
                // Move the buffer ahead to receive more data into the buffer.
                //
                pucData += g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].ulEpMaxPacketSize;
            }
#endif


        }
        else if(g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState == PIPE_STALLED)
        {
            USBHCDRxAbort(ulIndex, ulEndpoint);
            //
            // Zero out the size so that the caller knows that no data was read.
            //
            ulSize = 0;

            //
            // This is the actual endpoint number.
            //
             USBHCDClearFeature(ulIndex, 1, ulPipe, USB_FEATURE_EP_HALT);       

            //
            // If there was a stall, then no more data is coming so break out.
            //
            break;
        }
        else if(g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState == PIPE_ERROR)
        {
            // Clean-up the EP FIFOs and CPPIDMA (if applicable)
            USBHCDRxAbort(ulIndex, ulEndpoint);

            if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0)
            {
                if(!ulTimer)
                {
                     // Try re-establising the connection with device
                    ulConnectRetry = g_sUSBHCD[ulIndex].ulConnectRetry;
                    for(;ulConnectRetry;--ulConnectRetry)
                    {
                        if(USBHCDRetryConnect(ulIndex))
                        {
                             break;
                        }
                    }
                    // Abort the transfer if re-establising the connection with device failed.
                    if(!ulConnectRetry)
                    {
                       //
                       // Set the size to 0 to indicate Error.
                       //
                       ulSize = 0;
                       break;
                    }
                }
            }
            else
            {
               //
               // Set the size to 0 to indicate Error.
               //
               ulSize = 0;
               break;
            }
        }
    }

    //
    // Go Idle once this state has been reached.
    //
    if(!ulRemainingBytes)
    {
        g_sUSBHCD[ulIndex].USBINPipes[ulPipeIdx].eState = PIPE_IDLE;
    }

    return(ulSize);
}

//*****************************************************************************
//
//! This function is used to release a USB pipe.
//!
//! \param ulPipe is the allocated USB pipe to release.
//!
//! This function is used to release a USB pipe that was allocated by a call to
//! USBHCDPipeAlloc() for use by some other device endpoint in the system.
//! Freeing an unallocated or invalid pipe will not generate an error and will
//! instead simply return.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDPipeFree(unsigned int ulIndex, unsigned int ulPipe)
{
    int iDMAIdx;

    if(ulPipe & EP_PIPE_TYPE_OUT)
    {
        //
        // Clear the address and type for this endpoint to free it up.
        //
        g_sUSBHCD[ulIndex].USBOUTPipes[ulPipe & EP_PIPE_IDX_M].ulDevAddr = 0;
        g_sUSBHCD[ulIndex].USBOUTPipes[ulPipe & EP_PIPE_IDX_M].ulType = 0;
        g_sUSBHCD[ulIndex].USBOUTPipes[ulPipe & EP_PIPE_IDX_M].pfnCallback = 0;

        //
        // Get the dma channel used by this pipe.
        //
        iDMAIdx = g_sUSBHCD[ulIndex].USBOUTPipes[ulPipe & EP_PIPE_IDX_M].ucDMAChannel;

        //
        // Mark the channel as free for use.
        //
        g_sUSBHCD[ulIndex].ucDMAChannels[iDMAIdx] = USBHCD_DMA_UNUSED;

        //
        // Clear out the current channel in use by this pipe.
        //
        g_sUSBHCD[ulIndex].USBOUTPipes[ulPipe & EP_PIPE_IDX_M].ucDMAChannel =
            USBHCD_DMA_UNUSED;

        //
        // Free up the FIFO memory used by this endpoint.
        //
        if(g_sUSBHCD[ulIndex].USBOUTPipes[ulPipe & EP_PIPE_IDX_M].ucFIFOSize)
        {
            FIFOFree(&g_sUSBHCD[ulIndex].USBOUTPipes[ulPipe & EP_PIPE_IDX_M]);
        }
    }
    else if(ulPipe & EP_PIPE_TYPE_IN)
    {
        //
        // Clear the address and type for this endpoint to free it up.
        //
        g_sUSBHCD[ulIndex].USBINPipes[ulPipe & EP_PIPE_IDX_M].ulDevAddr = 0;
        g_sUSBHCD[ulIndex].USBINPipes[ulPipe & EP_PIPE_IDX_M].ulType = 0;
        g_sUSBHCD[ulIndex].USBINPipes[ulPipe & EP_PIPE_IDX_M].pfnCallback = 0;

        //
        // Get the dma channel used by this pipe.
        //
        iDMAIdx = g_sUSBHCD[ulIndex].USBINPipes[ulPipe & EP_PIPE_IDX_M].ucDMAChannel;

        //
        // Mark the channel as free for use.
        //
        g_sUSBHCD[ulIndex].ucDMAChannels[iDMAIdx] = USBHCD_DMA_UNUSED;

        //
        // Clear out the current channel in use by this pipe.
        //
        g_sUSBHCD[ulIndex].USBINPipes[ulPipe & EP_PIPE_IDX_M].ucDMAChannel =
            USBHCD_DMA_UNUSED;

        //
        // Free up the FIFO memory used by this endpoint.
        //
        if(g_sUSBHCD[ulIndex].USBINPipes[ulPipe & EP_PIPE_IDX_M].ucFIFOSize)
        {
            FIFOFree(&g_sUSBHCD[ulIndex].USBINPipes[ulPipe & EP_PIPE_IDX_M]);
        }
    }
}

//*****************************************************************************
//
// This internal function initializes the HCD code.
//
// \param ulIndex specifies which USB controller to use.
// \param pvPool is a pointer to the data to use as a memory pool for this
// controller.
// \param ulPoolSize is the size in bytes of the buffer passed in as pvPool.
//
// This function will perform all the necessary operations to allow the USB
// host controller to begin enumeration and communication with a device.  This
// function should typically be called once at the start of an application
// before any other calls are made to the host controller.
//
// \return None.
//
//*****************************************************************************
static void
USBHCDInitInternal(unsigned int ulIndex, void *pvPool,
                   unsigned int ulPoolSize)
{
    int iIdx;

    ASSERT(ulIndex == 0);

    //
    // The first 64 Bytes are allocated to endpoint 0.
    //
    g_ulAlloc[0] = 1;
    g_ulAlloc[1] = 0;

    //
    // Save the base address for this controller.
    //
    g_sUSBHCD[ulIndex].ulUSBBase = g_USBInstance[ulIndex].uiBaseAddr ;

    //
    // All Pipes are unused at start.
    //
    for(iIdx = 0; iIdx < MAX_NUM_PIPES; iIdx++)
    {
        g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulDevAddr = 0;
        g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulType = USBHCD_PIPE_UNUSED;
        g_sUSBHCD[ulIndex].USBINPipes[iIdx].ucDMAChannel = USBHCD_DMA_UNUSED;
        g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].ulDevAddr = 0;
        g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].ulType = USBHCD_PIPE_UNUSED;
        g_sUSBHCD[ulIndex].USBOUTPipes[iIdx].ucDMAChannel = USBHCD_DMA_UNUSED;
    }

    //
    // All DMA channels are unused at start.
    //
    for(iIdx = 0; iIdx < MAX_NUM_DMA_CHANNELS; iIdx++)
    {
        g_sUSBHCD[ulIndex].ucDMAChannels[iIdx] = USBHCD_DMA_UNUSED;
    }

    //
    // Initialized the device structure.
    //
    g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_IDLE;
    g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor = 0;

    //
    // Initialize the device descriptor.
    //
    g_sUSBHCD[ulIndex].USBDevice[0].DeviceDescriptor.bLength = 0;
    g_sUSBHCD[ulIndex].USBDevice[0].DeviceDescriptor.bMaxPacketSize0 = 0;

    //
    // Initialize the device address.
    //
    g_sUSBHCD[ulIndex].USBDevice[0].ulAddress = 0;

    //
    // Set the current interface to 0.
    //
    g_sUSBHCD[ulIndex].USBDevice[0].ulInterface = 0;

    //
    // Allocate the memory needed for reading descriptors.
    //
    g_sUSBHCD[ulIndex].pvPool = pvPool;
    g_sUSBHCD[ulIndex].ulPoolSize = ulPoolSize;

    //
    // Initialize the device class.
    //
    g_sUSBHCD[ulIndex].ulClass = USB_CLASS_EVENTS;

    //
    // Initialize the USB tick module.
    //
    InternalUSBTickInit();

    //
    // Only do hardware update if the stack is in Host mode, do not touch the
    // hardware for OTG mode operation.
    //
    if(g_eUSBMode == USB_MODE_HOST)
    {
        //
        // Configure the End point 0.
        //
        USBHostEndpointConfig(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 64, 0, 0,
                              (USB_EP_MODE_CTRL | USB_EP_SPEED_HIGH |
                               USB_EP_HOST_OUT));

        
        USBEnableOtgIntr(g_USBInstance[ulIndex].uiSubBaseAddr);

        //
        // Enable USB Interrupts.
        //
        USBIntEnableControl(g_USBInstance[ulIndex].uiBaseAddr , USB_INTCTRL_RESET |
                                       USB_INTCTRL_DISCONNECT |
                                       USB_INTCTRL_SOF |
                                       USB_INTCTRL_SESSION |
                                       USB_INTCTRL_BABBLE |
                                       USB_INTCTRL_CONNECT |
                                       USB_INTCTRL_RESUME |
                                       USB_INTCTRL_SUSPEND |
                                       USB_INTCTRL_VBUS_ERR |
                                       USB_INTCTRL_POWER_FAULT);
        USBIntEnableEndpoint(g_USBInstance[ulIndex].uiBaseAddr , USB_INTEP_ALL);

        //
        // Enable the USB interrupt.
        //
        //IntEnable(INT_USB0);
#ifdef _TMS320C6X
        /* No DSP API to enable USB0 event */
#else
        IntSystemEnable(g_USBInstance[ulIndex].uiInterruptNum);
#endif

        //
        // There is no automatic power in pure host mode.
        //
        USBHCDPowerConfigSet(ulIndex, g_sUSBHCD[ulIndex].ulPowerConfig & ~USB_HOST_PWREN_AUTO);

        //
        // This is required to get into host mode.
        //
       USBOTGSessionRequest(g_USBInstance[ulIndex].uiBaseAddr , true);
    }
}

//*****************************************************************************
//
//! This function is used to set the power pin and power fault configuration.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ulPwrConfig is the power configuration to use for the application.
//!
//! This function must be called before HCDInit() is called so that the power
//! pin configuration can be set before power is enabled.  The \e ulPwrConfig
//! flags specify the power fault level sensitivity, the power fault action,
//! and the power enable pin level and source.
//!
//! One of the following can be selected as the power fault level sensitivity:
//!
//! - \b USBHCD_FAULT_LOW - An external power fault is indicated by the pin
//!                         being driven low.
//! - \b USBHCD_FAULT_HIGH - An external power fault is indicated by the pin
//!                          being driven high.
//!
//! One of the following can be selected as the power fault action:
//!
//! - \b USBHCD_FAULT_VBUS_NONE - No automatic action when power fault
//!   detected.
//! - \b USBHCD_FAULT_VBUS_TRI - Automatically Tri-state the USBnEPEN pin on a
//!                              power fault.
//! - \b USBHCD_FAULT_VBUS_DIS - Automatically drive the USBnEPEN pin to it's
//!                              inactive state on a power fault.
//!
//! One of the following can be selected as the power enable level and source:
//!
//! - \b USBHCD_VBUS_MANUAL - Power control is completely managed by the
//!                           application, the USB library will provide a
//!                           power callback to request power state changes.
//! - \b USBHCD_VBUS_AUTO_LOW - USBEPEN is driven low by the USB controller
//!                             automatically if USBOTGSessionRequest() has
//!                             enabled a session.
//! - \b USBHCD_VBUS_AUTO_HIGH - USBEPEN is driven high by the USB controller
//!                              automatically if USBOTGSessionRequest() has
//!                              enabled a session.
//!
//! If USBHCD_VBUS_MANUAL is used then the application must provide an
//! event driver to receive the USB_EVENT_POWER_ENABLE and
//! USB_EVENT_POWER_DISABLE events and enable and disable power to VBUS when
//! requested by the USB library.  The application should respond to a power
//! control callback by enabling or disabling VBUS as soon as possible and
//! before returning from the callback function.
//!
//! \note The following values should no longer be used with the USB library:
//! USB_HOST_PWRFLT_LOW, USB_HOST_PWRFLT_HIGH, USB_HOST_PWRFLT_EP_NONE,
//! USB_HOST_PWRFLT_EP_TRI, USB_HOST_PWRFLT_EP_LOW, USB_HOST_PWRFLT_EP_HIGH,
//! USB_HOST_PWREN_LOW, USB_HOST_PWREN_HIGH, USB_HOST_PWREN_VBLOW, and
//! USB_HOST_PWREN_VBHIGH.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDPowerConfigInit(unsigned int ulIndex, unsigned int ulPwrConfig)
{
    ASSERT(ulIndex == 0);

    //
    // Save the value as it will be used later.
    //
    g_sUSBHCD[ulIndex].ulPowerConfig = ulPwrConfig;
}

//*****************************************************************************
//
//! This function is used to get the power pin and power fault configuration.
//!
//! \param ulIndex specifies which USB controller to use.
//!
//! This function will return the current power control pin configuration as
//! set by the USBHCDPowerConfigInit() function or the defaults if not yet set.
//! See the USBHCDPowerConfigInit() documentation for the meaning of the bits
//! that are returned by this function.
//!
//! \return The configuration of the power control pins.
//!
//*****************************************************************************
unsigned int
USBHCDPowerConfigGet(unsigned int ulIndex)
{
    ASSERT(ulIndex == 0);

    //
    // Save the value as it will be used later.
    //
    return(g_sUSBHCD[ulIndex].ulPowerConfig);
}

//*****************************************************************************
//
//! This function is used to set the power pin and power fault configuration.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ulConfig specifies which USB power configuration to use.
//!
//! This function will set the current power control pin configuration as
//! set by the USBHCDPowerConfigInit() function or the defaults if not yet set.
//! See the USBHCDPowerConfigInit() documentation for the meaning of the bits
//! that are set by this function.
//!
//! \return Returns zero to indicate the power setting is now active.
//!
//*****************************************************************************
unsigned int
USBHCDPowerConfigSet(unsigned int ulIndex, unsigned int ulConfig)
{
    ASSERT(ulIndex == 0);

    //
    // Remember the current setting.
    //
    g_sUSBHCD[ulIndex].ulPowerConfig = ulConfig;

    //
    // Clear out the two flag bits.
    //
    ulConfig = g_sUSBHCD[ulIndex].ulPowerConfig & ~(USBHCD_VBUS_MANUAL | USBHCD_FAULT_VBUS_DIS);

    //
    // If there is an automatic disable power action specified then set the
    // polarity of the signal to match EPEN.
    //
    if(g_sUSBHCD[ulIndex].ulPowerConfig & USBHCD_FAULT_VBUS_DIS)
    {
        //
        // Insure that the assumption below is true.
        //
        ASSERT((USBHCD_VBUS_AUTO_HIGH & 1) == 1);
        ASSERT((USBHCD_VBUS_AUTO_LOW & 1) == 0);

        //
        // This is taking advantage of the difference between
        // USBHCD_VBUS_AUTO_LOW and USBHCD_VBUS_AUTO_HIGH being that bit
        // one is set when EPEN is active high.
        //
        if(g_sUSBHCD[ulIndex].ulPowerConfig & 1)
        {
            g_sUSBHCD[ulIndex].ulPowerConfig |= USB_HOST_PWRFLT_EP_HIGH;
        }
        else
        {
            g_sUSBHCD[ulIndex].ulPowerConfig |= USB_HOST_PWRFLT_EP_LOW;
        }
    }

    //
    // Initialize the power configuration.
    //
    USBHostPwrConfig(g_USBInstance[ulIndex].uiBaseAddr , ulConfig);

    //
    // If not in manual mode then just turn on power.
    //
    if((g_sUSBHCD[ulIndex].ulPowerConfig & USBHCD_VBUS_MANUAL) == 0)
    {
        //
        // Power the USB bus.
        //
        USBHostPwrEnable(g_USBInstance[ulIndex].uiBaseAddr );
    }

    //
    // Return success.
    //
    return(0);
}

//*****************************************************************************
//
//! This function returns if the current power settings will automatically
//! handle enabling and disabling VBUS power.
//!
//! \param ulIndex specifies which USB controller to query.
//!
//! This function returns if the current power control pin configuration will
//! automatically apply power or whether it will be left to the application
//! to turn on power when it is notified.
//!
//! \return A non-zero value indicates that power is automatically applied and
//! a value of zero indicates that the application must manually apply power.
//!
//*****************************************************************************
unsigned int
USBHCDPowerAutomatic(unsigned int ulIndex)
{
    //
    // Check if the controller is automatically applying power or not.
    //
    if(g_sUSBHCD[ulIndex].ulPowerConfig & USBHCD_VBUS_MANUAL)
    {
        return(0);
    }
    return(1);
}

//*****************************************************************************
//
//! This function is used to initialize the HCD code.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param pvPool is a pointer to the data to use as a memory pool for this
//! controller.
//! \param ulPoolSize is the size in bytes of the buffer passed in as pvPool.
//!
//! This function will perform all the necessary operations to allow the USB
//! host controller to begin enumeration and communication with devices.  This
//! function should typically be called once at the start of an application
//! once all of the device and class drivers are ready for normal operation.
//! This call will start up the USB host controller and any connected device
//! will immediately start the enumeration sequence.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDInit(unsigned int ulIndex, void *pvPool, unsigned int ulPoolSize)
{
    int iDriver;

    //
    // Check the arguments.
    //
    ASSERT(ulIndex == 0);

    //
    // Make sure there is at least enough to read the configuration descriptor.
    //
    ASSERT(ulPoolSize >= sizeof(tConfigDescriptor));

    //
    // Should not call this if the stack is in device mode.
    //
    ASSERT(g_eUSBMode != USB_MODE_DEVICE)

    
    if(ulIndex == 0)
    {
        g_USBInstance[ulIndex].uiUSBInstance = ulIndex;
        g_USBInstance[ulIndex].uiBaseAddr = USB0_BASE;
        g_USBInstance[ulIndex].uiSubBaseAddr = USB_0_OTGBASE;
        g_USBInstance[ulIndex].uiInterruptNum = SYS_INT_USB0;
        g_USBInstance[ulIndex].uiSubInterruptNum = SYS_INT_USBSSINT;
        g_USBInstance[ulIndex].uiPHYConfigRegAddr = CFGCHIP2_USBPHYCTRL;
    }
#if (USB_NUM_INSTANCE == 2)
    else if(ulIndex == 1)
    {
        g_USBInstance[ulIndex].uiUSBInstance = ulIndex;
        g_USBInstance[ulIndex].uiBaseAddr = USB1_BASE;
        g_USBInstance[ulIndex].uiSubBaseAddr = USB_1_OTGBASE;
        g_USBInstance[ulIndex].uiInterruptNum = SYS_INT_USB1;
        g_USBInstance[ulIndex].uiSubInterruptNum = SYS_INT_USBSSINT;
        g_USBInstance[ulIndex].uiPHYConfigRegAddr = CFGCHIP2_USB1PHYCTRL;
    }
#endif

   g_sUSBHCD[ulIndex].USBHTimeOut.Value.slEP0 = USB_EP0_TIMEOUT_MILLISECS;
   g_sUSBHCD[ulIndex].USBHTimeOut.Value.slNonEP0= USB_NONEP0_TIMEOUT_MILLISECS;
   g_sUSBHCD[ulIndex].USBHTimeOut.Status.slEP0 = 0;
   g_sUSBHCD[ulIndex].USBHTimeOut.Status.slNonEP0= 0;
    //
    // If no mode is set then make the mode become host mode.
    //
    if(g_eUSBMode == USB_MODE_NONE)
    {
        g_eUSBMode = USB_MODE_HOST;
    }

    //
    // Only do hardware update if the stack is in Host mode, do not touch the
    // hardware for OTG mode operation.
    //
    if(g_eUSBMode == USB_MODE_HOST)
    {
        //
        // Enable Clocking to the USB controller.
        //        
        USBModuleClkEnable(ulIndex, g_USBInstance[ulIndex].uiBaseAddr );

        USBReset(g_USBInstance[ulIndex].uiSubBaseAddr);
        //
        // Turn on USB Phy clock.
        //
        UsbPhyOn(ulIndex);
    }

    //
    // Call our internal function to perform the initialization.
    //
    USBHCDInitInternal(ulIndex, pvPool, ulPoolSize);

    //
    // No event driver is present by default.
    //
    g_sUSBHCD[ulIndex].iEventDriver = -1;
	
    //
    // Retry recovery and comunication with device on error.
    //
    g_sUSBHCD[ulIndex].ulConnectRetry = USBHCD_DEV_RECOVER_RETRY;

    //
    // Search through the Host Class driver list for the devices class.
    //
    for(iDriver = 0; iDriver < g_sUSBHCD[ulIndex].ulNumClassDrivers; iDriver++)
    {
        if(g_sUSBHCD[ulIndex].pClassDrivers[iDriver]->ulInterfaceClass ==
           USB_CLASS_EVENTS)
        {
            //
            // Event driver was found so remember it.
            //
            g_sUSBHCD[ulIndex].iEventDriver = iDriver;
        }
    }    
    
}

//*****************************************************************************
//
//! This function is used to initialize the HCD class driver list.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ppHClassDrvs is an array of host class drivers that are
//! supported on this controller.
//! \param ulNumDrivers is the number of entries in the \e pHostClassDrivers
//! array.
//!
//! This function will set the host classes supported by the host controller
//! specified by the \e ulIndex parameter.  This function should be called
//! before enabling the host controller driver with the USBHCDInit() function.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDRegisterDrivers(unsigned int ulIndex,
                      const tUSBHostClassDriver * const *ppHClassDrvs,
                      unsigned int ulNumDrivers)
{
    ASSERT(ulIndex == 0);

    g_sUSBHCD[ulIndex].ulIndex = ulIndex;

    //
    // Save the class drivers.
    //
    g_sUSBHCD[ulIndex].pClassDrivers = ppHClassDrvs;

    //
    // Save the number of class drivers.
    //
    g_sUSBHCD[ulIndex].ulNumClassDrivers = ulNumDrivers;
}

//*****************************************************************************
//
//! This function is used to terminate the HCD code.
//!
//! \param ulIndex specifies which USB controller to release.
//!
//! This function will clean up the USB host controller and disable it in
//! preparation for shutdown or a switch to USB device mode.  Once this call is
//! made, \e USBHCDInit() may be called to reinitialize the controller and
//! prepare for host mode operation.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDTerm(unsigned int ulIndex)
{
    ASSERT(ulIndex == 0);

    //
    // End the session.
    //
    USBOTGSessionRequest(g_USBInstance[ulIndex].uiBaseAddr , false);

    //
    // Remove power from the USB bus.
    //
    USBHostPwrDisable(g_USBInstance[ulIndex].uiBaseAddr );

    //
    // Disable USB interrupts.
    //
#ifdef _TMS320C6X
    /* No DSP API to disable USB0 event */
#else
    IntSystemDisable(g_USBInstance[ulIndex].uiInterruptNum);
#endif

    USBIntDisableControl(g_USBInstance[ulIndex].uiBaseAddr , USB_INTCTRL_ALL);

    USBIntDisableEndpoint(g_USBInstance[ulIndex].uiBaseAddr , USB_INTEP_ALL);

    //
    // Set the host controller state back to it's initial values.
    //
    g_sUSBHCD[ulIndex].USBINPipes[0].ulType = USBHCD_PIPE_UNUSED;
    g_sUSBHCD[ulIndex].USBINPipes[1].ulType = USBHCD_PIPE_UNUSED;
    g_sUSBHCD[ulIndex].USBINPipes[2].ulType = USBHCD_PIPE_UNUSED;
    g_sUSBHCD[ulIndex].USBOUTPipes[0].ulType = USBHCD_PIPE_UNUSED;
    g_sUSBHCD[ulIndex].USBOUTPipes[1].ulType = USBHCD_PIPE_UNUSED;
    g_sUSBHCD[ulIndex].USBOUTPipes[2].ulType = USBHCD_PIPE_UNUSED;
    g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_IDLE;
    g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor = 0;
    g_sUSBHCD[ulIndex].USBDevice[0].DeviceDescriptor.bLength = 0;
    g_sUSBHCD[ulIndex].USBDevice[0].DeviceDescriptor.bMaxPacketSize0 = 0;
    g_sUSBHCD[ulIndex].USBDevice[0].ulAddress = 0;
    g_sUSBHCD[ulIndex].USBDevice[0].ulInterface = 0;
    g_sUSBHCD[ulIndex].pvPool = 0;
    g_sUSBHCD[ulIndex].ulPoolSize = 0;
}

//*****************************************************************************
//
//! This function generates reset signaling on the USB bus.
//!
//! \param ulIndex specifies which USB controller to use.
//!
//! This function handles sending out reset signaling on the USB bus.  After
//! returning from this function, any attached device on the USB bus should
//! have returned to it's reset state.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDReset(unsigned int ulIndex)
{
    ASSERT(ulIndex == 0);

    //
    // Start the reset signaling.
    //
    USBHostReset(g_USBInstance[ulIndex].uiBaseAddr , 1);

    //
    // Wait 20ms
    //    
    Sysdelay(20);

    //
    // End reset signaling on the bus.
    //
    USBHostReset(g_USBInstance[ulIndex].uiBaseAddr , 0);

    //
    // Need to wait at least 10ms to let the device recover from
    // the reset.  This is the delay specified in the USB 2.0 spec.
    // We will hold the reset for 20ms.
    //  
    Sysdelay(20);
}

//*****************************************************************************
//
//! This function will generate suspend signaling on the USB bus.
//!
//! \param ulIndex specifies which USB controller to use.
//!
//! This function is used to generate suspend signaling on the USB bus.  In
//! order to leave the suspended state, the application should call
//! USBHCDResume().
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDSuspend(unsigned int ulIndex)
{
    ASSERT(ulIndex == 0);

    //
    // Start the suspend signaling.
    //
    USBHostSuspend(g_USBInstance[ulIndex].uiBaseAddr );
}

//*****************************************************************************
//
//! This function will generate resume signaling on the USB bus.
//!
//! \param ulIndex specifies which USB controller to use.
//!
//! This function is used to generate resume signaling on the USB bus in order
//! to cause  USB devices to leave their suspended state.  This call should
//! not be made unless a preceding call to USBHCDSuspend() has been made.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDResume(unsigned int ulIndex)
{
    ASSERT(ulIndex == 0);

    //
    // Start the resume signaling.
    //
    USBHostResume(g_USBInstance[ulIndex].uiBaseAddr , 1);

    //
    // Wait 100ms
    // 
      Sysdelay(100);

    //
    // End reset signaling on the bus.
    //
    USBHostResume(g_USBInstance[ulIndex].uiBaseAddr , 0);
}

//*****************************************************************************
//
//! This function issues a request for the current configuration descriptor
//! from a device.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param pDevice is a pointer to the device structure that holds the buffer
//! to store the configuration descriptor.
//!
//! This function will request the configuration descriptor from the device.
//! The \e pDevice->ConfigDescriptor member variable is used to hold the data
//! for this request.  This buffer will be allocated from the pool provided by
//! the HCDInit() function.  \e pDevice->DeviceDescriptor.bMaxPacketSize0
//! should be valid prior to this call in order to correctly receive the
//! configuration descriptor.  If this variable is not valid then this call
//! will not return accurate data.
//!
//! \return The number of bytes returned due to the request.  This value can be
//! zero if the device did not respond.
//
//*****************************************************************************
static unsigned int
USBHCDGetConfigDescriptor(unsigned int ulIndex, tUSBHostDevice *pDevice)
{
    tUSBRequest SetupPacket;
    unsigned int ulBytes;

    ASSERT(ulIndex == 0);

    ulBytes = 0;

    //
    // This is a Standard Device IN request.
    //
    SetupPacket.bmRequestType =
        USB_RTYPE_DIR_IN | USB_RTYPE_STANDARD | USB_RTYPE_DEVICE;

    //
    // Request a Device Descriptor.
    //
    SetupPacket.bRequest = USBREQ_GET_DESCRIPTOR;
    SetupPacket.wValue = USB_DTYPE_CONFIGURATION << 8;

    //
    // Index is always 0 for device configurations requests.
    //
    SetupPacket.wIndex = 0;

    //
    // Only ask for the configuration header first to see how big the
    // whole thing is.
    //
    if(g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor == 0)
    {
        //
        // Only request the space available.
        //
        SetupPacket.wLength = sizeof(tConfigDescriptor);

        //
        // Set the memory to use for the config descriptor and save the size.
        //
        g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor = g_sUSBHCD[ulIndex].pvPool;
        g_sUSBHCD[ulIndex].USBDevice[0].ulConfigDescriptorSize = g_sUSBHCD[ulIndex].ulPoolSize;

        //
        // Put the setup packet in the buffer.
        //
        ulBytes =
            USBHCDControlTransfer(ulIndex, &SetupPacket, pDevice->ulAddress,
                                  (unsigned char *)pDevice->pConfigDescriptor,
                                  sizeof(tConfigDescriptor),
                                  pDevice->DeviceDescriptor.bMaxPacketSize0);
    }

    //
    // If the Configuration header was successfully returned then get the
    // full configuration descriptor.
    //
    if(ulBytes == sizeof(tConfigDescriptor))
    {
        //
        // Save the total size and request the full configuration descriptor.
        //
        SetupPacket.wLength =
            g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor->wTotalLength;

        //
        // Don't allow the buffer to be larger than was allocated.
        //
        if(SetupPacket.wLength > g_sUSBHCD[ulIndex].ulPoolSize)
        {
            SetupPacket.wLength = g_sUSBHCD[ulIndex].ulPoolSize;
        }

        //
        // Put the setup packet in the buffer.
        //
        ulBytes =
            USBHCDControlTransfer(ulIndex, &SetupPacket, pDevice->ulAddress,
                                  (unsigned char *)pDevice->pConfigDescriptor,
                                  SetupPacket.wLength,
                                  pDevice->DeviceDescriptor.bMaxPacketSize0);
    }

    return(ulBytes);
}

//*****************************************************************************
//
//! This function issues a request for a device descriptor from a device.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param pDevice is a pointer to the device structure that holds the buffer
//! to store the device descriptor into.
//!
//! This function will request the device descriptor from the device.  The
//! \e pDevice->DeviceDescriptor descriptor is used to hold the data for this
//! request.  \e pDevice->DeviceDescriptor.bMaxPacketSize0 should be
//! initialized to zero or to the valid maximum packet size if it is known.  If
//! this variable is not set to zero, then this call will determine the maximum
//! packet size for endpoint 0 and save it in the structure member
//! bMaxPacketSize0.
//!
//! \return The number of bytes returned due to the request.  This value can be
//! zero if the device did not respond.
//
//*****************************************************************************
static unsigned int
USBHCDGetDeviceDescriptor(unsigned int ulIndex, tUSBHostDevice *pDevice)
{
    tUSBRequest SetupPacket;
    unsigned int ulBytes;
    unsigned int retStatus = 1;

    ASSERT(ulIndex == 0);

    //
    // This is a Standard Device IN request.
    //
    SetupPacket.bmRequestType =
        USB_RTYPE_DIR_IN | USB_RTYPE_STANDARD | USB_RTYPE_DEVICE;

    //
    // Request a Device Descriptor.
    //
    SetupPacket.bRequest = USBREQ_GET_DESCRIPTOR;
    SetupPacket.wValue = USB_DTYPE_DEVICE << 8;

    //
    // Index is always 0 for device requests.
    //
    SetupPacket.wIndex = 0;

    //
    // All devices must have at least an 8 byte max packet size so just ask
    // for 8 bytes to start with.
    //
    SetupPacket.wLength = 8;

    ulBytes = 0;

    //
    // Discover the max packet size for endpoint 0.
    //
    if(pDevice->DeviceDescriptor.bMaxPacketSize0 == 0)
    {
        //
        // Put the setup packet in the buffer.
        //
        ulBytes =
            USBHCDControlTransfer(ulIndex, &SetupPacket, pDevice->ulAddress,
                                  (unsigned char *)&(pDevice->DeviceDescriptor),
                                  sizeof(tDeviceDescriptor),
                                  MAX_PACKET_SIZE_EP0);
    }
    retStatus = ulBytes;

    //
    // Now get the full descriptor now that the actual maximum packet size
    // is known.
    //
    if(retStatus && (ulBytes < sizeof(tDeviceDescriptor)))
    {
        SetupPacket.wLength = (unsigned short)sizeof(tDeviceDescriptor);

        ulBytes =
            USBHCDControlTransfer(ulIndex, &SetupPacket, pDevice->ulAddress,
                                  (unsigned char *)&(pDevice->DeviceDescriptor),
                                  sizeof(tDeviceDescriptor),
                                  pDevice->DeviceDescriptor.bMaxPacketSize0);
    }

    return(ulBytes);
}

//*****************************************************************************
//
//! This function is used to send the set address command to a device.
//!
//! \param ulDevAddress is the new device address to use for a device.
//!
//! The USBHCDSetAddress() function is used to set the USB device address, once
//! a device has been discovered on the bus.  This is typically issued
//! following a USB reset which is triggered by a call the USBHCDReset().  The
//! address passed into this function via the \e ulDevAddress parameter should
//! be used for all further communications with the device once this function
//! returns.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDSetAddress(unsigned int ulIndex, unsigned int ulDevAddress)
{
    tUSBRequest SetupPacket;

    //
    // This is a Standard Device OUT request.
    //
    SetupPacket.bmRequestType =
        USB_RTYPE_DIR_OUT | USB_RTYPE_STANDARD | USB_RTYPE_DEVICE;

    //
    // Request a Device Descriptor.
    //
    SetupPacket.bRequest = USBREQ_SET_ADDRESS;
    SetupPacket.wValue = ulDevAddress;

    //
    // Index is always 0 for device requests.
    //
    SetupPacket.wIndex = 0;

    //
    // Only request the space available.
    //
    SetupPacket.wLength = 0;

    //
    // Put the setup packet in the buffer.
    //
    USBHCDControlTransfer(ulIndex, &SetupPacket, 0, 0, 0, MAX_PACKET_SIZE_EP0);

    //
    // Must delay 2ms after setting the address.
    //
    Sysdelay(2);
}

//*****************************************************************************
//
//! This function is used to send a Clear Feature request to a device.
//!
//! \param ulDevAddress is the USB bus address of the device that will receive
//! this request.
//! \param ulPipe is the pipe that will be used to send the request.
//! \param ulFeature is one of the USB_FEATURE_* definitions.
//!
//! This function will issue a Clear Feature request to the device indicated
//! by the \e ulDevAddress parameter.  The \e ulPipe parameter is the USB pipe
//! that should be used to send this request.  The \e ulFeature parameter
//! should be one of the following values:
//!
//! * \b USB_FEATURE_EP_HALT is used to end a HALT condition on a devices
//!   endpoint.
//! * \b USB_FEATURE_REMOTE_WAKE is used to disable a device's remote wake
//!   feature.
//! * \b USB_FEATURE_TEST_MODE is used take the USB device out of test mode.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDClearFeature(unsigned int devIndex, unsigned int ulDevAddress, 
                        unsigned int ulPipe, unsigned int ulFeature)
{
    tUSBRequest SetupPacket;
    unsigned int ulIndex;
    
    //
    // Get the index number from the allocated pipe.
    //
    ulIndex = (ulPipe & EP_PIPE_IDX_M);

    //
    // This is a Standard Device OUT request.
    //
    SetupPacket.bmRequestType =
        USB_RTYPE_DIR_OUT | USB_RTYPE_STANDARD | USB_RTYPE_ENDPOINT;

    //
    // Request a Device Descriptor.
    //
    SetupPacket.bRequest = USBREQ_CLEAR_FEATURE;
    SetupPacket.wValue = ulFeature;

    //
    // Set the endpoint to access.
    //
    if(ulPipe & EP_PIPE_TYPE_IN)
    {
        SetupPacket.wIndex = g_sUSBHCD[devIndex].USBINPipes[ulIndex].ucEPNumber | 0x80;
    }
    else
    {
        SetupPacket.wIndex = g_sUSBHCD[devIndex].USBOUTPipes[ulIndex].ucEPNumber;
    }

    //
    // This is always 0.
    //
    SetupPacket.wLength = 0;

    //
    // Put the setup packet in the buffer.
    //
    USBHCDControlTransfer(devIndex, &SetupPacket, ulDevAddress, 0, 0,
                          MAX_PACKET_SIZE_EP0);

    //
    // Set the endpoint to access.
    //
    if(ulPipe & EP_PIPE_TYPE_IN)
    {
        USBEndpointDataToggleClear(g_USBInstance[devIndex].uiBaseAddr , INDEX_TO_USB_EP(ulIndex + 1),
                                   USB_EP_HOST_IN);
    }
    else
    {
        USBEndpointDataToggleClear(g_USBInstance[devIndex].uiBaseAddr , INDEX_TO_USB_EP(ulIndex + 1),
                                   USB_EP_HOST_OUT);
    }

    //
    // Must delay 2ms after clearing the feature.
    //
    Sysdelay(2);
}

//*****************************************************************************
//
//! This function is used to set the current configuration for a device.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ulDevice is the USB device for this function.
//! \param ulConfiguration is one of the devices valid configurations.
//!
//! This function is used to set the current device configuration for a USB
//! device.  The \e ulConfiguration value must be one of the configuration
//! indexes that was returned in the configuration descriptor from the device,
//! or a value of 0.  If 0 is passed in, the device will return to it's
//! addressed state and no longer be in a configured state.  If the value is
//! non-zero then the device will change to the requested configuration.
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDSetConfig(unsigned int ulIndex, unsigned int ulDevice,
                unsigned int ulConfiguration)
{
    tUSBRequest SetupPacket;
    tUSBHostDevice *pDevice;

    ASSERT(ulIndex == 0);

    pDevice = (tUSBHostDevice *)ulDevice;

    //
    // This is a Standard Device OUT request.
    //
    SetupPacket.bmRequestType =
        USB_RTYPE_DIR_OUT | USB_RTYPE_STANDARD | USB_RTYPE_DEVICE;

    //
    // Request a Device Descriptor.
    //
    SetupPacket.bRequest = USBREQ_SET_CONFIG;
    SetupPacket.wValue = ulConfiguration;

    //
    // Index is always 0 for device requests.
    //
    SetupPacket.wIndex = 0;

    //
    // Only request the space available.
    //
    SetupPacket.wLength = 0;

    //
    // Put the setup packet in the buffer.
    //
    USBHCDControlTransfer(ulIndex, &SetupPacket, pDevice->ulAddress, 0, 0,
                          MAX_PACKET_SIZE_EP0);
}

//*****************************************************************************
//
//! This function is used to set the current interface and alternate setting
//! for an interface on a device.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ulDevice is the USB device for this function.
//! \param ulInterface is one of the valid interface numbers for a device.
//! \param ulAltSetting is one of the valid alternate interfaces for the
//! ulInterface number.
//!
//! This function is used to change the alternate setting for one of the valid
//! interfaces on a USB device.  The \e ulDevice specifies the device instance
//! that was returned when the device was connected.  This call will set the
//! USB device's interface based on the \e ulInterface and \e ulAltSetting.
//!
//! \b Example: Set the USB device interface 2 to alternate setting 1.
//!
//! \verbatim
//! USBHCDSetInterface(0, ulDevice, 2, 1);
//! \endverbatim
//!
//! \return None.
//
//*****************************************************************************
void
USBHCDSetInterface(unsigned int ulIndex, unsigned int ulDevice,
                   unsigned int ulInterface, unsigned ulAltSetting)
{
    tUSBRequest SetupPacket;
    tUSBHostDevice *pDevice;

    ASSERT(ulIndex == 0);

    pDevice = (tUSBHostDevice *)ulDevice;

    //
    // This is a Standard Device OUT request.
    //
    SetupPacket.bmRequestType =
        USB_RTYPE_DIR_OUT | USB_RTYPE_STANDARD | USB_RTYPE_INTERFACE;

    //
    // Request a Device Descriptor.
    //
    SetupPacket.bRequest = USBREQ_SET_INTERFACE;

    //
    // Index is the interface to access.
    //
    SetupPacket.wIndex = ulInterface;

    //
    // wValue is the alternate setting.
    //
    SetupPacket.wValue = ulAltSetting;


    //
    // Only request the space available.
    //
    SetupPacket.wLength = 0;

    //
    // Put the setup packet in the buffer.
    //
    USBHCDControlTransfer(ulIndex, &SetupPacket, pDevice->ulAddress, 0, 0,
                          MAX_PACKET_SIZE_EP0);
}

//*****************************************************************************
//
// The internal function to see if a new schedule event should occur.
//
// This function is called by the main interrupt handler due to start of frame
// interrupts to determine if a new scheduler event should be sent to the USB
// pipe.
//
// \return None.
//
//*****************************************************************************
void
USBHostCheckPipes(unsigned int ulIndex)
{
    int iIdx;

    for(iIdx = 0; iIdx < 3; iIdx++)
    {
        //
        // Skip unused pipes.
        //
        if(g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulType == USBHCD_PIPE_UNUSED)
        {
            continue;
        }

        //
        // If the tick has expired and it has an interval then update it.
        //
        if((g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulInterval != 0) &&
           (g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulNextEventTick == g_sUSBHCD[ulIndex].ulCurrentTick))
        {
            //
            // Schedule the next event.
            //
            g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulNextEventTick +=
                g_sUSBHCD[ulIndex].USBINPipes[iIdx].ulInterval;

            //
            // If the pipe is IDLE and there is a callback, let the higher
            // level drivers know that a new transfer can be scheduled.
            //
            if((g_sUSBHCD[ulIndex].USBINPipes[iIdx].eState == PIPE_IDLE) &&
               (g_sUSBHCD[ulIndex].USBINPipes[iIdx].pfnCallback))
            {
                g_sUSBHCD[ulIndex].USBINPipes[iIdx].pfnCallback(ulIndex, IN_PIPE_HANDLE(ulIndex, iIdx),
                                                       USB_EVENT_SCHEDULER);
            }
        }
    }
}

//*****************************************************************************
//
// The internal USB host mode interrupt handler.
//
// \param ulIndex is the USB controller associated with this interrupt.
// \param ulStatus is the current interrupt status as read via a call to
// \e USBIntStatusControl().
//
// This the main USB interrupt handler called when operating in host mode.
// This handler will branch the interrupt off to the appropriate handlers
// depending on the current status of the USB controller.
//
// The two-tiered structure for the interrupt handler ensures that it is
// possible to use the same handler code in both host and OTG modes and
// means that device code can be excluded from applications that only require
// support for USB host mode operation.
//
// \return None.
//
//*****************************************************************************
void
USBHostIntHandlerInternal(unsigned int ulIndex, unsigned int ulStatus, unsigned int *endPStatus)
{
    unsigned int ulEPStatus;
    static unsigned int ulSOFDivide = 0;
    unsigned int ulEvent;
    unsigned int ulIdx;
    unsigned int epStatus;
    unsigned int epnStatus = 0;
#ifdef DMA_MODE
    unsigned int pendReg = 0;    
#endif
    //
    // Get the controller interrupt status from the wrapper registers
    // Only the lower 16bits contain EP intr data
    //
    if(endPStatus == NULL)
    {
        epStatus= 0xFFFF & ulStatus;
        ulStatus >>=16;
    }
    else
    {
        epStatus = *endPStatus;
    }
    ulStatus |= USBIntStatusControl(g_USBInstance[ulIndex].uiBaseAddr );

    if(ulStatus & USB_INTCTRL_SOF)
    {
        g_sUSBHCD[ulIndex].ulCurrentTick++;

        USBHostCheckPipes(ulIndex);
    }

    //
    // In the event of a USB VBUS error, end the session and remove power to
    // the device.
    //
    if(ulStatus & USB_INTCTRL_VBUS_ERR)
    {
        //
        // Set the VBUS error event.  We deliberately clear all other events
        // since this one means anything else that is outstanding is
        // irrelevant.
        //
        g_sUSBHCD[ulIndex].ulUSBHIntEvents = INT_EVENT_VBUS_ERR;
        return;
    }

    //
    // Babble Interrupt generated.
    //
    if(ulStatus & USB_INTCTRL_BABBLE)
    {
        g_sUSBHCD[ulIndex].ulUSBHIntEvents |= INT_EVENT_BABBLE_FAULT;
	return;
    }

    //
    // Suspend was signaled on the bus.
    //
    if(ulStatus & USB_INTCTRL_SUSPEND)
    {
    }

    //
    // Start the session.
    //
    if(ulStatus & USB_INTCTRL_SESSION)
    {
        //
        // Power the USB bus.
        //
        USBHostPwrEnable(g_USBInstance[ulIndex].uiBaseAddr );

        USBOTGSessionRequest(g_USBInstance[ulIndex].uiBaseAddr , true);
    }

    //
    // Resume was signaled on the bus.
    //
    if(ulStatus & USB_INTCTRL_RESUME)
    {
    }

    //
    // Device connected so tell the main routine to issue a reset.
    //
    if(ulStatus & USB_INTCTRL_CONNECT)
    {
        //
        // Set the connect flag and clear disconnect if it happens to be set.
        //
        g_sUSBHCD[ulIndex].ulUSBHIntEvents |= INT_EVENT_CONNECT;
        g_sUSBHCD[ulIndex].ulUSBHIntEvents &= ~INT_EVENT_DISCONNECT;

        //
        // Power the USB bus.
        //
        USBHostPwrEnable(g_USBInstance[ulIndex].uiBaseAddr );
    }

    //
    // Device was unplugged.
    //
    if(ulStatus & USB_INTCTRL_DISCONNECT)
    {
        //
        // Set the disconnect flag and clear connect if it happens to be set.
        //
        g_sUSBHCD[ulIndex].ulUSBHIntEvents |= INT_EVENT_DISCONNECT;
        g_sUSBHCD[ulIndex].ulUSBHIntEvents &= ~INT_EVENT_CONNECT;
    }

    //
    // Start of Frame was received.
    //
    if(ulStatus & USB_INTCTRL_SOF)
    {
        //
        // Increment the global Start of Frame counter.
        //
        g_ulUSBSOFCount++;

        //
        // Increment our SOF divider.
        //
        ulSOFDivide++;

        //
        // Have we counted enough SOFs to allow us to call the tick function?
        //
        if(ulSOFDivide == USB_SOF_TICK_DIVIDE)
        {
            //
            // Yes - reset the divider and call the SOF tick handler.
            //
            ulSOFDivide = 0;
            InternalUSBStartOfFrameTick(USB_SOF_TICK_DIVIDE, ulIndex);
        }
    }

    //
    // Handle end point 0 interrupts.
    //
    if(epStatus & USB_INTEP_0)
    {
        USBHCDEnumHandler(ulIndex);
    }

    
    /*
    converting the epstatus(Wrapper register data) to ulStatus( MUSB register data)
    */
    
    if(endPStatus ==  NULL)
    {
        epnStatus = 0xFF & epStatus;
        epnStatus = epnStatus | ((0xFF00 & epStatus)<<8);
    }
    else
    {
            epnStatus = epStatus;
    }

#ifdef DMA_MODE
    // Get the DMA Interrupt status
    pendReg = CppiDmaGetPendStatus(ulIndex);
#endif

    //
    // Check to see if any uDMA transfers are pending
    //
    for(ulIdx = 0; ulIdx < MAX_NUM_PIPES; ulIdx++)
    {

        
        if((epnStatus == 0) && (g_sUSBHCD[ulIndex].ulDMAPending == 0))
        {
            break;
        }

#ifdef DMA_MODE
        //
        // Check for any pending RX transaction
        //
        if((pendReg & CPDMA_RX_PENDING) && (g_sUSBHCD[ulIndex].ulDMAPending & 
                                (DMA_PEND_RECEIVE_FLAG << ulIdx)))
        {
            //    
            //Reset the pending flag
            //
            g_sUSBHCD[ulIndex].ulDMAPending &= ~(DMA_PEND_RECEIVE_FLAG << ulIdx);

            //
            //Read the completion queue
            //
            g_sUSBHCD[ulIndex].rxBuffer = (unsigned char *)dmaRxCompletion(ulIndex,  
                                        INDEX_TO_USB_EP(ulIdx + 1));
            //
            //Send an ACk
            //
             USBHostEndpointDataAck(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1));
            //
            //Set the pipe status
            //
            g_sUSBHCD[ulIndex].USBINPipes[ulIdx].eState = PIPE_DATA_READY;

            //
            //Set the RX event 
            //
            ulEvent = USB_EVENT_RX_AVAILABLE;

            //
            // Only call a handler if one is present.
            //
            if(g_sUSBHCD[ulIndex].USBINPipes[ulIdx].pfnCallback)
            {
                g_sUSBHCD[ulIndex].USBINPipes[ulIdx].pfnCallback(ulIndex,
                    IN_PIPE_HANDLE(ulIndex, ulIdx), ulEvent);
            }
        }

        //
        //Check for any pending TX transaction
        //
        if((pendReg & CPDMA_TX_PENDING) && (g_sUSBHCD[ulIndex].ulDMAPending & 
                                (DMA_PEND_TRANSMIT_FLAG << ulIdx)))
        {

            //
            // Handle the case where the pipe is writing
            //
            if(g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].eState == PIPE_WRITING)
            {    
                //
                //Reset the the pending flag
                //
                g_sUSBHCD[ulIndex].ulDMAPending &= ~(DMA_PEND_TRANSMIT_FLAG << ulIdx);

                //
                //Read the completion queue
                //
                dmaTxCompletion(ulIndex,  
                                        INDEX_TO_USB_EP(ulIdx + 1));
                //
                // Data was transmitted successfully.
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].eState = PIPE_DATA_SENT;

                //
                // Notify the pipe that its last transaction was completed.
                //
                ulEvent = USB_EVENT_TX_COMPLETE;
                
            }
        }
        
#endif       

        //
        // Check the next pipe, the first time through this will clear out
        // any interrupts dealing with endpoint zero since it was handled above.
        //
        
        epnStatus >>= 1;

        //
        // Check the status of the transmit(OUT) pipes.
        //
        if(epnStatus & 1)
        {
            //
            // Read the status of the endpoint connected to this pipe.
            //
            ulEPStatus = USBEndpointStatus(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1));

            if(ulEPStatus & USB_HOST_OUT_ERROR)
            {
                //
                // 3 failed attemps made to send packet. Device is non responsive.
                // Clear the error condition on the endpoint.
                //
                USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1),
                                           USB_HOST_OUT_ERROR);

                //
                // Data OUT failed. Flush the FIFO.
                //
                USBFIFOFlush(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1),
                                           USB_EP_HOST_OUT);

                //
                // Save the Pipes error state.
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].eState = PIPE_ERROR;

                //
                // Notify the pipe that had an error.
                //
                ulEvent = USB_EVENT_ERROR;
            }
            else if(ulEPStatus & USB_HOST_OUT_STALL)
            {
                //
                // Clear the stall condition on this endpoint pipe.
                //
                USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1),
                                           USB_HOST_OUT_STALL);

                //
                // Save the STALLED state.
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].eState = PIPE_STALLED;

                //
                // Notify the pipe that it was stalled.
                //
                ulEvent = USB_EVENT_STALL;
            }
            else
            {
                //
                // Data was transmitted successfully.
                //
                g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].eState = PIPE_DATA_SENT;

                //
                // Notify the pipe that its last transaction was completed.
                //
                ulEvent = USB_EVENT_TX_COMPLETE;
            }
            // Clear the stall condition on this endpoint pipe.
            //
            USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr,
                                       INDEX_TO_USB_EP(ulIdx + 1),
                                       ulEPStatus);


            //
            // Only call a handler if one is present.
            //
            if(g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].pfnCallback)
            {
                g_sUSBHCD[ulIndex].USBOUTPipes[ulIdx].pfnCallback(ulIndex, OUT_PIPE_HANDLE(ulIndex, ulIdx),
                                                         ulEvent);
            }
        }

        //
        // Check the status of the receive(IN) pipes.
        //
        if(epnStatus & 0x10000)
        {
            //
            // Clear the status flag for the IN Pipe.
            //
            epnStatus &= ~0x10000;

            //
            // Read the status of the endpoint connected to this pipe.
            //
            ulEPStatus = USBEndpointStatus(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1));

            if(ulEPStatus & USB_HOST_IN_ERROR)
            {
                //
                // 3 failed attemps made to receive packet. Device is non responsive.
                // Clear the error condition on the endpoint.
                //
                USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr,
                                           INDEX_TO_USB_EP(ulIdx + 1),
                                           USB_HOST_IN_ERROR);

                //
                // Data IN failed. Flush the FIFO.
                //
                USBFIFOFlush(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1),
                                           USB_EP_HOST_IN);
                //
                // Save the Pipes error state.
                //
                g_sUSBHCD[ulIndex].USBINPipes[ulIdx].eState = PIPE_ERROR;

                //
                // Notify the pipe that it was stalled.
                //
                ulEvent = USB_EVENT_ERROR;
            }
            else if(ulEPStatus & USB_HOST_IN_STALL)
            {
                //
                // Clear the stall condition on this endpoint pipe.
                //
                USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr ,
                                           INDEX_TO_USB_EP(ulIdx + 1),
                                           USB_HOST_IN_STALL);

                //
                // Save the STALLED state.
                //
                g_sUSBHCD[ulIndex].USBINPipes[ulIdx].eState = PIPE_STALLED;

                //
                // Notify the pipe that it was stalled.
                //
                ulEvent = USB_EVENT_STALL;
            }
            else
            {
                //
                // Data is available.
                //
                g_sUSBHCD[ulIndex].USBINPipes[ulIdx].eState = PIPE_DATA_READY;

                //
                // Notify the pipe that its last transaction was completed.
                //
                ulEvent = USB_EVENT_RX_AVAILABLE;
            }

            //
            // Only call a handler if one is present.
            //
            if(g_sUSBHCD[ulIndex].USBINPipes[ulIdx].pfnCallback)
            {
                g_sUSBHCD[ulIndex].USBINPipes[ulIdx].pfnCallback(ulIndex, IN_PIPE_HANDLE(ulIndex, ulIdx),
                                                        ulEvent);
            }
        }

    }

    //
    // If there is an active driver and it has a call back then call it.
    //
    if((g_sUSBHCD[ulIndex].iUSBHActiveDriver >= 0) &&
       (g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iUSBHActiveDriver]->pfnIntHandler))
    {
        g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iUSBHActiveDriver]->
            pfnIntHandler(g_sUSBHCD[ulIndex].pvDriverInstance);
    }
}

//*****************************************************************************
//
//! The USB host mode interrupt handler for controller index 0.
//!
//! This the main USB interrupt handler entry point.  This handler will branch
//! the interrupt off to the appropriate handlers depending on the current
//! status of the USB controller.   This function must be placed in the
//! interrupt table in order for the USB Library host stack to function.
//!
//! \return None.
//
//*****************************************************************************
void
USB0HostIntHandler(void)
{
    unsigned int ulStatus = 0;
    unsigned int ulIndex = 0;

#if defined (am335x_15x15) || defined(am335x) || defined(c6a811x)
    unsigned int epStatus = 0;

    ulStatus = HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_1);
    epStatus = HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_0);
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_1) = ulStatus;
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_0) = epStatus;

#ifdef DMA_MODE    
    HWREG(USBSS_BASE + USBSS_IRQ_STATUS) = 
        HWREG(USBSS_BASE + USBSS_IRQ_STATUS);
#endif

    USBHostIntHandlerInternal(ulIndex, ulStatus, &epStatus);
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_EOI) = 0;

#ifdef DMA_MODE
    HWREG(USBSS_BASE + USBSS_IRQ_EOI) = 0;
#endif

#else

    //
    // Get the control interrupt status.
    //
    
    ulStatus = HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_INTR_SRC);
  
    // Clear the Interrupts
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_INTR_SRC_CLEAR) = ulStatus;
#ifdef _TMS320C6X
    IntEventClear(g_USBInstance[ulIndex].uiInterruptNum);
#else
    IntSystemStatusClear(g_USBInstance[ulIndex].uiInterruptNum);
#endif

    //
    // Call the internal handler to process the interrupts.
    //
    USBHostIntHandlerInternal(ulIndex, ulStatus, NULL);

    // End of Interrupts
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_END_OF_INTR) = 0;
#endif

}


//*****************************************************************************
//
//! The USB host mode interrupt handler for controller index 1.
//!
//! This the main USB interrupt handler entry point.  This handler will branch
//! the interrupt off to the appropriate handlers depending on the current
//! status of the USB controller.   This function must be placed in the
//! interrupt table in order for the USB Library host stack to function.
//!
//! \return None.
//
//*****************************************************************************
void
USB1HostIntHandler(void)
{
    unsigned int ulStatus = 0;
    unsigned int ulIndex = 1;

#if defined (am335x_15x15) || defined(am335x) || defined(c6a811x)
    unsigned int epStatus = 0;

    ulStatus = HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_1);
    epStatus = HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_0);
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_1) = ulStatus;
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_STATUS_0) = epStatus;

#ifdef DMA_MODE    
    HWREG(USBSS_BASE + USBSS_IRQ_STATUS) = 
        HWREG(USBSS_BASE + USBSS_IRQ_STATUS);
#endif

    USBHostIntHandlerInternal(ulIndex, ulStatus, &epStatus);
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_IRQ_EOI) = 0;

#ifdef DMA_MODE
    HWREG(USBSS_BASE + USBSS_IRQ_EOI) = 0;
#endif

#else

    //
    // Get the control interrupt status.
    //
    
    ulStatus = HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_INTR_SRC);
  
    // Clear the Interrupts
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_INTR_SRC_CLEAR) = ulStatus;
#ifdef _TMS320C6X
    IntEventClear(g_USBInstance[ulIndex].uiInterruptNum);
#else
    IntSystemStatusClear(g_USBInstance[ulIndex].uiInterruptNum);
#endif

    //
    // Call the internal handler to process the interrupts.
    //
    USBHostIntHandlerInternal(ulIndex, ulStatus, NULL);

    // End of Interrupts
    HWREG(g_USBInstance[ulIndex].uiSubBaseAddr + USB_0_END_OF_INTR) = 0;    
#endif

}

//*****************************************************************************
//
//! This function opens the class driver.
//!
//! \param ulIndex specifies which USB controller to use.
//! \param ulDeviceNum is the device number for the driver to load.
//!
//! This function opens the driver needed based on the class value found in
//! the device's interface descriptor.
//!
//! \return This function returns -1 if no driver is found, or it returns the
//! index of the driver found in the list of host class drivers.
//
//*****************************************************************************
static int
USBHCDOpenDriver(unsigned int ulIndex, unsigned int ulDeviceNum)
{
    int iDriver;
    unsigned int ulClass;
    tInterfaceDescriptor *pInterface;

    ASSERT(ulIndex == 0);

    //
    // Get the interface descriptor.
    //
    pInterface = USBDescGetInterface(g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor,
                                     g_sUSBHCD[ulIndex].USBDevice[0].ulInterface,
                                     USB_DESC_ANY);

    //
    // Read the interface class.
    //
    ulClass = pInterface->bInterfaceClass;

    //
    // Search through the Host Class driver list for the devices class.
    //
    for(iDriver = 0; iDriver < g_sUSBHCD[ulIndex].ulNumClassDrivers; iDriver++)
    {
        //
        // If a driver was found call the open for this driver and save which
        // driver is in use.
        //
        if(g_sUSBHCD[ulIndex].pClassDrivers[iDriver]->ulInterfaceClass == ulClass)
        {
            //
            // Call the open function for the class driver.
            //
            g_sUSBHCD[ulIndex].pvDriverInstance = g_sUSBHCD[ulIndex].pClassDrivers[iDriver]->pfnOpen(
                &g_sUSBHCD[ulIndex].USBDevice[0], ulDeviceNum);

            //
            // If the driver was successfully loaded then break out of the
            // loop.
            //
            if(g_sUSBHCD[ulIndex].pvDriverInstance != 0)
            {
                break;
            }
        }
    }

    //
    // If no drivers were found then return -1 to indicate an invalid
    // driver instance.
    //
    if(iDriver == g_sUSBHCD[ulIndex].ulNumClassDrivers)
    {
        //
        // Send an unknown connection event.
        //
        SendUnknownConnect(ulIndex, ulClass,  ulDeviceNum);

        //
        // Indicate that no driver was found.
        //
        iDriver = -1;
    }

    return(iDriver);
}

//*****************************************************************************
//
// This function will send an event to a registered event driver.
//
// \param ulIndex is one of the USB_EVENT_* values.
//
// This function is only used internally to the USB library and will check
// if an event driver is registered and send on the event.
//
// Note: This function should not be called outside of the USB library.
//
// \return None.
//
//*****************************************************************************
void
InternalUSBHCDSendEvent(unsigned int ulIndex, unsigned int ulEvent)
{
    //
    // Make sure that an event driver has been registered.
    //
    if((g_sUSBHCD[ulIndex].iEventDriver != -1) &&
       (g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iEventDriver]->pfnIntHandler))
    {
        //
        // Send an event to the application.
        //
        g_sUSBHCD[ulIndex].EventInfo.ulEvent = ulEvent;

        g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iEventDriver]->pfnIntHandler(
           &g_sUSBHCD[ulIndex].EventInfo);
    }
}


//*****************************************************************************
//
// This function handles the necessary clean up for device disconnect.
//
// \param ulIndex is the device number for the device that was disconnected.
//
// This function handles all of the necessary clean up after a device
// disconnect has been detected by the stack.  This includes calling back the
// appropriate driver if necessary.
//
// \return None.
//
//*****************************************************************************
static void
USBHCDDeviceDisconnected(unsigned int ulIndex, unsigned int ulInstance)
{
    ASSERT(ulIndex == 0);

    if(g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor)
    {
        //
        // Invalidate the configuration descriptor.
        //
        g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor = 0;
    }

    //
    // Reset the max packet size so that this will be re-read from new devices.
    //
    g_sUSBHCD[ulIndex].USBDevice[0].DeviceDescriptor.bMaxPacketSize0 = 0;

    //
    // No longer have a device descriptor.
    //
    g_sUSBHCD[ulIndex].USBDevice[0].DeviceDescriptor.bLength = 0;

    //
    // No longer addressed.
    //
    g_sUSBHCD[ulIndex].USBDevice[0].ulAddress = 0;

    //
    // If this was an active driver then close it out.
    //
    if(g_sUSBHCD[ulIndex].iUSBHActiveDriver >= 0)
    {
        //
        // Call the driver Close entry point.
        //
        g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iUSBHActiveDriver]->
            pfnClose((void *)ulInstance);

        //
        // No active driver now present.
        //
        g_sUSBHCD[ulIndex].iUSBHActiveDriver = -1;
        g_sUSBHCD[ulIndex].pvDriverInstance = 0;
    }
    else
    {
        if((g_sUSBHCD[ulIndex].iEventDriver != -1) &&
           (g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iEventDriver]->pfnIntHandler))
        {
            //
            // Send the generic disconnect event.
            //
            g_sUSBHCD[ulIndex].EventInfo.ulEvent = USB_EVENT_DISCONNECTED;

            g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iEventDriver]->pfnIntHandler(
               &g_sUSBHCD[ulIndex].EventInfo);

            //
            // Reset the class and the instance.
            //
            g_sUSBHCD[ulIndex].ulClass = USB_CLASS_EVENTS;
            g_sUSBHCD[ulIndex].EventInfo.ulInstance = 0;
        }
    }
    
}

//*****************************************************************************
//
//! This function is the main routine for the Host Controller Driver.
//!
//! This function is the main routine for the host controller driver, and must
//! be called periodically by the main application outside of a callback
//! context.  This allows for a simple cooperative system to access the the
//! host controller driver interface without the need for an RTOS.  All time
//! critical operations are handled in interrupt context but all blocking
//! operations are run from the this function to allow them to block and wait
//! for completion without holding off other interrupts.
//!
//! \return None.
//
//*****************************************************************************

void
USBHCDMain(unsigned int ulIndex, unsigned int ulInstance)
{
    unsigned int ulIntState;
    tUSBHDeviceState eOldState;

    //
    // Save the old state to detect changes properly.
    //
    eOldState = g_sUSBHCD[ulIndex].eDeviceState[0];
     //
     // Perform this fixup with interrupts disabled to prevent race
     // conditions related to g_sUSBHCD[ulIndex].ulUSBHIntEvents.
     //
#ifdef _TMS320C6X
        ulIntState = IntGlobalDisable();
#else
        ulIntState = IntDisable();
#endif

    //
    // Fix up the state if any important interrupt events occurred.
    //
    if(g_sUSBHCD[ulIndex].ulUSBHIntEvents)
    {
        if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_POWER_FAULT)
        {
            //
            // A power fault has occurred so notify the application.
            //
            if((g_sUSBHCD[ulIndex].iEventDriver != -1) &&
               (g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex]
                       .iEventDriver]->pfnIntHandler))
            {
                //
                // Send the generic power fault event.
                //
                g_sUSBHCD[ulIndex].EventInfo.ulEvent = USB_EVENT_POWER_FAULT;

                g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex]
                    .iEventDriver]->pfnIntHandler(&g_sUSBHCD[ulIndex].EventInfo);
            }

            g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_POWER_FAULT;
        }
        else if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_VBUS_ERR)
        {
            //
            // A VBUS error has occurred.  This event trumps connect and
            // disconnect since it will cause a controller reset.
            //
            g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_VBUS_ERROR;
        }
        else if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_BABBLE_FAULT)
        {
            //
            // A VBUS error has occurred.  This event trumps connect and
            // disconnect since it will cause a controller reset.
            //
            g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_BABBLE_ERROR;
        }
        else
        {
            //
            // Has a device connected?
            //
            if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_CONNECT)
            {
                g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_RESET;
            }
            else
            {
                //
                // Has a device disconnected?
                //
                if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & INT_EVENT_DISCONNECT)
                {
                    g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_DISCONNECTED;
                }
            }
        }

        //
        // Clear the flags.
        //
        g_sUSBHCD[ulIndex].ulUSBHIntEvents = 0;

        //
        // Turn interrupts back on if they were on when we were called.
        //
    }
#ifdef _TMS320C6X
        IntGlobalRestore(ulIntState);
#else
        IntEnable(ulIntState);
#endif

    switch(g_sUSBHCD[ulIndex].eDeviceState[0])
    {
        //
        // There was a power fault condition so shut down and wait for the
        // application to re-initialized the system.
        //
        case HCD_POWER_FAULT:
        {
            break;
        }

        //
        // There was a VBUS error so handle it.
        //
        case HCD_VBUS_ERROR:
        {
            //
            // Disable USB interrupts.
            //
#ifdef _TMS320C6X
            /* No DSP API to disable USB0 event */
#else
            IntSystemDisable(g_USBInstance[ulIndex].uiInterruptNum);
#endif
           
            //
            // If there was a device in any state of connection then indicate
            // that it has been disconnected.
            //
            if((eOldState != HCD_IDLE) && (eOldState != HCD_POWER_FAULT))
            {
                //
                // Handle device disconnect.
                //
                USBHCDDeviceDisconnected(ulIndex, ulInstance);
            }

            //
            // Reset the controller.
            //
            //SysCtlPeripheralReset(SYSCTL_PERIPH_USB0);
            USBReset(g_USBInstance[ulIndex].uiSubBaseAddr);

            //
            // Wait for 100ms before trying to re-power the device.
            //            
            Sysdelay(100);

            //
            // Re-initialize the HCD.
            //
            USBHCDInitInternal(ulIndex, g_sUSBHCD[ulIndex].pvPool, 
                                        g_sUSBHCD[ulIndex].ulPoolSize);

#ifdef _TMS320C6X
            /* No DSP API to disable USB0 event */
#else
            IntSystemEnable(g_USBInstance[ulIndex].uiInterruptNum);
#endif
             break;
        }
        //
        // Trigger a reset to the connected device.
        //
        case HCD_DEV_RESET:
        {
            //
            // Trigger a Reset.
            //
            USBHCDReset(ulIndex);

            //
            // The state moves to connected but not configured.
            //
            g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_CONNECTED;

            break;
        }
        //
        // Device connection has been established now start enumerating
        // the device.
        //
        case HCD_DEV_CONNECTED:
        {
            //
            // First get the speed of the device
            //
            g_sUSBHCD[ulIndex].USBDevice[0].ulDeviceSpeed = 
                                           USBHCDGetSpeed(ulIndex);
            
            //
            // First check if we have read the device descriptor at all
            // before proceeding.
            //
            if(g_sUSBHCD[ulIndex].USBDevice[0].DeviceDescriptor.bLength == 0)
            {
                //
                // Initialize a request for the device descriptor.
                //
                if(USBHCDGetDeviceDescriptor(ulIndex, &g_sUSBHCD[ulIndex]
                                                        .USBDevice[0]) == 0)
                {
                    //
                    // If the device descriptor cannot be read then the device
                    // will be treated as unknown.
                    //
                    g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_ERROR;

                    //
                    // Send an unknown connection event.
                    //
                    SendUnknownConnect(ulIndex, 0,  ulInstance);
                }
            }
            //
            // If we have the device descriptor then move on to setting
            // the address of the device.
            //
            else if(g_sUSBHCD[ulIndex].USBDevice[0].ulAddress == 0)
            {
                //
                // Send the set address command.
                //
                USBHCDSetAddress(ulIndex, 1);

                //
                // Save the address.
                //
                g_sUSBHCD[ulIndex].USBDevice[0].ulAddress = 1;

                //
                // Move on to the addressed state.
                //
                g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_ADDRESSED;
            }
            break;
        }
        case HCD_DEV_ADDRESSED:
        {
            //
            // First check if we have read the configuration descriptor.
            //
            if (g_sUSBHCD[ulIndex].USBDevice[0].pConfigDescriptor == 0)
            {
                //
                // Initialize a request for the device descriptor.
                //
                if(USBHCDGetConfigDescriptor(ulIndex, &g_sUSBHCD[ulIndex]
                                                        .USBDevice[0]) == 0)
                {
                    //
                    // If the device descriptor cannot be read then the device
                    // will be treated as unknown.
                    //
                    g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_ERROR;

                    //
                    // Send an unknown connection event.
                    //
                    SendUnknownConnect(ulIndex, 0, ulInstance);
                }
            }
            //
            // Now have addressed and received the device configuration,
            // so get ready to set the device configuration.
            //
            else
            {
                //
                // Use the first configuration to set the device
                // configuration.
                //
                USBHCDSetConfig(ulIndex, (unsigned int)&g_sUSBHCD[ulIndex]
                                                            .USBDevice[0], 1);

                //
                // Move on to the configured state.
                //
                g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_CONFIGURED;

                //
                // Open the driver for device 0.
                //
                g_sUSBHCD[ulIndex].iUSBHActiveDriver = 
                        USBHCDOpenDriver(ulIndex, ulInstance);
            }
            break;
        }
        //
        // The device was making a request and is now complete.
        //
        case HCD_DEV_REQUEST:
        {
            g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_DEV_CONNECTED;
            break;
        }
        //
        // The strings are currently not accessed.
        //
        case HCD_DEV_GETSTRINGS:
        {
            break;
        }
        //
        // Basically Idle at this point.
        //
        case HCD_DEV_DISCONNECTED:
        {
            //
            // Handle device disconnect.
            //
            USBHCDDeviceDisconnected(ulIndex, ulInstance);

            //
            // Return to the Idle state.
            //
            g_sUSBHCD[ulIndex].eDeviceState[0] = HCD_IDLE;
            break;
        }

        //
        // Connection and enumeration is complete so allow this function
        // to exit.
        //
        case HCD_DEV_CONFIGURED:
        {
            break;
        }
        case HCD_BABBLE_ERROR:
        {
             USBHCDTerm(ulIndex);
             USBHCDDeviceDisconnected(ulIndex, ulInstance);
             UsbPhyOff(ulIndex);
             USBReset(g_USBInstance[ulIndex].uiSubBaseAddr);
             g_sUSBHCD[ulIndex].EventInfo.ulEvent = USB_EVENT_BABBLE_ERROR;
             g_sUSBHCD[ulIndex].EventInfo.ulInstance = ulIndex;
             g_sUSBHCD[ulIndex].pClassDrivers[g_sUSBHCD[ulIndex].iEventDriver]->pfnIntHandler(
                  &g_sUSBHCD[ulIndex].EventInfo);             	
             break;
        }
	
        //
        // Poorly behaving device are in limbo in this state until removed.
        //
        case HCD_DEV_ERROR:
        {
            //
            // If there was a device in any state of connection then indicate
            // that it has been disconnected.
            //
            if((eOldState != HCD_IDLE) && (eOldState != HCD_POWER_FAULT))
            {
                //
                // Handle device disconnect.
                //
                USBHCDDeviceDisconnected(ulIndex, ulInstance);
            }

            //
            // Reset the controller.
            //
            USBReset(g_USBInstance[ulIndex].uiSubBaseAddr);

            //
            // Wait for 100ms before trying to re-power the device.
            //            
            Sysdelay(100);

            //
            // Re-initialize the HCD.
            //
            USBHCDInitInternal(ulIndex, g_sUSBHCD[ulIndex].pvPool, 
                                        g_sUSBHCD[ulIndex].ulPoolSize);
            break;
        }
        default:
        {
            break;
        }
    }
    g_sUSBHCD[ulIndex].ulConnectRetry = g_ulConnectRetry[ulIndex];
}

//*****************************************************************************
//
//! This function completes a control transaction to a device.
//!
//! \param ulIndex is the controller index to use for this transfer.
//! \param pSetupPacket is the setup request to be sent.
//! \param ulDevAddress is the address of the device for this request.
//! \param pData is the data to send for OUT requests or the receive buffer
//! for IN requests.
//! \param ulSize is the size of the buffer in pData.
//! \param ulMaxPacketSize is the maximum packet size for the device for this
//! request.
//!
//! This function handles the state changes necessary to send a control
//! transaction to a device.  This function should not be called from within
//! an interrupt callback as it is a blocking function.
//!
//! \return The number of bytes of data that were sent or received as a result
//! of this request.
//
//*****************************************************************************
unsigned int
USBHCDControlTransfer(unsigned int ulIndex, tUSBRequest *pSetupPacket,
                      unsigned int ulDevAddress, unsigned char *pData,
                      unsigned int ulSize, unsigned int ulMaxPacketSize)
{
    unsigned int ulRemaining;
    unsigned int ulDataSize;
    unsigned int ulTimer = 0;
    unsigned int retStatus = 1;

    ASSERT(g_sUSBHEP0State[ulIndex].eState == EP0_STATE_IDLE);
    ASSERT(ulIndex == 0);

    //
    // Initialize the state of the data for this request.
    //
    g_sUSBHEP0State[ulIndex].pData = pData;
    g_sUSBHEP0State[ulIndex].ulBytesRemaining = ulSize;
    g_sUSBHEP0State[ulIndex].ulDataSize = ulSize;

    //
    // Set the maximum packet size.
    //
    g_sUSBHEP0State[ulIndex].ulMaxPacketSize = ulMaxPacketSize;

    //
    // Save the current address.
    //
    g_sUSBHEP0State[ulIndex].ulDevAddress = ulDevAddress;

    //
    // Set the address the host will used to communicate with the device.
    //
    USBHostAddrSet(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 
            g_sUSBHEP0State[ulIndex].ulDevAddress, USB_EP_HOST_OUT);

    //
    // Put the data in the correct FIFO.
    //
    USBEndpointDataPut(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 
                    (unsigned char *)pSetupPacket, sizeof(tUSBRequest));

    //
    // If this is an IN request, change to that state.
    //
    if(pSetupPacket->bmRequestType & USB_RTYPE_DIR_IN)
    {
        g_sUSBHEP0State[ulIndex].eState = EP0_STATE_SETUP_IN;
    }
    else
    {
        //
        // If there is no data then this is not an OUT request.
        //
        if(ulSize != 0)
        {
            //
            // Since there is data, this is an OUT request.
            //
            g_sUSBHEP0State[ulIndex].eState = EP0_STATE_SETUP_OUT;
        }
        else
        {
            //
            // Otherwise this request has no data and just a status phase.
            //
            g_sUSBHEP0State[ulIndex].eState = EP0_STATE_STATUS_IN;
        }
    }

    //
    // Send the Setup packet.
    //
    USBEndpointDataSend(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 
                            USB_TRANS_SETUP);

    if(USB_TIMEOUT_DISABLE!=g_sUSBHCD[ulIndex].USBHTimeOut.Value.slEP0)
    {
        ulTimer = g_sUSBHCD[ulIndex].USBHTimeOut.Value.slEP0;
    }

    //StartTimer(ulTimer);
    timer_interval(&UsbControlTransferTimer[ulIndex], ulTimer);
    timer_enable(&UsbControlTransferTimer[ulIndex]);

    //
    // Block until endpoint 0 returns to the IDLE state.
    //
    while((g_sUSBHEP0State[ulIndex].eState != EP0_STATE_IDLE)&&(!timer_tick(&UsbControlTransferTimer[ulIndex])/*!IsTimerElapsed()*/))
    {
        if(g_sUSBHEP0State[ulIndex].eState == EP0_STATE_ERROR)
        {
            USBHCDTxAbort(ulIndex, 0);
            USBHCDRxAbort(ulIndex, 0);
            retStatus = 0;
            break;
        }

        //
        // If we aborted the transfer due to an error, tell the caller
        // that no bytes were transferred.
        //
        if(g_sUSBHCD[ulIndex].ulUSBHIntEvents & (INT_EVENT_VBUS_ERR 
                                        | INT_EVENT_DISCONNECT|INT_EVENT_BABBLE_FAULT))
        {
            USBHCDTxAbort(ulIndex, 0);
            USBHCDRxAbort(ulIndex, 0);
            retStatus = 0;
            break;
        }
    }

    timer_disable(&UsbControlTransferTimer[ulIndex]);
    //StopTimer();

    //
    // Calculate and return the number of bytes that were sent or received.
    // The extra copy into local variables is required to prevent some
    // compilers from warning about undefined order of volatile access.
    //
    if(g_sUSBHEP0State[ulIndex].eState == EP0_STATE_IDLE)
    {
        ulDataSize = g_sUSBHEP0State[ulIndex].ulDataSize;
        ulRemaining = g_sUSBHEP0State[ulIndex].ulBytesRemaining;
        retStatus = (ulDataSize - ulRemaining);
    }
    else
    {
        retStatus = 0;
        g_sUSBHCD[ulIndex].USBHTimeOut.Status.slEP0 = 1;
    }

    return retStatus;
}

//*****************************************************************************
//
// This is the endpoint 0 interrupt handler.
//
// \return None.
//
//*****************************************************************************
static void
USBHCDEnumHandler(unsigned int ulIndex)
{
    unsigned int ulEPStatus;
    unsigned int ulDataSize;

    //
    // Get the end point 0 status.
    //
    ulEPStatus = USBEndpointStatus(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0);

    //
    // If there was an error then go to the error state.
    //
    if(ulEPStatus == USB_HOST_EP0_ERROR)
    {
        //
        // Clear this status indicating that the status packet was
        // received.
        //
        USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr, 
                                    USB_EP_0, USB_HOST_EP0_ERROR);
        USBFIFOFlush(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 0);

        //
        // Just go back to the idle state.
        //
        g_sUSBHEP0State[ulIndex].eState = EP0_STATE_ERROR;

        return;
    }

    switch(g_sUSBHEP0State[ulIndex].eState)
    {
        //
        // Handle the status state, this is a transitory state from
        // USB_STATE_TX or USB_STATE_RX back to USB_STATE_IDLE.
        //
        case EP0_STATE_STATUS:
        {
            //
            // Handle the case of a received status packet.
            //
            if(ulEPStatus & (USB_HOST_EP0_RXPKTRDY | USB_HOST_EP0_STATUS))
            {
                //
                // Clear this status indicating that the status packet was
                // received.
                //
                USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr, 
                                            USB_EP_0, (USB_HOST_EP0_RXPKTRDY |
                                            USB_HOST_EP0_STATUS));
            }

            //
            // Just go back to the idle state.
            //
            g_sUSBHEP0State[ulIndex].eState = EP0_STATE_IDLE;

            break;
        }

        //
        // This state triggers a STATUS IN request from the device.
        //
        case EP0_STATE_STATUS_IN:
        {
            //
            // Generate an IN request from the device.
            //
            USBHostRequestStatus(g_USBInstance[ulIndex].uiBaseAddr );

            //
            // Change to the status phase and wait for the response.
            //
            g_sUSBHEP0State[ulIndex].eState =  EP0_STATE_STATUS;

            break;
        }

        //
        // In the IDLE state the code is waiting to receive data from the host.
        //
        case EP0_STATE_IDLE:
        {
            break;
        }

        //
        // Data is still being sent to the host so handle this in the
        // EP0StateTx() function.
        //
        case EP0_STATE_SETUP_OUT:
        {
            //
            // Send remaining data if necessary.
            //
            USBHCDEP0StateTx(ulIndex);

            break;
        }

        //
        // Handle the receive state for commands that are receiving data on
        // endpoint 0.
        //
        case EP0_STATE_SETUP_IN:
        {
            //
            // Generate a new IN request to the device.
            //
            USBHostRequestIN(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0);

            //
            // Proceed to the RX state to receive the requested data.
            //
            g_sUSBHEP0State[ulIndex].eState =  EP0_STATE_RX;

            break;
        }

        //
        // The endponit remains in this state until all requested data has
        // been received.
        //
        case EP0_STATE_RX:
        {
            //
            // There was a stall on endpoint 0 so go back to the idle state
            // as this command has been terminated.
            //
            if(ulEPStatus & USB_HOST_EP0_RX_STALL)
            {
                g_sUSBHEP0State[ulIndex].eState = EP0_STATE_IDLE;

                //
                // Clear the stalled state on endpoint 0.
                //
                USBHostEndpointStatusClear(g_USBInstance[ulIndex].uiBaseAddr, 
                                            USB_EP_0, ulEPStatus);
                break;
            }
            //
            // Set the number of bytes to get out of this next packet.
            //
            if(g_sUSBHEP0State[ulIndex].ulBytesRemaining > MAX_PACKET_SIZE_EP0)
            {
                //
                // Don't send more than EP0_MAX_PACKET_SIZE bytes.
                //
                ulDataSize = MAX_PACKET_SIZE_EP0;
            }
            else
            {
                //
                // There was space so send the remaining bytes.
                //
                ulDataSize = g_sUSBHEP0State[ulIndex].ulBytesRemaining;
            }

            if(ulDataSize != 0)
            {
                //
                // Get the data from the USB controller end point 0.
                //
                USBEndpointDataGet(g_USBInstance[ulIndex].uiBaseAddr , 
                                    USB_EP_0, g_sUSBHEP0State[ulIndex].pData,
                                           &ulDataSize);
            }

            //
            // Advance the pointer.
            //
            g_sUSBHEP0State[ulIndex].pData += ulDataSize;

            //
            // Decrement the number of bytes that are being waited on.
            //
            g_sUSBHEP0State[ulIndex].ulBytesRemaining -= ulDataSize;

            //
            // Need to ack the data on end point 0 in this case
            // without setting data end.
            //
            USBDevEndpointDataAck(g_USBInstance[ulIndex].uiBaseAddr, 
                                    USB_EP_0, false);

            //
            // If there was not more than the maximum packet size bytes of data
            // the this was a short packet and indicates that this transfer is
            // complete.  If there were exactly g_sUSBHEP0State[ulIndex].ulMaxPacketSize
            // remaining then there still needs to be null packet sent before
            // this transfer is complete.
            //
            if((ulDataSize < g_sUSBHEP0State[ulIndex].ulMaxPacketSize) ||
               (g_sUSBHEP0State[ulIndex].ulBytesRemaining == 0))
            {
                //
                // Return to the idle state.
                //
                g_sUSBHEP0State[ulIndex].eState =  EP0_STATE_STATUS;

                //
                // No more data.
                //
                g_sUSBHEP0State[ulIndex].pData = 0;

                //
                // Send a null packet to acknowledge that all data was received.
                //
                USBEndpointDataSend(g_USBInstance[ulIndex].uiBaseAddr, 
                                        USB_EP_0, USB_TRANS_STATUS);
            }
            else
            {
                //
                // Request more data.
                //
                USBHostRequestIN(g_USBInstance[ulIndex].uiBaseAddr, USB_EP_0);
            }
            break;
        }

        //
        // The device stalled endpoint zero so check if the stall needs to be
        // cleared once it has been successfully sent.
        //
        case EP0_STATE_STALL:
        {
            //
            // Reset the global end point 0 state to IDLE.
            //
            g_sUSBHEP0State[ulIndex].eState = EP0_STATE_IDLE;

            break;
        }

        //
        // Halt on an unknown state, but only in DEBUG builds.
        //
        default:
        {
            ASSERT(0);
            break;
        }
    }
}

//*****************************************************************************
//
// This internal function handles sending data on endpoint 0.
//
// \return None.
//
//*****************************************************************************
static void
USBHCDEP0StateTx(unsigned int ulIndex)
{
    unsigned int ulNumBytes;
    unsigned char *pData;
    
    //
    // In the TX state on endpoint 0.
    //
    g_sUSBHEP0State[ulIndex].eState = EP0_STATE_SETUP_OUT;

    //
    // Set the number of bytes to send this iteration.
    //
    ulNumBytes = g_sUSBHEP0State[ulIndex].ulBytesRemaining;

    //
    // Limit individual transfers to 64 bytes.
    //
    if(ulNumBytes > 64)
    {
        ulNumBytes = 64;
    }

    //
    // Save the pointer so that it can be passed to the USBEndpointDataPut()
    // function.
    //
    pData = (unsigned char *)g_sUSBHEP0State[ulIndex].pData;

    //
    // Advance the data pointer and counter to the next data to be sent.
    //
    g_sUSBHEP0State[ulIndex].ulBytesRemaining -= ulNumBytes;
    g_sUSBHEP0State[ulIndex].pData += ulNumBytes;

    //
    // Put the data in the correct FIFO.
    //
    USBEndpointDataPut(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 
                        pData, ulNumBytes);

    //
    // If this is exactly 64 then don't set the last packet yet.
    //
    if(ulNumBytes == 64)
    {
        //
        // There is more data to send or exactly 64 bytes were sent, this
        // means that there is either more data coming or a null packet needs
        // to be sent to complete the transaction.
        //
        USBEndpointDataSend(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 
                                USB_TRANS_OUT);
    }
    else
    {
        //
        // Send the last bit of data.
        //
        USBEndpointDataSend(g_USBInstance[ulIndex].uiBaseAddr , USB_EP_0, 
                                USB_TRANS_OUT);

        //
        // Now go to the status state and wait for the transmit to complete.
        //
        g_sUSBHEP0State[ulIndex].eState = EP0_STATE_STATUS_IN;
    }
}

//*****************************************************************************
//
// This internal function handles Aborts Tx by flusing the EP FIFOs and DMA(when enabled).
//
// \return None.
//
//*****************************************************************************
static unsigned int
USBHCDTxAbort(unsigned int ulIndex,  unsigned int endPoint) 
{
 
   USBFIFOFlush(g_USBInstance[ulIndex].uiBaseAddr,
                                                   endPoint, USB_EP_HOST_OUT);
   USBFIFOFlush(g_USBInstance[ulIndex].uiBaseAddr,
                                                   endPoint, USB_EP_HOST_OUT);
   if (endPoint != USB_EP_0) 
    {   
        #ifdef DMA_MODE
            //
            // Initite Tx channel Teardown 
            // : broadly there are 2 sets of actions - On the musb Controller side
            // and then on the CPPI 4.1 DMA side 
            // 	
            Cppi41DmaInitTddPool( ulIndex );
            USBDmaTxChDisable(g_USBInstance[ulIndex].uiBaseAddr, endPoint);
	
            Cppi41DmaTxChTeardown(ulIndex, endPoint);
            USBHostTxFifoFlush( g_USBInstance[ulIndex].uiBaseAddr, endPoint );
		
        #endif
    }
return 1;
}

//*****************************************************************************
//
// This internal function handles Aborts Rx by flusing the EP FIFOs and DMA(when enabled).
//
// \return None.
//
//*****************************************************************************
static unsigned int
USBHCDRxAbort(unsigned int ulIndex,  unsigned int endPoint) 
{   

    USBHostAutoReqClear(g_USBInstance[ulIndex].uiBaseAddr,
                                                   endPoint);
    if (endPoint != USB_EP_0)
#ifdef DMA_MODE
    { 
        //
        // Initite Rx channel Teardown 
        // : broadly there are 2 sets of actions - On the musb Controller side
        // and then on the CPPI 4.1 DMA side 
        // 	
        
        /*	CPPI DMA issue  Teardown hang: Frequent teardowns 
		cause controller to hang. Solution/workaround: 250 micro seconds delay  
		to be added to RxDMA teardown path.*/	
        /* 1ms delay*/         
        Sysdelay(1);
        Cppi41DmaInitTddPool( ulIndex );
        USBRxChAbort(g_USBInstance[ulIndex].uiBaseAddr, endPoint);
	
        Cppi41DmaRxChTeardown(ulIndex, endPoint);
        /* Clear auto request register once done */
        // clear auto request register
	
        // if( host ) 
        // HWREG(usbInstance->otgBaseAddress + USB_1_RX_MODE_AUTO_REQ_REG_OFFSET)|= 0x00000000;
        USBHostAutoReqClear(g_USBInstance[ulIndex].uiBaseAddr,
                                                   endPoint);	

    }    
 else
#endif
 {
    
   USBHostRequestINClear(g_USBInstance[ulIndex].uiBaseAddr,
                                                   endPoint);

   USBFIFOFlush(g_USBInstance[ulIndex].uiBaseAddr,
                                                   endPoint, USB_EP_HOST_IN);
   USBFIFOFlush(g_USBInstance[ulIndex].uiBaseAddr,
                                                   endPoint, USB_EP_HOST_IN);

   USBHostAutoReqSet(g_USBInstance[ulIndex].uiBaseAddr,
 
                                                   endPoint);
}                                                   
return 1;
}

//*****************************************************************************
//
// This internal function trys to re-establish connenction with the device.
//
// \return None.
//
//*****************************************************************************
static unsigned int
USBHCDRetryConnect(unsigned int ulIndex) 
{
    //
    // Trigger a Reset.
    //
    USBHCDReset(ulIndex);

    if(USBHCDGetDeviceDescriptor(ulIndex, &g_sUSBHCD[ulIndex].USBDevice[0]) == 0)
    {
        return 0;
    }
    return 1;
}

//*****************************************************************************
//
// This function sets the timeout value applicable communication types.
// This API should be called after 'USBHCDInit' call.
//
// \return None.
//
//*****************************************************************************
void
USBHCDTimeOutHook(unsigned int ulIndex, tUSBHTimeOut **USBHTimeOut)
{
   (*USBHTimeOut) = &(g_sUSBHCD[ulIndex].USBHTimeOut);
}

//*****************************************************************************
//
//! This function returns the speed of the device connected on the bus.
//! 
//! \param ulIndex specifies which USB controller to use.
//!
//! This function calls the HCD lower layer function to return the speed
//! of the connected device. High speed devices get detected as high speed 
//! only after the 2nd reset and chirp sequence. Till that time they report FS 
//!
//! \return device speed as unsigned integer.
//
//*****************************************************************************

unsigned int
USBHCDGetSpeed(unsigned int ulIndex)
{
   ASSERT(ulIndex == 0);
	
   //
   // Call the lower Abstraction layer speed get routine
   //
   return( USBHostSpeedGet( g_USBInstance[ulIndex].uiBaseAddr ) );
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
