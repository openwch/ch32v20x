/********************************** (C) COPYRIGHT *******************************
 * File Name          : MB_Operation_Flash.c
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
Flash_Operation_Type  Flash_M;

/******************************************************************************
* Function Name  : SysclkFreq_Change144_48
* Description    : Switch system from 144M to 48M
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void SysclkFreq_Change144_48(void)
{
        RCC->CFGR0&=~(0x3<<0);

        RCC->CFGR0&=~(0x3<<2);
        RCC->CFGR0|=(RCC_SW_HSI);

        RCC->CTLR &=~ RCC_PLLON;
        EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;

        /* HCLK = SYSCLK */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
        /* PCLK2 = HCLK */
        RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
        /* PCLK1 = HCLK */
        RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;

        /*  PLL configuration: PLLCLK = HSI * 6 = 48 MHz */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

         RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL6);

        /* Enable PLL */
        RCC->CTLR |= RCC_PLLON;
        /* Wait till PLL is ready */
        while((RCC->CTLR & RCC_PLLRDY) == 0)
        {
        }
        /* Select PLL as system clock source */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
        RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        {
        }
}

/******************************************************************************
* Function Name  : SysclkFreq_Change144_48
* Description    : Switch system from 48M to 144M
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void SysclkFreq_Change48_144(void)
{

        RCC->CFGR0&=~(0x3<<0);

        RCC->CFGR0&=~(0x3<<2);
        RCC->CFGR0|=(RCC_SW_HSI);

        RCC->CTLR &=~ RCC_PLLON;
        EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;

        /* HCLK = SYSCLK */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
        /* PCLK2 = HCLK */
        RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
        /* PCLK1 = HCLK */
        RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;

        /*  PLL configuration: PLLCLK = HSI * 18 = 144 MHz */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

        RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL18);

        /* Enable PLL */
        RCC->CTLR |= RCC_PLLON;
        /* Wait till PLL is ready */
        while((RCC->CTLR & RCC_PLLRDY) == 0)
        {
        }
        /* Select PLL as system clock source */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
        RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        {
        }
}

/******************************************************************************
* Function Name  : Flash_Operation_Init
* Description    : Flash Init , Read the Flash value of the 4K Flash after erasing 64K
* Input          : operate Flash struct point
* Output         : None
* Return         : None
******************************************************************************/
void Flash_Operation_Init(Flash_Operation_Type *FlashM)
{
    uint32_t Address = 0x0;

    FlashM->MemAddress=PAGE_WRITE_START_ADDR;
    FlashM->FlashExcep=0;
    FlashM->FLASHStatus=FLASH_COMPLETE;
    FlashM->MemoryProgramStatus=PASSED;

    SysclkFreq_Change144_48();

    FLASH_Unlock();

    Address = PAGE_WRITE_START_ADDR;

    while((Address < (PAGE_WRITE_HALF_END_ADDR-2)) && (FlashM->MemoryProgramStatus != FAILED))
    {
        FlashM->FlashData[0]=(*(__IO uint16_t*) Address);

        if( FlashM->FlashData[0]== 0xe339)
        {
            FlashM->MemoryProgramStatus=FAILED;
            if(Address>PAGE_WRITE_START_ADDR)
            {
                FlashM->FlashData[0]=(*(__IO uint16_t*) (Address-2));
                FlashM->FlashData[1]=(*(__IO uint16_t*) (Address-2+2048));
              
            }
        }
        else
            Address += 2;
    }

    FlashM->FLASHStatus= FLASH_ErasePage(PAGE_WRITE_START_ADDR);
        if(FlashM->FLASHStatus != FLASH_COMPLETE)
        {
            FlashM->FlashExcep=1;
        }

    SysclkFreq_Change48_144();

    FLASH_Lock();//
}

/******************************************************************************
* Function Name  : Flash_ProgramHalfWord
* Description    : Flash program Half word
* Input          : operate Flash struct point/ address/ data
* Output         : None
* Return         : None
******************************************************************************/
void Flash_ProgramHalfWord(Flash_Operation_Type *FlashM ,uint32_t Address,uint16_t Data )
{
    FLASH_Unlock();

    FlashM->FLASHStatus = FLASH_ProgramHalfWord(Address, Data);

    if(FlashM->FLASHStatus != FLASH_COMPLETE)
    {
        FlashM->FlashExcep = 1;
    }

    FLASH_Lock();
}
