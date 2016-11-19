/**
  ******************************************************************************
  * @file    stm32f4_discovery_sdio_sd.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   This file provides a set of functions needed to manage the SDIO SD
  *          Card memory mounted on STM324xG-EVAL evaluation board.
  *
  *
  *  @verbatim
  *
  *          ===================================================================
  *                                   How to use this driver
  *          ===================================================================
  *          It implements a high level communication layer for read and write
  *          from/to this memory. The needed STM32 hardware resources (SDIO and
  *          GPIO) are defined in stm32f4_discovery.h file, and the initialization is
  *          performed in SD_LowLevel_Init() function declared in stm32f4_discovery.c
  *          file.
  *          You can easily tailor this driver to any other development board,
  *          by just adapting the defines for hardware resources and
  *          SD_LowLevel_Init() function.
  *
  *          A - SD Card Initialization and configuration
  *          ============================================
  *            - To initialize the SD Card, use the SD_Init() function.  It
  *              Initializes the SD Card and put it into StandBy State (Ready
  *              for data transfer). This function provide the following operations:
  *
  *              1 - Apply the SD Card initialization process at 400KHz and check
  *                  the SD Card type (Standard Capacity or High Capacity). You
  *                  can change or adapt this frequency by adjusting the
  *                  "SDIO_INIT_CLK_DIV" define inside the stm32f4_discovery.h file.
  *                  The SD Card frequency (SDIO_CK) is computed as follows:
  *
  *                     +---------------------------------------------+
  *                     | SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) |
  *                     +---------------------------------------------+
  *
  *                  In initialization mode and according to the SD Card standard,
  *                  make sure that the SDIO_CK frequency don't exceed 400KHz.
  *
  *              2 - Get the SD CID and CSD data. All these information are
  *                  managed by the SDCardInfo structure. This structure provide
  *                  also ready computed SD Card capacity and Block size.
  *
  *              3 - Configure the SD Card Data transfer frequency. By Default,
  *                  the card transfer frequency is set to 24MHz. You can change
  *                  or adapt this frequency by adjusting the "SDIO_TRANSFER_CLK_DIV"
  *                  define inside the stm32f4_discovery.h file.
  *                  The SD Card frequency (SDIO_CK) is computed as follows:
  *
  *                     +---------------------------------------------+
  *                     | SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) |
  *                     +---------------------------------------------+
  *
  *                  In transfer mode and according to the SD Card standard,
  *                  make sure that the SDIO_CK frequency don't exceed 25MHz
  *                  and 50MHz in High-speed mode switch.
  *                  To be able to use a frequency higher than 24MHz, you should
  *                  use the SDIO peripheral in bypass mode. Refer to the
  *                  corresponding reference manual for more details.
  *
  *              4 -  Select the corresponding SD Card according to the address
  *                   read with the step 2.
  *
  *              5 -  Configure the SD Card in wide bus mode: 4-bits data.
  *
  *          B - SD Card Read operation
  *          ==========================
  *           - You can read SD card by using two function: SD_ReadBlock() and
  *             SD_ReadMultiBlocks() functions. These functions support only
  *             512-byte block length.
  *           - The SD_ReadBlock() function read only one block (512-byte). This
  *             function can transfer the data using DMA controller or using
  *             polling mode. To select between DMA or polling mode refer to
  *             "SD_DMA_MODE" or "SD_POLLING_MODE" inside the stm32f4_discovery_sdio_sd.h
  *             file and uncomment the corresponding line. By default the SD DMA
  *             mode is selected
  *           - The SD_ReadMultiBlocks() function read only mutli blocks (multiple
  *             of 512-byte).
  *           - Any read operation should be followed by two functions to check
  *             if the DMA Controller and SD Card status.
  *              - SD_ReadWaitOperation(): this function insure that the DMA
  *                controller has finished all data transfer.
  *              - SD_GetStatus(): to check that the SD Card has finished the
  *                data transfer and it is ready for data.
  *
  *           - The DMA transfer is finished by the SDIO Data End interrupt.
  *             User has to call the SD_ProcessIRQ() function inside the SDIO_IRQHandler()
  *             and SD_ProcessDMAIRQ() function inside the DMA2_Streamx_IRQHandler().
  *             Don't forget to enable the SDIO_IRQn and DMA2_Stream3_IRQn or
  *             DMA2_Stream6_IRQn interrupts using the NVIC controller.
  *
  *          C - SD Card Write operation
  *          ===========================
  *           - You can write SD card by using two function: SD_WriteBlock() and
  *             SD_WriteMultiBlocks() functions. These functions support only
  *             512-byte block length.
  *           - The SD_WriteBlock() function write only one block (512-byte). This
  *             function can transfer the data using DMA controller or using
  *             polling mode. To select between DMA or polling mode refer to
  *             "SD_DMA_MODE" or "SD_POLLING_MODE" inside the stm32f4_discovery_sdio_sd.h
  *             file and uncomment the corresponding line. By default the SD DMA
  *             mode is selected
  *           - The SD_WriteMultiBlocks() function write only mutli blocks (multiple
  *             of 512-byte).
  *           - Any write operation should be followed by two functions to check
  *             if the DMA Controller and SD Card status.
  *              - SD_ReadWaitOperation(): this function insure that the DMA
  *                controller has finished all data transfer.
  *              - SD_GetStatus(): to check that the SD Card has finished the
  *                data transfer and it is ready for data.
  *
  *           - The DMA transfer is finished by the SDIO Data End interrupt.
  *             User has to call the SD_ProcessIRQ() function inside the SDIO_IRQHandler()
  *             and SD_ProcessDMAIRQ() function inside the DMA2_Streamx_IRQHandler().
  *             Don't forget to enable the SDIO_IRQn and DMA2_Stream3_IRQn or
  *             DMA2_Stream6_IRQn interrupts using the NVIC controller.
  *
  *
  *          D - SD card status
  *          ==================
  *           - At any time, you can check the SD Card status and get the SD card
  *             state by using the SD_GetStatus() function. This function checks
  *             first if the SD card is still connected and then get the internal
  *             SD Card transfer state.
  *           - You can also get the SD card SD Status register by using the
  *             SD_SendSDStatus() function.
  *
  *          E - Programming Model (Selecting DMA for SDIO data Transfer)
  *          ============================================================
  *             Status = SD_Init(); // Initialization Step as described in section A
  *
  *             // SDIO Interrupt ENABLE
  *             NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  *             NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  *             NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  *             NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  *             NVIC_Init(&NVIC_InitStructure);
  *             // DMA2 STREAMx Interrupt ENABLE
  *             NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
  *             NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  *             NVIC_Init(&NVIC_InitStructure);
  *
  *             // Write operation as described in Section C
  *             Status = SD_WriteBlock(buffer, address, 512);
  *             Status = SD_WaitWriteOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK);
  *
  *             Status = SD_WriteMultiBlocks(buffer, address, 512, NUMBEROFBLOCKS);
  *             Status = SD_WaitWriteOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK);
  *
  *             // Read operation as described in Section B
  *             Status = SD_ReadBlock(buffer, address, 512);
  *             Status = SD_WaitReadOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK);
  *
  *             Status = SD_ReadMultiBlocks(buffer, address, 512, NUMBEROFBLOCKS);
  *             Status = SD_WaitReadOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK);
  *
  *             - Add the SDIO and DMA2 StreamX (3 or 6) IRQ Handlers:
  *                 void SDIO_IRQHandler(void)
  *                 {
  *                   SD_ProcessIRQ();
  *                 }
  *                 void SD_SDIO_DMA_IRQHANDLER(void)
  *                 {
  *                   SD_ProcessDMAIRQ();
  *                 }
  *
  *          F - Programming Model (Selecting Polling for SDIO data Transfer)
  *          ================================================================
  *            //Only SD Card Single Block operation are managed.
  *            Status = SD_Init(); // Initialization Step as described in section
  *
  *            // Write operation as described in Section C
  *            Status = SD_WriteBlock(buffer, address, 512);
  *
  *            // Read operation as described in Section B
  *            Status = SD_ReadBlock(buffer, address, 512);
  *
  *          STM32 SDIO Pin assignment
  *          =========================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+---------------+-------------+
  *          |  STM32 SDIO Pins            |     SD        |    Pin      |
  *          +-----------------------------+---------------+-------------+
  *          |      SDIO D2                |   D2          |    1        |
  *          |      SDIO D3                |   D3          |    2        |
  *          |      SDIO CMD               |   CMD         |    3        |
  *          |                             |   VCC         |    4 (3.3 V)|
  *          |      SDIO CLK               |   CLK         |    5        |
  *          |                             |   GND         |    6 (0 V)  |
  *          |      SDIO D0                |   D0          |    7        |
  *          |      SDIO D1                |   D1          |    8        |
  *          +-----------------------------+---------------+-------------+
  *
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    stm32f4_discovery_sdio_sd.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file provides a set of functions needed to manage the SDIO SD
  *          Card memory mounted on STM324xG-EVAL evaluation board.
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB, STM32F4DIS-CAM
  *          and STM32F4DIS-LCD modules.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include <interface/mmcsd_interface.h>
#include <stdio.h>
#include "main.h"
#include "board_init.h"
#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal_conf.h"
#include "driver/stm32f4xx_hal_sd.h"
#include "driver/stm32f4xx_hal_dma.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_cortex.h"
#include "driver/stm32f4xx_hal.h"
#include "driver/misc.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "interface/gpio_interface.h"
#include "lib/fat_fs/inc/ff.h"
#include "board_init.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM324xG_EVAL
  * @{
  */

