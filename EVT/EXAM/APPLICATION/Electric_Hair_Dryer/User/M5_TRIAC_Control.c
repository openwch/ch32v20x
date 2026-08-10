/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_TRIAC_Control.c
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

TRIAC_ctl_Type Triac_ctl;
//u16 Temp_PhaseDuty[SPEEDPOINT_RANK][TKEY_RANK_SET+1]={{0,85,110,125},{0,110,125,140},{0,125,145,165}};
_iq24 DutyLim_table[256]=DUTY_LIM_TABLE;
/******************************************************************************
* Function Name  : Triac_Parameter_Define
* Description    : Silicon Controlled Rectifier Control Parameter Initialization
* Input          : Thyristor control pointer Temperature control pointer
* Output         : None
* Return         : None
******************************************************************************/
void Triac_Parameter_Define(TRIAC_ctl_Type *TriacM,Temperature_ctl_Type *TempM)
{
    TriacM->PeriodCtl.NumData=NUM50HZ_DATA;

    TriacM->PeriodCtl.Period = (uint16_t)(TRIAC_TIME/TRIAC_CONTOROL_PERIOD);
    TriacM->PeriodCtl.HalfPeriod = (TriacM->PeriodCtl.Period -1)/2;
    TriacM->PeriodCtl.Serial_Nummax=0;
    TriacM->PeriodCtl.Serial_Nummin=0;
    TriacM->PeriodCtl.Serial_Num=0;
    TriacM->PeriodCtl.Duty_TS=0;

    TriacM->PeriodCtl.Dutymax_T1S1=DUTYMAX_T1S1;
    TriacM->PeriodCtl.Dutymin_T1S1=DUTYMIN_T1S1;
    TriacM->PeriodCtl.Dutydefault_T1S1=DUTYDEFAULT_T1S1;

    TriacM->PeriodCtl.Dutymax_T2S1=DUTYMAX_T2S1;
    TriacM->PeriodCtl.Dutymin_T2S1=DUTYMIN_T2S1;
    TriacM->PeriodCtl.Dutydefault_T2S1=DUTYDEFAULT_T2S1;

    TriacM->PeriodCtl.Dutymax_T3S1=DUTYMAX_T3S1;
    TriacM->PeriodCtl.Dutymin_T3S1=DUTYMIN_T3S1;
    TriacM->PeriodCtl.Dutydefault_T3S1=DUTYDEFAULT_T3S1;


    TriacM->PeriodCtl.Dutymax_T1S2=DUTYMAX_T1S2;
    TriacM->PeriodCtl.Dutymin_T1S2=DUTYMIN_T1S2;
    TriacM->PeriodCtl.Dutydefault_T1S2=DUTYDEFAULT_T1S2;

    TriacM->PeriodCtl.Dutymax_T2S2=DUTYMAX_T2S2;
    TriacM->PeriodCtl.Dutymin_T2S2=DUTYMIN_T2S2;
    TriacM->PeriodCtl.Dutydefault_T2S2=DUTYDEFAULT_T2S2;

    TriacM->PeriodCtl.Dutymax_T3S2=DUTYMAX_T3S2;
    TriacM->PeriodCtl.Dutymin_T3S2=DUTYMIN_T3S2;
    TriacM->PeriodCtl.Dutydefault_T3S2=DUTYDEFAULT_T3S2;

    TriacM->PeriodCtl.Dutymax_T1S3=DUTYMAX_T1S3;
    TriacM->PeriodCtl.Dutymin_T1S3=DUTYMIN_T1S3;
    TriacM->PeriodCtl.Dutydefault_T1S3=DUTYDEFAULT_T1S3;

    TriacM->PeriodCtl.Dutymax_T2S3=DUTYMAX_T2S3;
    TriacM->PeriodCtl.Dutymin_T2S3=DUTYMIN_T2S3;
    TriacM->PeriodCtl.Dutydefault_T2S3=DUTYDEFAULT_T2S3;

    TriacM->PeriodCtl.Dutymax_T3S3=DUTYMAX_T3S3;
    TriacM->PeriodCtl.Dutymin_T3S3=DUTYMIN_T3S3;
    TriacM->PeriodCtl.Dutydefault_T3S3=DUTYDEFAULT_T3S3;

#ifdef   ZEROCROSSOVER_OC
    TriacM->Zero.Zcrossset=(uint16_t)OC_ZEROCROSS_VERYTIME;//光耦过零
#else
    TriacM->Zero.Zcrossset=(uint16_t)TR_ZEROCROSS_VERYTIME;//三极管过零
#endif

    TriacM->Zero.Zcrosslostset=(uint16_t)(ZEROCROSSLOST_VERYTIME*ZEROCROSSLOST_VERYFREQ);
    TriacM->Zero.SecondZcrossSet=(uint16_t)(ZEROCROSSLOST_VERYFREQ/ZEROCROSS_FREQ);
    TriacM->Zero.ZcrossDelayTrigset=(uint16_t)(TRIAC_TIME_TO_TRIGGER*SPEED_LOOP_CAL_FREQ);

    TriacM->TonEnable_cntset=(uint16_t)(TRIAC_ENABLEWAIT*TIMER_FREQ_SPEED_LOOP);

    TempM->TempRatedADCInv=_IQ(1.0)/65536;
    TempM->TempAverSizeInv1 = _IQ(1.0)/TEMP_AVER_SIZE;
    TempM->TempAverSizeInv2 = _IQ(1.0) - TempM->TempAverSizeInv1;

}

