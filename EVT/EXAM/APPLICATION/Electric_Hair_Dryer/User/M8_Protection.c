/********************************** (C) COPYRIGHT *******************************
 * File Name          : M8_Protection.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Protection parameter initialization, protection state setting
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "M0_Control_Library.h"

InstPhaseOC_Type  InstPhaseOC_M;            
DCbusProtection_Type DCBusProtecion_M;      
OverLoad_Type           Overload_M;         
LPF_First_Order_Type    OverloadLPF1st_M;   
Phaseloss_Type Phaseloss_M;               

/******************************************************************************
* Function Name  : Protection_and_Emergency_Parameter_Define
* Description    : Fault protection parameter initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Protection_and_Emergency_Parameter_Define (void)
{
    InstPhaseOC_M.Cnt = 0;
    InstPhaseOC_M.CurrThreshold = INST_PHASEOC_THRE_M;
    InstPhaseOC_M.CntThreshold = INST_PHASEOC_CNT_THRE_M;

    DCBusProtecion_M.OverThresh = DCBUS_OVER_VOLT_THRE;
    DCBusProtecion_M.UnderThresh = DCBUS_UNDER_VOLT_THRE;
    DCBusProtecion_M.OverCntThresh = DCBUS_OVER_VOLT_CNT;
    DCBusProtecion_M.UnderCntThresh = DCBUS_UNDER_VOLT_CNT;
    DCBusProtecion_M.OverCnt = 0;
    DCBusProtecion_M.UnderCnt = 0;
    DCBusProtecion_M.Overvolt = FALSE;
    DCBusProtecion_M.Undervolt = FALSE;
    DCBusProtecion_M.PeakOverCnt=0;
    DCBusProtecion_M.PeakOverCntThresh=PEAKDCBUS_OVER_VOLT_CNT;
    DCBusProtecion_M.PeakOverDetectiontime=(u_int16_t)(PEAKDCBUS_OVER_VOLT_DETECTIONTIME*FREQ_SVPWM);
    DCBusProtecion_M.PeakOverDetectiontCnt=0;

    Overload_M.CurrThresh = OVERLOAD_CURR_THRE;
    Overload_M.CalCoef = OVERLOAD_CAL_COEF;
    Overload_M.CntThresh = (uint16_t)(OVERLOAD_TIME_THRE*OVERLOAD_CAL_FREQ);
    Overload_M.Cnt = 0;
    Overload_M.CurrCal = 0;
    OverloadLPF1st_M.Input_Coef = _IQdiv(OVERLOADLPF_WcT_M,(OVERLOADLPF_WcT_M + UNIT_Q24));
    OverloadLPF1st_M.Output_Coef = _IQdiv(UNIT_Q24,(OVERLOADLPF_WcT_M + UNIT_Q24));

    Phaseloss_M.CurrThresh = PHASELOSS_CURR_THRE;
    Phaseloss_M.CntThresh = (uint16_t)(PHASELOSS_TIME_THRE * PHASELOSS_CAL_FREQ);
    Phaseloss_M.MultiphaseCnt = 0;
    Phaseloss_M.SiglephaseCntA = 0;
    Phaseloss_M.SiglephaseCntB = 0;
    Phaseloss_M.SiglephaseCntC = 0;
    Phaseloss_M.CurrBuffPara.Length = PHASELOSS_CAL_BUFFSIZE;
    Phaseloss_M.CurrBuffPara.Inv1 = _IQ(1.0)/PHASELOSS_CAL_BUFFSIZE;
    Phaseloss_M.CurrBuffPara.Inv2 = _IQ(1.0) - Phaseloss_M.CurrBuffPara.Inv1;

    Tempctl_M.NTCOverThresh = NTC_OVERTEMP_RATEADC;
    Tempctl_M.NTCOpenThresh = NTC_OPENTEMP_RATEADC;
    Tempctl_M.NTCOverCntThresh = (uint16_t)(TEMP_OVER_TIME*NTC_VERIFY_FREQ);
    Tempctl_M.NTCOpenCntThresh = (uint16_t)(NTC_OPEN_TIME*NTC_VERIFY_FREQ);
    Tempctl_M.NTCOverCnt = 0;
    Tempctl_M.NTCOpenCnt = 0;
    Tempctl_M.NTCOpenStatus =FALSE;
    Tempctl_M.NTCOverStatus =FALSE;

}

/******************************************************************************
* Function Name  : Protection_and_Emergency_Init
* Description    : Fault protection parameter initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Protection_and_Emergency_Init ()
{
    InstPhaseOC_M.Cnt = 0;

    DCBusProtecion_M.OverCnt = 0;
    DCBusProtecion_M.UnderCnt = 0;

    DCBusProtecion_M.Overvolt = FALSE;
    DCBusProtecion_M.Undervolt = FALSE;
    DCBusProtecion_M.PeakOverDetectiontCnt=0;
    DCBusProtecion_M.PeakOverCnt=0;
    Overload_M.Cnt = 0;
    Overload_M.CurrCal = 0;

    Phaseloss_M.MultiphaseCnt = 0;
    Phaseloss_M.SiglephaseCntA = 0;
    Phaseloss_M.SiglephaseCntB = 0;
    Phaseloss_M.SiglephaseCntC = 0;

//    Tempctl_M.NTCOverCnt = 0;
//    Tempctl_M.NTCOpenCnt = 0;
  // Tempctl_M.NTCOverHysteresisCnt = 0;
  //  Tempctl_M.NTCOverRecoverCnt = 0;
  //  Tempctl_M.NTCOpenStatus =FALSE;
  //  Tempctl_M.NTCOverStatus =FALSE;
  // Tempctl_M.NTCOverHysStatus =FALSE;
    Tempctl_M.NTCFlickerStatus =FALSE;
}

/******************************************************************************
* Function Name  : Protection_SetFault
* Description    : Fault Setting
* Input          : Fault type
* Output         : None
* Return         : None
******************************************************************************/
void Protection_SetFault(uint16_t Fault_type)
{
    Time_Delay_Set(Operation_M.FaultHoldingSet,&(Operation_M.FaultHoldingCnt));

    TIM_CtrlPWMOutputs(TIM1, DISABLE); 

    TRIAC_OFF();

    RunningStatus_M = FAULT;           

    System_Status_Global |= Fault_type;
}

