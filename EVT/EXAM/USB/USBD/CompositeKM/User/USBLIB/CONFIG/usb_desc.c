/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb_desc.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2019/10/15
 * Description        : USB Descriptors.
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
    0x00,                           // bDeviceClass
    0x00,                           // bDeviceSubClass
    0x00,                           // bDeviceProtocol
    DEF_USBD_UEP0_SIZE,             // bMaxPacketSize0
    0x86, 0x1A,                     // idVendor
    0x00, 0xFE,                     // idProduct
    0x00, 0x01,                     // bcdDevice
    0x01,                           // iManufacturer
    0x02,                           // iProduct
    0x00,                           // iSerialNumber
    0x01,                           // bNumConfigurations
};

/* USB Configration Descriptors */
const uint8_t  USBD_ConfigDescriptor[] = { 
    /* Configuration Descriptor */
    0x09,                           // bLength
    0x02,                           // bDescriptorType
    USBD_SIZE_CONFIG_DESC & 0xFF, USBD_SIZE_CONFIG_DESC >> 8, // wTotalLength
    0x02,                           // bNumInterfaces
    0x01,                           // bConfigurationValue
    0x00,                           // iConfiguration
    0xA0,                           // bmAttributes: Bus Powered; Remote Wakeup
    0x32,                           // MaxPower: 100mA

    /* Interface Descriptor (Keyboard) */
    0x09,                           // bLength
    0x04,                           // bDescriptorType
    0x00,                           // bInterfaceNumber
    0x00,                           // bAlternateSetting
    0x01,                           // bNumEndpoints
    0x03,                           // bInterfaceClass
    0x01,                           // bInterfaceSubClass
    0x01,                           // bInterfaceProtocol: Keyboard
    0x00,                           // iInterface

    /* HID Descriptor (Keyboard) */
    0x09,                           // bLength
    0x21,                           // bDescriptorType
    0x11, 0x01,                     // bcdHID
    0x00,                           // bCountryCode
    0x01,                           // bNumDescriptors
    0x22,                           // bDescriptorType
    USBD_SIZE_REPORT_DESC_KB & 0xFF, USBD_SIZE_REPORT_DESC_KB >> 8, // wDescriptorLength

    /* Endpoint Descriptor (Keyboard) */
    0x07,                           // bLength
    0x05,                           // bDescriptorType
    0x81,                           // bEndpointAddress: IN Endpoint 1
    0x03,                           // bmAttributes
    DEF_ENDP_SIZE_KB & 0xFF, DEF_ENDP_SIZE_KB >> 8, // wMaxPacketSize
    0x0A,                           // bInterval: 10mS

    /* Interface Descriptor (Mouse) */
    0x09,                           // bLength
    0x04,                           // bDescriptorType
    0x01,                           // bInterfaceNumber
    0x00,                           // bAlternateSetting
    0x01,                           // bNumEndpoints
    0x03,                           // bInterfaceClass
    0x01,                           // bInterfaceSubClass
    0x02,                           // bInterfaceProtocol: Mouse
    0x00,                           // iInterface

    /* HID Descriptor (Mouse) */
    0x09,                           // bLength
    0x21,                           // bDescriptorType
    0x10, 0x01,                     // bcdHID
    0x00,                           // bCountryCode
    0x01,                           // bNumDescriptors
    0x22,                           // bDescriptorType
    USBD_SIZE_REPORT_DESC_MS & 0xFF, USBD_SIZE_REPORT_DESC_MS >> 8, // wItemLength

    /* Endpoint Descriptor (Mouse) */
    0x07,                           // bLength
    0x05,                           // bDescriptorType
    0x82,                           // bEndpointAddress: IN Endpoint 2
    0x03,                           // bmAttributes
    DEF_ENDP_SIZE_MS & 0xFF, DEF_ENDP_SIZE_MS >> 8, // wMaxPacketSize
    0x01                            // bInterval: 1mS                   /* Endpoint Descriptor */
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

/* HID Report Descriptor */
const uint8_t USBD_KeyRepDesc[USBD_SIZE_REPORT_DESC_KB] =
{
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x06,                     // Usage (Keyboard)
    0xA1, 0x01,                     // Collection (Application)
    0x05, 0x07,                     // Usage Page (Key Codes)
    0x19, 0xE0,                     // Usage Minimum (224)
    0x29, 0xE7,                     // Usage Maximum (231)
    0x15, 0x00,                     // Logical Minimum (0)
    0x25, 0x01,                     // Logical Maximum (1)
    0x75, 0x01,                     // Report Size (1)
    0x95, 0x08,                     // Report Count (8)
    0x81, 0x02,                     // Input (Data,Variable,Absolute)
    0x95, 0x01,                     // Report Count (1)
    0x75, 0x08,                     // Report Size (8)
    0x81, 0x01,                     // Input (Constant)
    0x95, 0x03,                     // Report Count (3)
    0x75, 0x01,                     // Report Size (1)
    0x05, 0x08,                     // Usage Page (LEDs)
    0x19, 0x01,                     // Usage Minimum (1)
    0x29, 0x03,                     // Usage Maximum (3)
    0x91, 0x02,                     // Output (Data,Variable,Absolute)
    0x95, 0x05,                     // Report Count (5)
    0x75, 0x01,                     // Report Size (1)
    0x91, 0x01,                     // Output (Constant,Array,Absolute)
    0x95, 0x06,                     // Report Count (6)
    0x75, 0x08,                     // Report Size (8)
    0x26, 0xFF, 0x00,               // Logical Maximum (255)
    0x05, 0x07,                     // Usage Page (Key Codes)
    0x19, 0x00,                     // Usage Minimum (0)
    0x29, 0x91,                     // Usage Maximum (145)
    0x81, 0x00,                     // Input(Data,Array,Absolute)
    0xC0                            // End Collection
};
const uint8_t USBD_MouseRepDesc[USBD_SIZE_REPORT_DESC_MS] =
{
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x02,                     // Usage (Mouse)
    0xA1, 0x01,                     // Collection (Application)
    0x09, 0x01,                     // Usage (Pointer)
    0xA1, 0x00,                     // Collection (Physical)
    0x05, 0x09,                     // Usage Page (Button)
    0x19, 0x01,                     // Usage Minimum (Button 1)
    0x29, 0x03,                     // Usage Maximum (Button 3)
    0x15, 0x00,                     // Logical Minimum (0)
    0x25, 0x01,                     // Logical Maximum (1)
    0x75, 0x01,                     // Report Size (1)
    0x95, 0x03,                     // Report Count (3)
    0x81, 0x02,                     // Input (Data,Variable,Absolute)
    0x75, 0x05,                     // Report Size (5)
    0x95, 0x01,                     // Report Count (1)
    0x81, 0x01,                     // Input (Constant,Array,Absolute)
    0x05, 0x01,                     // Usage Page (Generic Desktop)
    0x09, 0x30,                     // Usage (X)
    0x09, 0x31,                     // Usage (Y)
    0x09, 0x38,                     // Usage (Wheel)
    0x15, 0x81,                     // Logical Minimum (-127)
    0x25, 0x7F,                     // Logical Maximum (127)
    0x75, 0x08,                     // Report Size (8)
    0x95, 0x03,                     // Report Count (3)
    0x81, 0x06,                     // Input (Data,Variable,Relative)
    0xC0,                           // End Collection
    0xC0                            // End Collection
};





