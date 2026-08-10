/********************************** (C) COPYRIGHT *******************************
 * File Name          : M4_SVPWM_ADC1R.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __MOTOR_SVPWM_ADC1R_H
#define __MOTOR_SVPWM_ADC1R_H
#define DCBUS_RATED_VOLT    _IQ18(311)      

#define SYSTEM_CLOCK_FREQ   144000000uL     
#define TIMER_PRESCALER_SVPWM       0
#define TIMER_CLOCK_FREQ_SVPWM      (SYSTEM_CLOCK_FREQ/(TIMER_PRESCALER_SVPWM+1))
#define TIMER_UPDATE_RATE_SVPWM     1
#define FREQ_SVPWM                  30000

#define DEADTIME_SVPWM   1000
#define DEADTIME_CLOCK_SVPWM  (uint16_t)((DEADTIME_SVPWM*(SYSTEM_CLOCK_FREQ/1000000))/1000)
#define DEADTIME_CONFIGURE  (uint8_t)136

#define BRK_GPIO_M    GPIOB            
#define BRK_PIN_M     GPIO_Pin_12      

#define CURR_SAMPLE_RESISTOR_ADC    _IQ18(0.1)     
#define CURR_AMPLIFIER_ADC          _IQ18(3.73333)  
#define CURR_SAMPLE_RANGE_ADC       _IQ18div(_IQ18(1.65),_IQ18mpy(CURR_SAMPLE_RESISTOR_ADC,CURR_AMPLIFIER_ADC))
#define CURR_OFFSET_READ_NUM_ADC    ((uint8_t)16)   


#define SAMPLING_TIMER_CLOCK        ADC_SampleTime_7Cycles5

#define CONVER_TIME_ADC         750

#define CURR_CONVERT_CLOCK_ADC (uint16_t)((CONVER_TIME_ADC*(SYSTEM_CLOCK_FREQ/1000000))/1000)

#define CURR_RISE_TIME_ADC      900

#define CURR_RISE_CLOCK_ADC     (uint16_t)((CURR_RISE_TIME_ADC*(SYSTEM_CLOCK_FREQ/1000000))/1000)

#define CURR_NOISE_TIME_ADC     650

#define CURR_NOISE_CLOCK_ADC    (uint16_t)((CURR_NOISE_TIME_ADC*(SYSTEM_CLOCK_FREQ/1000000))/1000)

#if(CURR_RISE_TIME_ADC > CURR_NOISE_TIME_ADC)
    #define CURR_MAX_TRTN_CLOCK_ADC  CURR_RISE_CLOCK_ADC
#else
    #define CURR_MAX_TRTN_CLOCK_ADC  CURR_NOISE_CLOCK_ADC
#endif

#define ADC1_CURR_CHANNEL_PHASE_M         ADC_Channel_4
#define ADC2_CURR_CHANNEL_PHASE_M         ADC_Channel_5

#define DCVOLT_INIT         _IQ(1.0)          
#define DCVOLT_CONV_RATIO   _IQ18(0.006651884)  
#define DCVOLT_AVER_SIZE    100                 

#define DCVOLT_LPF_CF_M         1000                
#define DCVOLT_LPF_PERIOD_M     LBG_CAL_PERIOD_M    
#define DCVOLT_LPF_WcT_M        _IQmpyI32(_IQmpy(DCVOLT_LPF_PERIOD_M,PI_2_Q24),DCVOLT_LPF_CF_M)

/* Exported variables -------------------------------------------------------*/
extern SVPWM_1R_Type    SVPWM_Cal_M;    
extern ADCStruc_Type    ADC_M;          
extern PWMCali_Type     PWMCali_M;      

void SVPWM_1R_Paramter_Init(SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali,Motor_Base_Type *Mbase);

void SVPWM_1R_Status_Init_M(SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali);

void ADC_Parameter_Init (ADCStruc_Type *ADCStruc,Motor_Base_Type *Mbase);

void ADC_Status_Init (ADCStruc_Type *ADCStruc);

void SVPWM_1R_Timer_Configure(void);

void ADC_OPA_Configure(ADCStruc_Type *ADCStruc);

void ADC_1R_Offset_Reading(ADC_TypeDef *ADCx, ADCStruc_Type *ADCStruc,u_int8_t ADC_num);

void ADC_1R_Start (FunctionalState cmd);

void SVPWM_Updateup(SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali);

void SVPWM_Updatedown (SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali);

void ADC_Trigger_Update(PWMCali_Type *PWMCali);

void DCBUS_Volt_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx);

void SVPWM_Calculation(SVPWM_1R_Type *SVPWM_Cal, Component_2S_Type *VoltRef);

void SVPWM_Calibration(SVPWM_1R_Type *SVPWM_Cal, PWMCali_Type *PWMCali_Struc);

Component_3S_Type Current_Cal_1R(uint8_t Sector, ADCStruc_Type *ADCStruc);

void SVPWM_Zerovolt_Output(SVPWM_1R_Type *SVPWM_Cal);

void SVPWM_Lowside_Effective(SVPWM_1R_Type *SVPWM_Cal);

void SVPWM_Zerovolt_Output_Immediate(SVPWM_1R_Type *SVPWM_Cal);

void SVPWM_Lowside_Effective_Immediate(void);

void NTCSample(Temperature_ctl_Type *TempM, ADC_TypeDef *ADCx);

#endif
