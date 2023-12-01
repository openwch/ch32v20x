/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb_endp.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/08/08
 * Description        : Endpoint routines
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "usb_prop.h"
#include "UART.h"

uint8_t USBD_Endp3_Busy;
uint16_t USB_Rx_Cnt = 0; 
uint8_t HID_Buffer[DEF_USBD_MAX_PACK_SIZE];
/*********************************************************************
 * @fn      EP1_IN_Callback
 *
 * @brief  Endpoint 1 IN.
 *
 * @return  none
 */
void EP1_IN_Callback (void)
{ 
	
}

/*********************************************************************
 * @fn      EP2_OUT_Callback
 *
 * @brief  Endpoint 2 OUT.
 *
 * @return  none
 */
void EP2_OUT_Callback (void)
{ 
	uint32_t len;
    len = GetEPRxCount( EP2_OUT & 0x7F );
    PMAToUserBufferCopy( &UART2_Tx_Buf[ ( Uart.Tx_LoadNum * DEF_USB_FS_PACK_LEN ) ], GetEPRxAddr( EP2_OUT & 0x7F ), len );
    Uart.Tx_PackLen[ Uart.Tx_LoadNum ] = len;
    Uart.Tx_LoadNum++;
    if( Uart.Tx_LoadNum >= DEF_UARTx_TX_BUF_NUM_MAX )
    {
        Uart.Tx_LoadNum = 0x00;
    }
    Uart.Tx_RemainNum++;

	if( Uart.Tx_RemainNum >= ( DEF_UARTx_TX_BUF_NUM_MAX - 2 ) )
    {
        Uart.USB_Down_StopFlag = 0x01;
    }
    else
    {
        SetEPRxValid( ENDP2 );
    }
}


/*********************************************************************
 * @fn      EP3_IN_Callback
 *
 * @brief  Endpoint 3 IN.
 *
 * @return  none
 */
void EP3_IN_Callback (void)
{ 
	USBD_Endp3_Busy = 0;
	Uart.USB_Up_IngFlag = 0x00;
    
}

/*********************************************************************
 * @fn      EP4_IN_Callback
 *
 * @brief  Endpoint 4 IN.
 *
 * @return  none
 */
void EP4_IN_Callback (void)
{ 
    
}

/*********************************************************************
 * @fn      EP4_OUT_Callback
 *
 * @brief  Endpoint 4 OUT.
 *
 * @return  none
 */
void EP4_OUT_Callback (void)
{ 
    uint16_t i;
    USB_Rx_Cnt = USB_SIL_Read(EP4_IN,HID_Buffer);
    for ( i = 0; i < USB_Rx_Cnt; i++)
    {
        HID_Buffer[i] = ~HID_Buffer[i];
    }
    USB_SIL_Write( EP4_OUT, HID_Buffer, USB_Rx_Cnt );
    SetEPTxValid( ENDP4 );
    SetEPRxValid( ENDP4 );
}

/*********************************************************************
 * @fn      USBD_ENDPx_DataUp
 *
 * @brief  USBD ENDPx DataUp Function
 * 
 * @param   endp - endpoint num.
 *          *pbuf - A pointer points to data.
 *          len - data length to transmit.
 * 
 * @return  data up status.
 */
uint8_t USBD_ENDPx_DataUp( uint8_t endp, uint8_t *pbuf, uint16_t len )
{
	if( endp == ENDP3 )
	{
		if (USBD_Endp3_Busy)
		{
			return USB_ERROR;
		}
		USB_SIL_Write( EP3_IN, pbuf, len );
		USBD_Endp3_Busy = 1;
		SetEPTxStatus( ENDP3, EP_TX_VALID );
	}
	else
	{
		return USB_ERROR;
	}
	return USB_SUCCESS;
}
