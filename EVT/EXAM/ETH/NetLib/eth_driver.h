/********************************** (C) COPYRIGHT *******************************
* File Name          : eth_driver.h
* Author             : WCH
* Version            : V1.3.0
* Date               : 2022/05/27
* Description        : This file contains the headers of the ETH Driver.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __ETH_DRIVER__
#define __ETH_DRIVER__

#ifdef __cplusplus
 extern "C" {
#endif 

#include "debug.h"

#define USE_10M_BASE                            1  // Internal 10M PHY

#ifndef PHY_MODE
#define PHY_MODE                                USE_10M_BASE
#endif

#define ROM_CFG_USERADR_ID                      0x1FFFF7E8

#define PHY_LINK_TASK_PERIOD                    50

#define PHY_ANLPAR_SELECTOR_FIELD               0x1F
#define PHY_ANLPAR_SELECTOR_VALUE               0x01       // 5B'00001

#define PHY_LINK_INIT                           0x00
#define PHY_LINK_SUC_P                          (1<<0)
#define PHY_LINK_SUC_N                          (1<<1)
#define PHY_LINK_WAIT_SUC                       (1<<7)

#define PHY_PN_SWITCH_P                         (0<<2)
#define PHY_PN_SWITCH_N                         (1<<2)
#define PHY_PN_SWITCH_AUTO                      (2<<2)

#ifndef WCHNETTIMERPERIOD
#define WCHNETTIMERPERIOD                       10   /* Timer period, in Ms. */
#endif

#define PHY_NEGOTIATION_PARAM_INIT()      do{\
        phySucCnt = 0;\
        phyStatus = 0;\
        phyLinkCnt = 0;\
        phyRetryCnt = 0;\
        phyPNChangeCnt = 0;\
        phyLinkStatus = PHY_LINK_INIT;\
}while(0)


/* definition for Ethernet frame */
#define ETH_MAX_PACKET_SIZE    1536    /* ETH_HEADER + ETH_EXTRA + MAX_ETH_PAYLOAD + ETH_CRC */
#define ETH_HEADER               14    /* 6 byte Dest addr, 6 byte Src addr, 2 byte length/type */
#define ETH_CRC                   4    /* Ethernet CRC */
#define ETH_EXTRA                 2    /* Extra bytes in some cases */
#define VLAN_TAG                  4    /* optional 802.1q VLAN Tag */
#define MIN_ETH_PAYLOAD          46    /* Minimum Ethernet payload size */
#define MAX_ETH_PAYLOAD        1500    /* Maximum Ethernet payload size */

/* Bit or field definition of TDES0 register (DMA Tx descriptor status register)*/
#define ETH_DMATxDesc_OWN         ((uint32_t)0x80000000)  /* OWN bit: descriptor is owned by DMA engine */

/* Bit or field definition of RDES0 register (DMA Rx descriptor status register) */
#define ETH_DMARxDesc_OWN         ((uint32_t)0x80000000)  /* OWN bit: descriptor is owned by DMA engine  */
#define ETH_DMARxDesc_FL          ((uint32_t)0x3FFF0000)  /* Receive descriptor frame length  */
#define ETH_DMARxDesc_ES          ((uint32_t)0x00008000)  /* Error summary:  */
#define ETH_DMARxDesc_FS          ((uint32_t)0x00000200)  /* First descriptor of the frame  */
#define ETH_DMARxDesc_LS          ((uint32_t)0x00000100)  /* Last descriptor of the frame  */

#define ETH_DMARxDesc_FrameLengthShift       16

/* ETHERNET errors */
#define  ETH_ERROR              ((uint32_t)0)
#define  ETH_SUCCESS            ((uint32_t)1)

/* ETH structure definition */
typedef struct
{
    uint32_t volatile  Status;        /* Status */
    uint32_t   ControlBufferSize;     /* Control and Buffer1, Buffer2 lengths */
    uint32_t   Buffer1Addr;           /* Buffer1 address pointer */
    uint32_t   Buffer2NextDescAddr;   /* Buffer2 or next descriptor address pointer */
} ETH_DMADESCTypeDef;


#include "wchnet.h"

extern SOCK_INF SocketInf[ ];

void ETH_PHYLink( void );
void WCHNET_ETHIsr( void );
void WCHNET_MainTask( void );
void ETH_LedConfiguration(void);
void ETH_Init( uint8_t *macAddr );
void ETH_LedLinkSet( uint8_t mode );
void ETH_LedDataSet( uint8_t mode );
void WCHNET_TimeIsr( uint16_t timperiod );
void ETH_Configuration( uint8_t *macAddr );
uint8_t ETH_LibInit( uint8_t *ip, uint8_t *gwip, uint8_t *mask, uint8_t *macaddr);

#ifdef __cplusplus
}
#endif

#endif
