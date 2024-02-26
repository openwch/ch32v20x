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
 *(1),Example routine to emulate a simulate USB-CDC Device, USE USART2(PA2/PA3),
 *Please note: This code uses the default serial port 1 for debugging,
 *if you need to modify the debugging serial port, please do not use USART2
 *(2),Merged HID devices, using endpoint 4 data downlink to fetch reverse upload;
*/

#include "UART.h"
#include "debug.h"
#include "usb_lib.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	Delay_Init(); 
	USART_Printf_Init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf("USBD SimulateCDC-HID Demo\r\n");
    
    RCC_Configuration( );
    TIM2_Init( );
    UART2_Init( 1, DEF_UARTx_BAUDRATE, DEF_UARTx_STOPBIT, DEF_UARTx_PARITY );
    
	Set_USBConfig(); 
    USB_Init();	    
 	USB_Interrupts_Config();    
 	
	while(1)
	{
        UART2_DataRx_Deal( );
        UART2_DataTx_Deal( );
	}
}







