/********************************** (C) COPYRIGHT *******************************
 * File Name          : peripheral.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/12/11
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Peripheral Task Events
#define SBP_START_DEVICE_EVT (1 << 0)
#define SBP_PERIODIC_EVT     (1 << 1)
#define SBP_READ_RSSI_EVT    (1 << 2)
#define SBP_PARAM_UPDATE_EVT (1 << 3)
#define SBP_PHY_UPDATE_EVT   (1 << 4)
#define SBP_SET_SYNC_PARAM   (1 << 14)

/*********************************************************************
 * MACROS
 */
typedef struct
{
    uint16_t connHandle; // Connection handle of current connection
    uint16_t connInterval;
    uint16_t connSlaveLatency;
    uint16_t connTimeout;
} peripheralConnItem_t;
extern peripheralConnItem_t peripheralConnList;
extern uint16_t             syncHandle;
extern uint8_t              syncTran;

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Application
 */
extern void Peripheral_Init(void);
void        Sync_Init(void);

/*
 * Task Event Processor for the BLE Application
 */
extern uint16_t Peripheral_ProcessEvent(uint8_t task_id, uint16_t events);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
