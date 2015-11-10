/*
 * main.h
 *
 *  Created on: Nov 9, 2015
 *      Author: John Smith
 */
/*#####################################################*/
#ifndef MAIN_H_
#define MAIN_H_
#include "device/mpu60x0_9150.h"
#include "device/ak8975.h"
#include "device/bmp180.h"
#include "device/sht11.h"
#include "device/srf02.h"
#include "device/mhc5883.h"
#include "device/ms5611.h"
#include "device/adxl345.h"
#include "device/hih6130.h"
#include "device/mpl3115a2.h"
#include "device/mpr121.h"
#include "device/lepton_flir.h"
/*#####################################################*/
#define _USE_MPU60x0_9150	0
#define _USE_AK8975			0
#define _USE_BMP180			0
#define _USE_MPL3115A2		0
#define _USE_MS5611			0
#define _USE_MHC5883		0
#define _USE_SRF02			0
#define _USE_ADXL345		0
#define _USE_HIH613x		0
#define _USE_LEPTON_FLIR	0
#define _USE_SHT11 			0
#define _USE_MPR121			1
#define _USE_INT_ADC		0
/*#####################################################*/
#define LED1_PORT		1
#define LED1_PIN		7

#define LED2_PORT		1
#define LED2_PIN		6

#define LED3_PORT		1
#define LED3_PIN		5

#define LED4_PORT		1
#define LED4_PIN		4

#define LEDS_NR			4

#define HARDBTN1_PORT	0
#define HARDBTN1_PIN	30

#define HARDBTN2_PORT	2
#define HARDBTN2_PIN	3

#define HARDBTN3_PORT	2
#define HARDBTN3_PIN	2

#define HARDBTN4_PORT	2
#define HARDBTN4_PIN	5

#define HARDBTNS_NR		4
/*#####################################################*/
#if _USE_MPU60x0_9150 == 1
#define USE_MPU60x0_9150 \
	new_mpu60x0_9150 *MPU60x0_9150
/*-----------------------------------------------------*/
#define MPU60x0_9150_INIT(TWI_INTERFACE) \
	MPU60x0_9150 = new_(new_mpu60x0_9150);\
	MPU60x0_9150->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_AK8975 == 1
#define USE_AK8975 \
	new_ak8975 *AK8975
/*-----------------------------------------------------*/
#define AK8975_INIT(TWI_INTERFACE) \
	AK8975 = new_(new_ak8975);\
	AK8975->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_BMP180 == 1
#define USE_BMP180 \
	new_bmp180 *BMP180
/*-----------------------------------------------------*/
#define BMP180_INIT(TWI_INTERFACE) \
	BMP180 = new_(new_bmp180);\
	BMP180->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_MS5611 == 1
#define USE_MS5611 \
	new_ms5611 *MS5611
/*-----------------------------------------------------*/
#define MS5611_INIT(TWI_INTERFACE) \
	MS5611 = new_(new_ms5611);\
	MS5611->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_MHC5883 == 1
#define USE_MHC5883 \
	new_mhc5883 *MHC5883
/*-----------------------------------------------------*/
#define MHC5883_INIT(TWI_INTERFACE) \
	MHC5883 = new_(new_mhc5883);\
	MHC5883->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_MPL3115A2 == 1
#define USE_MPL3115A2 \
	new_mpl3115a2 *MPL3115A2
/*-----------------------------------------------------*/
#define MPL3115A2_INIT(TWI_INTERFACE) \
	MPL3115A2 = new_(new_mpl3115a2);\
	MPL3115A2->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_MPR121 == 1
#define USE_MPR121 \
	new_mpr121 *MPR121
/*-----------------------------------------------------*/
#define MPR121_INIT(TWI_INTERFACE) \
	MPR121 = new_(new_mpr121);\
	MPR121->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_SRF02 == 1
#define USE_SRF02 \
	new_srf02 *SRF02
