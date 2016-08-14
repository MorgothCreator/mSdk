/*
 *  lib/api/adc_api.c
 *
 *  Copyright (C) 2014  Iulian Gheorghiu <morgoth.creator@gmail.com>
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

#include "stdbool.h"
#include "stdio.h"
#include "include/stm32f4xx.h"
#include "api/adc_def.h"
#include "board_init.h"
#include "adc_interface.h"
#include "driver/stm32f4xx_hal_adc.h"
#include "driver/stm32f4xx_hal_dma.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_cortex.h"
/*#####################################################*/
ADC_HandleTypeDef    AdcHandle[3];
DMA_HandleTypeDef DMA_InitStructure[3];
volatile unsigned int channel_converted = 0;

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	volatile unsigned int adc_nr = 0;
	switch((unsigned int)AdcHandle->Instance)
	{
		case((unsigned int)ADC1):
			adc_nr = 0;
			break;
		case((unsigned int)ADC2):
			adc_nr = 1;
			break;
		case((unsigned int)ADC3):
			adc_nr = 2;
			break;
	}
  /* Get the converted value of regular channel */
	_ADC[adc_nr]->ConvResult[_ADC[adc_nr]->ChannelCount] = HAL_ADC_GetValue(AdcHandle);
	_ADC[adc_nr]->ChannelCount++;
	if(_ADC[adc_nr]->ChannelCount >= _ADC[adc_nr]->EnabledChannelsNr)
		_ADC[adc_nr]->EndOfConversion = true;
}


