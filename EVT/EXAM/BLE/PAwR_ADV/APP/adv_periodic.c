/********************************** (C) COPYRIGHT *******************************
 * File Name          : adv_periodic.C
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/12/10
 * Description        : APPeriodic
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "CONFIG.h"
#include "adv.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define DEFAULT_PERIO_DATA_LENGTH 80

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t Periodic_TaskID = INVALID_TASK_ID; // Task ID for internal task/event processing

static uint8_t advPeriodicData[B_MAX_ADV_PERIODIC_LEN] =
    {
        0x04,
        GAP_ADTYPE_LOCAL_NAME_SHORT,
        '1',
        '2',
        '3',

        0x09, // length of this data
        GAP_ADTYPE_URI,
        0x16,
        0x00,
        'w',
        'c',
        'h',
        '.',
        'c',
        'n',
};
#define PAWR_SUBEVENT_NUM  4
#define PAWR_RESPOSE_COUNT 16
#define PAWR_DATALEN_MAX   32

static uint8_t gAdvHandle = 0;
static uint8_t pawrData[PAWR_SUBEVENT_NUM][PAWR_DATALEN_MAX];
static uint8_t gPawr = TRUE;
static uint8_t gSubevent = 0xFF;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
uint16_t    Periodic_ProcessEvent(uint8_t task_id, uint16_t events);
static void Peripheral_ProcessTMOSMsg(tmos_event_hdr_t *pMsg);
static void peripheralStateNotificationCB(gapRole_States_t newState, gapRoleEvent_t *pEvent);

// GAP Role Callbacks
static gapRolesCBs_t Peripheral_PeripheralCBs = {
    peripheralStateNotificationCB, // Profile State Change Callbacks
    NULL,
    NULL};

uint8_t gPeriodicState;

/*********************************************************************
 * @fn      Peripheral_Init
 *
 * @brief   Initialization function for the Peripheral App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by TMOS.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void Periodic_Init(void)
{
    Periodic_TaskID = TMOS_ProcessEventRegister(Periodic_ProcessEvent);
    PRINT("Periodic_TaskID=%d\n", Periodic_TaskID);
    // Setup the GAP Peripheral Role Profile
    {
        uint8_t map = GAP_ADVCHAN_ALL;
        uint8_t advertise_event_type = GAP_ADTYPE_EXT_NONCONN_NONSCAN_UNDIRECT;
        uint8_t initial_advertising_enable = TRUE;

        GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(uint8_t), &advertise_event_type);
        GAPRole_SetParameter(GAPROLE_ADV_CHANNEL_MAP, sizeof(uint8_t), &map);
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initial_advertising_enable);
    }

    {
        uint16_t advInt = 80;

        // Set advertising interval
        GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, advInt);
        GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, advInt);

        // Enable scan req notify
        GAP_SetParamValue(TGAP_ADV_SCAN_REQ_NOTIFY, ENABLE);
    }
    {
        //  Set Periodic advertisement
        for(int i = 0; i < DEFAULT_PERIO_DATA_LENGTH; i++)
        {
            advPeriodicData[i] = i;
        }
        GAPRole_SetParameter(GAPROLE_PERIODIC_ADVERT_DATA, DEFAULT_PERIO_DATA_LENGTH, advPeriodicData);
    }
    tmos_set_event(Periodic_TaskID, START_DEVICE_EVT);
}

/*********************************************************************
 * @fn      Peripheral_ProcessEvent
 *
 * @brief   Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16_t Periodic_ProcessEvent(uint8_t task_id, uint16_t events)
{
    //  VOID task_id; // TMOS required parameter that isn't used in this function

    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;

        if((pMsg = tmos_msg_receive(Periodic_TaskID)) != NULL)
        {
            Peripheral_ProcessTMOSMsg((tmos_event_hdr_t *)pMsg);
            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }
    if(events & START_DEVICE_EVT)
    {
        // Start the Device
        GAPRole_PeripheralStartDevice(Periodic_TaskID, NULL, &Peripheral_PeripheralCBs);
        return (events ^ START_DEVICE_EVT);
    }
    if(events & START_SYNC_CONN_TEST)
    {
        gapPawrCreateConnection_t parm;
        bStatus_t                 s;

        parm.advHandle = gAdvHandle;
        parm.subevent = gSubevent;
        parm.ownAddrType = 0;
        parm.peerAddrType = 0;
        tmos_memcpy(parm.peerAddr, PeerAddrDef, 6);
        parm.connIntervalMin = 10;
        parm.connIntervalMax = 50;
        parm.maxLatency = 0;
        parm.supervisionTimeout = 400;
        s = GAPPawr_CreatConnection(&parm);
        PRINT("start pawr connection.%x\n", s);
        return (events ^ START_SYNC_CONN_TEST);
    }
    return 0;
}

/*********************************************************************
 * @fn      Peripheral_ProcessGAPMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void Peripheral_ProcessGAPMsg(gapRoleEvent_t *pEvent)
{
    static uint8_t test_param;

    switch(pEvent->gap.opcode)
    {
        case GAP_PERI_ADV_SUBEVENT_DATA_REQ_EVENT:
        {
            gapPawrDataRequestEvent_t *pEvent_t = (gapPawrDataRequestEvent_t *)pEvent;

            PRINT("subevent data request.\n");
            //          PRINT("advHandle=%x\n",pEvent_t->advHandle);
            //          PRINT("subeventStart=%d subeventDataCount=%d\n",pEvent_t->subeventStart,pEvent_t->subeventDataCount);
            {
                gapPawrSetData_t param[PAWR_SUBEVENT_NUM];
                if(pEvent_t->subeventDataCount > PAWR_SUBEVENT_NUM)
                {
                    PRINT("subevent count error:%d  max=%d\n", pEvent_t->subeventDataCount, PAWR_SUBEVENT_NUM);
                }
                for(int i = 0; i < pEvent_t->subeventDataCount; i++)
                {
                    param[i].subevent = pEvent_t->subeventStart + i;
                    param[i].rspSlotStart = 4;
                    param[i].rspSlotCount = PAWR_RESPOSE_COUNT - param[i].rspSlotStart;
                    param[i].rspMaxLength = param[i].rspSlotCount * 10;
                    param[i].dataLength = 16;
                    //              tmos_memset( pawrData[i],0,param[i].dataLength);
                    tmos_memset(pawrData[i], i, param[i].dataLength);
                    pawrData[i][0] = test_param++;
                    param[i].pData = pawrData[i];
                }
                GAPPawr_SetSubeventData(gAdvHandle, pEvent_t->subeventDataCount, param);
            }
            break;
        }

        case GAP_PERI_ADV_RESPONSE_REPORT_EVENT:
        {
            gapPawrResponseEvent_t *pEvent_t = (gapPawrResponseEvent_t *)pEvent;
            {
                pawrResponseInfo_t *pList_t = pEvent_t->pList;
                for(int i = 0; i < pEvent_t->numResponses; i++)
                {
                    if(pList_t[i].dataStatus != 0xFF)
                    {
                        PRINT("rspSlot=%d len=%d rssi=%d\n", pList_t[i].rspSlot, pList_t[i].dataLength, pList_t[i].rssi);
                        for(int k = 0; k < pList_t[i].dataLength; k++)
                        {
                            PRINT("%02x ", pList_t[i].pData[k]);
                        }
                        PRINT("\n");
                        if(gSubevent == 0xFF)
                        {
                            gSubevent = i;
                            tmos_set_event(Periodic_TaskID, START_SYNC_CONN_TEST);
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

/*********************************************************************
 * @fn      Peripheral_ProcessTMOSMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void Peripheral_ProcessTMOSMsg(tmos_event_hdr_t *pMsg)
{
    switch(pMsg->event)
    {
        case GAP_MSG_EVENT:
        {
            Peripheral_ProcessGAPMsg((gapRoleEvent_t *)pMsg);
            break;
        }

        default:
            break;
    }
}

/*********************************************************************
 * @fn      peripheralStateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void peripheralStateNotificationCB(gapRole_States_t newState, gapRoleEvent_t *pEvent)
{
    gapRoleStates_t *pState = (gapRoleStates_t *)&newState;

    if(pEvent)
    {
        PRINT("\n------------------------------\n");
        PRINT("gap opcode:%x\n\n", pEvent->gap.opcode);
        PRINT("periodic state=%x\n", pState->periState);
        if(pState->periValid)
        {
            if((newState & GAPROLE_STATE_PERIODIC_MASK) == GAPROLE_PERIODIC_ENABLE)
            {
                gPeriodicState = 1;
            }
            else
            {
                gPeriodicState = 0;
            }
            PRINT("gPeriodicState = %d\n", gPeriodicState);
            return;
        }
        else if(pState->cteValid)
        {
            PRINT("cte state\n");
            return;
        }
    }
    switch(newState & GAPROLE_STATE_ADV_MASK)
    {
        case GAPROLE_STARTED:
            PRINT("Initialized..\n");
            break;

        case GAPROLE_ADVERTISING:
            if(pEvent->gap.opcode == GAP_MAKE_DISCOVERABLE_DONE_EVENT)
            {
                PRINT("Advertising..\n");
                gAdvHandle = 1;
                if(gPawr)
                {
                    bStatus_t         s;
                    gapPawrSetParam_t param;

                    param.taskID = Periodic_TaskID;
                    param.advHandle = gAdvHandle;
                    param.enable = TRUE;
                    param.advProperties = GAP_PERI_PROPERTIES_INCLUDE_TXPOWER;
                    param.rspSlotDelay = 0x2;
                    param.rspSlotSpacing = 0x02;
                    param.numRspSlots = PAWR_RESPOSE_COUNT;
                    param.numSubevents = PAWR_SUBEVENT_NUM;
                    param.subInterval = 2 + param.rspSlotDelay + (1 + (param.numRspSlots * param.rspSlotSpacing) / 10) + 4;
                    param.advIntervalMin = ((uint16_t)(param.numSubevents) * param.subInterval) + 1000;
                    param.advIntervalMax = param.advIntervalMin + 100;
                    s = GAPPawr_SetParameters(&param);
                    if(!s)
                        gPawr = FALSE;
                    PRINT("set pawr .s = %x\n", s);
                }
            }
            break;

        case GAPROLE_WAITING:
            if(pEvent->gap.opcode == GAP_END_DISCOVERABLE_DONE_EVENT)
            {
                PRINT("Waiting for advertising..\n");
            }
            else
            {
                PRINT("Error..%x\n", pEvent->gap.opcode);
            }
            break;

        case GAPROLE_ERROR:
            PRINT("Error..\n");
            break;

        default:
            break;
    }
    PRINT("Adv State:     %x\n", pState->advState);
    PRINT("Periodic State:%x\n", pState->periState);
    PRINT("CTE State:     %x\n", pState->cteState);
}

/********************************* end@periodic *************************************/
