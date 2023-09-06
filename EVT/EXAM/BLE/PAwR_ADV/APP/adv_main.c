/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2020/08/06
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* Header file contains */
#include "CONFIG.h"
#include "hal.h"
#include "adv.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x83, 0x82, 0x82};
#endif

/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   Main loop
 *
 * @return  none
 */
__attribute__((section(".highcode")))
__attribute__((noinline)) void
Main_Circulation(void)
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
    PRINT("start.%s\n", __TIME__);
    PRINT("%s\n", VER_LIB);
    WCHBLE_Init();
    HAL_Init();
    GAPRole_PeripheralInit();
    GAPRole_CentralInit();
    Central_Init();
    Periodic_Init();
    Main_Circulation();
}

/******************************** endfile @ main ******************************/
