/********************************** (C) COPYRIGHT *******************************
 * File Name          : M1_Motor_Parameter_Init.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Motor parameters and state initialization.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "M0_Control_Library.h"
Motor_Sructure_Type MStruc_M;           
Motor_Base_Type  MBase_M;               
Runningstatus_Type  RunningStatus_M;    
volatile uint16_t System_Status_Global; 

/******************************************************************************
* Function Name  : Motor_Prameter_Define
* Description    : Motor Prameter Define
* Input          : Pointer to motor base value parameter structure
* Output         : None
* Return         : None
******************************************************************************/
void Motor_Prameter_Define(Motor_Base_Type *Basestruc)
{

    Basestruc->Fn = RATED_FREQ_M;
    Basestruc->Vn = RATED_VOLT_M;
    Basestruc->In = RATED_CURR_M;
    Basestruc->Flux = ROTOR_FLUX_M;
    Basestruc->Pn = POLEPARE_M;
    Basestruc->Rs = STATOR_RES_M;
    Basestruc->Ld = STATOR_LD_M;
    Basestruc->Lq = STATOR_LQ_M;
    Basestruc->Rstotal = STATOR_RES_TOTAL_M;
    Basestruc->Inertia = INERTIA_M;
    Basestruc->Friction = FRICTION_M;
}

/******************************************************************************
* Function Name  : Motor_Status_Initial_M
* Description    : Motor Status Initialization
* Input          : Pointer to the motor overall status structure
* Output         : None
* Return         : None
******************************************************************************/
void Motor_Status_Initial_M(Motor_Sructure_Type *MStruc)
{
    MStruc->VRef2R.cd = _IQ(0.0);
    MStruc->VRef2R.cq = _IQ(0.0);
    MStruc->VRef2R.peak = _IQ(0.0);
    MStruc->VRef2RC.cd = _IQ(0.0);
    MStruc->VRef2RC.cq = _IQ(0.0);
    MStruc->VRef2RC.peak = _IQ(0.0);
    MStruc->VRef2S.Alpha = _IQ(0.0);
    MStruc->VRef2S.Beta = _IQ(0.0);
    MStruc->VRef2SC.Alpha = _IQ(0.0);
    MStruc->VRef2SC.Beta = _IQ(0.0);
    MStruc->CRef2R.cd = _IQ(0.0);
    MStruc->CRef2R.cq = _IQ(0.0);
    MStruc->CRef2R.peak = _IQ(0.0);
    MStruc->C3S.PhaseA = _IQ(0.0);
    MStruc->C3S.PhaseB = _IQ(0.0);
    MStruc->C3S.PhaseC = _IQ(0.0);
    MStruc->C2S.Alpha = _IQ(0.0);
    MStruc->C2S.Beta = _IQ(0.0);

    MStruc->C2R_H.cd = _IQ(0.0);
    MStruc->C2R_H.cq = _IQ(0.0);
    MStruc->C2R_L.cd = _IQ(0.0);
    MStruc->C2R_L.cq = _IQ(0.0);
    MStruc->C2R.cd = _IQ(0.0);
    MStruc->C2R.cq = _IQ(0.0);

    MStruc->C2R_Obs.cd = _IQ(0.0);
    MStruc->C2R_Obs.cq = _IQ(0.0);
    MStruc->C2S_Obs.Alpha = _IQ(0.0);
    MStruc->C2S_Obs.Beta = _IQ(0.0);

    MStruc->IdLPF1stH.Out_Pre = _IQ(0.0);
    MStruc->IdLPF1stH.Out_New = _IQ(0.0);
    MStruc->IdLPF1stL.Out_Pre = _IQ(0.0);
    MStruc->IdLPF1stL.Out_New = _IQ(0.0);

    MStruc->IqLPF1stH.Out_Pre = _IQ(0.0);
    MStruc->IqLPF1stH.Out_New = _IQ(0.0);
    MStruc->IqLPF1stL.Out_Pre = _IQ(0.0);
    MStruc->IqLPF1stL.Out_New = _IQ(0.0);

    MStruc->IdLPF1stObs.Out_Pre = _IQ(0.0);
    MStruc->IdLPF1stObs.Out_New = _IQ(0.0);
    MStruc->IqLPF1stObs.Out_Pre = _IQ(0.0);
    MStruc->IqLPF1stObs.Out_New = _IQ(0.0);

    MStruc->IdLPF1stInj.Out_Pre = _IQ(0.0);
    MStruc->IdLPF1stInj.Out_New = _IQ(0.0);
    MStruc->IqLPF1stInj.Out_Pre = _IQ(0.0);
    MStruc->IqLPF1stInj.Out_New = _IQ(0.0);

}

/******************************************************************************
* Function Name  : Global_Status_Initial
* Description    : Initialization of system status and motor operation status
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Global_Status_Initial(void)
{
    System_Status_Global |= SPEED_CONTROL_M;  
    System_Status_Global |= FIRST_START_M;    
    RunningStatus_M = IDLE;                   
}

/******************************************************************************
* Function Name  : Motor_FilerPara_Cal_M
* Description    : Motor Phase Filter Parameter Initialization
* Input          : Pointer to the motor overall status structure
* Output         : None
* Return         : None
******************************************************************************/
void Motor_FilerPara_Cal_M(Motor_Sructure_Type *MStruc)
{

    MStruc->IdLPF1stH.Input_Coef = _IQdiv(IDQLPF_WcT_HIGH_M,(IDQLPF_WcT_HIGH_M + UNIT_Q24));
    MStruc->IdLPF1stH.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_HIGH_M + UNIT_Q24));
    MStruc->IdLPF1stL.Input_Coef = _IQdiv(IDQLPF_WcT_LOW_M,(IDQLPF_WcT_LOW_M + UNIT_Q24));
    MStruc->IdLPF1stL.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_LOW_M + UNIT_Q24));

    MStruc->IqLPF1stH.Input_Coef = _IQdiv(IDQLPF_WcT_HIGH_M,(IDQLPF_WcT_HIGH_M + UNIT_Q24));
    MStruc->IqLPF1stH.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_HIGH_M + UNIT_Q24));
    MStruc->IqLPF1stL.Input_Coef = _IQdiv(IDQLPF_WcT_LOW_M,(IDQLPF_WcT_LOW_M + UNIT_Q24));
    MStruc->IqLPF1stL.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_LOW_M + UNIT_Q24));

    MStruc->IdLPF1stObs.Input_Coef = _IQdiv(IDQLPF_WcT_OBSERVER_M,(IDQLPF_WcT_OBSERVER_M + UNIT_Q24));
    MStruc->IdLPF1stObs.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_OBSERVER_M + UNIT_Q24));

    MStruc->IqLPF1stObs.Input_Coef = _IQdiv(IDQLPF_WcT_OBSERVER_M,(IDQLPF_WcT_OBSERVER_M + UNIT_Q24));
    MStruc->IqLPF1stObs.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_OBSERVER_M + UNIT_Q24));

    MStruc->IdLPF1stInj.Input_Coef = _IQdiv(IDQLPF_WcT_INITIAL_M,(IDQLPF_WcT_INITIAL_M + UNIT_Q24));
    MStruc->IdLPF1stInj.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_INITIAL_M + UNIT_Q24));

    MStruc->IqLPF1stInj.Input_Coef = _IQdiv(IDQLPF_WcT_INITIAL_M,(IDQLPF_WcT_INITIAL_M + UNIT_Q24));
    MStruc->IqLPF1stInj.Output_Coef = _IQdiv(UNIT_Q24,(IDQLPF_WcT_INITIAL_M + UNIT_Q24));
}
