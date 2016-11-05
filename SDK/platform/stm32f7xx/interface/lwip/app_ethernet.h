/**
  ******************************************************************************
  * @file    LwIP/LwIP_TCP_Echo_Server/Inc/app_ethernet.h 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   Header for app_ethernet.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ETHERNET_H
#define __APP_ETHERNET_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lwip/netif.h"

 /* DHCP process states */
 #define DHCP_START                 (unsigned char) 1
 #define DHCP_WAIT_ADDRESS          (unsigned char) 2
 #define DHCP_ADDRESS_ASSIGNED      (unsigned char) 3
 #define DHCP_TIMEOUT               (unsigned char) 4
 #define DHCP_LINK_DOWN             (unsigned char) 5

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void User_notification(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);
void DHCP_Periodic_Handle(struct netif *netif);
void DHCP_Process(struct netif *netif);

#ifdef __cplusplus
}
#endif

#endif /* __APP_ETHERNET_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