/** @addtogroup STM324xG_EVAL_SDIO_SD
  * @brief      This file provides all the SD Card driver firmware functions.
  * @{
  */

/** @defgroup STM324xG_EVAL_SDIO_SD_Private_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup STM324xG_EVAL_SDIO_SD_Private_Defines
  * @{
  */

/**
  * @brief  SDIO Static flags, TimeOut, FIFO Address
  */
//#define NULL 0
#define SDIO_STATIC_FLAGS               ((unsigned long)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((unsigned long)0x00010000)

/**
  * @brief  Mask for errors Card Status R1 (OCR Register)
  */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((unsigned long)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((unsigned long)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((unsigned long)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((unsigned long)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((unsigned long)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((unsigned long)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((unsigned long)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((unsigned long)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((unsigned long)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((unsigned long)0x00200000)
#define SD_OCR_CC_ERROR                 ((unsigned long)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((unsigned long)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((unsigned long)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((unsigned long)0x00020000)
//#define SD_OCR_CID_CSD_OVERWRIETE       ((unsigned long)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((unsigned long)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((unsigned long)0x00004000)
#define SD_OCR_ERASE_RESET              ((unsigned long)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((unsigned long)0x00000008)
#define SD_OCR_ERRORBITS                ((unsigned long)0xFDFFE008)

/**
  * @brief  Masks for R6 Response
  */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((unsigned long)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((unsigned long)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((unsigned long)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((unsigned long)0x80100000)
#define SD_HIGH_CAPACITY                ((unsigned long)0x40000000)
#define SD_STD_CAPACITY                 ((unsigned long)0x00000000)
//#define SD_CHECK_PATTERN                ((unsigned long)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((unsigned long)0x0000FFFF)
#define SD_ALLZERO                      ((unsigned long)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((unsigned long)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((unsigned long)0x00010000)
#define SD_CARD_LOCKED                  ((unsigned long)0x02000000)

//#define SD_DATATIMEOUT                  ((unsigned long)0xFFFFFFFF)
#define SD_0TO7BITS                     ((unsigned long)0x000000FF)
#define SD_8TO15BITS                    ((unsigned long)0x0000FF00)
#define SD_16TO23BITS                   ((unsigned long)0x00FF0000)
#define SD_24TO31BITS                   ((unsigned long)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((unsigned long)0x01FFFFFF)

#define SD_HALFFIFO                     ((unsigned long)0x00000008)
#define SD_HALFFIFOBYTES                ((unsigned long)0x00000020)

/**
  * @brief  Command Class Supported
  */
#define SD_CCCC_LOCK_UNLOCK             ((unsigned long)0x00000080)
#define SD_CCCC_WRITE_PROT              ((unsigned long)0x00000040)
#define SD_CCCC_ERASE                   ((unsigned long)0x00000020)

/**
  * @brief  Following commands are SD Card Specific commands.
  *         SDIO_APP_CMD should be sent before sending these commands.
  */
#define SDIO_SEND_IF_COND               ((unsigned long)0x00000008)

/**
  * @}
  */

/** @defgroup STM324xG_EVAL_SDIO_SD_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM324xG_EVAL_SDIO_SD_Private_Variables
  * @{
  */

volatile unsigned long StopCondition = 0;
volatile HAL_SD_ErrorTypedef TransferError = SD_OK;
volatile unsigned long TransferEnd = 0, DMAEndOfTransfer = 0;
//HAL_SD_CardInfoTypedef SDCardInfo[1];

SDIO_InitTypeDef SDIO_InitStructure;
SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
SDIO_DataInitTypeDef SDIO_DataInitStructure;
/**
  * @}
  */
FATFS MmcSdFatFs;

#define PATH_BUF_SIZE   6
static char g_cCwdBuf0[PATH_BUF_SIZE] = "SD1:/";

DIR g_sDirObject;

static SD_HandleTypeDef uSdHandle;
HAL_SD_CardInfoTypedef uSdCardInfo[1];

Gpio_t *card_detect = NULL;
Gpio_t *status_led = NULL;

static bool card_detected = false;
//Gpio_t *Cs;

/**
  * @brief  Initializes the SD card device.
  * @retval SD status
  */
unsigned char BSP_SD_Init(void)
{
  unsigned char sd_state = MSD_OK;

  /* PLLSAI is dedicated to LCD periph. Do not use it to get 48MHz*/

  /* uSD device interface configuration */
  uSdHandle.Instance = SDIO;

  uSdHandle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
  uSdHandle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
  uSdHandle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.BusWide             = SDIO_BUS_WIDE_1B;
  uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
  uSdHandle.Init.ClockDiv            = SDIO_TRANSFER_CLK_DIV;

  /* Msp SD Detect pin initialization */
  //BSP_SD_Detect_MspInit(&uSdHandle, NULL);
  if(BSP_SD_IsDetected() != SD_PRESENT)   /* Check if SD card is present */
  {
    return MSD_ERROR_SD_NOT_PRESENT;
  }

  /* Msp SD initialization */
  BSP_SD_MspInit(&uSdHandle, NULL);

  /* HAL SD initialization */
  if(HAL_SD_Init(&uSdHandle, &uSdCardInfo[0]) != SD_OK)
  {
    sd_state = MSD_ERROR;
  }

  /* Configure SD Bus width */
  if(sd_state == MSD_OK)
  {
    /* Enable wide operation */
    if(HAL_SD_WideBusOperation_Config(&uSdHandle, SDIO_BUS_WIDE_4B) != SD_OK)
    {
      sd_state = MSD_ERROR;
    }
    else
    {
      sd_state = MSD_OK;
    }
  }
  return  sd_state;
}

/**
  * @brief  DeInitializes the SD card device.
  * @retval SD status
  */
unsigned char BSP_SD_DeInit(void)
{
  unsigned char sd_state = MSD_OK;

  uSdHandle.Instance = SDIO;

  /* HAL SD deinitialization */
  if(HAL_SD_DeInit(&uSdHandle) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  /* Msp SD deinitialization */
  uSdHandle.Instance = SDIO;
  BSP_SD_MspDeInit(&uSdHandle, NULL);

  return  sd_state;
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
unsigned char BSP_SD_IsDetected(void)
{

	volatile unsigned char status = SD_PRESENT;

	  /*!< Check GPIO to detect SD */
	  if (card_detect != NULL && gpio.in(card_detect) != 0)
	  {
	    status = SD_NOT_PRESENT;
	  }
	  return status;
}



/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read
  * @retval SD status
  */
unsigned char BSP_SD_ReadBlocks(unsigned long *pData, unsigned long long ReadAddr, unsigned long BlockSize, unsigned long NumOfBlocks)
{
  if(HAL_SD_ReadBlocks(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
unsigned char BSP_SD_WriteBlocks(unsigned long *pData, unsigned long long WriteAddr, unsigned long BlockSize, unsigned long NumOfBlocks)
{
  if(HAL_SD_WriteBlocks(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read
  * @retval SD status
  */
unsigned char BSP_SD_ReadBlocks_DMA(unsigned long *pData, unsigned long long ReadAddr, unsigned long BlockSize, unsigned long NumOfBlocks)
{
  unsigned char sd_state = MSD_OK;

  /* Read block(s) in DMA transfer mode */
  if(HAL_SD_ReadBlocks_DMA(&uSdHandle, pData, ReadAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    sd_state = MSD_ERROR;
  }

  /* Wait until transfer is complete */
  if(sd_state == MSD_OK)
  {
    if(HAL_SD_CheckReadOperation(&uSdHandle, (unsigned long)SD_DATATIMEOUT) != SD_OK)
    {
      sd_state = MSD_ERROR;
    }
    else
    {
      sd_state = MSD_OK;
    }
  }

  return sd_state;
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
unsigned char BSP_SD_WriteBlocks_DMA(unsigned long *pData, unsigned long long WriteAddr, unsigned long BlockSize, unsigned long NumOfBlocks)
{
  unsigned char sd_state = MSD_OK;

  /* Write block(s) in DMA transfer mode */
  if(HAL_SD_WriteBlocks_DMA(&uSdHandle, pData, WriteAddr, BlockSize, NumOfBlocks) != SD_OK)
  {
    sd_state = MSD_ERROR;
  }

  /* Wait until transfer is complete */
  if(sd_state == MSD_OK)
  {
    if(HAL_SD_CheckWriteOperation(&uSdHandle, (unsigned long)SD_DATATIMEOUT) != SD_OK)
    {
      sd_state = MSD_ERROR;
    }
    else
    {
      sd_state = MSD_OK;
    }
  }

  return sd_state;
}

/**
  * @brief  Erases the specified memory area of the given SD card.
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
unsigned char BSP_SD_Erase(unsigned long long StartAddr, unsigned long long EndAddr)
{
  if(HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != SD_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Initializes the SD MSP.
  * @param  hsd: SD handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
void BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params)
{
  static DMA_HandleTypeDef dma_rx_handle;
  static DMA_HandleTypeDef dma_tx_handle;
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable SDIO clock */
  __HAL_RCC_SDIO_CLK_ENABLE();

  /* Enable DMA2 clocks */
  __DMAx_TxRx_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_SDIO;

  /* GPIOC configuration */
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
#if 1
  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SDIO_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDIO_IRQn);

  /* Configure DMA Rx parameters */
  dma_rx_handle.Init.Channel             = SD_DMAx_Rx_CHANNEL;
  dma_rx_handle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  dma_rx_handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  dma_rx_handle.Init.MemInc              = DMA_MINC_ENABLE;
  dma_rx_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  dma_rx_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  dma_rx_handle.Init.Mode                = DMA_PFCTRL;
  dma_rx_handle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  dma_rx_handle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  dma_rx_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  dma_rx_handle.Init.MemBurst            = DMA_MBURST_INC4;
  dma_rx_handle.Init.PeriphBurst         = DMA_PBURST_INC4;

  dma_rx_handle.Instance = SD_DMAx_Rx_STREAM;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsd, hdmarx, dma_rx_handle);

  /* Deinitialize the stream for new transfer */
  HAL_DMA_DeInit(&dma_rx_handle);

  /* Configure the DMA stream */
  HAL_DMA_Init(&dma_rx_handle);

  /* Configure DMA Tx parameters */
  dma_tx_handle.Init.Channel             = SD_DMAx_Tx_CHANNEL;
  dma_tx_handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  dma_tx_handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  dma_tx_handle.Init.MemInc              = DMA_MINC_ENABLE;
  dma_tx_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  dma_tx_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  dma_tx_handle.Init.Mode                = DMA_PFCTRL;
  dma_tx_handle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  dma_tx_handle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  dma_tx_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  dma_tx_handle.Init.MemBurst            = DMA_MBURST_INC4;
  dma_tx_handle.Init.PeriphBurst         = DMA_PBURST_INC4;

  dma_tx_handle.Instance = SD_DMAx_Tx_STREAM;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hsd, hdmatx, dma_tx_handle);

  /* Deinitialize the stream for new transfer */
  HAL_DMA_DeInit(&dma_tx_handle);

  /* Configure the DMA stream */
  HAL_DMA_Init(&dma_tx_handle);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(SD_DMAx_Rx_IRQn);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(SD_DMAx_Tx_IRQn);
#endif
}

/**
  * @brief  DeInitializes the SD MSP.
  * @param  hsd: SD handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
void BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params)
{
    static DMA_HandleTypeDef dma_rx_handle;
    static DMA_HandleTypeDef dma_tx_handle;

    /* Disable NVIC for DMA transfer complete interrupts */
    HAL_NVIC_DisableIRQ(SD_DMAx_Rx_IRQn);
    HAL_NVIC_DisableIRQ(SD_DMAx_Tx_IRQn);

    /* Deinitialize the stream for new transfer */
    dma_rx_handle.Instance = SD_DMAx_Rx_STREAM;
    HAL_DMA_DeInit(&dma_rx_handle);

    /* Deinitialize the stream for new transfer */
    dma_tx_handle.Instance = SD_DMAx_Tx_STREAM;
    HAL_DMA_DeInit(&dma_tx_handle);

    /* Disable NVIC for SDIO interrupts */
    HAL_NVIC_DisableIRQ(SDIO_IRQn);

    /* DeInit GPIO pins can be done in the application
       (by surcharging this __weak function) */

    /* Disable SDIO clock */
    __HAL_RCC_SDIO_CLK_DISABLE();

    /* GPOI pins clock and DMA cloks can be shut down in the applic
       by surcgarging this __weak function */
}

/**
  * @brief  Handles SD card interrupt request.
  */
void BSP_SD_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle);
}

/**
  * @brief  Handles SD DMA Tx transfer interrupt request.
  */
void BSP_SD_DMA_Tx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uSdHandle.hdmatx);
}

/**
  * @brief  Handles SD DMA Rx transfer interrupt request.
  */
void BSP_SD_DMA_Rx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  *            @arg  SD_TRANSFER_ERROR: Data transfer error
  */
HAL_SD_TransferStateTypedef BSP_SD_GetStatus(void)
{
  return(HAL_SD_GetStatus(&uSdHandle));
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypedef *CardInfo)
{
  /* Get SD card Information */
  HAL_SD_Get_CardInfo(&uSdHandle, CardInfo);
}


void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{
	HAL_SD_CardInfoTypedef* _SdCtrlStruct = &uSdCardInfo[0];
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           //if(_SdCtrlStruct->CardType == MMCSD_CARD_SD) *buffer = _SdCtrlStruct->card->nBlks;
           //else if(_SdCtrlStruct->card->cardType == MMCSD_CARD_MMC) *buffer = extCsd.sec_count;
           //else *buffer = 0;
           *buffer = _SdCtrlStruct->CardCapacity / _SdCtrlStruct->CardBlockSize;

            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = _SdCtrlStruct->CardBlockSize;

            break;
        }
        default:
        {
            *buffer = 0;
            break;
        }

    }
}

//HAL_SD_ErrorTypedef SD_WriteMultiBlocks(unsigned char *writebuff, unsigned long WriteAddr, uint16_t BlockSize, unsigned long NumberOfBlocks)
//HAL_SD_ErrorTypedef SD_WriteBlock(unsigned char *writebuff, unsigned long WriteAddr, uint16_t BlockSize)
//HAL_SD_ErrorTypedef SD_ReadMultiBlocks(unsigned char *readbuff, unsigned long ReadAddr, uint16_t BlockSize, unsigned long NumberOfBlocks)
//HAL_SD_ErrorTypedef SD_ReadBlock(unsigned char *readbuff, unsigned long ReadAddr, uint16_t BlockSize)

unsigned int _mmcsd_read(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks)
{
	//HAL_SD_CardInfoTypedef *_SDCardInfo = (HAL_SD_CardInfoTypedef *)SdStruct;
	if(status_led)
		gpio.out(status_led, 1);
	HAL_SD_ErrorTypedef status = MSD_OK;
	status =  BSP_SD_ReadBlocks_DMA((unsigned long *)ptr, block << 9, 512, nblks);
	if (status == MSD_OK) {
		if(status_led)
			gpio.out(status_led, 0);
		return 1;
	} else {
		if(status_led)
			gpio.out(status_led, 0);
		return 0;
	}
}

unsigned int _mmcsd_write(void *SdStruct, void *ptr, unsigned long block, unsigned int nblks)
{
	//HAL_SD_CardInfoTypedef *_SDCardInfo = (HAL_SD_CardInfoTypedef *)SdStruct;
	if(status_led)
		gpio.out(status_led, 1);
	HAL_SD_ErrorTypedef status = MSD_OK;
	status =  BSP_SD_WriteBlocks_DMA((unsigned long *)ptr, block << 9, 512, nblks);
	if (status == MSD_OK) {
		if(status_led)
			gpio.out(status_led, 0);
		return 1;
	} else {
		if(status_led)
			gpio.out(status_led, 0);
		return 0;
	}
}



bool _mmcsd_idle(unsigned int unit_nr)
{
	if(card_detect != NULL)
	{
		gpio_idle(card_detect);
		if(card_detect->event.state_up)
		{
			card_detect->event.state_up = false;
			card_detected = false;
		}
	}
	if(card_detect == NULL || card_detect->event.state_dn)
	{
		HAL_SD_ErrorTypedef res = SD_OK;
		res =  BSP_SD_Init();
		if(res != SD_OK)
		{
			if(card_detect != NULL)
				card_detect->event.state_dn = false;
			//if(DebugCom)
				//UARTprintf(DebugCom,   "MMCSD%d ERROR initializing card.\n\r" , 0);
			card_detected = false;
			return false;
		}
		if(card_detect != NULL)
			card_detect->event.state_dn = false;
        MmcSdFatFs.drv_rw_func.DriveStruct = (void *)&uSdCardInfo;//SdStruct;
        MmcSdFatFs.drv_rw_func.drv_r_func = _mmcsd_read;
        MmcSdFatFs.drv_rw_func.drv_w_func = _mmcsd_write;
#if (_FFCONF == 82786)
        if(!f_mount(2, &MmcSdFatFs))
#else
        if(!f_mount(&MmcSdFatFs,"SD1:", 1))
#endif
        {
                    if(f_opendir(&g_sDirObject, g_cCwdBuf0) == FR_OK)
                    {
#ifdef MMCSD_DEBUG_EN
						if(DebugCom)
						{
							uart.printf(DebugCom,   "MMCSD%d drive %d mounted\n\r" , 0 , 0);
							uart.printf(DebugCom,   "MMCSD%d Fat fs detected\n\r" , 0);
							uart.printf(DebugCom, "MMCSD%d Fs type:                 " , 0);
							if(MmcSdFatFs.fs_type == FS_FAT12)	{
								uart.printf(DebugCom, "Fat12");}
							else if(MmcSdFatFs.fs_type == FS_FAT16){
								uart.printf(DebugCom, "Fat16");}
							else if(MmcSdFatFs.fs_type == FS_FAT32){
								uart.printf(DebugCom, "Fat32");}
							else if(MmcSdFatFs.fs_type == FS_EXFAT){
								uart.printf(DebugCom, "exFat");}
							else								{ 				uart.printf(DebugCom, "None");}
							uart.printf(DebugCom, "\n\r");
																				//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
							uart.printf(DebugCom, "MMCSD%d BytesPerSector:          %d \n\r",0, /*(int)g_sFatFs.s_size*/512);
							uart.printf(DebugCom, "MMCSD%d SectorsPerCluster:       %d \n\r",0, (int)MmcSdFatFs.csize);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																				//UARTprintf(DebugCom, "MMCSD%d NumberOfFats:            %d \n\r",0, (int)MmcSdFatFs.n_fats);
																				//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																				//UARTprintf(DebugCom, "MMCSD%d DataSectionBegin:        %d \n\r",0, (int)MmcSdFatFs.fatbase);
							uart.printf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r",0, (unsigned long)((unsigned long long)((unsigned long long)MmcSdFatFs.n_fatent * (unsigned long long)/*g_sFatFs.s_size*/512 *(unsigned long long)MmcSdFatFs.csize) / 1000000));
						}
#endif
			        	card_detected = true;
                    } else
                    {
                    	if(DebugCom)										uart.printf(DebugCom,   "MMCSD%d ERROR oppening path\n\r" , 0);
                    	card_detected = false;
                    }
        }
        else
        {
        	if(DebugCom)												uart.printf(DebugCom,   "MMCSD%d ERROR mounting disk\n\r" , 0);
        	card_detected = false;
        }
	}
	return card_detected;
}
/*#####################################################*/

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

void _mmcsd_init(unsigned int unit_nr, new_gpio* CardDetect, new_gpio* StatusLed)
{
	status_led = StatusLed;
	card_detect = CardDetect;
	return;
}


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

/**
  * @}
  */

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
