/*
 * hs_mmcsd_interface.c
 *
 * Created: 2/11/2013 3:10:04 PM
 *  Author: XxXx
 */ 
//#######################################################################################
#include <stdbool.h>
#include "board_properties.h"
#include "hs_mmcsd_interface.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "driver/uart.h"
#include "driver/dma.h"
#include "lib/fs/fat.h"
#include "general/unions.h"
//#######################################################################################
new_dma_ch* DMA_MMCSD_TRANSMIT = NULL;
new_dma_ch* DMA_MMCSD_RECEIVE = NULL;
new_uart* MMCSD = NULL;
extern new_uart* DebugCom;
//#######################################################################################
uint8_t sd_io_data(void* Struct, uint8_t Value)
{
	return usart_spi_data((Uart_t *)Struct, Value);
}
//#######################################################################################
static void sd_delay(uint16_t DelayMs)
{
	//timer_interval(&sd_timer_delay,DelayMs);
	//timer_en(&sd_timer_delay,True);
	//while(timer_tick(&sd_timer_delay) == False);
	//timer_en(&sd_timer_delay,False);
	for (uint16_t CountMs = 0; CountMs < DelayMs; CountMs++)
	{
		_delay_ms(1);
	}
}	
//#######################################################################################
void sd_cs_assert(SD_Struct_t *SD_Struct)
{
	SD_Struct->CS_Port->OUTCLR = SD_Struct->CS_PinMask;
}
//#######################################################################################
void sd_cs_deassert(SD_Struct_t *SD_Struct)
{
	SD_Struct->CS_Port->OUTSET = SD_Struct->CS_PinMask;
}
//#######################################################################################
uint8_t sd_cs_readstate(SD_Struct_t *SD_Struct)
{
	return ((SD_Struct->CS_Port->IN) & (SD_Struct->CS_PinMask));
}
//#######################################################################################
static uint8_t sd_read_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	uint8_t Temp = 255;
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		Temp = sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
		if(Temp != 255) break;
	}
	 return Temp;
}
//#######################################################################################
static void sd_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
	}
}
//#######################################################################################
static uint8_t sd_send_cmd(SD_Struct_t *SD_Struct, uint8_t Command, uint32_t Address, uint8_t TryNr, uint8_t ExpectedResponse, uint16_t IdleBytes)
{
	uint8_t Tmp = 0;
	convert32to8 Addr;
	Addr.i32 = Address;
	if(TryNr == 255)
	{
		sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
		sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Command|0x40);
		sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte3);
		sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte2);
		sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte1);
		sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte0);
		if(Command == MMC_SEND_IF_COND)
		{
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x87);
		}
		else if(Command == MMC_CMD_RESET)
		{
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x95);
		}
		else sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x01);
		if(Command == MMC_CMD_STOP_TRANSMISSION) sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
		else Tmp = sd_read_idle_bytes(SD_Struct, IdleBytes);
	}
	else
	{
		for(uint8_t TryCount = 0; TryCount < TryNr; TryCount++)
		{
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Command|0x40);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte3);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte2);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte1);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Addr.Byte0);
			if(Command == MMC_SEND_IF_COND)
			{
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x87);
			}
			else if(Command == MMC_CMD_RESET)
			{
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x95);
			}
			else sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x01);
			if(Command == MMC_CMD_STOP_TRANSMISSION) Tmp = sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
			else Tmp = sd_read_idle_bytes(SD_Struct, IdleBytes);
			if(Tmp == ExpectedResponse) 
			{
				Tmp = true;
				break;
			}
			else Tmp = false;
		}		
	}
	return Tmp;
}
//#######################################################################################
void sd_init(SD_Struct_t *SD_Struct)
{
	//ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	//{
		//if(sd_cs_readstate(SD_Struct) == 0)
		//{
		//	SD_Struct.SD_Init_OK = False;
		//}
		//else
		{
			//s_timers_init();
			SD_CS_Port.DIRSET = 1<<SD_CS_Pin;
			SD_Struct->CS_Port->DIRSET = SD_Struct->CS_PinMask;
			sd_cs_deassert(SD_Struct);
			//SD_CS_Port.DIRCLR = 1<<SD_CS_Pin;
			//SD_CS_Port.OUTSET = 1<<SD_CS_Pin;
	//return sd_io_data(Struct,Value);
			//struct Spi_Struct *SpiStruct = (struct Spi_Struct*)malloc(sizeof(struct Spi_Struct));
			MMCSD = new_(new_uart);
			MMCSD->UartNr = SD_SPI_Comunication;
			MMCSD->BaudRate = 400000;
			MMCSD->Mode = UsartCom_Mode_Spi;
			MMCSD->Priority = Usart_Int_Disabled;
			MMCSD->CS_Port = SD_Struct->CS_Port;
			MMCSD->CS_PinMask = SD_Struct->CS_PinMask;
			uart_open(MMCSD);
			SD_Struct->SD_SPI_Struct_Addr = (void*)MMCSD;
			//SD_Struct->CS_Port = &SD_CS_Port;
			//SD_Struct->CS_PinMask = 1<<SD_CS_Pin;

			sd_delay(20);
			sd_idle_bytes(SD_Struct, 20);
			sd_cs_deassert(SD_Struct);
			sd_idle_bytes(SD_Struct, 20);
			sd_cs_assert(SD_Struct);
			uint8_t Tmp = 0;
			SD_Struct->SD_Hc = IsNoCard;
			SD_Struct->SD_Init_OK = false;
			if(sd_send_cmd(SD_Struct, MMC_CMD_RESET, 0, SdCard_NumberToTryResetCmd, 1, 256))
			{
				Tmp = sd_send_cmd(SD_Struct, MMC_SEND_IF_COND, 0x1AA, 255, 1, 65535);
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
				//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
				//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
				//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
				//fprintf_P(SCREEN1, PSTR("%x\n\r"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
				if(Tmp == false)
				{
					SD_Struct->SD_Init_OK = false;
				}
				else
				{
					uint8_t Flag = false;
					for(uint8_t SdHcInitCount = 0;SdHcInitCount < 15; SdHcInitCount++)
					{
						sd_delay(SdDriverDelaysConstant);
						sd_send_cmd(SD_Struct, MMC_CMD_APP_CMD, 0x00000000, 255, 0, 256);
						if(sd_send_cmd(SD_Struct, MMC_CMD_START_INITIALIZATION, 0x00000000, SdCard_NumberToTryIfCondCmd, 0, 256))
						{

							SD_Struct->SD_Hc = IsSd;
							sd_send_cmd(SD_Struct, MMC_CMD_SET_BLOCK_SIZE, 512, 255, 0, 256);
							SD_Struct->SD_Init_OK = true;
							SD_Struct->SD_Structure_Addr = SD_Struct;
							Flag = true;
							break;
						}
					}
					if (Flag == false)
					{
						SD_Struct->SD_Hc = IsSdhc;
						sd_send_cmd(SD_Struct, MMC_CMD_RESET, 0, SdCard_NumberToTryResetCmd, 1, 256);
						Tmp = sd_send_cmd(SD_Struct, MMC_SEND_IF_COND, 0x1AA, 255, 1, 256);
						sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
						sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
						sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
						sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
						//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
						//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
						//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
						//fprintf_P(SCREEN1, PSTR("%x\n\r"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
						if(Tmp == false)
						{
							SD_Struct->SD_Init_OK = Tmp;
						}
						else
						{
							for(uint8_t SdHcInitCount = 0;SdHcInitCount < 50; SdHcInitCount++)
							{
								sd_delay(SdDriverDelaysConstant);
								sd_send_cmd(SD_Struct, MMC_CMD_APP_CMD, 0x00000000, 255, 0, 256);
								if(sd_send_cmd(SD_Struct, MMC_CMD_START_INITIALIZATION, 0x40000000, SdCard_NumberToTryIfCondCmd, 0, 256) && SdHcInitCount < 10)
								{
									//Tmp = sd_send_cmd(SD_Struct, MMC_CMD_READ_OCR, 0x00000000, SdCard_NumberToTryIfCondCmd, 1, 256);
									//fprintf_P(SCREEN1, PSTR("MMC_CMD_READ_OCR = %x-"), (int32_t)Tmp);
									//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
									//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
									//fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
									//fprintf_P(SCREEN1, PSTR("%x\n\r"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
									SD_Struct->SD_Hc = IsSdhc;
									sd_send_cmd(SD_Struct, MMC_CMD_SET_BLOCK_SIZE, 512, 255, 0, 256);
									SD_Struct->SD_Init_OK = true;
									SD_Struct->SD_Structure_Addr = SD_Struct;
									break;
								}
								/*else
								{
									if((Tmp = sd_send_cmd(SD_Struct, MMC_CMD_APP_CMD, 0x00000000, 255, 1, 256)) = True)
									{
										fprintf_P(SCREEN1, PSTR("MMC_CMD_APP_CMD = %x\n\r"), (int32_t)Tmp);
										if((Tmp = sd_send_cmd(SD_Struct, MMC_CMD_SD_SEND_OP_COND, 0x00000000, SdCard_NumberToTryIfCondCmd, 1, 256)) == True)
										{
											fprintf_P(SCREEN1, PSTR("MMC_CMD_SD_SEND_OP_COND = %x\n\r"), (int32_t)Tmp);
											Tmp = sd_send_cmd(SD_Struct, MMC_CMD_READ_OCR, 0x00000000, SdCard_NumberToTryIfCondCmd, 1, 256);
											fprintf_P(SCREEN1, PSTR("MMC_CMD_READ_OCR = %x-"), (int32_t)Tmp);
											fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
											fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
											fprintf_P(SCREEN1, PSTR("%x-"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
											fprintf_P(SCREEN1, PSTR("%x\n\r"), (int32_t)sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255));
											//SD_Struct->SD_Hc = True;
											//sd_send_cmd(SD_Struct, MMC_CMD_SET_BLOCK_SIZE, 512, 255, 0, 256);
											//SD_Struct.SD_Init_OK = True;
											//SD_Struct.SD_Structure_Addr = SD_Struct;
											SdHcInitCount = 0;
											break;
										}
									}								
								}	*/						
								//fprintf_P(SCREEN1, PSTR("%x_"), (int32_t)Tmp);
								//else if(SdHcInitCount = 10)
								//{
								//	sd_send_cmd(SD_Struct, MMC_CMD_SET_BLOCK_SIZE, 512, 255, 0, 256);
								//	SD_Struct.SD_Init_OK = True;
								//	SD_Struct.SD_Structure_Addr = SD_Struct;
								//	break;
								//}
							}
						}
					}										
				}					
			}
			else SD_Struct->SD_Init_OK = false;
			//if(SD_Struct.SD_Init_OK == True)
			//{
			//	SpiStruct->Spi_BaudDiv	= _SpiClkDiv4;
			//	spi_enable((void*)SpiStruct,True);
			//}
		}
		((Uart_t *)SD_Struct->SD_SPI_Struct_Addr)->BaudRate	= 4000000;
		uart_set_baud_rate(((Uart_t *)SD_Struct->SD_SPI_Struct_Addr)->BaseAddr, ((Uart_t *)SD_Struct->SD_SPI_Struct_Addr)->BaudRate);
		sd_cs_deassert(SD_Struct);
		//return SD_Struct;
	//}		
}
//#######################################################################################
static inline bool rcvr_datablock(void *_SD_Struct, unsigned char *buff, unsigned int bytes_to_read)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	unsigned char token;
	//unsigned char Timer1 = 10;
	do
	{							/* Wait for data packet in timeout of 100ms */
		token = sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
	} while ((token == 0xFF)/* && Timer1--*/);

	if(token != MMC_DATA_TOKEN) return false;	/* If not valid data token, return with error */

	do {							/* Receive the data block into buffer */
		*buff++ = sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);
	} while (--bytes_to_read);

	sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);						/* Discard CRC */
	sd_io_data(SD_Struct->SD_SPI_Struct_Addr,255);

	return true;					/* Return with success */
}
//#######################################################################################
unsigned int _sd_read_page(void *_SD_Struct, void* _Buffer, unsigned long block, unsigned int nblks)
{
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
		if(sd_cs_readstate(SD_Struct))
		{
			unsigned long Addr = block;
			if(SD_Struct->SD_Hc == IsSd) Addr = Addr<<9;
			sd_cs_assert(SD_Struct);
			uint8_t Tmp;
			if (nblks > 1)
			{
				Tmp = sd_send_cmd(SD_Struct, MMC_CMD_READ_MULTIPLE_BLOCKS, Addr, 255, 0, 0);
				//Tmp = sd_send_cmd(SD_Struct, MMC_CMD_READ_MULTIPLE_BLOCKS, Addr, 1, 0, DelayReadCommand);
				if(Tmp == false)
				{	
					sd_cs_deassert(SD_Struct);
					return false;
				} 
			} 
			else
			{
				//sd_send_cmd(SD_Struct, MMC_CMD_READ_SINGLE_BLOCK, Addr, 255, MMC_DATA_TOKEN, 1);
				Tmp = sd_send_cmd(SD_Struct, MMC_CMD_READ_SINGLE_BLOCK, Addr, 1, 0, DelayReadCommand);
				if(Tmp == false)
				{
					sd_cs_deassert(SD_Struct);
					return false;
				} 
			}
			unsigned int _nblks = nblks;
			unsigned char* Buffer = (unsigned char*)_Buffer;
			//unsigned int TransferedBlocks = 0;
			do
			{
				if(!rcvr_datablock(SD_Struct, Buffer/* + (512*TransferedBlocks)*/, 512)) 
				{
					sd_cs_deassert(SD_Struct);
					return false;
				}					
				Buffer += 512;
				//TransferedBlocks++;
			}while (--_nblks);

			if (nblks > 1)
			{
				//Tmp = sd_io_data(SD_Struct->SD_SPI_Struct_Addr, MMC_CMD_STOP_TRANSMISSION);
				//do
				//{
				//	Tmp = sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
				//}while(Tmp != MMC_DATA_TOKEN);
				sd_send_cmd(SD_Struct, MMC_CMD_STOP_TRANSMISSION, 0, 255, MMC_DATA_TOKEN, 256);
			}
			sd_cs_deassert(SD_Struct);
			return true;
		}
#ifdef _UTIL_ATOMIC_H_
	}	
