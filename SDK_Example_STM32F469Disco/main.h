/*
 * bioloid_test.h
 *
 * Created: 5/1/2015 12:22:37 AM
 *  Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */
/*#####################################################*/
#ifndef MAIN_H_
#define MAIN_H_
/*#####################################################*/
#include "board/STM32F469I-DISCO.h"

#define _USE_MPU60x0_9150			1
#define _USE_AK8975					1
#define _USE_BMP180					1
#define _USE_MPL3115A2				1
#define _USE_MS5611					1
#define _USE_MHC5883				1
#define _USE_SRF02					0
#define _USE_ADXL345				0
#define _USE_HIH613x				1
#define _USE_LEPTON_FLIR			0
#define _USE_SHT11 					0
#define _USE_MPR121					0
#define _USE_INT_ADC				0

#define USE_DRIVER_SEMAPHORE		true
#define USE_LCD_TYPE_DSI

#define LEPTON_SPI_INTERFACE_NR		SPI_EXTENSION_UNIT
#define LEPTON_SPI_INSTANCE_NR		0
#define LEPTON_TWI_INSTANCE_NR		DEFAULT_I2C_UNIT

#define NRF24L01_IRQ_PORT			IOC
#define NRF24L01_IRQ_PIN			13
#define NRF24L01_CE_PORT			IOA
#define NRF24L01_CE_PIN				8
/*#define USBH_MSC_DEBUG_EN
#define MMCSD_DEBUG_EN*/
/*#####################################################*/
#endif /* MAIN_H_ */
/*#####################################################*/
