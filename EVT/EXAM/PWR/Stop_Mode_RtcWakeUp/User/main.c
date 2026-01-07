/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.1
 * Date               : 2025/09/29
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 * Stop Mode:
 * The example demonstrates WFI\WFE enter the STOP-Regulator_On mode or the STOP-Regulator_LowPower mode. 
 * In the example, the RTC alarm clock serves as the wake-up source. By setting the frequency division coefficient of 
 * the RTC clock and the alarm value, the system is awakened at regular intervals based on the set alarm time.
 * When using the STOP-Regulator_LowPower mode, The LDO and ULLDO need to be adjusted before the system 
 * enters the stop mode and restored first after waking up.If the program is running in a non-zero wait state, 
 * the sleep part of the program needs to be placed in RAM for execution(add prefix __attribute__((section("highcode"))) 
 * before function,printf functions and C function library is not applicable to run in RAM.).
 * 
 */

#include "debug.h"

/* Global define */

/* definition of voltage regulator mode in stop mode */
#define STOP_On_Mode        0
#define STOP_LowPower_Mode  1
#define STOP_Mode  STOP_On_Mode
// #define STOP_Mode  STOP_LowPower_Mode

/* definition of program operate area */
#define RunIn_ZeroWaitArea      0
#define RunIn_NonZeroWaitArea   1
#define Operating_Area_Type   RunIn_ZeroWaitArea
// #define Operating_Area_Type   RunIn_NonZeroWaitArea

/* definition of execute with WFI or WFE */
#define Enter_WFI_Mode    0
#define Enter_WFE_Mode    1
// #define Enter_Mode  Enter_WFI_Mode
#define Enter_Mode  Enter_WFE_Mode

/* Global Variable */
volatile uint32_t REGPSC, REGCNT, REGALR = 0;

/*********************************************************************
 * @fn      RTC_Alarm_WakeUp
 *
 * @brief  Initializes RTC alarm wake-up function.
 *
 * @return  none
 */
void RTC_Alarm_WakeUp(void)
{
    EXTI_InitTypeDef EXTI_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RTC_ClearITPendingBit(RTC_IT_ALR);
    RTC_ClearITPendingBit(RTC_IT_SEC);
    /* Enable LSI oscillator */
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    /* Enable the RTC clock */
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForLastTask();
    RTC_WaitForSynchro();
    /* Enable alarm interrupt */
    RTC_ITConfig( RTC_IT_ALR, ENABLE );
    RTC_WaitForLastTask();
    RTC_EnterConfigMode();
    /* Set the RTC prescaler value */
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();
    RTC_SetCounter(0);
    RTC_WaitForLastTask();
    RTC_ExitConfigMode();
#if(Enter_Mode == Enter_WFI_Mode)
    /* Enable the RTC alarm interrupt */
    NVIC_SetPriority(RTCAlarm_IRQn, 1);
    NVIC_EnableIRQ(RTCAlarm_IRQn);
#endif
    /* Configure EXTI line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
#if(Enter_Mode == Enter_WFI_Mode)
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
#elif(Enter_Mode == Enter_WFE_Mode)
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
#endif
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /* Set the RTC alarm value */
    RTC_SetAlarm(RTC_GetCounter()+5);
    RTC_WaitForLastTask();

}

/*********************************************************************
 * @fn      GPIO_ALL_IPD
 *
 * @brief   Set all pins mode to pull-down.
 *
 * @return  none
 */
