/*
 *  lib/api/uart_api.c
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

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include "uart_api.h"
#include "uart_def.h"
#include "sys/plat_properties.h"
#include "interface/uart_interface.h"
/*#####################################################*/
/* A mapping from an integer between 0 and 15 to its ASCII character
 * equivalent. */
static const char * const g_pcHex = "0123456789abcdef";
/*#####################################################*/
void UARTPutc(Uart_t* UartSettings, unsigned char byteTx)
{
	if(!UartSettings) return;
	_UARTCharPut(UartSettings->BaseAddr, byteTx);
}
/*#####################################################*/
unsigned char UARTGetc(Uart_t* UartSettings)
{
	if(!UartSettings) return 0;
	return (signed char)_UARTCharGet(UartSettings->BaseAddr);
}
/*#####################################################*/
bool UARTPutcNoBlocking(Uart_t* UartSettings, unsigned char byteTx)
{
	if(!UartSettings) return false;
	return _UARTCharPutNonBlocking(UartSettings->BaseAddr, byteTx);
}
/*#####################################################*/
signed short UARTGetcNoBlocking(Uart_t* UartSettings)
{
	if(!UartSettings) return -1;
	return _UARTCharGetNonBlocking(UartSettings->BaseAddr);
}
/*#####################################################*/
unsigned int UARTRxGetError(Uart_t* UartSettings)
{
	if(!UartSettings) return 0;
	return _UARTRxErrorGet(UartSettings->BaseAddr);
}
/*#####################################################*/
void UARTBaudRateSet(Uart_t* UartSettings, unsigned long BaudRate)
{
	if(!UartSettings) return;
	_UARTBaudSetRate(UartSettings->BaseAddr, BaudRate);
}
/*#####################################################*/
/**
 * \brief  This function writes data from a specified buffer onto the
 *         transmitter FIFO of UART.
 *
 * \param  pTxBuffer        Pointer to a buffer in the transmitter.
 * \param  numBytesToWrite  Number of bytes to be transmitted to the
 *                          transmitter FIFO. The user has the freedom to not
 *                          specify any valid value for this if he wants to
 *                          print until the occurence of a NULL character.
 *                          In this case, he has to pass a negative value as
 *                          this parameter.
 *
 * \return  Number of bytes written to the transmitter FIFO.
 *
 * \note   1> Whenever a null character(\0) is encountered in the
 *            data to be transmitted, the transmission is stopped. \n
 *         2> Whenever the transmitter data has a new line character(\n),
 *            it is interpreted as a new line(\n) + carraige return(\r)
 *            characters. This is because the serial console interprets a
 *            new line character as it is and does not introduce a carraige
 *            return. \n
 *
 *         Some example function calls of this function are: \n
 *
 *         UARTPuts(txArray, -2): This shall print the contents of txArray[]
 *         until the occurence of a NULL character. \n
 *
 *         UARTPuts("Hello World", 8): This shall print the first 8 characters
 *         of the string shown. \n
 *
 *         UARTPuts("Hello World", 20): This shall print the string shown until
 *         the occurence of the NULL character. Here, the NULL character is
 *         encountered earlier than the length of 20 bytes.\n
 *
 */
unsigned int UARTPuts(Uart_t* UartSettings,
		char *pTxBuffer, int numBytesToWrite)
{
	if(!UartSettings) return numBytesToWrite;
     unsigned int count = 0;
     unsigned int flag = 0;

     if(numBytesToWrite < 0)
     {
          flag = 1;
     }
     if(UartSettings->UseDma)
     {

     }
     else
     {
		 while(('\0' != *pTxBuffer && flag == 1) || flag == 0)
		 {
			  /* Checks if data is a newline character. */
			  if('\n' == *pTxBuffer)
			  {
				   /* Ensuring applicability to serial console.*/
				   //UARTPutc(UartSettings,'\r');
				   UARTPutc(UartSettings,'\n');
			  }
			  else
			  {
				   UARTPutc(UartSettings,(unsigned char)*pTxBuffer);
			  }
			  pTxBuffer++;
			  count++;

			  if((0 == flag) && (count == numBytesToWrite))
			  {
				   break;
			  }

		 }
     }
   /* Returns the number of bytes written onto the transmitter FIFO. */
   return count;
}

/**
 * \brief  This function reads data from the receiver FIFO to a buffer
 *         in the receiver.
 *
 * \param  pRxBuffer        Pointer to a buffer in the receiver.
 * \param  numBytesToRead   The number of bytes the receiver buffer can hold.
 *                          However, if the user wishes not to specify this
 *                          parameter, he has freedom to do so. In that case,
 *                          the user has to pass a negative value for this
 *                          parameter.
 *
 * \return  Number of bytes read from the receiver FIFO.
 *
 * \note   The two exit points for this function are:
 *         1> To enter an ESC character or a carraige return character('Enter'
 *            key on the Keyboard).\n
 *         2> Specify a limit to the number of bytes to be read. \n
 *
 *         Some example function calls of this function are:
 *
 *         UARTGets(rxBuffer, -2): This reads characters from
 *         the receiver FIFO of UART until the occurence of a carriage return
 *         ('Enter' key on the keyboard pressed) or an ESC character.
 *
 *         UARTGets(rxBuffer, 12): This reads characters until
 *         12 characters have been read or until an occurence of a carriage
 *         return or an ESC character, whichever occurs first.
 */

