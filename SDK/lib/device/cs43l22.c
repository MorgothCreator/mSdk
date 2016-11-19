/**
  ******************************************************************************
  * @file    cs43l22.c
  * @author  MCD Application Team
  * @version V2.0.2
  * @date    06-October-2015
  * @brief   This file provides the CS43L22 Audio Codec driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cs43l22.h"
#include "api/i2s_def.h"
#include "api/twi_def.h"
#include "api/twi_api.h"

/** @defgroup CS43L22_Private_Defines
  * @{
  */
#define VOLUME_CONVERT(Volume)    (((Volume) > 100)? 100:((unsigned char)(((Volume) * 255) / 100)))
/* Uncomment this line to enable verifying data sent to codec after each write
   operation (for debug purpose) */
#if !defined (VERIFY_WRITTENDATA)
/* #define VERIFY_WRITTENDATA */
#endif /* VERIFY_WRITTENDATA */
/**
  * @}
  */

/** @defgroup CS43L22_Private_Macros
  * @{
  */

/**
  * @}
  */
/** @defgroup CS43L22_Private_Variables
  * @{
  */

/* Audio codec driver structure initialization */
I2S_DeviceDriver cs43l22_drv =
{
  cs43l22_Init,
  cs43l22_DeInit,
  cs43l22_ReadID,

  cs43l22_Play,
  cs43l22_Pause,
  cs43l22_Resume,
  cs43l22_Stop,

  cs43l22_SetFrequency,
  cs43l22_SetVolume,
  cs43l22_SetMute,
  cs43l22_SetOutputMode,
  cs43l22_Reset
};



static unsigned char Is_cs43l22_Stop = 1;

volatile unsigned char OutputDev = 0;

/**
  * @}
  */

/** @defgroup CS43L22_Function_Prototypes
  * @{
  */
static unsigned char AUDIO_IO_Write(void *param, unsigned char Reg, unsigned char Value);
/**
  * @}
  */

/** @defgroup CS43L22_Private_Functions
  * @{
  */

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  Value: Data to be written
  */
