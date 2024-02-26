/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/29
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
CH223 chip application routine

*/

#include "debug.h"
#include"CH223.h"

u8 ndo = 0;//Number of NDOs in SrcCap
u8 IT_flag = 0;
u8 rst_IT = 0, cc_IT = 0, srccap_IT = 0, ps_IT = 0;//Interrupt flag
s16 Calibrattion_Val = 0;
u16 TxBuf[1024];

/*********************************************************************
 * @fn      ADC_Function_Init
 *
 * @brief   Initializes ADC collection.
 *          PB0-->ADC_IN
 *
 * @return  none
 */
void ADC_Function_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_BufferCmd(ADC1, DISABLE); //disable buffer
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    Calibrattion_Val = Get_CalibrationValue(ADC1);

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
u16 Get_ADC_Val(u8 ch)
{
    u16 val;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    val = ADC_GetConversionValue(ADC1);

    return val;
}

/*********************************************************************
 * @fn      DMA_Tx_Init
 *
 * @brief   Initializes the DMAy Channelx configuration.
 *
 * @param   DMA_CHx - x can be 1 to 7.
 *          ppadr - Peripheral base address.
 *          memadr - Memory base address.
 *          bufsize - DMA channel buffer size.
 *
 * @return  none
 */
void DMA_Tx_Init(DMA_Channel_TypeDef *DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ppadr;
    DMA_InitStructure.DMA_MemoryBaseAddr = memadr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = bufsize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);
}
/*********************************************************************
 * @fn      Get_ConversionVal
 *
 * @brief   Get Conversion Value.
 *
 * @param   val - Sampling value
 *
 * @return  val+Calibrattion_Val - Conversion Value.
 */
u16 Get_ConversionVal(s16 val)
{
    if((val + Calibrattion_Val) < 0)
        return 0;
    if((Calibrattion_Val + val) > 4095||val==4095)
        return 4095;
    return (val + Calibrattion_Val);
}

/*********************************************************************
 * @fn      EXTI0_INT_INIT
 *
 * @brief   Initialize external trigger interrupt.
 *
 *  PB13-->INT
 *
 * @return  none
 */
/*********************************************************************
 * @fn      EXTI0_INT_INIT
 *
 * @brief   Initializes EXTI0 collection.
 *
 * @return  none
 */
void EXTI0_INT_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIOB ----> EXTI_Line0 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************
 * @fn      Adc_test
 *
 * @brief   ADC measure VBUS output voltage.
 *
 *
 * @return  none
 */
