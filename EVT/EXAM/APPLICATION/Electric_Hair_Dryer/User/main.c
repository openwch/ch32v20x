/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "M0_Control_Library.h"
#include "ch32v20x_conf.h"
#include "IQmath_RV32.h"
#include "debug.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    Delay_Init();
    Delay_Ms(100);
    Flash_Operation_Init(&Flash_M);    //Post 4K FLASH Erase and Read
    KeyandLED_IOInit();                //Button and indicator light IO configuration
    KeyandLED_Parameter_Define(&Key_M, &LED_M);    //Button and indicator parameters definition
    Global_Status_Initial();                       //Initialization of global state	
    Triac_Parameter_Define(&Triac_ctl,&Tempctl_M); //SCR State Initialization
    Operation_Parameter_Define(&Operation_M);      //Operation Parameter Definition
    Motor_Prameter_Define(&MBase_M);               //Motor Parameter Definition
    Motor_FilerPara_Cal_M(&MStruc_M);              //Motor Phase Filter Parameter Initialization
    Motor_BaseCal_M(&MBase_M);                     //Base Value Parameter Calculation
    PID_Prameter_Define(&Mpid_M);                  //PID Parameter Definition
    DCurrProcess_Parameter_Define(&DCurrProcess_M);    
    PID_ParaBase_Cal_M(&MBase_M,&Mpid_M);              //PID Base Value Parameter Calculation
    Injstart_Parameter_Define_M(&InjStart_M);          //Current Injection Start-up Parameter Configuration
    Protection_and_Emergency_Parameter_Define();       //Protection Parameter Configuration
    Speedrampctr_Parameter_Define(&SpeedRamp_M);       //Rotational Speed Operation Parameter Configuration
    SVPWM_1R_Paramter_Init(&SVPWM_Cal_M,&PWMCali_M ,&MBase_M);    //SVPWM Calculation Parameter Configuration
    SVPWM_1R_Timer_Configure();                                   //SVPWM uses timer and GPIO configuration
    ADC_Parameter_Init(&ADC_M,&MBase_M);             //ADC Calculation Parameter Configuration
    ADC_OPA_Configure(&ADC_M);                       //ADC and OPA configuration
    TempInit_Measure(&Tempctl_M,ADC1);               //Initial temperature measurement
    LBG_Parameter_Define(&MLBG_M,&LBG_Status_M);     //LBG Observer Parameter Definition
    LBG_Paramter_Cal (&MBase_M,&MLBG_M);             //LBG Observer Parameter Calculation
    HFVQI_Parameter_Define(&HFVQI_StartStruc_M,&SVPWM_Cal_M,&PWMCali_M);    //Pulse Positioning Parameter Configuration
    Poweroperation_Initial(&Powerctr_M);                                    //Power On/Off Control Parameter Configuration
    Powercom_Prameter_Define(&Powercom_M);
    Systick_Init();                //Speed Loop Configuration
    Interrupt_Configuration();    //Configure Interrupt 
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    SYS_TIM_Cmd(ENABLE);
    while(1)
    {
        Motor_Operation_Process();    //Motor Operation
    }
}
