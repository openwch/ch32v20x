/********************************** (C) COPYRIGHT *******************************
 * File Name          : hw_config.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/08/08
 * Description        : USB configuration file.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_istr.h"
#include "hw_config.h"
#include "usb_pwr.h"  
#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"	


void USBWakeUp_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USB_LP_CAN1_RX0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*******************************************************************************
 * @fn        USBWakeUp_IRQHandler
 *
 * @brief     This function handles USB wake up exception.
 *
 * @return    None
 */
void USBWakeUp_IRQHandler(void) 
{
	EXTI_ClearITPendingBit(EXTI_Line18);
} 

/*******************************************************************************
 * @fn           USBWakeUp_IRQHandler
 *
 * @brief  T     his function handles USB exception.
 *
 * @return None
 */
void USB_LP_CAN1_RX0_IRQHandler(void) 
{
	USB_Istr();
} 

/*******************************************************************************
 * @fn        Set_USBConfig
 *
 * @brief     Set_USBConfig .
 *
 * @return    None
 */
void Set_USBConfig( )
{
    RCC_ClocksTypeDef RCC_ClocksStatus={0};
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    if( RCC_ClocksStatus.SYSCLK_Frequency == 144000000 )
    {
        RCC_USBCLKConfig( RCC_USBCLKSource_PLLCLK_Div3 );
    }
    else if( RCC_ClocksStatus.SYSCLK_Frequency == 96000000 ) 
    {
        RCC_USBCLKConfig( RCC_USBCLKSource_PLLCLK_Div2 );
    }
    else if( RCC_ClocksStatus.SYSCLK_Frequency == 48000000 ) 
    {
        RCC_USBCLKConfig( RCC_USBCLKSource_PLLCLK_Div1 );
    }
#if defined(CH32V20x_D8W) || defined(CH32V20x_D8)
    else if ( RCC_ClocksStatus.SYSCLK_Frequency == 240000000 && RCC_USB5PRE_JUDGE() == SET )
    {
        RCC_USBCLKConfig( RCC_USBCLKSource_PLLCLK_Div5 );
    }
#endif
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);	 		 
}

/*******************************************************************************
 * @fn        Enter_LowPowerMode
 *
 * @brief     Enter low power mode.
 *
 * @return    None
 */
void Enter_LowPowerMode(void)
{  
	bDeviceState=SUSPENDED;
} 

/*******************************************************************************
 * @fn         Leave_LowPowerMode
 *
 * @brief      Leave low power mode.
 *
 * @return     None
 */
void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo=&Device_Info;
	if (pInfo->Current_Configuration!=0)bDeviceState=CONFIGURED; 
	else bDeviceState = ATTACHED; 
} 

/*******************************************************************************
 * @fn         USB_Interrupts_Config
 *
 * @brief      Configrate USB interrupt.
 *
 * @return     None
 */
void USB_Interrupts_Config(void)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_ClearITPendingBit(EXTI_Line18);
	EXTI_InitStructure.EXTI_Line = EXTI_Line18; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 	 

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitStructure);   
}	

/*******************************************************************************
 * @fn         USB_Port_Set
 *
 * @brief      Set USB IO port.
 *
 * @param      NewState: DISABLE or ENABLE.
 *             Pin_In_IPU: Enables or Disables intenal pull-up resistance.
 *
 * @return     None
 */
void USB_Port_Set(FunctionalState NewState, FunctionalState Pin_In_IPU)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	if(NewState) {
		_SetCNTR(_GetCNTR()&(~(1<<1)));
		GPIOA->CFGHR&=0XFFF00FFF;
		GPIOA->OUTDR&=~(3<<11);	//PA11/12=0
        GPIOA->CFGHR|=0X00044000; //float
	}
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  
		GPIOA->CFGHR&=0XFFF00FFF;
		GPIOA->OUTDR&=~(3<<11);	//PA11/12=0
		GPIOA->CFGHR|=0X00033000;	// LOW
	}
	
	if(Pin_In_IPU) (EXTEN->EXTEN_CTR) |= EXTEN_USBD_PU_EN; 
	else (EXTEN->EXTEN_CTR) &= ~EXTEN_USBD_PU_EN;
}  








