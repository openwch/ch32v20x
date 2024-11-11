/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/02/20
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2024 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 This example shows that the USART peripheral is configured in smart card mode
 and reads ATR data, and then attempts to read the data in T=0 mode.
 pins:
    SMARTCARD_CK  -- PA4
    SMARTCARD_IO  -- PA2
    SMARTCARD_RST -- PA5
    SMARTCARD_VCC -- 3.3V
 *
 */

#include "debug.h"
#include "string.h"

/* Paramenters */
#define ATR_MAX_SIZE ((uint8_t)33)         /* Maximum size of ATR byte array */
#define ATR_MAX_HISTORICAL ((uint8_t)15)   /* Maximum number of historical bytes */
#define ATR_MAX_PROTOCOLS ((uint8_t)7)     /* Maximun number of protocols */
#define ATR_MAX_IB ((uint8_t)4)            /* Maximum number of interface bytes per protocol */
#define ATR_INTERFACE_BYTE_TA ((uint8_t)0) /* Interface byte TAi */
#define ATR_INTERFACE_BYTE_TB ((uint8_t)1) /* Interface byte TBi */
#define ATR_INTERFACE_BYTE_TC ((uint8_t)2) /* Interface byte TCi */
#define ATR_INTERFACE_BYTE_TD ((uint8_t)3) /* Interface byte TDi */

#define STATE_PARSE_TS 1
#define STATE_PARSE_T0 2
#define STATE_PARSE_TA 3
#define STATE_PARSE_TB 4
#define STATE_PARSE_TC 5
#define STATE_PARSE_TD 6
#define STATE_PARSE_HIST_BYTES 7
#define STATE_PARSE_TCK 8
#define STATE_PARSE_END 255

#define LC_MAX (20)
#define RST_PIN GPIO_Pin_5

typedef struct {
    uint32_t length;
    uint8_t TS; /* Initial character */
    uint8_t T0; /* Format character */

    struct {
        uint8_t value;                        /* The value of the Interface byte */
        uint8_t present;                      /* The presence of the Interface byte */
    } ib[ATR_MAX_PROTOCOLS][ATR_MAX_IB], TCK; /* Interface bytes, TCK */

    uint8_t pn;                     /* Protocol number: the value of i of the Interface bytes */
    uint8_t Ti[ATR_MAX_HISTORICAL]; /* Historical bytes table */
    uint8_t Tin;                    /* Historical bytes number */
    int8_t protocol;
} ATR_TypeDef;

int ATR_Parse(ATR_TypeDef *atr, uint8_t *atr_buf, int32_t len) {
    uint8_t data;
    uint8_t TCK = 0;
    uint8_t K = 0;
    uint8_t Yi = 0;
    int32_t k, state, index, length, protocol;
    uint8_t *ptr;

    length = len;
    ptr = atr_buf;
    state = STATE_PARSE_TS;
    index = 0;
    k = 0;
    protocol = 0;

    while (ptr < (atr_buf + length)) {
        data = *ptr++;
        if (state != STATE_PARSE_TS) {
            TCK ^= data;
        }

        switch (state) {
        case STATE_PARSE_TS:
            atr->TS = data;
            printf("Ts=%x \n", data);
            if (data == 0x3b) {
                state = STATE_PARSE_T0;
            } else if (data == 0x03) {
                printf("not support\n");
                return 1;
            } else {
                printf("error Ts\n");
                return 1;
            }
            break;
        case STATE_PARSE_T0:
            atr->T0 = data;
            atr->Tin = data & 0x0f;
            printf("T0=%x\n", data);
            printf("Historical bytes number=%d\n", data & 0x0f);
            K = data & 0x0F;
            Yi = data;
            if (data & 0x10) {
                state = STATE_PARSE_TA;
            } else if (data & 0x20) {
                state = STATE_PARSE_TB;
            } else {
                if (data & 0x40) {
                    state = STATE_PARSE_TC;
                } else if (data & 0x80) {
                    state = STATE_PARSE_TD;
                } else {
                    state = STATE_PARSE_HIST_BYTES;
                }
            }
            break;
        case STATE_PARSE_TA:
            printf("TA%d=%x \n", index, data);
            atr->ib[index][ATR_INTERFACE_BYTE_TA].present = 1;
            atr->ib[index][ATR_INTERFACE_BYTE_TA].value = data;
            if (Yi & 0x20) {
                state = STATE_PARSE_TB;
            } else if (Yi & 0x40) {
                state = STATE_PARSE_TC;
            } else if (Yi & 0x80) {
                state = STATE_PARSE_TD;
            } else {
                state = STATE_PARSE_HIST_BYTES;
            }
            break;
        case STATE_PARSE_TB:
            printf("TB%d=%x \n", index, data);
            atr->ib[index][ATR_INTERFACE_BYTE_TB].present = 1;
            atr->ib[index][ATR_INTERFACE_BYTE_TB].value = data;
            if (Yi & 0x40) {
                state = STATE_PARSE_TC;
            } else if (Yi & 0x80) {
                state = STATE_PARSE_TD;
            } else {
                state = STATE_PARSE_HIST_BYTES;
            }
            break;
        case STATE_PARSE_TC:
            printf("TC%d=%x \n", index, data);
            atr->ib[index][ATR_INTERFACE_BYTE_TC].present = 1;
            atr->ib[index][ATR_INTERFACE_BYTE_TC].value = data;
            if (Yi & 0x80) {
                state = STATE_PARSE_TD;
            } else {
                state = STATE_PARSE_HIST_BYTES;
            }
            break;
        case STATE_PARSE_TD:

            Yi = data;
            printf("TD%d=%x \n", index, data);
            atr->ib[index][ATR_INTERFACE_BYTE_TD].present = 1;
            atr->ib[index][ATR_INTERFACE_BYTE_TD].value = data;
            if (index == 0) {
                protocol = Yi & 0x0F;
            }
            index++;
            atr->pn = index;

            if (Yi & 0xF0) {
                if (Yi & 0x10) {
                    /* TAx character present */
                    state = STATE_PARSE_TA;
                } else if (Yi & 0x20) {
                    /* TBx character present */
                    state = STATE_PARSE_TB;
                } else if (Yi & 0x40) {
                    /* TCx character present */
                    state = STATE_PARSE_TC;
                } else if (Yi & 0x80) {
                    /* TDx character present */
                    state = STATE_PARSE_TD;
                } else {
                    state = STATE_PARSE_HIST_BYTES;
                }
            } else {
                state = STATE_PARSE_HIST_BYTES;
            }
            break;
        case STATE_PARSE_HIST_BYTES:
            if (K) {

                if (k < K) {
                    atr->Ti[k] = data;
                    printf("T[%d]=%x\n", k, data);
                    k++;
                    if (k == K) {
                        if (protocol > 0) {
                            state = STATE_PARSE_TCK;
                        }
                    }
                }
                break;
            }
        case STATE_PARSE_TCK:
            atr->TCK.present = 1;
            atr->TCK.value = data;
            printf("get TCK %x\n", data);
            if (TCK) {
                // TCK != 0
                printf("TCK error\n");
                return 1;
            }
            //            printf("cal TCK result %x\n", TCK);
            break;
        }
        if (state == STATE_PARSE_HIST_BYTES && K == 0 && protocol == 0)
            break;
    }

    atr->protocol = protocol;
    return 0;
}

