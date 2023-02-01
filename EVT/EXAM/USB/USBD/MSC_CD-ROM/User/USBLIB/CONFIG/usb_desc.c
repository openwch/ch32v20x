/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb_desc.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/11/25
 * Description        : USB Descriptors.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/ 
#include "usb_lib.h"
#include "usb_desc.h"

/* USB Device Descriptors */
const uint8_t  USBD_DeviceDescriptor[] =
{
    0x12,                                                                        // bLength
    0x01,                                                                        // bDescriptorType (Device)
    0x00, 0x02,                                                                  // bcdUSB 2.00
    0x00,                                                                        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,                                                                        // bDeviceSubClass
    0x00,                                                                        // bDeviceProtocol
    0x40,                                                                        // bMaxPacketSize0 64
    0x86, 0x1A,                                                                  // idVendor 0x1A86
    0x11, 0xFE,                                                                  // idProduct 0xFE11
    DEF_PROG_VERSION, 0x00,                                                      // bcdDevice 0.00
    0x01,                                                                        // iManufacturer (String Index)
    0x02,                                                                        // iProduct (String Index)
    0x00,                                                                        // iSerialNumber (String Index)
    0x01,                                                                        // bNumConfigurations 1
};

/* USB Configration Descriptors */
const uint8_t  USBD_ConfigDescriptor[] =
{
    /* Configuration Descriptor */
    0x09,                                                                        // bLength
    0x02,                                                                        // bDescriptorType (Configuration)
    0x20, 0x00,                                                                  // wTotalLength 32
    0x01,                                                                        // bNumInterfaces 1
    0x01,                                                                        // bConfigurationValue
    0x00,                                                                        // iConfiguration (String Index)
    0x80,                                                                        // bmAttributes
    0x32,                                                                        // bMaxPower 100mA

    /*****************************************************************/
    /* Interface Descriptor(CDROM) */
    0x09,                                                                        // bLength
    0x04,                                                                        // bDescriptorType (Interface)
    0x00,                                                                        // bInterfaceNumber 0
    0x00,                                                                        // bAlternateSetting
    0x02,                                                                        // bNumEndpoints 2
    0x08,                                                                        // bInterfaceClass
    0x06,                                                                        // bInterfaceSubClass
    0x50,                                                                        // bInterfaceProtocol
    0x00,                                                                        // iInterface (String Index)

    /* Endpoint Descriptor */
    0x07,                                                                        // bLength
    0x05,                                                                        // bDescriptorType (Endpoint)
    0x82,                                                                        // bEndpointAddress (IN/D2H)
    0x02,                                                                        // bmAttributes (Bulk)
    0x40, 0x00,                                                                  // wMaxPacketSize 64
    0x00,                                                                        // bInterval 0 (unit depends on device speed)

    /* Endpoint Descriptor */
    0x07,                                                                        // bLength
    0x05,                                                                        // bDescriptorType (Endpoint)
    0x03,                                                                        // bEndpointAddress (OUT/H2D)
    0x02,                                                                        // bmAttributes (Bulk)
    0x40, 0x00,                                                                  // wMaxPacketSize 64
    0x00,                                                                        // bInterval 0 (unit depends on device speed)
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
    'C',0,'H',0,'2',0,'0',0,'x',0,'C',0,'D',0,'-',0,'R',0,'O',0,'M',0,
};

/* USB Device String Serial */
const uint8_t USBD_StringSerial[USBD_SIZE_STRING_SERIAL] = {
	USBD_SIZE_STRING_SERIAL,          
	USB_STRING_DESCRIPTOR_TYPE,                   
	'W', 0, 'C', 0, 'H', 0, '3', 0, '2', 0
};




