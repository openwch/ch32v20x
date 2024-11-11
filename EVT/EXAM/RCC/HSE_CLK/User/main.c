/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 *HSE frequency check routine:
 *Checking the frequency value of HSE by using of HSI.
 * When HSE is 3 ~ 25M,the HSE clock is effective ,When CH32V20x_D6 used.
 * When HSE is 32M,the HSE clock is effective ,When CH32V20x_D8 and CH32V20x_D8W used.
 *HSE value -the frequency of HSE
 *MCO(PA8)	- outputs the HSE clock
 *
 */

#include "debug.h"

uint8_t HSEFrequencyMhz;
uint8_t value;
#define  F1M                      1000000
#define  CHECK_ERR_COUNT          (8)

#define  RTC_COUNT_DELAY          (100)
#define  HSIClock                 (8000000)
#define  PPL2_ENABLE              1
#define  HSE_ENABLE               0

#define  STclk_div8               0
#define  STclk_div1               1

#define  CHECK_MODE    STclk_div8
//#define  CHECK_MODE      STclk_div1
uint8_t value=0;

/*********************************************************************
 * @fn      HSE_FrequencyCheck
 *
 * @brief   Checking HSE Frequency  Values.
 *
 * @return  HSEFrequencyMhz - HSE Frequency
 */
uint8_t HSE_FrequencyCheck(void) {

    uint32_t rtc_val;
    uint32_t tick_val;
    uint32_t HSEFrequency, HSEFrequency_x, RTC_DIV_VAL,RTC_CHANGE_TIMEOUT,RTC_COUNT_MAX_TIMEOUT;
    uint8_t count;
#if defined (CH32V20x_D6)
    if(((*(uint32_t *) 0x40022030) & 0x0F000000) != 0)
     {
       RTC_DIV_VAL=128;
     }
  else {
      RTC_DIV_VAL=512;
    }

#else
           RTC_DIV_VAL=512;
#endif

    RTC_CHANGE_TIMEOUT=(HSIClock/F1M)*RTC_DIV_VAL;
    RTC_COUNT_MAX_TIMEOUT=(HSIClock/F1M)*RTC_DIV_VAL*RTC_COUNT_DELAY;

    Delay_Init();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    BKP_DeInit();
#if defined(CH32V20x_D8) || defined(CH32V20x_D8W)
   RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div512);
#else

    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
#endif
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForLastTask();
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    RTC_EnterConfigMode();
    RTC_SetPrescaler(0);
    RTC_WaitForLastTask();
    RTC_ExitConfigMode();
    while( (RCC->BDCTLR & (1<<15)) != (1<<15) ) {}
    count = CHECK_ERR_COUNT;
    while(1) {
        SysTick->CTLR = 0;
        SysTick->CNT =0;
#if (CHECK_MODE == STclk_div1)
        SysTick->CTLR = (1<<5)|(1<<2)|(1<<0);
#elif (CHECK_MODE == STclk_div8)
        SysTick->CTLR = (1<<5)|(1<<0);
#endif
        rtc_val = RTC_GetCounter();

        while( rtc_val == RTC_GetCounter()) {
            if( SysTick->CNT > RTC_CHANGE_TIMEOUT ) {

                return 0;
            }
        }
        SysTick->CTLR = 0;
        SysTick->CNT =0;
#if (CHECK_MODE == STclk_div1)
        SysTick->CTLR = (1<<5)|(1<<2)|(1<<0);
#elif (CHECK_MODE == STclk_div8)
        SysTick->CTLR = (1<<5)|(1<<0);
#endif
        rtc_val += (RTC_COUNT_DELAY);
        while( rtc_val >= RTC_GetCounter() ) {
            if( SysTick->CNT > RTC_COUNT_MAX_TIMEOUT ) {
                return 0;
            }
        }
        tick_val = SysTick->CNT;
        SysTick->CTLR = 0;
#if (CHECK_MODE == STclk_div1)
        HSEFrequency = HSIClock/(tick_val/RTC_DIV_VAL)*RTC_COUNT_DELAY;
#elif (CHECK_MODE == STclk_div8)
        HSEFrequency = HSIClock/(tick_val/(RTC_DIV_VAL/8))*RTC_COUNT_DELAY;
#endif
        HSEFrequencyMhz = (HSEFrequency+(F1M/2))/F1M;


        SysTick->CTLR = 0;
        SysTick->CNT =0;
#if (CHECK_MODE == STclk_div1)
        SysTick->CTLR = (1<<5)|(1<<2)|(1<<0);
#elif (CHECK_MODE == STclk_div8)
        SysTick->CTLR = (1<<5)|(1<<0);
#endif
        rtc_val = RTC_GetCounter();
        while( rtc_val == RTC_GetCounter() ) {
            if( SysTick->CNT > RTC_CHANGE_TIMEOUT ) {
                return 0;
            }
        }
        SysTick->CTLR = 0;
        SysTick->CNT =0;
#if (CHECK_MODE == STclk_div1)
        SysTick->CTLR = (1<<5)|(1<<2)|(1<<0);
#elif (CHECK_MODE == STclk_div8)
        SysTick->CTLR = (1<<5)|(1<<0);
#endif
        rtc_val += (RTC_COUNT_DELAY);
        while( rtc_val >= RTC_GetCounter() ) {
            if( SysTick->CNT > RTC_COUNT_MAX_TIMEOUT ) {
                return 0;
            }
        }
        tick_val = SysTick->CNT;
        SysTick->CTLR = 0;
#if (CHECK_MODE == STclk_div1)
        HSEFrequency = HSIClock/(tick_val/RTC_DIV_VAL)*RTC_COUNT_DELAY;
#elif (CHECK_MODE == STclk_div8)
        HSEFrequency = HSIClock/(tick_val/(RTC_DIV_VAL/8))*RTC_COUNT_DELAY;
#endif

        HSEFrequency_x = (HSEFrequency+(F1M/2))/F1M;

        if( HSEFrequencyMhz == HSEFrequency_x ) {
            break;
        }
        if( --count == 0 ) {
            HSEFrequencyMhz = 253;
            printf("HSE check error.\n");
            break;
        }
    }
    return HSEFrequencyMhz;
}

