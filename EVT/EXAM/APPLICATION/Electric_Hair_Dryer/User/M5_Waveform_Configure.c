/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Waveform_Configure.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "M0_Control_Library.h"

/******************************************************************************
* Function Name  : Waveform_Display
* Description    : Waveform Display Variable Settings
* Input          : None
* Return         : None
******************************************************************************/
void Waveform_Display (void)
{
    sendUsbData.sendDataBuf[sendUsbData.head+0] = (uint16_t)(SVPWM_Cal_M.VoltLimMark*1000);
    sendUsbData.sendDataBuf[sendUsbData.head+1] = (uint16_t)(MLBG_M.Cal_Struc.Angspeed_Aver2>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+2] = (uint16_t)(ADC_M.DCVoltAver>>10);
    sendUsbData.sendDataBuf[sendUsbData.head+3] = (uint16_t)(MStruc_M.CRef2R.cq>>10);
    sendUsbData.head += MaxUsbDataLen;
}
