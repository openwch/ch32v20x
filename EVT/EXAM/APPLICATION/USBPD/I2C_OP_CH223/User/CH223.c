/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH223.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/08/15
 * Description        : CH223芯片操作相关文件
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/******************************************************************************/
#include "debug.h"
#include "CH223.h"
#include "string.h"

u32 Buffer[7] = {0};//NDO raw data in SRCCAP
u16 BufferV[SIZE] = {0};//Analyzing voltage data in SRCCAP
u16 BufferI[SIZE] = {0};//Analyzing Current Data in SRCCAP
u16 BufferVPPS[SIZE] = {0};//Parsing PPS data in SRCCAP

/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 *  PB11------SDA
 *  PB10------SCL
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTSturcture={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );
//    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C2, &I2C_InitTSturcture );

    I2C_Cmd( I2C2, ENABLE );
}
/*********************************************************************
 * @fn      CH223_Init
 *
 * @brief   Initializes CH223.
 *
 * @return  none
 */
void CH223_Init()
{
    IIC_Init(100000, 0XA0);
}

/*********************************************************************
 * @fn      CH223_ReadOneByte
 *
 * @brief   Read one data from CH223.
 *
 * @param   ReadAddr - Read frist address.
 *
 * @return  temp - Read data.
 */
u8 CH223_ReadOneByte(u16 ReadAddr)
{
    u8 temp = 0;
    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET )
    {

    }
    I2C_GenerateSTART( I2C2, ENABLE );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );

    I2C_Send7bitAddress( I2C2, 0X44, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    #if (Address_Lenth  == Address_8bit)
        I2C_SendData( I2C2, (u8)(ReadAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    #elif (Address_Lenth  == Address_16bit)
    I2C_SendData( I2C2, (u8)(ReadAddr>>8) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData( I2C2, (u8)(ReadAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    #endif

    I2C_GenerateSTART( I2C2, ENABLE );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C2, 0X45, I2C_Direction_Receiver );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_RXNE ) ==  RESET )
    I2C_AcknowledgeConfig( I2C2, DISABLE );

    temp = I2C_ReceiveData( I2C2 );
    I2C_GenerateSTOP( I2C2, ENABLE );

    return temp;
}

/*********************************************************************
 * @fn      CH223_WriteOneByte
 *
 * @brief   Write one data to CH223.
 *
 * @param   WriteAddr - Write frist address.
 *
 * @return  DataToWrite - Write data.
 */
void CH223_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C2, ENABLE );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C2, 0X45, I2C_Direction_Transmitter );

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    #if (Address_Lenth  == Address_8bit)
    I2C_SendData( I2C2, (u8)(WriteAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    #elif (Address_Lenth  == Address_16bit)
        I2C_SendData( I2C2, (u8)(WriteAddr>>8) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    I2C_SendData( I2C2, (u8)(WriteAddr&0x00FF) );
    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    #endif

    if( I2C_GetFlagStatus( I2C2, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2C2, DataToWrite );
    }

    while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
    I2C_GenerateSTOP( I2C2, ENABLE );
}

/*********************************************************************
 * @fn      CH223_Read
 *
 * @brief   Read multiple data from CH223.
 *
 * @param   ReadAddr - Read frist address.
 *          pBuffer - Read data.
 *          NumToRead - Data number.
 *
 * @return  none
 */
void CH223_Read(u16 ReadAddr, u32 *pBuffer, u16 NumToRead)
{
    u8 t = 0;
    u32 temp[4] = {0};
    u32 temp1 = 0;

    while(NumToRead)
    {
        temp[4] = 0;
        for (t = 0; t < 4; t++)
        {
            temp[t] = CH223_ReadOneByte(ReadAddr++);
        }
        temp1 = (temp[0] + (temp[1] << 8) + (temp[2] << 16) + (temp[3] << 24));
        *pBuffer++=temp1;
        NumToRead--;
    }
}
/*********************************************************************
 * @fn      CH223_Write
 *
 * @brief   Write multiple data to CH223.
 *
 * @param   WriteAddr - Write frist address.
 *          pBuffer - Write data.
 *          NumToWrite - Data number.
 *
 * @return  none
 */
void CH223_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    while(NumToWrite--)
    {
        CH223_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
        Delay_Ms(2);
    }
}


/*********************************************************************
 * @fn      Buffer_Change
 *
 * @brief   Analysis of SRCCAP message
 *
 * @param   buffer[] - NDO data.
 *                 n - NDO_Number.
 *
 * @return  none
*/
void Buffer_Change(u32 buffer[], int n)
{
    int i = 0;
    u32 tempV = 0, tempI = 0, tempVmax = 0;
    for(i = 0; i < n; i++)
    {
        tempV = buffer[i];
        tempI = buffer[i];
        if((tempV & 0xC0000000) == 0)
        {
            tempV >>= 10;
            tempV &= 0x03FF;
            BufferV[i] = tempV * 0.5;

            tempI &= 0x03FF;
            BufferI[i] = tempI * 0.1;
        }
        else {
            tempV >>= 8;
            tempVmax = tempV >> 9;
            tempV &= 0x00FF;
            BufferV[i] = tempV * 1;
            tempVmax &= 0x00FF;
            BufferVPPS[i] = tempVmax * 1;

            tempI &= 0x00FF;
            BufferI[i] = tempI * 0.5;
        }
    }
}
/*********************************************************************
 * @fn      Current_VI
 *
 * @brief   Read and display the current output voltage value
 *
 * @param   ndo_select - NDO_Select.
 *
 *
 * @return  none
*/
void Current_VI(u8 ndo_select)
{
    u8 tempV = 0, tempI = 0;
    u8 current[2];
    tempV = BufferV[ndo_select] % 10;
    current[0] = BufferV[ndo_select] / 10;

    tempI = BufferI[ndo_select] % 10;
    current[1] = BufferI[ndo_select] / 10;

    printf("The current regulating voltage is：%d.%dV,%d.%dA\t\r\n", current[0], tempV, current[1], tempI);
}
/*********************************************************************
 * @fn      User_Set_VI
 *
 * @brief   Set VBUS voltage
 *
 * @param   TargetData - Target voltage value.
 *             VBUS voltage=TargetData*200mv
 *
 * @return  none
*/

void User_Set_VI(u8 TargetData)
{
    u8 ndo_select = 0, state_flag = 0;
    CH223_WriteOneByte(0xB6, TargetData);
    Delay_Ms(5000);
    state_flag = CH223_ReadOneByte(0xB5);
    ndo_select = (state_flag >> 3) & 0x07;
    printf("nod_select_user-->%d\r\n",ndo_select);
    Current_VI(ndo_select - 1);
}
