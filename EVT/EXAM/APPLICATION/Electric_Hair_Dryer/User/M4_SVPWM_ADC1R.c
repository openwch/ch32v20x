/********************************** (C) COPYRIGHT *******************************
 * File Name          : M4_SVPWM_ADC1R.c
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

SVPWM_1R_Type    SVPWM_Cal_M;    
ADCStruc_Type    ADC_M;          
PWMCali_Type     PWMCali_M;      

/******************************************************************************
* Function Name  : SVPWM_1R_Paramter_Init
* Description    : SVPWM Parameter Initialization
* Input          : SVPWM calculation structure, PWM correction structure pointer,
*                                    motor base value parameter structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void SVPWM_1R_Paramter_Init(SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali,Motor_Base_Type *Mbase)
{
    SVPWM_Cal->T = ((uint16_t)(TIMER_CLOCK_FREQ_SVPWM/(uint32_t)(FREQ_SVPWM)));
    SVPWM_Cal->T_IQ16 = (_IQ16(SVPWM_Cal->T));
    SVPWM_Cal->HALF_T = SVPWM_Cal->T/2;
    SVPWM_Cal->Quard_T = SVPWM_Cal->T/4;

    SVPWM_Cal->BusVolt = _IQ18div(DCBUS_RATED_VOLT,Mbase->Vbase);

    SVPWM_Cal->coef1 = _IQ16mpy(_IQ16div(SVPWM_Cal->T_IQ16/2,SVPWM_Cal->BusVolt>>2),Three_Q16);
    SVPWM_Cal->coef2 = _IQ16mpy(_IQ16div(SVPWM_Cal->T_IQ16/2,SVPWM_Cal->BusVolt>>2),SQRT3_Q16);

    SVPWM_Cal->Maxmodulesqure = MAX_MODULATIONSQUARE;
    SVPWM_Cal->Maxmodule = MAX_MODULATION;

    PWMCali->Currconverttime = CURR_CONVERT_CLOCK_ADC;
    PWMCali->ADCTimeMin = CURR_MAX_TRTN_CLOCK_ADC + DEADTIME_CLOCK_SVPWM + CURR_CONVERT_CLOCK_ADC ;
    PWMCali->ADCTimeMinDouble = PWMCali->ADCTimeMin*2;
    PWMCali->ADCTimeMinHalf = PWMCali->ADCTimeMin/2;
}

/******************************************************************************
* Function Name  : SVPWM_Status_Initial_M
* Description    : SVPWM State Initialization
* Input          : SVPWM calculation structure, PWM correction structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void SVPWM_1R_Status_Init_M(SVPWM_1R_Type *SVPWM_Cal,PWMCali_Type *PWMCali)
{
    TIM1->CH1CVR = SVPWM_Cal->Quard_T;
    TIM1->CH2CVR = SVPWM_Cal->Quard_T;
    TIM1->CH3CVR = SVPWM_Cal->Quard_T;
    TIM1->CH4CVR = SVPWM_Cal->Quard_T - PWMCali->Currconverttime;   
    TIM3->CH4CVR = SVPWM_Cal->HALF_T - PWMCali->Currconverttime;    

    SVPWM_Cal->cc1 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc2 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc3 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc1Load = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc2Load = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc3Load = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc1Loadup = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc2Loadup = SVPWM_Cal->Quard_T;
    SVPWM_Cal->cc3Loadup = SVPWM_Cal->Quard_T;

    SVPWM_Cal->sector = 1;
    SVPWM_Cal->VoltLimMark = 0;

    SVPWM_Cal->PWMUcali1 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->PWMUcali2 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->PWMVcali1 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->PWMVcali2 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->PWMWcali1 = SVPWM_Cal->Quard_T;
    SVPWM_Cal->PWMWcali2 = SVPWM_Cal->Quard_T;

    PWMCali->Cali_Buff.Cali1 = SVPWM_Cal->Quard_T;
    PWMCali->Cali_Buff.Cali2 = SVPWM_Cal->Quard_T;
    PWMCali->Cali_Buff.Cali3 = SVPWM_Cal->Quard_T;
    PWMCali->Cali_Buff.Cali4 = SVPWM_Cal->Quard_T;
    PWMCali->Cali_Buff.Cali5 = SVPWM_Cal->Quard_T;
    PWMCali->Cali_Buff.Cali6 = SVPWM_Cal->Quard_T;

    PWMCali->ADC_Trig.Trig1 = SVPWM_Cal->Quard_T - PWMCali->Currconverttime; 
    PWMCali->ADC_Trig.Trig2 = SVPWM_Cal->HALF_T - PWMCali->Currconverttime; 
    PWMCali->ADC_Trig.Trig1_limmark = FALSE;
    PWMCali->ADC_Trig.Trig2_limmark = FALSE;

    PWMCali->DMA_Trig.Trig1 = 1;
    PWMCali->DMA_Trig.Trig2 = SVPWM_Cal->HALF_T +1;
    PWMCali->DMA_Trig.Trig3 = 1;
    PWMCali->DMA_Trig.Trig4 = SVPWM_Cal->HALF_T +1;
    PWMCali->DMA_Trig.Trig5 = 1;
    PWMCali->DMA_Trig.Trig6 = SVPWM_Cal->HALF_T +1;

    TIM2->CH3CVR = PWMCali->DMA_Trig.Trig1;
    TIM3->CH3CVR = PWMCali->DMA_Trig.Trig2;
    TIM4->CH2CVR = PWMCali->DMA_Trig.Trig3;
    TIM2->CH1CVR = PWMCali->DMA_Trig.Trig4;
    TIM3->CH1CVR = PWMCali->DMA_Trig.Trig5;
    TIM2->CH2CVR = PWMCali->DMA_Trig.Trig6;

    PWMCali->CaliMark = 0;
}

/******************************************************************************
* Function Name  : ADC_Parameter_Init
* Description    : ADC Parameter Calculation
* Input          : ADC calculation structure pointer, 
*                    motor base value parameter structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void ADC_Parameter_Init (ADCStruc_Type *ADCStruc,Motor_Base_Type *Mbase)
{
    ADCStruc->RatedCurr_ConvInv = _IQ16mpyIQX(_IQ16div(_IQ16(128),_IQ16(32767)),\
                            16,_IQ18div(CURR_SAMPLE_RANGE_ADC,Mbase->Ibase),18);

    ADCStruc->AdcChannel1 = ADC1_CURR_CHANNEL_PHASE_M; 
    ADCStruc->AdcChannel2 = ADC2_CURR_CHANNEL_PHASE_M; 

    ADCStruc->DCVoltRatedADC = _IQ18div(_IQ18mpy(DCVOLT_CONV_RATIO,DCBUS_RATED_VOLT),_IQ18(3.3))*4096;

    ADCStruc->DCVoltRatedADCInv =_IQ18div(_IQ18(1.0),ADCStruc->DCVoltRatedADC);

    ADCStruc->DCVoltAverSizeInv1 = _IQ(1.0)/DCVOLT_AVER_SIZE;
    ADCStruc->DCVoltAverSizeInv2 = _IQ(1.0) - ADCStruc->DCVoltAverSizeInv1;

    ADCStruc->VdcLPF1st.Input_Coef = _IQdiv(DCVOLT_LPF_WcT_M,(DCVOLT_LPF_WcT_M + UNIT_Q24));
    ADCStruc->VdcLPF1st.Output_Coef = _IQdiv(UNIT_Q24,(DCVOLT_LPF_WcT_M + UNIT_Q24));

    ADCStruc->DCVoltPres=0;
    ADCStruc->DCVoltAver=0;
    ADCStruc->DCVoltcali=0;
    ADCStruc->DCVolt_Filter=0;
    ADCStruc->VdcLPF1st.Out_New=0;
    ADCStruc->VdcLPF1st.Out_Pre=0;


}

/******************************************************************************
* Function Name  : ADC_Status_Init
* Description    : ADC Status Init
* Input          : ADC calculation structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void ADC_Status_Init (ADCStruc_Type *ADCStruc)
{

    ADCStruc->ADC_Result.Value1 = ADC_M.OffsetValue1;
    ADCStruc->ADC_Result.Value2 = ADC_M.OffsetValue2;

//    ADCStruc->DCVoltPres = DCVOLT_INIT;
//    ADCStruc->DCVoltAver = DCVOLT_INIT;
//    ADCStruc->DCVolt_Filter = DCVOLT_INIT;
//    ADCStruc->DCVoltcali = _IQ(1.0);
//
//    ADCStruc->VdcLPF1st.Out_New =0;
//    ADCStruc->VdcLPF1st.Out_Pre =0;
}

/******************************************************************************
* Function Name  : SVPWM_1R_Timer_Configure
* Description    : SVPWM Port and Timer Initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void SVPWM_1R_Timer_Configure()
{
    TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure={0};
    TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStructure= {0};
    TIM_OCInitTypeDef TIM1_OCInitStructure={0};
    TIM_OCInitTypeDef TIM3_OCInitStructure= {0};
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};
    DMA_InitTypeDef DMA1_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB\
                          | RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1, ENABLE);

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3\
                           | RCC_APB1Periph_TIM4, ENABLE );

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12|GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);

    TIM_DeInit(TIM1);

    TIM_TimeBaseStructInit(&TIM1_TimeBaseStructure);

    TIM1_TimeBaseStructure.TIM_Prescaler = TIMER_PRESCALER_SVPWM;

    TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;

    TIM1_TimeBaseStructure.TIM_Period = SVPWM_Cal_M.HALF_T-1;

    TIM1_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM1_TimeBaseStructure.TIM_RepetitionCounter = TIMER_UPDATE_RATE_SVPWM;

    TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseStructure);

    TIM_OCStructInit(&TIM1_OCInitStructure);

    TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

    TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

    //dummy value
    TIM1_OCInitStructure.TIM_Pulse = SVPWM_Cal_M.HALF_T/2;

    TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    //TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;   //SA2603

    TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM1_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM1_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM1_OCInitStructure);

    TIM_OCStructInit(&TIM1_OCInitStructure);

    TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

    TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

    TIM1_OCInitStructure.TIM_Pulse = SVPWM_Cal_M.HALF_T - 1;

    TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM1_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_Low;

    TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC4Init(TIM1, &TIM1_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;

    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;

    TIM_BDTRInitStructure.TIM_DeadTime = DEADTIME_CONFIGURE;

    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;

    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;

    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_SetCounter(TIM1,0);                
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);      
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); 

    DMA_DeInit(DMA1_Channel1);
    DMA_DeInit(DMA1_Channel2);
    DMA_DeInit(DMA1_Channel4);
    DMA_DeInit(DMA1_Channel5);
    DMA_DeInit(DMA1_Channel6);
    DMA_DeInit(DMA1_Channel7);

    DMA1_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                
    DMA1_InitStructure.DMA_BufferSize = 1;                            
    DMA1_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   
    DMA1_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;           
    DMA1_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA1_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA1_InitStructure.DMA_Mode = DMA_Mode_Circular;                    
    DMA1_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;            
    DMA1_InitStructure.DMA_M2M = DMA_M2M_Disable;                   


    DMA1_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CH1CVR);
    DMA1_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&(PWMCali_M.Cali_Buff.Cali1);
    DMA_Init(DMA1_Channel1, &DMA1_InitStructure);

    DMA1_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CH1CVR);
    DMA1_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&(PWMCali_M.Cali_Buff.Cali2);
    DMA_Init(DMA1_Channel2, &DMA1_InitStructure);

    DMA1_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CH2CVR);
    DMA1_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&(PWMCali_M.Cali_Buff.Cali3);
    DMA_Init(DMA1_Channel4, &DMA1_InitStructure);

    DMA1_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CH2CVR);
    DMA1_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&(PWMCali_M.Cali_Buff.Cali4);
    DMA_Init(DMA1_Channel5, &DMA1_InitStructure);

    DMA1_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CH3CVR);
    DMA1_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&(PWMCali_M.Cali_Buff.Cali5);
    DMA_Init(DMA1_Channel6, &DMA1_InitStructure);

    DMA1_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM1->CH3CVR);
    DMA1_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&(PWMCali_M.Cali_Buff.Cali6);
    DMA_Init(DMA1_Channel7, &DMA1_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    DMA_Cmd(DMA1_Channel6, ENABLE);
    DMA_Cmd(DMA1_Channel7, ENABLE);

    TIM_DeInit(TIM3);
    TIM_DeInit(TIM2);
    TIM_DeInit(TIM4);

    TIM_TimeBaseStructInit(&TIM3_TimeBaseInitStructure);
    TIM3_TimeBaseInitStructure.TIM_Prescaler = TIMER_PRESCALER_SVPWM;   
    TIM3_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM3_TimeBaseInitStructure.TIM_Period = SVPWM_Cal_M.T-1;           

    TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseInitStructure);
    TIM_TimeBaseInit(TIM2, &TIM3_TimeBaseInitStructure);
    TIM_TimeBaseInit(TIM4, &TIM3_TimeBaseInitStructure);

    TIM_OCStructInit(&TIM3_OCInitStructure);
    TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             
    TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      

    TIM_OC3Init(TIM2, &TIM3_OCInitStructure);   
    TIM_OC3Init(TIM3, &TIM3_OCInitStructure);   
    TIM_OC2Init(TIM4, &TIM3_OCInitStructure);   
    TIM_OC1Init(TIM2, &TIM3_OCInitStructure);   
    TIM_OC1Init(TIM3, &TIM3_OCInitStructure);   
    TIM_OC2Init(TIM2, &TIM3_OCInitStructure);   

    TIM_OC4Init(TIM3, &TIM3_OCInitStructure);  

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);   
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);   
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  

    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);   

    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  

    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    TIM_SelectInputTrigger(TIM3, TIM_TS_ITR0);        
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);     

    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);  
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);              
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);         

    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);   
    TIM_SelectInputTrigger(TIM4, TIM_TS_ITR0);              
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);        

    TIM_SetCounter(TIM3,0);
    TIM_SetCounter(TIM2,0);
    TIM_SetCounter(TIM4,0);
}

/******************************************************************************
* Function Name  : ADC_OPA_Configure
* Description    : ADC/OPA Configuration
* Input          : ADC calculation structure pointer, ADC register pointer
* Output         : None
* Return         : None
******************************************************************************/
void ADC_OPA_Configure(ADCStruc_Type *ADCStruc)
{

    ADC_InitTypeDef ADC_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};
    OPA_InitTypeDef  OPA1_InitStructure = {0};
    OPA_InitTypeDef  OPA2_InitStructure = {0};

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB\
                           |RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1\
                           | RCC_APB2Periph_ADC2, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_2  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;//CH4 CH5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    OPA2_InitStructure.OPA_NUM = OPA2;
    OPA2_InitStructure.PSEL = CHP1;
    OPA2_InitStructure.NSEL = CHN0;
    OPA2_InitStructure.Mode = OUT_IO_OUT1;
    OPA_Init( &OPA2_InitStructure );
    OPA_Cmd( OPA2, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    OPA1_InitStructure.OPA_NUM = OPA1;
    OPA1_InitStructure.PSEL = CHP1;
    OPA1_InitStructure.NSEL = CHN1;
    OPA1_InitStructure.Mode = OUT_IO_OUT1;
    OPA_Init( &OPA1_InitStructure );
    OPA_Cmd( OPA1, ENABLE );

    ADC_DeInit(ADC1);
    ADC_DeInit(ADC2);

    ADC_StructInit(&ADC_InitStructure);                 
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;        
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
    ADC_InitStructure.ADC_NbrOfChannel = 1;            
    ADC_Init(ADC1, &ADC_InitStructure);                 
    ADC_Init(ADC2, &ADC_InitStructure);                 

    ADC_RegularChannelConfig(ADC2,ADC_Channel_1, 1, SAMPLING_TIMER_CLOCK );

    ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 1, SAMPLING_TIMER_CLOCK );

    ADC_Cmd(ADC1, ENABLE); 
    ADC_Cmd(ADC2, ENABLE); 

    ADC_BufferCmd(ADC1, DISABLE); //disable buffer
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    ADC_BufferCmd(ADC2, DISABLE); //disable buffer
    ADC_ResetCalibration(ADC2);
    while(ADC_GetResetCalibrationStatus(ADC2));
    ADC_StartCalibration(ADC2);
    while(ADC_GetCalibrationStatus(ADC2));
}

