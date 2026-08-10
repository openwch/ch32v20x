/********************************** (C) COPYRIGHT *******************************
 * File Name          : M9_Appication_Initl.c
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

Powercom_type Powercom_M;              

/*******************************************************************************
* Function Name  : Powercom_Prameter_Define
* Description    : Power Compensation Parameter Definition
* Input          : Power Fluctuation Suppression Structure
* Output         : None
* Return         : None
*******************************************************************************/
void Powercom_Prameter_Define(Powercom_type *Powercom)
{
    Powercom->Power_HPF1st.Input_Coef = _IQdiv(UNIT_Q24,(POWERCOM_HPF1st_WcT_M+UNIT_Q24));
    Powercom->Power_HPF1st.Output_Coef = _IQdiv(UNIT_Q24,(POWERCOM_HPF1st_WcT_M+UNIT_Q24));

    Powercom->Power_LPF1st.Input_Coef = _IQdiv(POWERCOM_LPF1st_WcT_M,(POWERCOM_LPF1st_WcT_M+UNIT_Q24));
    Powercom->Power_LPF1st.Output_Coef = _IQdiv(UNIT_Q24,(POWERCOM_LPF1st_WcT_M+UNIT_Q24));

    Powercom->Kpdefine = POWERCOM_KP;
    Powercom->Kidefine = POWERCOM_KIT;
    Powercom->Kddefine = POWERCOM_KD;
    Powercom->Kpadj1 = POWERCOM_KPADJ1_M;
    Powercom->Kpadj2 = POWERCOM_KPADJ2_M;
    Powercom->Kpadj3 = POWERCOM_KPADJ3_M;
    Powercom->Kpadj = Powercom->Kpadj1;

    Powercom->PowerPID.Kp = _IQMPY_MACRO(Powercom->Kpdefine,Powercom->Kpadj);
    Powercom->PowerPID.Ki = Powercom->Kidefine;
    Powercom->PowerPID.Kd = Powercom->Kddefine;
    Powercom->PowerPID.Upper_Lim = POWERCOM_ULIM_M;
    Powercom->PowerPID.Lower_Lim = POWERCOM_LLIM_M;
    Powercom->PowerPID.Deltalim = POWERCOM_DLIM_M;

    Powercom->Startcom_cntset = POWERCOM_FREQ_M*POWERCOM_STARTTIME_M;
    Powercom->Speedcolse_cntset = POWERCOM_FREQ_M*POWERCOM_SPEEDCLOSETIME_M;
    Powercom->Speederrthre = POWERCOM_SPEEDERRTHRE_M;
    Powercom->Voltthre = POWERCOM_VOLT_THRESHOLD_M;
}

/*******************************************************************************
* Function Name  : Powercom_Status_Init
* Description    : Power Compensation Parameter Initialization
* Input          : Power Fluctuation Suppression Structure
* Output         : None
* Return         : None
*******************************************************************************/
void Powercom_Status_Init(Powercom_type *Powercom)
{
    Powercom->Power_HPF1st.In_Pre = 0;
    Powercom->Power_HPF1st.Out_Pre = 0;
    Powercom->Power_HPF1st.Out_New = 0;

    Powercom->Power_LPF1st.Out_Pre = 0;
    Powercom->Power_LPF1st.Out_New = 0;

    Powercom->PowerPID.Err_New = 0;
    Powercom->PowerPID.Err_Prev1 = 0;
    Powercom->PowerPID.Err_Prev2 = 0;
    Powercom->PowerPID.Differential = 0;
    Powercom->PowerPID.Deltaoutput = 0;
    Powercom->PowerPID.Output_New = 0;
    Powercom->PowerPID.Output_Prev = 0;
    Powercom->PowerPID.Difference = 0;

    Powercom->Speedcom =0;

    Powercom->Power = _IQ(0.0);
    Powercom->PowerfilterH = _IQ(0.0);
    Powercom->PowerfilterL = _IQ(0.0);

    Powercom->Startcom_cnt=0;
    Powercom->Speedclose_cnt = 0;
}

