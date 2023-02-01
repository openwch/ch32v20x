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
uint8_t USBD_Endp2_Busy;
u16 USB_Rx_Cnt=0; 

/*********************************************************************
 * @fn      EP1_OUT_Callback
 *
 * @brief  Endpoint 1 OUT.
 *
 * @return  none
 */
void EP1_OUT_Callback (void)
{ 
	PMAToUserBufferCopy( &Data_Buffer[(RingBuffer_Comm.LoadPtr) * DEF_USBD_MAX_PACK_SIZE], GetEPRxAddr( ENDP1 ), GetEPRxCount( ENDP1 ) );
	RingBuffer_Comm.PackLen[RingBuffer_Comm.LoadPtr] = DEF_USBD_MAX_PACK_SIZE;
	RingBuffer_Comm.LoadPtr ++;
	if(RingBuffer_Comm.LoadPtr == DEF_Ring_Buffer_Max_Blks)
	{
		RingBuffer_Comm.LoadPtr = 0;
	}
	RingBuffer_Comm.RemainPack ++;
	if(RingBuffer_Comm.RemainPack >= DEF_Ring_Buffer_Max_Blks-DEF_RING_BUFFER_REMINE)
	{
		SetEPRxStatus(ENDP1,EP_RX_NAK);
		RingBuffer_Comm.StopFlag = 1;
	}
	else
	{
		SetEPRxValid(ENDP1);
	}

}

/*********************************************************************
 * @fn      EP2_IN_Callback
 *
 * @brief  Endpoint 2 IN.
 *
 * @return  none
 */
void EP2_IN_Callback (void)
{ 
	USBD_Endp2_Busy = 0;
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
	if( endp == ENDP2 )
	{
		if (USBD_Endp2_Busy)
		{
			return USB_ERROR;
		}
		USB_SIL_Write( EP2_IN, pbuf, len );
		USBD_Endp2_Busy = 1;
		SetEPTxStatus( ENDP2, EP_TX_VALID );
	}
	else
	{
		return USB_ERROR;
	}
	return USB_SUCCESS;
}
