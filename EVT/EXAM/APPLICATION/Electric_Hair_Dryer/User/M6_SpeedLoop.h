/********************************** (C) COPYRIGHT *******************************
 * File Name          : M6_SpeedLoop.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : The speed loop is defined by timer parameters and control counting parameters.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __M6_SPEEDLOOP_H
#define __M6_SPEEDLOOP_H
#define TIMER_CLOCK_FREQ_SPEED      144000000uL     
#define TIMER_FREQ_SPEED_LOOP       2000            

#define TIMER_PERIDO_SYSTICK     TIMER_CLOCK_FREQ_SPEED / (u32)(TIMER_FREQ_SPEED_LOOP*8)

#define TIMER_PRESCALER_SPEED_LOOP  17             

#define SPEED_LOOP_CAL_FREQ         2000

#define SPEED_LOOP_CAL_PERIOD      _IQ(1.0)/SPEED_LOOP_CAL_FREQ

#define SPEED_SAMPLE_PID_PERIOD     TIMER_FREQ_SPEED_LOOP/SPEED_LOOP_CAL_FREQ
#define LED_PERIOD                      4

//void Timer4_Init(void);
void Systick_Init(void);
void SYS_TIM_Cmd(FunctionalState NewState);
#endif
