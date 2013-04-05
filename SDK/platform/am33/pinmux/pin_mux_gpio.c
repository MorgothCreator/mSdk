/*
 * pin_mux_gpio.c
 *
 *  Created on: Nov 20, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "pin_mux_gpio.h"
#include "include/hw/hw_control_AM335x.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_cm_wkup.h"
#include "include/hw/hw_cm_per.h"
#include "include/hw/hw_types.h"
/*#####################################################*/
void gpio_mux_setup(unsigned char PortNr, unsigned char PinNr, unsigned char Mode)
{
	if(PortNr == 0)
	{
		switch(PinNr)
		{
			case 0:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_DATA)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 1:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_CLK)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 2:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_SCLK)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 3:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D0)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 4:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D1)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 5:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS0)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 6:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS1)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 7:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_ECAP0_IN_PWM0_OUT)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 8:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(12))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 9:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(13))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 10:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(14))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 11:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(15))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 12:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 13:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 14:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 15:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 16:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD3)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 17:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD2)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 18:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_USB_DRVVBUS(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 19:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_XDMA_EVENT_INTR(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 20:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_XDMA_EVENT_INTR(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 21:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD1)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 22:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(8))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
				break;
			case 23:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(9))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 26:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(10)) = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 27:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(11)) = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 28:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD0) = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 29:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_RMII1_REFCLK) = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 30:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WAIT0) = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 31:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WPN) = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
		}
	}
	else if(PortNr == 1)
	{
		if(PinNr <= 7)                 HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(PinNr))       = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
		else if(PinNr >= 12 || PinNr <= 15) HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(PinNr))       = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
		else if(PinNr >= 16 || PinNr <= 27) HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_A((PinNr - 16))) = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
		else
		{
			switch(PinNr)
			{
				case 8:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 9:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
					break;
				case 10:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 11:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
					break;
				case 28:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_BE1N)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
					break;
				case 29:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CSN(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
					break;
				case 30:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CSN(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
					break;
				case 31:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CSN(2))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) |(1 << 0x04) | (1 << 0x05);
					break;
			}
		}
	}
	else if(PortNr == 2)
	{
		if(PinNr >= 6 || PinNr <= 17) HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(PinNr-6))       = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
		else
		{
			switch(PinNr)
			{
				case 0:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CSN(3))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 1:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CLK)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 2:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_ADVN_ALE)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 3:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_OEN_REN)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 4:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WEN)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 5:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_BE0N_CLE)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 18:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD3)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 19:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD2)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 20:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD1)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 21:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD0)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 22:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_VSYNC)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 23:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_HSYNC)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 24:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_PCLK)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 25:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_AC_BIAS_EN)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 26:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT3)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 27:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT2)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 28:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT1)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 29:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT0)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 30:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CLK)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
				case 31:
					HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CMD)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
					break;
			}
		}
	}
	else if(PortNr == 3)
	{
		switch(PinNr)
		{
			case 0:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_COL)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 1:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_CRS)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 2:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXERR)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 3:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXEN)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 4:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXDV)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 5:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C0_SDA)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 6:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C0_SCL)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 7:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_EMU(0))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 8:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_EMU(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 9:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXCLK)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 10:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXCLK)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 13:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_USB_DRVVBUS(1))  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 14:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_ACLKX)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 15:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_FSX)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 16:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AXR0)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 17:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AHCLKR)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 18:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_ACLKR)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 19:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_FSR)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 20:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AXR1)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
			case 21:
				HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AHCLKX)  = CONTROL_CONF_MUXMODE(Mode) | (0 << 0x03) | (1 << 0x04)| (1 << 0x05);
				break;
		}
	}
}
/*#####################################################*/



