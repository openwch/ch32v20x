/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_SpeedLoop.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Speed loop processing, related protection treatment, power up and down control processing
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "M0_Control_Library.h"

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
static uint8_t Speed_Sample_PID_CNT = SPEED_SAMPLE_PID_PERIOD;  

/******************************************************************************
* Function Name  : SysTick_Handler
* Description    : Speed loop processing interrupt
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/

void SysTick_Handler(void)
{
    Speed_Sample_PID_CNT --;

    Time_Delay_Process();

    Led_Operation(&LED_M);

    Poweroperation_Process(&SpeedRamp_M,&ADC_M,&Powerctr_M,&RunningStatus_M,&(Mpid_M.Speed),\
                            MLBG_M.Cal_Struc.Angspeed_Aver2);

    NTCSample(&Tempctl_M,ADC1); 

    Coldkeycheck(&Key_M);

    Tempkeycheck(&Key_M);

    ACKEY_CHECK(&Key_M);

    if(RunningStatus_M == IDLE)
        PSC_Process(&Operation_M);

    if((Operation_M.PSC.Mark == FALSE)&&(Operation_M.PSC.Process ==TRUE))
        Speedkeycheck_Recover(&Key_M,&SpeedRamp_M);

    Event_1ms_Process(&Emanager_M);

    ACZerocross_Process(&Tempctl_M,&Key_M,&Triac_ctl);

    if (Speed_Sample_PID_CNT == 0 )
    {
        Speed_Sample_PID_CNT = SPEED_SAMPLE_PID_PERIOD;

        Buffer_Update(&(LBG_SpeedBuffer_M[0]), &(MLBG_M.SpeedBuffPara), MLBG_M.Cal_Struc.Angspeed_Inst);

        MLBG_M.Cal_Struc.Angspeed_Aver1 = LPF_1st_Process(&(MLBG_M.Cal_Struc.SpeedLPF1st_M),\
                                          MLBG_M.Cal_Struc.Angspeed_Inst);

        LBG_Status_M.Reliable =  Speed_Reliable_Verify(&(MLBG_M.SpeedBuffPara),&LBG_SpeedBuffer_M[0],\
                                MLBG_M.Cal_Struc.Angspeed_Aver1,LBG_Status_M.VarianceThresh);

        switch(RunningStatus_M)
        {
            case START:
                if(_IQabs(InjStart_M.RampInjSpeedTarget)<_IQ(0.2))
                    LBG_Status_M.Reliable = TRUE;

                Speed_Converged_Verify1(InjStart_M.RampInjSpeedRef, &MLBG_M, &LBG_Status_M);
                break;

            case RUN:
                if(SpeedRamp_M.Break == FALSE)
                {
                    if((Key_M.Skeystop == FALSE)&&(Operation_M.Faultstop == FALSE)&&(Powerctr_M.Poweron == TRUE))
                    {
                        VDC_Limitspeed(&SpeedRamp_M,&ADC_M,&Powerctr_M);
                    }
                    if(Powerctr_M.Poweron == TRUE)
                        Speed_Ramp_Process(&SpeedRamp_M);
                    else
                        Poweroff_Speed_Ramp_Process(&SpeedRamp_M);

	                if(STARTUP_DIRECTION==1)
	                    SpeedRamp_M.Aftrampcom = SpeedRamp_M.Aftramp + Powercom_M.Speedcom;
	                else if(STARTUP_DIRECTION==-1)
	                    SpeedRamp_M.Aftrampcom = SpeedRamp_M.Aftramp - Powercom_M.Speedcom;

                    Speed_Stable_Verify(&LBG_Status_M, MLBG_M.Cal_Struc.Angspeed_Aver1);
                    if(LBG_Status_M.Stable==FALSE)
                        Protection_SetFault(SPEED_ABNORMAL_M);

                    Motor_Stall_Verify1(SpeedRamp_M.Aftramp, &MLBG_M, MStruc_M.CRef2R.cq,&LBG_Status_M);
                    if(LBG_Status_M.Stall==TRUE)
                      Protection_SetFault(MOTOR_STALL_M);

                    MStruc_M.CRef2R.cq = PID_Calculaion_Delta (SpeedRamp_M.Aftrampcom, MLBG_M.Cal_Struc.Angspeed_Aver2, &(Mpid_M.Speed));

                    if((DCurrProcess_M.Stage == 1)&&(_IQabs(MStruc_M.CRef2R.cd) <= DCurrProcess_M.Target1))
                        Dcurr_Process_Init(&DCurrProcess_M,2);

                    MStruc_M.CRef2R.cd = Dcurr_Process(&DCurrProcess_M, MLBG_M.Cal_Struc.Angspeed_Aver1, MStruc_M.CRef2R.cd);
                }
                break;
        default:
                break;
        }
    }

    DCOvervolt_Protection(ADC_M.DCVoltAver, &DCBusProtecion_M);

    DCUndervolt_Protection1(ADC_M.DCVoltAver, &DCBusProtecion_M,&Powerctr_M);

    NTC_Protection(&Tempctl_M,Tempctl_M.TempAver);

    SysTick->SR &= ~(1 << 0);
}

/************************************************************************************************************
* Function Name  : Systick_Init
* Description    : Systick Init
* Input          : NewState - ENABLE or DISABLE.
* Output         : None
* Return         : None
*************************************************************************************************************/
void Systick_Init(void)
{
    SysTick->CTLR  =0;
    SysTick->CMP   =0;
    SysTick->SR   &= ~(1 << 0);
    SysTick->CNT   = 0;
    SysTick->CMP   = TIMER_PERIDO_SYSTICK; 
    SysTick->CTLR |=(1 << 1)|(1 << 3)|(1 << 5);
}

/************************************************************************************************************
* Function Name  : SYS_TIM_Cmd
* Description    : SysTick Enable
* Input          : NewState - ENABLE or DISABLE.
* Output         : None
* Return         : None
*************************************************************************************************************/
void SYS_TIM_Cmd(FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        SysTick->CTLR |= TIM_CEN;
    }
    else
    {
        SysTick->CTLR &= (uint16_t)(~((uint16_t)TIM_CEN));
    }
}
