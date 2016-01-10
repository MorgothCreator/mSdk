/*
 * _45db.cpp
 *
 * Created: 1/1/2016 4:55:49 PM
 *  Author: John Smith
 */ 

#include <stdbool.h>
#include "45db.h"
#include "api/mcspi_def.h"
#include "api/mcspi_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"

bool device_45db_init(dev45db_t *Settings, unsigned char Wp_PortNr, unsigned char Wp_PinNr, unsigned char Hold_PortNr, unsigned char Hold_PinNr)
{
	Settings->WP_Port = gpio_assign(Wp_PortNr, Wp_PinNr, GPIO_OUT_PUSH_PULL, false);
	Settings->Reset_Port = gpio_assign(Hold_PortNr, Hold_PinNr, GPIO_OUT_PUSH_PULL, false);
	if(Settings->WP_Port == NULL || Settings->Reset_Port == NULL)
	{
		gpio_free(Settings->WP_Port);
		gpio_free(Settings->Reset_Port);
		return false;
	}
	gpio_out(Settings->WP_Port, 1);
	gpio_out(Settings->Reset_Port, 1);
	return true;
}

bool device_45db_read_status(Mcspi_t *SpiStruct, char *status) {
	//char tmp = 0xD7;
	SpiStruct->Buff[0] = 0xD7;
	mcspi_transfer(SpiStruct, 1, 1);
	//DEVICE_45DB_CS_ASSERT
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//DEVICE_45DB_CS_DEASSERT
	*status = SpiStruct->Buff[1];
	return true;
}

bool device_45db_enter_sleep(Mcspi_t *SpiStruct) {
	/*char tmp = 0xB9;
	DEVICE_45DB_CS_ASSERT
	SPI_MasterTransmitReceive(&tmp);
	DEVICE_45DB_CS_DEASSERT
	_delay_us(4);*/
	return true;
}

bool device_45db_resume_sleep(Mcspi_t *SpiStruct) {
	/*char tmp = 0xAB;
	DEVICE_45DB_CS_ASSERT
	SPI_MasterTransmitReceive(&tmp);
	DEVICE_45DB_CS_DEASSERT
	_delay_us(40);*/
	return true;
}

bool device_45db_write_buff(Mcspi_t *SpiStruct, unsigned char buff_nr, unsigned int page_nr, char *data) {
	//char tmp = 0;
	//DEVICE_45DB_CS_ASSERT
	if(buff_nr == 0)
		SpiStruct->Buff[0] = 0x84;
	else
		SpiStruct->Buff[0] = 0x87;
	//SPI_MasterTransmitReceive(&tmp);
	//tmp = 0;
	//SPI_MasterTransmitReceive(&tmp);
	//tmp = 0;
	//SPI_MasterTransmitReceive(&tmp);
	//tmp = 0;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[1] = 0x00;
	SpiStruct->Buff[2] = 0x00;
	SpiStruct->Buff[3] = 0x00;
	SpiStruct->Buff[4] = 0x00;
	unsigned int cnt = 5;
	for (; cnt < DEVICE_45DB_PAGE_SIZE + 5; cnt++)
	{
		//asm("wdr");
		SpiStruct->Buff[cnt] = data[cnt];
		//SPI_MasterTransmitReceive(&tmp);
	}
	mcspi_transfer(SpiStruct, DEVICE_45DB_PAGE_SIZE + 5, 0);
	//DEVICE_45DB_CS_DEASSERT
	return true;
}

bool device_45db_write_from_buffer(Mcspi_t *SpiStruct, unsigned char buff_nr, unsigned int page_nr) {
	char tmp = 0;
	//DEVICE_45DB_CS_ASSERT
	//if(buff_nr == 0)
	//	tmp = 0x83;
	//else
	//	tmp = 0x86;
	//SPI_MasterTransmitReceive(&tmp);
	if(buff_nr == 0)
		SpiStruct->Buff[0] = 0x83;
	else
		SpiStruct->Buff[0] = 0x86;
	unsigned long addr_tmp = ((unsigned long)page_nr) << 10;
	addr_tmp = addr_tmp & ~0x3FF;
	SpiStruct->Buff[1] = addr_tmp>>16;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[2] = addr_tmp>>8;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[3] = addr_tmp;
	//SPI_MasterTransmitReceive(&tmp);
	//DEVICE_45DB_CS_DEASSERT
	mcspi_transfer(SpiStruct, 4, 0);
	volatile unsigned long timeout = DEVICE_45DB_WRITE_TIMEOUT;

	do
	{
		device_45db_read_status(SpiStruct, &tmp);
		if(!timeout--)
		return false;
	} while (!(tmp & (1<<DEVICE_45DB_RDY)));
	return true;
}

