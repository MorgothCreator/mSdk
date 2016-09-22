/*
 * nrf24l01.c
 *
 *  Created on: Aug 28, 2016
 *      Author: John Smith
 */

#include "nrf24l01.h"

extern unsigned long FCPU;

void nrf24l01_Delay (unsigned long a)
{
	volatile unsigned long b = a;
	do{

	}
	while (--b!=0);
}

// Transmit byte via SPI0 chanel
unsigned char  nrf24l01_SPI_SendByte(void *param, unsigned char data)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->spi->CsSelect = _param->spi_instance;
	return mcspi_send_byte(_param->spi, data);
}

void nrf24l01_cs_assert(void *param)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->spi->CsSelect = _param->spi_instance;
	mcspi_assert(_param->spi);
}
void nrf24l01_cs_deassert(void *param)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->spi->CsSelect = _param->spi_instance;
	mcspi_deassert(_param->spi);
}

void nrf24l01_ce_assert(void *param)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	gpio_out(_param->ce, 1);
}
void nrf24l01_ce_deassert(void *param)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	gpio_out(_param->ce, 0);
}

bool nrf24l01_irq_state(void *param)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	if(gpio_in(_param->irq))
		return false;
	else
		return true;
}


unsigned char nrf24l01_send_command_with_ADDR (void *param, unsigned char cmd, unsigned char addr, unsigned char data_byte)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
  unsigned char temp,command = 0;
  command = (cmd << 5) | addr;
  nrf24l01_cs_assert(param);
  if (cmd == R_REGISTER)
  {
	  if (addr == STATUS_ADDR)
	  {
	      nrf24l01_SPI_SendByte(param, command);
	      temp=nrf24l01_SPI_SendByte(param, NOP);
	      nrf24l01_cs_deassert(param);
	      return temp;
	  }
    if (addr == RX_ADDR_P0 || addr == RX_ADDR_P1 || addr == TX_ADDR)
    {

    	_param->status=nrf24l01_SPI_SendByte(param, command);
    	int k;
    	for (k=0;k!=5;k++)
      {
    	  _param->read_addr[k]=nrf24l01_SPI_SendByte(param, NOP);
      }
      nrf24l01_cs_deassert(param);
      return _param->status;
    }
    else
    {
    	_param->status=nrf24l01_SPI_SendByte(param, command);
      temp=nrf24l01_SPI_SendByte(param, NOP);
      nrf24l01_cs_deassert(param);
      return temp;
    }
  }
  if (cmd == W_REGISTER)
  {
    if (addr == RX_ADDR_P0)
    {
    	_param->status=nrf24l01_SPI_SendByte(param, command);
    	int j;
      for (j=0;j!=5;j++)
        {
          temp=_param->rx_addr0[j];
          nrf24l01_SPI_SendByte(param, temp);
        }
      nrf24l01_cs_deassert(param);
      return _param->status;
    }

    if (addr == RX_ADDR_P1)
    {
    	_param->status=nrf24l01_SPI_SendByte(param, command);
    	int j;
      for (j=0;j!=5;j++)
        {
          temp=_param->rx_addr1[j];
          nrf24l01_SPI_SendByte(param, temp);
        }
      nrf24l01_cs_deassert(param);
      return _param->status;
    }

    if (addr == TX_ADDR)
    {
    	_param->status=nrf24l01_SPI_SendByte(param, command);
    	int j;
      for (j=0;j!=5;j++)
        {
          temp=_param->tx_addr[j];
          nrf24l01_SPI_SendByte(param, temp);
        }
      nrf24l01_cs_deassert(param);
      return _param->status;
    }

    else
    {
      temp=nrf24l01_SPI_SendByte(param, command);
      nrf24l01_SPI_SendByte(param, data_byte);
      nrf24l01_cs_deassert(param);
      return temp;
    }
  }

  return 1;
}

unsigned char nrf24l01_send_command_without_ADDR (void *param, unsigned char cmd, unsigned char data_byte)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	unsigned char temp = 0;
	nrf24l01_cs_assert(param);
	if (cmd == R_RX_PAYLOAD || cmd == R_RX_PL_WID)
	{
		_param->status=nrf24l01_SPI_SendByte(param, cmd);
		temp=nrf24l01_SPI_SendByte(param, NOP);
		nrf24l01_cs_deassert(param);
		return temp;
	}
	if (cmd == W_TX_PAYLOAD)
	{
		_param->status=nrf24l01_SPI_SendByte(param, cmd);
		nrf24l01_SPI_SendByte(param, data_byte);
		nrf24l01_cs_deassert(param);
		return _param->status;
	}
	_param->status = nrf24l01_SPI_SendByte(param, cmd);
	nrf24l01_cs_deassert(param);
	return _param->status;
}

void nrf24l01_set_enc_soc_burst(void *param, unsigned char pipe_mask)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, EN_AA, pipe_mask & 0x1F);
}

void nrf24l01_set_pipe(void *param, unsigned char pipe_mask)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, EN_RXADDR, pipe_mask & 0x1F);
}

