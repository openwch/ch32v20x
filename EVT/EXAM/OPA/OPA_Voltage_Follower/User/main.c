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
 *OPA1 is used as a voltage follower output, the external voltage is output to OPA1,
 *and the ADC samples OPA1
 *OPA1_CHP1 -- PB0
 *OPA1_CHN1 -- PA6
 *OPA1_OUT_IO_OUT0 -- PA3
 *
 *    In this example, PA6 and PA3 are short-circuited, and the external voltage is input
 *    from PB0. If the negative feedback is connected to a resistor, it can also form
 *    an operational amplifier.
 */

#include "debug.h"

/* Global define */


/* Global Variable */

/*********************************************************************
 * @fn      OPA1_Init
 *
 * @brief   Initializes OPA1 collection.
 *
 * @return  none
 */
void OPA1_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    OPA_InitTypeDef  OPA_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    OPA_InitStructure.OPA_NUM = OPA1;
    OPA_InitStructure.PSEL = CHP1;
    OPA_InitStructure.NSEL = CHN1;
    OPA_InitStructure.Mode = OUT_IO_OUT0;
    OPA_Init( &OPA_InitStructure );
    OPA_Cmd( OPA1, ENABLE );

}

/*********************************************************************
 * @fn      ADC_Channel3_Init
 *
 * @brief   Initializes ADC Channel3 collection.
 *
 * @return  none
 */
void ADC_Channel3_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig( RCC_PCLK2_Div8 );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init( ADC1, &ADC_InitStructure );

    ADC_Cmd( ADC1, ENABLE );
    ADC1->CTLR1 |= ( 1 << 26 );  //buffer en
}

/*********************************************************************
 * @fn      Get_ADC_Val
 *
 * @brief   Returns ADCx conversion result data.
 *
 * @param   ch - ADC channel.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *            ADC_Channel_10 - ADC Channel10 selected.
 *            ADC_Channel_11 - ADC Channel11 selected.
 *            ADC_Channel_12 - ADC Channel12 selected.
 *            ADC_Channel_13 - ADC Channel13 selected.
 *            ADC_Channel_14 - ADC Channel14 selected.
 *            ADC_Channel_15 - ADC Channel15 selected.
 *            ADC_Channel_16 - ADC Channel16 selected.
 *            ADC_Channel_17 - ADC Channel17 selected.
 *
 * @return  none
 */
u16 Get_ADC_Val( u8 ch )
{
    u16 val;

    ADC_RegularChannelConfig( ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd( ADC1, ENABLE );

    while( !ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC ) );

    val = ADC_GetConversionValue( ADC1 );

    return val;
}

/*********************************************************************
 * @fn      Get_ADC_Average
 *
 * @brief   Returns ADCx conversion result average data.
 *
 * @param   ch - ADC channel.
 *            ADC_Channel_0 - ADC Channel0 selected.
 *            ADC_Channel_1 - ADC Channel1 selected.
 *            ADC_Channel_2 - ADC Channel2 selected.
 *            ADC_Channel_3 - ADC Channel3 selected.
 *            ADC_Channel_4 - ADC Channel4 selected.
 *            ADC_Channel_5 - ADC Channel5 selected.
 *            ADC_Channel_6 - ADC Channel6 selected.
 *            ADC_Channel_7 - ADC Channel7 selected.
 *            ADC_Channel_8 - ADC Channel8 selected.
 *            ADC_Channel_9 - ADC Channel9 selected.
 *            ADC_Channel_10 - ADC Channel10 selected.
 *            ADC_Channel_11 - ADC Channel11 selected.
 *            ADC_Channel_12 - ADC Channel12 selected.
 *            ADC_Channel_13 - ADC Channel13 selected.
 *            ADC_Channel_14 - ADC Channel14 selected.
 *            ADC_Channel_15 - ADC Channel15 selected.
 *            ADC_Channel_16 - ADC Channel16 selected.
 *            ADC_Channel_17 - ADC Channel17 selected.
 *
 * @return  val - The Data conversion value.
 */
u16 Get_ADC_Average( u8 ch, u8 times )
{
    u32 temp_val = 0;
    u8 t;
    u16 val;

    for( t = 0; t < times; t++ ){
        temp_val += Get_ADC_Val( ch );
        Delay_Ms( 5 );
    }

    val = temp_val / times;

    return val;
}


/*********************************************************************
 * @fn      main
 *
 * @brief   ADC_Channel0_Init
 *
 * @return  none
 */
int main( void )
{
    u16 ADC_val, i;
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init( 115200 );
    printf( "SystemClk:%d\r\n", SystemCoreClock );
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf( "OPA Test\r\n" );
    OPA1_Init();
    ADC_Channel3_Init();

    while( 1 )
    {
        for( i = 0; i < 6; i++ ){
            ADC_val = Get_ADC_Average( ADC_Channel_3, 10 );
            printf( "OPA_OUT:%04d\r\n", ADC_val );
            Delay_Ms( 500 );
        }
    }
}
