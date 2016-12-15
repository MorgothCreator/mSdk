/*
 * lan_interface.c
 *
 *  Created on: Jul 3, 2016
 *      Author: John Smith
 */

#include "lan_interface.h"
#include "api/uart_api.h"
#include "driver/stm32f7xx_hal_eth.h"
#include "interface/LwIp/app_ethernet.h"
#include "interface/LwIp/src/include/lwip/netif.h"
#include "interface/LwIp/src/include/lwip/init.h"
#include "interface/LwIp/ethernetif.h"
#include "interface/LwIp/src/include/netif/etharp.h"
#include "interface/LwIp/src/include/lwip/lwip_timers.h"
extern new_uart* DebugCom;
struct netif gnetif;


/*
** Displays the IP addrss on the UART Console
*/
void _lan_ip_addr_display(unsigned int ipAddr)
{
    //unsigned char byte;
    //int cnt;

    unsigned int n1 = (ipAddr >> 24) & 0xFF;
    unsigned int n2 = (ipAddr >> 16) & 0xFF;
    unsigned int n3 = (ipAddr >> 8) & 0xFF;
    unsigned int n4 = ipAddr & 0xFF;
    uart.printf(DebugCom, "%u.%u.%u.%u", n1, n2, n3, n4);
}

void _lan_init(unsigned char instance, unsigned long ip)
{
    /* Enable MFX interrupt for ETH MII pin */
    //BSP_IO_ConfigPin(MII_INT_PIN, IO_MODE_IT_FALLING_EDGE);
    /* do lwip library init only once */
    //static unsigned int lwipInitFlag = 0;
    //if(0 == lwipInitFlag)
    //{
        lwip_init();
    //}
        ip_addr_t ipaddr;
        ip_addr_t netmask;
        ip_addr_t gw;

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

    /* Notify user about the network interface config */
    User_notification(&gnetif);
}

unsigned char _lan_idle(unsigned char instance)
{
    /* Read a received packet from the Ethernet buffers and send it
       to the lwIP for handling */
    ethernetif_input(&gnetif);
    /* Handle timeouts */
    sys_check_timeouts();
    DHCP_Periodic_Handle(&gnetif);
    return DHCP_state;
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
//void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
//{
  /* Prevent unused argument(s) compilation warning */
//  UNUSED(heth);
//  ethernetif_input(&gnetif);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_TxCpltCallback could be implemented in the user file
  */
//}
