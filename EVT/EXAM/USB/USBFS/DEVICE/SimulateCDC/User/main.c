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
 *Example routine to emulate a simulate USB-CDC Device, USE USART2(PA2/PA3);
 *Please note: This code uses the default serial port 1 for debugging,
 *if you need to modify the debugging serial port, please do not use USART2
*/

#include "ch32v20x_usbfs_device.h"
#include "debug.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	Delay_Init( );
	USART_Printf_Init( 115200 );
	printf("SystemClk:%d\r\n",SystemCoreClock);
	RCC_Configuration( );

	/* Tim2 init */
    TIM2_Init( );

	/* Usart1 init */
    UART2_Init( 1, DEF_UARTx_BAUDRATE, DEF_UARTx_STOPBIT, DEF_UARTx_PARITY );

    /* USB20 device init */
    USBFS_RCC_Init( );
    USBFS_Device_Init( ENABLE );

    printf( "main\r\n" );

	while(1)
	{
        UART2_DataRx_Deal( );
        UART2_DataTx_Deal( );
	}
}
