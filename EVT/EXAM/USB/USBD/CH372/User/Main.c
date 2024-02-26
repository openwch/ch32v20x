/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/08/08
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 

/*
 *@Note
  This routine demonstrates the use of the USBD to emulate a custom device(CH372 like device),
  with endpoints 1/3 downlinking data and uploading it via endpoints 2/4/6 respectively
  where endpoint 1/2 is implemented via a ring buffer and data is not inverted,and endpoints
  3/4  is directly copied and inverted for upload
  The device can be operated using Bushund or other upper computer software.
*/

#include "debug.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "usb_prop.h"

/* Global define */


/* Global Variable */    

/*********************************************************************
 * @fn      Var_Init
 *
 * @brief   Software parameter initialization
 *
 * @return  none
 */
void Var_Init(void)
{
    uint16_t i;
    RingBuffer_Comm.LoadPtr = 0;
    RingBuffer_Comm.StopFlag = 0;
    RingBuffer_Comm.DealPtr = 0;
    RingBuffer_Comm.RemainPack = 0;
    for(i=0; i<DEF_Ring_Buffer_Max_Blks; i++)
    {
        RingBuffer_Comm.PackLen[i] = 0;
    }
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{   
    uint8_t ret;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("USBD CH372 Device Demo\r\n");

    Set_USBConfig();
    USB_Init();
    USB_Interrupts_Config();
    
    while(1)
    {
        if( bDeviceState == CONFIGURED )
        {
            /* Data Transfer */
            if(RingBuffer_Comm.RemainPack)
            {
                ret = USBD_ENDPx_DataUp(ENDP2, &EP1_Data_Buffer[(RingBuffer_Comm.DealPtr) * DEF_USBD_MAX_PACK_SIZE], RingBuffer_Comm.PackLen[RingBuffer_Comm.DealPtr]);
                if(ret == USB_SUCCESS)
                {
                    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
                    RingBuffer_Comm.RemainPack--;
                    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
                    RingBuffer_Comm.DealPtr++;
                    if(RingBuffer_Comm.DealPtr == DEF_Ring_Buffer_Max_Blks)
                    {
                        RingBuffer_Comm.DealPtr = 0;
                    }
                }
            }

            /* Monitor whether the remaining space is available for further downloads */
            if(RingBuffer_Comm.RemainPack < (DEF_Ring_Buffer_Max_Blks - DEF_RING_BUFFER_RESTART))
            {
                if(RingBuffer_Comm.StopFlag)
                {
                    printf("USB ring buffer full, stop receiving further data.\n");
                    RingBuffer_Comm.StopFlag = 0;
                    SetEPRxValid(ENDP1);
                }
            }
        }
    }
}







