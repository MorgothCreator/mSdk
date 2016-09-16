/*
 * rfm12.c
 *
 *  Created on: Sep 16, 2016
 *      Author: John Smith
 */

#include "rfm12.h"
//#include "config.h"
#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include "api/timer_api.h"
#include "string.h"
//#include "io_usart.h"

//extern Io_Usart DEBUG_UART;

#define RFM12_USE_SERVICE false

void rfm12_cs_assert(void *param)
{
	rfm12_t *_param = (rfm12_t *)param;
	gpio_out(_param->CS_PORT, 0);
}

void rfm12_cs_deassert(void *param)
{
	rfm12_t *_param = (rfm12_t *)param;
	gpio_out(_param->CS_PORT, 1);
}

unsigned short rfm12_cmd(void *param, unsigned short command)
{
	rfm12_t *_param = (rfm12_t *)param;
	//while(_param->IRQ_PORT->IN & (1 << _param->IRQ_PIN));
	rfm12_cs_assert(param);
	unsigned char tmp_buff[2];
	tmp_buff[0] = command >> 8;
	tmp_buff[1] = command;
	_param->spi_unit->Buff = tmp_buff;
	mcspi_transfer(_param->spi_unit, 2, 0);
	rfm12_cs_deassert(param);
	return (tmp_buff[0] << 8) + tmp_buff[1];
}


void rfm12_init(void *param, new_mcspi *spi_unit, unsigned char spi_instance, new_gpio *IRQ_PORT, new_gpio *CS_PORT)
{
	rfm12_t *_param = (rfm12_t *)param;
	_param->spi_unit = spi_unit;
	_param->spi_instance = spi_instance;
	_param->CS_PORT = CS_PORT;
	_param->IRQ_PORT = IRQ_PORT;
	_param->rx_buffer = (unsigned char *)calloc(1, _param->rx_buff_len);
	_param->tx_buffer = (unsigned char *)calloc(1, _param->tx_buff_len);
	if (!_param->rx_buffer || !_param->tx_buffer)
	{
		if(!_param->rx_buffer)
			free(_param->rx_buffer);
		if(!_param->tx_buffer)
			free(_param->tx_buffer);
		_param->driver_initialized = false;
		return;
	}
	Sysdelay(10);
	/*cmd(0x0000);
	cmd(0x80D8);//EL,EF,433band,12.5pF
	cmd(0x8208);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	cmd(0xA680);//A140=430.8MHz
	cmd(0xC647);//4.8kbps
	cmd(0x94A2);//VDI,FAST,134kHz,0dBm,-103dBm
	cmd(0xC2AC);//AL,!ml,DIG,DQD4
	cmd(0xCA81);//FIFO8,SYNC,!ff,DR
	cmd(0xCED4);//SYNC=2DD4?
	cmd(0xC483);//@PWR,NO RSTRIC,!st,!fi,OE,EN
	cmd(0x9850);//!mp,9810=30kHz,MAX OUT
	cmd(0xCC77);//OB1?OB0,?lpx,?ddy?DDIT?BW0
	cmd(0xE000);//NOT USE
	cmd(0xC800);//NOT USE
	cmd(0xC040);//1.66MHz,2.2V*/

	/*cmd(0x0000);
	cmd(0x80D7);//EL,EF,433band,12.5pF
	cmd(0x8208);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	cmd(0xA640);//A140=430.8MHz
	cmd(0xC602);//4.8kbps
	cmd(0x9080);//Receiver Control Command
	//cmd(0x94A2);//VDI,FAST,134kHz,0dBm,-103dBm
	cmd(0xC2EF);//AL,!ml,DIG,DQD4
	cmd(0xCA81);//FIFO8,SYNC,!ff,DR
	cmd(0xCED4);//SYNC=2DD4?
	cmd(0xC47F);//@PWR,NO RSTRIC,!st,!fi,OE,EN
	cmd(0x9870);//!mp,9810=30kHz,MAX OUT
	cmd(0xCC13);//OB1?OB0,?lpx,?ddy?DDIT?BW0
	cmd(0xE000);//NOT USE
	cmd(0xC800);//NOT USE
	cmd(0xC040);//1.66MHz,2.2V*/
	unsigned int cnt = 0;
	while (1)
	{
#ifdef FLASH_DEVICE
		if(pgm_read_word_near(&settings[cnt]))
			cmd(pgm_read_word_near(&settings[cnt]));
#else
		if(_param->settings[cnt])
			rfm12_cmd(param, _param->settings[cnt]);
#endif
		else
			break;
		cnt++;
	}

	Sysdelay(10);
}

