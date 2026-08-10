/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_CurrentLoop.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : ADC sampling result reading, current loop processing, FOC calculation, overcurrent phaseloss protection processing
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "M0_Control_Library.h"

void ADC1_2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern void FOC_Cal(Motor_Sructure_Type *MStruc,SVPWM_1R_Type *SVPWM_Cal,Motor_PID_Type *Mpid,\
        ADCStruc_Type *ADCStruc,Motor_LBG_Type *MLBG,PWMCali_Type *PWMCali);

uint8_t ADC_1Rsample_Counter_test = 0;    
uint8_t Observer_countertest = 0; 

/******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : ADC1&2 Interrupt response function, read current conversion results
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void ADC1_2_IRQHandler(void)
{
   // ADC_1Rsample_Counter_test++;

    if((ADC1->STATR & ADC_FLAG_JEOC) == ADC_FLAG_JEOC)  
    {
        ADC_M.ADC_Result.Value1= (ADC1->IDATAR1)<<1;    
        ADC1->STATR = ~(uint32_t)ADC_FLAG_JEOC;         
    }
    if((ADC2->STATR & ADC_FLAG_JEOC) == ADC_FLAG_JEOC)  
    {
 //       GPIO_SetBits(GPIOB, GPIO_Pin_8);
        ADC_M.ADC_Result.Value2= (ADC2->IDATAR1)<<1;    

        if(SpeedRamp_M.Break == TRUE)
            SVPWM_Lowside_Effective(&SVPWM_Cal_M);
        else
        {
            switch(RunningStatus_M)
            {
                case POSITION:
                    HFVQI_PosiSearch_Process(&HFVQI_StartStruc_M,&SVPWM_Cal_M,\
                                                  &PWMCali_M,&ADC_M,&InjStart_M,SpeedRamp_M.Direction);
                    if(HFVQI_StartStruc_M.Finish == TRUE)
                    {
                        RunningStatus_M = START;
                        InjStart_M.Status = RAMP;
                    }
                  break;

                case START:
                    Injstart_Process_M(&InjStart_M,&MStruc_M,&MLBG_M);

                    if (LBG_Status_M.Convergence == TRUE)
                    {
                        Controlloop_Initial(&InjStart_M, MLBG_M.Cal_Struc.Angspeed_Aver1,&MStruc_M,&Mpid_M,&SpeedRamp_M);
                        Time_Delay_Set(Triac_ctl.TonEnable_cntset,&Triac_ctl.TonEnable_cnt);
                        RunningStatus_M = RUN;
                    }
                    FOC_Cal(&MStruc_M,&SVPWM_Cal_M,&Mpid_M,&ADC_M,&MLBG_M,&PWMCali_M);
                    break;

                case RUN:
                    FOC_Cal(&MStruc_M,&SVPWM_Cal_M,&Mpid_M,&ADC_M,&MLBG_M,&PWMCali_M);
                    if(Triac_ctl.TonEnable==0)
                    {
                        if(Time_Delay_Elapse(Triac_ctl.TonEnable_cnt) == TRUE)
                            Triac_ctl.TonEnable=1;
                    }
#ifdef   ZEROCROSSOVER_OC
                    AC_Zerocross_Detect_OC(&Triac_ctl);
#else
                    AC_Zerocross_Detect_TR(&Triac_ctl);
#endif

                    break;

                default:
                  break;
            }

            if ( ((RunningStatus_M==START)||(RunningStatus_M==RUN))&&(Powerctr_M.Poweron == TRUE) )
            {
                if(Phaseloss_Verify(&(MStruc_M.C3S),&Phaseloss_M ,SVPWM_Cal_M.sector))
                    Protection_SetFault(PHASELOSS_FALUT);
            }
        }
        ADC_Trigger_Update(&PWMCali_M);

        SVPWM_Updateup(&SVPWM_Cal_M, &PWMCali_M);

        if ((RunningStatus_M==START)||(RunningStatus_M==RUN))
        {
            if(Phase_Ins_OverCurrent_Verify(&(MStruc_M.C3S),&InstPhaseOC_M) == TRUE)
                Protection_SetFault(MOTOR_OVER_CURR_SOFT_M);
        }

        Break_Verify(&Powerctr_M,&SpeedRamp_M,&RunningStatus_M,&ADC_M);

        ADC_SoftwareStartConvCmd(ADC2, ENABLE);
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
        ADC2->STATR = ~(uint32_t)ADC_FLAG_JEOC;

//        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    }
}
