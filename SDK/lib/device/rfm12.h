/*
 * rfm12.h
 *
 *  Created on: Sep 16, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_RFM12_H_
#define LIB_DEVICE_RFM12_H_

#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include "stdbool.h"
#include "stdlib.h"
#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include <api/spi_api.h>
#include <api/spi_def.h>

/** \name States for rx and tx buffers
* \anchor rxtx_states
* \see rfm12_rx_status() and rfm12_control_t
* @{
*/
//! Indicates that the buffer is free
#define STATUS_FREE 0
//! Indicates that the buffer is in use by the library
#define STATUS_OCCUPIED 1
//! Indicates that a receive buffer holds a complete transmission
#define STATUS_COMPLETE 2
//@}


/** \name  Return values for rfm12_tx() and rfm12_start_tx()
* \anchor tx_retvals
* \see rfm12_tx() and rfm12_start_tx()
* @{
*/
//!  The packet data is longer than the internal buffer
#define RFM12_TX_ERROR 0x02
//! The transmit buffer is already occupied
#define RFM12_TX_OCCUPIED 0x03
//! The packet has been enqueued successfully
#define RFM12_TX_ENQUEUED 0x80
//@}

const unsigned short settings_4_8kbs[]
#ifdef FLASH_DEVICE
	PROGMEM
#endif
= {
	0x80D8,//EL,EF,433band,12.5pF
	0x8208,//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	0xA680,//A140=430.8MHz
	0xC647,//4.8kbps
	0x94A2,//VDI,FAST,134kHz,0dBm,-103dBm
	0xC2AC,//AL,!ml,DIG,DQD4
	0xCA81,//FIFO8,SYNC,!ff,DR
	0xCED4,//SYNC=2DD4?
	0xC483,//@PWR,NO RSTRIC,!st,!fi,OE,EN
	0x9850,//!mp,9810=30kHz,MAX OUT
	0xCC77,//OB1?OB0,?lpx,?ddy?DDIT?BW0
	0xE000,//NOT USE
	0xC800,//NOT USE
	0xC040,//1.66MHz,2.2V*/
	0x0000
};

const unsigned short settings_114kbs[]
#ifdef FLASH_DEVICE
	PROGMEM
#endif
= {
	0x80D7,
	0x8208,//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	0xA640,//A140=430.8MHz
	0xC602,//114.943kbps
	0x9080,//Receiver Control Command
	0xC2EF,//AL,!ml,DIG,DQD4
	0xCA81,//FIFO8,SYNC,!ff,DR
	0xCED4,//SYNC=2DD4?
	0xC47F,//@PWR,NO RSTRIC,!st,!fi,OE,EN
	0x9870,//!mp,dev120khz,MAX OUT
	0xCC13,//OB1?OB0,?lpx,?ddy?DDIT?BW0
	0xE000,//NOT USE
	0xC800,//NOT USE
	0xC040,//1.66MHz,2.2V
	0x0000
};

typedef struct rfm12_s
{
	bool driver_initialized;
	unsigned char spi_instance;
	new_mcspi *spi_unit;
	unsigned short rx_buff_len;
	unsigned short tx_buff_len;
	unsigned char *tx_buffer;
	unsigned char *rx_buffer;
	unsigned int rtx_cnt;//Bytes received/sent.
	unsigned int rtx_bytes_rs;//Bytes to send.
	unsigned char *rtx_buff;
	unsigned int rtx_buff_len;
	unsigned short rx_packet_len;//Packet size on the header.
	unsigned long address;
	enum{
		RTX_STATUS_FREE = 0,
		RTX_STATUS_TX_BUSY = -1,
		RTX_STATUS_RX_BUSY = -2,
		RTX_STATUS_TIMEOUT = -3,
		RTX_STATUS_RX_BUFF_OVERFLOW = -4,
		RTX_STATUS_ADDR_NOT_MATCH = -5,
		RTX_STATUS_CHK_SUM_ERR = -6,
		RTX_STATUS_PACKET_RX_COMPLETE = -7,
		RTX_STATUS_ADDR_MATCH = -8,
	}service_status;
	unsigned short *settings;
	new_gpio *IRQ_PORT;
	unsigned char IRQ_PIN;
	new_gpio *CS_PORT;
	unsigned char CS_PIN;
}rfm12_t;


void rfm12_init(void *param, new_mcspi *spi_unit, unsigned char spi_instance, new_gpio *IRQ_PORT, new_gpio *CS_PORT);
void rfm12_deinit(void *param);
bool rfm12_packet_send(void *param, unsigned char *buff, unsigned short len, unsigned long address);
bool rfm12_enable_rx(void *param, unsigned long address);
signed int rfm12_packet_receive(void *param, unsigned char *buff, unsigned short max_len, unsigned long timeout);
void rfm12_set_power(void *param, unsigned char power, unsigned char mod);
void rfm12_rtx_service(void *param);
/*#####################################################*/
#define new_rfm12 rfm12_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_rfm12(address) free(address);
/*#####################################################*/
#endif /* LIB_DEVICE_RFM12_H_ */