/*-----------------------------------------------------*/
#define SRF02_INIT(TWI_INTERFACE) \
	SRF02 = new_(new_srf02);\
	SRF02->measure_unit = SRF02_CENTIMETER;\
	SRF02->addr = (0xE0 >> 1);/*the real address is 0x70*/\
	SRF02->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_SHT11 == 1
#define USE_SHT11 \
	new_sht11 *SHT11
/*-----------------------------------------------------*/
#define SHT11_INIT() \
	SHT11 = new_(new_sht11);\
	SHT11->Scl = gpio_assign(IOB, 12, GPIO_OUT_OPEN_DRAIN, false); \
	SHT11->Sda = gpio_assign(IOB, 13, GPIO_OUT_OPEN_DRAIN, false); \
	SHT11->state_delay = 200;\
	SHT11->vdd_comp = SH11_VDD_3_5V;
#endif
/*#####################################################*/
#if _USE_HIH613x == 1
#define USE_HIH613x \
	new_hih613x *HIH613x
/*-----------------------------------------------------*/
#define HIH613x_INIT(TWI_INTERFACE) \
	HIH613x = new_(new_hih613x);\
	HIH613x->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_LEPTON_FLIR == 1
#define USE_LEPTON_FLIR \
	new_lepton_flir *LEPTON_FLIR
/*-----------------------------------------------------*/
#define LEPTON_FLIR_INIT(SPI_INTERFACE, TWI_INTERFACE) \
	LEPTON_FLIR = new_(new_lepton_flir);\
	LEPTON_FLIR->SPI = SPI[SPI_INTERFACE]; \
	LEPTON_FLIR->TWI = TWI[TWI_INTERFACE];
#endif
/*#####################################################*/
#if _USE_ADXL345 == 1
#define USE_ADXL345 \
	new_adxl345 *ADXL345
/*-----------------------------------------------------*/
#define ADXL345_INIT(TWI_INTERFACE) \
	ADXL345 = new_(new_adxl345);\
	ADXL345->MasterSlaveAddr = 0x53;\
	ADXL345->TwiStruct = TWI[TWI_INTERFACE];\
	ADXL345->Calibration_X = 0;\
	ADXL345->Calibration_Y = 0;\
	ADXL345->Calibration_Z = 0;\
	ADXL345->FilterBuffSize = 4;\
	adxl345_init(ADXL345);
#endif
/*#####################################################*/
#define UART_0_INIT(UART_INTERFACE) \
	Uart[UART_INTERFACE] = new_(new_uart);\
	Uart[UART_INTERFACE]->BaudRate = 115200;\
	Uart[UART_INTERFACE]->Priority = 0;\
	Uart[UART_INTERFACE]->UartNr = UART_INTERFACE;\
	Uart[UART_INTERFACE]->TxPort = IOE;\
	Uart[UART_INTERFACE]->RxPort = IOE;\
	Uart[UART_INTERFACE]->TxPin = 16;\
	Uart[UART_INTERFACE]->RxPin = 15;\
	Uart[UART_INTERFACE]->rxFifoTrigLevel = 1;\
	Uart[UART_INTERFACE]->txFifoTrigLevel = 1;\
	uart_open(Uart[UART_INTERFACE]);\
	DebugCom = Uart[UART_INTERFACE];