unsigned int UARTGets(Uart_t* UartSettings,
		char *pRxBuffer, int numBytesToRead)
{
	if(!UartSettings) return numBytesToRead;
     unsigned int count = 0;
     unsigned int flag = 0;

     if(numBytesToRead < 0)
     {
          flag = 1;
     }
     if(UartSettings->UseDma)
     {

     }
     else
     {
		 do
		 {
			  *pRxBuffer = (signed char)UARTGetc(UartSettings);

			  /*
			  ** 0xD - ASCII value of Carriage Return.
			  ** 0x1B - ASCII value of ESC character.
			  */
#ifdef USE_WDR
			  WDR();
#endif
			  if(0xD == *pRxBuffer || 0x1B == *pRxBuffer)
			  {
				   *pRxBuffer = '\0';
				   break;
			  }

			  /* Echoing the typed character back to the serial console. */
			  //UARTPutc(UartSettings,(unsigned char)*pRxBuffer);
			  pRxBuffer++;
			  count++;

			  if(0 == flag && (count == numBytesToRead))
			  {
				   break;
			  }

		 }while(1);
     }

     return count;
}


/**
 *  \brief   This function prints the specified numbers(positive or negative)
 *           on the serial console.
 *
 *  \param   value     The number to be printed on the serial console.
 *
 *  \return  None.
 *
 *  \note    The numbers that this function can print should lie in the
 *           following range:
 *           [-2^(31)] to [2^(31) - 1] i.e.
 *           0x80000000 to 0x7FFFFFFF
 *
 */

void UARTPutNum(Uart_t* UartSettings,
		int value)
{
	if(!UartSettings) return;
     unsigned char num[10] = {0};
     unsigned long quotient = 0;
     unsigned long dividend = 0;
     int count = 0;

     if(UartSettings->UseDma)
     {

     }
     else
     {
		 if(value < 0)
		 {
			  UARTPutc(UartSettings,'-');
			  /*
			  ** Making the negative number as positive.
			  ** This is done to simplify further processing and printing.
			  */
			  value = -value;
		 }

		 dividend = value;
		 do
		 {
			  quotient = dividend/10;
			  num[count] = (unsigned char)(dividend % 10);
			  if(0 == quotient)
			  {
				   break;
			  }
			  count++;
			  dividend = quotient;

		 }while(count < 10);

		 if(10 == count)
		 {
			  count--;
		 }

		 /* Printing the digits. */
		 do
		 {
			  /* We add 0x30 to a digit to obtain its respective ASCII value.*/
			  UARTPutc(UartSettings,num[count--] + 0x30);
		 }while(count >= 0);
     }
}

/**
 * \brief  This function is used to print hexadecimal numbers on the serial
 *         console.
 *
 * \param  hexValue   The Hexadecimal number to be printed.
 *
 * \return None
 */

void UARTPutHexNum(Uart_t* UartSettings,
		unsigned long hexValue)
{
	if(!UartSettings) return;
    unsigned char num[8] = {0};
    unsigned long quotient = 0;
    unsigned long dividend = 0;
    int count = 0;

    dividend = hexValue;

    if(UartSettings->UseDma)
    {

    }
    else
    {
		do
		{
			quotient = (dividend >> 4);
			num[count] = (unsigned char)(dividend % 16);
			if(0 == quotient)
			{
				break;
			}
			count++;
			dividend = quotient;

		}while(count < 8);

		if(8 == count)
		{
			count--;
		}

		UARTPutc(UartSettings,'0');
		UARTPutc(UartSettings,'x');

		while(count >= 0)
		{
			/* Checking for alphanumeric numbers. */
			if((16 - num[count]) <= 6)
			{
				/* Printing alphanumeric numbers. */
				UARTPutc(UartSettings,num[count--] + 0x37);
			}
			else
			{
				/* Printing numbers in the range 0 to 9. */
				UARTPutc(UartSettings,num[count--] + 0x30);
			}
		}
    }
}

/**
 * \brief   This function reads the numbers typed on the serial console.
 *
 * \return  The value entered on the serial console.
 *
 * \note    The numbers that this function can recieve should lie in the
 *          following range:
 *          [-2^(31)] to [2^(31) - 1] i.e.
 *          0x80000000 to  0x7FFFFFFF
 *
 */

long UARTGetNum(Uart_t* UartSettings)
{
	if(!UartSettings) return 0;
     signed char rxByte;
     signed char sign = 1;
     long value = 0;

     if(UartSettings->UseDma)
     {

     }
     else
     {
		 rxByte = (signed char)UARTGetc(UartSettings);

		 /* Accounting for negative numbers.*/
		 if('-' == rxByte)
		 {
			  UARTPutc(UartSettings,'-');
			  sign = -1;
		 }
		 else
		 {
			  UARTPutc(UartSettings,(unsigned char)rxByte);
			  value = value*10 + (rxByte - 0x30);
		 }

		 do
		 {
			  rxByte = (signed char)UARTGetc(UartSettings);

			  /* Echoing the typed characters to the serial console.*/
			  UARTPutc(UartSettings,(unsigned char)rxByte);
			  /*
			  ** Checking if the entered character is a carriage return.
			  ** Pressing the 'Enter' key on the keyboard executes a
			  ** carriage return on the serial console.
			  */
#ifdef USE_WDR
			  WDR();
#endif
			  if('\r' == rxByte)
			  {
				   break;
			  }
			  /*
			  ** Subtracting 0x30 to convert the representation of the digit
			  ** from ASCII to hexadecimal.
			  */
			  value = value*10 + (rxByte - 0x30);

		  }while(1);

		  /* Accounting for the sign of the number.*/
		  value = value * sign;
		 }
     return value;
}

