/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_and_Display.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Motor operation related processing
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "M0_Control_Library.h"
static volatile uint16_t Timewait = 0;

Speedrampctr_Type   SpeedRamp_M;       
Powercontrol_Type   Powerctr_M;         
Led_Manager_Type    LED_M;              
Key_Manager_Type    Key_M;              
Operation_Type      Operation_M;        

Temperature_ctl_Type    Tempctl_M;      

uint8_t Keymembuff[EEWRITE_CYCLE];      
//uint8_t testcounter =0;

/******************************************************************************
* Function Name  : Motor_Operation_Process
* Description    : Motor Operation Handling
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Motor_Operation_Process(void)
{

    switch (RunningStatus_M)
    {
        case IDLE:
            if((Powerctr_M.Poweron == TRUE)&&(Key_M.ACkey_Poweron == TRUE))
            {
                if(Key_M.Keymemreadmark == FALSE)
                {
                    Key_Memread(&Key_M,&SpeedRamp_M,&Flash_M);
                    Key_M.Keymemreadmark = TRUE;
                }
                if((Operation_M.PSC.Mark ==FALSE)&&(Operation_M.PSC.Process ==TRUE))
                {
                    #if defined KEY_START
                        if(SpeedRamp_M.Speed_adncnt >0)
                        {
                            Key_Memwrite(&Key_M,&SpeedRamp_M,&Flash_M);
                            RunningStatus_M = INIT;
                        }
                    #elif defined POWERON_START
                        if(Powerctr_M.Poweron == TRUE)
                        {
                            RunningStatus_M = INIT;
                        }
                    #endif
                }
            }
            break;
        case INIT:
            Motor_Start_Init_M();       
            Time_Delay_Set(SpeedRamp_M.Precharge_cntset,&SpeedRamp_M.Precharge_cnt);
            RunningStatus_M = PRESTART;    
            break;

        case PRESTART:
            break;

        case POSITION:
            break;

        case START:
            break;

        case RUN:
            break;

        case STOP:
            TIM_CtrlPWMOutputs(TIM1, DISABLE);  
            ADC_1R_Start(DISABLE);              
            TRIAC_OFF();
            LED_OFF(&LED_M);
            RunningStatus_M = WAIT;             
            Time_Delay_Set(Operation_M.StopwaitSet,&Operation_M.StopwaitCnt);

		   break;

        case WAIT:

            if (Time_Delay_Elapse(Operation_M.StopwaitCnt) == TRUE)
            {
                RunningStatus_M = IDLE;
                LED_OFF(&LED_M);
            }
            break;
        case FAULT:
           LED_Flicker(&(LED_M.LED4));
           TRIAC_OFF();
            break;

        default:
            break;
    }

    switch (RunningStatus_M)
    {
        case PRESTART:
        case POSITION:
        case START:
        case RUN:
            if(Powerctr_M.Poweron == TRUE)
            {
                if(((Key_M.Skeyeffective == TRUE)||(Key_M.Tkeyeffective == TRUE))&&(Flash_M.FlashExcep==0))
                {
                    Key_Memwrite(&Key_M,&SpeedRamp_M,&Flash_M);
                }
            }
            else
            {
                Key_M.Skeyeffective = FALSE; 
                Key_M.Tkeyeffective = FALSE;   
            }
        break;

        default:
        break;
    }

   Event_10ms_Process(&Emanager_M);

   Event_2s_Process(&Emanager_M);
}

/******************************************************************************
* Function Name  : Speedrampctr_Parameter_Define
* Description    : Definition of Speed Control Parameters
* Input          : Acceleration and deceleration control structure pointer
* Output         : None
* Return         : None
*****************************************************************************/
void Speedrampctr_Parameter_Define(Speedrampctr_Type *Speed_Ramp)
{
    Speed_Ramp->Direction=STARTUP_DIRECTION;
    Speed_Ramp->Speed_Rank = SPEEDPOINT_RANK;
    Speed_Ramp->Speed1 = SPEEDPOINT1*Speed_Ramp->Direction;
    Speed_Ramp->Speed2 = SPEEDPOINT2*Speed_Ramp->Direction;
    Speed_Ramp->Speed3 = SPEEDPOINT3*Speed_Ramp->Direction;

    Speed_Ramp->Aftramp = RAMP_INJ_SPEED_M*Speed_Ramp->Direction;

    Speed_Ramp->AccStep = _IQ(1.0)/(SPEED_LOOP_CAL_FREQ*ACCE_TIME_M);
    Speed_Ramp->DecStep = _IQ(1.0)/(SPEED_LOOP_CAL_FREQ*DECE_TIME_M);
    Speed_Ramp->PoweroffDecStep= _IQ(1.0)/(SPEED_LOOP_CAL_FREQ*POWEROFF_DECE_TIME_M);

    Speed_Ramp->Startfinishspeed = START_FINISH_SPEED;
    Speed_Ramp->Startfinish =FALSE;
    Speed_Ramp->Faststopspeed = FAST_DECE_SPEED*Speed_Ramp->Direction;

    Speed_Ramp->Break =  FALSE;
    Speed_Ramp->BreakStop = FALSE;
    Speed_Ramp->Breakspeed = BREAKSPEED;
    Speed_Ramp->Breakwait_cntset = (uint16_t)(BREAKTIME*SPEED_RAMP_FREQ_M);

    Speed_Ramp->Precharge_cntset = (uint16_t)(PRECHAGE_TIME*SPEED_RAMP_FREQ_M);

#ifdef POWERON_START
    Speed_Ramp->Stopmode =1;
#else
    Speed_Ramp->Stopmode =2;
#endif

#ifdef KEY_START           //按键启动
    Speed_Ramp->Speed_adncnt =0;
#endif

    Speed_Ramp->VdcLPF1st.Input_Coef = _IQdiv(VDCLPF_SPEEDLIM_WcT_M,(VDCLPF_SPEEDLIM_WcT_M + UNIT_Q24));
    Speed_Ramp->VdcLPF1st.Output_Coef = _IQdiv(UNIT_Q24,(VDCLPF_SPEEDLIM_WcT_M + UNIT_Q24));
    Speed_Ramp->VdcLPF1st.Out_New =_IQ(1.0);
    Speed_Ramp->VdcLPF1st.Out_Pre =_IQ(1.0);
    Speed_Ramp->Voltbegin = VDC_SPEEDLIM_BEGIN;
    Speed_Ramp->Voltrelease = VDC_SPEEDLIM_RELEASE;
    Speed_Ramp->Speedlimmark = FALSE;
    Speed_Ramp->Vdclimcoef = _IQdiv(_IQ(1.0),_IQmpy(VDC_SPEEDLIM_ADJ,VDC_SPEEDLIM_BEGIN));
}

