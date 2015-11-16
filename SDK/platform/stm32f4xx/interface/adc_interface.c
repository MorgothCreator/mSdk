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
#include "api/adc_def.h"
#include "adc_interface.h"
#include "driver/stm32f4xx_hal_adc.h"
#include "driver/stm32f4xx_hal_dma.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_gpio.h"
/*#####################################################*/
bool _adc_init(Adc_t *Adc_s)
{
#if 0
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
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	else
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);


	switch(Adc_s->UnitNr)
	{
	case 0:
		Adc_s->BaseAddr = ADC1;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		break;
	case 1:
		Adc_s->BaseAddr = ADC2;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
		break;
	case 2:
		Adc_s->BaseAddr = ADC3;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		break;
	default:
		return false;
	}
	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	unsigned int ext_trig = ADC_ExternalTrigConv_T1_CC1;
	switch(Adc_s->ExtTrig)
	{
		case 0:
			break;
		case 1:
			ext_trig = ADC_ExternalTrigConv_T1_CC1;
			break;
		case 2:
			ext_trig = ADC_ExternalTrigConv_T1_CC2;
			break;
		case 3:
			ext_trig = ADC_ExternalTrigConv_T1_CC3;
			break;
		case 4:
			ext_trig = ADC_ExternalTrigConv_T2_CC2;
			break;
		case 5:
			ext_trig = ADC_ExternalTrigConv_T2_CC3;
			break;
		case 6:
			ext_trig = ADC_ExternalTrigConv_T2_CC4;
			break;
		case 7:
			ext_trig = ADC_ExternalTrigConv_T2_TRGO;
			break;
		case 8:
			ext_trig = ADC_ExternalTrigConv_T3_CC1;
			break;
		case 9:
			ext_trig = ADC_ExternalTrigConv_T3_TRGO;
			break;
		case 10:
			ext_trig = ADC_ExternalTrigConv_T4_CC4;
			break;
		case 11:
			ext_trig = ADC_ExternalTrigConv_T5_CC1;
			break;
		case 12:
			ext_trig = ADC_ExternalTrigConv_T5_CC2;
			break;
		case 13:
			ext_trig = ADC_ExternalTrigConv_T5_CC3;
			break;
		case 14:
			ext_trig = ADC_ExternalTrigConv_T8_CC1;
			break;
		case 15:
			ext_trig = ADC_ExternalTrigConv_T8_TRGO;
			break;
		case 16:
			ext_trig = ADC_ExternalTrigConv_Ext_IT11;
			break;
	}

	/* ADC enable channels by mask */
	unsigned char ChCnt = 0;
	unsigned char RankCnt = 1;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	unsigned long enabled_gpio_clk = 0;
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
				enabled_gpio_clk |= RCC_AHB1Periph_GPIOA;
				break;
			case 8:
			case 9:
				enabled_gpio_clk |= RCC_AHB1Periph_GPIOB;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				enabled_gpio_clk |= RCC_AHB1Periph_GPIOC;
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
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
			return false;
		case 1:
			Adc_s->BaseAddr = NULL;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, DISABLE);
			return false;
		case 2:
			Adc_s->BaseAddr = NULL;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, DISABLE);
			return false;
		default:
			return false;
		}

	}
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(enabled_gpio_clk, ENABLE);
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = Adc_s->Mode;//ADC_Mode_Independent;
	switch(Adc_s->Prescaller)
	{
	case 2:
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
		break;
	case 4:
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		break;
	case 6:
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
		break;
	case 8:
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
		break;
	default:
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	}
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);

	switch(Adc_s->ResolutionBits)
	{
	case 6:
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_6b;
		break;
	case 8:
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
		break;
	case 10:
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
		break;
	case 12:
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		break;
	default:
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		break;
	}
	ADC_InitStructure.ADC_ScanConvMode = Adc_s->ConvMode;//ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = Adc_s->ContinuousMode;//ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ext_trig;//ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = (Adc_s->ExtTrigEdge << 26) & ADC_ExternalTrigConvEdge_RisingFalling;
	ADC_InitStructure.ADC_DataAlign = Adc_s->Align;//ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = RankCnt - 1;//1;
	ADC_Init((ADC_TypeDef *)Adc_s->BaseAddr, &ADC_InitStructure);

	if(Adc_s->IntRefEn || Adc_s->TempSensorEn)
		ADC_TempSensorVrefintCmd(ENABLE);
	else
		ADC_TempSensorVrefintCmd(DISABLE);

	if(Adc_s->VBatEn)
		ADC_VBATCmd(ENABLE);
	else
		ADC_VBATCmd(DISABLE);

	ChCnt = 0;
	for(; ChCnt < 18; ChCnt++)
	{
		if(Adc_s->EnCh[ChCnt]) {
			switch(Adc_s->EnCh[ChCnt] - 1)
			{
			case 0:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 1:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 2:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 3:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 4:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 5:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 6:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 7:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			case 8:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			case 9:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			case 10:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 11:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 12:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 13:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 14:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 15:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				break;
			case 16:
			case 17:
			case 18:
				break;
			}
			ADC_RegularChannelConfig((ADC_TypeDef *)Adc_s->BaseAddr, Adc_s->EnCh[ChCnt] - 1, RankCnt, Adc_s->SampleTime[ChCnt]);
			RankCnt++;
		}
	}

	DMA_InitTypeDef DMA_InitStructure;
	ADC_TypeDef *ADC_Base = (ADC_TypeDef *)Adc_s->BaseAddr;

	DMA_Stream_TypeDef *DMA_Channel;
	switch(Adc_s->DmaUnitNr)
	{
	case 0:
		switch(Adc_s->DmaChannel)
		{
		case 0:
			DMA_Channel = DMA1_Stream0;
			break;
		case 1:
			DMA_Channel = DMA1_Stream1;
			break;
		case 2:
			DMA_Channel = DMA1_Stream2;
			break;
		case 3:
			DMA_Channel = DMA1_Stream3;
			break;
		case 4:
			DMA_Channel = DMA1_Stream4;
			break;
		case 5:
			DMA_Channel = DMA1_Stream5;
			break;
		case 6:
			DMA_Channel = DMA1_Stream6;
			break;
		case 7:
			DMA_Channel = DMA1_Stream7;
			break;
		default:
			return false;
		}
		break;
	case 1:
		switch(Adc_s->DmaChannel)
		{
		case 0:
			DMA_Channel = DMA2_Stream0;
			break;
		case 1:
			DMA_Channel = DMA2_Stream1;
			break;
		case 2:
			DMA_Channel = DMA2_Stream2;
			break;
		case 3:
			DMA_Channel = DMA2_Stream3;
			break;
		case 4:
			DMA_Channel = DMA2_Stream4;
			break;
		case 5:
			DMA_Channel = DMA2_Stream5;
			break;
		case 6:
			DMA_Channel = DMA2_Stream6;
			break;
		case 7:
			DMA_Channel = DMA2_Stream7;
			break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA_Channel);
	DMA_InitStructure.DMA_Channel = (Adc_s->DmaChannel << 25) & 0x0E000000;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)&ADC_Base->DR/*0x4001244C*/);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Adc_s->ConvResult[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = RankCnt - 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_Channel, &DMA_InitStructure);
	DMA_Cmd(DMA_Channel, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd((ADC_TypeDef *)Adc_s->BaseAddr, ENABLE);

	/* Enable ADC */
	ADC_Cmd((ADC_TypeDef *)Adc_s->BaseAddr, ENABLE);

	/* Enable ADC reset calibration register */
	//ADC_ResetCalibration((ADC_TypeDef *)Adc_s->BaseAddr);
	/* Check the end of ADC1 reset calibration register */
	//while(ADC_GetResetCalibrationStatus((ADC_TypeDef *)Adc_s->BaseAddr));

	/* Start ADC calibration */
	//ADC_StartCalibration((ADC_TypeDef *)Adc_s->BaseAddr);
	/* Check the end of ADC calibration */
	//while(ADC_GetCalibrationStatus((ADC_TypeDef *)Adc_s->BaseAddr));

	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv((ADC_TypeDef *)Adc_s->BaseAddr);
#endif
	return false;
}
/*#####################################################*/
