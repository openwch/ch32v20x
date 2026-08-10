/********************************** (C) COPYRIGHT *******************************
 * File Name          : MB_Operation_Flash.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "M0_Control_Library.h"

#define PAGE_WRITE_START_ADDR       ((uint32_t)0x08010000) /* Start from 64K */
#define PAGE_WRITE_HALF_END_ADDR    ((uint32_t)0x08010800) /* End at 66K */
#define PAGE_WRITE_END_ADDR         ((uint32_t)0x08011000) /* End at 68K */
#define FLASH_PAGE_SIZE             4096

extern Flash_Operation_Type  Flash_M;

void Flash_Operation_Init(Flash_Operation_Type *FlashM);//²Ù×÷Flash³õÊ¼»¯

void Flash_ProgramHalfWord(Flash_Operation_Type *FlashM ,uint32_t Address,uint16_t Data );
