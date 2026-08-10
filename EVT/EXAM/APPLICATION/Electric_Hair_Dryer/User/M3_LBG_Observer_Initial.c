/********************************** (C) COPYRIGHT *******************************
 * File Name          : M3_LBG_Observer_Initial.c
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
Motor_LBG_Type  MLBG_M;                         
Observer_Status_Type    LBG_Status_M;           
_iq24 LBG_SpeedBuffer_M[LBG_SPEED_BUFFERSIZE_M];

/******************************************************************************
* Function Name : LBG_Parameter_Define
* Description   : LBG Observer Parameter Definition
* Input         : LBG observer structure pointer, LBG observer state structure pointer
* Output        : None
* Return        : None
******************************************************************************/
void LBG_Parameter_Define (Motor_LBG_Type *MLBG,Observer_Status_Type *LBGStatus)
{

    MLBG->Para.CalFreq = LBG_CAL_FREQ_M;
    MLBG->Para.CalT = LBG_CAL_PERIOD_M;
    MLBG->Para.K1CerrC = LBG_K1_CERROR_CURR_M;
    MLBG->Para.K2CerrE = LBG_K2_CERROR_EMF_M;
    MLBG->Para.kCE_Adj = LBG_K2_CERROR_EMF_ADJ_M;
    MLBG->Para.Caltimes = LBG_CAL_TIMES_M;

    MLBG->Cal_Struc.EUlim = LBG_MAX_EMF_M;
    MLBG->Cal_Struc.ELlim = LBG_MIN_EMF_M;
    MLBG->Cal_Struc.CUlim = LBG_MAX_CURR_M;
    MLBG->Cal_Struc.CLlim = LBG_MIN_CURR_M;

    MLBG->Cal_Struc.SpeedLPF1st_M.Input_Coef = _IQdiv(LBG_SPEEDLPF_WcT_M,(LBG_SPEEDLPF_WcT_M + UNIT_Q24));
    MLBG->Cal_Struc.SpeedLPF1st_M.Output_Coef = _IQdiv(UNIT_Q24,(LBG_SPEEDLPF_WcT_M + UNIT_Q24));

    MLBG->Cal_Struc.Posicom_Enable = LBG_POSICOM_ENABLE;

    MLBG->Speed_PLL.Kpreal = PLL_KP_M;
    MLBG->Speed_PLL.Kireal = PLL_KI_M;
    MLBG->Speed_PLL.Upper_Lim = LBG_SPEED_MAX_M;
    MLBG->Speed_PLL.Lower_Lim = LBG_SPEED_MIN_M;

    MLBG->Adaptor.Enable = LBG_ADAPTOR_ENABLE;
    MLBG->Adaptor.Gain = LBG_ADAPTOR_COEF_M;
    MLBG->Adaptor.EMF_Adj = LBG_ADAPTOR_EMF_ADJ_M;

    MLBG->SpeedBuffPara.Length = LBG_SPEED_BUFFERSIZE_M;

    LBGStatus->OverspeedThresh = LBG_OVERSPEED_THRESH_M;
    LBGStatus->OverspeedcntThresh = LBG_OVERSPEED_CNT_THRESH_M;
    LBGStatus->UnderspeedThresh = LBG_UNDERSPEED_THRESH_M;
    LBGStatus->UnderspeedcntThresh = LBG_UNDERSPEED_CNT_THRESH_M;
    LBGStatus->VarianceThresh = LBG_SPEED_VARIANCE_THRESH_M;
    LBGStatus->UnstableCntThresh = LBG_UNSTABLE_CNT_THRESH_M;
    LBGStatus->UnstableSpeedThresh = LBG_UNSTABLE_SPEED_THRESH_M;
    LBGStatus->ConvergeCntThresh = LBG_CONVERGED_CNT_THRESH_M;
    LBGStatus->ConvergeUpper = LBG_CONVERGED_UPPER_THRESH_M;
    LBGStatus->ConvergeLower = LBG_CONVERGED_LOWER_THRESH_M;
    LBGStatus->ConvergeEmfBandup = LBG_CONVERGED_EMFUP_THRESH_M;
    LBGStatus->ConvergeEmfBanddown = LBG_CONVERGED_EMFDOWN_THRESH_M;
    LBGStatus->StallSpeedBand = LBG_STALLBAND_M;
    LBGStatus->StallSpeed = LBG_STALLSPEED_M;
    LBGStatus->StallCurr =  LBG_STALLCURR_M;
    LBGStatus->StallCntThresh = LBG_STALL_CNT_THRESH_M;
    LBGStatus->ReverseCntThresh = LBG_REVERSE_CNT_THRESH_M;
    LBGStatus->ReverseSpeedThresh = LBG_REVERSE_SPEED_THRESH_M;

    MLBG->Cal_Struc.EMFLPF1st_M.Input_Coef = _IQdiv(LBG_EMFLPF_WcT_M,(LBG_EMFLPF_WcT_M + UNIT_Q24));
    MLBG->Cal_Struc.EMFLPF1st_M.Output_Coef = _IQdiv(UNIT_Q24,(LBG_EMFLPF_WcT_M + UNIT_Q24));
}