/******************************************************************************
* Function Name  : Speedrampctr_Start_Init_M
* Description    : Speed Control Parameter Initialization
* Input          : Acceleration and deceleration control structure pointer
* Output         : None
* Return         : None
*****************************************************************************/
void Speedrampctr_Start_Init_M(Speedrampctr_Type *Speed_Ramp)
{
    Speed_Ramp->Aftramp = RAMP_INJ_SPEED_M*Speed_Ramp->Direction;
    Speed_Ramp->Startfinish =FALSE;
    Speed_Ramp->BreakStop = FALSE;
    Speed_Ramp->Faststopspeed = FAST_DECE_SPEED*Speed_Ramp->Direction;
    Speed_Ramp->Break =  FALSE;
    Speed_Ramp->Speedlimmark = FALSE;

    Speed_Ramp->VdcLPF1st.Out_New =ADC_M.DCVoltAver;
    Speed_Ramp->VdcLPF1st.Out_Pre =ADC_M.DCVoltAver;
}

/******************************************************************************
* Function Name  : Poweroperation_Initial
* Description    : Power-on and power-off parameter initialization and state initialization
* Input          : Power on/off control structure pointer
* Output         : None
* Return         : None
*****************************************************************************/
void Poweroperation_Initial(Powercontrol_Type *PowerOper)
{
    PowerOper->Poweron_CntThresh = (uint16_t)(POWERVERY_FREQ*POWERON_TIME);
    PowerOper->Poweron_Volt = POWERON_VOLT;
    PowerOper->Poweroff_Volt1 = POWEROFF_VOLT1;
    PowerOper->Poweroff_Volt2 = POWEROFF_VOLT2;
    PowerOper->Poweron = FALSE;
    PowerOper->Poweron_Cnt = 0;
    PowerOper->DCVoltAverSizeInv1 = _IQ(1.0)/POWERVERY_DCVOLTAVERSIZE;
    PowerOper->DCVoltAverSizeInv2 = _IQ(1.0) - PowerOper->DCVoltAverSizeInv1;
    PowerOper->DCVoltAver = 0;
}

