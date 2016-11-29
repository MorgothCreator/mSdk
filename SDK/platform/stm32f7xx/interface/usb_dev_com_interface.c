/*
 * usb_dev_com_interface.c
 *
 *  Created on: Oct 17, 2015
 *      Author: John Smith
 */

#include "stdbool.h"
#include "driver/USBD/Class/CDC/Inc/usbd_cdc.h"
#include "driver/USBD/Class/CDC/usbd_cdc_interface.h"
#include "driver/USBD/Class/CDC/usbd_cdc_desc.h"
#include "driver/USBD/Core/Inc/usbd_core.h"
#include "driver/stm32f7xx_hal_tim.h"
#include "lib/buffers/ring_buff.h"

fifo_settings_t *usb_cdc_dev_rx_fifo;
fifo_settings_t *usb_cdc_dev_tx_fifo;

//extern uint8_t UserRxBuffer[];/* Received Data over USB are stored in this buffer */
//extern uint8_t UserTxBuffer[];/* Received Data over UART (CDC interface) are stored in this buffer */

//extern uint32_t UserTxBufPtrIn;/* Increment this pointer or roll it back to
                               //start address when data are received over USART */
//extern uint32_t UserTxBufPtrOut; /* Increment this pointer or roll it back to
                                 //start address when data are sent over USB */

//static int extract_receive_char_ptr = 0;
int put_receive_char_ptr = 0;

//extern unsigned int UserRxBuffCnt;

USBD_HandleTypeDef  usb_cdc_dev_param;

bool _usb_com_dev_init(unsigned int instance)
{
	usb_cdc_dev_rx_fifo = fifo_open(APP_RX_DATA_SIZE);
	usb_cdc_dev_tx_fifo = fifo_open(APP_RX_DATA_SIZE);
	/* Init Device Library */
	USBD_Init(&usb_cdc_dev_param, &VCP_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&usb_cdc_dev_param, USBD_CDC_CLASS);

	/* Add CDC Interface Class */
	USBD_CDC_RegisterInterface(&usb_cdc_dev_param, &USBD_CDC_fops);

	/* Start Device Process */
	USBD_Start(&usb_cdc_dev_param);


	return true;
}
unsigned int _usb_com_dev_receive(unsigned char* buff)
{
	fifo_pop_return_t data_return = fifo_pop(usb_cdc_dev_rx_fifo);
	buff[0] = data_return.character;
	if(data_return.status == true)
		return 1;
	else
		return 0;
}
unsigned int _usb_com_dev_send(unsigned char* buff, unsigned int nbytes)
{
	unsigned int cnt = 0;
    USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) usb_cdc_dev_param.pClassData;
	for(; cnt < nbytes; cnt++)
	{
		if(fifo_push(usb_cdc_dev_tx_fifo, buff[cnt]) == false && hcdc && hcdc->TxBuffer && hcdc->RxBuffer)
			break;
	}
	return cnt;
}

