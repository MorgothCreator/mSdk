/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */

#include "mcspi_interface.h"
#include "api/mcspi_def.h"
#include "driver/stm32f4xx_rcc.h"
#include "driver/stm32f4xx_spi.h"
#include "gpio_interface.h"
//#include "driver/spi.h"

extern unsigned long CoreFreq;
#define SPIn                             3

const uint32_t SPI_CLK[] = {
		RCC_APB2Periph_SPI1
#ifdef RCC_APB1Periph_SPI2
		, RCC_APB1Periph_SPI2
#endif
#ifdef RCC_APB1Periph_SPI3
		, RCC_APB1Periph_SPI3
#endif
#ifdef RCC_APB1Periph_SPI4
		, RCC_APB1Periph_SPI4
#endif
#ifdef RCC_APB1Periph_SPI5
		, RCC_APB1Periph_SPI5
#endif
#ifdef RCC_APB1Periph_SPI6
		, RCC_APB1Periph_SPI6
#endif
};

SPI_TypeDef* _SPI_[] = { SPI1, SPI2, SPI3};


bool _mcspi_open(new_mcspi *McspiStruct)
{
	RCC_APB2PeriphClockCmd(GET_PORT_CLK_ADDR[McspiStruct->MosiPort] | GET_PORT_CLK_ADDR[McspiStruct->MisoPort] | GET_PORT_CLK_ADDR[McspiStruct->SckPort] | GET_PORT_CLK_ADDR[McspiStruct->Cs0Port], ENABLE);
	switch(McspiStruct->McspiNr) {
	case 0:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		RCC_APB1PeriphClockCmd(SPI_CLK[McspiStruct->McspiNr], ENABLE);
		break;
	default:
		return false;
	}

	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_PinAFConfig(GET_GPIO_PORT_ADDR[McspiStruct->MosiPort], McspiStruct->MosiPin, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GET_GPIO_PORT_ADDR[McspiStruct->MisoPort], McspiStruct->MisoPin, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GET_GPIO_PORT_ADDR[McspiStruct->SckPort], McspiStruct->SckPin, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->MosiPin;
	GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->MosiPort], &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->MisoPin;
	GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->MisoPort], &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->SckPin;
	GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->SckPort], &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->Cs0Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->Cs0Port], &GPIO_InitStructure);


	SPI_InitTypeDef  SPI_InitStructure;

	/*!< _SPI_ configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(_SPI_[McspiStruct->McspiNr], &SPI_InitStructure);

	/*!< Enable the sFLASH_SPI  */
	SPI_Cmd(_SPI_[McspiStruct->McspiNr], ENABLE);
	return true;//spi_enable(McspiStruct);
}
/*#####################################################*/
void _mcspi_close(new_mcspi *McspiStruct)
{
	//spi_disable(McspiStruct);
	  GPIO_InitTypeDef GPIO_InitStructure;

	  /*!< Disable the sFLASH_SPI  ************************************************/
	  SPI_Cmd(_SPI_[McspiStruct->McspiNr], DISABLE);

	  /*!< DeInitializes the sFLASH_SPI *******************************************/
	  SPI_I2S_DeInit(_SPI_[McspiStruct->McspiNr]);

	  /*!< sFLASH_SPI Periph clock disable ****************************************/
		switch(McspiStruct->McspiNr) {
		case 0:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			RCC_APB1PeriphClockCmd(SPI_CLK[McspiStruct->McspiNr], DISABLE);
			break;
		default:
			return;
		}

	  /*!< Configure all pins used by the _SPI_ as input floating *******************/
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->SckPin;
	  GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->SckPort], &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->MisoPin;
	  GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->MisoPort], &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->MosiPin;
	  GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->MosiPort], &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = 1 << McspiStruct->Cs0Pin;
	  GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->Cs0Port], &GPIO_InitStructure);
}
/*#####################################################*/
unsigned char _mcspi_SendByte(Mcspi_t *McspiStruct, unsigned char byte)
{
  /*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(_SPI_[McspiStruct->McspiNr], SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(_SPI_[McspiStruct->McspiNr], byte);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(_SPI_[McspiStruct->McspiNr], SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the _SPI_ bus */
  return SPI_I2S_ReceiveData(_SPI_[McspiStruct->McspiNr]);
}
/*#####################################################*/
bool _mcspi_transfer(Mcspi_t *McspiStruct)
{
	/*McspiStruct->numOfBytes = NumOfBytesSend + NumOfBytesReceive;
	unsigned char response = SPI_MasterInterruptTransceivePacket(McspiStruct);
	//memcpy(McspiStruct->Buff, McspiStruct->Buff + NumOfBytesSend, NumOfBytesReceive);
	if(response) return false;*/
	if(!McspiStruct->DisableCsHandle)
		GPIO_ResetBits(GET_GPIO_PORT_ADDR[McspiStruct->Cs0Port], 1 << McspiStruct->Cs0Pin);

	unsigned int transfer_cnt = 0;
	for(; transfer_cnt < McspiStruct->numOfBytes; transfer_cnt++) {
		McspiStruct->Buff[transfer_cnt] = _mcspi_SendByte(McspiStruct, McspiStruct->Buff[transfer_cnt]);
	}

	if(!McspiStruct->DisableCsHandle)
		GPIO_SetBits(GET_GPIO_PORT_ADDR[McspiStruct->Cs0Port], 1 << McspiStruct->Cs0Pin);
	return true;
}
/*#####################################################*/
