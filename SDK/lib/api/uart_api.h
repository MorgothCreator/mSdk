/*
 *  lib/api/uart_api.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UART_API_H_
#define UART_API_H_
/*#####################################################*/
#include <stdbool.h>
#include "uart_def.h"
/*#####################################################*/
#define MAX_SCANF_STRING_WIDTH		128
#define BASE_10             (10u)
#define BASE_16             (16u)
#define INVALID_INPUT       (0xFFu)
#define MAX_STRING_WIDTH    (80u)


/*
** Loop continuously until user enters any character other
** than space, carriage return, tab space and backspace
*/
#define IS_WHITESPACE(UartSettings, rxByte)    rxByte = uart_getc(UartSettings); \
                                 while(('\r' == rxByte) || (' ' == rxByte) || \
                                       ('\t' == rxByte) || ('\b' == rxByte)) \
                                 { \
                                      uart_putc(UartSettings, rxByte);   \
                                      rxByte = uart_getc(UartSettings);\
                                 }

/*
** Echoes back the delimiting character entered by the user
** after the actual input value has been read from the user.
*/
#define UART_SCANF_ECHO_INPUT(UartSettings, rxByte)                                     \
                                 if(('\n' == rxByte) || ('\r' == rxByte)) \
                                 {                                        \
                                     uart_putc(UartSettings, '\r');                      \
                                     uart_putc(UartSettings, '\n');                      \
                                 }                                        \
                                 else                                     \
                                 {                                        \
                                     uart_putc(UartSettings, rxByte);                    \
                                 }

/*#####################################################*/
extern const uart_t uart;
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "uart_api.c"
#endif
/*#####################################################*/
#endif /* UART_H_ */
/*#####################################################*/
