/********************************** (C) COPYRIGHT *******************************
* File Name          : usb_prop.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2022/08/20
* Description        : USB processing.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __usb_prop_H
#define __usb_prop_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v20x.h"

#define USBD_GetConfiguration          NOP_Process
//#define USBD_SetConfiguration          NOP_Process
#define USBD_GetInterface              NOP_Process
#define USBD_SetInterface              NOP_Process
#define USBD_GetStatus                 NOP_Process
// #define USBD_ClearFeature              NOP_Process
#define USBD_SetEndPointFeature        NOP_Process
// #define USBD_SetDeviceFeature          NOP_Process
//#define USBD_SetDeviceAddress          NOP_Process

#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODING             0x20
#define GET_LINE_CODING             0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23


/* Definition of HID */

#define HID_GET_REPORT      0x01  
#define HID_GET_IDLE        0x02 
#define HID_GET_PROTOCOL    0x03  
#define HID_SET_REPORT      0x09  
#define HID_SET_IDLE        0x0A 
#define HID_SET_PROTOCOL    0x0B 

#define HID_DESCRIPTOR          0x21
#define HID_REPORT_DESCRIPTOR   0x22

#define HID_SET_REPORT_DEAL_OVER          0x00
#define HID_SET_REPORT_WAIT_DEAL          0x01


extern volatile uint8_t USBD_Sleep_Status;

void USBD_init(void);
void USBD_Reset(void);
void USBD_SetConfiguration(void);
void USBD_SetDeviceAddress (void);
void USBD_SetDeviceFeature (void);
void USBD_ClearFeature (void);
void USBD_Status_In (void);
void USBD_Status_Out (void);
RESULT USBD_Data_Setup(uint8_t);
RESULT USBD_NoData_Setup(uint8_t);
RESULT USBD_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *USBD_GetDeviceDescriptor(uint16_t );
uint8_t *USBD_GetConfigDescriptor(uint16_t);
uint8_t *USBD_GetStringDescriptor(uint16_t);
uint8_t *USBD_GetReportDescriptor(uint16_t);
uint8_t *HID_Set_Report(uint16_t);
uint8_t *HID_Get_Idle(uint16_t);
uint8_t *HID_Get_Protocol(uint16_t);
#ifdef __cplusplus
}
#endif

#endif /* __usb_prop_H */







