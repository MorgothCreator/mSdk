#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "src/include/lwip/err.h"
#include "src/include/lwip/netif.h"

err_t ethernetif_init(struct netif *netif);
err_t ethernetif_input(struct netif *netif);

#endif