/******************************************************************************
* Function Name  : ADC_1R_Offset_Reading
* Description    : ADC current sampling zero reading
* Input          : ADC register pointer, ADC calculation structure pointer, ADC number
* Output         : None
* Return         : None
******************************************************************************/
void ADC_1R_Offset_Reading(ADC_TypeDef *ADCx, ADCStruc_Type *ADCStruc,u_int8_t ADC_num)
{
    uint16_t i;

    if(ADC_num ==1)
        ADCStruc->OffsetValue1 = 0;
    else if (ADC_num ==2)
        ADCStruc->OffsetValue2 = 0;

    ADC_ITConfig(ADCx, ADC_IT_JEOC, DISABLE);

    ADC_ExternalTrigInjectedConvConfig(ADCx, ADC_ExternalTrigInjecConv_None);

    ADC_ExternalTrigInjectedConvCmd(ADCx,ENABLE);

    ADC_InjectedSequencerLengthConfig(ADCx,1);

    if(ADC_num ==1)
        ADC_InjectedChannelConfig(ADCx, ADCStruc->AdcChannel1,1,SAMPLING_TIMER_CLOCK);
    else if (ADC_num ==2)
        ADC_InjectedChannelConfig(ADCx, ADCStruc->AdcChannel2,1,SAMPLING_TIMER_CLOCK);

    ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);

    ADC_SoftwareStartInjectedConvCmd(ADCx,ENABLE);

    for(i=0; i <CURR_OFFSET_READ_NUM_ADC; i++)
    {
        while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_JEOC));

        if(ADC_num ==1)
            ADCStruc->OffsetValue1 += (ADC_GetInjectedConversionValue(ADCx,ADC_InjectedChannel_1)>>3);
        else if (ADC_num ==2)
            ADCStruc->OffsetValue2 += (ADC_GetInjectedConversionValue(ADCx,ADC_InjectedChannel_1)>>3);

        ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);
        ADC_SoftwareStartInjectedConvCmd(ADCx,ENABLE);
    }

    ADC_ExternalTrigInjectedConvCmd(ADCx,ENABLE);
}