/******************************************************************************
* Function Name  : KEY_AND_LEDIO_Init
* Description    : Button and indicator light IO configuration
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void KeyandLED_IOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|\
            RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;        
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;             
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;        
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;               
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  
    GPIO_SetBits(GPIOB, GPIO_Pin_7);                        

}

/******************************************************************************
* Function Name  : KeyandLED_Parameter_Define
* Description    : Rotation Speed and LED Parameter Definition
* Input          : Pointer to key management structure, pointer to LED management structure
* Output         : None
* Return         : None
******************************************************************************/
void KeyandLED_Parameter_Define(Key_Manager_Type *KeyM,Led_Manager_Type *LEDM)
{
    KeyM->SHkey_checkinterval = (uint16_t)(SHKEY_CHECK_INTERVAL*KEY_CHECK_FREQ);
    KeyM->SHkey_effcnt_thresh = (uint16_t)(SHKEY_EFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->SHkey_ineff_cntthresh = (uint16_t)(SHKEY_INEFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->SHkey_Speedset=FALSE;

    KeyM->SRkey_effcnt_thresh = (uint16_t)(SRKEY_EFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->SRkey_ineff_cntthresh = (uint16_t)(SRKEY_INEFFECTIVE_TIME*KEY_CHECK_FREQ);

    KeyM->Tkey_rankset = TKEY_RANK_SET;
    KeyM->Tkey_cyccntthresh  = (uint16_t)(TKEY_LONG_TIME*KEY_CHECK_FREQ);
    KeyM->Tkey_shtcntthreshdown  = (uint16_t)(TKEY_SHORT_TIME_DOWM*KEY_CHECK_FREQ);
    KeyM->Tkey_shtcntthreshup  = (uint16_t)(TKEY_SHORT_TIME_UP*KEY_CHECK_FREQ);

    KeyM->EEwritewaitset  = (uint16_t)(EEWRITE_WAITING_TIME*KEY_CHECK_FREQ);
    KeyM->EEwritecycset  = EEWRITE_CYCLE;

    KeyM->TCkey_effcnt_thresh=(uint16_t)(TCKEY_EFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->TCkey_ineff_cntthresh=(uint16_t)(TCKEY_INEFFECTIVE_TIME*KEY_CHECK_FREQ);

    KeyM->Keymemreadmark = FALSE;

    KeyM->ACkey_checkinterval = (uint16_t)(ACKEY_CHECK_INTERVAL*KEY_CHECK_FREQ);
    KeyM->ACkey_effcnt_thresh = (uint16_t)(ACKEY_EFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->ACkey_ineff_cntthresh = (uint16_t)(ACKEY_INEFFECTIVE_TIME*KEY_CHECK_FREQ);
    KeyM->ACkey_checkcnt = 0;
    KeyM->ACkey_effcnt = 0;
    KeyM->ACkey_ineff_cnt = 0;
    KeyM->ACkey_Poweron = FALSE;

    KeyM->TCkey_effcnt = 0;
    KeyM->TCkey_ineff_cnt = 0;
    KeyM->TCkeycold= FALSE;

#ifndef POWERON_START
    SpeedRamp_M.Speed_adncnt = 0;
#endif

    LEDM->Pin1.Group = GPIOB;
    LEDM->Pin1.Pin = GPIO_Pin_8;

    LEDM->Pin2.Group = GPIOA;
    LEDM->Pin2.Pin = GPIO_Pin_0;

    LEDM->Pin3.Group = GPIOA;
    LEDM->Pin3.Pin = GPIO_Pin_11;

    LEDM->LED4.FlickInterval = (uint16_t)(LED_PROCESS_FREQ*LED_FLICKER_TIME);
    LEDM->LED5.FlickInterval = (uint16_t)(LED_PROCESS_FREQ*LED_FLICKER_TIME);
    LEDM->LED6.FlickInterval = (uint16_t)(LED_PROCESS_FREQ*LED_FLICKER_TIME);
    LEDM->LED7.FlickInterval = (uint16_t)(LED_PROCESS_FREQ*LED_FLICKER_TIME);
    LEDM->LED8.FlickInterval = (uint16_t)(LED_PROCESS_FREQ*LED_FLICKER_TIME);
    LEDM->LED9.FlickInterval = (uint16_t)(LED_PROCESS_FREQ*LED_FLICKER_TIME);

    LEDM->LED4.LightFlag=OFF;
    LEDM->LED5.LightFlag=OFF;
    LEDM->LED6.LightFlag=OFF;
    LEDM->LED7.LightFlag=OFF;
    LEDM->LED8.LightFlag=OFF;
    LEDM->LED9.LightFlag=OFF;
}

/******************************************************************************
* Function Name  : KeyandLED_Status_Ini
* Description    : Initialize rotational speed and LED status
* Input          : Pointer to key management structure, pointer to LED management structure
* Output         : None
* Return         : None
******************************************************************************/
void KeyandLED_Status_Ini(Key_Manager_Type *KeyM,Led_Manager_Type *LEDM)
{
    KeyM->SHkey_checkcnt = 0;
    KeyM->SHkey_effcnt1 = 0;
    KeyM->SHkey_ineff_cnt1 = 0;
    KeyM->SHkey_effcnt2 = 0;
    KeyM->SHkey_ineff_cnt2 = 0;

    KeyM->SRkey_effcnt =0;
    KeyM->SRkey_ineff_cnt =0;

    KeyM->Skeyeffective = FALSE;
    KeyM->Skeystop = FALSE;
    KeyM->Skeystopcnt = 0;

    KeyM->Tkeycnt = 0;
    KeyM->Tkeyeffective = FALSE;

    KeyM->EEwritecyccnt = 0;
    KeyM->EEwritewaitcnt = 0;
}

/******************************************************************************
* Function Name  : Tempkeycheck
* Description    : Temperature control key detection
* Input          : Button management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void Tempkeycheck(Key_Manager_Type *KeyM)
{
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14) == 0)
    {
        if(KeyM->Tkeycnt< U16_MAX)
            KeyM->Tkeycnt++;
    }
    else    
    {
        if((RunningStatus_M==RUN)&&((KeyM->Tkeycnt >= KeyM->Tkey_shtcntthreshdown)&&(KeyM->Tkeycnt <=KeyM->Tkey_shtcntthreshup)) && ( KeyM->TCkeycold==FALSE) && (Tempctl_M.NTCOverStatus==FALSE) && (Tempctl_M.NTCOpenStatus==FALSE))//短按 没有按一键制冷
        {
            if(KeyM->Tkey_rank >= KeyM->Tkey_rankset)
                KeyM->Tkey_rank_Dir = DIRDOWN;
            else if (KeyM->Tkey_rank <=0)
                KeyM->Tkey_rank_Dir = DIRUP;

                if(KeyM->Tkey_rank_Dir == DIRUP)
                    KeyM->Tkey_rank ++;
                else if(KeyM->Tkey_rank_Dir == DIRDOWN)
                    KeyM->Tkey_rank --;

                KeyM->Tkeycyc = FALSE;     
                KeyM->Tkeycnt = 0;         
                KeyM->Tkeyeffective = TRUE;
                TempLed_Set(&Key_M,&LED_M);
         }

//        else if(KeyM->Tkeycnt >= KeyM->Tkey_cyccntthresh)
//        {
//            KeyM->Tkeycyc = TRUE;      
//            KeyM->Tkeycnt = 0;        
//        }
        else
        {
            KeyM->Tkeycnt = 0;         
        }
    }
}
/******************************************************************************
* Function Name  : Coldkeycheck
* Description    : One-touch cooling symbol
* Input          : Button management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void Coldkeycheck(Key_Manager_Type *KeyM)
{

    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
        KeyM->TCkey_effcnt++;
    else
        KeyM->TCkey_ineff_cnt++;


        if(KeyM->TCkey_effcnt >= KeyM->TCkey_effcnt_thresh)
        {
            KeyM->TCkeycold=TRUE;
            KeyM->TCkey_effcnt = 0;
            KeyM->TCkey_ineff_cnt = 0;
        }
        else if(KeyM->TCkey_ineff_cnt >= KeyM->TCkey_ineff_cntthresh)
        {
            KeyM->TCkeycold=FALSE;
            KeyM->TCkey_effcnt = 0;
            KeyM->TCkey_ineff_cnt = 0;
        }
}

/******************************************************************************
* Function Name  : Speedkeycheck_Hold
* Description    : Speed Button Detection_Push-to-Shift Hold Type
* Input          : Pointer to key management structure, acceleration and deceleration control structure
* Output         : None
* Return         : None
******************************************************************************/
void Speedkeycheck_Hold(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp)
{
    KeyM->SHkey_checkcnt++;

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
        KeyM->SHkey_effcnt1++;
    else
        KeyM->SHkey_ineff_cnt1++;

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
        KeyM->SHkey_effcnt2++;
    else
        KeyM->SHkey_ineff_cnt2++;

    if (KeyM->SHkey_checkcnt >= KeyM->SHkey_checkinterval)
    {
        if(KeyM->SHkey_effcnt2 >= KeyM->SHkey_effcnt_thresh)
        {
            if((KeyM->ACkey_Poweron==TRUE)&&(Powerctr_M.Poweron == TRUE))
            {
                if( Speed_Ramp->Speed_adncnt !=2)
                {
                    Speed_Ramp->Speed_adncnt = 2;
                    KeyM->Skeyeffective = TRUE;
                    KeyM->SHkey_Speedset= TRUE;
                }
            }
        }
        else if(KeyM->SHkey_effcnt1 >= KeyM->SHkey_effcnt_thresh)
        {
            if((KeyM->ACkey_Poweron==TRUE)&&(Powerctr_M.Poweron == TRUE))
            {
                if( Speed_Ramp->Speed_adncnt != 1)
                {
                    Speed_Ramp->Speed_adncnt = 1;
                    KeyM->Skeyeffective = TRUE;
                    KeyM->SHkey_Powerdown = FALSE;
                    KeyM->SHkey_Speedset= TRUE;
                }
            }
        }

        KeyM->SHkey_checkcnt = 0;
        KeyM->SHkey_effcnt1 = 0;
        KeyM->SHkey_ineff_cnt1 = 0;
        KeyM->SHkey_effcnt2 = 0;
        KeyM->SHkey_ineff_cnt2 = 0;
    }
    switch(RunningStatus_M)
    {
        case PRESTART:
        case POSITION:
        case START:
        case RUN:
        if(Powerctr_M.Poweron == TRUE)
        {
            if((KeyM->Skeystop == TRUE)||(Operation_M.Faultstop == TRUE))
            {
                if(KeyM->Skeystopcnt<U8_MAX)
                    KeyM->Skeystopcnt ++;

                if(KeyM->Skeystopcnt <=1)
                {
                      if(RunningStatus_M == RUN)
                      {
                         if(_IQabs(Speed_Ramp->Aftramp) > _IQabs(MLBG_M.Cal_Struc.Angspeed_Aver2))
                         Speed_Ramp->Aftramp = MLBG_M.Cal_Struc.Angspeed_Aver2;
                         Speed_Ramp->Preramp = Speed_Ramp->Faststopspeed;
                         Mpid_M.Speed.Output_New = 0;
                         Mpid_M.Speed.Output_Prev = 0;
                         Speed_Ramp->BreakStop = TRUE;
                      }
                      else if(RunningStatus_M == START)
                      {
                          Speed_Ramp->BreakStop = TRUE;
                      }
                      else
                          RunningStatus_M = STOP;
                }
            }
            if((KeyM->SHkey_Speedset==TRUE))
            {
                if((KeyM->Skeystop == FALSE)&&(Operation_M.Faultstop == FALSE))
                {
                    Speed_Set(Speed_Ramp);
                }
                KeyM->SHkey_Speedset=FALSE;
            }
        }
        break;
        default:
            break;
    }
    if((KeyM->ACkey_Poweron==FALSE)||(Powerctr_M.Poweron == FALSE))
    {
        Speed_Ramp->Speed_adncnt = 0;
        KeyM->SHkey_Speedset = FALSE;
    }
}

/******************************************************************************
* Function Name  : Speedkeycheck_Recover
* Description    : Speed Button Detection_Restore Cyclic Type
* Input          : Pointer to key management structure, acceleration and deceleration control structure
* Output         : None
* Return         : None
******************************************************************************/
void Speedkeycheck_Recover(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp)
{
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
        KeyM->SRkey_effcnt++;
    else
        KeyM->SRkey_ineff_cnt++;

    if(KeyM->SRkey_ineff_cnt >= KeyM->SRkey_ineff_cntthresh)
    {
        if(KeyM->SRkey_effcnt >= KeyM->SRkey_effcnt_thresh)
         {
                #if defined POWERON_START
                if(((RunningStatus_M==RUN)||(RunningStatus_M==START))&&\
                   (Powerctr_M.Poweron == TRUE)&&(KeyM->ACkey_Poweron == TRUE)&&(Operation_M.Faultstop==FALSE))
                {
                    if(Speed_Ramp->Speed_adncnt >= Speed_Ramp->Speed_Rank)
                        KeyM->Skey_rank_Dir = DIRDOWN;
                    else if (Speed_Ramp->Speed_adncnt <=1)
                        KeyM->Skey_rank_Dir = DIRUP;

                    if(KeyM->Skey_rank_Dir == DIRUP)
                        Speed_Ramp->Speed_adncnt ++;
                    else if(KeyM->Skey_rank_Dir == DIRDOWN)
                        Speed_Ramp->Speed_adncnt --;
                    KeyM->Skeyeffective = TRUE;//档位变化有效
                    KeyM->SHkey_Speedset= TRUE;
                }

                #elif defined  KEY_START
                if((Powerctr_M.Poweron == TRUE)&&(KeyM->ACkey_Poweron == TRUE)&&(Operation_M.Faultstop==FALSE)&&((RunningStatus_M==IDLE)||(RunningStatus_M==RUN)||(RunningStatus_M==START)))
                {
                    Speed_Ramp->Speed_adncnt ++;
                    if(Speed_Ramp->Speed_adncnt > Speed_Ramp->Speed_Rank)
                    {
                        Speed_Ramp->Speed_adncnt = 0;
                        Key_M.Skeystop = TRUE;
                    }
                    if((RunningStatus_M==RUN)||(RunningStatus_M==START))//
                        KeyM->SHkey_Speedset= TRUE;
                   // KeyM->Skeyeffective = TRUE;//档位变化有效
                }
                #endif
        }
        KeyM->SRkey_effcnt = 0;
        KeyM->SRkey_ineff_cnt = 0;
    }
    switch(RunningStatus_M)
    {
        case PRESTART:
        case POSITION:
        case START:
        case RUN:
       if(Powerctr_M.Poweron == TRUE)
        {
            if(KeyM->SHkey_Speedset==TRUE)
            {
                if(Operation_M.Faultstop==FALSE)
                {
                    Speed_Set(Speed_Ramp);
                    SpeedLed_Set(Speed_Ramp,&LED_M);
                }
                KeyM->SHkey_Speedset=FALSE;
            }
            if((Key_M.Skeystop == TRUE)||(Operation_M.Faultstop == TRUE))
            {
                if(Key_M.Skeystopcnt<U8_MAX)
                    Key_M.Skeystopcnt ++;

                if(Key_M.Skeystopcnt <=1)
                {
                      if(RunningStatus_M == RUN)
                      {
                        if(_IQabs(Speed_Ramp->Aftramp)>_IQabs(MLBG_M.Cal_Struc.Angspeed_Aver2))
                         Speed_Ramp->Aftramp = MLBG_M.Cal_Struc.Angspeed_Aver2;

                         Speed_Ramp->Preramp = Speed_Ramp->Faststopspeed;
                         Mpid_M.Speed.Output_New = 0;
                         Mpid_M.Speed.Output_Prev = 0;
                         Speed_Ramp->BreakStop = TRUE;
                      }
                      else if(RunningStatus_M == START)
                      {
                          Speed_Ramp->BreakStop = TRUE;
                      }
                      else
                         RunningStatus_M = STOP;
                }
            }
        }
        break;

        default:
            break;
    }
    if((KeyM->ACkey_Poweron==FALSE)||(Powerctr_M.Poweron == FALSE))
    {
    #if defined KEY_START
        Speed_Ramp->Speed_adncnt = 0;
    #endif
        KeyM->SHkey_Speedset=FALSE;
    }

}

/******************************************************************************
* Function Name  : Speed_Set
* Description    : Speed setting
* Input          : Pointer to acceleration and deceleration control structure, pointer to power-on and power-off control structure
* Output         : None
* Return         : None
*****************************************************************************/
void Speed_Set(Speedrampctr_Type *Speed_Ramp)
{
    switch(Speed_Ramp->Speed_adncnt)
    {
        case 1:
           Speed_Ramp->Preramp = Speed_Ramp->Speed1;
           break;
       case 2:
           Speed_Ramp->Preramp = Speed_Ramp->Speed2;
           break;
       case 3:
           Speed_Ramp->Preramp = Speed_Ramp->Speed3;
           break;
       default:
           break;
    }
}

/******************************************************************************
* Function Name  : Temp_Set
* Description    : Temperature Setting
* Input          : Pointer to key management structure, pointer to acceleration and deceleration control structure
* Output         : None
* Return         : None
*****************************************************************************/
void Temp_Set(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp,TRIAC_ctl_Type *TriacM)
{
    switch(KeyM->Tkey_rank)
        {
            case 1:
                switch(Speed_Ramp->Speed_adncnt)
                {
                    case 1:
                        TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T1S1-TriacM->PeriodCtl.NumData);
                        TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T1S1-TriacM->PeriodCtl.NumData);
                        TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T1S1);
                       break;
                   case 2:
                       TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T1S2-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T1S2-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T1S2);
                       break;
                   case 3:
                       TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T1S3-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T1S3-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T1S3);
                       break;
                   default:
                       break;
                }
               break;
           case 2://温度挡位2
               switch(Speed_Ramp->Speed_adncnt)
               {
                   case 1:
                       TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T2S1-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T2S1-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T2S1);
                      break;
                  case 2:
                      TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T2S2-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T2S2-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T2S2);
                      break;
                  case 3:
                      TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T2S3-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T2S3-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T2S3);
                      break;
                  default:
                      break;
               }
               break;
           case 3:
               switch(Speed_Ramp->Speed_adncnt)
               {
                   case 1:
                       TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T3S1-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T3S1-TriacM->PeriodCtl.NumData);
                       TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T3S1);
                      break;
                  case 2:
                      TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T3S2-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T3S2-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T3S2);
                      break;
                  case 3:
                      TriacM->PeriodCtl.Serial_Nummax=(TriacM->PeriodCtl.Dutymax_T3S3-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Serial_Nummin=(TriacM->PeriodCtl.Dutymin_T3S3-TriacM->PeriodCtl.NumData);
                      TriacM->PeriodCtl.Duty_TS=(TriacM->PeriodCtl.Dutydefault_T3S3);
                      break;
                  default:
                      break;
               }
               break;
           default:
               break;
        }