unsigned char AUDIO_IO_Write(void *param, unsigned char Reg, unsigned char Value)
{
  //I2C2_WriteMultiple(Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Value, 1);
	cs43l22_t *_param = (cs43l22_t *)param;
	unsigned char buff[2];
	buff[0] = Reg;
	buff[1] = Value;
	twi.tx(_param->TWI, _param->TWI_Addr, buff, 2);
	return 0;
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @retval Data to be read
  */
unsigned char AUDIO_IO_Read(void *param, unsigned char Reg)
{
	cs43l22_t *_param = (cs43l22_t *)param;
	unsigned char read_value = 0;

  //I2C2_ReadMultiple(Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&read_value, 1);
	twi.trx(_param->TWI, _param->TWI_Addr, &Reg, 1, &read_value, 1);
	return read_value;
}
/**
  * @brief Initializes the audio codec and the control interface.
  * @param DeviceAddr: Device address on communication Bus.
  * @param OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_Init(void *param, unsigned short OutputDevice, unsigned char Volume, unsigned long AudioFreq)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;

  /* Initialize the Control interface of the Audio Codec */
  //AUDIO_IO_Init();

  /* Keep Codec powered OFF */
  counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL1, 0x01);

  /*Save Output device for mute ON/OFF procedure*/
  switch (OutputDevice)
  {
  case OUTPUT_DEVICE_SPEAKER:
    OutputDev = 0xFA;
    break;

  case OUTPUT_DEVICE_HEADPHONE:
    OutputDev = 0xAF;
    break;

  case OUTPUT_DEVICE_BOTH:
    OutputDev = 0xAA;
    break;

  case OUTPUT_DEVICE_AUTO:
    OutputDev = 0x05;
    break;

  default:
    OutputDev = 0x05;
    break;
  }

  counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, OutputDev);

  /* Clock configuration: Auto detection */
  counter += AUDIO_IO_Write(_param, CS43L22_REG_CLOCKING_CTL, 0x81);

  /* Set the Slave Mode and the audio Standard */
  counter += AUDIO_IO_Write(_param, CS43L22_REG_INTERFACE_CTL1, CODEC_STANDARD);

  /* Set the Master volume */
  counter += cs43l22_SetVolume(param, Volume);

  /* If the Speaker is enabled, set the Mono mode and volume attenuation level */
  if(OutputDevice != OUTPUT_DEVICE_HEADPHONE)
  {
    /* Set the Speaker Mono mode */
    counter += AUDIO_IO_Write(_param, CS43L22_REG_PLAYBACK_CTL2, 0x06);

    /* Set the Speaker attenuation level */
    counter += AUDIO_IO_Write(_param, CS43L22_REG_SPEAKER_A_VOL, 0x00);
    counter += AUDIO_IO_Write(_param, CS43L22_REG_SPEAKER_B_VOL, 0x00);
  }

  /* Additional configuration for the CODEC. These configurations are done to reduce
  the time needed for the Codec to power off. If these configurations are removed,
  then a long delay should be added between powering off the Codec and switching
  off the I2S peripheral MCLK clock (which is the operating clock for Codec).
  If this delay is not inserted, then the codec will not shut down properly and
  it results in high noise after shut down. */

  /* Disable the analog soft ramp */
  counter += AUDIO_IO_Write(param, CS43L22_REG_ANALOG_ZC_SR_SETT, 0x00);
  /* Disable the digital soft ramp */
  counter += AUDIO_IO_Write(param, CS43L22_REG_MISC_CTL, 0x04);
  /* Disable the limiter attack level */
  counter += AUDIO_IO_Write(param, CS43L22_REG_LIMIT_CTL1, 0x00);
  /* Adjust Bass and Treble levels */
  counter += AUDIO_IO_Write(param, CS43L22_REG_TONE_CTL, 0x0F);
  /* Adjust PCM volume level */
  counter += AUDIO_IO_Write(param, CS43L22_REG_PCMA_VOL, 0x0A);
  counter += AUDIO_IO_Write(param, CS43L22_REG_PCMB_VOL, 0x0A);

  /* Return communication control value */
  return counter;
}

/**
  * @brief  Deinitializes the audio codec.
  * @param  None
  * @retval  None
  */
void cs43l22_DeInit(void)
{
  /* Deinitialize Audio Codec interface */
  //AUDIO_IO_DeInit();
}

/**
  * @brief  Get the CS43L22 ID.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval The CS43L22 ID
  */
unsigned long cs43l22_ReadID(void *param)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned char Value;
  /* Initialize the Control interface of the Audio Codec */
  //AUDIO_IO_Init();

  Value = AUDIO_IO_Read(_param, CS43L22_CHIPID_ADDR);
  Value = (Value & CS43L22_ID_MASK);

  return((unsigned long) Value);
}

/**
  * @brief Start the audio Codec play feature.
  * @note For this codec no Play options are required.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_Play(void *param, unsigned short* pBuffer, unsigned short Size)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;

  if(Is_cs43l22_Stop == 1)
  {
    /* Enable the digital soft ramp */
    counter += AUDIO_IO_Write(_param, CS43L22_REG_MISC_CTL, 0x06);

    /* Enable Output device */
    counter += cs43l22_SetMute(param, AUDIO_MUTE_OFF);

    /* Power on the Codec */
    counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL1, 0x9E);
    Is_cs43l22_Stop = 0;
  }

  /* Return communication control value */
  return counter;
}

/**
  * @brief Pauses playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_Pause(void *param)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;

  /* Pause the audio file playing */
  /* Mute the output first */
  counter += cs43l22_SetMute(param, AUDIO_MUTE_ON);

  /* Put the Codec in Power save mode */
  counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL1, 0x01);

  return counter;
}

/**
  * @brief Resumes playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_Resume(void *param)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;
  volatile unsigned long index = 0x00;
  /* Resumes the audio file playing */
  /* Unmute the output first */
  counter += cs43l22_SetMute(param, AUDIO_MUTE_OFF);

  for(index = 0x00; index < 0xFF; index++);

  counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, OutputDev);

  /* Exit the Power save mode */
  counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL1, 0x9E);

  return counter;
}