/*#####################################################*/
/*void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	volatile unsigned int adc_nr = 0;
	switch((unsigned int)hadc->Instance)
	{
		case((unsigned int)ADC1):
			adc_nr = 0;
			break;
		case((unsigned int)ADC2):
			adc_nr = 1;
			break;
		case((unsigned int)ADC3):
			adc_nr = 2;
			break;
	}
	//if(	_ADC[adc_nr]->ContinuousMode == ADC_CONTINUOUS_MODE_CONTINUOUS)
	//{
		unsigned int ChCnt = 0;
		unsigned int ChCntEnChannels = 0;
		for(; ChCnt < 18; ChCnt++)
		{
			if(_ADC[adc_nr]->EnCh[ChCnt])
				ChCntEnChannels++;
		}

		//DMA_DeInit(DMA_Stream);
		//HAL_ADC_Init(&AdcHandle[adc_nr]);
		//HAL_DMA_Init(&DMA_InitStructure[adc_nr]);


	    //hadc->Instance->CR2 |= ADC_CR2_SWSTART;
		//HAL_ADC_Start_DMA(hadc, (uint32_t*)&_ADC[adc_nr]->ConvResult[0],ChCntEnChannels);
	//}
}*/
/*#####################################################*/
bool _adc_start_conversion(Adc_t *Adc_s)
{
	unsigned int ChCnt = 0;
	unsigned int ChCntEnChannels = 0;
	for(; ChCnt < 18; ChCnt++)
	{
		if(Adc_s->EnCh[ChCnt])
			ChCntEnChannels++;
	}
	Adc_s->ChannelCount = 0;
	Adc_s->EndOfConversion = false;
	if(HAL_ADC_Start_IT(&AdcHandle[Adc_s->UnitNr]) && ChCntEnChannels != 0)
		return true;
	else
		return false;
}
/*#####################################################*/
bool _adc_init(Adc_t *Adc_s)
{
	bool return_status = true;
//#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
	/* ADCCLK = PCLK2/2 */
//	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
//#else
	/* ADCCLK = PCLK2/4 */
//	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
//#endif
	/* Enable peripheral clocks ------------------------------------------------*/
	/* Enable DMA1 clock */
	if(Adc_s->DmaUnitNr == 0)
		__HAL_RCC_DMA1_CLK_ENABLE();
	else
		__HAL_RCC_DMA2_CLK_ENABLE();


	switch(Adc_s->UnitNr)
	{
	case 0:
		Adc_s->BaseAddr = ADC1;
		__HAL_RCC_ADC1_CLK_ENABLE();
		break;
	case 1:
		Adc_s->BaseAddr = ADC2;
		__HAL_RCC_ADC2_CLK_ENABLE();
		break;
	case 2:
		Adc_s->BaseAddr = ADC3;
		__HAL_RCC_ADC3_CLK_ENABLE();
		break;
	default:
		return false;
	}
	//ADC_InitTypeDef ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	unsigned int ext_trig = ADC_SOFTWARE_START;
	switch(Adc_s->ExtTrig)
	{
		case 0:
			break;
		case 1:
			ext_trig = ADC_EXTERNALTRIGCONV_T1_CC1;
			break;
		case 2:
			ext_trig = ADC_EXTERNALTRIGCONV_T1_CC2;
			break;
		case 3:
			ext_trig = ADC_EXTERNALTRIGCONV_T1_CC3;
			break;
		case 4:
			ext_trig = ADC_EXTERNALTRIGCONV_T2_CC2;
			break;
		case 5:
			ext_trig = ADC_EXTERNALTRIGCONV_T2_CC3;
			break;
		case 6:
			ext_trig = ADC_EXTERNALTRIGCONV_T2_CC4;
			break;
		case 7:
			ext_trig = ADC_EXTERNALTRIGCONV_T2_TRGO;
			break;
		case 8:
			ext_trig = ADC_EXTERNALTRIGCONV_T3_CC1;
			break;
		case 9:
			ext_trig = ADC_EXTERNALTRIGCONV_T3_TRGO;
			break;
		case 10:
			ext_trig = ADC_EXTERNALTRIGCONV_T4_CC4;
			break;
		case 11:
			ext_trig = ADC_EXTERNALTRIGCONV_T5_CC1;
			break;
		case 12:
			ext_trig = ADC_EXTERNALTRIGCONV_T5_CC2;
			break;
		case 13:
			ext_trig = ADC_EXTERNALTRIGCONV_T5_CC3;
			break;
		case 14:
			ext_trig = ADC_EXTERNALTRIGCONV_T8_CC1;
			break;
		case 15:
			ext_trig = ADC_EXTERNALTRIGCONV_T8_TRGO;
			break;
		case 16:
			ext_trig = ADC_EXTERNALTRIGCONV_Ext_IT11;
			break;
	}

	/* ADC enable channels by mask */
	unsigned char ChCnt = 0;
	unsigned int RankCnt = 1;

	/* Enable GPIO clock */
	for(; ChCnt < 18; ChCnt++) {
		if(Adc_s->EnCh[ChCnt]) {
			switch(Adc_s->EnCh[ChCnt]) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				__HAL_RCC_GPIOA_CLK_ENABLE();
				break;
			case 8:
			case 9:
				__HAL_RCC_GPIOB_CLK_ENABLE();
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				__HAL_RCC_GPIOC_CLK_ENABLE();
				break;
			case 16:
			case 17:
			case 18:
				break;
			default:
				return_status = false;
				break;
			}
		}
	}
	if(!return_status) {
		switch(Adc_s->UnitNr)
		{
		case 0:
			Adc_s->BaseAddr = NULL;
			__HAL_RCC_ADC1_CLK_DISABLE();
			return false;
		case 1:
			Adc_s->BaseAddr = NULL;
			__HAL_RCC_ADC2_CLK_DISABLE();
			return false;
		case 2:
			Adc_s->BaseAddr = NULL;
			__HAL_RCC_ADC3_CLK_DISABLE();
			return false;
		default:
			return false;
		}
	}
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	AdcHandle[Adc_s->UnitNr].Instance = Adc_s->BaseAddr;
	//ADC_CommonInitStructure = Adc_s->Mode;//ADC_Mode_Independent;
	switch(Adc_s->Prescaller)
	{
	case 2:
		AdcHandle[Adc_s->UnitNr].Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
		break;
	case 4:
		AdcHandle[Adc_s->UnitNr].Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
		break;
	case 6:
		AdcHandle[Adc_s->UnitNr].Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV6;
		break;
	case 8:
		AdcHandle[Adc_s->UnitNr].Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
		break;
	default:
		AdcHandle[Adc_s->UnitNr].Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	}
	//ADC_CommonInitStructure.Init. = ADC_TwoSamplingDelay_5Cycles;
	//ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	//ADC_CommonInit(&ADC_CommonInitStructure);

	switch(Adc_s->ResolutionBits)
	{
	case 6:
		AdcHandle[Adc_s->UnitNr].Init.Resolution = ADC_RESOLUTION_6B;
		break;
	case 8:
		AdcHandle[Adc_s->UnitNr].Init.Resolution = ADC_RESOLUTION_8B;
		break;
	case 10:
		AdcHandle[Adc_s->UnitNr].Init.Resolution = ADC_RESOLUTION_10B;
		break;
	case 12:
		AdcHandle[Adc_s->UnitNr].Init.Resolution = ADC_RESOLUTION_12B;
		break;
	default:
		AdcHandle[Adc_s->UnitNr].Init.Resolution = ADC_RESOLUTION_12B;
		break;
	}
	AdcHandle[Adc_s->UnitNr].Init.ScanConvMode = Adc_s->ConvMode;//ENABLE;
	AdcHandle[Adc_s->UnitNr].Init.ContinuousConvMode = Adc_s->ContinuousMode;//ENABLE;
	AdcHandle[Adc_s->UnitNr].Init.ExternalTrigConv = ext_trig;//ADC_ExternalTrigConv_None;
	AdcHandle[Adc_s->UnitNr].Init.ExternalTrigConvEdge = (Adc_s->ExtTrigEdge << 26) & ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING;
	AdcHandle[Adc_s->UnitNr].Init.DataAlign = Adc_s->Align;//ADC_DataAlign_Right;
	AdcHandle[Adc_s->UnitNr].Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	/*if(Adc_s->IntRefEn || Adc_s->TempSensorEn)
		ADC_TempSensorVrefintCmd(ENABLE);
	else
		ADC_TempSensorVrefintCmd(DISABLE);

	if(Adc_s->VBatEn)
		ADC_VBATCmd(ENABLE);
	else
		ADC_VBATCmd(DISABLE);*/

	ChCnt = 0;
	for(; ChCnt < 18; ChCnt++)
	{
		if(Adc_s->EnCh[ChCnt]) {
			switch(Adc_s->EnCh[ChCnt] - 1)
			{
			case 0:
				GPIO_InitStructure.Pin = GPIO_PIN_0;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 1:
				GPIO_InitStructure.Pin = GPIO_PIN_1;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 2:
				GPIO_InitStructure.Pin = GPIO_PIN_2;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 3:
				GPIO_InitStructure.Pin = GPIO_PIN_3;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 4:
				GPIO_InitStructure.Pin = GPIO_PIN_4;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 5:
				GPIO_InitStructure.Pin = GPIO_PIN_5;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 6:
				GPIO_InitStructure.Pin = GPIO_PIN_6;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 7:
				GPIO_InitStructure.Pin = GPIO_PIN_7;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 8:
				GPIO_InitStructure.Pin = GPIO_PIN_0;
				HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			case 9:
				GPIO_InitStructure.Pin = GPIO_PIN_1;
				HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			case 10:
				GPIO_InitStructure.Pin = GPIO_PIN_0;
				HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 11:
				GPIO_InitStructure.Pin = GPIO_PIN_1;
				HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 12:
				GPIO_InitStructure.Pin = GPIO_PIN_2;
				HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 13:
				GPIO_InitStructure.Pin = GPIO_PIN_3;
				HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 14:
				GPIO_InitStructure.Pin = GPIO_PIN_4;
				HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 15:
				GPIO_InitStructure.Pin = GPIO_PIN_5;
				HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 16:
			case 17:
			case 18:
				break;
			}
			ADC_ChannelConfTypeDef sConfig;
			sConfig.Channel = Adc_s->EnCh[ChCnt];
			sConfig.Rank = RankCnt;
			sConfig.SamplingTime = Adc_s->SampleTime[ChCnt];

			HAL_ADC_ConfigChannel(&AdcHandle[Adc_s->UnitNr], &sConfig);
			//HAL_ADC_RegularChannelConfig((ADC_TypeDef *)Adc_s->BaseAddr, Adc_s->EnCh[ChCnt] - 1, RankCnt, Adc_s->SampleTime[ChCnt]);
			RankCnt++;
		}
	}
	AdcHandle[Adc_s->UnitNr].Init.NbrOfConversion = RankCnt - 1;//1;
	HAL_ADC_Init(&AdcHandle[Adc_s->UnitNr]);


	DMA_Stream_TypeDef *DMA_Stream;
	//int DMA_Type;
	switch(Adc_s->DmaUnitNr)
	{
	case 0:
		switch(Adc_s->DmaChannel)
		{
		case 0:
			DMA_Stream = DMA1_Stream0;
			//DMA_Type = DMA1_Stream0_IRQn;
			break;
		case 1:
			DMA_Stream = DMA1_Stream1;
			//DMA_Type = DMA1_Stream1_IRQn;
			break;
		case 2:
			DMA_Stream = DMA1_Stream2;
			//DMA_Type = DMA1_Stream2_IRQn;
			break;
		case 3:
			DMA_Stream = DMA1_Stream3;
			//DMA_Type = DMA1_Stream3_IRQn;
			break;
		case 4:
			DMA_Stream = DMA1_Stream4;
			//DMA_Type = DMA1_Stream4_IRQn;
			break;
		case 5:
			DMA_Stream = DMA1_Stream5;
			//DMA_Type = DMA1_Stream5_IRQn;
			break;
		case 6:
			DMA_Stream = DMA1_Stream6;
			//DMA_Type = DMA1_Stream6_IRQn;
			break;
		case 7:
			DMA_Stream = DMA1_Stream7;
			//DMA_Type = DMA1_Stream7_IRQn;
			break;
		default:
			return false;
		}
		break;
	case 1:
		switch(Adc_s->DmaChannel)
		{
		case 0:
			DMA_Stream = DMA2_Stream0;
			//DMA_Type = DMA2_Stream0_IRQn;
			break;
		case 1:
			DMA_Stream = DMA2_Stream1;
			//DMA_Type = DMA2_Stream1_IRQn;
			break;
		case 2:
			DMA_Stream = DMA2_Stream2;
			//DMA_Type = DMA2_Stream2_IRQn;
			break;
		case 3:
			DMA_Stream = DMA2_Stream3;
			//DMA_Type = DMA2_Stream3_IRQn;
			break;
		case 4:
			DMA_Stream = DMA2_Stream4;
			//DMA_Type = DMA2_Stream4_IRQn;
			break;
		case 5:
			DMA_Stream = DMA2_Stream5;
			//DMA_Type = DMA2_Stream5_IRQn;
			break;
		case 6:
			DMA_Stream = DMA2_Stream6;
			//DMA_Type = DMA2_Stream6_IRQn;
			break;
		case 7:
			DMA_Stream = DMA2_Stream7;
			//DMA_Type = DMA2_Stream7_IRQn;
			break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	/* DMA1 channel1 configuration ----------------------------------------------*/
	/*DMA_InitStructure[Adc_s->UnitNr].Instance = DMA_Stream;
	DMA_InitStructure[Adc_s->UnitNr].Init.Channel = (Adc_s->DmaChannel << 25) & 0x0E000000;
	DMA_InitStructure[Adc_s->UnitNr].Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_InitStructure[Adc_s->UnitNr].Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_InitStructure[Adc_s->UnitNr].Init.MemInc = DMA_MINC_ENABLE;
	DMA_InitStructure[Adc_s->UnitNr].Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	DMA_InitStructure[Adc_s->UnitNr].Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	DMA_InitStructure[Adc_s->UnitNr].Init.Mode = DMA_CIRCULAR;
	DMA_InitStructure[Adc_s->UnitNr].Init.Priority = DMA_PRIORITY_HIGH;
	DMA_InitStructure[Adc_s->UnitNr].Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	DMA_InitStructure[Adc_s->UnitNr].Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	DMA_InitStructure[Adc_s->UnitNr].Init.MemBurst = DMA_MBURST_SINGLE;
	DMA_InitStructure[Adc_s->UnitNr].Init.PeriphBurst = DMA_PBURST_SINGLE;
	HAL_DMA_Init(&DMA_InitStructure[Adc_s->UnitNr]);*/

	/* Associate the initialized DMA handle to the the ADC handle */
	//__HAL_LINKDMA(&AdcHandle[Adc_s->UnitNr], DMA_Handle, DMA_InitStructure[Adc_s->UnitNr]);

	/*##-4- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	//HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);


	/* Enable ADC1 DMA */
	//ADC_DMACmd((ADC_TypeDef *)Adc_s->BaseAddr, ENABLE);

	/* Enable ADC */
	//ADC_Cmd((ADC_TypeDef *)Adc_s->BaseAddr, ENABLE);



	/* Enable ADC reset calibration register */
	//ADC_ResetCalibration((ADC_TypeDef *)Adc_s->BaseAddr);
	/* Check the end of ADC1 reset calibration register */
	//while(ADC_GetResetCalibrationStatus((ADC_TypeDef *)Adc_s->BaseAddr));

	/* Start ADC calibration */
	//ADC_StartCalibration((ADC_TypeDef *)Adc_s->BaseAddr);
	/* Check the end of ADC calibration */
	//while(ADC_GetCalibrationStatus((ADC_TypeDef *)Adc_s->BaseAddr));

	/* Start ADC Software Conversion */
	//ADC_SoftwareStartConv((ADC_TypeDef *)Adc_s->BaseAddr);
	//HAL_ADC_Start(&AdcHandle);
	//HAL_ADC_Start_DMA(&AdcHandle[Adc_s->UnitNr], (uint32_t*)&Adc_s->ConvResult[0], RankCnt - 1);

	  HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(ADC_IRQn);

	return true;
}
