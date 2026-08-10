/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_MotorStart.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Initialization of the motor before starting
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "M0_Control_Library.h"

InjStart_Type InjStart_M; 

/******************************************************************************
* Function Name  : Motor_Start_Init
* Description    : Motor Start Init
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Motor_Start_Init_M(void)
{
    LED_OFF(&LED_M);

    KeyandLED_Status_Ini(&Key_M,&LED_M);

    Operation_Status_Init(&Operation_M);

    Eventmanager_Initial(&Emanager_M);

    Speedrampctr_Start_Init_M(&SpeedRamp_M);

    Speed_Set(&SpeedRamp_M);   

    Temp_Set(&Key_M,&SpeedRamp_M,&Triac_ctl);

    Protection_and_Emergency_Init();

    PID_Output_Init(&Mpid_M);

    Motor_Status_Initial_M(&MStruc_M);

    Injstart_Status_Init_M(&InjStart_M);

    Dcurr_Process_Init(&DCurrProcess_M,1);

    SVPWM_1R_Status_Init_M(&SVPWM_Cal_M,&PWMCali_M);

    LBG_Status_Init(&MLBG_M,&LBG_Status_M);

    Buffer_Clear(&(LBG_SpeedBuffer_M[0]),&(MLBG_M.SpeedBuffPara));

    HFVQI_Status_Init_M(&HFVQI_StartStruc_M);

    Buffer_Clear(&(HFVQIPosi_CBuff_M[0]),&HFVQIPosi_CBuffPara_M);

    Powercom_Status_Init(&Powercom_M);

    ADC_1R_Offset_Reading(ADC1,&ADC_M,1);
    ADC_1R_Offset_Reading(ADC2,&ADC_M,2);

    ADC_Status_Init (&ADC_M);

    Triac_Status_Init(&Triac_ctl,&Tempctl_M);

    SVPWM_Zerovolt_Output_Immediate(&SVPWM_Cal_M);

    TIM_CtrlPWMOutputs(TIM1,ENABLE);

    TempLed_Set(&Key_M,&LED_M);

    SpeedLed_Set(&SpeedRamp_M,&LED_M);
}

/******************************************************************************
* Function Name  : Injstart_Parameter_Define_M
* Description    : Definition of Current Injection Start Parameters
* Input          : Pointer to motor start control structure
* Output         : None
* Return         : None
******************************************************************************/
void Injstart_Parameter_Define_M (InjStart_Type *InjStart)
{

    InjStart->ForceInjCurrStep = _IQdiv(FORCE_INJ_CURR_M,FORCE_INJ_CURR_RAMP_TIME_M)/FORCE_INJ_FREQ_M;

    InjStart->ForceInjCurrTarget = FORCE_INJ_CURR_M;

    InjStart->ForceInjCurrAngle = FORCE_INJ_ANGLE_M;

    InjStart->ForceInjStage = RAMP_INJ_STAGE_M;

    InjStart->RampInjCurrTarget = RAMP_INJ_CURR_FINAL_M;

    InjStart->RampInjCurrStep = _IQdiv((RAMP_INJ_CURR_FINAL_M - RAMP_INJ_CURR_INIT_M),\
                                  RAMP_INJ_CURR_RAMP_TIME_M)/RAMP_INJ_FREQ_M;

    InjStart->RampInjSpeedTarget = RAMP_INJ_SPEED_M*STARTUP_DIRECTION;

    InjStart->RampInjSpeedStep = _IQdiv(RAMP_INJ_SPEED_M,RAMP_INJ_SPEED_RAMP_TIME_M)/RAMP_INJ_FREQ_M;

    InjStart->K_WetoPosi = RAMP_INJ_PERIOD_M*RATED_FREQ_M;

    InjStart->CurrErrThresh = INJECTION_CURRERR_THRESH_M;

    InjStart->RampInjStage1 = RAMP_INJ_STAGE1_M;
    InjStart->RampInjStage2 = RAMP_INJ_STAGE2_M;

    InjStart->Speedadj = RAMP_INJ_SPEED_ADJ*STARTUP_DIRECTION;
    InjStart->Curradj = RAMP_INJ_CURR_ADJ;

    InjStart->IF_Vicom.Kcom = IF_VICOM_COEF_M * STARTUP_DIRECTION;
    InjStart->IF_Vicom.Wecomupper = IF_VICOM_UPPER_M;
    InjStart->IF_Vicom.Wecomlower = IF_VICOM_LOWER_M;
    InjStart->IF_Vicom.Power_HPF1st.Input_Coef=_IQdiv(UNIT_Q24,(IF_VICOM_HPF1st_WcT_M + UNIT_Q24));
    InjStart->IF_Vicom.Power_HPF1st.Output_Coef=_IQdiv(UNIT_Q24,(IF_VICOM_HPF1st_WcT_M + UNIT_Q24));
}

