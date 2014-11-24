/*
 * bridge_eMMC_MMCSD1_to_USB0.c
 *
 *  Created on: Nov 24, 2014
 *      Author: Morgoth
 */

#include <stdlib.h>
#include "bridge_eMMC_MMCSD1_to_USB0.h"
#include "main.h"

#ifdef bridge_eMMC_MMCSD1_to_USB0

#include "api/core_init_api.h"
#include "api/usb_api.h"
#include "api/mmcsd_api.h"
#include "api/gpio_api.h"
#include "api/uart_api.h"
#include "pinmux/pin_mux_uart.h"
#include "interface/mmcsdlib/mmcsd_proto.h"

new_gpio* eMMC_Res = NULL;
new_uart* DebugCom = NULL;
new_gpio* LED[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
mmcsdCtrlInfo sdCtrl[2];

int main(void)
{
	core_init();
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->TxPin = Uart0_Tx_PinMux_E16;
	DebugCom->RxPin = Uart0_Rx_PinMux_E15;
	DebugCom->BaudRate = 115200;
	DebugCom->RxBuffSize = 256;
	DebugCom->TxBuffSize = 256;
	DebugCom->rxFifoTrigLevel = 1;
	DebugCom->txFifoTrigLevel = 1;
	DebugCom->UartNr = 0;
	uart_open(DebugCom);
/*-----------------------------------------------------*/
	LED[0] = gpio_assign(1, 21, GPIO_DIR_OUTPUT, false);
	LED[1] = gpio_assign(1, 22, GPIO_DIR_OUTPUT, false);
	LED[2] = gpio_assign(1, 23, GPIO_DIR_OUTPUT, false);
	LED[3] = gpio_assign(1, 24, GPIO_DIR_OUTPUT, false);
/*-----------------------------------------------------*/
#ifdef BridgeUsbDev0ToMmcSd0
	UARTPuts(DebugCom, "Init MMCSD0 .......", -1);
	sdCtrl[0].SdNr = 0;
	mmcsd_init(&sdCtrl[0], 0, 6, LED[0]);
	UARTPuts(DebugCom, "OK.\n\r", -1);
	mmcsd_idle(&sdCtrl[0]);
/*-----------------------------------------------------*/
#elif defined(BridgeUsbDev0ToMmcSd1)
	eMMC_Res = gpio_assign(1, 20, GPIO_DIR_OUTPUT, false);
	gpio_out(eMMC_Res, 0);
	Sysdelay(10);
	gpio_out(eMMC_Res, 1);
	UARTPuts(DebugCom, "Init MMCSD1 .......", -1);
	sdCtrl[1].SdNr = 1;
	mmcsd_init(&sdCtrl[1], -1, -1, LED[0]);
	UARTPuts(DebugCom, "OK.\n\r", -1);
	mmcsd_idle(&sdCtrl[1]);
#endif
/*-----------------------------------------------------*/
#ifdef BridgeUsbDev0ToMmcSd0
	UARTPuts(DebugCom, "Bridge USBMSC0 Dev for MMCSD0 Interface.......", -1);
	drv_rw_func.DriveStruct = &sdCtrl[0];
	drv_rw_func.drv_ioctl_func = mmcsd_ioctl;
	drv_rw_func.drv_r_func = mmcsd_read;
	drv_rw_func.drv_w_func = mmcsd_write;
	usb_msc_dev_init(0);
	UARTPuts(DebugCom, "OK.\n\r", -1);
/*-----------------------------------------------------*/
#elif defined(BridgeUsbDev0ToMmcSd1)
	UARTPuts(DebugCom, "Bridge USBMSC0 Dev for MMCSD1 Interface.......", -1);
	drv_rw_func.DriveStruct = &sdCtrl[1];
	drv_rw_func.drv_ioctl_func = mmcsd_ioctl;
	drv_rw_func.drv_r_func = mmcsd_read;
	drv_rw_func.drv_w_func = mmcsd_write;
	usb_msc_dev_init(0);
	UARTPuts(DebugCom, "OK.\n\r", -1);
#endif
/*-----------------------------------------------------*/
	bool old_connected = false;

#ifdef BridgeUsbDev0ToMmcSd0
	if(sdCtrl[0].connected == false) usb_msc_dev_media_change_state(0, false);
#elif defined(BridgeUsbDev0ToMmcSd1)
	if(sdCtrl[1].connected == false) usb_msc_dev_media_change_state(0, false);
#endif
/*-----------------------------------------------------*/
	while(1)
	{
        mmcsd_idle(&sdCtrl[0]);
#ifdef BridgeUsbDev0ToMmcSd0
        if(old_connected == false && sdCtrl[0].connected == true)
        {
        	old_connected = true;
        	usb_msc_dev_media_change_state(0, true);
        }
        else if(old_connected == true && sdCtrl[0].connected == false)
        {
        	old_connected = false;
        	usb_msc_dev_media_change_state(0, false);
        }

#elif defined(BridgeUsbDev0ToMmcSd1)
        mmcsd_idle(&sdCtrl[1]);
        if(old_connected == false && sdCtrl[1].connected == true)
        {
        	old_connected = true;
        	usb_msc_dev_media_change_state(1, true);
        }
        else if(old_connected == true && sdCtrl[1].connected == false)
        {
        	old_connected = false;
        	usb_msc_dev_media_change_state(1, false);
        }
#endif
	}
}
#endif