/******************************************************************************
* Function Name  : ADC_1R_Start
* Description    : ADC Conversion Start
* Input          : Enable command
* Output         : None
* Return         : None
******************************************************************************/
void ADC_1R_Start (FunctionalState cmd)
{
    if (cmd == ENABLE)
    {
        ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);
        ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_T3_CC4);

        ADC1->STATR = ~(uint32_t)ADC_FLAG_JEOC;
        ADC2->STATR = ~(uint32_t)ADC_FLAG_JEOC;

        ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
        ADC_ITConfig(ADC2, ADC_IT_JEOC, ENABLE);

        TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);
        TIM_DMACmd(TIM2, TIM_DMA_CC2, ENABLE);
        TIM_DMACmd(TIM2, TIM_DMA_CC3, ENABLE);
        TIM_DMACmd(TIM3, TIM_DMA_CC1, ENABLE);
        TIM_DMACmd(TIM3, TIM_DMA_CC3, ENABLE);
        TIM_DMACmd(TIM4, TIM_DMA_CC2, ENABLE);
    }
    else
    {

        ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);
        ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_None);

        TIM_DMACmd(TIM2, TIM_DMA_CC1, DISABLE);
        TIM_DMACmd(TIM2, TIM_DMA_CC2, DISABLE);
        TIM_DMACmd(TIM2, TIM_DMA_CC3, DISABLE);
        TIM_DMACmd(TIM3, TIM_DMA_CC1, DISABLE);
        TIM_DMACmd(TIM3, TIM_DMA_CC3, DISABLE);
        TIM_DMACmd(TIM4, TIM_DMA_CC2, DISABLE);

        ADC_ExternalTrigInjectedConvCmd(ADC1,DISABLE);
        ADC_ExternalTrigInjectedConvCmd(ADC2,DISABLE);
        TIM_CtrlPWMOutputs(TIM1, DISABLE);
    }
}

