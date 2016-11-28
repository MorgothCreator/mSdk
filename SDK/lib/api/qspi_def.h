/*
 * qspi_api_def.h
 *
 *  Created on: Nov 20, 2016
 *      Author: John Smith
 */

#ifndef LIB_API_QSPI_DEF_H_
#define LIB_API_QSPI_DEF_H_

/**
  * @brief  N25Q128A N25Q256A N25Q512A Commands
  */
/* Reset Operations */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99

/* Identification Operations */
#define READ_ID_CMD                          0x9E
#define READ_ID_CMD2                         0x9F
#define MULTIPLE_IO_READ_ID_CMD              0xAF
#define READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5A

/* Read Operations */
#define READ_CMD                             0x03
#define READ_4_BYTE_ADDR_CMD                 0x13

#define FAST_READ_CMD                        0x0B
#define FAST_READ_DTR_CMD                    0x0D
#define FAST_READ_4_BYTE_ADDR_CMD            0x0C

#define DUAL_OUT_FAST_READ_CMD               0x3B
#define DUAL_OUT_FAST_READ_DTR_CMD           0x3D
#define DUAL_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x3C

#define DUAL_INOUT_FAST_READ_CMD             0xBB
#define DUAL_INOUT_FAST_READ_DTR_CMD         0xBD
#define DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xBC

#define QUAD_OUT_FAST_READ_CMD               0x6B
#define QUAD_OUT_FAST_READ_DTR_CMD           0x6D
#define QUAD_OUT_FAST_READ_4_BYTE_ADDR_CMD   0x6C

#define QUAD_INOUT_FAST_READ_CMD             0xEB
#define QUAD_INOUT_FAST_READ_DTR_CMD         0xED
#define QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xEC

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define READ_STATUS_REG_CMD                  0x05
#define WRITE_STATUS_REG_CMD                 0x01

#define READ_LOCK_REG_CMD                    0xE8
#define WRITE_LOCK_REG_CMD                   0xE5

#define READ_FLAG_STATUS_REG_CMD             0x70
#define CLEAR_FLAG_STATUS_REG_CMD            0x50

#define READ_NONVOL_CFG_REG_CMD              0xB5
#define WRITE_NONVOL_CFG_REG_CMD             0xB1

#define READ_VOL_CFG_REG_CMD                 0x85
#define WRITE_VOL_CFG_REG_CMD                0x81

#define READ_ENHANCED_VOL_CFG_REG_CMD        0x65
#define WRITE_ENHANCED_VOL_CFG_REG_CMD       0x61

#define READ_EXT_ADDR_REG_CMD                0xC8
#define WRITE_EXT_ADDR_REG_CMD               0xC5

/* Program Operations */
#define PAGE_PROG_CMD                        0x02
#define PAGE_PROG_4_BYTE_ADDR_CMD            0x12

#define DUAL_IN_FAST_PROG_CMD                0xA2
#define EXT_DUAL_IN_FAST_PROG_CMD            0xD2

#define QUAD_IN_FAST_PROG_CMD                0x32
#define EXT_QUAD_IN_FAST_PROG_CMD            0x12 /*0x38*/
#define QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD    0x34

/* Erase Operations */
#define SUBSECTOR_ERASE_CMD                  0x20
#define SUBSECTOR_ERASE_4_BYTE_ADDR_CMD      0x21

#define SECTOR_ERASE_CMD                     0xD8
#define SECTOR_ERASE_4_BYTE_ADDR_CMD         0xDC

#define BULK_ERASE_CMD                       0xC7

#define PROG_ERASE_RESUME_CMD                0x7A
#define PROG_ERASE_SUSPEND_CMD               0x75

/* One-Time Programmable Operations */
#define READ_OTP_ARRAY_CMD                   0x4B
#define PROG_OTP_ARRAY_CMD                   0x42

/* 4-byte Address Mode Operations */
#define ENTER_4_BYTE_ADDR_MODE_CMD           0xB7
#define EXIT_4_BYTE_ADDR_MODE_CMD            0xE9

/* Quad Operations */
#define ENTER_QUAD_CMD                       0x35
#define EXIT_QUAD_CMD                        0xF5
/**
  * @brief  N25Q512A Registers
  */
/* Status Register */
#define SR_WIP                      ((unsigned char)0x01)    /*!< Write in progress */
#define SR_WREN                     ((unsigned char)0x02)    /*!< Write enable latch */
#define SR_BLOCKPR                  ((unsigned char)0x5C)    /*!< Block protected against program and erase operations */
#define SR_PRBOTTOM                 ((unsigned char)0x20)    /*!< Protected memory area defined by BLOCKPR starts from top or bottom */
#define SR_SRWREN                   ((unsigned char)0x80)    /*!< Status register write enable/disable */