//#endif
}

/******************************************************************************
* Function Name  : Key_Memwrite
* Description    : Key Memory Writing
* Input          : Pointer to key management structure, acceleration and deceleration control structure
* Output         : None
* Return         : None
******************************************************************************/
void Key_Memwrite(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp,Flash_Operation_Type *FlashM)
{
    KeyM->Keytowrite = ((Speed_Ramp->Speed_adncnt&0x0F)<<4)&0x70;

    KeyM->Keytowrite = KeyM->Keytowrite|(KeyM->Tkey_rank&0x07);

    if(KeyM->Tkey_rank_Dir == DIRUP)
    {
        KeyM->Keytowrite = KeyM->Keytowrite|0x08; 
    }
    else
    {
        KeyM->Keytowrite = KeyM->Keytowrite&0xF7; 
    }

    if(KeyM->Skey_rank_Dir == DIRUP)
    {
        KeyM->Keytowrite = KeyM->Keytowrite|0x80; 
    }
    else
    {
        KeyM->Keytowrite = KeyM->Keytowrite&0x7F; 
    }

    if(FlashM->MemAddress<(PAGE_WRITE_HALF_END_ADDR-4))
    {
        for(KeyM->EEwritecyccnt=0;KeyM->EEwritecyccnt < KeyM->EEwritecycset;KeyM->EEwritecyccnt++)
        {
            Flash_ProgramHalfWord(FlashM,(KeyM->EEwritecyccnt*2048+FlashM->MemAddress),(u16)(KeyM->Keytowrite));
        }
        FlashM->MemAddress+=2;
    }
    if(FlashM->FlashExcep!=1)
    {
        FlashM->FlashData[0]=KeyM->Keytowrite;
        FlashM->FlashData[1]=KeyM->Keytowrite;
    }

    KeyM->Skeyeffective = FALSE;   
    KeyM->Tkeyeffective = FALSE;   

}

