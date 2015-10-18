/*
 * pin_mux_uart.h
 *
 *  Created on: Aug 14, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */
/**********************************************/
#include <stdbool.h>
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/hw_types.h"
#include "api/gpio_def.h"
/**********************************************/
#ifndef PIN_MUX_UART_H_
#define PIN_MUX_UART_H_
/**********************************************/
#define Uart0_Rx_PinMux_B16	0
#define Uart0_Rx_PinMux_E15	1
#define Uart0_Rx_PinMux_D16	2

#define Uart0_Tx_PinMux_A16	0
#define Uart0_Tx_PinMux_E16	1
#define Uart0_Tx_PinMux_D15	2
/**********************************************/
#define Uart1_Rx_PinMux_D16	0

#define Uart1_Tx_PinMux_D15	0
/**********************************************/
#define Uart2_Rx_PinMux_H17	0
#define Uart2_Rx_PinMux_K18	1
#define Uart2_Rx_PinMux_A17	2
#define Uart2_Rx_PinMux_G17	3

#define Uart2_Tx_PinMux_J15	0
#define Uart2_Tx_PinMux_L18	1
#define Uart2_Tx_PinMux_B17	2
#define Uart2_Tx_PinMux_G18	3
/**********************************************/
#define Uart3_Rx_PinMux_L17	0
#define Uart3_Rx_PinMux_C15	1
#define Uart3_Rx_PinMux_G15	2

#define Uart3_Tx_PinMux_L16	0
#define Uart3_Tx_PinMux_C18	1
#define Uart3_Tx_PinMux_G16	2
/**********************************************/
#define Uart4_Rx_PinMux_E18	0
#define Uart4_Rx_PinMux_J18	1
#define Uart4_Rx_PinMux_T17	2

#define Uart4_Tx_PinMux_E17	0
#define Uart4_Tx_PinMux_K15	1
#define Uart4_Tx_PinMux_U17	2
/**********************************************/
#define Uart5_Rx_PinMux_H16	0
#define Uart5_Rx_PinMux_M17	1
#define Uart5_Rx_PinMux_U2	2
#define Uart5_Rx_PinMux_V4	3

#define Uart5_Tx_PinMux_J17	0
#define Uart5_Tx_PinMux_M18	1
#define Uart5_Tx_PinMux_U1	2
#define Uart5_Tx_PinMux_H18	3
/**********************************************/
unsigned int pin_mux_uart0_rx(unsigned int PinNr);
unsigned int pin_mux_uart0_tx(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_uart1_rx(unsigned int PinNr);
unsigned int pin_mux_uart1_tx(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_uart2_rx(unsigned int PinNr);
unsigned int pin_mux_uart2_tx(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_uart3_rx(unsigned int PinNr);
unsigned int pin_mux_uart3_tx(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_uart4_rx(unsigned int PinNr);
unsigned int pin_mux_uart4_tx(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_uart5_rx(unsigned int PinNr);
unsigned int pin_mux_uart5_tx(unsigned int PinNr);
/*#####################################################*/
bool pin_mux_uart_rx(unsigned int UartNr);
bool pin_mux_uart_tx(unsigned int UartNr);
bool UARTPinMuxSetup(unsigned int instanceNum);
/*#####################################################*/
void pin_mux_uart_transmit(gpio_port_enum port, unsigned char pin);
void pin_mux_uart_receive(gpio_port_enum port, unsigned char pin);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_uart.c"
#endif

#endif /* PIN_MUX_H_ */
