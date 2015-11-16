/*
 * init_api_def.h
 *
 *  Created on: Nov 15, 2015
 *      Author: John Smith
 */

#ifndef LIB_API_INIT_API_DEF_H_
#define LIB_API_INIT_API_DEF_H_

#include "api/core_init_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/gpio_def.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/touchscreen_def.h"
#include "api/mmcsd_api.h"

#include "device/ADXL345.h"
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


#include "interface/hs_mmcsd_interface_def.h"
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*########    Internal devices and sensors   ##########*/
/*#####################################################*/
#define INIT_UART(UART_INTERFACE, TransferRate, _TxPort, _TxPin, _RxPort, _RxPin) \
		Uart[UART_INTERFACE] = new_(new_uart);\
		Uart[UART_INTERFACE]->BaudRate = TransferRate;\
		Uart[UART_INTERFACE]->Priority = 0;\
		Uart[UART_INTERFACE]->UartNr = UART_INTERFACE;\
		Uart[UART_INTERFACE]->TxPort = _TxPort;\
		Uart[UART_INTERFACE]->RxPort = _RxPort;\
		Uart[UART_INTERFACE]->TxPin = _TxPin;\
		Uart[UART_INTERFACE]->RxPin = _RxPin;\
		Uart[UART_INTERFACE]->rxFifoTrigLevel = 1;\
		Uart[UART_INTERFACE]->txFifoTrigLevel = 1;\
		uart_open(Uart[UART_INTERFACE])
/*#####################################################*/
#define INIT_TWI(TWI_INTERFACE, _TransferRate, _SclPort, _SclPin, _SdaPort, _SdaPin, _RxBuffSize, _TxBuffSize) \
		TWI[TWI_INTERFACE] = new_(new_twi);\
		TWI[TWI_INTERFACE]->BaudRate = _TransferRate;\
		TWI[TWI_INTERFACE]->TwiNr = TWI_INTERFACE;\
		TWI[TWI_INTERFACE]->Priority = 0;\
		TWI[TWI_INTERFACE]->UseDma = false;\
		TWI[TWI_INTERFACE]->RxBuffSize = _RxBuffSize;\
		TWI[TWI_INTERFACE]->TxBuffSize = _TxBuffSize;\
		TWI[TWI_INTERFACE]->BusyTimeOut = 5;\
		TWI[TWI_INTERFACE]->SclPort = _SclPort;\
		TWI[TWI_INTERFACE]->SdaPort = _SdaPort;\
		TWI[TWI_INTERFACE]->SclPin = _SclPin;\
		TWI[TWI_INTERFACE]->SdaPin = _SdaPin;\
		twi_open(TWI[TWI_INTERFACE])
/*#####################################################*/
#define INIT_SPI(SPI_INTERFACE, _TransferRate, _WordSize, _SckPort, _SckPin, _MosiPort, _MosiPin, _MisoPort, _MisoPin, _CsPort, _CsPin) \
		SPI[SPI_INTERFACE] = new_(new_mcspi); \
		SPI[SPI_INTERFACE]->CsPort[0] = _CsPort; \
		SPI[SPI_INTERFACE]->CsPin[0] = _CsPin; \
		SPI[SPI_INTERFACE]->MisoPort = _MisoPort; \
		SPI[SPI_INTERFACE]->MisoPin = _MisoPin; \
		SPI[SPI_INTERFACE]->MosiPort = _MosiPort; \
		SPI[SPI_INTERFACE]->MosiPin = _MosiPin; \
		SPI[SPI_INTERFACE]->SckPort = _SckPort; \
		SPI[SPI_INTERFACE]->SckPin = _SckPin; \
		SPI[SPI_INTERFACE]->McspiNr = SPI_INTERFACE; \
		SPI[SPI_INTERFACE]->Cpol = true; \
		SPI[SPI_INTERFACE]->Cpha = true; \
		SPI[SPI_INTERFACE]->LsbFirst = false; \
		SPI[SPI_INTERFACE]->WordSize = _WordSize; \
		SPI[SPI_INTERFACE]->Slave = false; \
		SPI[SPI_INTERFACE]->BaudRate = _TransferRate; \
		mcspi_open(SPI[SPI_INTERFACE])
/*#####################################################*/
#define INIT_MMCSD(UNIT_NR, GpioMmcSdDetect, GpioLed) \
		mmcsd_init(UNIT_NR, GpioMmcSdDetect, GpioLed); \
		mmcsd_idle(UNIT_NR)