/******************************************************************************
* Function Name  : Keymem_Read
* Description    : Key Memory Recall
* Input          : Pointer to key management structure, acceleration and deceleration control structure
* Output         : None
* Return         : None
*****************************************************************************/
void Key_Memread(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp,Flash_Operation_Type *FlashM)
{
    uint8_t i,j;

    for(i=0; i<KeyM->EEwritecycset; i++)
        Keymembuff[i]=FlashM->FlashData[i];

    for(i=0,j=1; i<KeyM->EEwritecycset-1; i++,j++)
    {
        if((Keymembuff[i]!=Keymembuff[j])||(FlashM->FlashExcep==1))
        {
            KeyM->Tkey_rank = 0;
            #ifdef POWERON_START
            Speed_Ramp->Speed_adncnt = 1;
            #endif
            break;
        }
        else
        {
            //温度档赋值
            KeyM->Tkey_rank = Keymembuff[0]&0x07;//bit210；
            if(KeyM->Tkey_rank > KeyM->Tkey_rankset)
                KeyM->Tkey_rank = 0;

            if((Keymembuff[0]&0x08)!=0)
                KeyM->Tkey_rank_Dir = DIRUP;
            else
                KeyM->Tkey_rank_Dir = DIRDOWN;

            #ifdef POWERON_START

            Speed_Ramp->Speed_adncnt = (Keymembuff[0]&0x70)>>4;//bit654

            if((Keymembuff[0]&0x80)!=0)
                KeyM->Skey_rank_Dir = DIRUP;
            else
                KeyM->Skey_rank_Dir = DIRDOWN;

            if((Speed_Ramp->Speed_adncnt > Speed_Ramp->Speed_Rank)||(Speed_Ramp->Speed_adncnt ==0))
                Speed_Ramp->Speed_adncnt = 1;

            Key_Memwrite(&Key_M,&SpeedRamp_M,&Flash_M);
            #endif
        }
    }
}