/**
 * \brief   This function receives hexadecimal numbers entered on the serial
 *          console of the host machine.
 *
 * \param   None
 *
 * \return  The hexadecimal number entered on the serial console of the host
 *          machine.
 *
 * \note    1> The points below explain the ways of entering hexadecimal
 *             numbers:\n
 *             - 0xABCDEF12 - Hexadecimal number preceded by a '0x'.\n
 *             - 0XABCDEF12 - Hexadecimal number preceded by a '0X'.\n
 *             - 0xabcdef12 - Lower-case alphanumeric numbers are allowed.\n
 *             - 0xaBcDeF12 - Intermixing of lower-case and upper-case
 *                            alphanumeric numbers is allowed.\n
 *             - abcdef12   - A preceding '0x' or '0X' is not requried.
 *                            The entered number shall be treated as a
 *                            hexadecimal number.\n
 *             - 12345678   - Interpreted as 0x12345678 and not decimal
 *                            12345678.\n
 *             - xABCDEF12  - A preceding '0' is not required.\n
 *          2> This function does not take into consideration any character
 *             other than a hexadecimal character after reception.\n
 *             Example: Characters in the range G,H,I,...Z or g,h,...,z
 *             are not taken into consideration.\n
 *          3> The maximum value that can be returned by this function
 *             is 0xFFFFFFFF. The reception exits after eight characters have
 *             been received.\n
 *          4> To input a  number lesser that eight characters (Ex: 0x1AB),
 *             press the 'Enter' key after entering the number.\n
 */

unsigned long UARTGetHexNum(Uart_t* UartSettings)
{
	if(!UartSettings) return 0;
    unsigned char rxByte;
    unsigned long value = 0;
    unsigned long loopIndex;
    unsigned long byteCount = 0;

    for(loopIndex = 0; loopIndex < 2; loopIndex++)
    {
#ifdef USE_WDR
    	WDR();
#endif
        /* Receiving bytes from the host machine through serial console. */
        rxByte = UARTGetc(UartSettings);

        /*
        ** Checking if the entered character is a carriage return.
        ** Pressing the 'Enter' key on the keyboard executes a
        ** carriage return on the serial console.
        */
        if('\r' == rxByte)
        {
            break;
        }

        /*
        ** Checking if the character entered is one among the alphanumeric
        ** character set A,B,C...F
        */
        if(('A' <= rxByte) && (rxByte <= 'F'))
        {
            /* Echoing the typed characters to the serial console.*/
            UARTPutc(UartSettings,rxByte);
            value = (value << 4) + (rxByte - 0x37);
            byteCount++;
        }

        /*
        ** Checking if the character entered is one among the alphanumeric
        ** character set a,b,c...f
        */
        else if(('a' <= rxByte) && (rxByte <= 'f'))
        {
            UARTPutc(UartSettings,rxByte);
            value = (value << 4) + (rxByte - 0x57);
            byteCount++;
        }

        /*
        ** Checking if the character entered is one among the decimal
        ** number set 0,1,2,3,....9
        */
        else if(('0' <= rxByte) && (rxByte <= '9'))
        {
            UARTPutc(UartSettings,rxByte);
            value = (value << 4) + (rxByte - 0x30);
            byteCount++;
        }

        /*
        ** Checking if the character is either a 'x'(lower-case) or an 'X'
        ** (upper-case).
        */
        else if(('x' == rxByte) || ('X' == rxByte))
        {
            UARTPutc(UartSettings,rxByte);
            value = 0;
            break;
        }
    }

    if(0 == value)
    {
        byteCount = 0;
    }

    do
    {
        rxByte = UARTGetc(UartSettings);

#ifdef USE_WDR
        WDR();
#endif
        if('\r' == rxByte)
        {
            break;
        }

        /*
        ** Checking if the character entered is one among the alphanumeric
        ** character set A,B,C...F
        */
        if(('A' <= rxByte) && (rxByte <= 'F'))
        {
            UARTPutc(UartSettings,rxByte);
            value = (value << 4) + (rxByte - 0x37);
            byteCount++;
        }

        /*
        ** Checking if the character entered is one among the alphanumeric
        ** character set a,b,c...f
        */
        else if(('a' <= rxByte) && (rxByte <= 'f'))
        {
            UARTPutc(UartSettings,rxByte);
            value = (value << 4) + (rxByte - 0x57);
            byteCount++;
        }

        /*
        ** Checking if the character entered is one among the decimal
        ** number set 0,1,2,3,....9
        */
        else if(('0' <= rxByte) && (rxByte <= '9'))
        {
            UARTPutc(UartSettings,rxByte);
            value = (value << 4) + (rxByte - 0x30);
            byteCount++;
        }

        /*
        ** Not receiving any other character other than the one belonging
        ** to the above three categories.
        */
        else
        {
            /* Intentionally left empty. */
        }

    }while(byteCount < 8);

    return value;
}

/**
 * Writes a string of characters to the UART output.
 *
 * \param pcBuf points to a buffer containing the string to transmit.
 * \param len   is the length of the string to transmit.
 *
 * This function will transmit the string to the UART output.  The number of
 * characters transmitted is determined by the \e len parameter.  This
 * function does no interpretation or translation of any characters.  Since
 * the output is sent to a UART, any LF (/n) characters encountered will be
 * replaced with a CRLF pair.
 *
 * Besides using the \e len parameter to stop transmitting the string, if a
 * null character (0) is encountered, then no more characters will be
 * transmitted and the function will return.
 *
 * In non-buffered mode, this function is blocking and will not return until
 * all the characters have been written to the output FIFO.  In buffered mode,
 * the characters are written to the UART transmit buffer and the call returns
 * immediately.  If insufficient space remains in the transmit buffer,
 * additional characters are discarded.
 *
 * \return Returns the count of characters written.
 */
