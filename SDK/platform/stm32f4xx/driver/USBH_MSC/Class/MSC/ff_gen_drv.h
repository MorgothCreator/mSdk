/**
  ******************************************************************************
  * @file    ff_gen_drv.h 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    08-May-2015
  * @brief   Header for ff_gen_drv.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FF_GEN_DRV_H
#define __FF_GEN_DRV_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "lib/fat_fs/inc/diskio.h"
#include "lib/fat_fs/inc/ff.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  Disk IO Driver structure definition  
  */ 
typedef struct
{
  DSTATUS (*disk_initialize) (BYTE);                     /*!< Initialize Disk Drive                     */
  DSTATUS (*disk_status)     (BYTE);                     /*!< Get Disk Status                           */
  DRESULT (*disk_read)       (BYTE, BYTE*, DWORD, UINT);       /*!< Read Sector(s)                            */
#if _USE_WRITE == 1 
  DRESULT (*disk_write)      (BYTE, const BYTE*, DWORD, UINT); /*!< Write Sector(s) when _USE_WRITE = 0       */
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1  
  DRESULT (*disk_ioctl)      (BYTE, BYTE, void*);              /*!< I/O control operation when _USE_IOCTL = 1 */
#endif /* _USE_IOCTL == 1 */

}Diskio_drvTypeDef;

/** 
  * @brief  Global Disk IO Drivers structure definition  
  */ 
typedef struct
{ 
  unsigned char                 is_initialized[_VOLUMES];
  Diskio_drvTypeDef       *drv[_VOLUMES];
  unsigned char                 lun[_VOLUMES];
  volatile unsigned char            nbr;

}Disk_drvTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
unsigned char FATFS_LinkDriverEx(Diskio_drvTypeDef *drv, char *path, unsigned char lun);
unsigned char FATFS_LinkDriver(Diskio_drvTypeDef *drv, char *path);
unsigned char FATFS_UnLinkDriver(char *path);
unsigned char FATFS_LinkDriverEx(Diskio_drvTypeDef *drv, char *path, BYTE lun);
unsigned char FATFS_UnLinkDriverEx(char *path, BYTE lun);
unsigned char FATFS_GetAttachedDriversNbr(void);

#ifdef __cplusplus
}
#endif

#endif /* __FF_GEN_DRV_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