void USART2_ReCFG(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    USART_ClockInitTypeDef USART_ClockInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* USART2  Ck-->A.4   TX-->A.2  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
    USART_ClockInit(USART2, &USART_ClockInitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);

    /// Enable NACK transmission
    USART_SmartCardNACKCmd(USART2, ENABLE);

    /// Enable Smartcard mode
    USART_SmartCardCmd(USART2, ENABLE);

    USART_SetPrescaler(USART2, 5);
    USART_SetGuardTime(USART2, 15);
}

void RST_Init() {
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, RST_PIN);
}

void SC_ClearUart() {
    int32_t i = 0;
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) {
        i++;
        if (i > 1) {
            break;
        }
        USART_ReceiveData(USART2);
    }
}

void SC_TransmitData(uint8_t *data, uint16_t size) {
    uint8_t dat = 0;
    for (uint8_t index = 0; index < size; index++) {
        dat = data[index];
        USART_SendData(USART2, dat);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {
        }
    }
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {
    }
}

void SC_ReceiveData(uint8_t *data, uint16_t *size) {
    uint32_t i = 0, t = 0;
    SC_ClearUart();
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET) {
        t++;
        Delay_Ms(1);
        if (t > 10) {
            break;
        }
    }
    t = 0;
    while (1) {
        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) {
            data[i] = USART_ReceiveData(USART2);
            i++;
            t = 0;
        } else if (t < 10) {
            Delay_Ms(1);
            t++;
        } else {
            break;
        }
    }
    *size = i;
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void) {
    ATR_TypeDef atr = {0}; /* Answer To Reset structure */

    uint8_t pps[] = {0xff, 0x00, 0xff};
    uint8_t testcmd[] = {0x00, 0x84, 0x00, 0x00, 0x08};
    uint8_t recvbuf[ATR_MAX_SIZE] = {0};
    uint16_t recvpos = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());
    printf("USART SmartCard TEST\r\n");
    RST_Init();
    USART2_ReCFG(); /* USART2 INIT */

    GPIO_ResetBits(GPIOA, RST_PIN);
    Delay_Ms(1);
    GPIO_SetBits(GPIOA, RST_PIN);

    // try to get ATR
    SC_ReceiveData(recvbuf, &(recvpos));

    if (recvpos == 0) {
        printf("cannot get atr\n");
        printf("end\n");
        while (1) {
        }
    }

    if (ATR_Parse(&atr, recvbuf, recvpos) != 0) {
        printf("end\n");
        while (1) {
        }
    }

    printf("mode T=%d \n", atr.protocol);
    if (atr.protocol != 0) {
        recvpos = 0;
        // send pps
        SC_TransmitData(pps, 3);
        SC_ReceiveData(recvbuf, &(recvpos));
        if (memcmp(pps, recvbuf, recvpos) != 0) {
            printf("cannot switch to T=0 mode\n");
            printf("end\n");
            while (1) {
            }
        } else {
            printf("switch to T=0 mode\n");
        }
    }

    SC_TransmitData(testcmd, 5);
    SC_ReceiveData(recvbuf, &(recvpos));

    for (int i = 0; i < recvpos; i++) {
        printf("[%d]=%x \n", i, recvbuf[i]);
    }
    printf("SW1 = %x \n", recvbuf[recvpos - 2]);
    printf("SW2 = %x \n", recvbuf[recvpos - 1]);
    if (recvbuf[recvpos - 2] == 0x90 && recvbuf[recvpos - 1] == 0x00 && recvpos == 11) {
        printf("success\n");
    }
    printf("end\n");
    while (1) {
    }
}
