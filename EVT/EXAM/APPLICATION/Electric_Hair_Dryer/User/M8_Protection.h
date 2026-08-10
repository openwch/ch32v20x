/********************************** (C) COPYRIGHT *******************************
 * File Name          : M8_Protection.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Protection parameter definition
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __M8_PROTECTION_H
#define __M8_PROTECTION_H
#define INST_PHASEOC_THRE_M       _IQ(1.3)     
#define INST_PHASEOC_CNT_THRE_M   (uint16_t)5   

#define DCBUS_OVER_VOLT_THRE    _IQ(1.175)      
#define DCBUS_UNDER_VOLT_THRE   _IQ(0.72)       
#define DCBUS_OVER_VOLT_CNT     (uint16_t)500   
#define DCBUS_UNDER_VOLT_CNT    (uint16_t)3000   

#define PEAKDCBUS_OVER_VOLT_CNT     (uint16_t)1500   
#define PEAKDCBUS_OVER_VOLT_DETECTIONTIME     0.3   

#define NTC_OVERHYSTERESIS_TEMP_TIME    5      

#define NTC_OVERTEMP_OHM     _IQ18(0.8844)
#define NTC_OVERTEMP_RATEADC _IQ18toIQ(_IQ18div(NTC_OVERTEMP_OHM , NTC_OVERTEMP_OHM + PT_RS))

#define NTC_OPENTEMP_OHM     _IQ18(981.5699)
#define NTC_OPENTEMP_RATEADC _IQ18toIQ(_IQ18div(NTC_OPENTEMP_OHM , NTC_OPENTEMP_OHM + PT_RS))

#define NTC_VERIFY_FREQ     TIMER_FREQ_SPEED_LOOP   

#define TEMP_OVER_TIME       0.1                     
#define NTC_OPEN_TIME       0.1                     

#define OVERLOAD_CURR_THRE      _IQ(1.1)        
#define OVERLOAD_TIME_THRE      0.5             
#define OVERLOAD_CAL_FREQ       TIMER_FREQ_SPEED_LOOP   
#define OVERLOAD_CAL_COEF       _IQ(1/1.91)     
#define OVERLOADLPF_CF_M        500             
#define OVERLOADLPF_WcT_M       (PI_2_Q24/OVERLOAD_CAL_FREQ)*OVERLOADLPF_CF_M

#define PHASELOSS_CURR_THRE     _IQ(0.1)        
#define PHASELOSS_TIME_THRE      0.1            
#define PHASELOSS_CAL_FREQ       FREQ_SVPWM     
#define PHASELOSS_CAL_BUFFSIZE   32             
#define PHASELOSS_CNT_THRE       3              

/* Exported variables -------------------------------------------------------*/
extern InstPhaseOC_Type  InstPhaseOC_M;        
extern DCbusProtection_Type DCBusProtecion_M; 
extern OverLoad_Type        Overload_M;        
extern LPF_First_Order_Type OverloadLPF1st_M;  
extern Phaseloss_Type Phaseloss_M;             

/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Protection_and_Emergency_Parameter_Define (void);

void Protection_and_Emergency_Init (void);

void Protection_SetFault(uint16_t Fault_type);

void Protction_Fault_Reset_M(void);

Truth_Verify_Type Phase_Ins_OverCurrent_Verify (Component_3S_Type *PhaseABC, InstPhaseOC_Type * InstOC);

void DCOvervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec);

void DCUndervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec);
void DCUndervolt_Protection1(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec,Powercontrol_Type *PowerOper);
void Speed_Outrange_Verify(_iq24 Speedref, _iq24 Speedact, Observer_Status_Type *Status);

Truth_Verify_Type Speed_Reliable_Verify(BuffPara_Type *Buffpara,_iq24 *Buffer,_iq24 Averspeed,_iq24 Threshold);

void Speed_Stable_Verify(Observer_Status_Type *Status,_iq24 Speedact);

void Motor_Stall_Verify1(_iq24 Speedref, Motor_LBG_Type *MLBG,_iq24 Iqref, Observer_Status_Type *Status);
void Motor_Stall_Verify2(_iq24 Speedref, _iq24 Speedact , _iq24 Iqref, Observer_Status_Type *Status);

void Motor_Reverse_Verify(_iq24 Speedref, _iq24 Speedact, Observer_Status_Type *Status);

void Speed_Converged_Verify1(_iq24 Speedref,Motor_LBG_Type *MLBG,Observer_Status_Type *Status);
void Speed_Converged_Verify2(_iq24 Speedref,_iq24 Speedact,Observer_Status_Type *Status);

Truth_Verify_Type OverLoad_Verify (Component_3S_Type *PhaseABC, OverLoad_Type * Overload, LPF_First_Order_Type * LPF);

Truth_Verify_Type Phaseloss_Verify (Component_3S_Type *PhaseABC,Phaseloss_Type *Phaseloss, uint8_t PWMsector);

void NTC_Protection(Temperature_ctl_Type *Temctl,_iq24 Temp);

void PeakDCOvervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec);
#endif
