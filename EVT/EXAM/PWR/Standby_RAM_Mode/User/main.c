/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.2
 * Date               : 2026/10/15
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 * when LV is not enabled in standby mode, RAM 2k and 30K data holding routines:
 * This routine demonstrates writing data at the specified location of 2K RAM and 30K RAM(Applicable to
 * CH32V20x_D8 and CH32V20x_D8W,CH32V20x_D6 has a 20K RAM data holding option),then WFI enters STANDBY
 * sleep mode, through the PA0 (wakeup) pin Input high level to exit standby mode, print RAM data after
 * waking up, and test whether RAM is maintained data.
 *
 */

#include "debug.h"

/* Global define */
#define RAM_BASE            0x20000000U
#define RAM2K_BASE          (RAM_BASE) /*Applicable to CH32V20x_D8 CH32V20x_D8W,CH32V20x_D6 has a 20K RAM data holding option */ 
#define RAM2K_SIZE          (2U  * 1024U)
#define RAM30K_BASE         (RAM_BASE + 2U * 1024U) /*Applicable to CH32V20x_D8 CH32V20x_D8W,CH32V20x_D6 has a 20K RAM data holding option */   
#define RAM30K_SIZE         (30U * 1024U)

#define RAM30K_TEST_ADDR    (RAM_BASE + 10U * 1024U)      
#define RAM30K_TEST_WORDS   1024U                          
#define RAM2K_TEST_ADDR     (RAM_BASE + 1U * 1024U + 512U)  
#define RAM2K_TEST_WORDS    10U  

#define RAM_TEST_PATTERN    0x3333AAAAU

#define RAM_TEST_OK         0U
#define RAM_TEST_FAIL       1U
/* Global Variable */

/*********************************************************************
 * @fn      TestDataWrite
 *
 * @brief   Write data to certain address of 2K RAM and 30K RAM.
 *
 * @return  none
 */
uint32_t TestDataWrite(void)
{
    uint32_t i=0;
    uint32_t myAddr = RAM30K_TEST_ADDR;
    uint32_t myAddr2 = RAM2K_TEST_ADDR;
    
    for(i = 0; i < RAM30K_TEST_WORDS; i++)//Write 30KRAM
    {
        *(uint32_t volatile *)(myAddr+(i<<2)) = RAM_TEST_PATTERN;
    }
    for(i = 0; i < RAM30K_TEST_WORDS; i++)//Check 30KRAM
    {
        if((*(uint32_t volatile *)(myAddr+(i<<2))) != RAM_TEST_PATTERN)
        {
            printf("30K RAM write error at 0x%08x\r\n", (unsigned)(myAddr + (i << 2)));
            return RAM_TEST_FAIL;
        }
    }
    printf("Write 30K RAM Successfully!\r\n");

    for(i = 0; i < RAM2K_TEST_WORDS; i++)//Write 2KRAM
    {
        *(uint32_t volatile *)(myAddr2+(i<<2)) = RAM_TEST_PATTERN;
    }
    for(i = 0; i < RAM2K_TEST_WORDS; i++)//Check 2KRAM
    {
        if((*(uint32_t volatile *)(myAddr2+(i<<2))) != RAM_TEST_PATTERN)
        {
            printf("2K RAM write error at 0x%08x\r\n", (unsigned)(myAddr2 + (i << 2)));
            return RAM_TEST_FAIL;
        }
    }
    printf("Write 2K RAM Successfully!\r\n");
    return RAM_TEST_OK;
}

/*********************************************************************
 * @fn      TestDataRead
 *
 * @brief   Print the data of certain address of 2K RAM and 30K RAM.
 *
 * @return  none
 */
void TestDataRead(void)
{
    uint32_t i = 0;

    printf("2K RAM:\r\n");
    for(i = 0; i < RAM2K_TEST_WORDS; i++){
        printf("0x%08x=0x%08x\r\n", (unsigned)(RAM2K_TEST_ADDR + (i << 2)),
               (unsigned)*(uint32_t volatile *)(RAM2K_TEST_ADDR + (i << 2)));
    }

    printf("30K RAM:\r\n");
    printf("0x%08x=0x%08x\r\n", (unsigned)(RAM30K_TEST_ADDR + (i << 2)),
               (unsigned)*(uint32_t volatile *)(RAM30K_TEST_ADDR + (i << 2)));
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
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    Delay_Ms(1000);
    /* To reduce power consumption, unused GPIOs need to be set as pull-down inputs */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
        RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Delay 1.5s to avoid entering low power mode immediately after reset*/
    Delay_Ms(1500);

    TestDataRead();
    printf("Standby RAM Mode Test\r\n");
    TestDataWrite();
    PWR_WakeUpPinCmd(ENABLE);
    PWR_EnterSTANDBYMode_RAM();
    while(1);
}