/******************************************************************************
* Function Name  : Time_Delay_Process
* Description    : The program is used for variable decrement processing based on interrupt time (0.5 ms).
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Time_Delay_Process(void)
{
    if (Timewait != 0)
        Timewait --;

    if (Operation_M.StopwaitCnt != 0)
        Operation_M.StopwaitCnt --;

    if (Key_M.EEwritewaitcnt != 0)
        Key_M.EEwritewaitcnt --;

    if (Operation_M.FaultHoldingCnt != 0)
        Operation_M.FaultHoldingCnt--;

	if (SpeedRamp_M.Breakwait_cnt != 0)
	{
		SpeedRamp_M.Breakwait_cnt --;
	}

    if (SpeedRamp_M.Precharge_cnt != 0)
        SpeedRamp_M.Precharge_cnt --;

    if (LED_M.LED4.FlickCnt != 0)
        LED_M.LED4.FlickCnt --;

    if (LED_M.LED5.FlickCnt != 0)
        LED_M.LED5.FlickCnt --;

    if (LED_M.LED6.FlickCnt != 0)
        LED_M.LED6.FlickCnt --;

    if (LED_M.LED7.FlickCnt != 0)
        LED_M.LED7.FlickCnt --;

    if (LED_M.LED8.FlickCnt != 0)
        LED_M.LED8.FlickCnt --;

    if (Emanager_M.Event_10ms_cnt != 0)
        Emanager_M.Event_10ms_cnt --;
    else
        Emanager_M.Event_10ms = TRUE;

    if (Emanager_M.Event_1ms_cnt != 0)
        Emanager_M.Event_1ms_cnt --;
    else
        Emanager_M.Event_1ms = TRUE;

    if (Emanager_M.Event_2s_cnt != 0)
        Emanager_M.Event_2s_cnt --;
    else
        Emanager_M.Event_2s = TRUE;


    if (Operation_M.PSC.Ledcnt != 0)
        Operation_M.PSC.Ledcnt --;

    if (Triac_ctl.TonEnable_cnt != 0)
        Triac_ctl.TonEnable_cnt--;

    if (Triac_ctl.Zero.ZcrossDelayTrigcnt != 0)
         Triac_ctl.Zero.ZcrossDelayTrigcnt--;


}

/******************************************************************************
* Function Name  : Time_Wait
* Description    : The function wait for a delay to be over.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Time_Wait(uint16_t time)
{
    Timewait = time;
    while (Timewait != 0)
    {;}
}

/******************************************************************************
* Function Name  : Time_Delay_Set
* Description    : Timer time setting function
* Input          : Timing time, timer count variable pointer
* Output         : None
* Return         : None
******************************************************************************/
void Time_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter)
{
    *Delaycounter = Delayset;
}

/******************************************************************************
* Function Name  : Time_Delay_Elapse
* Description    : Timer expiration determination
* Input          : timing and counting variable
* Output         : Judgment result, time reached: TRUE, time not reached: FALSE
* Return         : Judgment result
******************************************************************************/
Truth_Verify_Type Time_Delay_Elapse(uint16_t Delaycounter)
{
     if (Delaycounter == 0)
       return (TRUE);
     else
       return (FALSE);
}

/******************************************************************************
* Function Name  : Operation_Parameter_Define
* Description    : Operation Parameter Definition
* Input          : Operate control structure pointer
* Output         : None
* Return         : None
*****************************************************************************/
void Operation_Parameter_Define(Operation_Type *Operastruc)
{
    Operastruc->StopwaitSet = (uint16_t)(STOP_WAITING_TIME * OPERATION_CAL_FREQ);
    Operastruc->FaultHoldingSet = (uint16_t)(FAULT_HOLDING_TIME * OPERATION_CAL_FREQ);
    Operastruc->Faultstop = FALSE;

    Operastruc->PSC.Ledinttimeset = (uint16_t)(PSC_LED_INTERVAL*PSC_FREQ);
    Operastruc->PSC.Ledintnumset = PSC_LED_INTERVAL_NUM;
    Operastruc->PSC.Ledintnumtotalset = (uint16_t)(PSC_LED_INTERVAL_TOTAL/PSC_LED_INTERVAL);
    Operastruc->PSC.Ledlasttimeset = (uint16_t)(PSC_LED_LAST*PSC_FREQ);

    Operastruc->PSC.Mark = FALSE;
    Operastruc->PSC.Stage = NONE;
    Operastruc->PSC.Process = FALSE;
    Operastruc->PSC.Ledintdir =DIRUP;
}

/******************************************************************************
* Function Name  : Operation_Status_Init
* Description    : Operation status initialization
* Input          : Operate control structure pointer
* Output         : None
* Return         : None
*****************************************************************************/
void Operation_Status_Init(Operation_Type *Operastruc)
{
    Operastruc->StopwaitCnt = 0;
    Operastruc->FaultHoldingCnt = 0;
    Operastruc->reset_cnt = 0;

    Operastruc->PSC.Ledcnt = 0;
    Operastruc->PSC.Ledintnum = 0;
    Operastruc->PSC.Ledintnumtotal = 0;
}

/******************************************************************************
* Function Name : Poweroperation_Process
* Description   : Power On/Off Control Logic
* Input         : Pointer to acceleration and deceleration control structure, 
*                      pointer to ADC sampling structure, pointer to power-on/off control structure,
*                      operating status, pointer to speed PID controller, actual speed
* Output        : None
* Return        : None
******************************************************************************/
void Poweroperation_Process(Speedrampctr_Type *SpeedRamp,ADCStruc_Type *ADCStruc,\
                            Powercontrol_Type *PowerOper,Runningstatus_Type *Runningstatus,\
                            PID_Structure_Type *Speedpid,_iq24 Speedact)
{
        PowerOper->DCVoltAver = _IQMPY_MACRO(PowerOper->DCVoltAver,PowerOper->DCVoltAverSizeInv2)\
                                + _IQMPY_MACRO(ADCStruc->DCVoltPres,PowerOper->DCVoltAverSizeInv1);

    if((_IQabs(SpeedRamp->Aftramp) >= SpeedRamp->Startfinishspeed)&&(_IQabs(Speedact) >= SpeedRamp->Startfinishspeed))
        SpeedRamp->Startfinish = TRUE;

    if( PowerOper->DCVoltAver <= PowerOper->Poweroff_Volt2)
    {
//        if(*Runningstatus == RUN || *Runningstatus == INIT \
//                || *Runningstatus == START|| *Runningstatus == PRESTART)
//        {
//            if(SpeedRamp->Break !=FALSE)//不在刹车状态
//            {
//                *Runningstatus = STOP;
//                SpeedRamp->Break =FALSE;
//            }
//        }
        PowerOper->Poweron = FALSE;
    }

    if(PowerOper->Poweron == TRUE)
    {
        if(( PowerOper->DCVoltAver < PowerOper->Poweroff_Volt1)&&( PowerOper->DCVoltAver > PowerOper->Poweroff_Volt2))
        {
            if(*Runningstatus == RUN)
            {
                if(_IQabs(SpeedRamp->Aftramp)>_IQabs(Speedact))
                SpeedRamp->Aftramp = Speedact;
                SpeedRamp->Preramp = SpeedRamp->Faststopspeed;
                Speedpid->Output_New = 0;
                Speedpid->Output_Prev = 0;
            }
            PowerOper->Poweron = FALSE;
        }
    }

    if( PowerOper->DCVoltAver > PowerOper->Poweron_Volt)
    {
        PowerOper->Poweron_Cnt++;
        if(PowerOper->Poweron_Cnt >= PowerOper->Poweron_CntThresh)
        {
            PowerOper->Poweron = TRUE;
            PowerOper->Poweron_Cnt = 0;
        }
    }
}

