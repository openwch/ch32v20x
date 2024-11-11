/********************************** (C) COPYRIGHT *******************************
 * File Name          : system_ch32v20x.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/06/26
 * Description        : CH32V20x Device Peripheral Access Layer System Source File.
 *                      For HSE = 32Mhz (CH32V208x/CH32V203RBT6)
 *                      For HSE = 8Mhz (other CH32V203x)
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32v20x.h" 

/* 
* Uncomment the line corresponding to the desired System clock (SYSCLK) frequency (after 
*/

//#define SYSCLK_FREQ_24MHz  24000000
//#define SYSCLK_FREQ_48MHz  48000000
#define SYSCLK_FREQ_72MHz  72000000

/* Clock Definitions */
#ifdef SYSCLK_FREQ_24MHz
  uint32_t SystemCoreClock         = SYSCLK_FREQ_24MHz;        /* System Clock Frequency (Core Clock) */
#elif defined SYSCLK_FREQ_48MHz
  uint32_t SystemCoreClock         = SYSCLK_FREQ_48MHz;        /* System Clock Frequency (Core Clock) */
#elif defined SYSCLK_FREQ_72MHz
  uint32_t SystemCoreClock         = SYSCLK_FREQ_72MHz;        /* System Clock Frequency (Core Clock) */
#endif

__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/* system_private_function_proto_types */
static void SetSysClock(void);

#ifdef SYSCLK_FREQ_24MHz
  static void SetSysClockTo24(void);
#elif defined SYSCLK_FREQ_48MHz
  static void SetSysClockTo48(void);
#elif defined SYSCLK_FREQ_72MHz
  static void SetSysClockTo72(void);
#endif


/*********************************************************************
 * @fn      SystemInit
 *
 * @brief   Setup the microcontroller system Initialize the Embedded Flash Interface,
 *        the PLL and update the SystemCoreClock variable.
 *
 * @return  none
 */
void SystemInit (void)
{
  RCC->CTLR |= (uint32_t)0x00000001;
  RCC->CFGR0 &= (uint32_t)0xF0FF0000;
  RCC->CTLR &= (uint32_t)0xFEF6FFFF;
  RCC->CTLR &= (uint32_t)0xFFFBFFFF;
  RCC->CFGR0 &= (uint32_t)0xFF00FFFF;
  RCC->INTR = 0x009F0000;    
  SetSysClock();
}

/*********************************************************************
 * @fn      SystemCoreClockUpdate
 *
 * @brief   Update SystemCoreClock variable according to Clock Register Values.
 *
 * @return  none
 */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, Pll_6_5 = 0;

  tmp = RCC->CFGR0 & RCC_SWS;

  switch (tmp)
  {
    case 0x00:
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:
      pllmull = RCC->CFGR0 & RCC_PLLMULL;
      pllsource = RCC->CFGR0 & RCC_PLLSRC;
      pllmull = ( pllmull >> 18) + 2;

      if(pllmull == 17) pllmull = 18;

      if (pllsource == 0x00)
      {
          if(EXTEN->EXTEN_CTR & EXTEN_PLL_HSI_PRE){
              SystemCoreClock = HSI_VALUE * pllmull;
          }
          else{
              SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
          }
      }
      else
      {
#if defined (CH32V20x_D8W) || defined (CH32V20x_D8)
        if(((RCC->CFGR0 & (3<<22)) == (3<<22)) && (RCC_USB5PRE_JUDGE()== SET))
        {
          SystemCoreClock = ((HSE_VALUE>>1)) * pllmull;
        }
        else
#endif
        if ((RCC->CFGR0 & RCC_PLLXTPRE) != (uint32_t)RESET)
        {
#if defined (CH32V20x_D8) || defined (CH32V20x_D8W)
          SystemCoreClock = ((HSE_VALUE>>2) >> 1) * pllmull;
#else
          SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
#endif
        }
        else
        {
#if defined (CH32V20x_D8) || defined (CH32V20x_D8W)
            SystemCoreClock = (HSE_VALUE>>2) * pllmull;
#else
          SystemCoreClock = HSE_VALUE * pllmull;
#endif
        }
      }

      if(Pll_6_5 == 1) SystemCoreClock = (SystemCoreClock / 2);

      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }

  tmp = AHBPrescTable[((RCC->CFGR0 & RCC_HPRE) >> 4)];
  SystemCoreClock >>= tmp;
}


/*********************************************************************
 * @fn      SetSysClock
 *
 * @brief   Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClock(void)
{
  //GPIO_IPD_Unused();
#ifdef SYSCLK_FREQ_24MHz
  SetSysClockTo24();
#elif defined SYSCLK_FREQ_48MHz
  SetSysClockTo48();
#elif defined SYSCLK_FREQ_72MHz
  SetSysClockTo72();
#endif
}


#ifdef SYSCLK_FREQ_24MHz

/*********************************************************************
 * @fn      SetSysClockTo24
 *
 * @brief   Sets System clock frequency to 24MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo24(void)
{

#if (PLL_Source == HSI)
  EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;

#endif

 
    

    

    /* HCLK = SYSCLK */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;   
    /* PCLK2 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1; 
    /* PCLK1 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV1;

    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

#if (PLL_Source == HSI)
    /*  PLL configuration: PLLCLK = HSI * 3 = 24 MHz */
    RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL3);

#else
  /*  PLL configuration: PLLCLK = HSI/2 * 6 = 24 MHz */
    RCC->CFGR0 |= (uint32_t)(RRCC_PLLSRC_HSI_Div2 | RCC_PLLMULL6);

#endif

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


#elif defined SYSCLK_FREQ_48MHz

/*********************************************************************
 * @fn      SetSysClockTo48
 *
 * @brief   Sets System clock frequency to 48MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo48(void)
{

#if (PLL_Source == HSI)
  EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;

#endif
 
    

    

    /* HCLK = SYSCLK */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;    
    /* PCLK2 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;  
    /* PCLK1 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;

    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

#if (PLL_Source == HSI)
    /*  PLL configuration: PLLCLK = HSI * 6 = 48 MHz */
    RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL6);

#else
    /*  PLL configuration: PLLCLK = HSI/2 * 12 = 48 MHz */
  RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL12);
  
#endif
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

#elif defined SYSCLK_FREQ_72MHz

/*********************************************************************
 * @fn      SetSysClockTo72
 *
 * @brief   Sets System clock frequency to 72MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo72(void)
{

#if (PLL_Source == HSI)
  EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;

#endif
 
    

   

    /* HCLK = SYSCLK */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1; 
    /* PCLK2 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1; 
    /* PCLK1 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;


    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

#if (PLL_Source == HSI)
    /*  PLL configuration: PLLCLK = HSI * 9 = 72 MHz */
    RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL9);

#else
    /*  PLL configuration: PLLCLK = HSI/2 * 18 = 72 MHz */
  RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL18);

#endif

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
#endif

