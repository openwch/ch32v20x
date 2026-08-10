/********************************** (C) COPYRIGHT *******************************
 * File Name          : Motor_SPI_USART_MonitorData.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "Motor_SPI_USART_MonitorData.h"

t_sendUsbData sendUsbData = {0,0,0,0,0,0,0};
const u_int8_t usbFrameByteNum[12] = {0, 32, 32, 28, 28, 24, 28, 32, 20, 22, 24, 26};
const u_int8_t needSendLen[12] = {0, USB_NEEDSEND_LEN1, USB_NEEDSEND_LEN2, USB_NEEDSEND_LEN3, USB_NEEDSEND_LEN4, USB_NEEDSEND_LEN5,
        USB_NEEDSEND_LEN6, USB_NEEDSEND_LEN7, USB_NEEDSEND_LEN8, USB_NEEDSEND_LEN9, USB_NEEDSEND_LEN10, USB_NEEDSEND_LEN11};

/******************************************************************************
* Function Name  : getUsbBufLen
* Description    : get Usb Buf Len
* Input          : u_int8_t data
* Output         : None
* Return         : Return head and tail pointer length
******************************************************************************/
u_int8_t getUsbBufLen(void)
{
    u_int8_t reDataLen;
    if(sendUsbData.head >= sendUsbData.end)
    {
        reDataLen = sendUsbData.head - sendUsbData.end;
    }
    else
    {
        reDataLen = (u_int16_t)USB_SENDBUF_LEN - sendUsbData.end + sendUsbData.head;
    }
    return reDataLen;
}

/******************************************************************************
* Function Name  : SPI_Deal_sendMonitorData
* Description    : SPI send Monitor Data
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void SPI_Deal_sendMonitorData(void)
{
    u_int16_t data=0;
    u_int8_t i=0, frameByteNum=0;
    sendUsbData.sendDataLen=7;
//    if(sendUsbData.sendDataLen)
//    {
        if(getUsbBufLen() >= 16)
        {
            GPIO_ResetBits(GPIOB,GPIO_Pin_6);                              
            __asm volatile ("nop");                                         
            while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);     

            SPI1->DATAR = 0x55AA;                                           
            while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);     

            sendUsbData.heartBeat++;
            data=sendUsbData.heartBeat<<8;
            data|=32;
            SPI1->DATAR = data;                                            
            while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);    

            frameByteNum = 4;
//            while(frameByteNum < usbFrameByteNum[sendUsbData.sendDataLen])//{0, 32, 32, 28, 28, 24, 28, 32, 20, 22, 24, 26};
            while(frameByteNum < 32)
            {
                for(i = 0; i < sendUsbData.sendDataLen; i++)
                {
                    SPI1->DATAR=sendUsbData.sendDataBuf[sendUsbData.end+i];
                    while((SPI1->STATR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);    
                    frameByteNum+=2;
                }
                sendUsbData.end += MaxUsbDataLen;
            }

            while( (SPI1->STATR & SPI_I2S_FLAG_BSY) != (uint16_t)RESET);    
            __asm volatile ("nop");                                         
            GPIO_SetBits(GPIOB,GPIO_Pin_6);                                
                                          
        }
  //  }
}

/******************************************************************************
* Function Name  : SPI_FullDuplex_Init
* Description    : SPI Init
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void SPI_FullDuplex_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    SPI_InitTypeDef SPI_InitStructure={0};

    RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOB | RCC_APB2Periph_SPI1|RCC_APB2Periph_AFIO, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );//SCK

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

    GPIO_SetBits(GPIOB,GPIO_Pin_6);    

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;//16λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//72/4=16M

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_Init( SPI1, &SPI_InitStructure );

    GPIO_ResetBits(GPIOB,GPIO_Pin_6); 
    SPI_Cmd( SPI1, ENABLE );           
}

/******************************************************************************
* Function Name  : Usart_Init
* Description    : SPI Init
* Input          : uint32_t Baud 4000000
* Output         : None
* Return         : None
******************************************************************************/
void Usart_Init(uint32_t Baud)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);


    USART_InitStructure.USART_BaudRate = Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx ;

    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

}

/******************************************************************************
* Function Name  : Usart_Deal_sendMonitorData
* Description    : Usart Send
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Usart_Deal_sendMonitorData(void)
{
    u_int8_t i=0, frameByteNum=0;
    sendUsbData.sendDataLen=4;
    if(sendUsbData.sendDataLen)
    {
        if(getUsbBufLen() >= needSendLen[sendUsbData.sendDataLen])
        {
            __asm volatile ("nop");                                        
            while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);     
            USART1->DATAR=0xAA;
            while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);      
            USART1->DATAR=0x55;
            while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);      

            USART1->DATAR=usbFrameByteNum[sendUsbData.sendDataLen];
            while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);      
            sendUsbData.heartBeat++;
            USART1->DATAR=sendUsbData.heartBeat;
            while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);      


            frameByteNum = 4;
            while(frameByteNum < usbFrameByteNum[sendUsbData.sendDataLen])//{0, 32, 32, 28, 28, 24, 28, 32, 20, 22, 24, 26};
            {
                for(i = 0; i < sendUsbData.sendDataLen; i++)
                {
                    USART1->DATAR = (u_int8_t)((sendUsbData.sendDataBuf[sendUsbData.end+i])&0x00FF);
                    frameByteNum++;
                    while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);      
                    USART1->DATAR = (u_int8_t)((sendUsbData.sendDataBuf[sendUsbData.end+i])>>8);
                    frameByteNum++;
                    while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);      
                }
                sendUsbData.end += MaxUsbDataLen;
            }
            while((USART1->STATR & USART_FLAG_TXE) == (uint16_t)RESET);      
            __asm volatile ("nop");                                         
        }
    }
}
