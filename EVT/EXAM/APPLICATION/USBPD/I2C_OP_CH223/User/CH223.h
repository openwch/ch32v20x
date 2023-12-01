/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH223.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/08/15
 * Description        : header file for CH223.c
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include"debug.h"

#define Address_8bit    0
#define Address_16bit   1
#define Address_Lenth   Address_8bit
#define SIZE sizeof(Buffer)

extern u32 Buffer[7] ;//NDO raw data in SRCCAP
extern u16 BufferV[SIZE];//Analyzing voltage data in SRCCAP
extern u16 BufferI[SIZE];//Analyzing Current Data in SRCCAP
extern u16 BufferVPPS[SIZE];//Parsing PPS data in SRCCAP

void IIC_Init(u32 bound, u16 address);
void CH223_Init();
u8 CH223_ReadOneByte(u16 ReadAddr);
void CH223_WriteOneByte(u16 WriteAddr, u8 DataToWrite);
void CH223_Read(u16 ReadAddr, u32 *pBuffer, u16 NumToRead);
void CH223_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);
void Buffer_Change(u32 buffer[], int n);
void Current_VI(u8 ndo_select);
void User_Set_VI(u8 TargetData);