void GPIO_ALL_IPD(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    /* To reduce power consumption, unused GPIOs need to be set as pull-down inputs */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|
                           RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      LDO_Deal
 *
 * @brief   Deal LDO ULLDO mode.
 *
 * @return  none
 */
 #if(Operating_Area_Type == RunIn_NonZeroWaitArea)
__attribute__((section(".highcode")))
#endif
void LDO_Deal(void)
{
    uint32_t tmpreg = 0;

    REGPSC = 0;
    REGALR = 0;
    REGCNT = 0;
    if((uint8_t)((*(vu32*)0x4002203C >> 24) & 0xFF) != 0xAB)
    {
        if((RCC->APB1PCENR & (1 << 27)) == RESET) 
        {
            RCC->APB1PCENR |= (1 << 27);
            REGPSC |= (1 << 27);
        } 
        if((PWR->CTLR & (1 << 8)) == RESET)
        {
            PWR->CTLR |= (1 << 8);
            REGPSC |= (1 << 26);
        } 
        if((EXTI->EVENR & (1 << 17)) == RESET)
        {
            EXTI->EVENR |= (1 << 17);
            REGPSC |= (1 << 25);
        } 
        if((EXTI->RTENR & (1 << 17)) == RESET)
        {
            EXTI->RTENR |= (1 << 17);
            REGPSC |= (1 << 24);
        } 

        if((RCC->BDCTLR & (1 << 15)) == RESET)
        { 
            if((RCC->RSTSCKR & (1 << 1)) == RESET) REGPSC |= (1 << 30);

            if((RCC->BDCTLR & (1 << 8)) == RESET)
            {
                RCC->RSTSCKR |= (1 << 0);
                while ((RCC->RSTSCKR & (1 << 1)) == RESET);
                RCC->BDCTLR |= (1 << 9);
            }
            REGPSC |= (1 << 29);
        }
    }
    REGPSC |= (0xF << 20);

    tmpreg = EXTEN->EXTEN_CTR;
    tmpreg &= ~(0xF << 8);
    tmpreg |= (0x3 << 8);
    EXTEN->EXTEN_CTR = tmpreg;
}

/*********************************************************************
 * @fn      LDO_Restore
 *
 * @brief   Restore LDO Value.
 *
 * @return  none
 */
#if(Operating_Area_Type == RunIn_NonZeroWaitArea)
__attribute__((section(".highcode")))
#endif
void LDO_Restore(void)
{
    uint32_t tmpreg = 0, t = 0;
    uint16_t high1a = 0, high1b = 0, high2a = 0, high2b = 0;
    uint16_t low1 = 0, low2 = 0;
    
    if((REGPSC & 0xF00000) == (uint32_t)0xF00000)
    {
        tmpreg = EXTEN->EXTEN_CTR;
        tmpreg &= ~(0xF << 8);
        tmpreg |= (0xA << 8);
        EXTEN->EXTEN_CTR = tmpreg;
        if(((*(vu32*)0x4002203C >> 24) & 0xFF) != 0xAB)
        {
            RCC->BDCTLR |= (1 << 15);
            RTC->CTLRL &= (uint16_t)~RTC_FLAG_RSF;
            while((RTC->CTLRL & RTC_FLAG_RSF) == (uint16_t)RESET);

            REGPSC |= (((RTC->PSCRH & 0xF) << 16) | RTC->PSCRL);
            REGALR = (RTC->ALRMH << 16 | RTC->ALRML);
            do{
                high1a = RTC->CNTH;
                high1b = RTC->CNTH;
            }while(high1a != high1b);
            do{
                low1 = RTC->CNTL;
                low2 = RTC->CNTL;
            }while(low1 != low2);
            do{
                high2a = RTC->CNTH;
                high2b = RTC->CNTH;
            }while(high2a != high2b);
            if(high1b != high2b) {
                do{
                    low1 = RTC->CNTL;
                    low2 = RTC->CNTL;
                }while(low1 != low2);
            }
            REGCNT = (((uint32_t)high2b << 16) | low2);

            RTC->CTLRL |= (1 << 4);
            RTC->PSCRH = 0;
            RTC->PSCRL = 1;
            RTC->CNTH = 0;
            RTC->CNTL = 0;
            RTC->ALRMH = 0;
            RTC->ALRML = 1;
            RTC->CTLRL &= (uint16_t) ~(1 << 4);
            while((RTC->CTLRL & RTC_FLAG_RTOFF) == (uint16_t)RESET);
            
            PWR->CTLR &= ~(3 << 0);
            NVIC->SCTLR |= (1 << 2);
            NVIC->SCTLR |= (1 << 5); 
            t = NVIC->SCTLR;
            t &= ~(1 << 5);
            t |= (1 << 3);
            NVIC->SCTLR = t;
            asm volatile ("wfi");
            t = NVIC->SCTLR;
            t &= ~(1 << 5);
            t |= (1 << 3);
            NVIC->SCTLR = t;
            asm volatile ("wfi");
            if(EXTEN->EXTEN_CTR & (1 << 6))
            {
                NVIC->SCTLR |= (1 << 5);
            }
            NVIC->SCTLR &= ~(1 << 2);

            RTC->CTLRL |= (1 << 4);
            RTC->PSCRH = (uint16_t)((REGPSC >> 16) & 0xF);
            RTC->PSCRL = (uint16_t)(REGPSC & 0xFFFF);
            RTC->CNTH = (uint16_t)(REGCNT >> 16);
            RTC->CNTL = (uint16_t)(REGCNT & 0xFFFF);
            RTC->ALRMH = (uint16_t)(REGALR >> 16);
            RTC->ALRML = (uint16_t)(REGALR & 0xFFFF);
            RTC->CTLRL &= (uint16_t) ~(1 << 4);
            while((RTC->CTLRL & RTC_FLAG_RTOFF) == (uint16_t)RESET);  

			if(REGPSC & (1 << 29)) RCC->BDCTLR &= ~(1 << 15);
            if(REGPSC & (1 << 30)) RCC->RSTSCKR &= ~(1 << 0);
            if(REGPSC & (1 << 27)) RCC->APB1PCENR &= ~(1 << 27);
            if(REGPSC & (1 << 26)) PWR->CTLR &= ~(1 << 8);
            if(REGPSC & (1 << 25)) EXTI->EVENR &= ~(1 << 17);
            if(REGPSC & (1 << 24)) EXTI->RTENR &= ~(1 << 17);
        }
        else 
        {
            FLASH->CTLR |= (1 << 23);
        }
        REGPSC &= ~(0xF << 20);
    }
}

/*********************************************************************
 * @fn      Enter_StopLowPower
 *
 * @brief   Enter stop low-power mode.
 *
 * @return  none
 */
#if(Operating_Area_Type == RunIn_NonZeroWaitArea)
__attribute__((section(".highcode")))
#endif
void Enter_StopLowPower(void)
{
    /* The kernel voltage needs to be adjusted before waking up */
    LDO_Deal();

#if (Operating_Area_Type == RunIn_NonZeroWaitArea)

    #if (Enter_Mode == Enter_WFI_Mode)
    {
        uint32_t tmpreg = 0;
        tmpreg = PWR->CTLR;
        tmpreg &= ~(0x3);
        tmpreg |= 0x1;
        PWR->CTLR = tmpreg;

        NVIC->SCTLR |= (1 << 2);   
        NVIC->SCTLR &= ~(1 << 3); 
        asm volatile ("wfi");
        NVIC->SCTLR &= ~(1 << 2);        
    }
    #else
    {
        uint32_t tmpreg = 0, t=0;
        tmpreg = PWR->CTLR;
        tmpreg &= ~(0x3);
        tmpreg |= 0x1;
        PWR->CTLR = tmpreg;
        NVIC->SCTLR |= (1 << 2);
        NVIC->SCTLR |= (1 << 5); 
        t = NVIC->SCTLR;
        t &= ~(1 << 5);
        t |= (1 << 3);
        NVIC->SCTLR = t;
        asm volatile ("wfi");
        t = NVIC->SCTLR;
        t &= ~(1 << 5);
        t |= (1 << 3);
        NVIC->SCTLR = t;
        asm volatile ("wfi");
        if(EXTEN->EXTEN_CTR & (1 << 6))
        {
            NVIC->SCTLR |= (1 << 5);
        }
        NVIC->SCTLR &= ~(1 << 2);
    }
    #endif

#elif (Operating_Area_Type == RunIn_ZeroWaitArea)

    #if (Enter_Mode == Enter_WFI_Mode)
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    #else
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
    #endif

#endif

#if (Enter_Mode == Enter_WFE_Mode)
    /* The kernel voltage must be restored first after waking up */
    LDO_Restore();
    SystemInit();
#endif

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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    Delay_Ms(1000);
    GPIO_ALL_IPD();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    printf("\r\nenter stop mode ...\r\n");

    RTC_Alarm_WakeUp();

#if (STOP_Mode == STOP_On_Mode)
    #if (Enter_Mode == Enter_WFI_Mode)
        PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
    #else
        PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
        SystemInit();
    #endif

#else
    Enter_StopLowPower();
#endif

    Delay_Ms(100);
    printf("\r\n#####################\r\n");
    printf("wake up !!!\r\n");
    while(1)
    {
        Delay_Ms(1000);
        printf("run in main\r\n");
    }
}

#if (Enter_Mode == Enter_WFI_Mode)
void RTCAlarm_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));   
/*********************************************************************
 * @fn      RTCAlarm_IRQHandler
 *
 * @brief   This function handles RTCAlarm Handler.
 *
 * @return  none
 */
 #if(Operating_Area_Type == RunIn_NonZeroWaitArea)
 __attribute__((section(".highcode")))
 #endif
void RTCAlarm_IRQHandler(void)
{
#if(STOP_Mode == STOP_LowPower_Mode)
    /* After waking up, the kernel voltage needs to be restored first */
    LDO_Restore();
#endif
    SystemInit();
    if((EXTI->INTFR & EXTI_Line17))
    {
        /* Clear Flag */
        EXTI->INTFR = EXTI_Line17; 
        RTC->CTLRL &= (uint16_t)~RTC_IT_ALR;
        while((RTC->CTLRL & RTC_FLAG_RTOFF) == (uint16_t)RESET);
    }
}
#endif

