/*
 * pin_mux_uart.c
 *
 *  Created on: Aug 14, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */
/*#####################################################*/
#include <stdbool.h>
#include "pin_mux_uart.h"
#include "include/hw/soc_AM335x.h"
#include "include/hw/hw_control_AM335x.h"
#include "include/hw/hw_types.h"
/*#####################################################*/
unsigned int pin_mux_uart0_rx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart0_Rx_PinMux_B16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D1) =
								 (CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_SPI0_D1_CONF_SPI0_D1_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(4));
		break;
	case Uart0_Rx_PinMux_E15:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(0)) =
		         	 	 	 	 (CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	case Uart0_Rx_PinMux_D16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1)) =
		         	 	 	 	 (CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(5));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart0_tx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart0_Tx_PinMux_A16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS0) =
								 (CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_SPI0_CS0_CONF_SPI0_CS0_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(4));
		break;
	case Uart0_Tx_PinMux_E16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(0)) =
		         	 	 	 	 (CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	case Uart0_Tx_PinMux_D15:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1)) =
		         	 	 	 	 (CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(5));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart1_rx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart1_Rx_PinMux_D16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1)) =
		         	 	 	 	 (CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart1_tx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart1_Tx_PinMux_D15:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1)) =
		         	 	 	 	 (CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart2_rx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart2_Rx_PinMux_H17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_CRS) =
								 (CONTROL_CONF_MII1_CRS_CONF_MII1_CRS_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_CRS_CONF_MII1_CRS_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(6));
		break;
	case Uart2_Rx_PinMux_K18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXCLK) =
		         	 	 	 	 (CONTROL_CONF_MII1_TXCLK_CONF_MII1_TXCLK_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_TXCLK_CONF_MII1_TXCLK_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart2_Rx_PinMux_A17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_SCLK) =
		         	 	 	 	 (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart2_Rx_PinMux_G17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CLK) =
		         	 	 	 	 (CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart2_tx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart2_Tx_PinMux_J15:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXERR) =
								 (CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(6));
		break;
	case Uart2_Tx_PinMux_L18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXCLK) =
		         	 	 	 	 (CONTROL_CONF_MII1_RXCLK_CONF_MII1_RXCLK_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_RXCLK_CONF_MII1_RXCLK_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart2_Tx_PinMux_B17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D0) =
		         	 	 	 	 (CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart2_Tx_PinMux_G18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CMD) =
		         	 	 	 	 (CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart3_rx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart3_Rx_PinMux_L17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD3) =
								 (CONTROL_CONF_MII1_RXD3_CONF_MII1_RXD3_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_RXD3_CONF_MII1_RXD3_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart3_Rx_PinMux_C15:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS1) =
		         	 	 	 	 (CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_SPI0_CS1_CONF_SPI0_CS1_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart3_Rx_PinMux_G15:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT1) =
		         	 	 	 	 (CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart3_tx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart3_Tx_PinMux_L16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD2) =
								 (CONTROL_CONF_MII1_RXD2_CONF_MII1_RXD2_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_RXD2_CONF_MII1_RXD2_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart3_Tx_PinMux_C18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_ECAP0_IN_PWM0_OUT) =
		         	 	 	 	 (CONTROL_CONF_ECAP0_IN_PWM0_OUT_CONF_ECAP0_IN_PWM0_OUT_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_ECAP0_IN_PWM0_OUT_CONF_ECAP0_IN_PWM0_OUT_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart3_Tx_PinMux_G16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT1) =
		         	 	 	 	 (CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart4_rx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart4_Rx_PinMux_E18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(0)) =
								 (CONTROL_CONF_UART0_CTSN_CONF_UART0_CTSN_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART0_CTSN_CONF_UART0_CTSN_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart4_Rx_PinMux_J18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD3) =
		         	 	 	 	 (CONTROL_CONF_MII1_TXD3_CONF_MII1_TXD3_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_TXD3_CONF_MII1_TXD3_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	case Uart4_Rx_PinMux_T17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WAIT0) =
		         	 	 	 	 (CONTROL_CONF_GPMC_WAIT0_CONF_GPMC_WAIT0_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_GPMC_WAIT0_CONF_GPMC_WAIT0_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(6));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart4_tx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart4_Tx_PinMux_E17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(0)) =
								 (CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART0_RTSN_CONF_UART0_RTSN_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	case Uart4_Tx_PinMux_K15:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD2) =
		         	 	 	 	 (CONTROL_CONF_MII1_TXD2_CONF_MII1_TXD2_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_TXD2_CONF_MII1_TXD2_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	case Uart4_Tx_PinMux_U17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WPN) =
		         	 	 	 	 (CONTROL_CONF_GPMC_WPN_CONF_GPMC_WPN_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_GPMC_WPN_CONF_GPMC_WPN_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(6));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart5_rx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart5_Rx_PinMux_H16:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_COL) =
								 (CONTROL_CONF_MII1_COL_CONF_MII1_COL_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_COL_CONF_MII1_COL_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	case Uart5_Rx_PinMux_M17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_DATA) =
		         	 	 	 	 (CONTROL_CONF_MDIO_DATA_CONF_MDIO_DATA_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MDIO_DATA_CONF_MDIO_DATA_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(2));
		break;
	case Uart5_Rx_PinMux_U2:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(9)) =
		         	 	 	 	 (CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(4));
		break;
	case Uart5_Rx_PinMux_V4:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(14)) =
		         	 	 	 	 (CONTROL_CONF_LCD_DATA14_CONF_LCD_DATA14_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_LCD_DATA14_CONF_LCD_DATA14_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(4));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