unsigned int UARTwrite(Uart_t* UartSettings,
		const char *pcBuf, unsigned int len)
{
	if(!UartSettings) return 0;
    unsigned int uIdx = 0;

    if(UartSettings->UseDma)
    {

    }
    else
    {
		/* Send the characters */
		for(uIdx = 0; uIdx < len; uIdx++)
		{
			/* If the character to the UART is \n, then add a \r before it so that
			 * \n is translated to \n\r in the output. */
			if(pcBuf[uIdx] == '\n')
			{
				//UARTPutc(UartSettings,'\r');
			}

			/* Send the character to the UART output. */
		   UARTPutc(UartSettings,pcBuf[uIdx]);
		}
    }

    /* Return the number of characters written. */
    return(uIdx);
}

/**
 * A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
 * \%x, and \%X.
 *
 * \param pcString is the format string.
 * \param ... are the optional arguments, which depend on the contents of the
 * format string.
 *
 * This function is very similar to the C library <tt>fprintf()</tt> function.
 * All of its output will be sent to the UART.  Only the following formatting
 * characters are supported:
 *
 * - \%c to print a character
 * - \%d to print a decimal value
 * - \%s to print a string
 * - \%u to print an unsigned decimal value
 * - \%x to print a hexadecimal value using lower case letters
 * - \%X to print a hexadecimal value using lower case letters (not upper case
 * letters as would typically be used)
 * - \%p to print a pointer as a hexadecimal value
 * - \%\% to print out a \% character
 *
 * For \%s, \%d, \%u, \%p, \%x, and \%X, an optional number may reside
 * between the \% and the format character, which specifies the minimum number
 * of characters to use for that value; if preceded by a 0 then the extra
 * characters will be filled with zeros instead of spaces.  For example,
 * ``\%8d'' will use eight characters to print the decimal value with spaces
 * added to reach eight; ``\%08d'' will use eight characters as well but will
 * add zeroes instead of spaces.
 *
 * The type of the arguments after \e pcString must match the requirements of
 * the format string.  For example, if an integer was passed where a string
 * was expected, an error of some kind will most likely occur.
 *
 * \return None.
 */
#ifdef _TINY_PRINT_
Uart_t*  UARTprintf(Uart_t* UartSettings,const char *pcString, ...)
{
	if(!UartSettings) return UartSettings;
    unsigned long idx, pos, count, base, neg;
    char *pcStr, pcBuf[16], cFill;
    va_list vaArgP;
    long value;

    /* Start the varargs processing. */
    va_start(vaArgP, pcString);

    /* Loop while there are more characters in the string. */
    while(*pcString)
    {
        /* Find the first non-% character, or the end of the string. */
        for(idx = 0; (pcString[idx] != '%') && (pcString[idx] != '\0'); idx++)
        {
        }

        /* Write this portion of the string. */
        UARTwrite(UartSettings, pcString, idx);

        /* Skip the portion of the string that was written. */
        pcString += idx;

        /* See if the next character is a %. */
        if(*pcString == '%')
        {
            /* Skip the %. */
            pcString++;

            /* Set the digit count to zero, and the fill character to space
             * (i.e. to the defaults). */
            count = 0;
            cFill = ' ';

            /* It may be necessary to get back here to process more characters.
             * Goto's aren't pretty, but effective.  I feel extremely dirty for
             * using not one but two of the beasts. */
again:

            /* Determine how to handle the next character. */
            switch(*pcString++)
            {
                /* Handle the digit characters. */
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    /* If this is a zero, and it is the first digit, then the
                     * fill character is a zero instead of a space. */
                    if((pcString[-1] == '0') && (count == 0))
                    {
                        cFill = '0';
                    }

                    /* Update the digit count. */
                    count *= 10;
                    count += pcString[-1] - '0';

                    /* Get the next character. */
                    goto again;
                }

                /* Handle the %c command. */
                case 'c':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArgP, unsigned int);

                    /* Print out the character. */
                    UARTwrite(UartSettings, (char *)&value, 1);

                    /* This command has been handled. */
                    break;
                }

                /* Handle the %d command. */
                case 'd':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArgP, unsigned long);

                    /* Reset the buffer position. */
                    pos = 0;

                    /* If the value is negative, make it positive and indicate
                     * that a minus sign is needed. */
                    if((long)value < 0)
                    {
                        /* Make the value positive. */
                        value = -(long)value;

                        /* Indicate that the value is negative. */
                        neg = 1;
                    }
                    else
                    {
                        /* Indicate that the value is positive so that a minus
                         * sign isn't inserted. */
                        neg = 0;
                    }

                    /* Set the base to 10. */
                    base = 10;

                    /* Convert the value to ASCII. */
                    goto convert;
                }

                /* Handle the %s command. */
                case 's':
                {
                    /* Get the string pointer from the varargs. */
                    pcStr = va_arg(vaArgP, char *);

                    /* Determine the length of the string. */
                    for(idx = 0; pcStr[idx] != '\0'; idx++)
                    {
                    }

                    /* Write the string. */
                    UARTwrite(UartSettings, pcStr, idx);

                    /* Write any required padding spaces */
                    if(count > idx)
                    {
                        count -= idx;
                        while(count--)
                        {
                            UARTwrite(UartSettings, (const char *)" ", 1);
                        }
                    }
                    /* This command has been handled. */
                    break;
                }

                /* Handle the %u command. */
                case 'u':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArgP, unsigned long);

                    /* Reset the buffer position. */
                    pos = 0;

                    /* Set the base to 10. */
                    base = 10;

                    /* Indicate that the value is positive so that a minus sign
                     * isn't inserted. */
                    neg = 0;

                    /* Convert the value to ASCII. */
                    goto convert;
                }

                /* Handle the %x and %X commands.  Note that they are treated
                 * identically; i.e. %X will use lower case letters for a-f
                 * instead of the upper case letters is should use.  We also
                 * alias %p to %x. */
                case 'x':
                case 'X':
                case 'p':
                {
                    /* Get the value from the varargs. */
                    value = va_arg(vaArgP, unsigned long);

                    /* Reset the buffer position. */
                    pos = 0;

                    /* Set the base to 16. */
                    base = 16;

                    /* Indicate that the value is positive so that a minus sign
                     * isn't inserted. */
                    neg = 0;

                    /* Determine the number of digits in the string version of
                     * the value. */
convert:
                    for(idx = 1;
                        (((idx * base) <= value) &&
                         (((idx * base) / base) == idx));
                        idx *= base, count--)
                    {
                    }

                    /* If the value is negative, reduce the count of padding
                     * characters needed. */
                    if(neg)
                    {
                        count--;
                    }

                    /* If the value is negative and the value is padded with
                     * zeros, then place the minus sign before the padding. */
                    if(neg && (cFill == '0'))
                    {
                        /* Place the minus sign in the output buffer. */
                        pcBuf[pos++] = '-';

                        /* The minus sign has been placed, so turn off the
                         * negative flag. */
                        neg = 0;
                    }

                    /* Provide additional padding at the beginning of the
                     * string conversion if needed. */
                    if((count > 1) && (count < 16))
                    {
                        for(count--; count; count--)
                        {
                            pcBuf[pos++] = cFill;
                        }
                    }

                    /* If the value is negative, then place the minus sign
                     * before the number. */
                    if(neg)
                    {
                        /* Place the minus sign in the output buffer. */
                        pcBuf[pos++] = '-';
                    }

                    /* Convert the value into a string. */
                    for(; idx; idx /= base)
                    {
                        pcBuf[pos++] = g_pcHex[(value / idx) % base];
                    }

                    /* Write the string. */
                    UARTwrite(UartSettings, pcBuf, pos);

                    /* This command has been handled. */
                    break;
                }

                /* Handle the %% command. */
                case '%':
                {
                    /* Simply write a single %. */
                    UARTwrite(UartSettings, pcString - 1, 1);

                    /* This command has been handled. */
                    break;
                }

                /* Handle all other commands. */
                default:
                {
                    /* Indicate an error. */
                    UARTwrite(UartSettings, (const char *)"ERROR", 5);

                    /* This command has been handled. */
                    break;
                }
            }
        }
    }

    /* End the varargs processing. */
    va_end(vaArgP);
    return UartSettings;
}
#endif
bool uart_open(Uart_t* UartSettings)
{
	return _uart_open(UartSettings);
}
bool uart_close(Uart_t *UartSettings)
{
	return _uart_close(UartSettings);
}