/*#####################################################*/
#define TWI_INIT(TWI_INTERFACE) \
	UARTprintf(DebugCom, "Setup TWI1 with RxBuff = 258, TxBuff = 258....." , TWI_INTERFACE);\
	TWI[TWI_INTERFACE] = new_(new_twi);\
	TWI[TWI_INTERFACE]->BaudRate = 400000;\
	TWI[TWI_INTERFACE]->TwiNr = 0;\
	TWI[TWI_INTERFACE]->Priority = 0;\
	TWI[TWI_INTERFACE]->UseDma = false;\
	TWI[TWI_INTERFACE]->RxBuffSize = 258;\
	TWI[TWI_INTERFACE]->TxBuffSize = 258;\
	TWI[TWI_INTERFACE]->BusyTimeOut = 5;\
	TWI[TWI_INTERFACE]->SclPort = IOC;\
	TWI[TWI_INTERFACE]->SdaPort = IOC;\
	TWI[TWI_INTERFACE]->SclPin = 17;\
	TWI[TWI_INTERFACE]->SdaPin = 16;\
	twi_open(TWI[TWI_INTERFACE]);\
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*#####################################################*/
#define SPI_INIT(SPI_INTERFACE) \
	UARTprintf(DebugCom, "Setup SPI1 ....." , SPI_INTERFACE); \
	SPI[SPI_INTERFACE] = new_(new_mcspi);\
	SPI[SPI_INTERFACE]->BaudRate = 24000000;\
	SPI[SPI_INTERFACE]->BuffSize = 260;\
	SPI[SPI_INTERFACE]->Channel = 0;\
	SPI[SPI_INTERFACE]->McspiNr = 0;\
	SPI[SPI_INTERFACE]->Priority = 0;\
	SPI[SPI_INTERFACE]->MosiPort = IOB;\
	SPI[SPI_INTERFACE]->MisoPort = IOB;\
	SPI[SPI_INTERFACE]->SckPort = IOA;\
	SPI[SPI_INTERFACE]->CsPort[0] = IOA;\
	SPI[SPI_INTERFACE]->MosiPin = 17;\
	SPI[SPI_INTERFACE]->MisoPin = 16;\
	SPI[SPI_INTERFACE]->SckPin = 17;\
	SPI[SPI_INTERFACE]->CsPin[0] = 16;\
	mcspi_open(SPI[SPI_INTERFACE]); \
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*#####################################################*/
#if _USE_INT_ADC == 1
#define ADC_0_INIT(ADC_INTERFACE) \
	UARTprintf(DebugCom, "Setup ADC %d....." , ADC_INTERFACE);\
	ADC[ADC_INTERFACE] = new_(new_adc);\
	ADC[ADC_INTERFACE]->Align = ADC_ALIGN_RIGHT;\
	ADC[ADC_INTERFACE]->ContinuousMode = ADC_CONTINUOUS_MODE_CONTINUOUS;\
	ADC[ADC_INTERFACE]->ConvMode = ADC_CONV_MODE_MULTICHANNEL;\
	ADC[ADC_INTERFACE]->DmaChannel = 0;\
	ADC[ADC_INTERFACE]->DmaUnitNr = 0;\
	ADC[ADC_INTERFACE]->EnCh[0] = 1;\
	ADC[ADC_INTERFACE]->EnCh[1] = 2;\
	ADC[ADC_INTERFACE]->EnCh[2] = 17;\
	ADC[ADC_INTERFACE]->ExtTrig = ADC_EXT_TRIG_NONE;\
	ADC[ADC_INTERFACE]->ExtTrigEdge = ADC_EXT_TRIG_ADGE_NONE;\
	ADC[ADC_INTERFACE]->IntRefEn = true;\
	ADC[ADC_INTERFACE]->Mode = ADC_MODE_INDEPENDENT;\
	ADC[ADC_INTERFACE]->SampleTime[0] = ADC_SAMPLE_TIME_5;\
	ADC[ADC_INTERFACE]->SampleTime[1] = ADC_SAMPLE_TIME_5;\
	ADC[ADC_INTERFACE]->SampleTime[2] = ADC_SAMPLE_TIME_6;\
	ADC[ADC_INTERFACE]->TempSensorEn = true;\
	ADC[ADC_INTERFACE]->UnitNr = 0;\
	ADC[ADC_INTERFACE]->Prescaller = 2;\
	ADC[ADC_INTERFACE]->ResolutionBits = 12;\
	if(adc_init(ADC[ADC_INTERFACE]))
		UARTPuts(DebugCom, "OK.\n\r" , -1);\
	else  UARTPuts(DebugCom, "FAILED.\n\r" , -1);
#endif
/*#####################################################*/

#endif /* MAIN_H_ */