/**
  * @brief Stops audio Codec playing. It powers down the codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @param CodecPdwnMode: selects the  power down mode.
  *          - CODEC_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration
  *                           (user should re-Initialize the codec in order to
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_Stop(void *param, unsigned long CodecPdwnMode)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;

  /* Mute the output first */
  counter += cs43l22_SetMute(param, AUDIO_MUTE_ON);

  /* Disable the digital soft ramp */
  counter += AUDIO_IO_Write(_param, CS43L22_REG_MISC_CTL, 0x04);

  /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
  counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL1, 0x9F);

  Is_cs43l22_Stop = 1;
  return counter;
}

/**
  * @brief Sets higher or lower the codec volume level.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Volume: a byte value from 0 to 255 (refer to codec registers
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_SetVolume(void *param, unsigned char Volume)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;
  unsigned char convertedvol = VOLUME_CONVERT(Volume);

  if(Volume > 0xE6)
  {
    /* Set the Master volume */
    counter += AUDIO_IO_Write(_param, CS43L22_REG_MASTER_A_VOL, convertedvol - 0xE7);
    counter += AUDIO_IO_Write(_param, CS43L22_REG_MASTER_B_VOL, convertedvol - 0xE7);
  }
  else
  {
    /* Set the Master volume */
    counter += AUDIO_IO_Write(_param, CS43L22_REG_MASTER_A_VOL, convertedvol + 0x19);
    counter += AUDIO_IO_Write(_param, CS43L22_REG_MASTER_B_VOL, convertedvol + 0x19);
  }

  return counter;
}

/**
  * @brief Sets new frequency.
  * @param DeviceAddr: Device address on communication Bus.
  * @param AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_SetFrequency(void *param, unsigned long AudioFreq)
{
  return 0;
}

/**
  * @brief Enables or disables the mute feature on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_SetMute(void *param, unsigned long Cmd)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;

  /* Set the Mute mode */
  if(Cmd == AUDIO_MUTE_ON)
  {
    counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, 0xFF);
    counter += AUDIO_IO_Write(_param, CS43L22_REG_HEADPHONE_A_VOL, 0x01);
    counter += AUDIO_IO_Write(_param, CS43L22_REG_HEADPHONE_B_VOL, 0x01);
  }
  else /* AUDIO_MUTE_OFF Disable the Mute */
  {
    counter += AUDIO_IO_Write(_param, CS43L22_REG_HEADPHONE_A_VOL, 0x00);
    counter += AUDIO_IO_Write(_param, CS43L22_REG_HEADPHONE_B_VOL, 0x00);
    counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, OutputDev);
  }
  return counter;
}

/**
  * @brief Switch dynamically (while audio file is played) the output target
  *         (speaker or headphone).
  * @note This function modifies a global variable of the audio codec driver: OutputDev.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Output: specifies the audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_SetOutputMode(void *param, unsigned char Output)
{
	cs43l22_t *_param = (cs43l22_t *)param;
  unsigned long counter = 0;

  switch (Output)
  {
    case OUTPUT_DEVICE_SPEAKER:
      counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, 0xFA); /* SPK always ON & HP always OFF */
      OutputDev = 0xFA;
      break;

    case OUTPUT_DEVICE_HEADPHONE:
      counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, 0xAF); /* SPK always OFF & HP always ON */
      OutputDev = 0xAF;
      break;

    case OUTPUT_DEVICE_BOTH:
      counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, 0xAA); /* SPK always ON & HP always ON */
      OutputDev = 0xAA;
      break;

    case OUTPUT_DEVICE_AUTO:
      counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;

    default:
      counter += AUDIO_IO_Write(_param, CS43L22_REG_POWER_CTL2, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;
  }
  return counter;
}

/**
  * @brief Resets cs43l22 registers.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
unsigned long cs43l22_Reset(void *param)
{
  return 0;
}

/**
  * @brief  Writes/Read a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  Value: Data to be written
  * @retval None
  */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