/*********************************************************************
 * @fn      SetSysClockTo72
 *
 * @brief   Sets System clock frequency to 72MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo72(void) {
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    USART_Printf_Init(115200);
    RCC->CTLR |= ((uint32_t) RCC_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do {
        HSEStatus = RCC->CTLR & RCC_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CTLR & RCC_HSERDY) != RESET) {
        value = HSE_FrequencyCheck();
        HSEStatus = (uint32_t) 0x00;
#if defined(CH32V20x_D8) || defined(CH32V20x_D8W)
        if (value == 32) {
            HSEStatus = (uint32_t) 0x01;
        }
#else
      if ((value >= 3) && (value <= 25)) {
       HSEStatus = (uint32_t) 0x01;
   }
#endif

    } else {
        HSEStatus = (uint32_t) 0x00;
    }

    if (HSEStatus == (uint32_t) 0x01) {

        /* HCLK = SYSCLK */
        RCC->CFGR0 |= (uint32_t) RCC_HPRE_DIV1;
        /* PCLK2 = HCLK */
        RCC->CFGR0 |= (uint32_t) RCC_PPRE2_DIV1;
        /* PCLK1 = HCLK */
        RCC->CFGR0 |= (uint32_t) RCC_PPRE1_DIV2;

        /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
        RCC->CFGR0 &= (uint32_t) ((uint32_t) ~(RCC_PLLSRC | RCC_PLLXTPRE |
        RCC_PLLMULL));

        RCC->CFGR0 |= (uint32_t) (RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE
                | RCC_PLLMULL9);

        /* Enable PLL */
        RCC->CTLR |= RCC_PLLON;
        /* Wait till PLL is ready */
        while((RCC->CTLR & RCC_PLLRDY) == 0)
        {
        }
        /* Select PLL as system clock source */
        RCC->CFGR0 &= (uint32_t) ((uint32_t) ~(RCC_SW));
        RCC->CFGR0 |= (uint32_t) RCC_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        {
        }
    } else {

        EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;
        /* HCLK = SYSCLK */
        RCC->CFGR0 |= (uint32_t) RCC_HPRE_DIV1;
        /* PCLK2 = HCLK */
        RCC->CFGR0 |= (uint32_t) RCC_PPRE2_DIV1;
        /* PCLK1 = HCLK */
        RCC->CFGR0 |= (uint32_t) RCC_PPRE1_DIV2;

        /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
        RCC->CFGR0 &= (uint32_t) ((uint32_t) ~(RCC_PLLSRC | RCC_PLLXTPRE |
        RCC_PLLMULL));

        RCC->CFGR0 |= (uint32_t) (RCC_PLLSRC_HSI_Div2 | RCC_PLLXTPRE_HSE
                | RCC_PLLMULL9);

        /* Enable PLL */
        RCC->CTLR |= RCC_PLLON;
        /* Wait till PLL is ready */
        while((RCC->CTLR & RCC_PLLRDY) == 0)
        {
        }
        /* Select PLL as system clock source */
        RCC->CFGR0 &= (uint32_t) ((uint32_t) ~(RCC_SW));
        RCC->CFGR0 |= (uint32_t) RCC_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        {
        }
    }
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure={0};

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SetSysClockTo72();
	USART_Printf_Init(115200);
	SystemCoreClockUpdate();
	printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("HSE value  %02d\r\n", value);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* MCO Output GPIOA 8 */
	RCC_MCOConfig( RCC_MCO_HSE );

	while(1);
}
