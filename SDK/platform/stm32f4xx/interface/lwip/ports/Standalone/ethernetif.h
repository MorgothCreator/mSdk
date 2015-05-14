#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lib/lwip/include/lwip/err.h"
#include "lib/lwip/include/lwip/netif.h"

err_t ethernetif_init(struct netif *netif);
err_t ethernetif_input(struct netif *netif);

#endif
