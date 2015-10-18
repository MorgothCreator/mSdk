/*
 * pin_mux_spi.c
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */
/*#####################################################*/
#include "pin_mux_spi.h"
#include "api/gpio_def.h"
#include "pin_mux_gpio.h"
/*#####################################################*/
unsigned int pin_mux_spi_sclk_beaglebone(unsigned int SpiNr)
{
	switch(SpiNr)
	{
	case 0:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_SCLK) =
             (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL |
                  CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	case 1:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_ACLKX) =
             (CONTROL_CONF_MCASP0_ACLKX_CONF_MCASP0_ACLKX_PUTYPESEL |
                  CONTROL_CONF_MCASP0_ACLKX_CONF_MCASP0_ACLKX_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 1;
}

unsigned int pin_mux_spi_d0_beaglebone(unsigned int SpiNr)
{
	switch(SpiNr)
	{
	case 0:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D0) =
             (CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_PUTYPESEL |
                  CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	case 1:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_FSX) =
             (CONTROL_CONF_MCASP0_FSX_CONF_MCASP0_FSX_PUTYPESEL |
                  CONTROL_CONF_MCASP0_FSX_CONF_MCASP0_FSX_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 1;
}

unsigned int pin_mux_spi_d1_beaglebone(unsigned int SpiNr)
{
	switch(SpiNr)
	{
	case 0:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D1) =
             (CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_PUTYPESEL |
                  CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	case 1:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AXR0) =
             (CONTROL_CONF_MCASP0_AXR0_CONF_MCASP0_AXR0_PUTYPESEL |
                  CONTROL_CONF_MCASP0_AXR0_CONF_MCASP0_AXR0_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 1;
}

unsigned int pin_mux_spi_cs0_beaglebone(unsigned int SpiNr)
{
	switch(SpiNr)
	{
	case 0:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS0) =
             (CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_PUTYPESEL |
                  CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	case 1:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AHCLKR) =
             (CONTROL_CONF_MCASP0_AHCLKR_CONF_MCASP0_AHCLKR_PUTYPESEL |
                  CONTROL_CONF_MCASP0_AHCLKR_CONF_MCASP0_AHCLKR_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	default:
		return 0;
	}
	return 1;
}

unsigned int pin_mux_spi_cs1_beaglebone(unsigned int SpiNr)
{
	switch(SpiNr)
	{
	case 0:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS1) =
             (CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_PUTYPESEL |
                  CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 1;
}
/**********************************************/
unsigned int pin_mux_spi0_sclk(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi0_Sclk_PinMux_A17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_SCLK) =
             (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL |
                  CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi0_d0(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi0_D0_PinMux_B17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D0) =
             (CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_PUTYPESEL |
                  CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi0_d1(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi0_D1_PinMux_B16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D1) =
             (CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_PUTYPESEL |
                  CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi0_cs0(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi0_CS0_PinMux_A16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS0) =
             (CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_PUTYPESEL |
                  CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi0_cs1(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi0_CS1_PinMux_C15:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS1) =
             (CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_PUTYPESEL |
                  CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_RXACTIVE |
                  CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 0;
}
/**********************************************/
unsigned int pin_mux_spi1_sclk(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi1_Sclk_PinMux_H16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_COL) =
             (CONTROL_CONF_MII1_COL_CONF_MII1_COL_PUTYPESEL |
                  CONTROL_CONF_MII1_COL_CONF_MII1_COL_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_Sclk_PinMux_A13:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_ACLKX) =
             (CONTROL_CONF_MCASP0_ACLKX_CONF_MCASP0_ACLKX_PUTYPESEL |
                  CONTROL_CONF_MCASP0_ACLKX_CONF_MCASP0_ACLKX_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_Sclk_PinMux_C18:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_ECAP0_IN_PWM0_OUT) =
             (CONTROL_CONF_ECAP0_IN_PWM0_OUT_CONF_ECAP0_IN_PWM0_OUT_PUTYPESEL |
                  CONTROL_CONF_ECAP0_IN_PWM0_OUT_CONF_ECAP0_IN_PWM0_OUT_RXACTIVE |
                  CONTROL_CONF_MUXMODE(4));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi1_d0(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi1_D0_PinMux_H17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_CRS) =
             (CONTROL_CONF_MII1_CRS_CONF_MII1_CRS_PUTYPESEL |
                  CONTROL_CONF_MII1_CRS_CONF_MII1_CRS_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_D0_PinMux_B13:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_FSX) =
             (CONTROL_CONF_MCASP0_FSX_CONF_MCASP0_FSX_PUTYPESEL |
                  CONTROL_CONF_MCASP0_FSX_CONF_MCASP0_FSX_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_D0_PinMux_E18:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(0)) =
             (CONTROL_CONF_UART0_CTSN_CONF_UART0_CTSN_PUTYPESEL |
                  CONTROL_CONF_UART0_CTSN_CONF_UART0_CTSN_RXACTIVE |
                  CONTROL_CONF_MUXMODE(4));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi1_d1(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi1_D1_PinMux_J15:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXERR) =
             (CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_PUTYPESEL |
                  CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_D1_PinMux_D12:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AXR0) =
             (CONTROL_CONF_MCASP0_AXR0_CONF_MCASP0_AXR0_PUTYPESEL |
                  CONTROL_CONF_MCASP0_AXR0_CONF_MCASP0_AXR0_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_D1_PinMux_E17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(0)) =
             (CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_PUTYPESEL |
                  CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_RXACTIVE |
                  CONTROL_CONF_MUXMODE(4));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi1_cs0(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi1_CS0_PinMux_E15:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(0)) =
             (CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_PUTYPESEL |
                  CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_RXACTIVE |
                  CONTROL_CONF_MUXMODE(1));
		break;
	case Spi1_CS0_PinMux_E17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(0)) =
             (CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_PUTYPESEL |
                  CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_RXACTIVE |
                  CONTROL_CONF_MUXMODE(5));
		break;
	case Spi1_CS0_PinMux_H18:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_RMII1_REFCLK) =
             (CONTROL_CONF_RMII1_REFCLK_CONF_RMII1_REFCLK_PUTYPESEL |
                  CONTROL_CONF_RMII1_REFCLK_CONF_RMII1_REFCLK_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_CS0_PinMux_D18:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(1)) =
             (CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_PUTYPESEL |
                  CONTROL_CONF_UART1_CTSN_CONF_UART1_CTSN_RXACTIVE |
                  CONTROL_CONF_MUXMODE(4));
		break;
	case Spi1_CS0_PinMux_C12:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MCASP0_AHCLKR) =
             (CONTROL_CONF_MCASP0_AHCLKR_CONF_MCASP0_AHCLKR_PUTYPESEL |
                  CONTROL_CONF_MCASP0_AHCLKR_CONF_MCASP0_AHCLKR_RXACTIVE |
                  CONTROL_CONF_MUXMODE(3));
		break;
	default:
		return 0;
	}
	return 0;
}

unsigned int pin_mux_spi1_cs1(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Spi1_CS1_PinMux_E16:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(0)) =
             (CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_PUTYPESEL |
                  CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_RXACTIVE |
                  CONTROL_CONF_MUXMODE(1));
		break;
	case Spi1_CS1_PinMux_D17:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(1)) =
             (CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_PUTYPESEL |
                  CONTROL_CONF_UART1_RTSN_CONF_UART1_RTSN_RXACTIVE |
                  CONTROL_CONF_MUXMODE(4));
		break;
	case Spi1_CS1_PinMux_C18:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_ECAP0_IN_PWM0_OUT) =
             (CONTROL_CONF_ECAP0_IN_PWM0_OUT_CONF_ECAP0_IN_PWM0_OUT_PUTYPESEL |
                  CONTROL_CONF_ECAP0_IN_PWM0_OUT_CONF_ECAP0_IN_PWM0_OUT_RXACTIVE |
                  CONTROL_CONF_MUXMODE(2));
		break;
	case Spi1_CS1_PinMux_A15:
        HWREG(SOC_CONTROL_REGS + CONTROL_CONF_XDMA_EVENT_INTR(0)) =
             (CONTROL_CONF_XDMA_EVENT_INTR0_CONF_XDMA_EVENT_INTR0_PUTYPESEL |
            		 CONTROL_CONF_XDMA_EVENT_INTR0_CONF_XDMA_EVENT_INTR0_RXACTIVE |
                  CONTROL_CONF_MUXMODE(3));
		break;
	default:
		return 0;
	}
	return 0;
}

