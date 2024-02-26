/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *IAP upgrade routine:
 *Support serial port and USB for FLASH burning
 *
 *1. The routine needs to install the CH372 driver.
 *Note: FLASH operation keeps the frequency below 100Mhz, it is recommended that the main
 *frequency of IAP be below 100Mhz
 *
 */

#include "debug.h"
#include "ch32v20x_usbfs_device.h"
#include "iap.h"
#include "usb_istr.h"
#include "usb_desc.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
extern u8 End_Flag;
/*********************************************************************
 * @fn      IAP_2_APP
 *
 * @brief   IAP_2_APP program.
 *
 * @return  none
 */
void IAP_2_APP(void) {
    USB_Port_Set(DISABLE, DISABLE);
    USBFSH->HOST_CTRL = 0x00;
    USBFSD->BASE_CTRL = 0x00;
    USBFSD->INT_EN = 0x00;
    USBFSD->UEP2_3_MOD = 0x00;
    USBFSD->BASE_CTRL |= (1 << 1) | (1 << 2);
    Delay_Ms(50);
    printf("jump APP\r\n");
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    USART_DeInit(USART3);
    NVIC_DisableIRQ( USBFS_IRQn );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,DISABLE);
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_USBFS, DISABLE );
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
    Delay_Ms(10);
    NVIC_EnableIRQ(Software_IRQn);
    NVIC_SetPendingIRQ(Software_IRQn);
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void) {
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    Set_USBConfig();
    USB_Init();
    USB_Port_Set(DISABLE, DISABLE);
    Delay_Ms(700);
    USB_Port_Set(ENABLE, ENABLE);
    USB_Interrupts_Config();

    USBFS_Init();
    USART3_CFG(57600);
    while(1)
    {
        EP2_RecData_Deal();
        if( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET) {
            UART_Rx_Deal();
        }
        if (End_Flag)
        {
            Delay_Ms(10);
            IAP_2_APP();
            while(1);
        }
    }
}
