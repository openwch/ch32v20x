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
 *task1 and task2 alternate printing
 */

#include "debug.h"
#include "tos_k.h"

/* Global define */
#define TASK1_STK_SIZE       1024
#define TASK2_STK_SIZE       1024


/* Global Variable */
k_task_t task1;
__aligned(4) uint8_t task1_stk[TASK1_STK_SIZE];
k_task_t task2;
__aligned(4) uint8_t task2_stk[TASK2_STK_SIZE];


/*********************************************************************
 * @fn      task1_entry
 *
 * @brief   task1 program.
 *
 * @param  *arg - Parameters point of task1
 *
 * @return  none
 */
void task1_entry(void *arg)
{
    while (1)
    {
        printf("###I am task1\r\n");
        tos_task_delay(2000);
    }
}

/*********************************************************************
 * @fn      task2_entry
 *
 * @brief   task2 program.
 *
 * @param  *arg - Parameters point of task2
 *
 * @return  none
 */
void task2_entry(void *arg)
{
    while (1)
    {
        printf("***I am task2\r\n");
        tos_task_delay(1000);
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
    SystemCoreClockUpdate();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("Welcome to TencentOS tiny(%s)\r\n", TOS_VERSION);
    tos_knl_init();
    tos_task_create(&task1, "task1", task1_entry, NULL, 3, task1_stk, TASK1_STK_SIZE, 0); // Create task1
    tos_task_create(&task2, "task2", task2_entry, NULL, 3, task2_stk, TASK2_STK_SIZE, 0);// Create task2
    tos_knl_start();

    printf("should not run at here!\r\n");

    while(1)
    {
        asm("nop");
    }
}
