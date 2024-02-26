/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb_desc.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/08/08
 * Description        : Descriptors for Virtual Com Port Demo.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 
#include "usb_lib.h"
#include "usb_desc.h"

/* USB Device Descriptors */
const uint8_t  USBD_DeviceDescriptor[] = { 
    USBD_SIZE_DEVICE_DESC,           // bLength
    0x01,                           // bDescriptorType
    0x10, 0x01,                     // bcdUSB
    0xFF,                           // bDeviceClass
    0x80,                           // bDeviceSubClass
    0x55,                           // bDeviceProtocol
    DEF_USBD_UEP0_SIZE,             // bMaxPacketSize0
    0x86, 0x1A,                     // idVendor
    0x37, 0x55,                     // idProduct
    0x00, 0x01,                     // bcdDevice
    0x01,                           // iManufacturer
    0x02,                           // iProduct
    0x03,                           // iSerialNumber
    0x01,                           // bNumConfigurations
};

/* USB Configration Descriptors */
const uint8_t  USBD_ConfigDescriptor[] = { 
    /* Configuration Descriptor */
    0x09,                           // bLength
    0x02,                           // bDescriptorType
    USBD_SIZE_CONFIG_DESC & 0xFF, USBD_SIZE_CONFIG_DESC >> 8, // wTotalLength
    0x01,                           // bNumInterfaces
    0x01,                           // bConfigurationValue
    0x00,                           // iConfiguration
    0x80,                           // bmAttributes: Bus Powered; Remote Wakeup
    0x32,                           // MaxPower: 100mA

    0x09,                          // bLength
    0x04,                          // bDescriptorType (Interface)
    0x00,                          // bInterfaceNumber 0
    0x00,                          // bAlternateSetting
    0x04,                          // bNumEndpoints 6
    0xFF,                          // bInterfaceClass
    0x80,                          // bInterfaceSubClass
    0x55,                          // bInterfaceProtocol
    0x00,                          // iInterface (String Index)

    0x07,                          // bLength
    0x05,                          // bDescriptorType (Endpoint)
    0x84,                          // bEndpointAddress: IN Endpoint 4
    0x02,                          // bmAttributes (Bulk)
    0x40, 0x00,                    // wMaxPacketSize 64
    0x00,                          // bInterval 0 (unit depends on device speed)

    0x07,                          // bLength
    0x05,                          // bDescriptorType (Endpoint)
    0x03,                          // bEndpointAddress: OUT Endpoint 3
    0x02,                          // bmAttributes (Bulk)
    0x40, 0x00,                    // wMaxPacketSize 64
    0x00,                          // bInterval 0 (unit depends on device speed)

    0x07,                          // bLength
    0x05,                          // bDescriptorType (Endpoint)
    0x82,                          // bEndpointAddress: IN Endpoint 2
    0x02,                          // bmAttributes (Bulk)
    0x40, 0x00,                    // wMaxPacketSize 64
    0x00,                          // bInterval 0 (unit depends on device speed)

    0x07,                          // bLength
    0x05,                          // bDescriptorType (Endpoint)
    0x01,                          // bEndpointAddress: OUT Endpoint 1
    0x02,                          // bmAttributes (Bulk)
    0x40, 0x00,                    // wMaxPacketSize 64
    0x00,                          // bInterval 0 (unit depends on device speed)
};

/* USB String Descriptors */
const uint8_t USBD_StringLangID[USBD_SIZE_STRING_LANGID] = {
	USBD_SIZE_STRING_LANGID,
	USB_STRING_DESCRIPTOR_TYPE,
	0x09,
	0x04 
};

/* USB Device String Vendor */
const uint8_t USBD_StringVendor[USBD_SIZE_STRING_VENDOR] = {
	USBD_SIZE_STRING_VENDOR,    
	USB_STRING_DESCRIPTOR_TYPE,           
	'w',0,'c',0,'h',0,'.',0,'c',0,'n',0
};

/* USB Device String Product */
const uint8_t USBD_StringProduct[USBD_SIZE_STRING_PRODUCT] = {
	USBD_SIZE_STRING_PRODUCT,         
	USB_STRING_DESCRIPTOR_TYPE,        
    'C', 0, 'H', 0, '3', 0, '2', 0, 'V', 0, '2', 0 , '0', 0, 'x', 0
};

/* USB Device String Serial */
uint8_t USBD_StringSerial[USBD_SIZE_STRING_SERIAL] = {
	USBD_SIZE_STRING_SERIAL,          
	USB_STRING_DESCRIPTOR_TYPE,                   
	'0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0 , '6', 0, '7', 0, '8', 0, '9', 0
};






