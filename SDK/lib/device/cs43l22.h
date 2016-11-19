/*
 * cs43l22.h
 *
 *  Created on: Nov 15, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_CS43L22_H_
#define LIB_DEVICE_CS43L22_H_

#include <api/i2s_def.h>
/******************************************************************************/
/***************************  Codec User defines ******************************/
/******************************************************************************/
/**
  * @brief Audio I2C Slave address
  */
#define CS43l22_I2C_ADDRESS                ((uint16_t)0x94)
/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER         1
#define OUTPUT_DEVICE_HEADPHONE       2
#define OUTPUT_DEVICE_BOTH            3
#define OUTPUT_DEVICE_AUTO            4

/* Volume Levels values */
#define DEFAULT_VOLMIN                0x00
#define DEFAULT_VOLMAX                0xFF
#define DEFAULT_VOLSTEP               0x04

#define AUDIO_PAUSE                   0
#define AUDIO_RESUME                  1

/* Codec POWER DOWN modes */
#define CODEC_PDWN_HW                 1
#define CODEC_PDWN_SW                 2

/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0

/* AUDIO FREQUENCY */
#define AUDIO_FREQUENCY_192K          ((unsigned long)192000)
#define AUDIO_FREQUENCY_96K           ((unsigned long)96000)
#define AUDIO_FREQUENCY_48K           ((unsigned long)48000)
#define AUDIO_FREQUENCY_44K           ((unsigned long)44100)
#define AUDIO_FREQUENCY_32K           ((unsigned long)32000)
#define AUDIO_FREQUENCY_22K           ((unsigned long)22050)
#define AUDIO_FREQUENCY_16K           ((unsigned long)16000)
#define AUDIO_FREQUENCY_11K           ((unsigned long)11025)
#define AUDIO_FREQUENCY_8K            ((unsigned long)8000)

/** CS43l22 Registers  ***/
#define   CS43L22_REG_ID                  0x01
#define   CS43L22_REG_POWER_CTL1          0x02
#define   CS43L22_REG_POWER_CTL2          0x04
#define   CS43L22_REG_CLOCKING_CTL        0x05
#define   CS43L22_REG_INTERFACE_CTL1      0x06
#define   CS43L22_REG_INTERFACE_CTL2      0x07
#define   CS43L22_REG_PASSTHR_A_SELECT    0x08
#define   CS43L22_REG_PASSTHR_B_SELECT    0x09
#define   CS43L22_REG_ANALOG_ZC_SR_SETT   0x0A
#define   CS43L22_REG_PASSTHR_GANG_CTL    0x0C
#define   CS43L22_REG_PLAYBACK_CTL1       0x0D
#define   CS43L22_REG_MISC_CTL            0x0E
#define   CS43L22_REG_PLAYBACK_CTL2       0x0F
#define   CS43L22_REG_PASSTHR_A_VOL       0x14
#define   CS43L22_REG_PASSTHR_B_VOL       0x15
#define   CS43L22_REG_PCMA_VOL            0x1A
#define   CS43L22_REG_PCMB_VOL            0x1B
#define   CS43L22_REG_BEEP_FREQ_ON_TIME   0x1C
#define   CS43L22_REG_BEEP_VOL_OFF_TIME   0x1D
#define   CS43L22_REG_BEEP_TONE_CFG       0x1E
#define   CS43L22_REG_TONE_CTL            0x1F
#define   CS43L22_REG_MASTER_A_VOL        0x20
#define   CS43L22_REG_MASTER_B_VOL        0x21
#define   CS43L22_REG_HEADPHONE_A_VOL     0x22
#define   CS43L22_REG_HEADPHONE_B_VOL     0x23
#define   CS43L22_REG_SPEAKER_A_VOL       0x24
#define   CS43L22_REG_SPEAKER_B_VOL       0x25
#define   CS43L22_REG_CH_MIXER_SWAP       0x26
#define   CS43L22_REG_LIMIT_CTL1          0x27
#define   CS43L22_REG_LIMIT_CTL2          0x28
#define   CS43L22_REG_LIMIT_ATTACK_RATE   0x29
#define   CS43L22_REG_OVF_CLK_STATUS      0x2E
#define   CS43L22_REG_BATT_COMPENSATION   0x2F
#define   CS43L22_REG_VP_BATTERY_LEVEL    0x30
#define   CS43L22_REG_SPEAKER_STATUS      0x31
#define   CS43L22_REG_TEMPMONITOR_CTL     0x32
#define   CS43L22_REG_THERMAL_FOLDBACK    0x33
#define   CS43L22_REG_CHARGE_PUMP_FREQ    0x34

/******************************************************************************/
/****************************** REGISTER MAPPING ******************************/
/******************************************************************************/
/**
  * @brief  CS43L22 ID
  */
#define  CS43L22_ID            0xE0
#define  CS43L22_ID_MASK       0xF8
/**
  * @brief Chip ID Register: Chip I.D. and Revision Register
  *  Read only register
  *  Default value: 0x01
  *  [7:3] CHIPID[4:0]: I.D. code for the CS43L22.
  *        Default value: 11100b
  *  [2:0] REVID[2:0]: CS43L22 revision level.
  *        Default value:
  *        000 - Rev A0
  *        001 - Rev A1
  *        010 - Rev B0
  *        011 - Rev B1
  */
#define CS43L22_CHIPID_ADDR    0x01

/**
  * @}
  */

/** @defgroup CS43L22_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup CS43L22_Exported_Functions
  * @{
  */

typedef struct {
	unsigned char TWI_Addr;
	Twi_t *TWI;
}cs43l22_t;

/*------------------------------------------------------------------------------
                           Audio Codec functions
------------------------------------------------------------------------------*/
/* High Layer codec functions */
unsigned long cs43l22_Init(void *param, unsigned short OutputDevice, unsigned char Volume, unsigned long AudioFreq);
void     cs43l22_DeInit(void);
unsigned long cs43l22_ReadID(void *param);
unsigned long cs43l22_Play(void *param, unsigned short* pBuffer, unsigned short Size);
unsigned long cs43l22_Pause(void *param);
unsigned long cs43l22_Resume(void *param);
unsigned long cs43l22_Stop(void *param, unsigned long Cmd);
unsigned long cs43l22_SetVolume(void *param, unsigned char Volume);
unsigned long cs43l22_SetFrequency(void *param, unsigned long AudioFreq);
unsigned long cs43l22_SetMute(void *param, unsigned long Cmd);
unsigned long cs43l22_SetOutputMode(void *param, unsigned char Output);
unsigned long cs43l22_Reset(void *param);

/* AUDIO IO functions */
//void      AUDIO_IO_Init(void);
//void      AUDIO_IO_DeInit(void);
//void      AUDIO_IO_Write(unsigned char Addr, unsigned char Reg, unsigned char Value);
//unsigned char   AUDIO_IO_Read(unsigned char Addr, unsigned char Reg);

/* Audio driver structure */
extern I2S_DeviceDriver   cs43l22_drv;


#endif /* LIB_DEVICE_CS43L22_H_ */