/* Non volatile Configuration Register */
#define NVCR_NBADDR                 ((uint16_t)0x0001) /*!< 3-bytes or 4-bytes addressing */
#define NVCR_SEGMENT                ((uint16_t)0x0002) /*!< Upper or lower 128Mb segment selected by default */
#define NVCR_DUAL                   ((uint16_t)0x0004) /*!< Dual I/O protocol */
#define NVCR_QUAB                   ((uint16_t)0x0008) /*!< Quad I/O protocol */
#define NVCR_RH                     ((uint16_t)0x0010) /*!< Reset/hold */
#define NVCR_ODS                    ((uint16_t)0x01C0) /*!< Output driver strength */
#define NVCR_XIP                    ((uint16_t)0x0E00) /*!< XIP mode at power-on reset */
#define NVCR_NB_DUMMY               ((uint16_t)0xF000) /*!< Number of dummy clock cycles */

/* Volatile Configuration Register */
#define VCR_WRAP                    ((unsigned char)0x03)    /*!< Wrap */
#define VCR_XIP                     ((unsigned char)0x08)    /*!< XIP */
#define VCR_NB_DUMMY                ((unsigned char)0xF0)    /*!< Number of dummy clock cycles */

/* Extended Address Register */
#define EAR_A24                     ((unsigned char)0x01)    /*!< Select the lower or upper 128Mb segment */

/* Enhanced Volatile Configuration Register */
#define EVCR_ODS                    ((unsigned char)0x07)    /*!< Output driver strength */
#define EVCR_VPPA                   ((unsigned char)0x08)    /*!< Vpp accelerator */
#define EVCR_RH                     ((unsigned char)0x10)    /*!< Reset/hold */
#define EVCR_DUAL                   ((unsigned char)0x40)    /*!< Dual I/O protocol */
#define EVCR_QUAD                   ((unsigned char)0x80)    /*!< Quad I/O protocol */

/* Flag Status Register */
#define FSR_NBADDR                  ((unsigned char)0x01)    /*!< 3-bytes or 4-bytes addressing */
#define FSR_PRERR                   ((unsigned char)0x02)    /*!< Protection error */
#define FSR_PGSUS                   ((unsigned char)0x04)    /*!< Program operation suspended */
#define FSR_VPPERR                  ((unsigned char)0x08)    /*!< Invalid voltage during program or erase */
#define FSR_PGERR                   ((unsigned char)0x10)    /*!< Program error */
#define FSR_ERERR                   ((unsigned char)0x20)    /*!< Erase error */
#define FSR_ERSUS                   ((unsigned char)0x40)    /*!< Erase operation suspended */
#define FSR_READY                   ((unsigned char)0x80)    /*!< Ready or command in progress */
/**
 * @brief QSPI Info
 * */
typedef struct QSPI_Mem_Info_s{
  unsigned long FlashSize;          /*!< Size of the flash                         */
  unsigned long SectorSize;    /*!< Size of sectors for the erase operation   */
  unsigned long SubSectorSize;
  unsigned long PageSize;       /*!< Size of pages for the program operation   */

  unsigned char DumyCiclesRead;
  unsigned char DumyCiclesReadQuad;
  unsigned char DumyCiclesReadDtr;
  unsigned char DumyCiclesReadQuadDtr;

  unsigned long BulkEraseMaxTime;
  unsigned long SectorEraseMaxTime;
  unsigned long SubSectorEraseMaxTime;
} QSPI_Mem_Info_t;

typedef struct Qspi_Mem_s
{
	unsigned char qspi_nr;
	QSPI_Mem_Info_t *dev_info;
}Qspi_Mem_t;

typedef struct n25qxxx_s
{
	unsigned char 	(*init)					(Qspi_Mem_t *param);
	unsigned char 	(*de_init)				(Qspi_Mem_t *param);
	unsigned char 	(*read)					(Qspi_Mem_t *param, unsigned char* pData, unsigned long ReadAddr, unsigned long Size);
	unsigned char 	(*write)				(Qspi_Mem_t *param, unsigned char* pData, unsigned long WriteAddr, unsigned long Size);
	unsigned int 	(*read_block)			(void *param, void* pData, unsigned long block, unsigned int nblks);
	unsigned int 	(*write_block)			(void *param, void* pData, unsigned long block, unsigned int nblks);
	unsigned char 	(*flush_block)			(void *param);
	void 			(*ioctl_block)			(void *param, unsigned int  command,  unsigned int *buffer);
	unsigned char 	(*erase_block)			(Qspi_Mem_t *param, unsigned long BlockAddress);
	unsigned char 	(*erase_chip)			(Qspi_Mem_t *param);
	unsigned char 	(*get_status)			(Qspi_Mem_t *param);
	unsigned char 	(*memory_mapped_mode)	(Qspi_Mem_t *param);
}n25qxxx_t;

#endif /* LIB_API_QSPI_DEF_H_ */
