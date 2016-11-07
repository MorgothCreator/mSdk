/*
 * vs11xx.c
 *
 *  Created on: Sep 16, 2016
 *      Author: John Smith
 */

#include "vs10xx.h"

#include "api/timer_api.h"
#include "api/gpio_api.h"
#include "api/mcspi_api.h"

static void vs10xx_assert_rst(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	gpio_out(_param->rst, 0);
}

static void vs10xx_deassert_rst(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	gpio_out(_param->rst, 1);
}

static void vs10xx_assert_dcs(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	_param->spi->CsSelect = _param->spi_data_instance;
	mcspi_assert(_param->spi);
}

static void vs10xx_deassert_dcs(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	_param->spi->CsSelect = _param->spi_data_instance;
	mcspi_deassert(_param->spi);
}

static void vs10xx_assert_cs(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	_param->spi->CsSelect = _param->spi_cmd_instance;
	mcspi_assert(_param->spi);
}

static void vs10xx_deassert_cs(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	_param->spi->CsSelect = _param->spi_cmd_instance;
	mcspi_deassert(_param->spi);
}

static void vs10xx_check_busy(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	while(gpio_in(_param->dreq) == 0);
}

static bool vs10xx_check_busy_skip(void *param)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	if(gpio_in(_param->dreq) == 0)
		return true;
	else
		return false;
}

static void vs10xx_reg_write(void *param, unsigned char Reg, unsigned short Value)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	vs10xx_check_busy(param);
	vs10xx_assert_cs(param);
	unsigned char buffer[4];
	buffer[0] = VS_INS_WRITE;
	buffer[1] = Reg;
	buffer[2] = Value >> 8;
	buffer[3] = Value;
	_param->spi->Buff = (volatile unsigned char *)buffer;
	_param->spi->CsSelect = _param->spi_cmd_instance;
	mcspi_transfer(_param->spi, 4, 0);
	vs10xx_deassert_cs(param);
	sys_delay(1);
}

static unsigned short vs10xx_reg_read(void *param, unsigned char Reg)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	unsigned short Value;
	vs10xx_check_busy(param);
	vs10xx_assert_cs(param);
	unsigned char buffer[4];
	buffer[0] = VS_INS_READ;
	buffer[1] = Reg;
	buffer[2] = 0xFF;
	buffer[3] = 0xFF;
	_param->spi->Buff = (volatile unsigned char *)buffer;
	_param->spi->CsSelect = _param->spi_cmd_instance;
	mcspi_transfer(_param->spi, 2, 2);
	Value = buffer[2] << 8;
	Value += buffer[3];
	vs10xx_deassert_cs(param);
	sys_delay(1);
	return Value;
}

void vs10xx_soft_reset(void *param)
{
	vs10xx_reg_write(param, VS_SCI_MODE, 4);
	vs10xx_reg_write(param, VS_SCI_MODE, 0x0800);
	vs10xx_reg_write(param, VS_SCI_BASS, 0x7A00);
	//vs10xx_reg_write(param, VS_SCI_CLOCKF, 0x2000);
	vs10xx_check_busy(param);
}

void vs10xx_hard_reset(void *param)
{
	vs10xx_assert_rst(param);
	sys_delay(100);
	vs10xx_deassert_rst(param);
	vs10xx_reg_write(param, VS_SCI_MODE, 0x0800);
	vs10xx_reg_write(param, VS_SCI_BASS, 0x7A00);
	//vs10xx_reg_write(param, VS_SCI_CLOCKF, 0x2000);
	vs10xx_check_busy(param);
}

void vs10xx_set_pll(void *param, unsigned long QFreq)
{
	vs10xx_reg_write(param, VS_SCI_CLOCKF, QFreq/2000);
}

void vs10xx_set_volume(void *param, unsigned char R, unsigned char L)
{
	vs10xx_reg_write(param, VS_SCI_VOL, R + (L << 8));
}

unsigned short vs10xx_get_volume(void *param)
{
	return vs10xx_reg_read(param, VS_SCI_VOL);
}

void vs10xx_send_null(void *param, unsigned short len)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	vs10xx_check_busy(param);
	vs10xx_assert_dcs(param);
	_param->spi->CsSelect = _param->spi_data_instance;
	unsigned short NullCount;
	for(NullCount = 0; NullCount < len; NullCount++)
		mcspi_send_byte(_param->spi, 255);
	vs10xx_deassert_dcs(param);
}

bool vs10xx_send_32B_data(void *param, unsigned char* buffer)
{
	if(vs10xx_check_busy_skip(param))
		return false;
	vs10xx_t *_param = (vs10xx_t *)param;
	vs10xx_assert_dcs(param);
	_param->spi->Buff = (volatile unsigned char *)buffer;
	_param->spi->CsSelect = _param->spi_data_instance;
	_param->spi->DisableCsHandle = true;
	mcspi_transfer(_param->spi, 32, 0);
	vs10xx_deassert_dcs(param);
	return true;
}

bool vs10xx_send_1byte_data(void *param, unsigned char Data)
{
	if(vs10xx_check_busy_skip(param))
	{
		vs10xx_t *_param = (vs10xx_t *)param;
		mcspi_send_byte(_param->spi, 255);
		return true;
	}
	return false;
}

void vs10xx_init(void *param, new_mcspi *spi, unsigned char spi_cmd_instance, unsigned char spi_data_instance, 	new_gpio *rst, new_gpio *dreq)
{
	vs10xx_t *_param = (vs10xx_t *)param;
	_param->spi = spi;
	_param->spi_cmd_instance = spi_cmd_instance;
	_param->spi_data_instance = spi_data_instance;
	spi->ClkDiv[spi_cmd_instance] = 4;
	spi->ClkDiv[spi_data_instance] = 4;
	_param->rst = rst;
	_param->dreq = dreq;
	vs10xx_hard_reset(param);
	vs10xx_soft_reset(param);
	vs10xx_set_pll(param, 12288000);
}
