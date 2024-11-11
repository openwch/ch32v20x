/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2022/06/22
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* Header file contains */
#include "CONFIG.h"
#include "MESH_LIB.h"
#include "HAL.h"
#include "app_mesh_config.h"
#include "app.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];
#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif

/*********************************************************************
 * @fn      bt_mesh_lib_init
 *
 * @brief   mesh ø‚≥ı ºªØ
 *
 * @return  state
 */
uint8_t bt_mesh_lib_init(void)
{
    uint8_t ret;

    if(tmos_memcmp(VER_MESH_LIB, VER_MESH_FILE, strlen(VER_MESH_FILE)) == FALSE)
    {
        PRINT("mesh head file error...\n");
        while(1);
    }

    ret = RF_RoleInit();

#if((CONFIG_BLE_MESH_PROXY) ||   \
    (CONFIG_BLE_MESH_PB_GATT) || \
    (CONFIG_BLE_MESH_OTA))
    ret = GAPRole_PeripheralInit();
#endif /* PROXY || PB-GATT || OTA */

#if(CONFIG_BLE_MESH_PROXY_CLI)
    ret = GAPRole_CentralInit();
#endif /* CONFIG_BLE_MESH_PROXY_CLI */

    MeshTimer_Init();
    MeshDeamon_Init();
    ble_sm_alg_ecc_init();

#if(CONFIG_BLE_MESH_IV_UPDATE_TEST)
    bt_mesh_iv_update_test(TRUE);
#endif
    return ret;
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
    USART_Printf_Init( 115200 );
#endif
    PRINT("%s\n", VER_LIB);
    PRINT("%s\n", VER_MESH_LIB);
    WCHBLE_Init();
    HAL_Init();
    bt_mesh_lib_init();
    App_Init();
    Main_Circulation();
}

/******************************** endfile @ main ******************************/
