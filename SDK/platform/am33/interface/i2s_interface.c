/**
  ******************************************************************************
  * @file    stm32469i_discovery_audio.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-September-2015
  * @brief   This file provides the Audio driver for the STM32469I-Discovery board.
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

/*==============================================================================
                                 User NOTES

How To use this driver:
-----------------------
   + This driver supports STM32F4xx devices on STM32469I-Discovery (MB1189) Discovery boards.
   + Call the function BSP_AUDIO_OUT_Init(
                                    OutputDevice: physical output mode (OUTPUT_DEVICE_HEADPHONE1,
                                                  OUTPUT_DEVICE_HEADPHONE2 or OUTPUT_DEVICE_BOTH)
                                    Volume      : Initial volume to be set (0 is min (mute), 100 is max (100%)
                                    AudioFreq   : Audio frequency in Hz (8000, 16000, 22500, 32000...)
                                                  this parameter is relative to the audio file/stream type.
                                   )
      This function configures all the hardware required for the audio application (codec, I2C, SAI,
      GPIOs, DMA and interrupt if needed). This function returns AUDIO_OK if configuration is OK.
      If the returned value is different from AUDIO_OK or the function is stuck then the communication with
      the codec or the MFX has failed (try to un-plug the power or reset device in this case).
      - OUTPUT_DEVICE_HEADPHONE1 : only headphones 1 will be set as output for the audio stream.
      - OUTPUT_DEVICE_HEADPHONE2 : only headphones 2 will be set as output for the audio stream.
      - OUTPUT_DEVICE_BOTH       : both Headphones are used as outputs for the audio stream
                                 at the same time.
      Note. On STM32469I-Discovery SAI_DMA is configured in CIRCULAR mode. Due to this the application
        does NOT need to call BSP_AUDIO_OUT_ChangeBuffer() to assure straming.
   + Call the function BSP_Discovery_AUDIO_OUT_Play(
                                  pBuffer: pointer to the audio data file address
                                  Size   : size of the buffer to be sent in Bytes
                                 )
      to start playing (for the first time) from the audio file/stream.
   + Call the function BSP_AUDIO_OUT_Pause() to pause playing
   + Call the function BSP_AUDIO_OUT_Resume() to resume playing.
       Note. After calling BSP_AUDIO_OUT_Pause() function for pause, only BSP_AUDIO_OUT_Resume() should be called
          for resume (it is not allowed to call BSP_AUDIO_OUT_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).
   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named AUDIO_OUT_XXX_CallBack() and only their prototypes are declared in
      the stm32469i_discovery_audio.h file. (refer to the example for more details on the callbacks implementations)
   + To Stop playing, to modify the volume level, the frequency, the audio frame slot,
      the device output mode the mute or the stop, use the functions: BSP_AUDIO_OUT_SetVolume(),
      AUDIO_OUT_SetFrequency(), BSP_AUDIO_OUT_SetAudioFrameSlot(), BSP_AUDIO_OUT_SetOutputMode(),
      BSP_AUDIO_OUT_SetMute() and BSP_AUDIO_OUT_Stop().
   + The driver API and the callback functions are at the end of the stm32469i_discovery_audio.h file.

Driver architecture:
--------------------
   + This driver provide the High Audio Layer: consists of the function API exported in the stm32469i_discovery_audio.h file
     (BSP_AUDIO_OUT_Init(), BSP_AUDIO_OUT_Play() ...)
   + This driver provide also the Media Access Layer (MAL): which consists of functions allowing to access the media containing/
     providing the audio file/stream. These functions are also included as local functions into
     the stm32469i_discovery_audio_codec.c file (I2Sx_Init(), I2Sx_DeInit(), SAIx_Init() and SAIx_DeInit())

Known Limitations:
------------------
   1- If the TDM Format used to paly in parallel 2 audio Stream (the first Stream is configured in codec SLOT0 and second
      Stream in SLOT1) the Pause/Resume, volume and mute feature will control the both streams.
   2- Parsing of audio file is not implemented (in order to determine audio file properties: Mono/Stereo, Data size,
      File size, Audio Frequency, Audio Data header size ...). The configuration is fixed for the given audio file.
   3- Supports only Stereo audio streaming.
   4- Supports only 16-bits audio data size.
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "i2s_interface.h"

I2S_Out i2s_interface_out = {
		/*BSP_AUDIO_OUT_Init,
		BSP_AUDIO_OUT_Play,
		BSP_AUDIO_OUT_ChangeBuffer,
		BSP_AUDIO_OUT_Pause,
		BSP_AUDIO_OUT_Resume,
		BSP_AUDIO_OUT_Stop,
		BSP_AUDIO_OUT_SetVolume,
		BSP_AUDIO_OUT_SetFrequency,
		BSP_AUDIO_OUT_SetAudioFrameSlot,
		BSP_AUDIO_OUT_SetMute,
		BSP_AUDIO_OUT_SetOutputMode,
		BSP_AUDIO_OUT_DeInit*/
};

I2S_In i2s_interface_in = {
		/*BSP_AUDIO_IN_Init,
		BSP_AUDIO_IN_Record,
		BSP_AUDIO_IN_Stop,
		BSP_AUDIO_IN_Pause,
		BSP_AUDIO_IN_Resume,
		BSP_AUDIO_IN_SetVolume,
		BSP_AUDIO_IN_DeInit,
		BSP_AUDIO_IN_PDMToPCM*/
};


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