void rfm12_deinit(void *param)
{
	rfm12_t *_param = (rfm12_t *)param;
	if(!_param->rx_buffer)
		free(_param->rx_buffer);
	if(!_param->tx_buffer)
		free(_param->tx_buffer);
	_param->driver_initialized = false;
}

void rfm12_byte_send(void *param, unsigned char data)
{
	rfm12_t *_param = (rfm12_t *)param;
	while(gpio_in(_param->IRQ_PORT));
	rfm12_cmd(param, 0xB800 + data);
}

bool rfm12_byte_receive(void *param, unsigned char *data, unsigned long timeout)
{
	rfm12_t *_param = (rfm12_t *)param;
	while(gpio_in(_param->IRQ_PORT))
	{
		if(!timeout--)
			return false;
	}
	//cmd(0x0000);
	*data = rfm12_cmd(param, 0xB000) & 0x00FF;
	return true;
}

bool rfm12_packet_send(void *param, unsigned char *buff, unsigned short len, unsigned long address)
{
#if RFM12_USE_SERVICE
	if(len + 13 > rtx_buff_len)
		return false;
	unsigned char tmp = 0;
	unsigned int cnt = 0;
	unsigned short ChkSum = 0;
	rtx_buff[cnt++] = 0xAA;
	rtx_buff[cnt++] = 0xAA;
	rtx_buff[cnt++] = 0xAA;
	rtx_buff[cnt++] = 0xAA;
	rtx_buff[cnt++] = 0x2D;
	rtx_buff[cnt++] = 0xD4;
	tmp = address >> 24;
	rtx_buff[cnt++] = tmp;
	ChkSum += tmp;
	tmp = address >> 16;
	rtx_buff[cnt++] = tmp;
	ChkSum += tmp;
	tmp = address >> 8;
	rtx_buff[cnt++] = tmp;
	ChkSum += tmp;
	tmp = address;
	rtx_buff[cnt++] = tmp;
	ChkSum += tmp;
	tmp = len >> 8;
	rtx_buff[cnt++] = tmp;
	ChkSum += tmp;
	tmp = len;
	rtx_buff[cnt++] = tmp;
	ChkSum += tmp;
	memcpy(rtx_buff + cnt, buff, len);
	//unsigned int tmp_cnt = 0;
	for (cnt = 0; cnt < len; cnt++)
	{
		tmp = buff[cnt];
		ChkSum += tmp;
	}
	cnt += len;
	rtx_buff[cnt++] = ChkSum >> 8;
	rtx_buff[cnt++] = ChkSum;
	rtx_cnt = 0;
	service_status = RTX_STATUS_TX_BUSY;
	rtx_bytes_rs = cnt;
	cmd(0x0000);//read status register
	cmd(0x8238);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	param->CS_PORT->OUTCLR = 1 << param->CS_PIN;
	rfm12_byte_send(rtx_buff[rtx_cnt++]);//PREAMBLE
	return true;
#else
	rfm12_cmd(param, 0x0000);//read status register
	rfm12_cmd(param, 0x8238);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	rfm12_cs_assert(param);
	rfm12_byte_send(param, 0xAA);//PREAMBLE
	rfm12_byte_send(param, 0xAA);//PREAMBLE
	rfm12_byte_send(param, 0xAA);//PREAMBLE
	rfm12_byte_send(param, 0x2D);//SYNC HI BYTE
	rfm12_byte_send(param, 0xD4);//SYNC LOW BYTE
	unsigned short cnt = 0;
	unsigned char tmp;
	//param->spi_unit->spim_buff = &tmp;
	unsigned short ChkSum = 0;
	/* Send address */
	tmp = address >> 24;
	rfm12_byte_send(param, tmp);
	ChkSum += tmp;

	tmp = address >> 16;
	rfm12_byte_send(param, tmp);
	ChkSum += tmp;

	tmp = address >> 8;
	rfm12_byte_send(param, tmp);
	ChkSum += tmp;

	tmp = address;
	rfm12_byte_send(param, tmp);
	ChkSum += tmp;
	/* Send length */
	tmp = len >> 8;
	rfm12_byte_send(param, tmp);
	ChkSum += (tmp);
	tmp = len;
	rfm12_byte_send(param, len);
	ChkSum += tmp;
	/* Send data */
	for (cnt = 0; cnt < len; cnt++)
	{
		tmp = buff[cnt];
		ChkSum += tmp;
		rfm12_byte_send(param, tmp);
	}
	/* Send checksum */
	rfm12_byte_send(param, ChkSum >> 8);
	rfm12_byte_send(param, ChkSum);
	rfm12_byte_send(param, 0xAA);//PREAMBLE
	//rfm12_byte_send(0xAA);//PREAMBLE
	//rfm12_byte_send(0xAA);//PREAMBLE
	rfm12_cs_deassert(param);
	rfm12_cmd(param, 0x8200);
	return true;
#endif
}