bool device_45db_page_erase(Mcspi_t *SpiStruct, unsigned int page_nr) {
	char tmp = 0;
	//DEVICE_45DB_CS_ASSERT
	SpiStruct->Buff[0] = 0x81;
	//SPI_MasterTransmitReceive(&tmp);
	unsigned long addr_tmp = ((unsigned long)page_nr) << 10;
	addr_tmp = addr_tmp & ~0x3FF;
	SpiStruct->Buff[1] = addr_tmp>>16;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[2] = addr_tmp>>8;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[3] = addr_tmp;
	//SPI_MasterTransmitReceive(&tmp);
	//DEVICE_45DB_CS_DEASSERT
	mcspi_transfer(SpiStruct, 4, 0);

	volatile unsigned long timeout = DEVICE_45DB_WRITE_TIMEOUT;
	do
	{
		device_45db_read_status(SpiStruct, &tmp);
		if(!timeout--)
		return false;
	} while (!(tmp & (1<<DEVICE_45DB_RDY)));
	return true;
}

bool device_45db_write(Mcspi_t *SpiStruct, unsigned char buff_nr, unsigned int page_nr, char *data) {
	device_45db_page_erase(SpiStruct, page_nr);
	device_45db_write_buff(SpiStruct, buff_nr, page_nr, data);
	device_45db_write_from_buffer(SpiStruct, buff_nr, page_nr);
	return true;
}

bool device_45db_read(Mcspi_t *SpiStruct, unsigned int page_nr, char *data, unsigned int size) {
	//char tmp = 0;
	//DEVICE_45DB_CS_ASSERT
	SpiStruct->Buff[0] = 0xE8;
	//SPI_MasterTransmitReceive(&tmp);
	unsigned long addr_tmp = ((unsigned long)page_nr) << 10;
	addr_tmp = addr_tmp & ~0x3FF;
	SpiStruct->Buff[1] = addr_tmp>>16;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[2] = addr_tmp>>8;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[3] = addr_tmp;
	//SPI_MasterTransmitReceive(&tmp);
	
	SpiStruct->Buff[4] = 0xFF;
	SpiStruct->Buff[5] = 0xFF;
	SpiStruct->Buff[6] = 0xFF;
	SpiStruct->Buff[7] = 0xFF;
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	mcspi_transfer(SpiStruct, 8, size);

	unsigned int cnt = 8;
	unsigned int _size = size;
	if(_size > DEVICE_45DB_PAGE_SIZE)
	_size = DEVICE_45DB_PAGE_SIZE;
	for (; cnt < _size + 8; cnt++)
	{
		//SPI_MasterTransmitReceive(&tmp);
		data[cnt] = SpiStruct->Buff[cnt];
	}
	//DEVICE_45DB_CS_DEASSERT
	return true;
}

/*void device_45db_print_hex(Mcspi_t *SpiStruct, unsigned char data)
{
	unsigned char ch1 = data >> 4;
	unsigned char ch2 = data & 0x0F;
	if(ch1 < 10)
		Usart.transmit(ch1 + '0');
	else
		Usart.transmit((ch1 - 10) + 'A');
	if(ch2 < 10)
		Usart.transmit(ch2 + '0');
	else
		Usart.transmit((ch2 - 10) + 'A');
}*/


