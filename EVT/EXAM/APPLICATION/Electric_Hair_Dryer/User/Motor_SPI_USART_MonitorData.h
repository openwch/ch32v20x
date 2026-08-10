#ifndef __MOTOR_SPI_USART_MONITORDATA_H_
#define __MOTOR_SPI_USART_MONITORDATA_H_
/* Includes -----------------------------------------------------------------*/
#include "debug.h"
/* Exported constants -------------------------------------------------------*/
#define MaxUsbDataLen       8
#define USB_SENDBUF_LEN       (MaxUsbDataLen<<5)
#define USB_NEEDSEND_LEN1       (MaxUsbDataLen*14)
#define USB_NEEDSEND_LEN2       (MaxUsbDataLen*7)
#define USB_NEEDSEND_LEN3       (MaxUsbDataLen*4)
#define USB_NEEDSEND_LEN4       (MaxUsbDataLen*3)
#define USB_NEEDSEND_LEN5       (MaxUsbDataLen*2)
#define USB_NEEDSEND_LEN6       (MaxUsbDataLen*2)
#define USB_NEEDSEND_LEN7       (MaxUsbDataLen*2)
#define USB_NEEDSEND_LEN8       (MaxUsbDataLen)
#define USB_NEEDSEND_LEN9       (MaxUsbDataLen)
#define USB_NEEDSEND_LEN10       (MaxUsbDataLen)
#define USB_NEEDSEND_LEN11       (MaxUsbDataLen)

/* Exported types -----------------------------------------------------------*/
typedef struct
{
    u_int8_t heartBeat;        //---心跳包---
    u_int8_t delayNum;         //---延迟传输---
    u_int8_t volatile sendDataLen;       //----数据长度---
    //UINT8 frameByteNum;         //---发送包字节序列---
    u_int8_t curWriteFrame;         //----当前写数据帧---
    u_int8_t recDataLen;          //----接收上位机数据长度---
    u_int8_t end;
    u_int8_t volatile head;
    u_int8_t recDataBuf[16];          //---接收数据缓冲区---
    u_int8_t sendDataType[MaxUsbDataLen];    //---发送数据类型---
    u_int16_t sendDataBuf[USB_SENDBUF_LEN];    //---发送数据缓冲区---
}t_sendUsbData;

/* Exported variables -------------------------------------------------------*/
extern t_sendUsbData sendUsbData;

/* Exported functions--------------------------------------------------------*/
extern void Deal_sendMonitorData(void);
extern void SPI_FullDuplex_Init(void);
extern void SPI_Deal_sendMonitorData(void);
extern void Usart_Init(uint32_t Baud);
extern void Usart_Deal_sendMonitorData(void);
#endif