unsigned int pin_mux_uart5_tx(unsigned int PinNr)
{
	switch(PinNr)
	{
	case Uart5_Tx_PinMux_J17:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXDV) =
								 (CONTROL_CONF_MII1_RXDV_CONF_MII1_RXDV_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MII1_RXDV_CONF_MII1_RXDV_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	case Uart5_Tx_PinMux_M18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_CLK) =
		         	 	 	 	 (CONTROL_CONF_MDIO_CLK_CONF_MDIO_CLK_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MDIO_CLK_CONF_MDIO_CLK_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(2));
		break;
	case Uart5_Tx_PinMux_U1:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(8)) =
		         	 	 	 	 (CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(4));
		break;
	case Uart5_Tx_PinMux_H18:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_RMII1_REFCLK) =
		         	 	 	 	 (CONTROL_CONF_RMII1_REFCLK_CONF_RMII1_REFCLK_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_RMII1_REFCLK_CONF_RMII1_REFCLK_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	default:
		return 0;
	}
	return 1;
}
/*#####################################################*/
bool pin_mux_uart_rx(unsigned int UartNr)
{
	switch(UartNr)
	{
	case 0:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(0)) =
		         	 	 	 	 (CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	case 1:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1)) =
		         	 	 	 	 (CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	case 2:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_SCLK) =
		         	 	 	 	 (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	/*case 3:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT1) =
		         	 	 	 	 (CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;
	case 4:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WAIT0) =
		         	 	 	 	 (CONTROL_CONF_GPMC_WAIT0_CONF_GPMC_WAIT0_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_GPMC_WAIT0_CONF_GPMC_WAIT0_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(6));
		break;*/
	case 5:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(9)) =
		         	 	 	 	 (CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_LCD_DATA9_CONF_LCD_DATA9_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(4));
		break;
	default:
		return false;
	}
	return true;
}
/*#####################################################*/
bool pin_mux_uart_tx(unsigned int UartNr)
{
	switch(UartNr)
	{
	case 0:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(0)) =
								 (CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	case 1:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(1)) =
		         	 	 	 	(CONTROL_CONF_UART1_TXD_CONF_UART1_TXD_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_UART1_TXD_CONF_UART1_TXD_RXACTIVE |
			         	 	 	 CONTROL_CONF_MUXMODE(0));
		break;
	case 2:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D0) =
		         	 	 	 	 (CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_SPI0_D0_CONF_SPI0_D0_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(1));
		break;
	/*case 3:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT0) =
		         	 	 	 	 (CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(3));
		break;*/
	case 4:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WPN) =
		         	 	 	 	 (CONTROL_CONF_GPMC_WPN_CONF_GPMC_WPN_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_GPMC_WPN_CONF_GPMC_WPN_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(6));
		break;
	case 5:
		HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(8)) =
		         	 	 	 	 (CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_PUTYPESEL |
		         	 	 	 	CONTROL_CONF_LCD_DATA8_CONF_LCD_DATA8_RXACTIVE |
		         	 	 	 CONTROL_CONF_MUXMODE(4));
		break;
	default:
		return false;
	}
	return true;
}
/*#####################################################*/
bool UARTPinMuxSetup(unsigned int instanceNum)
{
	if(pin_mux_uart_rx(instanceNum) & pin_mux_uart_tx(instanceNum)) return true;
	else return false;
}
/*#####################################################*/