/******************************************************************************
* Function Name  : DCBUS_Volt_Cal
* Description    : Bus Voltage Calculation
* Input          : ADC calculation structure pointer, ADC register pointer
* Output         : None
* Return         : None
******************************************************************************/
void DCBUS_Volt_Cal(ADCStruc_Type *ADCStruc,ADC_TypeDef *ADCx)
{
    if((ADCx->STATR & ADC_FLAG_EOC) == ADC_FLAG_EOC)
   {
    ADCStruc->DCVoltPres=_IQ18toIQ(_IQ18MPY_MACRO(_IQ18(ADCx->RDATAR>>4),ADCStruc->DCVoltRatedADCInv));

    ADCStruc->DCVolt_Filter = LPF_1st_Process(&(ADCStruc->VdcLPF1st), ADCStruc->DCVoltPres);

    ADCStruc->DCVoltAver = _IQMPY_MACRO(ADCStruc->DCVoltAver,ADCStruc->DCVoltAverSizeInv2)\
                            + _IQMPY_MACRO(ADCStruc->DCVoltPres,ADCStruc->DCVoltAverSizeInv1);
   }
}

/******************************************************************************
* Function Name  : NTCSample
* Description    : NTC Sampling Calculation
* Input          : Temperature control structure pointer, ADC register pointer
* Output         : None
* Return         : None
******************************************************************************/
void NTCSample(Temperature_ctl_Type *TempM, ADC_TypeDef *ADCx)
{
    if((ADCx->STATR & ADC_FLAG_EOC) == ADC_FLAG_EOC)
    {
        TempM->TempPres=(_iq24)((u16)ADCx->RDATAR*TempM->TempRatedADCInv);
        //¾ùÖµ¼ÆËã


        TempM->TempAver =_IQMPY_MACRO(TempM->TempAver,TempM->TempAverSizeInv2)\
                + _IQMPY_MACRO(TempM->TempPres,TempM->TempAverSizeInv1);

    }
}
/******************************************************************************
* Function Name  : ADC_Trigger_Update
* Description    : Update at ADC trigger moment
* Input          : PWM correction calculation structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void ADC_Trigger_Update(PWMCali_Type *PWMCali)
{
    TIM1->CH4CVR = PWMCali->ADC_Trig.Trig1;
    TIM3->CH4CVR = PWMCali->ADC_Trig.Trig2;
}

/*******************************************************************************
* Function Name  : SVPWM_Zerovolt_Output_Immidiate
* Description    : 0 voltage output, takes effect immediately 
* Input          : SVPWM calculation structure pointer
* Output         : None
* Return         : None
*******************************************************************************/
void SVPWM_Zerovolt_Output_Immediate(SVPWM_1R_Type *SVPWM_Cal)
{
    TIM1->CH1CVR = SVPWM_Cal->Quard_T;
    TIM1->CH2CVR = SVPWM_Cal->Quard_T;
    TIM1->CH3CVR = SVPWM_Cal->Quard_T;
}

/*******************************************************************************
* Function Name  : SVPWM_Lowside_Effective
* Description    : All lower tubes are fully conductive and take effect immediately
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVPWM_Lowside_Effective_Immediate(void)
{
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = 0;
}