/******************************************************************************
* Function Name  : Protction_Fault_Reset_M
* Description    : Fault Reset
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Protction_Fault_Reset_M(void)
{
    if (Time_Delay_Elapse(Operation_M.FaultHoldingCnt))         
    {
        System_Status_Global &= ~STARTUP_FAILURE_M;        

        System_Status_Global &= ~SPEED_ABNORMAL_M;          
        LBG_Status_M.Unstable_CNT = 0;

        System_Status_Global &= ~MOTOR_OVER_CURR_SOFT_M;    
        InstPhaseOC_M.Cnt = 0;                              

        System_Status_Global &= ~MOTOR_OVERLOAD_M;          
        Overload_M.Cnt = 0;                                 

        if (GPIO_ReadInputDataBit(BRK_GPIO_M, BRK_PIN_M))   
          System_Status_Global &= ~DC_OVER_CURR_HARD_M;     

        System_Status_Global &= ~MOTOR_STALL_M;             
        LBG_Status_M.Stall_CNT = 0;

        System_Status_Global &= ~PHASELOSS_FALUT;           
        LBG_Status_M.Stall_CNT = 0;

        System_Status_Global &= ~NTC_OVER_ERR;             
        Tempctl_M.NTCOverCnt =0;
        Tempctl_M.NTCOverStatus =FALSE;

        System_Status_Global &= ~NTC_OPEN_ERR;              
        Tempctl_M.NTCOpenCnt =0;
        Tempctl_M.NTCOpenStatus =FALSE;

        if((System_Status_Global&DC_OVER_VOLT)==DC_OVER_VOLT)         
        {
            if(ADC_M.DCVolt_Filter < DCBusProtecion_M.OverThresh)   
            {
                System_Status_Global&= ~DC_OVER_VOLT;               
                DCBusProtecion_M.OverCnt = 0;
            }
        }
        if((System_Status_Global&DC_OVER_VOLT)==DC_UNDER_VOLT)        
        {
            if(ADC_M.DCVolt_Filter > DCBusProtecion_M.UnderThresh)  
            {
                System_Status_Global&= ~DC_UNDER_VOLT;              
                DCBusProtecion_M.UnderCnt = 0;
            }
        }
    }

    if ( (System_Status_Global & (STARTUP_FAILURE_M |SPEED_ABNORMAL_M | MOTOR_OVER_CURR_SOFT_M | MOTOR_OVERLOAD_M\
             | DC_OVER_CURR_HARD_M | MOTOR_STALL_M |PHASELOSS_FALUT|NTC_OVER_ERR|NTC_OPEN_ERR| DC_OVER_VOLT | DC_UNDER_VOLT)) == 0 )
    {
      RunningStatus_M = IDLE;                  
      System_Status_Global |= FIRST_START_M;    
    }
}

/******************************************************************************
* Function Name  : DCOvervolt_Protection
* Description    : Bus Overvoltage Protection
* Input          : Detect bus voltage, voltage protection structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void DCOvervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec)
{
    if (DCVolt >= DCbusProtec->OverThresh)
        DCbusProtec->OverCnt++;
    else
        DCbusProtec->OverCnt = 0;

    if(DCbusProtec->OverCnt > DCbusProtec->OverCntThresh)
    {
        DCBusProtecion_M.Overvolt = TRUE;
        Protection_SetFault(DC_OVER_VOLT);//Ä¸Ïß¹ýÑ¹

    }
}

/******************************************************************************
* Function Name  : PeakDCOvervolt_Protection
* Description    : Peak bus overvoltage protection
* Input          : Detect bus voltage, voltage protection structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void PeakDCOvervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec)
{
    DCbusProtec->PeakOverDetectiontCnt++;
    if(DCbusProtec->PeakOverDetectiontCnt<DCbusProtec->PeakOverDetectiontime)
    {
        if (DCVolt >= DCbusProtec->OverThresh)
            DCbusProtec->PeakOverCnt++;


        if(DCbusProtec->PeakOverCnt > DCbusProtec->PeakOverCntThresh)
        {
            DCBusProtecion_M.Overvolt = TRUE;
            Protection_SetFault(DC_OVER_VOLT);
        }
    }
    else
    {
        DCbusProtec->PeakOverCnt=0;
        DCbusProtec->PeakOverDetectiontCnt=0;
    }
}

/******************************************************************************
* Function Name  : DCUndervolt_Protection
* Description    : Bus Undervoltage Protection
* Input          : Detect bus voltage, voltage protection structure pointerv
* Output         : None
* Return         : None
******************************************************************************/
void DCUndervolt_Protection(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec)
{
    if (DCVolt <= DCbusProtec->UnderThresh)
         DCbusProtec->UnderCnt++;
    else
        DCbusProtec->UnderCnt = 0;

     if(DCbusProtec->UnderCnt > DCbusProtec->UnderCntThresh)
    {
        DCBusProtecion_M.Undervolt = TRUE;
        Protection_SetFault(DC_UNDER_VOLT);
    }
}