/******************************************************************************
* Function Name  : ZeroCross_Status_Ini
* Description    : Silicon-controlled rectifier start-up state initialization
* Input          : SCR control structure Temperature control structure
* Output         : None
* Return         : None
******************************************************************************/
void Triac_Status_Init(TRIAC_ctl_Type *TriacM,Temperature_ctl_Type *TempM)
{
    TriacM->Zero.Zcrosscnt = 0;
    TriacM->Zero.ZcrossEvent=0;
    TriacM->Zero.Zcrosslostcnt=0;
    TriacM->Zero.Zcrosslost=FALSE;
    TriacM->Zero.Zcrosseffective = FALSE;

    TriacM->Zero.ZcrossDelayTrigcnt=0;
    TriacM->Zero.Zcrosseffectivecnt=0;

    TriacM->Zero.SecondZcrossCnt = 0;
    TriacM->Zero.Prestatus = 0;
    TriacM->Zero.Aftstatus = 0;
    TriacM->Zero.Risingseffective =FALSE;
    TriacM->Zero.Inittatus =FALSE;
    TriacM->Zero.SecondZsseffective=FALSE;

    TriacM->TonEnable_cnt=0;
    TriacM->TonEnable=0;
    TriacM->ACVoltcaliMin=_IQ(1.0);
    TriacM->ACVoltcali=_IQ(1.0);
    TriacM->ACVoltcaliPrev=_IQ(1.0);
    TriacM->ACVoltcali_Square=_IQ(1.0);


    TriacM->PeriodCtl.Cnt=TriacM->PeriodCtl.Period;
    TriacM->PeriodCtl.Serial_Num=0;
    TriacM->PeriodCtl.DelayProcess_Flag=0;
    TriacM->PeriodCtl.DutychangeFlag=FALSE;

}

