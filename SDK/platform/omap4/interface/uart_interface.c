/*
 * usart_interface.c
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#include <stdlib.h>
#include "uart_interface.h"
#include "api/uart_def.h"
#include "clk/clk_uart.h"
#include "driver/uart.h"
#include "aintc/aintc_uart.h"
#include "pinmux/pin_mux_uart.h"

/*#####################################################*/
static void UartFIFOConfigure(Uart_t* settings_structure,
							unsigned int txTrigLevel,
                              unsigned int rxTrigLevel)
{
    unsigned int fifoConfig = 0;

    /* Setting the TX and RX FIFO Trigger levels as 1. No DMA enabled. */
    fifoConfig = UART_FIFO_CONFIG(UART_TRIG_LVL_GRANULARITY_1,
                                  UART_TRIG_LVL_GRANULARITY_1,
                                  txTrigLevel,
                                  rxTrigLevel,
                                  1,
                                  1,
                                  UART_DMA_EN_PATH_SCR,
                                  UART_DMA_MODE_0_ENABLE);

    /* Configuring the FIFO settings. */
    UARTFIFOConfig(settings_structure->BaseAddr, fifoConfig);
}
/*#####################################################*/
/*
** A wrapper function performing Baud Rate settings.
*/

static void UartBaudRateSet(Uart_t* settings_structure)
{
    unsigned int divisorValue = 0;

    /* Computing the Divisor Value. */
    divisorValue = UARTDivisorValCompute(UART_MODULE_INPUT_CLK,
    									settings_structure->BaudRate,
    									UART_MDR1_MODE_SELECT_UART16X,
                                         UART_MIR_OVERSAMPLING_RATE_42);

    /* Programming the Divisor Latches. */
    UARTDivisorLatchWrite(settings_structure->BaseAddr, divisorValue);
}