#ifndef _TINY_PRINT_


/*
 * Copyright Patrick Powell 1995 & modified by Iulian Gheorghiu
 * This code is based on code written by Patrick Powell (papowell@astart.com)
 * It may be used for any purpose as long as this notice remains intact
 * on all source code distributions
 */

/**************************************************************
 * Original:
 * Patrick Powell Tue Apr 11 09:48:21 PDT 1995
 * A bombproof version of doprnt (dopr) included.
 * Sigh.  This sort of thing is always nasty do deal with.  Note that
 * the version here does not include floating point...
 *
 * snprintf() is used instead of sprintf() as it does limit checks
 * for string length.  This covers a nasty loophole.
 *
 * The other functions are there to prevent NULL pointers from
 * causing nast effects.
 *
 * More Recently:
 *  Brandon Long <blong@fiction.net> 9/15/96 for mutt 0.43
 *  This was ugly.  It is still ugly.  I opted out of floating point
 *  numbers, but the formatter understands just about everything
 *  from the normal C string format, at least as far as I can tell from
 *  the Solaris 2.5 printf(3S) man page.
 *
 *  Brandon Long <blong@fiction.net> 10/22/97 for mutt 0.87.1
 *    Ok, added some minimal floating point support, which means this
 *    probably requires libm on most operating systems.  Don't yet
 *    support the exponent (e,E) and sigfig (g,G).  Also, fmtint()
 *    was pretty badly broken, it just wasn't being exercised in ways
 *    which showed it, so that's been fixed.  Also, formated the code
 *    to mutt conventions, and removed dead code left over from the
 *    original.  Also, there is now a builtin-test, just compile with:
 *           gcc -DTEST_SNPRINTF -o snprintf snprintf.c -lm
 *    and run snprintf for results.
 *
 *  Thomas Roessler <roessler@guug.de> 01/27/98 for mutt 0.89i
 *    The PGP code was using unsigned hexadecimal formats.
 *    Unfortunately, unsigned formats simply didn't work.
 *
 *  Michael Elkins <me@cs.hmc.edu> 03/05/98 for mutt 0.90.8
 *    The original code assumed that both snprintf() and vsnprintf() were
 *    missing.  Some systems only have snprintf() but not vsnprintf(), so
 *    the code is now broken down under HAVE_SNPRINTF and HAVE_VSNPRINTF.
 *
 *  Andrew Tridgell (tridge@samba.org) Oct 1998
 *    fixed handling of %.0f
 *    added test for HAVE_LONG_DOUBLE
 *
 **************************************************************/

/* Version number of package */
#undef VERSION//#include "config.h"

#if !defined(HAVE_SNPRINTF) || !defined(HAVE_VSNPRINTF)