#endif
	return false;
}
//#######################################################################################
unsigned int sd_read_page(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//unsigned int TransferedBlocks = 0;
	do 
	{
		if(!_sd_read_page(_SD_Struct, Buffer/* + (512*TransferedBlocks)*/, block++, 1)) return false;
		Buffer += 512;
		//TransferedBlocks++;
	} while (--nblks);
	return true;
}	
//#######################################################################################
unsigned int  _sd_write_page(void *_SD_Struct, void* _Buffer, unsigned long block, unsigned int nblks)
{
#ifdef _UTIL_ATOMIC_H_
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
		if(sd_cs_readstate(SD_Struct))
		{
			unsigned long Addr = block;
			if(SD_Struct->SD_Hc == IsSd) Addr = Addr<<9;
			sd_cs_assert(SD_Struct);
			//uint8_t Tmp = sd_send_cmd(SD_Struct, MMC_CMD_WRITE_SINGLE_BLOCK, Addr, 255, 0, DelayWriteCommand);
		
		
			convert32to8 _Addr;
			_Addr.i32 = Addr;
			if (nblks > 1)
			{
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr,MMC_CMD_WRITE_MULTIPLE_BLOCKS|0x40);
			}
			else
			{
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr,MMC_CMD_WRITE_SINGLE_BLOCK|0x40);
			}
			//sd_io_data(SD_Struct->SD_SPI_Struct_Addr,MMC_CMD_WRITE_SINGLE_BLOCK|0x40);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,_Addr.Byte3);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,_Addr.Byte2);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,_Addr.Byte1);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr,_Addr.Byte0);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x95);
			while(sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0x95) != 0);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0xFF);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 0xFF);
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr, MMC_DATA_TOKEN);

		
		
			/*if(Tmp != 0)
			{
				sd_cs_deassert();
				return False;
			} 
			sd_io_data(SD_Struct->SD_SPI_Struct_Addr, MMC_DATA_TOKEN);*/
			uint8_t* Buffer = (unsigned char*)_Buffer;
			unsigned int _nblks = nblks;
			do
			{
				for (unsigned short ReadBytesCount = 0; ReadBytesCount < 512; ReadBytesCount++)
				{
					sd_io_data(SD_Struct->SD_SPI_Struct_Addr,Buffer[ReadBytesCount]);
					//usart_print_char(6,Buffer[ReadBytesCount]);
				}
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255);
			}while (--_nblks);
			if (nblks > 1)
			{
				sd_io_data(SD_Struct->SD_SPI_Struct_Addr, MMC_CMD_STOP_TRANSMISSION);
			}
			if((sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255) & 0x1F) != 5)
			{
				sd_cs_deassert(SD_Struct);
				return false;
			}			
			while (sd_io_data(SD_Struct->SD_SPI_Struct_Addr, 255) == 0);
			sd_cs_deassert(SD_Struct);
			return true;
		}
