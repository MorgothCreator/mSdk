/*
 * lwip_api.c
 *
 *  Created on: Nov 7, 2012
 *      Author: XxXx
 */
#include "lwip_hardware_init.h"

//#include "clk/clk_cpsw.h"
//#include "pin_mux_cpsw.h"
//#include "aintc/aintc_cpsw.h"
//#include "driver/cpu.h"
//#include "ports/include/lwiplib.h"
#include "lib/util//ascii.h"
//#include "lib/lwip/simple_fs_httpd.h"
#include "driver/stm32f4x7_eth.h"
#include "driver/misc.h"
#include "netconf.h"
#include "main.h"
//#include "httpd.h"

#ifdef UseLwip
extern new_uart* DebugCom;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;

  /* wait until the desired delay finish */
  while(timingdelay > LocalTime)
  {
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

/*
** This function takes user input for value and checks given values is the
** range of min and max values. If not default value is assiagned.
*/
static unsigned int UserValueInfoGet(unsigned int min, unsigned int max,
                                     unsigned int def, unsigned int hex,
                                     unsigned char *help)
{
    unsigned int value = 0;

    UARTprintf(DebugCom, "%s", help);

    if(false == hex)
    {
    	UARTscanf(DebugCom, "%d", &value);
    }
    else
    {
    	UARTscanf(DebugCom, "%x", &value);
    }

    if((min > value) || (value > max))
    {
    	UARTprintf(DebugCom, "\n\rSetting Default Value: 0x%x\r\n", def);
        return def;
    }

    return value;
}

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
/*void CPSWCore0RxIsr(void)
{
	//CPUirqd();
	lwIPRxIntHandler(0);
	//CPUirqe();
}*/

/*
** Interrupt Handler for Core 0 Transmit interrupt
*/
/*void CPSWCore0TxIsr(void)
{
	//CPUirqd();
    lwIPTxIntHandler(0);
    //CPUirqe();
}*/


/**
 * \brief   This function returns the MAC address for the EVM
 *
 * \param   addrIdx    the MAC address index.
 * \param   macAddr    the Pointer where the MAC address shall be stored
 *     'addrIdx' can be either 0 or 1
 *
 * \return  None.
 */
/*void EVMMACAddrGet(unsigned int addrIdx, unsigned char *macAddr)
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
}*/


void lan_interface_init(void)
{
	UARTPuts(DebugCom, "Initialize the LWIP library.\n\r" , -1);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */
	ETH_BSP_Config();

	/* Initilaize the LwIP stack */
	LwIP_Init();
	/* Http webserver Init */
	//httpd_init();

}

void lwip_idle(void) {
    /* check if any packet received */
    if (ETH_CheckFrameReceived())
    {
      /* process received Ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);

}
#endif

