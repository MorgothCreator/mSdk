/*
 * board_init.c
 *
 *  Created on: Dec 4, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include <stdbool.h>
#include <string.h>
#include "board_init.h"
#include "api/core_init_api.h"
#include "clk/clk_core.h"
#include "driver/control_module.h"
//#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "device/ft5x06.h"
#include "device/ADXL345.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_uart* Uart1 = NULL;
new_uart* Uart2 = NULL;
new_uart* Uart3 = NULL;
new_twi* TWI0 = NULL;
new_twi* TWI1 = NULL;
new_twi* TWI2 = NULL;
new_twi* TWI3 = NULL;
new_gpio* HARDBTN1 = NULL;
new_gpio* HARDBTN2 = NULL;
new_gpio* HARDBTN3 = NULL;
new_gpio* HARDBTN4 = NULL;
new_gpio* HARDBTN5 = NULL;
new_gpio* HARDBTN6 = NULL;
new_gpio* LED1 = NULL;
new_gpio* LED2 = NULL;
new_gpio* LED3 = NULL;
new_gpio* LED4 = NULL;
new_gpio* LED5 = NULL;
new_gpio* LED6 = NULL;
new_adxl345* ADXL345_1 = NULL;
/*-----------------------------------------------------*/
new_touchscreen* TouchScreen = NULL;
new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
/*-----------------------------------------------------*/
/*-----------------------------------------------------*/
/*#####################################################*/
bool board_init()
{
	core_init();

/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->TxPin = g28_uart3_tx_irtx;
	DebugCom->RxPin = g27_uart3_rx_irrx;
	DebugCom->BaudRate = 115200;
	DebugCom->RxBuffSize = 256;
	DebugCom->TxBuffSize = 256;
	DebugCom->rxFifoTrigLevel = 1;
	DebugCom->txFifoTrigLevel = 1;
	DebugCom->UartNr = 2;
	uart_open(DebugCom);
/*-----------------------------------------------------*/
	/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
	LED1 = gpio_assign(pin_mux_convert_to_port_pin(110), GPIO_DIR_OUTPUT, false);
	LED2 = gpio_assign(pin_mux_convert_to_port_pin(8), GPIO_DIR_OUTPUT, false);
	HARDBTN1 = gpio_assign(1, 4, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI0 = new_(new_twi);
	TWI0->SdaPin = ae26_i2c1_sda;
	TWI0->SclPin = ae28_i2c1_scl;
	TWI0->BaudRate = 100000;
	TWI0->TwiNr = 0;
	TWI0->Priority = 0;
	TWI0->WithInterrupt = false;
	TWI0->RxBuffSize = 258;
	TWI0->TxBuffSize = 258;
	twi_open(TWI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 1 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI1 = new_(new_twi);
	TWI1->SdaPin = d26_i2c2_sda;
	TWI1->SclPin = c26_i2c2_scl;
	TWI1->BaudRate = 100000;
	TWI1->TwiNr = 1;
	TWI1->Priority = 0;
	TWI1->WithInterrupt = false;
	TWI1->RxBuffSize = 258;
	TWI1->TxBuffSize = 258;
	twi_open(TWI1);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 2 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI2 = new_(new_twi);
	TWI2->SdaPin = y27_i2c3_sda;
	TWI2->SclPin = w27_i2c3_scl;
	TWI2->BaudRate = 100000;
	TWI2->TwiNr = 2;
	TWI2->Priority = 0;
	TWI2->WithInterrupt = false;
	TWI2->RxBuffSize = 258;
	TWI2->TxBuffSize = 258;
	twi_open(TWI2);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	UARTPuts(DebugCom, "Setup TWI 3 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI3 = new_(new_twi);
	TWI3->SdaPin = ah22_i2c4_sda;
	TWI3->SclPin = ag21_i2c4_scl;
	TWI3->BaudRate = 100000;
	TWI3->TwiNr = 3;
	//TWI3->Priority = 0;
	TWI3->WithInterrupt = false;
	TWI3->RxBuffSize = 258;
	TWI3->TxBuffSize = 258;
	twi_open(TWI3);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	ADXL345_1 = new_(new_adxl345);
	ADXL345_1->MasterSlaveAddr = ADXL345_SLAVE_ADDR_ALT_0;
	ADXL345_1->TwiStruct = TWI3;
	ADXL345_1->Calibration_X = -14;
	ADXL345_1->Calibration_Y = 11;
	ADXL345_1->Calibration_Z = 24;
	ADXL345_1->FilterBuffSize = 4;
	if(adxl345_init(ADXL345_1))
	{
		UARTPuts(DebugCom, "ADXL345 detected on TWI3.\n\r" , -1);
	}
	else
	{
		free(ADXL345_1);
		UARTPuts(DebugCom, "ADXL345 not detected.\n\r" , -1);
	}
/*-----------------------------------------------------*/
	TouchScreen = new_(new_touchscreen);
	TouchScreen->TwiStruct = TWI1;
	if(ft5x06_init(TouchScreen, 1, 2))
	{
		ScreenBuff = new_(new_screen);
		ScreenBuff->LcdType = AT070TN92;
		ScreenBuff->BackLightPort = 3;
		ScreenBuff->BackLightPin = 17;
		//screen_init(ScreenBuff);
		UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, 24 Bit/pixel.\n\r" , ScreenBuff->Width, ScreenBuff->Height);
		screen_backlight_on(ScreenBuff);
		TouchScreen->screen_max_x = ScreenBuff->Width;
		TouchScreen->screen_max_y = ScreenBuff->Height;
		TouchScreen->pDisplay = ScreenBuff;
		UARTPuts(DebugCom, "Capacitive touch screen detected and set up successful.\n\r" , -1);
		UARTPuts(DebugCom, "I suppose that is PandaBoard Expansion from Chipsee.\n\r" , -1);

		HARDBTN2 = gpio_assign(1, 4, GPIO_DIR_INPUT, false);
		HARDBTN3 = gpio_assign(1, 0, GPIO_DIR_INPUT, false);
		HARDBTN4 = gpio_assign(1, 6, GPIO_DIR_INPUT, false);
		HARDBTN5 = gpio_assign(1, 1, GPIO_DIR_INPUT, false);
		HARDBTN6 = gpio_assign(1, 5, GPIO_DIR_INPUT, false);
	}
/*-----------------------------------------------------*/
/*	cm_muxmode_set(j27_gpio_0);//DVI-D Transmitter (TFP410) Power Down
	cm_muxmode_set(h27_gpio_1);//Ethernet/USB Hub LDO Power Enable
	cm_muxmode_set(l27_gpio_2);//Board ID Bit 4
	cm_muxmode_set(k27_gpio_3);//Board ID Bit 3
	cm_muxmode_set(m2_dpm_emu0);//JTAG Emulator 0 I/O
	cm_muxmode_set(n2_dpm_emu1);//JTAG Emulator 1 I/O
	cm_muxmode_set(v1_dispc2_data10);//Parallel Display Data Bit 10
	cm_muxmode_set(v2_dispc2_data9);//Parallel Display Data Bit 9
	cm_muxmode_set(w1_dispc2_data16);//Parallel Display Data Bit 16
	cm_muxmode_set(w2_dispc2_data17);//Parallel Display Data Bit 17
	cm_muxmode_set(w3_dispc2_hsync);//Parallel Display Horizontal Sync
	cm_muxmode_set(w4_dispc2_pclk);//Parallel Display Pixel Clock
	cm_muxmode_set(y2_dispc2_vsync);//Parallel Display Vertical Sync
	cm_muxmode_set(y3_dispc2_de);//Parallel Display Data Enable
	cm_muxmode_set(y4_dispc2_data8);//Parallel Display Data Bit 8
	cm_muxmode_set(aa1_dispc2_data7);//Parallel Display Data Bit 7
	cm_muxmode_set(aa2_dispc2_data6);//Parallel Display Data Bit 6
	cm_muxmode_set(aa3_dispc2_data5);//Parallel Display Data Bit 5
	cm_muxmode_set(aa4_dispc2_data4);//Parallel Display Data Bit 4
	cm_muxmode_set(ab2_dispc2_data3);//Parallel Display Data Bit 3
	cm_muxmode_set(ab3_dispc2_data2);//Parallel Display Data Bit 2
	cm_muxmode_set(a18_gpio_41);//HDMI Transceiver Level Shifter Output Enable
	cm_muxmode_set(a19_gpio_43);//Wireless LAN Enable
	cm_muxmode_set(a21_gpio_46);//Bluetooth Wakeup
	cm_muxmode_set(c20_gpio_48);//Board ID Bit 1
	cm_muxmode_set(d20_gpio_49);//Bluetooth Wakeup
	cm_muxmode_set(c22_gpio_53);//WLAN Interrupt In
	cm_muxmode_set(d22_gpio_60);//HDMI DC/DC-Hot Plug Enable
	cm_muxmode_set(b23_gpio_62);//Ethernet/USB Hub Reset
	cm_muxmode_set(b9_hdmi_hpd);//HDMI Hot Plug Detect
	cm_muxmode_set(b10_hdmi_cec);//HDMI Consumer Electronic Control
	cm_muxmode_set(a8_hdmi_ddc_scl);//Display Data Channel Serial Clock
	cm_muxmode_set(b8_hdmi_ddc_sda);//Display Data Channel Serial Data
	cm_muxmode_set(ae18_usbb1_ulpiphy_clk);//USBB1 ULPI Clock (Hub)
	cm_muxmode_set(ag19_usbb1_ulpiphy_stp);//USBB1 ULPI Stop (Hub)
	cm_muxmode_set(af19_usbb1_ulpiphy_dir);//USBB1 ULPI Direction (Hub)
	cm_muxmode_set(ae19_usbb1_ulpiphy_nxt);//USBB1 ULPI Next (Hub)
	cm_muxmode_set(af18_usbb1_ulpiphy_dat0);//USBB1 ULPI Data Bit 0 (Hub)
	cm_muxmode_set(ag18_usbb1_ulpiphy_dat1);//USBB1 ULPI Data Bit 1 (Hub)
	cm_muxmode_set(ae17_usbb1_ulpiphy_dat2);//USBB1 ULPI Data Bit 2 (Hub)
	cm_muxmode_set(af17_usbb1_ulpiphy_dat3);//USBB1 ULPI Data Bit 3 (Hub)
	cm_muxmode_set(ah17_usbb1_ulpiphy_dat4);//USBB1 ULPI Data Bit 4 (Hub)
	cm_muxmode_set(ae16_usbb1_ulpiphy_dat5);//USBB1 ULPI Data Bit 5 (Hub)
	cm_muxmode_set(af16_usbb1_ulpiphy_dat6);//USBB1 ULPI Data Bit 6 (Hub)
	cm_muxmode_set(ag16_usbb1_ulpiphy_dat7);//USBB1 ULPI Data Bit 7 (Hub)
	cm_muxmode_set(d2_sdmmc1_clk);//SDMMC Card Cage Clock (1.8v/3.0V)
	cm_muxmode_set(e3_sdmmc1_cmd);//SDMMC Card Cage Command (1.8v/3.0V)
	cm_muxmode_set(a24_gpio_101);//Board ID Input Bit 1
	cm_muxmode_set(e4_sdmmc1_dat0);//SDMMC Card Cage Data Bit 0 (1.8v/3.0V)
	cm_muxmode_set(e2_sdmmc1_dat1);//SDMMC Card Cage Data Bit 1 (1.8v/3.0V)
	cm_muxmode_set(e1_sdmmc1_dat2);//SDMMC Card Cage Data Bit 2 (1.8v/3.0V)
	cm_muxmode_set(f4_sdmmc1_dat3);//SDMMC Card Cage Data Bit 3 (1.8v/3.0V)
	cm_muxmode_set(f3_sdmmc1_dat4);//SDMMC Card Cage Data Bit 4 (1.8v/3.0V)
	cm_muxmode_set(f1_sdmmc1_dat5);//SDMMC Card Cage Data Bit 5 (1.8v/3.0V)
	cm_muxmode_set(g4_sdmmc1_dat6);//SDMMC Card Cage Data Bit 6 (1.8v/3.0V)
	cm_muxmode_set(g3_sdmmc1_dat7);//SDMMC Card Cage Data Bit 7 (1.8v/3.0V)
	cm_pin_set(ad27_gpio_110, false, 0, false);//LED D1 On/Off Control
	cm_muxmode_set(ac28_gpio_113);//Input from push-button switch S2
	cm_muxmode_set(ac26_abe_mcbsp1_clkx);//BT Audio I2S Clock
	cm_muxmode_set(ac25_abe_mcbsp1_dr);//BT Audio I2S Data Out
	cm_muxmode_set(ab25_abe_mcbsp1_dx);//BT Audio I2S Data In
	cm_muxmode_set(ac27_abe_mcbsp1_fsx);//BT Audio I2S Frame Sync
	cm_muxmode_set(ah26_abe_clks);//Clock input from TWL6030 Audio Companion IC
	cm_muxmode_set(ag24_dmtimer11_pwm_evt);//Display Expansion Connector(s) PWM Control
	cm_muxmode_set(ah24_gpio_122);//DVI-D Monitor Sense Input
	cm_muxmode_set(ab26_uart2_cts);//Bluetooth Host Control Interface Request to Send
	cm_muxmode_set(ab27_uart2_rts);//Bluetooth Host Control Interface Clear to Send
	cm_muxmode_set(aa25_uart2_rx);//Bluetooth Host Control Interface Transmit Data
	cm_muxmode_set(aa26_uart2_tx);//Bluetooth Host Control Interface Receive Data
	cm_muxmode_set(aa27_gpio_127);//TWL6040 Audio Power On
	//cm_muxmode_set(c26_i2c2_scl);//I2C2 Serial Clock
	//cm_muxmode_set(d26_i2c2_sda);//I2C2 Serial Data
	//cm_muxmode_set(w27_i2c3_scl);//I2C3 Serial Clock
	//cm_muxmode_set(y27_i2c3_sda);//I2C3 Serial Data
	//cm_muxmode_set(ag21_i2c4_scl);//I2C4 Serial Clock
	//cm_muxmode_set(ah22_i2c4_sda);//I2C4 Serial Data
	cm_muxmode_set(f27_uart3_cts_rctx);//Debug Terminal RS-232 Clear to Send
	cm_muxmode_set(f28_uart3_rts_sd);//Debug Terminal RS-232 Request to Send
	//cm_muxmode_set(g27_uart3_rx_irrx);//Debug Terminal RS-232 Receive Data
	//cm_muxmode_set(g28_uart3_tx_irtx);//Debug Terminal RS-232 Transmit Data
	cm_muxmode_set(ae5_sdmmc5_clk);//WLAN SDIO Clock Line
	cm_muxmode_set(af5_sdmmc5_cmd);//WLAN SDIO Command Line
	cm_muxmode_set(ae4_sdmmc5_dat0);//WLAN SDIO Data Bit 0
	cm_muxmode_set(af4_sdmmc5_dat1);//WLAN SDIO Data Bit 1
	cm_muxmode_set(ag3_sdmmc5_dat2);//WLAN SDIO Data Bit 2
	cm_muxmode_set(af3_sdmmc5_dat3);//WLAN SDIO Data Bit 3
	cm_muxmode_set(af12_dispc2_data23);//Parallel Display Data Bit 23
	cm_muxmode_set(ae12_dispc2_data22);//Parallel Display Data Bit 22
	cm_muxmode_set(ag13_dispc2_data21);//Parallel Display Data Bit 21
	cm_muxmode_set(ae11_dispc2_data20);//Parallel Display Data Bit 20
	cm_muxmode_set(af11_dispc2_data19);//Parallel Display Data Bit 19
	cm_muxmode_set(ag11_dispc2_data18);//Parallel Display Data Bit 18
	cm_muxmode_set(ah11_dispc2_data15);//Parallel Display Data Bit 15
	cm_muxmode_set(ae10_dispc2_data14);//Parallel Display Data Bit 14
	cm_muxmode_set(af10_dispc2_data13);//Parallel Display Data Bit 13
	cm_muxmode_set(ag10_dispc2_data12);//Parallel Display Data Bit 12
	cm_muxmode_set(ae9_dispc2_data11);//Parallel Display Data Bit 11
	cm_muxmode_set(g26_gpio_171);//Board ID Bit 2
	cm_muxmode_set(b5_usba0_otg_dp);//Internal USB Transceiver Data Plus
	cm_muxmode_set(b4_usba0_otg_dm);//Internal USB Transceiver Data Minus
	cm_muxmode_set(aa28_fref_clk1_out);//Camera Expansion Connector
	cm_muxmode_set(y28_gpio_182);//Board ID Bit 0
	cm_muxmode_set(af6_sys_nirq2);//System Interrupt from TWL6040 Audio IC
	cm_muxmode_set(f26_sys_boot0);//SYSBOOT Input 0
	cm_muxmode_set(e27_sys_boot1);//SYSBOOT Input 1
	cm_muxmode_set(e26_sys_boot2);//SYSBOOT Input 2
	cm_muxmode_set(e25_sys_boot3);//SYSBOOT Input 3
	cm_muxmode_set(d28_sys_boot4);//SYSBOOT Input 4
	cm_muxmode_set(d27_sys_boot5);//SYSBOOT Input 5
	cm_muxmode_set(ab4_dispc2_data1);//Parallel Display Data Bit 1
	cm_muxmode_set(ac4_dispc2_data0);//Parallel Display Data Bit 0
	cm_muxmode_set(ag8_fref_slicer_in);//38.4MHz Clock Input
	cm_muxmode_set(ad2_sys_drm_msecure);//MSECURE output to TWL6030 (RTC related)
	cm_muxmode_set(ac2_gpio_wk7);//TPS62361 VSEL0 control
	cm_pin_set(ac3_gpio_wk8, false, 0 , false);//LED D2 On/Off Control
	cm_muxmode_set(af8_sys_boot6);//SYSBOOT Input 6
	cm_muxmode_set(ae8_sys_boot7);//SYSBOOT Input 7
	cm_muxmode_set(ad4_fref_clk3_out);//USBB1 Phy Reference Clock (19.2 MHz)*/

	return true;
}


