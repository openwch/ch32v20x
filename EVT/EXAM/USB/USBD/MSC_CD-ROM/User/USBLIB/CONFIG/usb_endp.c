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
#include <SW_CDROM.h>
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "usb_prop.h"

uint16_t USB_Rx_Cnt = 0;
 __attribute__ ((aligned(4))) uint8_t CDROM_EP3_Pack_Buffer[DEF_CDROM_PACK_64];

/*********************************************************************
 * @fn      EP2_IN_Callback
 *
 * @brief  Endpoint 2 IN.
 *
 * @return  none
 */
void EP2_IN_Callback (void)
{ 
	CDROM_In_EP_Deal( );
}

/*********************************************************************
 * @fn      EP3_OUT_Callback
 *
 * @brief  Endpoint 3 OUT.
 *
 * @return  none
 */
void EP3_OUT_Callback (void)
{
    USB_Rx_Cnt = USB_SIL_Read(ENDP3, CDROM_EP3_Pack_Buffer);
    CDROM_Out_EP_Deal( CDROM_EP3_Pack_Buffer, USB_Rx_Cnt );
    SetEPRxValid(ENDP3);
}