/******************************************************************************
* Function Name : LED_Reset
* Description   : LED Reset
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void LED_Reset(Led_Manager_Type *LEDM)
{
       LEDM->Pin1.Group->CFGHR&=~(0xf<<(4*0));//PB8
       LEDM->Pin2.Group->CFGLR&=~(0xf<<(4*0));//PA0
       LEDM->Pin3.Group->CFGHR&=~(0xf<<(4*3));//PA11

       LEDM->Pin1.Group->CFGHR|=(0x4<<(4*0));//PB8
       LEDM->Pin2.Group->CFGLR|=(0x4<<(4*0));//PA0
       LEDM->Pin3.Group->CFGHR|=(0x4<<(4*3));//PA11

}
/******************************************************************************
* Function Name : LED4_ON
* Description   : LED4ON
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void LED4_ON(Led_Manager_Type *LEDM)
{
    LEDM->Pin1.Group->CFGHR&=~(0xf<<(4*0));//PB8
    LEDM->Pin1.Group->CFGHR|=0x3<<(4*0);
    GPIO_SetBits( LEDM->Pin1.Group,LEDM->Pin1.Pin);
}

/******************************************************************************
* Function Name : LED5_ON
* Description   : LED5ON
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void LED5_ON(Led_Manager_Type *LEDM)
{
    LEDM->Pin1.Group->CFGHR&=~(0xf<<(4*0));//PB8
    LEDM->Pin1.Group->CFGHR|=0x3<<(4*0);
    GPIO_ResetBits( LEDM->Pin1.Group,LEDM->Pin1.Pin);
}

/******************************************************************************
* Function Name : LED6_ON
* Description   : LED6ON
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void LED6_ON(Led_Manager_Type *LEDM)
{
    LEDM->Pin2.Group->CFGLR&=~(0xf<<(4*0));//PA0
    LEDM->Pin2.Group->CFGLR|=0x3<<(4*0);
    GPIO_ResetBits( LEDM->Pin2.Group,LEDM->Pin2.Pin);
}

/******************************************************************************
* Function Name : LED7_ON
* Description   : LED7ON
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void LED7_ON(Led_Manager_Type *LEDM)
{
    LEDM->Pin2.Group->CFGLR&=~(0xf<<(4*0));//PA0
    LEDM->Pin2.Group->CFGLR|=0x3<<(4*0);
    GPIO_SetBits( LEDM->Pin2.Group,LEDM->Pin2.Pin);
}

/******************************************************************************
* Function Name : LED8_ON
* Description   : LED8ON
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void LED8_ON(Led_Manager_Type *LEDM)
{
    LEDM->Pin3.Group->CFGHR&=~(0xf<<(4*3));//PA11
    LEDM->Pin3.Group->CFGHR|=0x3<<(4*3);
    GPIO_ResetBits( LEDM->Pin3.Group,LEDM->Pin3.Pin);
}

/******************************************************************************
* Function Name : LED9_ON
* Description   : LED9ON
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void LED9_ON(Led_Manager_Type *LEDM)
{
    LEDM->Pin3.Group->CFGHR&=~(0xf<<(4*3));//PA11
    LEDM->Pin3.Group->CFGHR|=0x3<<(4*3);
    GPIO_SetBits( LEDM->Pin3.Group,LEDM->Pin3.Pin);
}

/******************************************************************************
* Function Name : Led_Operation
* Description   : LED operation
* Input         : LED Management Pointer
* Output        : None
* Return        : None
******************************************************************************/
void Led_Operation(Led_Manager_Type *LEDM)
{
    static u8 Led_cnt;
    Led_cnt++;
    if(Led_cnt>2)
        Led_cnt=1 ;

    LED_Reset(LEDM);
    switch(Led_cnt)
    {
        case 1:
            if(((LEDM->LED5.LightFlag)&&(Key_M.TCkeycold==FALSE))||((LEDM->LED5.LightFlag)&&(Tempctl_M.NTCFlickerStatus ==TRUE))||((LEDM->LED5.LightFlag)&&(RunningStatus_M ==FAULT)))
            LED5_ON(LEDM);

            if(((LEDM->LED6.LightFlag)&&(Key_M.TCkeycold==FALSE))||((LEDM->LED6.LightFlag)&&(Tempctl_M.NTCFlickerStatus ==TRUE))||((LEDM->LED6.LightFlag)&&(RunningStatus_M ==FAULT)))
                LED6_ON(LEDM);

            if(LEDM->LED8.LightFlag)
                LED8_ON(LEDM);
            break;
        case 2:
            if(((LEDM->LED4.LightFlag)&&(Key_M.TCkeycold==FALSE))|| ((LEDM->LED4.LightFlag)&&(Tempctl_M.NTCFlickerStatus ==TRUE))||((LEDM->LED4.LightFlag)&&(RunningStatus_M ==FAULT)) )//温度
            LED4_ON(LEDM);

            if(LEDM->LED7.LightFlag)
                LED7_ON(LEDM);

            if(LEDM->LED9.LightFlag)
                LED9_ON(LEDM);
    }

}
/******************************************************************************
* Function Name  : SpeedLed_Set
* Description    : Speed gear LED setting
* Input          : Pointer to acceleration and deceleration control structure, pointer to LED management structure
* Output         : None
* Return         : None
*****************************************************************************/
void SpeedLed_Set(Speedrampctr_Type *Speed_Ramp,Led_Manager_Type *LEDM)
{
    switch(Speed_Ramp->Speed_adncnt)
    {
        case 0:
            LEDM->LED7.LightFlag=0;
            LEDM->LED8.LightFlag=0;
            LEDM->LED9.LightFlag=0;
           break;
        case 1:
            LEDM->LED7.LightFlag=1;
            LEDM->LED8.LightFlag=0;
            LEDM->LED9.LightFlag=0;
           break;
       case 2:
           LEDM->LED7.LightFlag=1;
           LEDM->LED8.LightFlag=1;
           LEDM->LED9.LightFlag=0;
           break;
       case 3:
           LEDM->LED7.LightFlag=1;
           LEDM->LED8.LightFlag=1;
           LEDM->LED9.LightFlag=1;
           break;
       default:
           break;
    }
}

