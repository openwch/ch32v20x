/********************************** (C) COPYRIGHT  *******************************
 * File Name          : usb_hid.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/29
 * Description        : 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


/********************************************************************************/
/* Header File */
#include "usb_host_config.h"

/*********************************************************************
 * @fn      HID_GetHidDesr
 *
 * @brief   Get USB device report descriptor.
 *
 * @para    num: Interface number.
 *          pbuf: Buffer for saving descriptor.
 *          len: Descriptor length.
 *
 * @return  none
 */
uint8_t HID_GetHidDesr( uint8_t ep0_size, uint8_t intf_num, uint8_t *pbuf, uint16_t *plen )
{
    memcpy( pUSBFS_SetupRequest, SetupGetHidDes, sizeof( USB_SETUP_REQ ) );
    pUSBFS_SetupRequest->wIndex = (uint16_t)intf_num;
    pUSBFS_SetupRequest->wLength = *plen;
    return USBFSH_CtrlTransfer( ep0_size, pbuf, plen );
}

/*********************************************************************
 * @fn      HID_SetReport
 *
 * @brief   Set report.
 *
 * @para    report: Desired report value.
 *          len: Report length.
 *          index: USB port num
 *
 * @return  none
 */
uint8_t HID_SetReport( uint8_t ep0_size, uint8_t intf_num, uint8_t *pbuf, uint16_t *plen )
{
    memcpy( pUSBFS_SetupRequest, SetupSetReport, sizeof( USB_SETUP_REQ ) );
    pUSBFS_SetupRequest->wIndex = (uint16_t)intf_num;
    return USBFSH_CtrlTransfer( ep0_size, pbuf, plen );
}

/*********************************************************************
 * @fn      HID_SetIdleSpeed
 *
 * @brief   Set idle.
 *
 * @para    speed: Report ID.
 *          index: Interface number.
 *
 * @return  none
 */
uint8_t HID_SetIdle( uint8_t ep0_size, uint8_t intf_num, uint8_t duration, uint8_t reportid )
{
    memcpy( pUSBFS_SetupRequest, SetupSetidle, sizeof( USB_SETUP_REQ ) );
    pUSBFS_SetupRequest->wValue = ( (uint16_t)duration << 8 ) | reportid;
    pUSBFS_SetupRequest->wIndex = (uint16_t)intf_num;
    return USBFSH_CtrlTransfer( ep0_size, NULL, NULL );
}
