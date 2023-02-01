/********************************** (C) COPYRIGHT *******************************
 * File Name          : uart.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/06/30
 * Description        : 
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "config.h"
#include "uart.h"
#include "atomic.h"
#include "test_dtm.h"

static tmosTaskID uart_taskid;

static uint8_t rx_buf[100];
static struct simple_buf *uart_buf = NULL;
static struct simple_buf uart_buffer;

enum uart_status {
    UART_STATUS_IDLE,
    UART_STATUS_RCVING,
    UART_STATUS_RCV_END,
    UART_STATUS_SENDING,

    UART_STATUS_NUM,
};

atomic_t uart_flag;

void uart_buffer_create(struct simple_buf **buf)
{
    *buf = simple_buf_create(&uart_buffer, rx_buf, sizeof(rx_buf));
}

int uart_start_receiving(void)
{
    PRINT("uart2 reeciving\n");
    atomic_set(&uart_flag, UART_STATUS_RCVING);
    uart_buffer_create(&uart_buf);
    USART_Cmd(USART2, ENABLE);

    return 0;
}

void uart2_send_data(uint8_t *data, uint16_t len)
{
    for(int i = 0; i < len; i++) {
        USART_SendData(USART2, data[i]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {
            /* waiting for sending finish */
            ;
        }
    }
}

int uart_send(struct simple_buf *buf)
{
    atomic_set(&uart_flag, UART_STATUS_SENDING);

    uint8_t *send_data;
    uint16_t send_len;

    send_len = buf->len;
    send_data = simple_buf_pull(buf, buf->len);
    uart2_send_data(send_data, send_len);
    atomic_set(&uart_flag, UART_STATUS_IDLE);

    PRINT("uart2 send %d bytes [", send_len);
    for(int i = 0; i < send_len; i++){
        if(i) PRINT(" ");
        PRINT("%#x", send_data[i]);
    }
    PRINT("]\n");

    return 0;
}

/*********************************************************************
 * @fn      uart_processevent
 *
 * @brief   process handle
 *
 * @param   task_id  - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
tmosEvents uart_processevent(tmosTaskID task_id, tmosEvents events)
{
    if(events & SYS_EVENT_MSG)
    {
        uint8_t *msgPtr;

        msgPtr = tmos_msg_receive(task_id);
        if(msgPtr)
        {
            /* De-allocate */
            tmos_msg_deallocate(msgPtr);
        }
        return (events ^ SYS_EVENT_MSG);
    }

    if(events & UART_RECEIVE_POLL_EVT) {
        if(atomic_get(&uart_flag) == UART_STATUS_RCV_END) {
            PRINT("uart2 recevied %d bytes [", uart_buf->len);
            for(int i = 0; i < uart_buf->len; i++) {
                if(i) PRINT(" ");
                PRINT("%#x", uart_buf->data[i]);
            }
            PRINT("]\n");

            struct uart_process_msg *uart_msg;

            uart_msg = (struct uart_process_msg *)    \
                tmos_msg_allocate(sizeof(struct uart_process_msg));

            if(uart_msg) {
                uart_msg->hdr.event = UART_PROCESS_EVT;
                uart_msg->hdr.status = true;
                uart_msg->data = (uint8_t *)uart_buf;
                tmos_msg_send(test_taskid, (uint8_t *)uart_msg );
            }
        }

        return (events ^ UART_RECEIVE_POLL_EVT);
    }

    return 0;
}


void uart_task_init(void)
{
    uart_taskid = TMOS_ProcessEventRegister(uart_processevent);

    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* USART2 TX-->A.2   RX-->A.3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    tmos_start_reload_task(uart_taskid, UART_RECEIVE_POLL_EVT, 
        MS1_TO_SYSTEM_TIME(100));

    atomic_set(&uart_flag, UART_STATUS_IDLE);
    uart_start_receiving();
}

/*********************************************************************
 * @fn      USART2_IRQHandler
 *
 * @brief   This function handles USART2 global interrupt request.
 *
 * @return  none
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        simple_buf_add_u8(uart_buf, USART_ReceiveData(USART2));
    }

    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        atomic_set(&uart_flag, UART_STATUS_RCV_END);

        /* clear IDLE flag */
        (void)USART_ReceiveData(USART2);
    }
}