/******************************************************************************
* Function Name  :TempLed_Set
* Description    : Temperature gear LED setting
* Input          : Pointer to key management structure, pointer to LED management structure
* Output         : None
* Return         : None
*****************************************************************************/
void TempLed_Set(Key_Manager_Type *KeyM,Led_Manager_Type *LEDM)
{
//    if((Tempctl_M.NTCFlickerStatus ==FALSE) && (KeyM->TCkeycold==FALSE) && (Tempctl_M.NTCShortStatus==FALSE) && (Tempctl_M.NTCOpenStatus==FALSE) )
//    {
        switch(KeyM->Tkey_rank)
        {
            case 0:
                LEDM->LED4.LightFlag=0;
                LEDM->LED5.LightFlag=0;
                LEDM->LED6.LightFlag=0;
                break;
            case 1:
                LEDM->LED4.LightFlag=0;
                LEDM->LED5.LightFlag=0;
                LEDM->LED6.LightFlag=1;
               break;
            case 2:
                LEDM->LED4.LightFlag=0;
                LEDM->LED5.LightFlag=1;
                LEDM->LED6.LightFlag=1;
               break;
           case 3:
               LEDM->LED4.LightFlag=1;
               LEDM->LED5.LightFlag=1;
               LEDM->LED6.LightFlag=1;
               break;
           default:
               break;
        }
   // }
}

/******************************************************************************
* Function Name  : LED_OFF
* Description    : LED OFF
* Input          : Pointer to LED management structure
* Output         : None
* Return         : None
*****************************************************************************/
void LED_OFF(Led_Manager_Type *LEDM)
{
    LED_M.LED4.LightFlag=0;
    LED_M.LED5.LightFlag=0;
    LED_M.LED6.LightFlag=0;
    LED_M.LED7.LightFlag=0;
    LED_M.LED8.LightFlag=0;
    LED_M.LED9.LightFlag=0;
}

/******************************************************************************
* Function Name  : LED_Flicker
* Description    : LED Flicker Handling
* Input          : LED structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void LED_Flicker(LED_Type *LedM)
{
    if (Time_Delay_Elapse(LedM->FlickCnt) == TRUE)
        {
            LedM->FlickCtl++;
            Time_Delay_Set(LedM->FlickInterval,&(LedM->FlickCnt));

            if(LedM->FlickCtl == 1)
            {
                LED_M.LED4.LightFlag=1;
                LED_M.LED5.LightFlag=1;
                LED_M.LED6.LightFlag=1;
            }

            if(LedM->FlickCtl == 2)
            {
                LED_M.LED4.LightFlag=0;
                LED_M.LED5.LightFlag=0;
                LED_M.LED6.LightFlag=0;
                LedM->FlickCtl =0;
            }

            LED_M.LED7.LightFlag=0;
            LED_M.LED8.LightFlag=0;
            LED_M.LED9.LightFlag=0;
        }
}

/******************************************************************************
* Function Name  : ACON_CHECK
* Description    : AC power-on detection
* Input          : Button management structure pointer
* Output         : None
* Return         : None
******************************************************************************/
void ACKEY_CHECK(Key_Manager_Type *KeyM)
{
    KeyM->ACkey_checkcnt++;

    //AC上电
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
        KeyM->ACkey_effcnt++;
    else
        KeyM->ACkey_ineff_cnt++;

    if (KeyM->ACkey_checkcnt >= KeyM->ACkey_checkinterval)
    {
        if(KeyM->ACkey_effcnt >= KeyM->ACkey_effcnt_thresh)
        {
            KeyM->ACkey_Poweron = TRUE;
        }
        else if(KeyM->ACkey_ineff_cnt >= KeyM->ACkey_ineff_cntthresh)
        {
            KeyM->ACkey_Poweron = FALSE;
        }

        KeyM->ACkey_checkcnt = 0;
        KeyM->ACkey_effcnt = 0;
        KeyM->ACkey_ineff_cnt = 0;
    }
}

/******************************************************************************
* Function Name  : PSC_Process
* Description    : Power-on self-test processing
* Input          : Pointer to key management structure, pointer to LED management structure
* Output         : None
* Return         : None
******************************************************************************/
void PSC_Process(Operation_Type *Operation)
{
    switch (Operation->PSC.Stage)
    {
        case NONE:
#ifdef  POWERONCHECK
            if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)&&(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)== 0)&&(Key_M.ACkey_Poweron == FALSE))
                Operation->PSC.Stage = CHECK;
#endif
                Operation->PSC.Process = TRUE;

            break;
        case CHECK:
            
            if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)&&(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)== 0)&&(Key_M.ACkey_Poweron == TRUE))
            {
                Operation->PSC.Mark =TRUE;

                Operation_M.PSC.Ledintnum = 0;
                Operation_M.PSC.Ledintnumtotal = 0;

                Operation->PSC.Stage = INTERVAL;
                Time_Delay_Set(Operation->PSC.Ledinttimeset,&(Operation->PSC.Ledcnt));
            }
        break;

        case INTERVAL:
            if(Operation->PSC.Ledcnt ==0)
            {
                if(Operation->PSC.Ledintdir == DIRUP)
                {
                    Operation_M.PSC.Ledintnum++;
                    if(Operation_M.PSC.Ledintnum >= (Operation_M.PSC.Ledintnumset-1))
                    {
                        Operation->PSC.Ledintdir = DIRDOWN;
                    }
                }
                else if(Operation->PSC.Ledintdir == DIRDOWN)
                {
                    Operation_M.PSC.Ledintnum--;
                    if(Operation_M.PSC.Ledintnum <= 0)
                    {
                        Operation->PSC.Ledintdir = DIRUP;
                    }
                }

                Operation_M.PSC.Ledintnumtotal++;

                Time_Delay_Set(Operation_M.PSC.Ledinttimeset,&(Operation->PSC.Ledcnt));
            }
            LED_Reset(&LED_M);
            switch(Operation->PSC.Ledintnum)
            {
                case 0:
                    LED9_ON(&LED_M);
                 break;

                case 1:
                    LED8_ON(&LED_M);
                  break;

                case 2:
                    LED7_ON(&LED_M);
                 break;

                default:
                 break;
            }
            if(Operation_M.PSC.Ledintnumtotal >= Operation_M.PSC.Ledintnumtotalset)
            {
                Operation_M.PSC.Stage = LAST;
                LED_Reset(&LED_M);
                Time_Delay_Set(Operation->PSC.Ledlasttimeset,&(Operation->PSC.Ledcnt));
            }
        break;

        case LAST:
            LED6_ON(&LED_M);
            if(Operation_M.PSC.Ledcnt ==0)
            {
                Operation_M.PSC.Stage = NONE;
                LED_Reset(&LED_M);
            }
        break;

        default:
        break;
    }
}
