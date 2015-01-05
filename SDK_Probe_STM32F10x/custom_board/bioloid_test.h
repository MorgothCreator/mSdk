/*
 * bioloid_test.h
 *
 * Created: 5/1/2015 12:22:37 AM
 *  Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */
/*#####################################################*/
#ifndef BIOLOID_TEST_H_
#define BIOLOID_TEST_H_
/*#####################################################*/
#ifdef BIOLOID_TEST
/*#####################################################*/
#define USE_SHT11
/*#####################################################*/
#define SHT11_INIT
/*#####################################################*/
#define UART_0_INIT \
	Uart[0] = new_(new_uart);\
	Uart[0]->BaudRate = 1000000;\
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
	TWI[1] = new_(new_twi);\
	TWI[1]->BaudRate = 100000;\
	TWI[1]->TwiNr = 1;\
	TWI[1]->Priority = 0;\
	TWI[1]->RxBuffSize = 258;\
	TWI[1]->TxBuffSize = 258;\
	TWI[1]->BusyTimeOut = 5;\
	TWI[1]->SclPort = IOA;\
	TWI[1]->SdaPort = IOA;\
	TWI[1]->SclPin = 0;\
	TWI[1]->SdaPin = 1;\
	twi_open(TWI[1]);\
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*#####################################################*/
#define DXL_INTERFACE_INIT \
	Uart[1] = new_(new_uart);\
	Uart[1]->BaudRate = 1000000;\
	Uart[1]->Priority = 0;\
	Uart[1]->UartNr = 1;\
	Uart[1]->TxPort = IOA;\
	Uart[1]->RxPort = IOA;\
	Uart[1]->TxPin = 2;\
	Uart[1]->RxPin = 3;\
	uart_open(Uart[1]);\
	\
	ENTX = gpio_assign(IOA, 4, GPIO_DIR_OUTPUT, false);\
	gpio_out(ENTX, 0);\
	\
	DXL = new_(new_dxl_actuator);\
	DXL->Uart = Uart[1];\
	DXL->TxEnGpio = ENTX;\
	DXL->timeout = 5;
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
#include "bioloid_test.c"
#endif
/*#####################################################*/
#endif
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
