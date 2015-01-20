/*
 * tda19988.c
 *
 *  Created on: Dec 24, 2014
 *      Author: Morgoth
 */

#include "stdbool.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_control_AM335x.h"
#include "include/hw/hw_types.h"

bool tda19988_init()
{
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_XDMA_EVENT_INTR(0)) =
              (2 << CONTROL_CONF_XDMA_EVENT_INTR0_CONF_XDMA_EVENT_INTR0_MMODE_SHIFT)    |
              (1 << CONTROL_CONF_XDMA_EVENT_INTR0_CONF_XDMA_EVENT_INTR0_PUDEN_SHIFT)    |
              (0 << CONTROL_CONF_XDMA_EVENT_INTR0_CONF_XDMA_EVENT_INTR0_PUTYPESEL_SHIFT)|
              (1 << CONTROL_CONF_XDMA_EVENT_INTR0_CONF_XDMA_EVENT_INTR0_RXACTIVE_SHIFT) |
              (0 << CONTROL_CONF_XDMA_EVENT_INTR0_CONF_XDMA_EVENT_INTR0_SLEWCTRL_SHIFT);
    return false;
}