bool rfm12_enable_rx(void *param, unsigned long address)
{
	rfm12_t *_param = (rfm12_t *)param;
	_param->address = address;
	return true;
}

signed int rfm12_packet_receive(void *param, unsigned char *buff, unsigned short max_len, unsigned long timeout)
{
	rfm12_t *_param = (rfm12_t *)param;
#if RFM12_USE_SERVICE
	rtx_cnt = 0;
	rx_packet_len = 0;
	service_status = RTX_STATUS_RX_BUSY;
	cmd(0x0000);
	cmd(0x82D8);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	//Init FIFO
	cmd(0xCA81);
	//Enable FIFO
	cmd(0xCA83);
	return 0;
#else
	rfm12_cmd(param, 0x0000);
	rfm12_cmd(param, 0x82D8);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	//Init FIFO
	rfm12_cmd(param, 0xCA81);
	//Enable FIFO
	rfm12_cmd(param, 0xCA83);
	unsigned short ChkSum = 0;
	unsigned short ChkSumRec = 0;
	rfm12_cs_assert(param);
	unsigned char tmp = 0;
	unsigned long addr = 0;
	unsigned short length = 0;
	//if(!byte_receive(&tmp, timeout))
		//return -1;
	//DEBUG_UART.Put_Hex_Char(tmp);
	/* Receive address */
	if(!rfm12_byte_receive(param, &tmp, timeout))
		return -1;
	addr += ((unsigned long)tmp << 24);
	ChkSum += tmp;
	if(!rfm12_byte_receive(param, &tmp, timeout))
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	addr += ((unsigned long)tmp << 16);
	ChkSum += tmp;
	if(!rfm12_byte_receive(param, &tmp, timeout))
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	addr += tmp << 8;
	ChkSum += tmp;
	if(!rfm12_byte_receive(param, &tmp, timeout))
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	addr += tmp;
	ChkSum += tmp;
	/* Receive length */
	if(!rfm12_byte_receive(param, &tmp, timeout))
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	length += tmp << 8;
	ChkSum += tmp;
	if(!rfm12_byte_receive(param, &tmp, timeout))
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	length += tmp;
	ChkSum += tmp;
	unsigned short cnt = 0;
	for(cnt = 0; cnt < length; cnt++)
	{
		if(cnt >= max_len)
		{
			rfm12_cs_deassert(param);
			return -1;
		}
		if(!rfm12_byte_receive(param, &tmp, timeout))
		{
			rfm12_cs_deassert(param);
			return -1;
		}
		buff[cnt] = tmp;
		ChkSum += tmp;
	}
	if(!rfm12_byte_receive(param, &tmp, timeout))
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	ChkSumRec += tmp << 8;
	if(!rfm12_byte_receive(param, &tmp, timeout))
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	ChkSumRec += tmp;
	if(ChkSumRec != ChkSum)
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	if(_param->address != addr)
	{
		rfm12_cs_deassert(param);
		return -1;
	}
	rfm12_cs_deassert(param);
	//Disable FIFO
	//cmd(0xCA81);
	rfm12_cmd(param, 0x8200);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
	return length;
#endif
}

void rfm12_set_power(void *param, unsigned char power, unsigned char mod)
{
	rfm12_cmd(param, 0x9800|(power&7)|((mod&15)<<4));
}

