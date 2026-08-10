/********************************** (C) COPYRIGHT *******************************
 * File Name          : M2_PID_Parameter.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Definition of PID control parameters.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __M2_MOTOR_PID_H
#define __M2_MOTOR_PID_H
#define SPEED_REGULATOR_FREQ_M    SPEED_LOOP_CAL_FREQ   
#define SPEED_REGULATOR_PERIOD_M  (_IQ(1.0)/SPEED_REGULATOR_FREQ_M)
#define SPEED_REGULATOR_KP_M      _IQ(0.000006)        
#define SPEED_REGULATOR_KI_M      _IQ(0.000006)         
#define SPEED_REGULATOR_KD_M      _IQ(0.0)              
#define SPEED_REGULATOR_KPADJ_M   _IQ(1.0)              
#define SPEED_REGULATOR_KIADJ_M   _IQ(1.0)              
#define SPEED_REGULATOR_KDADJ_M   _IQ(1.0)              
#define SPEED_REGULATOR_ULIM_M    _IQ(0.8)              
#define SPEED_REGULATOR_LLIM_M    _IQ(-0.1)             
#define SPEED_REGULATOR_DLIM_M    _IQ(1.0)              


#ifdef MAG100_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(1.0000)    
#define MAX_MODULATION              _IQ(1.0000)     
#elif  defined MAG098_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(0.9604)     
#define MAX_MODULATION              _IQ(0.9800)    
#elif  defined MAG096_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(0.9216)     
#define MAX_MODULATION              _IQ(0.9600)     
#elif  defined MAG094_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(0.8836)    
#define MAX_MODULATION              _IQ(0.9400)     
#elif  defined MAG092_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(0.8464)     
#define MAX_MODULATION              _IQ(0.9200)     
#elif  defined MAG090_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(0.8100)     
#define MAX_MODULATION              _IQ(0.9000)     
#elif  defined MAG088_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(0.7744)     
#define MAX_MODULATION              _IQ(0.8800)     
#elif  defined MAG086_TABLE_USED
#define MAX_MODULATIONSQUARE        _IQ(0.7396)     
#define MAX_MODULATION              _IQ(0.8600)     
#endif

#define CURR_REGULATOR_FREQ_M   FREQ_SVPWM              
#define CURR_REGULATOR_BW_M     (uint16_t)(PI_2f * 200) 

#define DAXIS_CURR_REGULATOR_KD_M       _IQ(0.0)        
#define DAXIS_CURR_REGULATOR_KPADJ_M    _IQ(5.0)       
#define DAXIS_CURR_REGULATOR_KIADJ_M    _IQ(1.0)        
#define DAXIS_CURR_REGULATOR_KDADJ_M    _IQ(1.0)        
#define DAXIS_CURR_REGULATOR_ULIM_M     _IQ(0.25)       
#define DAXIS_CURR_REGULATOR_LLIM_M     _IQ(-0.25)      
#define DAXIS_CURR_REGULATOR_DLIM_M     _IQ(1.0)        

#define QAXIS_CURR_REGULATOR_KD_M       _IQ(0.0)        
#define QAXIS_CURR_REGULATOR_KPADJ_M    _IQ(5.0)        
#define QAXIS_CURR_REGULATOR_KIADJ_M    _IQ(1.0)        
#define QAXIS_CURR_REGULATOR_KDADJ_M    _IQ(1.0)        
#define QAXIS_CURR_REGULATOR_ULIM_M     MAX_MODULATION 
#define QAXIS_CURR_REGULATOR_LLIM_M     (-MAX_MODULATION)  
#define QAXIS_CURR_REGULATOR_DLIM_M     _IQ(1.0)        


#define DAXIS_CURR_REF_INIT1_M          _IQ(0.3)       
#define DAXIS_CURR_REF_INIT2_M          _IQ(0.15)       
#define DAXIS_CURR_REF_STEP1_M          _IQ(0.0002)     
#define DAXIS_CURR_REF_STEP2_M          _IQ(0.00002)    
#define DAXIS_CURR_WEAK_SPEED_M         _IQ(1.5)        

/* Exported variables -------------------------------------------------------*/
extern Motor_PID_Type  Mpid_M;              
extern DCurrProcess_Type    DCurrProcess_M; 
extern _iq24 Magnitude_table[];            


/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void PID_Prameter_Define(Motor_PID_Type * Mpid);

void PID_Output_Init(Motor_PID_Type * Mpid);

void PID_ParaBase_Cal_M(Motor_Base_Type *Basestruc,Motor_PID_Type * Mpid);

void DCurrProcess_Parameter_Define(DCurrProcess_Type * DCurrProcess);

#endif
