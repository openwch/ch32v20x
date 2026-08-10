/********************************** (C) COPYRIGHT *******************************
 * File Name          : MA_HFVQI_Init.c
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
HFVQI_Start_Type    HFVQI_StartStruc_M;         
BuffPara_Type       HFVQIPosi_CBuffPara_M;      
_iq24   HFVQIPosi_CBuff_M[HFVQI_POSI_SECTOR_M]; 
uint8_t HFVQI_Sectorbuf_M[HFVQI_SECTORBUFF];   

/******************************************************************************/
/* Function Name  : HFVQI_Parameter_Define
/* Description : HFVQI Parameter define
/* Input:   None
/* Output : None
/* Return : None
/******************************************************************************/
void HFVQI_Parameter_Define (HFVQI_Start_Type *HFVQI_Start,SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali)
{
    HFVQI_Start->Vmag = HFVQI_POSI_VMAG_M;

    HFVQI_Start->Vmagmax = _IQ16((SVPWM_Cal->HALF_T - PWMCali->ADCTimeMin -2)/2);
    HFVQI_Start->Vmagmax = _IQ16div(HFVQI_Start->Vmagmax,SVPWM_Cal->coef2) <<8;
    HFVQI_Start->Vmagmin = _IQ16div(_IQ16(PWMCali->ADCTimeMin + 2) , SVPWM_Cal->coef2) <<8;

    HFVQI_Start->TimeStep = HFVQI_POSI_STEP_M;
    HFVQI_Start->TimeEffect = HFVQI_POSI_PULSE_EFFECTIVE_M;
    HFVQI_Start->AngleInit = _IQ(-1.0)/12;
    HFVQI_Start->AngleStep = _IQ(1.0)/HFVQI_POSI_SECTOR_M;
    HFVQI_Start->SectorSet = HFVQI_POSI_SECTOR_M;
    HFVQI_Start->Posi_Cali = _IQ(1.0)/(HFVQI_POSI_SECTOR_M*2);

    HFVQIPosi_CBuffPara_M.Length = HFVQI_POSI_SECTOR_M;
    HFVQI_Start->Sectorbuff = HFVQI_SECTORBUFF;
    HFVQI_Start->Cyclemax = HFVQI_CYCLE_MAX;

    HFVQI_Start->Phaseloss_Currthre = PHASELOSS_CURR_THRE;
    HFVQI_Start->Phaseloss_Cntthre = PHASELOSS_CNT_THRE;
}

/*******************************************************************************/
/* Function Name  : HFVQI_PosiSearch_Status_Init_M
/* Description : HFVQI initial positioning state initialization
/* Input:   None
/* Output : None
/* Return : None
/******************************************************************************/
void HFVQI_Status_Init_M (HFVQI_Start_Type *HFVQI_Start)
{
    HFVQI_Start->Vpolar = _IQ(0.0);
    HFVQI_Start->Counter = 0;
    HFVQI_Start->Position = 0;
    HFVQI_Start->SectorAct = 0;
    HFVQI_Start->CurrMax = 0;
    HFVQI_Start->SectorMax = 0;
    HFVQI_Start->Finish = FALSE;
    HFVQI_Start->AngleInit = _IQ(-1.0)/12;//¹Ì¶¨-30¡ã¿ªÊ¼

    HFVQI_Start->Curr3S.PhaseA = 0;
    HFVQI_Start->Curr3S.PhaseB = 0;
    HFVQI_Start->Curr3S.PhaseC = 0;
    HFVQI_Start->Curr2S.Alpha = 0;
    HFVQI_Start->Curr2S.Beta = 0;
    HFVQI_Start->Curr2R.cd = 0;
    HFVQI_Start->Curr2R.cq = 0;
    HFVQI_Start->VoltRef2R.cd = 0;
    HFVQI_Start->VoltRef2R.cq = 0;
    HFVQI_Start->VoltRef2RC.cd = 0;
    HFVQI_Start->VoltRef2RC.cq = 0;
    HFVQI_Start->VoltRef2SC.Alpha = 0;
    HFVQI_Start->VoltRef2SC.Beta = 0;

    HFVQI_Start->Verifytime =0;
    HFVQI_Start->Cycletime =0;

    HFVQI_Buffer_Clear();

    HFVQI_Start->Phaseloss_Cnt = 0;
}

/*******************************************************************************/
/* Function Name  : HFVQI_Buffer_Clear
/* Description : HFVQI Initial Positioning Sector Array Initialization
/* Input:   None
/* Output : None
/* Return : None
/******************************************************************************/
void HFVQI_Buffer_Clear(void)
{
    u_int8_t i;
    for(i=0; i < HFVQI_SECTORBUFF; i++)
    {
        HFVQI_Sectorbuf_M[i]=0;
    }
}