/*#####################################################*/
bool _uart_open(Uart_t* UartSettings)
{
	//if(UartSettings->UseDma)
	//{
		/* Initialize EDMA3 Controller */
		//EDMA3Initialize();
	//}
	switch (UartSettings->UartNr)
	{
	case 0:
		UartSettings->BaseAddr = SOC_UART_0_REGS;
	    /* Request DMA Channel and TCC for UART Transmit*/
		//if(UartSettings->UseDma)
		//{
		//	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS, EDMA3_CHANNEL_TYPE_DMA,
		//						EDMA3_CHA_UART0_TX, EDMA3_CHA_UART0_TX,
		//						0);
		//	/* Registering Callback Function for TX*/
		//	cb_Fxn[EDMA3_CHA_UART0_TX] = &uart0tx_callback;
		//}
	    break;
	case 1:
		UartSettings->BaseAddr = SOC_UART_1_REGS;
	    /* Request DMA Channel and TCC for UART Transmit*/
		//if(UartSettings->UseDma)
		//{
		//	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS, EDMA3_CHANNEL_TYPE_DMA,
		//						EDMA3_CHA_UART1_TX, EDMA3_CHA_UART1_TX,
		//						0);
		//	cb_Fxn[EDMA3_CHA_UART1_TX] = &uart1tx_callback;
		//}
		break;
	case 2:
		UartSettings->BaseAddr = SOC_UART_2_REGS;
	    /* Request DMA Channel and TCC for UART Transmit*/
		//if(UartSettings->UseDma)
		//{
		//	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS, EDMA3_CHANNEL_TYPE_DMA,
		//						EDMA3_CHA_UART2_TX, EDMA3_CHA_UART2_TX,
		//						0);
		//	cb_Fxn[EDMA3_CHA_UART2_TX] = &uart2tx_callback;
		//}
		break;
	case 3:
		UartSettings->BaseAddr = SOC_UART_3_REGS;
	    /* Request DMA Channel and TCC for UART Transmit*/
		//if(UartSettings->UseDma)
		//{
		//	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS, EDMA3_CHANNEL_TYPE_DMA,
		//						EDMA3_CHA_UART3_TX, EDMA3_CHA_UART3_TX,
		//						0);
		//	cb_Fxn[EDMA3_CHA_UART3_TX] = &uart3tx_callback;
		//}
		break;
	}

	unsigned char *RxBuff;
	unsigned char *TxBuff;
	if(UartSettings->WithInterrupt == TRUE && UartSettings->UseDma == FALSE)
	{
		if(UartSettings->RxBuffSize)
		{
			RxBuff = calloc(1, UartSettings->RxBuffSize);
			if(RxBuff == NULL) return 0;
			UartSettings->RxBuff = RxBuff;
		}
		if(UartSettings->TxBuffSize)
		{
			TxBuff = calloc(1, UartSettings->TxBuffSize);
			if(TxBuff == NULL ) return 0;
			UartSettings->TxBuff = TxBuff;
		}
		UARTAINTCConfigure(UartSettings);
	}
	UARTModuleClkConfig(UartSettings->UartNr);
	switch(UartSettings->UartNr)
	{
	case 0:
		Uart_Int_Service.Uart0 = UartSettings;
		pin_mux_uart_rx(UartSettings->RxPin);
		pin_mux_uart_tx(UartSettings->TxPin);
		break;
	case 1:
		Uart_Int_Service.Uart1 = UartSettings;
		pin_mux_uart_rx(UartSettings->RxPin);
		pin_mux_uart_tx(UartSettings->TxPin);
		break;
	case 2:
		Uart_Int_Service.Uart2 = UartSettings;
		pin_mux_uart_rx(UartSettings->RxPin);
		pin_mux_uart_tx(UartSettings->TxPin);
		break;
	case 3:
		Uart_Int_Service.Uart3 = UartSettings;
		pin_mux_uart_rx(UartSettings->RxPin);
		pin_mux_uart_tx(UartSettings->TxPin);
		break;
	}
	//UARTPinMuxSetup(UartSettings->UartNr);
    /* Performing a module reset. */
    UARTModuleReset(UartSettings->BaseAddr);

    /* Performing FIFO configurations. */
    UartFIFOConfigure(UartSettings, UartSettings->txFifoTrigLevel, UartSettings->rxFifoTrigLevel);

    /* Performing Baud Rate settings. */
    UartBaudRateSet(UartSettings);

    /* Switching to Configuration Mode B. */
    UARTRegConfigModeEnable(UartSettings->BaseAddr, UART_REG_CONFIG_MODE_B);

    /* Programming the Line Characteristics. */
    UARTLineCharacConfig(UartSettings->BaseAddr,
                         (UART_LCR_CHAR_LENGTH_8BIT | UART_FRAME_NUM_STB_1),
                         UART_PARITY_NONE);

    /* Disabling write access to Divisor Latches. */
    UARTDivisorLatchDisable(UartSettings->BaseAddr);

    /* Disabling Break Control. */
    UARTBreakCtl(UartSettings->BaseAddr, UART_BREAK_COND_DISABLE);

    /* Switching to UART16x operating mode. */
    UARTOperatingModeSelect(UartSettings->BaseAddr, UART_MDR1_MODE_SELECT_UART16X);
	return 1;
}

/*#####################################################*/
bool _uart_close(Uart_t *UartSettings)
{
    UARTModuleReset(UartSettings->BaseAddr);
    UARTModuleClkUnConfig(UartSettings->UartNr);
    if(UartSettings->RxBuff) free((void*)UartSettings->RxBuff);
    if(UartSettings->TxBuff) free((void*)UartSettings->TxBuff);
	switch(UartSettings->UartNr)
	{
	case 0:
		Uart_Int_Service.Uart0 = NULL;
		break;
	case 1:
		Uart_Int_Service.Uart1 = NULL;
		break;
	case 2:
		Uart_Int_Service.Uart2 = NULL;
		break;
	case 3:
		Uart_Int_Service.Uart3 = NULL;
		break;
	}
	if(UartSettings) free(UartSettings);
	return true;
}
/*#####################################################*/
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate)
{
	//uart_set_baud_rate(UartSettings->BaseAddr, BaudRate);
}
/*#####################################################*/
void _UARTCharPut(unsigned int BaseAddr, unsigned char byteTx)
{
	UARTCharPut(BaseAddr, byteTx);
}
/*#####################################################*/
unsigned char _UARTCharGet(unsigned int BaseAddr)
{
	return (signed char)UARTCharGet(BaseAddr);
}
/*#####################################################*/
void _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx)
{
	UARTCharPutNonBlocking(BaseAddr, byteTx);
}
/*#####################################################*/
signed char _UARTCharGetNonBlocking(unsigned int BaseAddr)
{
	return UARTCharGetNonBlocking(BaseAddr);
}
/*#####################################################*/
unsigned int _UARTRxErrorGet(unsigned int BaseAddr)
{
	return UARTRxErrorGet(BaseAddr);
}
/*#####################################################*/

