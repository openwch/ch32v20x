/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2019/11/05
 * Description        : Upgrade slave application main function and task system initialization
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* Header file contains */
#include "CONFIG.h"
#include "HAL.h"
#include "Peripheral.h"
#include "OTA.h"
#include "OTAprofile.h"

/* 记录当前的Image */
unsigned char CurrImageFlag = 0xff;

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

/* Note: The operation of Flash after the program is upgraded must be performed first 
 * without turning on any interruption to prevent operation interruption and failure 
 */
/*********************************************************************
 * @fn      ReadImageFlag
 *
 * @brief   Read the iMage logo of the current program. 
 *          If the DataFlash is empty, it will be Imagea by default.
 *
 * @return  none
 */
void ReadImageFlag(void)
{
    OTADataFlashInfo_t p_image_flash;

    FLASH_read(OTA_DATAFLASH_ADD, (uint8_t *)&p_image_flash, 4);
    CurrImageFlag = p_image_flash.ImageFlag;

    /* The program is executed for the first time, or it has not been updated, 
     * and the DataFLASH is erased after being updated in the future 
     */
    if((p_image_flash.flag[0] != 0x5A) || (p_image_flash.flag[1] != 0x5A) || (p_image_flash.flag[2] != 0x5A))
    {
        CurrImageFlag = IMAGE_A_FLAG;
    }

    PRINT("Image Flag %02x\n", CurrImageFlag);

    if(CurrImageFlag == IMAGE_A_FLAG)
    {
        PRINT("jump App \n");
        Delay_Ms(5);
        jumpApp();
    }
}

/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   Main loop
 *
 * @return  none
 */
__attribute__((section(".highcode")))
__attribute__((noinline))
void Main_Circulation(void)
{
    while(1)
    {
        TMOS_SystemProcess();
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main function
 *
 * @return  none
 */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
#ifdef DEBUG
    USART_Printf_Init(115200);
#endif
    PRINT("%s\n", VER_LIB);
    ReadImageFlag();
    if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET)
    {
        // Soft reset does not jump app app
    }
    else
    {
        if(CurrImageFlag == IMAGE_OTA_FLAG)
        {
            PRINT("jump App \n");
            Delay_Ms(5);
            jumpApp();
        }
    }
    WCHBLE_Init();
    HAL_Init();
    GAPRole_PeripheralInit();
    Peripheral_Init();
    Main_Circulation();
}

/******************************** endfile @ main ******************************/