/******************************************************************************
* Function Name : LBG_Status_Init
* Description   : LBG Observer State Initialization
* Input         : LBG observer structure pointer, LBG observer state structure pointer
* Output        : None
* Return        : None
******************************************************************************/
void LBG_Status_Init (Motor_LBG_Type *MLBG,Observer_Status_Type *LBGStatus)
{

    MLBG->Cal_Struc.Inimark = FALSE;

    MLBG->Cal_Struc.EalphaNew = _IQ(0.0);
    MLBG->Cal_Struc.EalphaPrev = _IQ(0.0);

    MLBG->Cal_Struc.EbetaNew = _IQ(0.0);
    MLBG->Cal_Struc.EbetaPrev  = _IQ(0.0);

    MLBG->Cal_Struc.CalphaNew = _IQ(0.0);
    MLBG->Cal_Struc.CalphaPrev = _IQ(0.0);
    MLBG->Cal_Struc.CbetaNew = _IQ(0.0);
    MLBG->Cal_Struc.CbetaPrev = _IQ(0.0);

    MLBG->Cal_Struc.ThetaErr = _IQ(0.0);
    MLBG->Cal_Struc.PosiElecPU = _IQ(0.0);
    MLBG->Cal_Struc.PosiElecPU_Pre = _IQ(0.0);
    MLBG->Cal_Struc.PosiElecPU_Cali = _IQ(0.0);
    MLBG->Cal_Struc.PosiElecPU_Com = _IQ(0.0);
    MLBG->Cal_Struc.Angspeed_Inst = _IQ(0.0);
    MLBG->Cal_Struc.Angspeed_Aver1 = _IQ(0.0);
    MLBG->Cal_Struc.Angspeed_Aver2 = _IQ(0.0);
    MLBG->Cal_Struc.Triangle.Cos = _IQ(0.0);
    MLBG->Cal_Struc.Triangle.Sin = _IQ(0.0);

    MLBG->Cal_Struc.Obs_Calcnt =0;

    LBGStatus->Overspeed = FALSE;
    LBGStatus->Underspeed = FALSE;
    LBGStatus->Reliable = FALSE;
    LBGStatus->Convergence = FALSE;
    LBGStatus->Timeout = FALSE;
    LBGStatus->Stable = TRUE;
    LBGStatus->Stall = FALSE;
    LBGStatus->Reverse = FALSE;

    LBGStatus->Overspeed_CNT =0;
    LBGStatus->Underspeed_CNT = 0;
    LBGStatus->Converged_CNT = 0;
    LBGStatus->Unstable_CNT = 0;
    LBGStatus->Stall_CNT = 0;
    LBGStatus->Reverse_CNT = 0;

    MLBG->Speed_PLL.Error_New = _IQ(0.0);
    MLBG->Speed_PLL.Error_Prev = _IQ(0.0);
    MLBG->Speed_PLL.Output_New = _IQ(0.0);
    MLBG->Speed_PLL.Output_Prev = _IQ(0.0);

#if defined OBSERVER_ADAPTOR

    MLBG->Adaptor.Alpha_Err = _IQ(0.0);
    MLBG->Adaptor.Alpha_Pre = _IQ(0.0);
    MLBG->Adaptor.Alpha_New = _IQ(0.0);
    MLBG->Adaptor.Beta_Err = _IQ(0.0);
    MLBG->Adaptor.Beta_Pre = _IQ(0.0);
    MLBG->Adaptor.Beta_New = _IQ(0.0);
#endif
}

/******************************************************************************
* Function Name : LBG_Cal_Init
* Description   : LBG Calculation Initialization
* Input         : Pointer to motor start control structure, 
*                 pointer to overall motor status structure, pointer to LBG observer structure
* Output        : None
* Return        : None
******************************************************************************/
void LBG_Cal_Init (InjStart_Type *InjStart,Motor_Sructure_Type *MStruc,Motor_LBG_Type *MLBG)
{
    LBG_EMF_CalInitial(MStruc,InjStart,MLBG);

    #if defined OBSERVER_ADAPTOR
    LBG_Adaptor_CalInitial(MLBG);
    #endif

    LBG_Speed_CalInitial(MLBG,&(LBG_SpeedBuffer_M[0]),InjStart->RampInjSpeedRef);
}
