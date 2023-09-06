/********************************** (C) COPYRIGHT *******************************
 * File Name          : rsp_sync.C
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/12/10
 * Description        : ESLSync
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "CONFIG.h"
#include "hal.h"
#include "rsp.h"

/*********************************************************************
 * MACROS
 */

#define PAWR_SUBEVENT_NUM  4
#define PAWR_RESPOSE_COUNT 16
#define PAWR_DATALEN_MAX   10

/*********************************************************************
 * LOCAL VARIABLES
 */

// Task ID for internal task/event processing
static uint8_t Sync_TaskId;
uint16_t       syncHandle = 0;
uint8_t        pawrRspData[PAWR_SUBEVENT_NUM][PAWR_DATALEN_MAX];

/*********************************************************************
 * LOCAL FUNCTIONS
 */
uint16_t    Central_ProcessEvent(uint8_t task_id, uint16_t events);
static void simpleBLECentralEventCB(gapRoleEvent_t *pEvent);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static const gapCentralRoleCB_t simpleBLERoleCB = {
    NULL,                    // RSSI callback
    simpleBLECentralEventCB, // Event callback
    NULL
};

/*********************************************************************
 * @fn      Central_Init
 *
 * @brief   Initialization function for the Simple BLE Central App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification).
 *
 * @param   task_id - the ID assigned by TMOS.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void Sync_Init(void)
{
    Sync_TaskId = TMOS_ProcessEventRegister(Central_ProcessEvent);
    //  {
    //    // Setup scan GAP
    //    GAP_SetParamValue( TGAP_DISC_SCAN_DURATION, 200 );
    //
    //    GAP_SetParamValue( TGAP_DISC_SCAN_INT, 32 );
    //    GAP_SetParamValue( TGAP_DISC_SCAN_WIND, 32 );
    //    GAP_SetParamValue( TGAP_DISC_SCAN_CODED_INT, 32 );
    //    GAP_SetParamValue( TGAP_DISC_SCAN_CODED_WIND, 32 );
    //    GAP_SetParamValue( TGAP_DISC_SCAN_PHY, GAP_PHY_BIT_LE_1M|GAP_PHY_BIT_LE_CODED );
    //  }
    // Setup a delayed profile startup
    tmos_set_event(Sync_TaskId, SBP_START_DEVICE_EVT);
}

/*********************************************************************
 * @fn      Central_ProcessEvent
 *
 * @brief   Simple BLE Central Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16_t Central_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;

        if((pMsg = tmos_msg_receive(Sync_TaskId)) != NULL)
        {
            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if(events & SBP_START_DEVICE_EVT)
    {
        // Start the Device
        PRINT("start device-%X.\n",
              GAPRole_CentralStartDevice(Sync_TaskId, NULL, (gapCentralRoleCB_t *)&simpleBLERoleCB));
        return (events ^ SBP_START_DEVICE_EVT);
    }
    return 0;
}

/*********************************************************************
 * @fn      simpleBLECentralEventCB
 *
 * @brief   Central event callback function.
 *
 * @param   pEvent - pointer to event structure
 *
 * @return  none
 */
static void simpleBLECentralEventCB(gapRoleEvent_t *pEvent)
{
    switch(pEvent->gap.opcode)
    {
        case GAP_SYNC_ESTABLISHED_EVENT:
        {
            PRINT("status=%x\n", pEvent->syncEstEvt.status);
            if(!pEvent->syncEstEvt.status)
            {
                syncHandle = pEvent->syncEstEvt.syncHandle;
                PRINT("sync established:%x\n", syncHandle);
            }
            break;
        }

        case GAP_PERIODIC_ADV_DEVICE_INFO_EVENT:
        {
            PRINT("sync report.%d\n", pEvent->devicePeriodicInfo.syncHandle);
            PRINT("tx power:%d\n", pEvent->devicePeriodicInfo.txPower);
            PRINT("dataStatus:%d\n", pEvent->devicePeriodicInfo.dataStatus);
            PRINT("data len:%d\n", pEvent->devicePeriodicInfo.dataLength);
            for(int i = 0; i < pEvent->devicePeriodicInfo.dataLength && i < 16; i++)
            {
                PRINT("%2x ", pEvent->devicePeriodicInfo.pEvtData[i]);
            }
            PRINT("\n");
            if(pEvent->devicePeriodicInfo.subevent != 0xFF)
            {
                if(!(pEvent->devicePeriodicInfo.eventCounter % 1) && pEvent->devicePeriodicInfo.subevent == 0)
                {
                    gapPawrSetResponseData_t parm;

                    parm.syncHandle = syncHandle;
                    parm.reqEvent = pEvent->devicePeriodicInfo.eventCounter;
                    parm.reqSubevent = pEvent->devicePeriodicInfo.subevent;
                    parm.rspSubevent = 1;
                    parm.rspSlot = 5;
                    parm.rspDataLength = 4;
                    for(int i = 0; i < parm.rspDataLength; i++)
                    {
                        pawrRspData[parm.rspSlot][i] = parm.rspSlot + i;
                    }
                    parm.pRspData = pawrRspData[parm.rspSlot];
                    GAPPawr_SetResponseData(&parm);
                    PRINT("set response data:%d\n", parm.rspDataLength);
                }
            }
            break;
        }

        case GAP_SYNC_LOST_EVENT:
        {
            syncHandle = 0;
            PRINT("sync lost:%x\n", pEvent->syncLostEvt.syncHandle);
            if(peripheralConnList.connHandle == GAP_CONNHANDLE_INIT)
            {
                uint8_t advertising_enable = TRUE;
                GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advertising_enable);
            }
            break;
        }

        default:
            break;
    }
}

/************************ endfile @ sync **************************/