void Adc_test(){
    u16 i = 0;
    float sum=0;
    DMA_Tx_Init(DMA1_Channel1, (u32)&ADC1->RDATAR, (u32)TxBuf, 1024);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    Delay_Ms(50);
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    for(i = 0; i < 1024; i++){
         sum+=Get_ConversionVal(TxBuf[i]);
    }
    printf("ADC_Value=%0.1fV\r\n",sum/1024/4095*3.3*6.1);
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
    u16 n = 0;
    u8 tempV = 0, tempI = 0, tempV0 = 0, tempI0 = 0, tempVmax = 0;
    u8 res = 0, ndo_select = 0, STATE_flag = 0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    Delay_Init();
    USART_Printf_Init( 115200 );
    CH223_Init();
    ADC_Function_Init();
    EXTI0_INT_INIT();
    printf("PD_CH223 Demo£¡\r\n");
    printf("\r\n");
    IT_flag = CH223_ReadOneByte(0xB4);
    if (IT_flag==0x0c)
    {
        printf("power-on reset£¡£¡£¡\r\n");
        printf("\r\n");
        Delay_Ms(10);
        printf("CC Connecting...\r\n");
        printf("\r\n");
    }
    while(1)
    {
        Delay_Ms(1);
        while(ps_IT)      //PS_RDY -->The power supply reaches the required working state
        {
            CH223_WriteOneByte(0xB6, 0x19);                    //The initial voltage is set to 9V£º0x2D
            res = CH223_ReadOneByte(0xB6);
            printf("The initial voltage is set to£º%dV\r\n",res*200/1000);
            printf("\r\n");
            Delay_Ms(300);
            if (rst_IT == 1)
            {
                printf("power-on reset£¡£¡£¡\r\n");
                printf("\r\n");
            }
            if (cc_IT == 1)
            {
                printf("CC Connecting...\r\n");
                printf("\r\n");
            }
            STATE_flag = CH223_ReadOneByte(0xB5);               //Read IIC status register
            ndo = STATE_flag & 0x07;                            //NDO_Number
            printf("Number of NDOs in SRCCAP:%d\r\n", ndo);
            printf("\r\n");
            CH223_Read(0x84, Buffer, ndo);                      //SRCCAP message
            Buffer_Change(Buffer, ndo);
            if (srccap_IT == 1)        //Display SRCCAP message
            {
                printf("Received SrcCap,NDO data is£º\n");
                printf("\r\n");
                for(n = 0; n < ndo; n++)
                {
                    if((Buffer[n] & 0xC0000000) == 0)
                    {
                        tempV = BufferV[n] % 10;
                        tempV0 = BufferV[n] / 10;
                        tempI = BufferI[n] % 10;
                        tempI0 = BufferI[n] / 10;
                        printf("%d.%dV,%d.%dA\t", tempV0, tempV, tempI0, tempI);
                    }
                    else
                    {
                        tempV = BufferV[n] % 10;
                        tempV0 = BufferV[n] / 10;
                        tempVmax = BufferVPPS[n] % 10;
                        BufferVPPS[n] /= 10;
                        tempI = BufferI[n] % 10;
                        tempI0 = BufferI[n] / 10;
                        printf("%d.%dV/%d.%dV,%d.%dA\t", tempV0, tempV, BufferVPPS[n], tempVmax, tempI0, tempI);
                    }
                }
                printf("\r\n");
            }
            ndo_select = (STATE_flag >> 3) & 0x07;
            printf("ndo_select:%d\r\n", ndo_select);
            printf("Waiting PS_RDY...\r\n");
            Delay_Ms(500);
            if (ps_IT == 1)
            {
                printf("Received PS_RDY\r\n");
                Current_VI(ndo_select - 1);
            }
            //Set VBUS voltage
            printf("\r\n");
            Delay_Ms(10);
            printf("Set voltage output 5V-->0x19\r\n");
            User_Set_VI(0x19); //0x19---5V
            Adc_test();
            Delay_Ms(50);
            printf("\r\n");
            Delay_Ms(10);
            printf("Set voltage output 9V-->0x2d\r\n");
            User_Set_VI(0x2d);   //0x2d---9V
            Adc_test();
            Delay_Ms(50);
            printf("\r\n");
            Delay_Ms(10);
            printf("Set voltage output 12V-->0x3c\r\n");
            User_Set_VI(0x3c);   //0x3c---12V
            Adc_test();
            Delay_Ms(50);
            printf("\r\n");
            Delay_Ms(10);
            printf("Set voltage output 15V-->0x4b\r\n");
            User_Set_VI(0x4b);   //0x4b---15V
            Adc_test();
            Delay_Ms(50);
            printf("\r\n");
            Delay_Ms(10);
            printf("Set voltage output 20V-->0x64\r\n");
            User_Set_VI(0x64);   //0x64---20V
            Adc_test();
            Delay_Ms(50);
            ps_IT = 0;
           }
       }

}

void EXTI15_10_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        Delay_Us(500);
        if(EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            IT_flag = CH223_ReadOneByte(0xB4);
            if ((IT_flag&0xF))
            {
                if (IT_flag&0x7)
                {
                    if(IT_flag&0x3)
                    {
                        if (IT_flag&0x1)
                        {
                            ps_IT = 1;
                        }
                        else
                        {
                            srccap_IT = 1;
                        }
                    }
                    else
                    {
                        rst_IT = 1;
                    }
                }else
                {
                    cc_IT = 1;
                }
            }
            EXTI_ClearITPendingBit(EXTI_Line13);     /* Clear Flag */
        }
    }
}
