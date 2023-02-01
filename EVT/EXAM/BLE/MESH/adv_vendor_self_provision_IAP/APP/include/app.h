/********************************** (C) COPYRIGHT *******************************
 * File Name          : app.h
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2021/11/18
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef app_H
#define app_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/* 整个用户code区分成五块，8K，152K，152K，136K，后四块下面分别叫做imageIAP, imageA（APP）,imageB（OTA）和LIB */

/* FLASH定义 */
#define FLASH_PAGE_SIZE                    256
#define FLASH_BLOCK_SIZE                   4096
#define IMAGE_SIZE                         152 * 1024

/* imageIAP定义 */
#define IMAGE_IAP_FLAG                     0x03
#define IMAGE_IAP_START_ADD                0x08000000
#define IMAGE_IAP_SIZE                     8 * 1024

/* imageA定义 */
#define IMAGE_A_FLAG                       0x01
#define IMAGE_A_START_ADD                  (IMAGE_IAP_START_ADD + IMAGE_IAP_SIZE)
#define IMAGE_A_SIZE                       IMAGE_SIZE

/* imageB定义 */
#define IMAGE_B_FLAG                       0x02
#define IMAGE_B_START_ADD                  (IMAGE_A_START_ADD + IMAGE_A_SIZE)
#define IMAGE_B_SIZE                       IMAGE_SIZE

/* 存放在DataFlash地址，不能占用蓝牙的位置 */
#define OTA_DATAFLASH_ADD                  0x08077000

/* 存放在DataFlash里的OTA信息 */
typedef struct
{
    unsigned char ImageFlag; //记录的当前的image标志
    unsigned char flag[3];
} OTADataFlashInfo_t;
/******************************************************************************/

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