void rfm12_rtx_service(void *param)
{
	rfm12_t *_param = (rfm12_t *)param;
	if(_param->service_status == RTX_STATUS_TX_BUSY)
	{
		if(_param->rtx_cnt < _param->rtx_bytes_rs)
		{
			rfm12_cs_deassert(param);
			rfm12_cmd(param, 0x8200);
			_param->service_status = RTX_STATUS_FREE;
			return;
		}
		rfm12_byte_send(param, _param->rtx_buff[_param->rtx_cnt++]);
	}
	else if(_param->service_status == RTX_STATUS_RX_BUSY || _param->service_status == RTX_STATUS_ADDR_NOT_MATCH || _param->service_status == RTX_STATUS_RX_BUFF_OVERFLOW || _param->service_status == RTX_STATUS_CHK_SUM_ERR)
	{
		if(_param->rtx_cnt < _param->rtx_buff_len)
		{
			_param->rtx_buff[_param->rtx_cnt++] = rfm12_cmd(param, 0xB000) & 0x00FF;
			if(_param->service_status != RTX_STATUS_ADDR_MATCH && _param->rtx_cnt > 4)
			{
				unsigned long rcv_address = ((unsigned long)_param->rtx_buff[0] << 24) + ((unsigned long)_param->rtx_buff[1] << 16) + (_param->rtx_buff[2] << 8) + _param->rtx_buff[3];
				if(_param->address != rcv_address)
				{
					rfm12_cs_deassert(param);
					rfm12_cmd(param, 0x8200);//!er,!ebb,ET,ES,EX,!eb,!ew,DC

					rfm12_cmd(param, 0x0000);
					rfm12_cmd(param, 0x82D8);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
					//Init FIFO
					rfm12_cmd(param, 0xCA81);
					//Enable FIFO
					rfm12_cmd(param, 0xCA83);
					_param->rtx_cnt = 0;
					_param->rx_packet_len = 0;
					_param->service_status = RTX_STATUS_ADDR_NOT_MATCH;
					return;
				}
				else
				{
					_param->service_status = RTX_STATUS_ADDR_MATCH;
				}
			}
			else if(_param->rx_packet_len == 0 && _param->rtx_cnt > 6 && _param->service_status == RTX_STATUS_ADDR_MATCH)
			{
				unsigned short data_len = (_param->rtx_buff[4] << 8) + _param->rtx_buff[5];
				if(data_len + 8 < _param->rtx_buff_len)
				{
					_param->rx_packet_len = data_len + 8;
				}
				else
				{
					rfm12_cs_deassert(param);
					rfm12_cmd(param, 0x8200);//!er,!ebb,ET,ES,EX,!eb,!ew,DC

					rfm12_cmd(param, 0x0000);
					rfm12_cmd(param, 0x82D8);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
					//Init FIFO
					rfm12_cmd(param, 0xCA81);
					//Enable FIFO
					rfm12_cmd(param, 0xCA83);
					_param->rtx_cnt = 0;
					_param->rx_packet_len = 0;
					_param->service_status = RTX_STATUS_RX_BUFF_OVERFLOW;
					return;
				}
			}
			else if(_param->rtx_cnt == _param->rx_packet_len)
			{
				unsigned short ChkSum = 0;
				unsigned short ChkSumRec = 0;
				unsigned int cnt = 0;
				for (cnt = 0; cnt < _param->rx_packet_len - 8; cnt++)
				{
					ChkSum += _param->rtx_buff[cnt];
				}
				if(ChkSum != ChkSumRec)
				{
					rfm12_cs_deassert(param);
					rfm12_cmd(param, 0x8200);//!er,!ebb,ET,ES,EX,!eb,!ew,DC

					rfm12_cmd(param, 0x0000);
					rfm12_cmd(param, 0x82D8);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
					//Init FIFO
					rfm12_cmd(param, 0xCA81);
					//Enable FIFO
					rfm12_cmd(param, 0xCA83);
					_param->rtx_cnt = 0;
					_param->rx_packet_len = 0;
					_param->service_status = RTX_STATUS_CHK_SUM_ERR;
					return;
				}
				else
				{
					_param->service_status = RTX_STATUS_PACKET_RX_COMPLETE;
				}
			}

		}
		else
		{
			rfm12_cs_deassert(param);
			rfm12_cmd(param, 0x8200);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
			_param->rx_packet_len -= 8;
			memcpy(_param->rtx_buff, _param->rtx_buff + 6, _param->rx_packet_len);
			_param->service_status = RTX_STATUS_PACKET_RX_COMPLETE;
		}
	}
}