/******************************************************************************
* Function Name  : DCUndervolt_Protection
* Description    : Bus Undervoltage Protection
* Input          : Detect bus voltage, voltage protection structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void DCUndervolt_Protection1(_iq24 DCVolt, DCbusProtection_Type *DCbusProtec,Powercontrol_Type *PowerOper)
{
    if (((DCVolt <= DCbusProtec->UnderThresh)||(PowerOper->Poweron==FALSE))&&(Key_M.ACkey_Poweron == TRUE))
    {
        if(DCbusProtec->UnderCnt<U16_MAX)
         DCbusProtec->UnderCnt++;
    }
    else
        DCbusProtec->UnderCnt = 0;

    if(DCbusProtec->UnderCnt > DCbusProtec->UnderCntThresh)
    {
        Protection_SetFault(DC_UNDER_VOLT);
        DCBusProtecion_M.Undervolt = TRUE;
    }
}

/******************************************************************************
* Function Name  : NTC_Protection
* Description    : NTC Protection
* Input          : Temperature control structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void NTC_Protection(Temperature_ctl_Type *Temctl,_iq24 Temp)
{
    if(Temp < Temctl->NTCOverThresh)
    {
        if(Temctl->NTCOverCnt<=U16_MAX)
        Temctl->NTCOverCnt ++;
    }
    else
        Temctl->NTCOverCnt =0;


    if(Temctl->NTCOverCnt >= Temctl-> NTCOverCntThresh)
    {
        Temctl->NTCOverStatus = TRUE;
        Protection_SetFault(NTC_OVER_ERR);
    }

    if(Temp > Temctl->NTCOpenThresh)
    {
        if(Temctl->NTCOpenCnt<=U16_MAX)
        Temctl->NTCOpenCnt ++;
    }
    else
        Temctl->NTCOpenCnt =0;

    if(Temctl->NTCOpenCnt >= Temctl-> NTCOpenCntThresh)
    {
        Temctl->NTCOpenStatus = TRUE;
        Protection_SetFault(NTC_OPEN_ERR);
    }
}