#include <string.h>
#include <ctype.h>
#include <sys/types.h>

/* Define this as a fall through, HAVE_STDARG_H is probably already set */

#define HAVE_VARARGS_H


/* varargs declarations: */
#define HAVE_STDARG_H

#if defined(HAVE_STDARG_H)
# include "stdarg.h"
# define HAVE_STDARGS    /* let's hope that works everywhere (mj) */
# define VA_LOCAL_DECL   va_list ap
# define VA_START(f)     va_start(ap, f)
# define VA_SHIFT(v,t)  ;   /* no-op for ANSI */
# define VA_END          va_end(ap)
#else
# if defined(HAVE_VARARGS_H)
#  include "varargs.h"
#  undef HAVE_STDARGS
#  define VA_LOCAL_DECL   va_list ap
#  define VA_START(f)     va_start(ap)      /* f is ignored! */
#  define VA_SHIFT(v,t) v = va_arg(ap,t)
#  define VA_END        va_end(ap)
# else
/*XX ** NO VARARGS ** XX*/
# endif
#endif

#ifdef HAVE_LONG_DOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif

//int snprintf (Uart_t* UartSettings, char *str, size_t count, const char *fmt, ...);
int _vsnprintf_ (Uart_t* UartSettings, char *str, size_t count, const char *fmt, va_list arg);// Renamed to avoid conflict with builtin 'vsnprintf' function.

static void dopr (Uart_t* UartSettings, char *buffer, size_t maxlen, const char *format,
                  va_list args);
static void fmtstr (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen,
		    char *value, int flags, int min, int max);
static void fmtint (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen,
		    long value, int base, int min, int max, int flags);
static void fmtfp (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen,
		   LDOUBLE fvalue, int min, int max, int flags);
static void dopr_outch (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen, char c );

/*
 * dopr(): poor man's version of doprintf
 */

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS   1
#define DP_S_MIN     2
#define DP_S_DOT     3
#define DP_S_MAX     4
#define DP_S_MOD     5
#define DP_S_CONV    6
#define DP_S_DONE    7

/* format flags - Bits */
#define DP_F_MINUS 	(1 << 0)
#define DP_F_PLUS  	(1 << 1)
#define DP_F_SPACE 	(1 << 2)
#define DP_F_NUM   	(1 << 3)
#define DP_F_ZERO  	(1 << 4)
#define DP_F_UP    	(1 << 5)
#define DP_F_UNSIGNED 	(1 << 6)

/* Conversion Flags */
#define DP_C_SHORT   1
#define DP_C_LONG    2
#define DP_C_LDOUBLE 3

#define char_to_int(p) (p - '0')
#define MAX(p,q) ((p >= q) ? p : q)

static void dopr (Uart_t* UartSettings, char *buffer, size_t maxlen, const char *format, va_list args)
{
  int ch;
  long value;
  LDOUBLE fvalue;
  char *strvalue;
  int min;
  int max;
  int state;
  int flags;
  int cflags;
  size_t currlen;

  state = DP_S_DEFAULT;
  currlen = flags = cflags = min = 0;
  max = -1;
  ch = *format++;

  while (state != DP_S_DONE)
  {
    if ((ch == '\0') || (currlen >= maxlen))
      state = DP_S_DONE;

    switch(state)
    {
    case DP_S_DEFAULT:
      if (ch == '%')
	state = DP_S_FLAGS;
      else
	dopr_outch (UartSettings, buffer, &currlen, maxlen, ch);
      ch = *format++;
      break;
    case DP_S_FLAGS:
      switch (ch)
      {
      case '-':
	flags |= DP_F_MINUS;
        ch = *format++;
	break;
      case '+':
	flags |= DP_F_PLUS;
        ch = *format++;
	break;
      case ' ':
	flags |= DP_F_SPACE;
        ch = *format++;
	break;
      case '#':
	flags |= DP_F_NUM;
        ch = *format++;
	break;
      case '0':
	flags |= DP_F_ZERO;
        ch = *format++;
	break;
      default:
	state = DP_S_MIN;
	break;
      }
      break;
    case DP_S_MIN:
      if (isdigit(ch))
      {
	min = 10*min + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	min = va_arg (args, int);
	ch = *format++;
	state = DP_S_DOT;
      }
      else
	state = DP_S_DOT;
      break;
    case DP_S_DOT:
      if (ch == '.')
      {
	state = DP_S_MAX;
	ch = *format++;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MAX:
      if (isdigit(ch))
      {
	if (max < 0)
	  max = 0;
	max = 10*max + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	max = va_arg (args, int);
	ch = *format++;
	state = DP_S_MOD;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MOD:
      /* Currently, we don't support Long Long, bummer */
      switch (ch)
      {
      case 'h':
	cflags = DP_C_SHORT;
	ch = *format++;
	break;
      case 'l':
	cflags = DP_C_LONG;
	ch = *format++;
	break;
      case 'L':
	cflags = DP_C_LDOUBLE;
	ch = *format++;
	break;
      default:
	break;
      }
      state = DP_S_CONV;
      break;
    case DP_S_CONV:
      switch (ch)
      {
      case 'd':
      case 'i':
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, short int);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, long int);
	else
	  value = va_arg (args, int);
	fmtint (UartSettings, buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'o':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short int);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long int);
	else
	  value = va_arg (args, unsigned int);
	fmtint (UartSettings, buffer, &currlen, maxlen, value, 8, min, max, flags);
	break;
      case 'u':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short int);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long int);
	else
	  value = va_arg (args, unsigned int);
	fmtint (UartSettings, buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'X':
	flags |= DP_F_UP;
      case 'x':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short int);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long int);
	else
	  value = va_arg (args, unsigned int);
	fmtint (UartSettings, buffer, &currlen, maxlen, value, 16, min, max, flags);
	break;
      case 'f':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	/* um, floating point? */
	fmtfp (UartSettings, buffer, &currlen, maxlen, fvalue, min, max, flags);
	break;
      case 'E':
	flags |= DP_F_UP;
      case 'e':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'G':
	flags |= DP_F_UP;
      case 'g':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'c':
	dopr_outch (UartSettings, buffer, &currlen, maxlen, va_arg (args, int));
	break;
      case 's':
	strvalue = va_arg (args, char *);
	if (max < 0)
	  max = maxlen; /* ie, no max */
	fmtstr (UartSettings, buffer, &currlen, maxlen, strvalue, flags, min, max);
	break;
      case 'p':
	strvalue = va_arg (args, void *);
	fmtint (UartSettings, buffer, &currlen, maxlen, (long) strvalue, 16, min, max, flags);
	break;
      case 'n':
	if (cflags == DP_C_SHORT)
	{
	  short int *num;
	  num = va_arg (args, short int *);
	  *num = currlen;
        }
	else if (cflags == DP_C_LONG)
	{
	  long int *num;
	  num = va_arg (args, long int *);
	  *num = currlen;
        }
	else
	{
	  int *num;
	  num = va_arg (args, int *);
	  *num = currlen;
        }
	break;
      case '%':
	dopr_outch (UartSettings, buffer, &currlen, maxlen, ch);
	break;
      case 'w':
	/* not supported yet, treat as next char */
	ch = *format++;
	break;
      default:
	/* Unknown, skip */
	break;
      }
      ch = *format++;
      state = DP_S_DEFAULT;
      flags = cflags = min = 0;
      max = -1;
      break;
    case DP_S_DONE:
      break;
    default:
      /* hmm? */
      break; /* some picky compilers need this */
    }
  }
  if (currlen < maxlen - 1)
    buffer[currlen] = '\0';
  else
    buffer[maxlen - 1] = '\0';
}