#ifdef _UTIL_ATOMIC_H_
	}	
#endif
return false;
}
//#######################################################################################
unsigned int sd_write_page(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//unsigned int TransferedBlocks = 0;
	do
	{
		if(!_sd_write_page(_SD_Struct, Buffer/* + (512*TransferedBlocks)*/, block++, 1)) return false;
		Buffer += 512;
		//TransferedBlocks++;
	} while (--nblks);
	return true;
}
//#######################################################################################
void _mmcsd_init(void *SdStruct, new_gpio* Cs, new_gpio* StatusLed)
{
	SD_Struct_t *SD_StructDisk = (SD_Struct_t *)SdStruct;

	sd_init(SD_StructDisk);
	//if(SD_StructDisk->SD_Init_OK) return true;

	/*MMCSD = new_(new_uart);
	if(!MMCSD) return false;
	MMCSD->BaudRate = 19200;
	MMCSD->RxBuffSize = 0;
	MMCSD->TxBuffSize = 0;
	MMCSD->Mode = Usart_Mode_Spi;
	MMCSD->Priority = Usart_Int_Disabled;
	MMCSD->UartNr = 7;
	uart_open(MMCSD);
	
	USART_t *UartBaseAddr = (USART_t *)MMCSD->BaseAddr;
	
	DMA_MMCSD_TRANSMIT = new_(new_dma_ch);
	DMA_MMCSD_TRANSMIT->Dma_BlockLength = 512;//DisplayStruct->NrOfLaths;
	DMA_MMCSD_TRANSMIT->Dma_BurstMode = _Dma_BurstMode_8;
	DMA_MMCSD_TRANSMIT->Dma_CompleteIntLevel = _Dma_Int_Disabled;
	DMA_MMCSD_TRANSMIT->Dma_DestAddr = (unsigned int)&UartBaseAddr->DATA;
	DMA_MMCSD_TRANSMIT->Dma_DestAddrMode = _Dma_DestAddressMode_Fixed;
	DMA_MMCSD_TRANSMIT->Dma_DestAddrReload = _Dma_DestAddressReload_None;
	DMA_MMCSD_TRANSMIT->Dma_ErrIntLevel = _Dma_Int_Disabled;
	DMA_MMCSD_TRANSMIT->Dma_RepeatMode = false;
	DMA_MMCSD_TRANSMIT->Dma_RepeatNr = 1;
	DMA_MMCSD_TRANSMIT->Dma_SingleShot = false;
	DMA_MMCSD_TRANSMIT->Dma_SrcAddr = 0;//(unsigned long)DisplayStruct->LathBuffer;
	DMA_MMCSD_TRANSMIT->Dma_SrcAddrMode = _Dma_SourceAddressMode_Dec;
	DMA_MMCSD_TRANSMIT->Dma_SrcAddrReload = _Dma_SourceAddressReload_Block;
	DMA_MMCSD_TRANSMIT->Dma_Trigers = _Dma_Triger_Off;
	dma_search_and_use(DMA_MMCSD_TRANSMIT, USED_MMCSD_DMA + 4);

	DMA_MMCSD_RECEIVE = new_(new_dma_ch);
	DMA_MMCSD_RECEIVE->Dma_BlockLength = 512;//DisplayStruct->NrOfLaths;
	DMA_MMCSD_RECEIVE->Dma_BurstMode = _Dma_BurstMode_8;
	DMA_MMCSD_RECEIVE->Dma_CompleteIntLevel = _Dma_Int_Disabled;
	DMA_MMCSD_RECEIVE->Dma_DestAddr = 0;
	DMA_MMCSD_RECEIVE->Dma_DestAddrMode = _Dma_DestAddressMode_Fixed;
	DMA_MMCSD_RECEIVE->Dma_DestAddrReload = _Dma_DestAddressReload_None;
	DMA_MMCSD_RECEIVE->Dma_ErrIntLevel = _Dma_Int_Disabled;
	DMA_MMCSD_RECEIVE->Dma_RepeatMode = false;
	DMA_MMCSD_RECEIVE->Dma_RepeatNr = 1;
	DMA_MMCSD_RECEIVE->Dma_SingleShot = false;
	DMA_MMCSD_RECEIVE->Dma_SrcAddr = (unsigned int)&UartBaseAddr->DATA;
	DMA_MMCSD_RECEIVE->Dma_SrcAddrMode = _Dma_DestAddressMode_Fixed;
	DMA_MMCSD_RECEIVE->Dma_SrcAddrReload = _Dma_SourceAddressReload_Block;
	DMA_MMCSD_RECEIVE->Dma_Trigers = _Dma_Triger_Off;
	dma_search_and_use(DMA_MMCSD_RECEIVE, USED_MMCSD_DMA + 4);*/
/*-----------------------------------------------------*/
	return false;
}
//#######################################################################################
void _mmcsd_idle(void *SdStruct)
{
	SD_Struct_t *SD_StructDisk = (SD_Struct_t *)SdStruct;
	Drives_Table[SD_StructDisk->DriveNr] = new_(new_fat_disk);
    Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SdDriverStructAddr = SdStruct;
    //Drives_Table[0]->drive_init = MMCSD_CardInit;
    Drives_Table[SD_StructDisk->DriveNr]->drive_read_page = sd_read_page;
    Drives_Table[SD_StructDisk->DriveNr]->drive_write_page = sd_write_page;
    if(_Fat_Mount(SD_StructDisk->DriveNr))
    {
        if(DebugCom)
        {
																					UARTprintf(DebugCom, "MMCSD%d drive mounted\n\r" , (signed long)SD_StructDisk->DriveNr);
																					UARTprintf(DebugCom, "MMCSD%d Fat fs detected\n\r" , (signed long)SD_StructDisk->DriveNr);
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == NoFs ){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 None\n\r" , (signed long)SD_StructDisk->DriveNr);}
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == Fat12){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 Fat12\n\r" , (signed long)SD_StructDisk->DriveNr);}
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == Fat16){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 Fat16\n\r" , (signed long)SD_StructDisk->DriveNr);}
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == Fat32){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 Fat32\n\r" , (signed long)SD_StructDisk->DriveNr);}
																					UARTprintf(DebugCom, "MMCSD%d BootSectorAddress:       %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BootSectorAddress);
																					UARTprintf(DebugCom, "MMCSD%d BytesPerSector:          %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BytesPerSector);
																					UARTprintf(DebugCom, "MMCSD%d SectorsPerCluster:       %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SectorsPerCluster);
																					UARTprintf(DebugCom, "MMCSD%d AllocTable1Begin:        %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTable1Begin);
																					UARTprintf(DebugCom, "MMCSD%d NumberOfFats:            %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_NumberOfFats);
																					UARTprintf(DebugCom, "MMCSD%d MediaType:               %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_MediaType);
																					UARTprintf(DebugCom, "MMCSD%d AllocTableSize:          %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTableSize);
																					UARTprintf(DebugCom, "MMCSD%d DataSectionBegin:        %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_DataSectionBegin);
			if(Drives_Table[0]->DiskInfo_FatType == Fat32)							UARTprintf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)((unsigned long long)((unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTableSize * 128 * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BytesPerSector) / 1000000));
			else if(Drives_Table[0]->DiskInfo_FatType == Fat16)						UARTprintf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)((unsigned long long)((unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTableSize * 256 * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BytesPerSector) / 1000000));
        }
    }
    else if(DebugCom)																UARTprintf(DebugCom,   "MMCSD%d Fat not detected\n\r" , (signed long)SD_StructDisk->DriveNr);

}
//#######################################################################################
