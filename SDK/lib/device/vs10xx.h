/*
 * vs10xx.h
 *
 *  Created on: Sep 16, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_VS10XX_H_
#define LIB_DEVICE_VS10XX_H_

#include "api/mcspi_def.h"
#include "api/gpio_def.h"

/*#####################################################*/
#define VS_SCI_MODE			0x00	//RW	Mode control
#define VS_SCI_STATUS		0x01	//RW	Status
#define VS_SCI_BASS			0x02	//RW	Built-in bass enhancer
#define VS_SCI_CLOCKF		0x03	//RW	Clock freq+doubler
#define VS_SCI_DEC_TIME		0x04	//R		Decode time in seconds
#define VS_SCI_AUDATA		0x05	//RW	Misc. audio data
#define VS_SCI_WRAM			0x06	//RW	RAM write
#define VS_SCI_WRAMADDR		0x07	//RW	Base address for RAM write
#define VS_SCI_HDAT0		0x08	//R		Stream header data 0
#define VS_SCI_HDAT1		0x09	//R		Stream header data 1
#define VS_SCI_AIADDR		0x0A	//RW	Start address of application
#define VS_SCI_VOL			0x0B	//RW	Volume control
#define VS_SCI_AICTRL0		0x0C	//RW	Application control register 0
#define VS_SCI_AICTR11		0x0D	//RW	Application control register 1
#define VS_SCI_AICTRL2		0x0E	//RW	Application control register 2
#define VS_SCI_AICTRL3		0x0F	//RW	Application control register 3

#define VS_INS_WRITE		0x02
#define VS_INS_READ			0x03
/*#####################################################*/
typedef struct vs10xx_s{
	unsigned char spi_cmd_instance;
	unsigned char spi_data_instance;
	new_mcspi *spi;
	new_gpio *rst;
	new_gpio *dreq;
}vs10xx_t;
/*#####################################################*/
void vs10xx_soft_reset(void *param);
void vs10xx_hard_reset(void *param);
void vs10xx_set_pll(void *param, unsigned long QFreq);
void vs10xx_set_volume(void *param, unsigned char R, unsigned char L);
unsigned short vs10xx_get_volume(void *param);
void vs10xx_send_null(void *param, unsigned short len);
bool vs10xx_send_32B_data(void *param, unsigned char* buffer);
bool vs10xx_send_1byte_data(void *param, unsigned char Data);
void vs10xx_init(void *param, new_mcspi *spi, unsigned char spi_cmd_instance, unsigned char spi_data_instance, 	new_gpio *rst, new_gpio *dreq);
/*#####################################################*/
#define new_vs10xx vs10xx_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_vs10xx(address) free(address);
/*#####################################################*/
#endif /* LIB_DEVICE_VS10XX_H_ */
