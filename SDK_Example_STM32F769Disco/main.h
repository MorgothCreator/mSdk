/*
 * main.h
 *
 *  Created on: Nov 25, 2016
 *      Author: John Smith
 */

#ifndef MAIN_H_
#define MAIN_H_

/*#####################################################*/
#include "board/STM32F769I-DISCO.h"

#define _USE_MPU60x0_9150				1
#define _USE_AK8975						1
#define _USE_BMP180						1
#define _USE_MPL3115A2					1
#define _USE_MS5611						1
#define _USE_MHC5883					0
#define _USE_HIH613x					1
#define _USE_ADXL345					0
#define _USE_SHT11 						0
#define _USE_MPR121						1
#define _USE_SX150x						0
#define _USE_INT_ADC					0
#define _USE_SRF02						0

#define _USE_LEPTON_FLIR				0
#define _USE_nRF25l01					0

#define USE_DRIVER_SEMAPHORE			true
#define USE_LCD_TYPE_DSI

#define LEPTON_SPI_INTERFACE_NR			SPI_EXTENSION_UNIT
#define LEPTON_SPI_INSTANCE_NR			0
#define LEPTON_TWI_INSTANCE_NR			DEFAULT_I2C_UNIT

#define NRF24L01_IRQ_PORT				IOC
#define NRF24L01_IRQ_PIN				13
#define NRF24L01_CE_PORT				IOA
#define NRF24L01_CE_PIN					8

#define USE_LWIP
#define USE_DHCP
#define USE_RMII_INTERFACE
#define HTTPD_PATH	"SD1:"
//#define HTTPD_PATH	"USB1:"
#define TFTPD_PATH	"SD1:"
//#define TFTPD_PATH	"USB1:"
#define LWIP_DEBUG_EN

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 0
#define IP_ADDR3   (uint8_t) 10

/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*Gateway Address*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 0
#define GW_ADDR3   (uint8_t) 1


#define USE_MMCSD						true
#define MMCSD_DEBUG_EN

#define USE_USB_HS
//#define USE_USB_DEV

#define USE_USB_HOST_MSC				false
#define USE_USB_HOST_MOUSE				false
#define USBH_MSC_DEBUG_EN

#define USE_USB_DEV_CDC					false
#define USE_USB_DEV_MSC_BRIDGE_MMCSD	false
#define USE_USB_DEV_MSC_BRIDGE_MMCSDSPI false
/*#####################################################*/


#endif /* MAIN_H_ */
