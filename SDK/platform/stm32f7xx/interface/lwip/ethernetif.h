#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

/* Exported types ------------------------------------------------------------*/
void ethernetif_update_config(struct netif *netif);
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
#endif
