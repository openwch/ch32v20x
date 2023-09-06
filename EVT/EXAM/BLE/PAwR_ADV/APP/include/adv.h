/********************************** (C) COPYRIGHT *******************************
 * File Name          : central.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/11/12
 * Description        : 观察应用主函数及任务系统初始化
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#ifndef CENTRAL_H
#define CENTRAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Simple BLE Ceentral Task Events
#define START_DEVICE_EVT           (1 << 0)
#define START_DISCOVERY_EVT        (1 << 1)
#define START_SCAN_EVT             (1 << 2)
#define START_SVC_DISCOVERY_EVT    (1 << 3)
#define START_PARAM_UPDATE_EVT     (1 << 4)
#define START_PHY_UPDATE_EVT       (1 << 5)
#define START_READ_OR_WRITE_EVT    (1 << 6)
#define START_WRITE_CCCD_EVT       (1 << 7)
#define START_READ_RSSI_EVT        (1 << 8)
#define ESTABLISH_LINK_TIMEOUT_EVT (1 << 9)

#define START_SYNC_CONN_TEST       (1 << 10)
#define START_EMU_TEST             (1 << 11)
#define START_UPDATE_TEST          (1 << 12)
#define START_SYNC_T_TEST          (1 << 13)
#define START_PERIODIC_T_TEST      (1 << 14)

extern uint8_t gPeriodicState;
extern uint8_t PeerAddrDef[6];
/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Application
 */
extern void Central_Init(void);
void        Periodic_Init(void);

/*
 * Task Event Processor for the BLE Application
 */
extern uint16_t Central_ProcessEvent(uint8_t task_id, uint16_t events);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