void nrf24l01_set_addr_field_with(void *param, unsigned char size)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, SETUP_AW, (size - 2) & 0x03);
}

void nrf24l01_set_auto_retransmit_delay_count(void *param, unsigned char delay, unsigned char count)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, SETUP_RETR, (delay << 4 & 0xF0) | (count & 0x0F));
}

void nrf24l01_set_rx(void *param)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, CONFIG_REG_ADDR, 0x0F);
}

void nrf24l01_set_tx(void *param)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, CONFIG_REG_ADDR, 0x0E);
}

void nrf24l01_setup(void *param, bool continuous_carryer_transmit, unsigned char rf_data_speed, bool force_pll_lock, unsigned char rf_pwr)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RF_SETUP, (continuous_carryer_transmit << 7) | (rf_data_speed & 0x02) << 4 | force_pll_lock << 4 | (rf_data_speed & 0x01) << 3 | (rf_pwr & 0x03) << 1);
}

void nrf24l01_get_status(void *param, bool continuous_carryer_transmit, unsigned char rf_data_speed, bool force_pll_lock, unsigned char rf_pwr)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	nrf24l01_cs_assert(param);
	_param->status = nrf24l01_SPI_SendByte(param, NOP);
	nrf24l01_cs_deassert(param);
}

unsigned long _address = 0;

bool nrf24l01_enable_rx(void *param, unsigned long address)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	//if(_param->RX_STATE == NRF24L01_RX_DISABLED)
	//{
		_param->rx_addr0[0] = address;
		_param->rx_addr0[1] = address >> 8;
		_param->rx_addr0[2] = address >> 16;
		_param->rx_addr0[3] = address >> 24;
		//_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RX_ADDR_P0, NOP);
		//_param->RX_STATE = NRF24L01_RX_ACTIVE;
		//nrf24l01_set_rx(param);
		////nrf24l01_Delay(FCPU / (1.0 / 0.000013));
		//nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
		//nrf24l01_send_command_without_ADDR(param, FLUSH_TX, NOP);
		//nrf24l01_ce_assert(param);
		//return true;
	//}
	return true;
}
signed short nrf24l01_packet_receive(void *param, unsigned char *packet, unsigned short max_len, unsigned long timeout)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	if(_param->RX_STATE == NRF24L01_RX_DISABLED)
	{
		//_param->rx_addr0[0] = address;
		//_param->rx_addr0[1] = address >> 8;
		//_param->rx_addr0[2] = address >> 16;
		//_param->rx_addr0[3] = address >> 24;
		_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RX_ADDR_P0, NOP);
		_param->RX_STATE = NRF24L01_RX_ACTIVE;
		nrf24l01_set_rx(param);
		//nrf24l01_Delay(FCPU / (1.0 / 0.000013));
		nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
		nrf24l01_send_command_without_ADDR(param, FLUSH_TX, NOP);
		nrf24l01_ce_assert(param);
		//return true;
	}
	volatile unsigned char data_len = -1;
	if(_param->RX_STATE == NRF24L01_RX_ACTIVE)
	{
		if(nrf24l01_irq_state(param))
		{
			unsigned char status = nrf24l01_send_command_without_ADDR(param, R_REGISTER, NOP);
		    // Set high when new data arrives RX FIFO
			if ((status & RX_DR) != 0)
			{
				nrf24l01_ce_deassert(param);
				data_len = nrf24l01_send_command_without_ADDR(param, R_RX_PL_WID, NOP);
				if(data_len > 32)
				{
					nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
					data_len = 0;
					return -1;
				}
				unsigned int cnt = 0;
				nrf24l01_cs_assert(param);
				nrf24l01_SPI_SendByte(param, R_RX_PAYLOAD);
				for(; cnt < data_len; cnt++)
				{
					*packet++ = nrf24l01_SPI_SendByte(param, NOP);
				}
				nrf24l01_cs_deassert(param);
				// Clear RX_DR bit in status register
				nrf24l01_send_command_with_ADDR(param, W_REGISTER, STATUS_ADDR, (RX_DR));
				_param->RX_STATE = NRF24L01_RX_DISABLED;
				nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
				nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
				//nrf24l01_ce_assert(param);
				return data_len;
			}
			else
			{
				nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
				nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
			}
		}
	}
	return -1;
}

