/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_Cusevent.c
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

Eventmanager_Type   Emanager_M;         
uint8_t testcounter =0;

/******************************************************************************
* Function Name  : Eventmanager_Initial
* Description    : Event manage Init
* Input          : Event management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void Eventmanager_Initial(Eventmanager_Type *EventM)
{
    EventM->Event_10ms= FALSE;
    EventM->Event_10ms_cntset = (uint16_t)(EVENT_10ms*EVENT_CHECK_FREQ);
    EventM->Event_10ms_cnt = 0;

    EventM->Event_1ms= FALSE;
    EventM->Event_1ms_cntset = (uint16_t)(EVENT_1ms*EVENT_CHECK_FREQ);
    EventM->Event_1ms_cnt = 0;

    EventM->Event_2s= FALSE;
    EventM->Event_2s_cntset = (uint16_t)(EVENT_2s*EVENT_CHECK_FREQ);
    EventM->Event_2s_cnt = 0;
}

/******************************************************************************
* Function Name  : Zerocross_Process
* Description    : AC input zero-crossing event handling
* Input          : Temperature control structure pointer, thyristor control structure pointer, key management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void ACZerocross_Process(Temperature_ctl_Type *TempM,Key_Manager_Type *KeyM,TRIAC_ctl_Type *TriacM)
{
        if((KeyM->Tkeyeffective==TRUE)||(KeyM->Skeyeffective==TRUE))
            TriacM->PeriodCtl.DutychangeFlag=TRUE;

        if( (TriacM->Zero.ZcrossEvent==1) && (TriacM->Zero.Zcrosseffective == TRUE) )
        {
                    #ifdef ZEROCROSSOVER_OC       
                    if(TriacM->Zero.Zcrosseffectivecnt<2)
                        TriacM->Zero.Zcrosseffectivecnt++;
                    if(TriacM->Zero.Zcrosseffectivecnt>=2)
                    {
                        if(TriacM->PeriodCtl.DelayProcess_Flag==0)//
                        {
                            TriacM->Zero.ZcrossDelayTrigcnt=TriacM->Zero.ZcrossDelayTrigset;
                            TriacM->PeriodCtl.DelayProcess_Flag=1;
                        }
                        if((TriacM->Zero.ZcrossDelayTrigcnt==0)&&(TriacM->PeriodCtl.DelayProcess_Flag==1))
                        {
                            TriacZero_50HZControl_Process(TempM,KeyM,TriacM,&SpeedRamp_M);
                            TriacM->Zero.Zcrosseffectivecnt=0;
                            TriacM->Zero.ZcrossEvent=0;
                            TriacM->PeriodCtl.DelayProcess_Flag=0;
                        }
                    }
                    else
                    {
                        TriacM->Zero.ZcrossEvent=0;
                        TriacM->PeriodCtl.DelayProcess_Flag=0;
                    }

                    #else                       

                    if(TriacM->PeriodCtl.DelayProcess_Flag==0)//
                    {
                        TriacM->Zero.ZcrossDelayTrigcnt=TriacM->Zero.ZcrossDelayTrigset;
                        TriacM->PeriodCtl.DelayProcess_Flag=1;
                    }
                    if((TriacM->Zero.ZcrossDelayTrigcnt==0)&&(TriacM->PeriodCtl.DelayProcess_Flag==1))
                    {
                        TriacZero_50HZControl_Process(TempM,KeyM,TriacM,&SpeedRamp_M);
                        TriacM->Zero.ZcrossEvent=0;
                        TriacM->PeriodCtl.DelayProcess_Flag=0;
                    }
                    #endif
        }
}

/******************************************************************************
* Function Name  : Event_10ms_Process
* Description    : 10ms Event process
* Input          : Event management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void Event_10ms_Process(Eventmanager_Type *EventM)
{
    if(EventM->Event_10ms == TRUE)
    {
//        testcounter++;
//
//        if(testcounter ==2)
//            LED_Lighton(&LED_M.YLED);
//        if(testcounter ==4)
//        {
//            LED_Lightoff(&LED_M.YLED);
//            testcounter = 0;
//        }

        EventM->Event_10ms_cnt = EventM->Event_10ms_cntset;
        EventM->Event_10ms = FALSE;
    }
}

/******************************************************************************
* Function Name  : Event_1ms_Process
* Description    : 1ms Event Handling
* Input          : Event management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void Event_1ms_Process(Eventmanager_Type *EventM)
{
    //static u8 Event_50ms_cnt;
    if(EventM->Event_1ms == TRUE)
    {

        EventM->Event_1ms_cnt = EventM->Event_1ms_cntset;
        EventM->Event_1ms = FALSE;
    }
}

/******************************************************************************
* Function Name  : Event_1ms_Process
* Description    : 2s Event Handling
* Input          : Event management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void Event_2s_Process(Eventmanager_Type *EventM)
{
    if(EventM->Event_2s == TRUE)//¶¨Ê±µ½
    {
        EventM->Event_2s_cnt = EventM->Event_2s_cntset;
        EventM->Event_2s = FALSE;
    }
}
