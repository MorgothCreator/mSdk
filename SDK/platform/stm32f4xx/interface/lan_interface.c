/*
 * lan_interface.c
 *
 *  Created on: Jul 3, 2016
 *      Author: John Smith
 */

#include "lan_interface.h"
#include "api/uart_api.h"
#include "lwip/netif.h"
#include "lwip/ethernetif.h"
#include "netif/etharp.h"

extern new_uart* DebugCom;
struct netif gnetif;

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
void _lan_get_mac_addr(unsigned int addrIdx, unsigned char *macAddr)
{
    /*macAddr[0] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx))
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
                  & 0xFF;*/
}


/*
** Displays the IP addrss on the UART Console
*/
void _lan_ip_addr_display(unsigned int ipAddr)
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


void _lan_interface_init(unsigned char instance, unsigned long ip)
{
#if 0
	UARTPuts(DebugCom, "Initialize the LWIP library.\n\r" , -1);
    unsigned int ipAddr;
    LWIP_IF lwipIfPort1, lwipIfPort2;
    CPSWPinMuxSetup(1);
    CPSWClkEnable();
    /* Chip configuration RGMII selection */
    EVMPortMIIModeSelect();
    /* Get the MAC address */
    _lan_get_mac_addr(0, lwipIfPort1.macArray);
    _lan_get_mac_addr(1, lwipIfPort2.macArray);
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
        _lan_ip_addr_display(ipAddr);
        UARTPuts(DebugCom, "\n\r", -1);
    }
    else
    {
        UARTPuts(DebugCom, "\n\r\n\rPort 1 IP Address Acquisition Failed.\n\r", -1);
    }

    /* Initialize the sample httpd server. */
    //httpd_init();
#endif
    /* do lwip library init only once */
    //static unsigned int lwipInitFlag = 0;
    //if(0 == lwipInitFlag)
    //{
        lwip_init();
    //}
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    /* Add the network interface */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /*  Registers the default network interface */
    //if(0 == lwipInitFlag)
    //{
    	netif_set_default(&gnetif);
    	//lwipInitFlag = 1;
    //}

    if (netif_is_link_up(&gnetif))
    {
      /* When the netif is fully configured this function must be called */
      netif_set_up(&gnetif);
    }
    else
    {
      /* When the netif link is down this function must be called */
      netif_set_down(&gnetif);
    }

    /* Set the link callback function, this function is called on change of link status*/
    netif_set_link_callback(&gnetif, ethernetif_update_config);
}

