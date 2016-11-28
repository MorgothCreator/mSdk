/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */

#include <api/spi_api.h>
#include <api/spi_def.h>
#include <interface/mmcsd_interface.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "main.h"
#include "stm32f4xx_conf.h"

#include "board/STM32F469I-DISCO.h"

#include "sys/system_stm32f4xx.h"
#include "board_init.h"

#include "api/init_api_def.h"
#include "api/core_init_api.h"
#include "api/rtc_api.h"
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "api/adc_def.h"
#include "api/adc_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/mmcsd_api.h"
#include "api/usb_api.h"
#include "api/lan_api.h"
#include "api/i2s_def.h"
#include "api/i2s_api.h"
#include "device/ft5x06.h"
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
#include "device/mi0283.h"
#include "device/nrf24l01.h"
#include "device/vs10xx.h"
#include "device/mmcsd_spi.h"
#include "device/microe_touch.h"
#include "device/n25qxxx.h"

#include "lib/fat_fs/inc/ff_util.h"


/*#####################################################*/
new_uart* Uart[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
new_uart* VirtCom = NULL;
new_uart* DebugCom = NULL;
new_twi* TWI[3] = {NULL, NULL, NULL};
new_mcspi* SPI[3] = {NULL, NULL, NULL};
new_adc* _ADC[3] = {NULL, NULL, NULL};
new_gpio* LED[LEDS_NR] = {NULL};
new_gpio* HARDBTN[HARDBTNS_NR] = {NULL};
new_screen* Screen = NULL;

new_gpio* BAT_STAT = NULL;


new_gpio* LCD_BACKLIGHT = NULL;
new_gpio* LCD_DISPLAY_RST = NULL;
new_gpio* LCD_TOUCH_INT = NULL;
new_touchscreen * TOUCH = NULL;

new_gpio* SD_CD = NULL;

//NEW_SST25VF(SST25VF);
//FIL fs_file_settings;
NEW_MPU60x0_9150(MPU60x0_9150);
NEW_AK8975(AK8975);
NEW_BMP180(BMP180);
NEW_SHT11(SHT11);
NEW_SRF02(SRF02);
NEW_MHC5883(MHC5883);
NEW_MPL3115A2(MPL3115A2);
NEW_MS5611(MS5611);
NEW_ADXL345(ADXL345);
NEW_HIH613x(HIH613x);
NEW_MPR121(MPR121);
NEW_SX150x(SX150x);
NEW_LEPTON_FLIR(LEPTON_FLIR);
NEW_NRF24L01(NRF24L01);
NEW_N25Qxxx(N25Qxxx);
/*#####################################################*/
bool board_init()
{
	//RtcStruct.Rtc_ClkSource = _Rtc_Clk_Source_RCOSC_gc;
	core_init();
	timer_init();
	rtc_int_rtc_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	Uart[DEFAULT_UART_UNIT] = new_(new_uart);
	INIT_UART(DEFAULT_UART_UNIT, 115200, DEFAULT_UART_TX_PORT, DEFAULT_UART_TX_PIN, DEFAULT_UART_RX_PORT, DEFAULT_UART_RX_PIN);
	DebugCom = Uart[DEFAULT_UART_UNIT];
	/*VirtCom = new_(new_uart);
	DebugCom = VirtCom;//Uart[1];
	DebugCom->is_virtual = true;*/
	uart.open(DebugCom);

/*-----------------------------------------------------*/
/* Display board message*/
#if defined(BOARD_MESSAGE)
	//uart.putc(DebugCom, 0xFF);
	//uart.putc(DebugCom, 0xFF);
	//uart.putc(DebugCom, '\n');
	//uart.putc(DebugCom, '\r');
	uart.printf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with all on-board I2C devices */
	TWI[DEFAULT_I2C_UNIT] = new_(new_twi);
	INIT_TWI(DEFAULT_I2C_UNIT, 100000, DEFAULT_I2C_SCL_PORT, DEFAULT_I2C_SCL_PIN, DEFAULT_I2C_SDA_PORT, DEFAULT_I2C_SDA_PIN, 256, 256);
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with all on-board I2C devices */
	TWI[AUDIO_I2C_UNIT] = new_(new_twi);
	INIT_TWI(AUDIO_I2C_UNIT, 100000, AUDIO_I2C_SCL_PORT, AUDIO_I2C_SCL_PIN, AUDIO_I2C_SDA_PORT, AUDIO_I2C_SDA_PIN, 256, 256);
/*-----------------------------------------------------*/
	SPI[SPI_EXTENSION_UNIT] = new_(new_mcspi);
	/*SPI[DEFAULT_SPI_UNIT]->CsPort[1] = DEFAULT_SPI_CS_M25P80_PORT;
	SPI[DEFAULT_SPI_UNIT]->CsPin[1] = DEFAULT_SPI_CS_M25P80_PIN;
	SPI[DEFAULT_SPI_UNIT]->CsPort[2] = DEFAULT_SPI_CS_VS1053_CS_PORT;
	SPI[DEFAULT_SPI_UNIT]->CsPin[2] = DEFAULT_SPI_CS_VS1053_CS_PIN;
	SPI[DEFAULT_SPI_UNIT]->CsPort[3] = DEFAULT_SPI_CS_VS1053_DCS_PORT;
	SPI[DEFAULT_SPI_UNIT]->CsPin[3] = DEFAULT_SPI_CS_VS1053_DCS_PIN;*/
	INIT_SPI(SPI_EXTENSION_UNIT, 4, 8, SPI_EXTENSION_SCK_PORT, SPI_EXTENSION_SCK_PIN, SPI_EXTENSION_MOSI_PORT, SPI_EXTENSION_MOSI_PIN, SPI_EXTENSION_MISO_PORT, SPI_EXTENSION_MISO_PIN, SPI_EXTENSION_CS1_PORT, SPI_EXTENSION_CS1_PIN, 0);
	//INIT_LEPTON_FLIR(LEPTON_FLIR, LEPTON_SPI_INTERFACE_NR, LEPTON_SPI_INSTANCE_NR, LEPTON_TWI_INSTANCE_NR);
	//SPI[NRF24L01_SPI_UNIT] = new_(new_mcspi);
	//INIT_SPI(NRF24L01_SPI_UNIT, 3, 8, NRF24L01_SPI_SCK_PORT, NRF24L01_SPI_SCK_PIN, NRF24L01_SPI_MOSI_PORT, NRF24L01_SPI_MOSI_PIN, NRF24L01_SPI_MISO_PORT, NRF24L01_SPI_MISO_PIN, NRF24L01_SPI_CS0_PORT, NRF24L01_SPI_CS0_PIN, 0);
/*-----------------------------------------------------*/
	INIT_N25Qxxx(N25Qxxx, 0, &n25q128a);
	//n25qxxx.erase_chip(N25Qxxx);
	//unsigned char tmp_buff[512];
	//n25qxxx.read(N25Qxxx, tmp_buff, 0, sizeof(tmp_buff));
	//INIT_USB_DEV_MSC_TO_QSPI_N25Q_BRIDGE(0, QSPI_N25Q_PARAM, N25Qxxx);
	//usb_msc_dev_media_change_state(0, true);
/*-----------------------------------------------------*/
	INIT_MPU60x0_9150(MPU60x0_9150, DEFAULT_I2C_UNIT);
	MPU60x0_9150->XG_Cal = 28;
/*-----------------------------------------------------*/
	INIT_AK8975(AK8975, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	INIT_BMP180(BMP180, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	INIT_MS5611(MS5611, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	//INIT_SHT11(SHT11, IOB, 12, IOB, 13);
/*-----------------------------------------------------*/
	INIT_ADXL345(ADXL345, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	INIT_HIH613x(HIH613x, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	INIT_MPL3115A2(MPL3115A2, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	INIT_SX150x(SX150x, DEFAULT_I2C_UNIT, SX1509_ADDR1);
/*-----------------------------------------------------*/
	//INIT_MPR121(MPR121, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	INIT_MHC5883(MHC5883, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
	//INIT_SST25VF(SST25VF, DEFAULT_SPI_UNIT, 1, NULL, NULL);

	LCD_BACKLIGHT = gpio.assign(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, GPIO_OUT_PUSH_PULL, false);
	gpio.out(LCD_BACKLIGHT, 1);
	LCD_DISPLAY_RST = gpio.assign(LCD_DISPLAY_RST_PORT, LCD_DISPLAY_RST_PIN, GPIO_OUT_PUSH_PULL, false);
	LCD_TOUCH_INT = gpio.assign(LCD_TOUCH_INT_PORT, LCD_TOUCH_INT_PIN, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
	BAT_STAT = gpio.assign(BAT_STAT_PORT, BAT_STAT_PIN, GPIO_DIR_INPUT, false);
/* Set up the ADC 0 (VDD VOLT and INT TEMPERATURE)*/
	uart.puts(DebugCom, "Setup ADC 0....." , -1);
	_ADC[0] = new_(new_adc);
	_ADC[0]->Align = ADC_ALIGN_RIGHT;
	_ADC[0]->ContinuousMode = ADC_CONTINUOUS_MODE_SINGLE;
	_ADC[0]->ConvMode = ADC_CONV_MODE_MULTICHANNEL;
	_ADC[0]->DmaChannel = 0;
	_ADC[0]->DmaUnitNr = 1;
	_ADC[0]->ExtTrig = ADC_EXT_TRIG_NONE;
	_ADC[0]->IntRefEn = true;
	_ADC[0]->Mode = ADC_MODE_INDEPENDENT;
	_ADC[0]->TempSensorEn = true;
	_ADC[0]->UnitNr = 0;
	_ADC[0]->EnabledChannelsNr = 2;
	_ADC[0]->SampleTime[0] = ADC_SAMPLE_TIME_8;
	_ADC[0]->EnCh[0] = 9; // SIG ADC0
	_ADC[0]->SampleTime[1] = ADC_SAMPLE_TIME_8;
	_ADC[0]->EnCh[1] = 10;// SIG ADC1
	_ADC[0]->SampleTime[2] = ADC_SAMPLE_TIME_8;
	_ADC[0]->EnCh[2] = 1;// SIG ADC1
	//_ADC[0]->SampleTime[2] = ADC_SAMPLE_TIME_5;
	//_ADC[0]->EnCh[2] = 16;// SIG ADC1
	//_ADC[0]->SampleTime[3] = ADC_SAMPLE_TIME_6;
	//_ADC[0]->EnCh[3] = 17;// SIG ADC1
	if(adc_init(_ADC[0])) uart.puts(DebugCom, "OK.\n\r" , -1);
	else  uart.puts(DebugCom, "FAILED.\n\r" , -1);
	adc_start_conversion(_ADC[0]);
/*-----------------------------------------------------*/
/* Set up the ADC 1 (VDD VOLT and INT TEMPERATURE)*/
	/*UARTPuts(DebugCom, "Setup ADC 1....." , -1);
	_ADC[1] = new_(new_adc);
	_ADC[1]->Align = ADC_ALIGN_RIGHT;
	_ADC[1]->ContinuousMode = ADC_CONTINUOUS_MODE_CONTINUOUS;
	_ADC[1]->ConvMode = ADC_CONV_MODE_MULTICHANNEL;
	_ADC[1]->DmaChannel = 1;
	_ADC[1]->DmaUnitNr = 0;
	_ADC[1]->ExtTrig = ADC_EXT_TRIG_NONE;
	_ADC[1]->IntRefEn = true;
	_ADC[1]->Mode = ADC_MODE_INDEPENDENT;
	_ADC[1]->TempSensorEn = true;
	_ADC[1]->UnitNr = 0;
	_ADC[1]->EnCh[0] = 2;// VDD VOLT
	_ADC[1]->EnCh[1] = 17;// INT TEMPERATURE
	_ADC[1]->SampleTime[0] = ADC_SAMPLE_TIME_5;
	_ADC[1]->SampleTime[1] = ADC_SAMPLE_TIME_6;
	if(adc_init(_ADC[0])) UARTPuts(DebugCom, "OK.\n\r" , -1);
	else  UARTPuts(DebugCom, "FAILED.\n\r" , -1);*/
/*-----------------------------------------------------*/

	Screen = new_(new_screen);
	Screen->LcdTimings = &lcd_OTM8009A_PORTRAIT;
	Screen->BackLight = LCD_BACKLIGHT;
	lcd_set_params((void *)Screen);

	lcd_init(Screen);
	TOUCH = new_(new_touchscreen);
	TOUCH->pDisplay = Screen;
	TOUCH->TwiStruct = TWI[DEFAULT_I2C_UNIT];
	if(!ft5x06_init(TOUCH, IOJ, 5))
	{
		uart.puts(DebugCom, "Capacitive touch init failed.\n\r" , -1);
	}
	//microe_touch_calibrate(RES_TOUCH, Screen);

	uart.printf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , Screen->LcdTimings->X, Screen->LcdTimings->Y, Screen->LcdTimings->bus_size);
/*-----------------------------------------------------*/
/*-----------------------------------------------------*/
	INIT_ADXL345(ADXL345, DEFAULT_I2C_UNIT);
/*-----------------------------------------------------*/
#if defined(HARDBTN1_PORT) && defined(HARDBTN1_PIN)
	HARDBTN[0] = gpio.assign(HARDBTN1_PORT, HARDBTN1_PIN, GPIO_DIR_INPUT, false);
	gpio.up_dn(HARDBTN[0], 1);
#endif
#if defined(HARDBTN2_PORT) && defined(HARDBTN2_PIN)
	HARDBTN[1] = gpio_assign(HARDBTN2_PORT, HARDBTN2_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[1], 1);
#endif
#if defined(HARDBTN3_PORT) && defined(HARDBTN3_PIN)
	HARDBTN[2] = gpio_assign(HARDBTN3_PORT, HARDBTN3_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[2], 1);
#endif
#if defined(HARDBTN4_PORT) && defined(HARDBTN4_PIN)
	HARDBTN[3] = gpio_assign(HARDBTN4_PORT, HARDBTN4_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[3], 1);
#endif
/*-----------------------------------------------------*/
#if defined(LED1_PORT) && defined(LED1_PIN)
	LED[0] = gpio.assign(LED1_PORT, LED1_PIN, GPIO_DIR_OUTPUT, false);
	LED[0]->inverse = LED1_INVERSE;
#endif
#if defined(LED2_PORT) && defined(LED2_PIN)
	LED[1] = gpio.assign(LED2_PORT, LED2_PIN, GPIO_DIR_OUTPUT, false);
	LED[1]->inverse = LED2_INVERSE;
#endif
#if defined(LED3_PORT) && defined(LED3_PIN)
	LED[2] = gpio.assign(LED3_PORT, LED3_PIN, GPIO_DIR_OUTPUT, false);
	LED[2]->inverse = LED3_INVERSE;
#endif
#if defined(LED4_PORT) && defined(LED4_PIN)
	LED[3] = gpio.assign(LED4_PORT, LED4_PIN, GPIO_DIR_OUTPUT, false);
	LED[3]->inverse = LED4_INVERSE;
#endif
/*-----------------------------------------------------*/
	NRF24L01->rx_addr0[0] = 0x00;
	NRF24L01->rx_addr0[1] = 0x00;
	NRF24L01->rx_addr0[2] = 0x00;
	NRF24L01->rx_addr0[3] = 0x00;
	NRF24L01->rx_addr0[4] = 0x00;

	NRF24L01->rx_addr1[0] = 0x00;
	NRF24L01->rx_addr1[1] = 0x00;
	NRF24L01->rx_addr1[2] = 0x00;
	NRF24L01->rx_addr1[3] = 0x00;
	NRF24L01->rx_addr1[4] = 0x00;

	NRF24L01->tx_addr[0] = 0x00;
	NRF24L01->tx_addr[1] = 0x00;
	NRF24L01->tx_addr[2] = 0x00;
	NRF24L01->tx_addr[3] = 0x00;
	NRF24L01->tx_addr[4] = 0x00;
	INIT_NRF24L01(NRF24L01, SPI_EXTENSION_UNIT, 0, gpio.assign(NRF24L01_CE_PORT, NRF24L01_CE_PIN, GPIO_OUT_PUSH_PULL, false), gpio.assign(NRF24L01_IRQ_PORT, NRF24L01_IRQ_PIN, GPIO_IN_PULL_UP, false));
/*-----------------------------------------------------*/
#if USE_NARRATOR
	VS10XX = new_(new_vs10xx);
	vs10xx_init(VS10XX, SPI[DEFAULT_SPI_UNIT], DEFAULT_SPI_CS_VS1053_CS_INSTANCE, DEFAULT_SPI_CS_VS1053_DCS_INSTANCE, gpio_assign(VS11XX_RST_PORT, VS11XX_RST_PIN, GPIO_OUT_PUSH_PULL, false), gpio_assign(VS11XX_DREQ_PORT, VS11XX_DREQ_PIN, GPIO_IN_PULL_UP, false));
#endif
/*-----------------------------------------------------*/
	SD_CD = gpio.assign(SD_CD_PORT, SD_CD_PIN, GPIO_IN_PULL_UP, false);
	SD_CD->LastState = true;

	INIT_MMCSD(0, NULL, SD_CD, LED[3]);
	//INIT_USB_DEV_MSC_TO_MMCSD_SPI_BRIDGE(0, 0);

	INIT_USB_DEV_MSC_TO_MMCSD_BRIDGE(0, 0, MMCSD_SLAVE_CONTROLS, NULL, SD_CD, LED[3]);
	usb_msc_dev_media_change_state(0, true);
	//INIT_USB_MSC_HOST(0, LED[2]);
	return true;
}
