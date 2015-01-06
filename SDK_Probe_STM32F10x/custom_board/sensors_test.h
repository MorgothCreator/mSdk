/*
 * bioloid_test.h
 *
 * Created: 5/1/2015 12:22:37 AM
 *  Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */
/*#####################################################*/
#ifndef SENSORS_TEST_H_
#define SENSORS_TEST_H_
/*#####################################################*/
#include "project.h"
#ifdef SENSORS_TEST
/*#####################################################*/
#define USE_SRF02 \
	new_srf02 *SRF02
/*#####################################################*/
#define SRF02_INIT \
	SRF02 = new_(new_srf02);\
	SRF02->measure_unit = SRF02_CENTIMETER;\
	SRF02->addr = (0xE0 >> 1);/*the real address is 0x70*/\
	SRF02->TWI = TWI[0];
/*#####################################################*/
#define USE_SHT11 \
	new_sht11 *SHT11
/*#####################################################*/
#define SHT11_INIT \
	SHT11 = new_(new_sht11);\
	SHT11->Scl = gpio_assign(IOB, 12, GPIO_OUT_OPEN_DRAIN, false); \
	SHT11->Sda = gpio_assign(IOB, 13, GPIO_OUT_OPEN_DRAIN, false); \
	SHT11->state_delay = 200;\
	SHT11->vdd_comp = SH11_VDD_3_5V;
/*#####################################################*/
#define UART_0_INIT \
	Uart[0] = new_(new_uart);\
	Uart[0]->BaudRate = 115200;\
	Uart[0]->Priority = 0;\
	Uart[0]->UartNr = 0;\
	Uart[0]->TxPort = IOA;\
	Uart[0]->RxPort = IOA;\
	Uart[0]->TxPin = 9;\
	Uart[0]->RxPin = 10;\
	uart_open(Uart[0]);\
	DebugCom = Uart[0];
/*#####################################################*/
#define TWI_1_INIT \
	UARTPuts(DebugCom, "Setup TWI1 with RxBuff = 258, TxBuff = 258....." , -1);\
	TWI[0] = new_(new_twi);\
	TWI[0]->BaudRate = 100000;\
	TWI[0]->TwiNr = 0;\
	TWI[0]->Priority = 0;\
	TWI[0]->RxBuffSize = 258;\
	TWI[0]->TxBuffSize = 258;\
	TWI[0]->BusyTimeOut = 5;\
	TWI[0]->SclPort = IOB;\
	TWI[0]->SdaPort = IOB;\
	TWI[0]->SclPin = 6;\
	TWI[0]->SdaPin = 7;\
	twi_open(TWI[0]);\
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*#####################################################*/
#define DXL_INTERFACE_INIT
/*#####################################################*/
#define ADC_0_INIT \
	UARTPuts(DebugCom, "Setup ADC 0....." , -1);\
	ADC[0] = new_(new_adc);\
	ADC[0]->Align = ADC_ALIGN_RIGHT;\
	ADC[0]->ContinuousMode = ADC_CONTINUOUS_MODE_CONTINUOUS;\
	ADC[0]->ConvMode = ADC_CONV_MODE_MULTICHANNEL;\
	ADC[0]->DmaChannel = 0;\
	ADC[0]->DmaUnitNr = 0;\
	ADC[0]->EnCh[0] = 1;\
	ADC[0]->EnCh[1] = 2;\
	ADC[0]->EnCh[2] = 17;\
	ADC[0]->ExtTrig = ADC_EXT_TRIG_NONE;\
	ADC[0]->IntRefEn = true;\
	ADC[0]->Mode = ADC_MODE_INDEPENDENT;\
	ADC[0]->SampleTime[0] = ADC_SAMPLE_TIME_5;\
	ADC[0]->SampleTime[1] = ADC_SAMPLE_TIME_5;\
	ADC[0]->SampleTime[2] = ADC_SAMPLE_TIME_6;\
	ADC[0]->TempSensorEn = true;\
	ADC[0]->UnitNr = 0;\
	if(adc_init(ADC[0])) UARTPuts(DebugCom, "OK.\n\r" , -1);\
	else  UARTPuts(DebugCom, "FAILED.\n\r" , -1);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "sensors_test.c"
#endif
/*#####################################################*/
#endif
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
