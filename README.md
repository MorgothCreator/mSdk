mSdk
====

Multiplatform SDK for standalone applications

This SDK is intended to help the developers that he want to develop standalone applications that can be compiled for different platforms with minimum changes of source code.

At this moment is supported the next platforms:

1) TI AM335x (Code Composer Studio(GCC only)).

  <a href="http://beagleboard.org/bone">BeagleBone Board.</a>
  
  <a href="http://beagleboard.org/black">BeagleBone Black Board.</a>
  
  <a href="http://www.embest-tech.com/product/evaluation-boards/devkit8600-evaluation-board.html">DevKit8600 Board.</a>
  
2) TI OMAP4 (Code Composer Studio).

  <a href="http://pandaboard.org/content/pandaboard-es">PandaBoardEs Board.</a>
  
3) AVR32AP7000 (AVR32 Studio).

  <a href="http://www.atmel.com/tools/MATURENGW100NETWORKGATEWAYKIT.aspx">NGW100 Board.</a>
  
4) ATxmega (Atmel Studio).

  <a href="http://devboardshop.com/en/media-development-boards/9-xmegamediaplayer-v1.html">XmegaMediaPlayer Board.</a>
  
5) AT32UC3L (Atmel Studio).

  <a href="http://www.atmel.com/tools/UC3-L0XPLAINED.aspx">Xplained Board.</a>
  
6) STM32F0 (CoIDE).

  --

7) STM32F10x (CoIDE).

  Custom board (see example app for details).

8) STM32F3 (CoIDE).

  --

9) STM32F4xx (CoIDE).

  <a href="http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF252419">Discovery Board.</a>
  
  <a href="http://www.mikroe.com/mikromedia/stm32-m4/">Mikromedia board.</a>
  
  <a href="https://www.olimex.com/Products/ARM/ST/STM32-H407/open-source-hardware">STM32-E407.</a>
  
  <a href="http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f469idiscovery.html">STM32F469I DISCOVERY (System Workbench for STM32).</a>
  
9) STM32F7xx (System Workbench for STM32).

  <a href="http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-discovery-kits/32f769idiscovery.html">STM32F769I DISCOVERY.</a>

10)
  SmartRF06EB (Code Composer Studio).
  
I added the settings for all example projects to help beginners to understand how it works, is necessary to clone mSdk into "C:\GitHub\" directory.

<a href="http://forum.devboardshop.com/">Detalied description link.</a>

Done API's for every platform:

	AM335x:
		EDMA.
		GPIO.
		MMCSD:
			uSD.
			MMC.
			eMMC.
		Touchscreen:
			Internal AD touchscreen.
			FT5x06 capacitive touchscreen.
		LCD.
		SPI.
		RTC.
		TWI.
		UART.
		USB:
			DEV MSC.
			HOST MSC.
			HOST Mouse.
	AT32AP7000:
		GPIO.
		MMCSD.
		LCD.
		RTC.
		TWI.
		UART.
	AT32UC3L:
		ADC.
		GPIO.
		UART.
	ATXMEGA:
		GPIO.
		MMCSD using USART in SPI mode.
		SPI.
		TWI.
		USART.
	OMAP4:
		GPIO.
		TWI.
		UART.
	STM32F0:
		In development.
	STM32F1:
		GPIO.
		USART.
		TWI.
		ADC.
	STM32F3:
		In development.
	STM32F4:
		GPIO.
		USART.
		TWI.
		SPI.
		MMCSD.
		USB DEV MSC.
		USB DEV CDC.
		USB HOST MSC.
		QSPI Memory.
		ADC.
	STM32F7:
		GPIO.
		USART.
		MMCSD.
		USB DEV MSC.
		USB DEV CDC.
		USB HOST MSC.
		DMA2D.
		LCD.
		DSI.
	CC13xx and CC26xx:
		GPIO.
		SPI.
		UART.

Supported devices (this drivers works on all platforms that has API's for used communicatiun busses):

	24c I2C EEProm device.
	45db SPI Flash memory.
	ADXL345 I2C accelerometer device.
	AR1020 I2C touchscreen device.
	BMP180 I2C barometer and temperature sensor device.
	DS1307 I2C RTC device.
	DXL actuators from Dynamixel.
	FT5x06 I2C capacitive touchscreen device.
	HIH6130-HIH6131 I2C hidrometer and temperature sensor device(only STM32F4 controllers for now).
	Lepton FLIR termal image sensor SPI device.
	MHC5883 I2C compass device.
	MMCSD_SPI a driver to support MMC and SD/SDHC memory card using SPI interface.
	MPU60x0 I2C accelerometer, giroscope and temperature sensor device.
	MS5611 I2C pressure & temperature sensor device.
	MPL3115A2 I2C altitude & temperature sensor device.
	MPR121 I2C touch sensor 12 buttons.
	MPU9150 I2C accelerometer, giroscope, compass(AK8975) and temperature sensor device.
	SHT11 custom software interface humidity and temperature device.
	SRF02 I2C ultrasonic range finder device.
	SST25VF SPI flash memory device.
	TPS65217 I2C PMIC device.
	TPS65910 I2C PMIC device.
	AK8975 I2C compass sensor device.
	HIH6130.
	ILI9341.
	LEPTON FLIR.
	MHC5883.
	MI0283.
	MMCSDSPI.
	MPR121.
	nRF24l01.
	OTM8009a.
	RFM12.
	VS10xx.