signed int nrf24l01_packet_send(void *param, unsigned char *packet, unsigned char len, unsigned long address)
{
	nrf24l01_t *_param = (nrf24l01_t *)param;
	nrf24l01_ce_deassert(param);
	// Setting for TX device
	nrf24l01_send_command_without_ADDR(param, FLUSH_TX, NOP);
	nrf24l01_send_command_without_ADDR(param, FLUSH_RX, NOP);
	volatile unsigned char status;
	_param->RX_STATE = NRF24L01_RX_DISABLED;
    //nrf24l01_send_command_without_ADDR(param, W_TX_PAYLOAD_NO_ACK, NOP);
	_param->tx_addr[0] = address;
	_param->tx_addr[1] = address >> 8;
	_param->tx_addr[2] = address >> 16;
	_param->tx_addr[3] = address >> 24;
	nrf24l01_send_command_with_ADDR(param, W_REGISTER, TX_ADDR, NOP);
	nrf24l01_set_tx(param);
	nrf24l01_Delay(FCPU / (1.0 / 0.000002));
	//Send payload
	unsigned char cnt = 0;
	nrf24l01_cs_assert(param);
	nrf24l01_SPI_SendByte(param, W_TX_PAYLOAD);
	if(len > 32)
		len = 32;
	for(; cnt < len; cnt++)
	{
		nrf24l01_SPI_SendByte(param, *packet++);
	}
	for(; cnt < 32; cnt++)
	{
		nrf24l01_SPI_SendByte(param, 0);
	}
	nrf24l01_cs_deassert(param);
	// Pulse for CE -> starts the transmission.
	nrf24l01_ce_assert(param);
	nrf24l01_Delay(FCPU / (1.0 / 0.000002));
	nrf24l01_ce_deassert(param);
    // Read STATUS register
    //status = nrf24l01_send_command_with_ADDR(param, R_REGISTER, OBSERVE_TX, NOP);
	volatile int timeout = 100;
    do {
        status = nrf24l01_send_command_without_ADDR(param, R_REGISTER, NOP);
    }while(((~status) & MAX_RT) && ((~status) & TX_DS) && --timeout);
    signed int ret = -3;
    // if exceed number of transmission packets
    if ((status & MAX_RT) != 0) {

      // Clear MAX_RT bit in status register
      nrf24l01_send_command_with_ADDR(param, W_REGISTER, STATUS_ADDR, (MAX_RT));

      // No communication event here
      // ....
      ret = -1;
    }

    // If packet sent on TX
    if ((status & TX_DS) != 0) {

      // Clear TX_DS bit in status register
      nrf24l01_send_command_with_ADDR(param, W_REGISTER, STATUS_ADDR, (TX_DS));

      // Your code here
      // ...
      ret = len;
    }

    // If TX full
    if ((status & TX_FULL) != 0)  {

      // Flush TX FIFO (in TX mode)
      nrf24l01_send_command_without_ADDR(param, FLUSH_TX, NOP);

      // Your code here
      // ...
      ret = -2;
    }
    if(!timeout)
    	ret = -4;
    // Setting for RX device
    //Write CONFIG register -> 00001010 - CRC enable, power-up, RX
    nrf24l01_set_rx(param);
	return ret;
}

bool nrf24l01_init(void *param)
{
	nrf24l01_Delay(FCPU / (1.0 / 0.01));
	nrf24l01_t *_param = (nrf24l01_t *)param;
	  //Write CONFIG register (addres - 0x00)
	  //00001010 - CRC enable, power-up, RX
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, CONFIG_REG_ADDR, 0x0F);
	  // read
	  // config = SPI_Send_command_with_ADDR(R_REGISTER, CONFIG_REG_ADDR, NOP);

	  //Write RX_ADDR_P0 register -> Set receive address data Pipe0 -> address in RX_ADDRESS_P0 array
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RX_ADDR_P0, NOP);
	  // read
	  // status = SPI_Send_command_with_ADDR(R_REGISTER, RX_ADDR_P0, NOP);

	  //Write RX_ADDR_P1 register -> Set receive address data Pipe1 -> address in RX_ADDRESS_P1 array
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RX_ADDR_P1, NOP);
	  // read
	  // status = SPI_Send_command_with_ADDR(R_REGISTER,RX_ADDR_P1, NOP);

	  //Write TX_ADDR register -> Transmit address. Used for a PTX device only. Address in TX_ADDRESS array
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, TX_ADDR, NOP);
	  // read
	  // status = SPI_Send_command_with_ADDR(R_REGISTER, TX_ADDR, NOP);

	  //Write RX_PW_P0 register -> Set number of bytes in RX payload in data pipe0 -> 1 byte
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RX_PW_P0, 32);
	  // read
	  // dummy = SPI_Send_command_with_ADDR(R_REGISTER, RX_PW_P0, NOP);

	  //Write RX_PW_P1 register -> Set number of bytes in RX payload in data pipe1 -> 1 byte
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RX_PW_P1, 32);
	  // read
	  // dummy = SPI_Send_command_with_ADDR(R_REGISTER, RX_PW_P1, NOP);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, SETUP_AW, 2);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, EN_RXADDR, 1);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RF_CH, 64);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, EN_AA, 0x00);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, SETUP_RETR, 0x00);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, RF_SETUP, 0x07);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, DYNPD, 0/*DYNPD_DPL_P0 | DYNPD_DPL_P1*/);
	_param->status = nrf24l01_send_command_with_ADDR(param, W_REGISTER, FEATURE, 0/*FEATURE_EN_DPL | FEATURE_EN_ACK_PAY*/);
	//param->status = nrf24l01_send_command_with_ADDR(param, R_REGISTER, RX_ADDR_P0, 32);
	//param->status = nrf24l01_send_command_with_ADDR(param, R_REGISTER, TX_ADDR, 32);

return true;
}