static void fmtstr (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen,
		    char *value, int flags, int min, int max)
{
  int padlen, strln;     /* amount to pad */
  int cnt = 0;

  if (value == 0)
  {
    value = "<NULL>";
  }

  for (strln = 0; value[strln]; ++strln); /* strlen */
  padlen = min - strln;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justify */

  while ((padlen > 0) && (cnt < max))
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, ' ');
    --padlen;
    ++cnt;
  }
  while (*value && (cnt < max))
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, *value++);
    ++cnt;
  }
  while ((padlen < 0) && (cnt < max))
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, ' ');
    ++padlen;
    ++cnt;
  }
}

/* Have to handle DP_F_NUM (ie 0x and 0 alternates) */

static void fmtint (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen,
		    long value, int base, int min, int max, int flags)
{
  int signvalue = 0;
  unsigned long uvalue;
  char convert[20];
  int place = 0;
  int spadlen = 0; /* amount to space pad */
  int zpadlen = 0; /* amount to zero pad */
  int caps = 0;

  if (max < 0)
    max = 0;

  uvalue = value;

  if(!(flags & DP_F_UNSIGNED))
  {
    if( value < 0 ) {
      signvalue = '-';
      uvalue = -value;
    }
    else
      if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
	signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';
  }

  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */

  do {
    convert[place++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")
      [uvalue % (unsigned)base  ];
    uvalue = (uvalue / (unsigned)base );
  } while(uvalue && (place < 20));
  if (place == 20) place--;
  convert[place] = 0;

  zpadlen = max - place;
  spadlen = min - MAX (max, place) - (signvalue ? 1 : 0);
  if (zpadlen < 0) zpadlen = 0;
  if (spadlen < 0) spadlen = 0;
  if (flags & DP_F_ZERO)
  {
    zpadlen = MAX(zpadlen, spadlen);
    spadlen = 0;
  }
  if (flags & DP_F_MINUS)
    spadlen = -spadlen; /* Left Justifty */

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
      zpadlen, spadlen, min, max, place));
#endif

  /* Spaces */
  while (spadlen > 0)
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, ' ');
    --spadlen;
  }

  /* Sign */
  if (signvalue)
    dopr_outch (UartSettings, buffer, currlen, maxlen, signvalue);

  /* Zeros */
  if (zpadlen > 0)
  {
    while (zpadlen > 0)
    {
      dopr_outch (UartSettings, buffer, currlen, maxlen, '0');
      --zpadlen;
    }
  }

  /* Digits */
  while (place > 0)
    dopr_outch (UartSettings, buffer, currlen, maxlen, convert[--place]);

  /* Left Justified spaces */
  while (spadlen < 0) {
    dopr_outch (UartSettings, buffer, currlen, maxlen, ' ');
    ++spadlen;
  }
}

static LDOUBLE abs_val (LDOUBLE value)
{
  LDOUBLE result = value;

  if (value < 0)
    result = -value;

  return result;
}

static LDOUBLE _pow10_ (int exp)
{
  LDOUBLE result = 1;

  while (exp)
  {
    result *= 10;
    exp--;
  }

  return result;
}

static long _round_ (LDOUBLE value)
{
  long intpart;

  intpart = value;
  value = value - intpart;
  if (value >= 0.5)
    intpart++;

  return intpart;
}