/******************************************************************************
* Function Name  : AC_Zerocross_Detect_OC
* Description    : Optocoupler AC Input Zero-Crossing Detection
* Input          : SCR control structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void AC_Zerocross_Detect_OC(TRIAC_ctl_Type *TriacM)
{
    if(TriacM->Zero.Zcrosseffective == FALSE)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1)
            TriacM->Zero.Zcrosscnt++;
        else
            TriacM->Zero.Zcrosscnt = 0;

        if(TriacM->Zero.Zcrosscnt >= TriacM->Zero.Zcrossset)
        {
            TriacM->Zero.Zcrosseffective = TRUE;
            TriacM->Zero.Zcrosscnt=0;
            TriacM->Zero.ZcrossEvent=1;
            TriacM->Zero.Zcrosslostcnt=0;
            TriacM->Zero.Zcrosslost=FALSE;

            TriacM->PhaseCtl.Toncnt=0;
            TriacM->PhaseCtl.TonStatus=FALSE;
            TriacM->PhaseCtl.TonEvent=1;

            TriacM->ACVoltcali=TriacM->ACVoltcaliMin;  
            TriacM->ACVoltcaliMin=ADC_M.DCVoltcali;
        }
    }
    else  if((TriacM->Zero.Zcrosseffective == TRUE)&&( TriacM->Zero.ZcrossEvent==0))
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
            TriacM->Zero.Zcrosscnt++;
        else
            TriacM->Zero.Zcrosscnt = 0;

        if(TriacM->Zero.Zcrosscnt >= TriacM->Zero.Zcrossset)
        {
            TriacM->Zero.Zcrosseffective = FALSE;
            TriacM->Zero.Zcrosscnt=0;
        }
    }
    TriacM->Zero.Zcrosslostcnt++;
    if(TriacM->Zero.Zcrosslostcnt >= TriacM->Zero.Zcrosslostset)
    {
        TriacM->Zero.Zcrosslost=TRUE;
        TriacM->Zero.Zcrosslostcnt=0;
    }
}
/******************************************************************************
* Function Name  : AC_Zerocross_Detect_TR
* Description    : Transistor AC Input Zero-Crossing Detection
* Input          : SCR control structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void AC_Zerocross_Detect_TR(TRIAC_ctl_Type *TriacM)
{
    if(TriacM->Zero.Zcrosseffective == FALSE)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
            TriacM->Zero.Zcrosscnt++;
        else
            TriacM->Zero.Zcrosscnt = 0;

        if(TriacM->Zero.Zcrosscnt >= TriacM->Zero.Zcrossset)
        {
            TriacM->Zero.Zcrosseffective = TRUE;
            TriacM->Zero.Zcrosscnt=0;
            TriacM->Zero.ZcrossEvent=1;
            TriacM->Zero.Zcrosslostcnt=0;
            TriacM->Zero.Zcrosslost=FALSE;

            TriacM->PhaseCtl.Toncnt=0;
            TriacM->PhaseCtl.TonStatus=FALSE;
            TriacM->PhaseCtl.TonEvent=1;

            TriacM->ACVoltcali=TriacM->ACVoltcaliMin; 
            TriacM->ACVoltcaliMin=ADC_M.DCVoltcali;
        }
    }
    else  if((TriacM->Zero.Zcrosseffective == TRUE)&&( TriacM->Zero.ZcrossEvent==0))
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1)
            TriacM->Zero.Zcrosscnt++;
        else
            TriacM->Zero.Zcrosscnt = 0;

        if(TriacM->Zero.Zcrosscnt >= TriacM->Zero.Zcrossset)
        {
            TriacM->Zero.Zcrosseffective = FALSE;
            TriacM->Zero.Zcrosscnt=0;
        }
    }
    TriacM->Zero.Zcrosslostcnt++;
    if(TriacM->Zero.Zcrosslostcnt >= TriacM->Zero.Zcrosslostset)
    {
        TriacM->Zero.Zcrosslost=TRUE;
        TriacM->Zero.Zcrosslostcnt=0;
    }
}

/******************************************************************************
* Function Name  : TriacZero50HZ_Control_Process
* Description    : Thyristor Period Control Processing
* Input          : Silicon-controlled rectifier control pointer Temperature gear Temperature control pointer
* Output         : None
* Return         : None
******************************************************************************/
u_int32_t Triac50hz_Data[30]={0X80100100,0X80804040,0X81020810,0X82084108,0X84422110,0X88888888,0X89122448,0X92249224,\
                  0XA4924924,0X94949494,0XA4A5294A,0XA92A552A,0XA92B552A,0XAAAAAAAA,0XD5556AAA,0XD5AAD5AA,\
                  0XB5AD6B5A,0XD6D6D6D6,0XB6DB6DB6,0XDBB6DBB6,0XEDDBB76E,0XEEEEEEEE,0XF77BBDDE,0XF7DEF7DE,\
                  0XF7DFF7DE,0XFFDFF7DE,0XFFDFFFDE,0XFFFEFFFE,0XFFFFFFFE,0XFFFFFFFF};//占空比9.09%~96.9696%