/*-----------------------------------------------------*/
#define INIT_EMMC(UNIT_NR, GpioReset, GpioLed) \
		gpio_out(GpioReset, 0); \
		Sysdelay(1); \
		gpio_out(GpioReset, 1); \
		mmcsd_init(UNIT_NR, (Gpio_t*)NULL, LED[0]); \
		mmcsd_idle(UNIT_NR)
/*#####################################################*/
#define INIT_USB_DEV_MSC_BRIDGE(USB_DEVICE_NR, SLAVE_DEV_CONTROLS_PTR) \
	usb_msc_dev_init(USB_DEVICE_NR, SLAVE_DEV_CONTROLS_PTR)
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*########    External devices and sensors   ##########*/
/*#####################################################*/
#define NEW_MPU60x0_9150(name) \
		new_mpu60x0_9150 *name
/*-----------------------------------------------------*/
#define INIT_MPU60x0_9150(name, TWI_INTERFACE) \
		name = new_(new_mpu60x0_9150);\
		name->TWI = TWI[TWI_INTERFACE];\
		mpu60x0_9150_init(name)
/*#####################################################*/
#define NEW_AK8975(name) \
		new_ak8975 *name
/*-----------------------------------------------------*/
#define INIT_AK8975(name, TWI_INTERFACE) \
		name = new_(new_ak8975);\
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_BMP180(name) \
		new_bmp180 *name
/*-----------------------------------------------------*/
#define INIT_BMP180(name, TWI_INTERFACE) \
		name = new_(new_bmp180);\
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_MS5611(name) \
		new_ms5611 *name
/*-----------------------------------------------------*/
#define INIT_MS5611(name, TWI_INTERFACE) \
		name = new_(new_ms5611);\
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_MHC5883(name) \
		new_mhc5883 *name\
/*-----------------------------------------------------*/
#define INIT_MHC5883(name, TWI_INTERFACE) \
		name = new_(new_mhc5883);\
		name->TWI = TWI[TWI_INTERFACE];\
		mhc5883_init(name)
/*#####################################################*/
#define NEW_MPL3115A2(name) \
		new_mpl3115a2 *name
/*-----------------------------------------------------*/
#define INIT_MPL3115A2(name, TWI_INTERFACE) \
		name = new_(new_mpl3115a2);\
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_MPR121(name) \
		new_mpr121 *name
/*-----------------------------------------------------*/
#define INIT_MPR121(name, TWI_INTERFACE) \
		name = new_(new_mpr121);\
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_SRF02(name) \
		new_srf02 *name
/*-----------------------------------------------------*/
#define INIT_SRF02(name, TWI_INTERFACE) \
		name = new_(new_srf02);\
		name->measure_unit = SRF02_CENTIMETER;\
		name->addr = (0xE0 >> 1);/*the real address is 0x70*/\
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_SHT11(name) \
		new_sht11 *name
/*-----------------------------------------------------*/
#define INIT_SHT11(name, _SclPort, _SclPin, _SdaPort, _SdaPin) \
		name = new_(new_sht11);\
		name->Scl = gpio_assign(_SclPort, _SclPin, GPIO_OUT_PUSH_PULL, false); \
		name->Sda = gpio_assign(_SdaPort, _SdaPin, GPIO_OUT_PUSH_PULL, false); \
		name->state_delay = 200;\
		name->vdd_comp = SH11_VDD_3_5V
/*#####################################################*/
#define NEW_HIH613x(name) \
	new_hih613x *name
/*-----------------------------------------------------*/
#define INIT_HIH613x(name, TWI_INTERFACE) \
		name = new_(new_hih613x);\
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_LEPTON_FLIR(name) \
	new_lepton_flir *name
/*-----------------------------------------------------*/
#define INIT_LEPTON_FLIR(name, SPI_INTERFACE, TWI_INTERFACE) \
		name = new_(new_lepton_flir);\
		name->SPI = SPI[SPI_INTERFACE]; \
		name->TWI = TWI[TWI_INTERFACE]
/*#####################################################*/
#define NEW_ADXL345(name) \
	new_adxl345 *name
/*-----------------------------------------------------*/
#define INIT_ADXL345(name, TWI_INTERFACE) \
		name = new_(new_adxl345);\
		name->MasterSlaveAddr = 0x53;\
		name->TwiStruct = TWI[TWI_INTERFACE];\
		name->Calibration_X = 0;\
		name->Calibration_Y = 0;\
		name->Calibration_Z = 0;\
		name->FilterBuffSize = 4;\
		adxl345_init(name)
/*#####################################################*/









/*#####################################################*/
/*########               In future           ##########*/
/*#####################################################*/




#if 0
#define INIT_ADC(ADC_INTERFACE) \
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



#endif /* LIB_API_INIT_API_DEF_H_ */
