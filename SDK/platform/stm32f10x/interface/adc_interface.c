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
#include "driver/stm32f10x_adc.h"
#include "driver/stm32f10x_dma.h"
#include "driver/stm32f10x_rcc.h"
#include "driver/stm32f10x_gpio.h"
/*#####################################################*/
bool _adc_init(Adc_t *Adc_s)
{
	bool return_status = true;
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
	/* ADCCLK = PCLK2/2 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
#else
	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
#endif
	/* Enable peripheral clocks ------------------------------------------------*/
	/* Enable DMA1 clock */
	if(Adc_s->DmaUnitNr == 0)
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	else
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);


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
	unsigned int ext_trig = ADC_ExternalTrigConv_None;
	if(Adc_s->UnitNr == 0 || Adc_s->UnitNr == 1) {
		switch(Adc_s->ExtTrig)
		{
			case 1:
				ext_trig = ADC_ExternalTrigConv_T1_CC1;
				break;
			case 2:
				ext_trig = ADC_ExternalTrigConv_T1_CC2;
				break;
			case 3:
				ext_trig = ADC_ExternalTrigConv_T2_CC2;
				break;
			case 4:
				ext_trig = ADC_ExternalTrigConv_T3_TRGO;
				break;
			case 5:
				ext_trig = ADC_ExternalTrigConv_T4_CC4;
				break;
			case 6:
				ext_trig = ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO;
				break;
			default:
				break;
		}
	}
	else if(Adc_s->UnitNr == 2) {
		switch(Adc_s->ExtTrig)
		{
			case 1:
				ext_trig = ADC_ExternalTrigConv_T3_CC1;
				break;
			case 2:
				ext_trig = ADC_ExternalTrigConv_T2_CC3;
				break;
			case 3:
				ext_trig = ADC_ExternalTrigConv_T8_CC1;
				break;
			case 4:
				ext_trig = ADC_ExternalTrigConv_T8_TRGO;
				break;
			case 5:
				ext_trig = ADC_ExternalTrigConv_T5_CC1;
				break;
			case 6:
				ext_trig = ADC_ExternalTrigConv_T5_CC3;
				break;
			default:
				break;
		}
	}
	switch(Adc_s->ExtTrig)
	{
		case 0:
			ext_trig = ADC_ExternalTrigConv_None;
			break;
		case 7:
			ext_trig = ADC_ExternalTrigConv_T1_CC3;
			break;
		default:
			break;
	}

	/* ADC enable channels by mask */
	unsigned char ChCnt = 0;
	unsigned char RankCnt = 1;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
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
				enabled_gpio_clk |= RCC_APB2Periph_GPIOA;
				break;
			case 8:
			case 9:
				enabled_gpio_clk |= RCC_APB2Periph_GPIOB;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				enabled_gpio_clk |= RCC_APB2Periph_GPIOC;
				break;
			case 16:
			case 17:
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
	RCC_APB2PeriphClockCmd(enabled_gpio_clk, ENABLE);
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
				break;
			}
			ADC_RegularChannelConfig((ADC_TypeDef *)Adc_s->BaseAddr, Adc_s->EnCh[ChCnt] - 1, RankCnt, Adc_s->SampleTime[ChCnt]);
			RankCnt++;
		}
	}

	ADC_InitStructure.ADC_Mode = Adc_s->Mode;//ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = Adc_s->ConvMode;//ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = Adc_s->ContinuousMode;//ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ext_trig;//ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = Adc_s->Align;//ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = RankCnt - 1;//1;
	ADC_Init((ADC_TypeDef *)Adc_s->BaseAddr, &ADC_InitStructure);

	if(Adc_s->IntRefEn || Adc_s->TempSensorEn)
		ADC_TempSensorVrefintCmd(ENABLE);
	else
		ADC_TempSensorVrefintCmd(DISABLE);

	DMA_InitTypeDef DMA_InitStructure;
	ADC_TypeDef *ADC_Base = (ADC_TypeDef *)Adc_s->BaseAddr;

	DMA_Channel_TypeDef *DMA_Channel;
	switch(Adc_s->DmaUnitNr)
	{
	case 0:
		switch(Adc_s->DmaChannel)
		{
		case 0:
			DMA_Channel = DMA1_Channel1;
			break;
		case 1:
			DMA_Channel = DMA1_Channel2;
			break;
		case 2:
			DMA_Channel = DMA1_Channel3;
			break;
		case 3:
			DMA_Channel = DMA1_Channel4;
			break;
		case 4:
			DMA_Channel = DMA1_Channel5;
			break;
		case 5:
			DMA_Channel = DMA1_Channel6;
			break;
		case 6:
			DMA_Channel = DMA1_Channel7;
			break;
		default:
			return false;
		}
		break;
	case 1:
		switch(Adc_s->DmaChannel)
		{
		case 0:
			DMA_Channel = DMA2_Channel1;
			break;
		case 1:
			DMA_Channel = DMA2_Channel2;
			break;
		case 2:
			DMA_Channel = DMA2_Channel3;
			break;
		case 3:
			DMA_Channel = DMA2_Channel4;
			break;
		case 4:
			DMA_Channel = DMA2_Channel5;
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)&ADC_Base->DR/*0x4001244C*/);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Adc_s->ConvResult[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = RankCnt - 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_Channel, &DMA_InitStructure);
	DMA_Cmd(DMA_Channel, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd((ADC_TypeDef *)Adc_s->BaseAddr, ENABLE);

	/* Enable ADC */
	ADC_Cmd((ADC_TypeDef *)Adc_s->BaseAddr, ENABLE);

	/* Enable ADC reset calibration register */
	ADC_ResetCalibration((ADC_TypeDef *)Adc_s->BaseAddr);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus((ADC_TypeDef *)Adc_s->BaseAddr));

	/* Start ADC calibration */
	ADC_StartCalibration((ADC_TypeDef *)Adc_s->BaseAddr);
	/* Check the end of ADC calibration */
	while(ADC_GetCalibrationStatus((ADC_TypeDef *)Adc_s->BaseAddr));

	/* Start ADC Software Conversion */
	ADC_SoftwareStartConvCmd((ADC_TypeDef *)Adc_s->BaseAddr, ENABLE);

	return true;
}
/*#####################################################*/