void TriacZero_50HZControl_Process(Temperature_ctl_Type *TempM,Key_Manager_Type *KeyM,TRIAC_ctl_Type *TriacM,Speedrampctr_Type *Speed_Ramp)
{
    _iq24 ACVoltcali;
    u32 data=0;
    if((TriacM->TonEnable==1)&&(KeyM->Tkey_rank>0)&&( TriacM->Zero.Zcrosslost == FALSE)&&(KeyM->TCkeycold==FALSE)&&\
      (TempM->NTCOverStatus==FALSE)&&(TempM->NTCOpenStatus==FALSE)&&\
	  (RunningStatus_M==RUN)&&(KeyM->Skeystop==FALSE)&&(Operation_M.Faultstop == FALSE))
    {
        TriacM->PeriodCtl.Cnt++;
        if(TriacM->PeriodCtl.Cnt>TriacM->PeriodCtl.Period)
        {
            TriacM->PeriodCtl.Cnt=1;
            if(TriacM->PeriodCtl.DutychangeFlag==TRUE)
            {
                Temp_Set(KeyM,&SpeedRamp_M,TriacM);
                TriacM->PeriodCtl.DutychangeFlag=FALSE;
            }

            //ACVoltcali= TriacM->ACVoltcali;
            ACVoltcali=ADC_M.DCVoltcali;

            TriacM->ACVoltcali_Square= _IQmpy(ACVoltcali, ACVoltcali);

            TriacM->PeriodCtl.Serial_Num=(u_int8_t)((TriacM->PeriodCtl.Duty_TS*TriacM->ACVoltcali_Square)>>24)-TriacM->PeriodCtl.NumData;

//            TriacM->PeriodCtl.Serial_Num=(u_int8_t)(TriacM->PeriodCtl.Duty_TS-TriacM->PeriodCtl.NumData);

            if(TriacM->PeriodCtl.Serial_Num>=TriacM->PeriodCtl.Serial_Nummax)
                TriacM->PeriodCtl.Serial_Num=TriacM->PeriodCtl.Serial_Nummax;
            else if(TriacM->PeriodCtl.Serial_Num<=TriacM->PeriodCtl.Serial_Nummin)
                TriacM->PeriodCtl.Serial_Num=TriacM->PeriodCtl.Serial_Nummin;
        }
        data=Triac50hz_Data[TriacM->PeriodCtl.Serial_Num];
        if(TriacM->PeriodCtl.Cnt<TriacM->PeriodCtl.Period)
        {
            data<<=(TriacM->PeriodCtl.Cnt-1);

               if(data&0x80000000)
                   TRIAC_ON();
               else
                   TRIAC_OFF();
        }
        else
        {
            TRIAC_ON();
            //TRIAC_OFF();
        }
    }
    else
    {
        TriacM->PeriodCtl.Cnt=TriacM->PeriodCtl.Period;
        TRIAC_OFF();
    }
}

u_int32_t Triac100hz_Data1[58]={0X80100100,0X80200802,0X81010100,0X81010402,0X84104080,0X84104104,0X84081041,0X88422081,\
                        0X21084211,0X22222211,0X11111111,0X22248891,0X88912484,0X92222248,0X89224928,0X92492492,\
                        0X92492492,0XA9292492,0XA9292492,0XAA924952,0XAA924952,0XA9294AA9,0XAA4954AA,0XAA4AAAAA,\
                        0XAA4AAAAA,0XAAAAAAAA,0XAAAAAAAA,0X66D55555,0X66D55555,0X66D6D666,0X66D6D666,0X76D6D666,\
                        0X76D6D666,0X76DB7666,0X76DB7666,0X76DB7766,0X76DB7766,0X776B7767,0X776B7767,0X776B7777,\
                        0X776B7777,0X77777777,0X77777777,0XF77BDEEE,0XF77BDEEE,0X777F7F77,0X777F7F77,0X777F7F7F,\
                        0XF77F7F7E,0XF77F7F7F,0XF7FEFEFE,0XF7FFFEFE,0XF7FFFEFE,0XF7FFFEFF,0XFFFEFFFE,0XFFFFFFFE,\
                        0XFFFFFFFE,0XFFFFFFFF};
/* 6  ~ 13
 * 14 ~ 21
 * 22 ~ 29
 * 30 ~ 37
 * 38 ~ 45
 * 46 ~ 53
 * 54 ~ 61
 * 62 ~63
 */
u_int32_t Triac100hz_Data2[58]={0X80100100,0X00804040,0X81010100,0X08041008,0X84104080,0X10404210,0X11041110,0X11041110,\
                        0X10884442,0X10884442,0X11111111,0X11111111,0X88912444,0X92249224,0X89224928,0X49224928,\
                        0X92492492,0X92492492,0XA9292492,0XA9292492,0XAA924952,0X29294AAA,0XAA4954AA,0XAA4954AA,\
                        0XAA4AAAAA,0XAA4AAAAA,0XAAAAAAAA,0X55555555,0X66D55555,0X66D55555,0X66D6D666,0X66D6D666,\
                        0X76D6D666,0X76D6D666,0X76DB7666,0X76DB7666,0X76DB7766,0X776B5577,0X776B7767,0X776B7767,\
                        0X776B7777,0X776B7777,0X77777777,0XF777776E,0XF77BDEEE,0X777FDEEE,0X777F7F77,0X777F7F77,\
                        0XF77F7F7E,0XF77F7F7E,0XF7FEFEFE,0XF7FEFEFE,0XF7FFFEFE,0XF7FFFEFE,0XFFFEFFFE,0XFFFEFFFE,\
                        0XFFFFFFFE,0XFFFFFFFE};

