/********************************** (C) COPYRIGHT *******************************
 * File Name          : M7_Interrupt.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Interrupt configuration
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "M0_Control_Library.h"
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_BRK_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Observer_Calibration(Motor_Sructure_Type *MStruc,SVPWM_1R_Type *SVPWM_Cal,Motor_PID_Type *Mpid,\
             ADCStruc_Type *ADCStruc,Motor_LBG_Type *MLBG,PWMCali_Type *PWMCali);
uint8_t Timer1_Interrupt_Count_test = 0;   

/******************************************************************************
* Function Name  : Interrupt_Configuration
* Description    : Interrupt Priority Configuration
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Interrupt_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);             

    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;              
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             
    NVIC_Init(&NVIC_InitStructure);                            

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;          
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             
    NVIC_Init(&NVIC_InitStructure);                             

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;         
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             
    NVIC_Init(&NVIC_InitStructure);                             
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);                  
    TIM_ITConfig(TIM1, TIM_IT_Break,ENABLE);                    

    NVIC_InitStructure.NVIC_IRQChannel = SysTicK_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : Timer 1 overflow interrupt, processing waveform data output
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void TIM1_UP_IRQHandler(void)
{

    if( RunningStatus_M ==PRESTART)
    {
	 	if(Time_Delay_Elapse(SpeedRamp_M.Precharge_cnt) == TRUE)
        {
			ADC_1R_Start(ENABLE);
			RunningStatus_M = POSITION;
		}
    }

    if((RunningStatus_M == POSITION)||(RunningStatus_M == START)||(RunningStatus_M == RUN))
        SVPWM_Updatedown(&SVPWM_Cal_M, &PWMCali_M);  
    else
    {
        ADC_SoftwareStartConvCmd(ADC2, ENABLE);
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    }

    DCBUS_Volt_Cal(&ADC_M,ADC2);

    ADC_M.DCVoltcali= (_IQdiv(_IQ(1.0),ADC_M.DCVolt_Filter));

    if(RunningStatus_M == RUN)
    {
    if ((ADC_M.DCVoltcali<Triac_ctl.ACVoltcaliPrev)&&(Triac_ctl.ACVoltcaliMin>ADC_M.DCVoltcali))
        Triac_ctl.ACVoltcaliMin=ADC_M.DCVoltcali;
    }
    else
    {
        Triac_ctl.ACVoltcaliPrev= ADC_M.DCVoltcali;
        Triac_ctl.ACVoltcaliMin=ADC_M.DCVoltcali;
    }

#ifdef  PHASEANGLE_CONTROL  
    Triac_Control_Process(&Triac_ctl);
#endif
    if(SpeedRamp_M.Break == FALSE)
    {
        if((RunningStatus_M == START)||(RunningStatus_M == RUN))
         {
            Observer_Calibration(&MStruc_M,&SVPWM_Cal_M,&Mpid_M,&ADC_M,&MLBG_M,&PWMCali_M);
         }
    }
   // Waveform_Display();
    //GPIO_SetBits(GPIOD, GPIO_Pin_0);
    Powercom_Process(&Powercom_M,&MStruc_M,&Powerctr_M,&SpeedRamp_M);
    if(RunningStatus_M == RUN)
    {
        Triac_ctl.ACVoltcaliPrev= ADC_M.DCVoltcali;
        PeakDCOvervolt_Protection(ADC_M.DCVolt_Filter,&DCBusProtecion_M);
    }
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}

/******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : Brake interruption, set fault
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
    Protection_SetFault(DC_OVER_CURR_HARD_M);
    TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
}

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{

}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  while (1)
  {
      //NVIC_SystemReset();
  }
}