/**********************************************/
void pin_mux_spi_mosi(gpio_port_enum port, unsigned char pin)
{
	unsigned char mode = 0;
	if(port == IOA)
	{
		switch(pin)
		case 3:
			break;
	}
	else if(port == IOB)
	{

	}
	else if(port == IOC)
	{

	}
	else if(port == IOD)
	{

	}
	gpio_mux_setup(port, pin, mode, 1, 1, 1, 1);
}
/*#####################################################*/
void pin_mux_spi_miso(gpio_port_enum port, unsigned char pin)
{
	unsigned char mode = 0;
	if(port == IOA)
	{
		switch(pin)
		case 4:
			break;
	}
	else if(port == IOB)
	{

	}
	else if(port == IOC)
	{

	}
	else if(port == IOD)
	{

	}
	gpio_mux_setup(port, pin, mode, 1, 1, 1, 1);
}
/*#####################################################*/
void pin_mux_spi_sck(gpio_port_enum port, unsigned char pin)
{
	unsigned char mode = 0;
	if(port == IOA)
	{
		switch(pin)
		case 2:
			break;
	}
	else if(port == IOB)
	{

	}
	else if(port == IOC)
	{

	}
	else if(port == IOD)
	{

	}
	gpio_mux_setup(port, pin, mode, 1, 1, 1, 1);
}
/*#####################################################*/
void pin_mux_spi_cs0(gpio_port_enum port, unsigned char pin)
{
	unsigned char mode = 0;
	if(port == IOA)
	{
		switch(pin)
		case 5:
			break;
	}
	else if(port == IOB)
	{

	}
	else if(port == IOC)
	{

	}
	else if(port == IOD)
	{

	}
	gpio_mux_setup(port, pin, mode, 1, 1, 1, 1);
}
/*#####################################################*/
void pin_mux_spi_cs1(gpio_port_enum port, unsigned char pin)
{
	unsigned char mode = 0;
	if(port == IOA)
	{
		switch(pin)
		case 6:
			break;
	}
	else if(port == IOB)
	{

	}
	else if(port == IOC)
	{

	}
	else if(port == IOD)
	{

	}
	gpio_mux_setup(port, pin, mode, 1, 1, 1, 1);
}
/*#####################################################*/