/******************************************************************************
* Function Name  : TriacZero50HZ_Control_Process
* Description    : Thyristor Period Control Processing
* Input          : Silicon-controlled rectifier control pointer Temperature gear Temperature control pointer
* Output         : None
* Return         : None
******************************************************************************/
void TriacZero_100HZControl_Process(Temperature_ctl_Type *TempM,Key_Manager_Type *KeyM,TRIAC_ctl_Type *TriacM,Speedrampctr_Type *Speed_Ramp)
{
    u32 data=0;
    u32 Data1;
    u32 Data2;
    _iq24 ACVoltcali;
    if((TriacM->TonEnable==1)&&(KeyM->Tkey_rank>0)&&( TriacM->Zero.Zcrosslost == FALSE)&&(KeyM->TCkeycold==FALSE)&&\
      (TempM->NTCOverStatus==FALSE)&&(TempM->NTCOpenStatus==FALSE)&&\
      (RunningStatus_M==RUN)&&(KeyM->Skeystop==FALSE)&&(Operation_M.Faultstop == FALSE))
    {

        TriacM->PeriodCtl.Cnt++;
        if(TriacM->PeriodCtl.Cnt>TriacM->PeriodCtl.Period)//
        {
            TriacM->PeriodCtl.Cnt=1;
            if(TriacM->PeriodCtl.DutychangeFlag==TRUE)
           {
               Temp_Set(KeyM,&SpeedRamp_M,TriacM);
               TriacM->PeriodCtl.DutychangeFlag=FALSE;
           }

             //根据母线电压峰值矫正占空比
            ACVoltcali=ADC_M.DCVoltcali;

            TriacM->ACVoltcali_Square= _IQmpy(ACVoltcali, ACVoltcali);

            TriacM->PeriodCtl.Serial_Num=(u_int8_t)((TriacM->PeriodCtl.Duty_TS*TriacM->ACVoltcali_Square)>>24)-TriacM->PeriodCtl.NumData;
            //TriacM->PeriodCtl.Serial_Num=(u_int8_t)(TriacM->PeriodCtl.Duty_TS-TriacM->PeriodCtl.NumData);

            if(TriacM->PeriodCtl.Serial_Num>=TriacM->PeriodCtl.Serial_Nummax)
                TriacM->PeriodCtl.Serial_Num=TriacM->PeriodCtl.Serial_Nummax;
            else if(TriacM->PeriodCtl.Serial_Num<=TriacM->PeriodCtl.Serial_Nummin)
                TriacM->PeriodCtl.Serial_Num=TriacM->PeriodCtl.Serial_Nummin;
        }
        Data1=Triac100hz_Data1[TriacM->PeriodCtl.Serial_Num];
        Data2=Triac100hz_Data2[TriacM->PeriodCtl.Serial_Num];
        if(TriacM->PeriodCtl.Cnt<=TriacM->PeriodCtl.HalfPeriod)
        {
            data=Data1;
            data<<=(TriacM->PeriodCtl.Cnt-1);

               if(data&0x80000000)
                   TRIAC_ON();
               else
                   TRIAC_OFF();
        }
        else if((TriacM->PeriodCtl.Cnt>TriacM->PeriodCtl.HalfPeriod)&&(TriacM->PeriodCtl.Cnt<TriacM->PeriodCtl.Period))//Triac100hz_Data2 1~32 发波
        {
            data=Data2;
            data<<=(TriacM->PeriodCtl.Cnt-1-TriacM->PeriodCtl.HalfPeriod);

               if(data&0x80000000)
                   TRIAC_ON();
               else
                   TRIAC_OFF();
        }
        else 
        {
            TRIAC_OFF();
        }
    }
    else
    {
        TriacM->PeriodCtl.Cnt=TriacM->PeriodCtl.Period;
        TRIAC_OFF();
    }
}

/******************************************************************************
* Function Name  : TempInit_Measure
* Description    : NTC Temperature Reading
* Input          : Temperature Control Body Pointer ADC
* Output         : None
* Return         : None
******************************************************************************/
void TempInit_Measure(Temperature_ctl_Type *TempM,ADC_TypeDef *ADCx)
{
    u8 i;
    ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);
    ADC_SoftwareStartConvCmd(ADCx, ENABLE); 

    for(i=0; i <TEMP_AVER_SIZE; i++)
    {
        while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC));

        TempM->TempPres += ((_iq24)((u16)ADCx->RDATAR*(TempM->TempRatedADCInv))>>3);

        ADC_ClearFlag(ADCx, ADC_FLAG_JEOC);
        ADC_SoftwareStartConvCmd(ADCx,ENABLE);
    }
}