/******************************************************************************
* Function Name  : Injstart_Status_Init_M
* Description    : Current injection start-up state initialization
* Input          : Pointer to motor start control structure
* Output         : None
* Return         : None
******************************************************************************/
void Injstart_Status_Init_M(InjStart_Type *InjStart)
{
    InjStart->ForceInjCurrRef = _IQ(0);                 
    InjStart->ForceInjCounter = 0;                      

    InjStart->RampInjCurrRef = RAMP_INJ_CURR_INIT_M;    
    InjStart->RampInjSpeedRef = RAMP_INJ_SPEEDINT_M *STARTUP_DIRECTION;   
    InjStart->RampInjAngle = _IQ(0);                    
    InjStart->RampInjCounter =0;                        
    InjStart->Curr_2R.cd = _IQ(0);                      
    InjStart->Curr_2R.cq = _IQ(0);                      
    InjStart->VoltRef_2R.cd = _IQ(0);                   
    InjStart->VoltRef_2R.cq = _IQ(0);                   
    InjStart->Status = INITIAL;                        

    InjStart->InjStage=0;

    InjStart->IF_Vicom.Powerinst = 0;
    InjStart->IF_Vicom.Powerfilter = 0;
    InjStart->IF_Vicom.Wecom = 0;
    InjStart->IF_Vicom.Power_HPF1st.Out_New = 0;
    InjStart->IF_Vicom.Power_HPF1st.Out_Pre = 0;
    InjStart->IF_Vicom.Power_HPF1st.In_Pre = 0;
}

/******************************************************************************
* Function Name  : Injstart_Process_M
* Description    : Injection start, first force positioning, then current closed-loop start
* Input          : Pointer to motor start control structure,
*                  pointer to overall motor status structure,
*                  pointer to LBG observer structure
* Output         : None
* Return         : None
******************************************************************************/
void Injstart_Process_M(InjStart_Type *InjStart,Motor_Sructure_Type *MStruc,Motor_LBG_Type *MLBG)
{
    switch(InjStart->Status)//Æô¶¯×´Ì¬»ú
    {
        case INITIAL:
            InjStart->Status = INJECTION;
            break;

        case INJECTION:

            if(InjStart->ForceInjCounter < U32_MAX)
                InjStart->ForceInjCounter++;

            if(InjStart->ForceInjCurrRef < InjStart->ForceInjCurrTarget)
            {
                
                InjStart->ForceInjCurrRef += InjStart->ForceInjCurrStep;
                if(InjStart->ForceInjCurrRef > InjStart->ForceInjCurrTarget)
                    InjStart->ForceInjCurrRef = InjStart->ForceInjCurrTarget;

                MStruc->CRef2R.cd = InjStart->ForceInjCurrRef; 
                MStruc->CRef2R.cq = 0;                        

                MStruc->Cossin = TrigonoMetric_Function( InjStart->ForceInjCurrAngle);
            }
            else
            {
                if( _IQabs(MStruc->C2R_L.cd)>=_IQMPY_MACRO(InjStart->CurrErrThresh,
                     _IQabs(MStruc->CRef2R.cd)))
                {
                    if( InjStart->ForceInjCounter >= InjStart->ForceInjStage)
                    {
                        InjStart->RampInjCurrRef = InjStart->ForceInjCurrRef; 
                        InjStart->RampInjAngle = InjStart->ForceInjCurrAngle; 
                        InjStart->Status = RAMP;                              
                    }
                }
                else
                   Protection_SetFault(STARTUP_FAILURE_M); 
           }
            break;
        case RAMP:
            if(InjStart->RampInjCounter < U32_MAX)
                InjStart->RampInjCounter++;

            Injstart_Curr_Speed_Ramp(InjStart);

            #if defined IF_VIBRATION_COM
            IF_Vibration_Com(&(MStruc->VRef2R), &(MStruc->C2R_L), InjStart);
            InjStart->RampInjAngle = InjStart->RampInjAngle +\
                                     _IQMPY_MACRO(InjStart->RampInjSpeedRef2,InjStart->K_WetoPosi);
            #else
            InjStart->RampInjAngle = InjStart->RampInjAngle +\
                                     _IQMPY_MACRO(InjStart->RampInjSpeedRef,InjStart->K_WetoPosi);
            #endif

            InjStart->RampInjAngle = Round_Circle_Limitation_PU(InjStart->RampInjAngle);

            MStruc->Cossin = TrigonoMetric_Function(InjStart->RampInjAngle);

            MStruc->CRef2R.cd = InjStart->RampInjCurrRef;
            MStruc->CRef2R.cq = 0;

            if(InjStart->RampInjCounter >= (InjStart->RampInjStage1))
            {
                if(_IQabs(MStruc->C2R_L.cd)>=_IQMPY_MACRO(InjStart->CurrErrThresh,\
                       _IQabs(MStruc->CRef2R.cd)))
                {
                    if(MLBG->Cal_Struc.Inimark == TRUE)
                        Injstart_Transition_Cal(InjStart,MStruc,MLBG);
                    else
                    {
                        LBG_Cal_Init(InjStart,MStruc,MLBG);
                        MLBG->Cal_Struc.Inimark = TRUE;
                    }
                }
                else 
                    Protection_SetFault(STARTUP_FAILURE_M);
            }
            else
            {
                MLBG->Cal_Struc.CalphaPrev = MStruc->C2S.Alpha;
                MLBG->Cal_Struc.CbetaPrev = MStruc->C2S.Beta;
            }
            if (InjStart->RampInjCounter >=InjStart->RampInjStage2)
                    Protection_SetFault(STARTUP_FAILURE_M);
           break;
        default:
            break;
    }
}