/*bool device_45db_print(Mcspi_t *SpiStruct, unsigned int page_nr, unsigned int size) {
	//char tmp = 0;
	//DEVICE_45DB_CS_ASSERT
	SpiStruct->Buff[0] = 0xE8;
	//SPI_MasterTransmitReceive(&tmp);
	unsigned long addr_tmp = ((unsigned long)page_nr) << 10;
	addr_tmp = addr_tmp & ~0x3FF;
	SpiStruct->Buff[1] = addr_tmp>>16;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[2] = addr_tmp>>8;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[3] = addr_tmp;
	//SPI_MasterTransmitReceive(&tmp);
	
	SpiStruct->Buff[4] = 0xFF;
	SpiStruct->Buff[5] = 0xFF;
	SpiStruct->Buff[6] = 0xFF;
	SpiStruct->Buff[7] = 0xFF;
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	mcspi_transfer(SpiStruct, 8, size);

	unsigned int cnt = 8;
	unsigned int _size = size;
	if(_size > DEVICE_45DB_PAGE_SIZE)
	_size = DEVICE_45DB_PAGE_SIZE;
	for (; cnt < _size + 8; cnt++)
	{
		//SPI_MasterTransmitReceive(&tmp);
		//USART_Transmit((tmp & 0x0F) + '0');
		//USART_Transmit(((tmp >> 4) & 0x0F) + '0');
		device_45db_print_hex(SpiStruct->Buff[7]);
	}
	//DEVICE_45DB_CS_DEASSERT
	return true;
}*/

bool device_45db_check_page_empty(Mcspi_t *SpiStruct, unsigned int page_nr) {
	//char tmp = 0;
	//DEVICE_45DB_CS_ASSERT
	SpiStruct->Buff[0] = 0xE8;
	//SPI_MasterTransmitReceive(&tmp);
	unsigned long addr_tmp = ((unsigned long)page_nr) << 10;
	addr_tmp = addr_tmp & ~0x3FF;
	SpiStruct->Buff[1] = addr_tmp>>16;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[2] = addr_tmp>>8;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[3] = addr_tmp;
	//SPI_MasterTransmitReceive(&tmp);
	
	SpiStruct->Buff[4] = 0xFF;
	SpiStruct->Buff[5] = 0xFF;
	SpiStruct->Buff[6] = 0xFF;
	SpiStruct->Buff[7] = 0xFF;
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	mcspi_transfer(SpiStruct, 8, 64);
	
	unsigned int cnt = 8;
	for (; cnt < 64 + 8; cnt++)
	{
		//SPI_MasterTransmitReceive(&tmp);
		if(SpiStruct->Buff[cnt] != 0xFF)
		{
			//DEVICE_45DB_CS_DEASSERT
			return false;
		}
	}
	//DEVICE_45DB_CS_DEASSERT
	return true;
}

bool device_45db_werify(Mcspi_t *SpiStruct, unsigned int page_nr, char *data, unsigned int size) {
	//char tmp = 0;
	//DEVICE_45DB_CS_ASSERT
	SpiStruct->Buff[0] = 0xE8;
	//SPI_MasterTransmitReceive(&tmp);
	unsigned long addr_tmp = ((unsigned long)page_nr) << 10;
	addr_tmp = addr_tmp & ~0x3FF;
	SpiStruct->Buff[1] = addr_tmp>>16;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[2] = addr_tmp>>8;
	//SPI_MasterTransmitReceive(&tmp);
	SpiStruct->Buff[3] = addr_tmp;
	//SPI_MasterTransmitReceive(&tmp);
	
	SpiStruct->Buff[4] = 0xFF;
	SpiStruct->Buff[5] = 0xFF;
	SpiStruct->Buff[6] = 0xFF;
	SpiStruct->Buff[7] = 0xFF;
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	//SPI_MasterTransmitReceive(&tmp);
	mcspi_transfer(SpiStruct, 8, size);


	unsigned int cnt = 0;
	unsigned int _size = size;
	if(_size > DEVICE_45DB_PAGE_SIZE)
	_size = DEVICE_45DB_PAGE_SIZE;
	for (; cnt < _size; cnt++)
	{
		//SPI_MasterTransmitReceive(&tmp);
		if(data[cnt] != SpiStruct->Buff[cnt + 8]) {
			//DEVICE_45DB_CS_DEASSERT
			return false;
		}
	}
	//DEVICE_45DB_CS_DEASSERT
	return true;
}
