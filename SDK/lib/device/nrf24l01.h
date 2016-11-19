/*
 * nrf24l01.h
 *
 *  Created on: Aug 28, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_NRF24L01_H_
#define LIB_DEVICE_NRF24L01_H_
#include <api/spi_api.h>
#include <api/spi_def.h>
#include "stdbool.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"

//CONSTANTS
#define   NOP             	0xff
#define   R_REGISTER      	0x00
#define   W_REGISTER      	0x01
#define   R_RX_PL_WID    	0x60
#define   R_RX_PAYLOAD    	0x61
#define   W_TX_PAYLOAD    	0xA0
#define   W_TX_PAYLOAD_NO_ACK    0xB0
#define   FLUSH_TX        	0xE1
#define   FLUSH_RX        	0xE2
#define   REUSE_TX_PL     	0xE3

#define   RX_ADDR_P0      	0x0A
#define   RX_ADDR_P1      	0x0B
#define   TX_ADDR         	0x10
#define   RX_PW_P0        	0x11
#define   RX_PW_P1        	0x12
#define   FIFO_STATUS     	0x17

#define   MAX_RT          	0x10

#define   CONFIG_REG_ADDR 	0x00
#define   EN_AA				0x01
#define   EN_RXADDR			0x02
#define   SETUP_AW			0x03
#define   SETUP_RETR		0x04
#define   RF_CH				0x05
#define   RF_SETUP			0x06
#define   	RF_SETUP_CONT_WAVE	(1 << 7)
#define   	RF_SETUP_RF_DR_LOW	(1 << 5)
#define   	RF_SETUP_PLL_LOCK	(1 << 4)
#define   	RF_SETUP_RF_DR_HIGH	(1 << 3)
#define   	RF_SETUP_RF_PWR_18	(0 << 1)
#define   	RF_SETUP_RF_PWR_12	(1 << 1)
#define   	RF_SETUP_RF_PWR_6	(2 << 1)
#define   	RF_SETUP_RF_PWR_0	(3 << 1)

#define   STATUS_ADDR     	0x07
#define   OBSERVE_TX     	0x08

#define   DYNPD	     		0x1C
#define   	DYNPD_DPL_P5		(1 << 5)
#define   	DYNPD_DPL_P4		(1 << 4)
#define   	DYNPD_DPL_P3		(1 << 3)
#define   	DYNPD_DPL_P2		(1 << 2)
#define   	DYNPD_DPL_P1		(1 << 1)
#define   	DYNPD_DPL_P0		(1 << 0)

#define   FEATURE	     	0x1D
#define   	FEATURE_EN_DPL		(1 << 2)
#define   	FEATURE_EN_ACK_PAY	(1 << 1)
#define   	FEATURE_EN_DYN_ACK	(1 << 0)

#define   FLUSH_TX        	0xE1
#define   TX_FULL         	0x01
#define   RX_DR           	0x40
#define   TX_DS           	0x20

#define   RX_TX_TIME      	100000

typedef struct nrf24l01_s{
	Mcspi_t *spi;
	unsigned int spi_instance;
	unsigned char status;
	Gpio_t *ce;
	Gpio_t *irq;
	enum{
		NRF24L01_RX_DISABLED = 0,
		NRF24L01_RX_ACTIVE = 1,
	}RX_STATE;
	enum{
		NRF24L01_TX_DISABLED = 0,
		NRF24L01_TX_ACTIVE = 1,
	}TX_STATE;
	unsigned char rx_addr0[5];
	unsigned char rx_addr1[5];
	unsigned char tx_addr[5];
	unsigned char read_addr[5];
}nrf24l01_t;
void nrf24l01_set_enc_soc_burst(void *param, unsigned char pipe_mask);
void nrf24l01_set_pipe(void *param, unsigned char pipe_mask);
void nrf24l01_set_addr_field_with(void *param, unsigned char size);
void nrf24l01_set_auto_retransmit_delay_count(void *param, unsigned char delay, unsigned char count);
void nrf24l01_set_rx(void *param);
void nrf24l01_set_tx(void *param);
void nrf24l01_get_status(void *param, bool continuous_carryer_transmit, unsigned char rf_data_speed, bool force_pll_lock, unsigned char rf_pwr);
void nrf24l01_setup(void *param, bool continuous_carryer_transmit, unsigned char rf_data_speed, bool force_pll_lock, unsigned char rf_pwr);
bool nrf24l01_enable_rx(void *param, unsigned long address);
signed short nrf24l01_packet_receive(void *param, unsigned char *packet, unsigned short max_len, unsigned long timeout);
signed int nrf24l01_packet_send(void *param, unsigned char *packet, unsigned char packet_len, unsigned long address);
bool nrf24l01_init(void *param);
/*#####################################################*/
#define new_nrf24l01 nrf24l01_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_nrf24l01(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "nrf24l01.c"
#endif
/*#####################################################*/
#endif /* LIB_DEVICE_NRF24L01_H_ */