static void fmtfp (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen,
		   LDOUBLE fvalue, int min, int max, int flags)
{
  int signvalue = 0;
  LDOUBLE ufvalue;
  char iconvert[20];
  char fconvert[20];
  int iplace = 0;
  int fplace = 0;
  int padlen = 0; /* amount to pad */
  int zpadlen = 0;
  int caps = 0;
  long intpart;
  long fracpart;

  /*
   * AIX manpage says the default is 0, but Solaris says the default
   * is 6, and sprintf on AIX defaults to 6
   */
  if (max < 0)
    max = 6;

  ufvalue = abs_val (fvalue);

  if (fvalue < 0)
    signvalue = '-';
  else
    if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
      signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';

#if 0
  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */
#endif

  intpart = ufvalue;

  /*
   * Sorry, we only support 9 digits past the decimal because of our
   * conversion method
   */
  if (max > 9)
    max = 9;

  /* We "cheat" by converting the fractional part to integer by
   * multiplying by a factor of 10
   */
  fracpart = _round_ ((_pow10_ (max)) * (ufvalue - intpart));

  if (fracpart >= _pow10_ (max))
  {
    intpart++;
    fracpart -= _pow10_ (max);
  }

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "fmtfp: %f =? %d.%d\n", fvalue, intpart, fracpart));
#endif

  /* Convert integer part */
  do {
    iconvert[iplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[intpart % 10];
    intpart = (intpart / 10);
  } while(intpart && (iplace < 20));
  if (iplace == 20) iplace--;
  iconvert[iplace] = 0;

  /* Convert fractional part */
  do {
    fconvert[fplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[fracpart % 10];
    fracpart = (fracpart / 10);
  } while(fracpart && (fplace < 20));
  if (fplace == 20) fplace--;
  fconvert[fplace] = 0;

  /* -1 for decimal point, another -1 if we are printing a sign */
  padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0);
  zpadlen = max - fplace;
  if (zpadlen < 0)
    zpadlen = 0;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justifty */

  if ((flags & DP_F_ZERO) && (padlen > 0))
  {
    if (signvalue)
    {
      dopr_outch (UartSettings, buffer, currlen, maxlen, signvalue);
      --padlen;
      signvalue = 0;
    }
    while (padlen > 0)
    {
      dopr_outch (UartSettings, buffer, currlen, maxlen, '0');
      --padlen;
    }
  }
  while (padlen > 0)
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, ' ');
    --padlen;
  }
  if (signvalue)
    dopr_outch (UartSettings, buffer, currlen, maxlen, signvalue);

  while (iplace > 0)
    dopr_outch (UartSettings, buffer, currlen, maxlen, iconvert[--iplace]);

  /*
   * Decimal point.  This should probably use locale to find the correct
   * char to print out.
   */
  if (max > 0)
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, '.');

    while (fplace > 0)
      dopr_outch (UartSettings, buffer, currlen, maxlen, fconvert[--fplace]);
  }

  while (zpadlen > 0)
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, '0');
    --zpadlen;
  }

  while (padlen < 0)
  {
    dopr_outch (UartSettings, buffer, currlen, maxlen, ' ');
    ++padlen;
  }
}

static void dopr_outch (Uart_t* UartSettings, char *buffer, size_t *currlen, size_t maxlen, char c)
{
  /*if (*currlen < maxlen)
    buffer[(*currlen)++] = c;*/
	UARTPutc(UartSettings, (unsigned char)c);
}

#ifndef HAVE_VSNPRINTF
int _vsnprintf_ (Uart_t* UartSettings, char *str, size_t count, const char *fmt, va_list args)
{
  str[0] = 0;
  dopr(UartSettings, str, count, fmt, args);
  return(strlen(str));
}
#endif /* !HAVE_VSNPRINTF */

#ifndef HAVE_SNPRINTF
/* VARARGS3 */
#ifdef HAVE_STDARGS
/*
 * specifier	output
 *
 * d or i		Signed decimal integer							392
 * u			Unsigned decimal integer							7235
 * o			Unsigned octal										610
 * x			Unsigned hexadecimal integer						7fa
 * X			Unsigned hexadecimal integer (uppercase)			7FA
 * f			Decimal floating point, lowercase					392.65
 * F			Decimal floating point, uppercase					392.65
 * e			Scientific notation (mantissa/exponent), lowercase	3.9265e+2
 * E			Scientific notation (mantissa/exponent), uppercase	3.9265E+2
 * g			Use the shortest representation: %e or %f			392.65
 * G			Use the shortest representation: %E or %F			392.65
 * a			Hexadecimal floating point, lowercase				-0xc.90fep-2
 * A			Hexadecimal floating point, uppercase				-0XC.90FEP-2
 * c			Character											a
 * s			String of characters								sample
 * p			Pointer address										b8000000
 * n			Nothing printed.
 * 					The corresponding argument must be a pointer to a signed int.
 * 					The number of characters written so far is stored in the pointed location.
 * %			A % followed by another % character will write a single % to the stream.	%
 */
Uart_t*  UARTprintf(Uart_t* UartSettings,const char *pcString, ...)
//int snprintf (Uart_t* UartSettings, char *str,size_t count,const char *fmt,...)
#else
int snprintf (va_alist) //va_dcl
#endif
{
#ifndef HAVE_STDARGS
  char *str;
  size_t count;
  char *fmt;
#endif
  VA_LOCAL_DECL;

  VA_START (pcString);
  VA_SHIFT (str, char *);
  VA_SHIFT (count, size_t );
  VA_SHIFT (fmt, char *);
  char buff[1];
  (void) _vsnprintf_(UartSettings, buff, 1, pcString, ap);
  VA_END;
  return UartSettings;//(strlen(str));
}
#endif /* !HAVE_SNPRINTF */

#endif /* !HAVE_SNPRINTF */

#endif /* !_TINY_PRINT_ */
