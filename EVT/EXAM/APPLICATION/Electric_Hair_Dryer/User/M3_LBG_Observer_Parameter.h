/********************************** (C) COPYRIGHT *******************************
 * File Name          : M3_LBG_Observer_Parameter.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __M3_LBG_OBSERVER_H
#define __M3_LBG_OBSERVER_H
#define LBG_CAL_FREQ_M          FREQ_SVPWM  
#define LBG_CAL_PERIOD_M        _IQ(1.0)/LBG_CAL_FREQ_M
#define LBG_K1_CERROR_CURR_M    (-30000)    
#define LBG_K2_CERROR_EMF_M     40000      
#define LBG_K2_CERROR_EMF_ADJ_M 20          
#define LBG_CAL_TIMES_M         2           

#define LBG_MAX_EMF_M           _IQ(2.0)    
#define LBG_MIN_EMF_M           _IQ(-2.0)   

#define LBG_MAX_CURR_M          _IQ(2.0)    
#define LBG_MIN_CURR_M          _IQ(-2.0)   

#define PLL_KP_M                140         
#define PLL_KI_M                25148       
#define LBG_SPEED_MAX_M         _IQ(1.5)    
#define LBG_SPEED_MIN_M         _IQ(0.01)   

#if defined OBSERVER_ADAPTOR
#define LBG_ADAPTOR_ENABLE      1;
#else
#define LBG_ADAPTOR_ENABLE      0;
#endif

#define LBG_ADAPTOR_COEF_M      8000        
#define LBG_ADAPTOR_EMF_ADJ_M   _IQ(0.5)    
#define LBG_SPEED_BUFFERSIZE_M    96        

#if defined POSICOM
#define LBG_POSICOM_ENABLE      1;
#else
#define LBG_POSICOM_ENABLE      0;
#endif

#define LBG_SPEEDLPF_CF_M        50                
#define LBG_SPEEDLPF_PERIOD_M    (_IQ(1.0)/SPEED_LOOP_CAL_FREQ) 
#define LBG_SPEEDLPF_WcT_M       _IQmpyI32(_IQmpy(LBG_SPEEDLPF_PERIOD_M,PI_2_Q24),LBG_SPEEDLPF_CF_M)

#define LBG_OVERSPEED_THRESH_M          _IQ(1.5)    
#define LBG_UNDERSPEED_THRESH_M         _IQ(0.05)   
#define LBG_OVERSPEED_CNT_THRESH_M      500         
#define LBG_UNDERSPEED_CNT_THRESH_M     500        

#define LBG_SPEED_VARIANCE_THRESH_M     _IQ(0.2)    
#define LBG_UNSTABLE_CNT_THRESH_M       300         
#define LBG_UNSTABLE_SPEED_THRESH_M     _IQ(0.02)   

#define LBG_CONVERGED_CNT_THRESH_M      100         
#define LBG_CONVERGED_UPPER_THRESH_M    _IQ(1.2)    
#define LBG_CONVERGED_LOWER_THRESH_M    _IQ(0.8)   
#define LBG_CONVERGED_EMFUP_THRESH_M    _IQ(0.5)    
#define LBG_CONVERGED_EMFDOWN_THRESH_M  _IQ(-0.2)   
#define LBG_EMFLPF_CF_M        50                   
#define LBG_EMFLPF_PERIOD_M    (_IQ(1.0)/SPEED_LOOP_CAL_FREQ)   
#define LBG_EMFLPF_WcT_M       _IQmpyI32(_IQmpy(LBG_EMFLPF_PERIOD_M,PI_2_Q24),LBG_EMFLPF_CF_M)

#define LBG_STALLBAND_M         _IQ(0.5)
#define LBG_STALLSPEED_M        _IQ(0.1)
#define LBG_STALLCURR_M         _IQMPY_MACRO(_IQ(0.9),SPEED_REGULATOR_ULIM_M)
#define LBG_STALL_CNT_THRESH_M  100    

#define LBG_REVERSE_CNT_THRESH_M    100        
#define LBG_REVERSE_SPEED_THRESH_M  _IQ(0.02)   

/* Exported variables -------------------------------------------------------*/
extern Motor_LBG_Type  MLBG_M;                         
extern Observer_Status_Type    LBG_Status_M;          
extern _iq24 LBG_SpeedBuffer_M[LBG_SPEED_BUFFERSIZE_M];

void LBG_Parameter_Define (Motor_LBG_Type *MLBG,Observer_Status_Type *LBGStatus);

void LBG_Paramter_Cal (Motor_Base_Type *Basestruc,Motor_LBG_Type *MLBG);

void LBG_Status_Init (Motor_LBG_Type *MLBG,Observer_Status_Type *LBGStatus);

void LBG_EMF_CalInitial(Motor_Sructure_Type *MStruc,InjStart_Type * InjStart,Motor_LBG_Type *MLBG);

void LBG_Adaptor_CalInitial(Motor_LBG_Type *MLBG);

void LBG_Speed_CalInitial(Motor_LBG_Type *MLBG,_iq24 *Buffer,_iq24 SpeedInial);

void LBG_EMF_Cal(Motor_Sructure_Type *MStruc,Motor_LBG_Type *MLBG);

void LBG_Speed_Posi_Cal(Motor_LBG_Type *MLBG);

void LBG_Cal_Init (InjStart_Type *InjStart,Motor_Sructure_Type *MStruc,Motor_LBG_Type *MLBG);

#endif
