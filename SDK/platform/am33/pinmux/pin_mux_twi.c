/*
 * pin_mux_twi.c
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */
/*#####################################################*/
#include "pin_mux_twi.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_control_AM335x.h"
#include "include/hw/hw_types.h"
/*#####################################################*/
unsigned int pin_mux_twi_sda_beaglebone(unsigned int TwiNr)
{
	switch(TwiNr)
	{
	case 0:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C0_SDA)  =
               (CONTROL_CONF_I2C0_SDA_CONF_I2C0_SDA_RXACTIVE  |
                CONTROL_CONF_I2C0_SDA_CONF_I2C0_SDA_SLEWCTRL  |
                CONTROL_CONF_I2C0_SDA_CONF_I2C0_SDA_PUTYPESEL |
                CONTROL_CONF_MUXMODE(0));
        break;
	case 1:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS0) =
                   (CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_PUTYPESEL |
                    CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_RXACTIVE  |
                    CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_SLEWCTRL  |
                    CONTROL_CONF_MUXMODE(2));
        break;
	case 2:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(1)) =
                   (CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_PUTYPESEL |
                	CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_RXACTIVE  |
                	CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_SLEWCTRL  |
                    CONTROL_CONF_MUXMODE(3));
        break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_twi_scl_beaglebone(unsigned int TwiNr)
{
	switch(TwiNr)
	{
	case 0:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C0_SCL)  =
               (CONTROL_CONF_I2C0_SCL_CONF_I2C0_SCL_RXACTIVE  |
                CONTROL_CONF_I2C0_SCL_CONF_I2C0_SCL_SLEWCTRL  |
                CONTROL_CONF_I2C0_SCL_CONF_I2C0_SCL_PUTYPESEL |
                CONTROL_CONF_MUXMODE(0));
        break;
	case 1:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D1)  =
                   (CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_PUTYPESEL |
                    CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_RXACTIVE  |
                    CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_SLEWCTRL  |
                    CONTROL_CONF_MUXMODE(2));
        break;
	case 2:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(1))  =
                   (CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_PUTYPESEL |
                	CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_RXACTIVE  |
                	CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_SLEWCTRL  |
                    CONTROL_CONF_MUXMODE(3));
        break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_twi0_sda(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Twi0_Sda_PinMux_C17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C0_SDA)  =
               (CONTROL_CONF_I2C0_SDA_CONF_I2C0_SDA_RXACTIVE  |
                CONTROL_CONF_I2C0_SDA_CONF_I2C0_SDA_SLEWCTRL  |
                CONTROL_CONF_I2C0_SDA_CONF_I2C0_SDA_PUTYPESEL |
                CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 0;
}
/*#####################################################*/
unsigned int pin_mux_twi0_scl(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Twi0_Scl_PinMux_C16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C0_SCL)  =
               (CONTROL_CONF_I2C0_SCL_CONF_I2C0_SCL_RXACTIVE  |
                CONTROL_CONF_I2C0_SCL_CONF_I2C0_SCL_SLEWCTRL  |
                CONTROL_CONF_I2C0_SCL_CONF_I2C0_SCL_PUTYPESEL |
                CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 0;
}
/*#####################################################*/
unsigned int pin_mux_twi1_sda(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Twi1_Sda_PinMux_H17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_CRS)  =
               (CONTROL_CONF_MII1_CRS_CONF_MII1_CRS_RXACTIVE  |
                CONTROL_CONF_MII1_CRS_CONF_MII1_CRS_SLEWCTRL  |
                CONTROL_CONF_MII1_CRS_CONF_MII1_CRS_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi1_Sda_PinMux_E18:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(0))  =
               (CONTROL_CONF_UART0_CTSN_CONF_UART0_CTSN_RXACTIVE  |
                CONTROL_CONF_UART0_CTSN_CONF_UART0_CTSN_SLEWCTRL  |
                CONTROL_CONF_UART0_CTSN_CONF_UART0_CTSN_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi1_Sda_PinMux_D16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1))  =
               (CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_RXACTIVE  |
                CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_SLEWCTRL  |
                CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi1_Sda_PinMux_B16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D1)  =
               (CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_RXACTIVE  |
                CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_SLEWCTRL  |
                CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_PUTYPESEL |
                CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 0;
}
/*#####################################################*/
unsigned int pin_mux_twi1_scl(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Twi1_Scl_PinMux_J15:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXERR)  =
               (CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_RXACTIVE  |
                CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_SLEWCTRL  |
                CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi1_Scl_PinMux_E17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(0))  =
               (CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_RXACTIVE  |
                CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_SLEWCTRL  |
                CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi1_Scl_PinMux_D15:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(1))  =
               (CONTROL_CONF_UART1_TXD_CONF_UART1_TXD_RXACTIVE  |
                CONTROL_CONF_UART1_TXD_CONF_UART1_TXD_SLEWCTRL  |
                CONTROL_CONF_UART1_TXD_CONF_UART1_TXD_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi1_Scl_PinMux_A16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS0)  =
               (CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_RXACTIVE  |
                CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_SLEWCTRL  |
                CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_PUTYPESEL |
                CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 0;
}
/*#####################################################*/
unsigned int pin_mux_twi2_sda(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Twi2_Sda_PinMux_E15:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1))  =
               (CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_RXACTIVE  |
                CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_SLEWCTRL  |
                CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi2_Sda_PinMux_D18:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(1))  =
               (CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_RXACTIVE  |
                CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_SLEWCTRL  |
                CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi2_Sda_PinMux_A17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_SCLK)  =
               (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE  |
                CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_SLEWCTRL  |
                CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL |
                CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 0;
}
/*#####################################################*/
unsigned int pin_mux_twi2_scl(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Twi2_Scl_PinMux_E16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(1))  =
               (CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_RXACTIVE  |
                CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_SLEWCTRL  |
                CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi2_Scl_PinMux_D17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(1))  =
               (CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_RXACTIVE  |
                CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_SLEWCTRL  |
                CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_PUTYPESEL |
                CONTROL_CONF_MUXMODE(3));
		break;
	case Twi2_Scl_PinMux_B17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D0)  =
               (CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_RXACTIVE  |
                CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_SLEWCTRL  |
                CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_PUTYPESEL |
                CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 0;
}
/*#####################################################*/
unsigned int I2CPinMuxSetup(unsigned int instance)
{
	pin_mux_twi_sda_beaglebone(instance);
	pin_mux_twi_scl_beaglebone(instance);
	return 1;
}
/*#####################################################*/

