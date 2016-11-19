/*
 * audio_def.h
 *
 *  Created on: Nov 15, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_I2S_DEF_H_
#define LIB_API_I2S_DEF_H_

#include "api/twi_def.h"
/* Codec audio Standards */
#define CODEC_STANDARD                0x04
#define I2S_STANDARD                  I2S_STANDARD_PHILIPS

/**
  * @}
  */

/** @defgroup AUDIO_Exported_Types
  * @{
  */

/** @defgroup AUDIO_Driver_structure  Audio Driver structure
  * @{
  */
typedef struct
{
	unsigned long  	(*Init)(void *, unsigned short, unsigned char, unsigned long);
	void      		(*DeInit)(void);
	unsigned long  	(*ReadID)(void *);
	unsigned long  	(*Play)(void *, unsigned short*, unsigned short);
	unsigned long  	(*Pause)(void *);
	unsigned long  	(*Resume)(void *);
	unsigned long  	(*Stop)(void *, unsigned long);
	unsigned long  	(*SetFrequency)(void *, unsigned long);
	unsigned long  	(*SetVolume)(void *, unsigned char);
	unsigned long  	(*SetMute)(void *, unsigned long);
	unsigned long  	(*SetOutputMode)(void *, unsigned char);
	unsigned long  	(*Reset)(void *param);
	void *param;
}I2S_DeviceDriver;

typedef struct {
	unsigned char 	(*Init)(void *param, unsigned short OutputDevice, unsigned char Volume, unsigned long AudioFreq);
	unsigned char 	(*Play)(void *param, unsigned short* pBuffer, unsigned long Size);
	void    		(*ChangeBuffer)(void *param, unsigned short *pData, unsigned short Size);
	unsigned char 	(*Pause)(void *param);
	unsigned char 	(*Resume)(void *param);
	unsigned char 	(*Stop)(void *param, unsigned long Option);
	unsigned char 	(*SetVolume)(void *param, unsigned char Volume);
	void    		(*SetFrequency)(void *param, unsigned long AudioFreq);
	void    		(*SetAudioFrameSlot)(void *param, unsigned long AudioFrameSlot);
	unsigned char 	(*SetMute)(void *param, unsigned long Cmd);
	unsigned char 	(*SetOutputMode)(void *param, unsigned char Output);
	void    		(*DeInit)(void *param);
	struct {
		/* User Callbacks: user has to implement these functions in his code if they are needed. */
		/* This function is called when the requested data has been completely transferred.*/
		void    (*TransferComplete)(void);
		/* This function is called when half of the requested buffer has been transferred. */
		void    (*HalfTransfer)(void);
		/* This function is called when an Interrupt due to transfer error on or peripheral
		   error occurs. */
		void    (*Error)(void);
	}Callbacks;
}I2S_Out;

typedef struct {
	unsigned char 	(*Init)(void *param, unsigned long AudioFreq, unsigned long BitRes, unsigned long ChnlNbr);
	unsigned char 	(*Record)(void *param, unsigned short *pData, unsigned long Size);
	unsigned char 	(*Stop)(void *param);
	unsigned char 	(*Pause)(void *param);
	unsigned char 	(*Resume)(void *param);
	unsigned char 	(*SetVolume)(void *param, unsigned char Volume);
	void    		(*DeInit)(void *param);
	unsigned char 	(*PDMToPCM)(void *param, unsigned short* PDMBuf, unsigned short* PCMBuf);
	struct {
		/* This function should be implemented by the user application.
		   It is called into this driver when the current buffer is filled to prepare the next
		   buffer pointer and its size. */
		void    (*TransferComplete)(void);
		void    (*HalfTransfer)(void);
		/* This function is called when an Interrupt due to transfer error on or peripheral
		   error occurs. */
		void    (*Error)(void);

	};
}I2S_In;


typedef struct I2S_s
{
	I2S_DeviceDriver *device;
	I2S_Out *out;
	I2S_In *in;
}I2S_t;

/*#####################################################*/
#define new_i2s I2S_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_i2s(address) free(address);
/*#####################################################*/
#endif /* LIB_API_I2S_DEF_H_ */
