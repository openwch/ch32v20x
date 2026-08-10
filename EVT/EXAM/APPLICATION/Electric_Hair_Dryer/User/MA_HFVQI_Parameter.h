/********************************** (C) COPYRIGHT *******************************
 * File Name          : MA_HFVQI_Parameter.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        :
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef _MA_HFVQI_OBSERVER_H
#define _MA_HFVQI_OBSERVER_H

#define HFVQI_POSI_VMAG_M           _IQ(0.25)   

#if defined TWELVE_SECTOR_POSITION
#define HFVQI_POSI_SECTOR_M          12        
#endif

#if defined SIX_SECTOR_POSITION
#define HFVQI_POSI_SECTOR_M             6       
#endif

#define HFVQI_POSI_STEP_M               10      
#define HFVQI_POSI_PULSE_EFFECTIVE_M    3       
#define HFVQI_SECTORBUFF                2       
#define HFVQI_CYCLE_MAX                 2       

/* Exported variables -------------------------------------------------------*/
extern HFVQI_Start_Type HFVQI_StartStruc_M;     
extern BuffPara_Type    HFVQIPosi_CBuffPara_M;  
extern _iq24    HFVQIPosi_CBuff_M[];            
extern uint8_t  HFVQI_Sectorbuf_M[];            

//HFVQI Init
void HFVQI_Parameter_Define (HFVQI_Start_Type *HFVQI_Start,SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali);

//HFVQI Init
void HFVQI_Status_Init_M (HFVQI_Start_Type *HFVQI_Start);

//HFVQI Init
void HFVQI_Buffer_Clear(void);

//HFVQI Init
void HFVQI_PosiSearch_Process(HFVQI_Start_Type *HFVQI_Start,SVPWM_1R_Type *SVPWM_Cal,\
        PWMCali_Type *PWMCali,ADCStruc_Type *ADCStruc,InjStart_Type *Injstart, int8_t Direction);

#endif
