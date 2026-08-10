/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_MotorStart.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Initialization of the motor before starting
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __M5_MOTORSTART_H
#define __M5_MOTORSTART_H
#define FORCE_INJ_FREQ_M                FREQ_SVPWM          
#define FORCE_INJ_CURR_M                _IQ(0.3)            
#define FORCE_INJ_ANGLE_M              _IQ(0.083333*1)     
#define FORCE_INJ_CURR_RAMP_TIME_M      _IQ(0.2)            
#define RAMP_INJ_STAGE_M                (uint32_t)(FREQ_SVPWM*3) 

#define RAMP_INJ_FREQ_M             FREQ_SVPWM              
#define RAMP_INJ_PERIOD_M           _IQ(1.0)/FREQ_SVPWM     
#define RAMP_INJ_CURR_INIT_M        _IQ(0.00)               
#define RAMP_INJ_CURR_FINAL_M       _IQ(0.4)               
#define RAMP_INJ_CURR_RAMP_TIME_M   _IQ(0.001)              
#define RAMP_INJ_SPEEDINT_M         _IQ(0.005)              
#define RAMP_INJ_SPEED_M            _IQ(0.2)                
#define RAMP_INJ_SPEED_RAMP_TIME_M  _IQ(0.2)                
#define RAMP_INJ_STAGE1_M           (uint32_t)(FREQ_SVPWM*0.15)  
#define RAMP_INJ_STAGE2_M           (uint32_t)(FREQ_SVPWM*0.6)  
#define INJECTION_CURRERR_THRESH_M  _IQ(0.5)                
#define RAMP_INJ_SPEED_ADJ          _IQ(0.05)               
#define RAMP_INJ_CURR_ADJ           _IQ(1.1)                

#define IF_VICOM_COEF_M             _IQ(-0.15)               
#define IF_VICOM_UPPER_M            _IQ(0.01)               
#define IF_VICOM_LOWER_M            _IQ(-0.01)               
#define IF_VICOM_PERIOD_M           _IQ(1.0)/FREQ_SVPWM      
#define IF_VICOM_HPF1st_CF_M        10                       
#define IF_VICOM_HPF1st_WcT_M       _IQmpy(_IQmpyI32(IF_VICOM_PERIOD_M,IF_VICOM_HPF1st_CF_M),PI_2_Q24)

/* Exported variables -------------------------------------------------------*/
extern InjStart_Type InjStart_M;               

void Motor_Start_Init_M(void);

void Injstart_Parameter_Define_M (InjStart_Type *InjStart);

void Injstart_Status_Init_M(InjStart_Type *InjStart);

void Injstart_Process_M(InjStart_Type *InjStart,Motor_Sructure_Type *MStruc,Motor_LBG_Type *MLBG);

void Controlloop_Initial(InjStart_Type *InjStart,_iq24 Speedact,Motor_Sructure_Type *MStruc,\
                    Motor_PID_Type *Mpid,Speedrampctr_Type *Speed_Ramp);

void Injstart_Curr_Speed_Ramp(InjStart_Type *InjStart);

void Injstart_Transition_Cal(InjStart_Type *InjStart,Motor_Sructure_Type *MStruc,Motor_LBG_Type *MLBG);

void Speedrampctr_Start_Init_M(Speedrampctr_Type *Speed_Ramp);
#endif
