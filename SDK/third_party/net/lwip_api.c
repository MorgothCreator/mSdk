/*
 * lwip_api.c
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */
#include "lwip_api.h"

#include "clk/clk_cpsw.h"
#include "pin_mux_cpsw.h"
#include "aintc/aintc_cpsw.h"
#include "driver/cpu.h"

#ifdef UseLwip
extern new_uart* DebugCom;



/*
** Displays the IP addrss on the UART Console
*/
static void IpAddrDisplay(unsigned int ipAddr)
{
    unsigned char byte;
    int cnt;

    for(cnt = 0; cnt <= LEN_IP_ADDR - 1; cnt++)
    {
        byte = (ipAddr >> (cnt * 8)) & 0xFF;

        if(cnt)
        {
            UARTPuts(DebugCom, ".", -1);
        }

        UARTPutNum(DebugCom, (int)byte);
    }
}

/*
** Interrupt Handler for Core 0 Receive interrupt
*/
void CPSWCore0RxIsr(void)
{
	//CPUirqd();
	lwIPRxIntHandler(0);
	//CPUirqe();
}

/*
** Interrupt Handler for Core 0 Transmit interrupt
*/
void CPSWCore0TxIsr(void)
{
	//CPUirqd();
    lwIPTxIntHandler(0);
    //CPUirqe();
}


/**
 * \brief   This function returns the MAC address for the EVM
 *
 * \param   addrIdx    the MAC address index.
 * \param   macAddr    the Pointer where the MAC address shall be stored
 *     'addrIdx' can be either 0 or 1
 *
 * \return  None.
 */
void EVMMACAddrGet(unsigned int addrIdx, unsigned char *macAddr)
{
    macAddr[0] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx))
                   >> 8) & 0xFF;
    macAddr[1] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx)))
                  & 0xFF;
    macAddr[2] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 24) & 0xFF;
    macAddr[3] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 16) & 0xFF;
    macAddr[4] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 8) & 0xFF;
    macAddr[5] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx)))
                  & 0xFF;
}


void lan_interface_init(unsigned long ip)
{
	UARTPuts(DebugCom, "Initialize the LWIP library.\n\r" , -1);
    unsigned int ipAddr;
    LWIP_IF lwipIfPort1, lwipIfPort2;
    CPSWPinMuxSetup(1);
    CPSWClkEnable();
    /* Chip configuration RGMII selection */
    EVMPortMIIModeSelect();
    /* Get the MAC address */
    EVMMACAddrGet(0, lwipIfPort1.macArray);
    EVMMACAddrGet(1, lwipIfPort2.macArray);
    AintcCPSWIntrSetUp();
    UARTPuts(DebugCom, "StarterWare Ethernet Application. Access the embedded"
             " web page using http://<ip address assigned>/index.html"
             " via a web browser. \n\r\n\r" , -1);

    UARTPuts(DebugCom, "Acquiring IP Address for Port 1... \n\r" , -1);

//#define STATIC_IP_ADDRESS_PORT1 192<<24 | 168<<16 | 0<<8 | 10
//#define STATIC_IP_ADDRESS_PORT1  0

    if(ip)
    {
		lwipIfPort1.instNum = 0;
		lwipIfPort1.slvPortNum = 1;
		lwipIfPort1.ipAddr = ip;
		lwipIfPort1.netMask = 0;
		lwipIfPort1.gwAddr = 0;
		lwipIfPort1.ipMode = IPADDR_USE_STATIC;
    }
    else
    {
		lwipIfPort1.instNum = 0;
		lwipIfPort1.slvPortNum = 1;
		lwipIfPort1.ipAddr = 0;
		lwipIfPort1.netMask = 0;
		lwipIfPort1.gwAddr = 0;
		lwipIfPort1.ipMode = IPADDR_USE_DHCP;
    }
	ipAddr = lwIPInit(&lwipIfPort1);
    if(ipAddr)
    {
        UARTPuts(DebugCom, "\n\r\n\rPort 1 IP Address Assigned: ", -1);
        IpAddrDisplay(ipAddr);
        UARTPuts(DebugCom, "\n\r", -1);
    }
    else
    {
        UARTPuts(DebugCom, "\n\r\n\rPort 1 IP Address Acquisition Failed.\n\r", -1);
    }

    /* Initialize the sample httpd server. */
    //httpd_init();
}
#endif

