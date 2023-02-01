/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2022/06/22
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* Header file contains */
#include "CONFIG.h"
#include "HAL.h"
#include "eth.h"
#include "eth_driver.h"
#include "string.h"
#include "debug.h"
#include "wchnet.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */

#define KEEPLIVE_ENABLE         1                           //Enable keeplive function

u8 MACAddr[6];                                              //MAC address
u8 IPAddr[4] = { 192, 168, 1, 10 };                         //IP address
u8 GWIPAddr[4] = { 192, 168, 1, 1 };                        //Gateway IP address
u8 IPMask[4] = { 255, 255, 255, 0 };                        //subnet mask
u8 DESIP[4] = { 192, 168, 1, 100 };                         //destination IP address
u16 desport = 1000;                                         //destination port
u16 srcport = 1000;                                         //source port

u8 SocketId;
u8 socket[WCHNET_MAX_SOCKET_NUM];                           //Save the currently connected socket
u8 SocketRecvBuf[WCHNET_MAX_SOCKET_NUM][RECE_BUF_LEN];      //socket receive buffer
u8 MyBuf[RECE_BUF_LEN];
uint8_t eth_TaskID;
/*********************************************************************
 * @fn      mStopIfError
 *
 * @brief   check if error.
 *
 * @param   iError - error constants.
 *
 * @return  none
 */
void mStopIfError(u8 iError)
{
    if (iError == WCHNET_ERR_SUCCESS)
        return;
    printf("Error: %02X\r\n", (u16) iError);
}

/*********************************************************************
 * @fn      TIM2_Init
 *
 * @brief   Initializes TIM2.
 *
 * @return  none
 */
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / 1000000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = WCHNETTIMERPERIOD * 1000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/*********************************************************************
 * @fn      WCHNET_CreateTcpSocket
 *
 * @brief   Create TCP Socket
 *
 * @return  none
 */
void WCHNET_CreateTcpSocket(void)
{
    u8 i;
    SOCK_INF TmpSocketInf;

    memset((void *) &TmpSocketInf, 0, sizeof(SOCK_INF));
    memcpy((void *) TmpSocketInf.IPAddr, DESIP, 4);
    TmpSocketInf.DesPort = desport;
    TmpSocketInf.SourPort = srcport++;
    TmpSocketInf.ProtoType = PROTO_TYPE_TCP;
    TmpSocketInf.RecvBufLen = RECE_BUF_LEN;
    i = WCHNET_SocketCreat(&SocketId, &TmpSocketInf);
    printf("WCHNET_SocketCreat %d\r\n", SocketId);
    mStopIfError(i);
    i = WCHNET_SocketConnect(SocketId);                        //make a TCP connection
    mStopIfError(i);
}

/*********************************************************************
 * @fn      WCHNET_DataLoopback
 *
 * @brief   Data loopback function.
 *
 * @param   id - socket id.
 *
 * @return  none
 */
void WCHNET_DataLoopback(u8 id)
{
#if 1
    u8 i;
    u32 len;
    u32 endAddr = SocketInf[id].RecvStartPoint + SocketInf[id].RecvBufLen;       //Receive buffer end address

    if ((SocketInf[id].RecvReadPoint + SocketInf[id].RecvRemLen) > endAddr) {    //Calculate the length of the received data
        len = endAddr - SocketInf[id].RecvReadPoint;
    }
    else {
        len = SocketInf[id].RecvRemLen;
    }
    i = WCHNET_SocketSend(id, (u8 *) SocketInf[id].RecvReadPoint, &len);         //send data
    if (i == WCHNET_ERR_SUCCESS) {
        WCHNET_SocketRecv(id, NULL, &len);                                       //Clear sent data
    }
#else
    u32 len, totallen;
    u8 *p = MyBuf;

    len = WCHNET_SocketRecvLen(id, NULL);                                //query length
    WCHNET_SocketRecv(id, MyBuf, &len);                                  //Read the data of the receive buffer into MyBuf
    totallen = len;
    while(1){
        len = totallen;
        WCHNET_SocketSend(id, p, &len);                                  //Send the data
        totallen -= len;                                                 //Subtract the sent length from the total length
        p += len;                                                        //offset buffer pointer
        if(totallen) continue;                                           //If the data is not sent, continue to send
        break;                                                           //After sending, exit
    }
#endif
}

/*********************************************************************
 * @fn      WCHNET_HandleSockInt
 *
 * @brief   Socket Interrupt Handle
 *
 * @param   socketid - socket id.
 *          intstat - interrupt status
 *
 * @return  none
 */
void WCHNET_HandleSockInt(u8 socketid, u8 intstat)
{
    u8 i;
    u32 len;

    if (intstat & SINT_STAT_RECV)                              //receive data
    {
//        WCHNET_DataLoopback(socketid);                         //Data loopback
        WCHNET_SocketRecv(socketid, NULL, &len);
        printf("SINT_STAT_RECV %d\r\n",len);
    }
    if (intstat & SINT_STAT_CONNECT)                           //connect successfully
    {
#if KEEPLIVE_ENABLE
        WCHNET_SocketSetKeepLive(socketid, ENABLE);
#endif
        WCHNET_ModifyRecvBuf(socketid, (u32) SocketRecvBuf[socketid], RECE_BUF_LEN);
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++) {
            if (socket[i] == 0xff) {                           //save connected socket id
                socket[i] = socketid;
                break;
            }
        }
        printf("TCP Connect Success\r\n");
        printf("socket id: %d\r\n", socket[i]);
        tmos_start_task(eth_TaskID, ETH_SENG_DATA_EVENT, 160);
    }
    if (intstat & SINT_STAT_DISCONNECT)                         //disconnect
    {
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++) {           //delete disconnected socket id
            if (socket[i] == socketid) {
                socket[i] = 0xff;
                break;
            }
        }
        printf("TCP Disconnect\r\n");
        tmos_stop_task(eth_TaskID, ETH_SENG_DATA_EVENT);
    }
    if (intstat & SINT_STAT_TIM_OUT)                             //timeout disconnect
    {
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++) {            //delete disconnected socket id
            if (socket[i] == socketid) {
                socket[i] = 0xff;
                break;
            }
        }
        printf("TCP Timeout\r\n");
        WCHNET_CreateTcpSocket();
    }
}

/*********************************************************************
 * @fn      WCHNET_HandleGlobalInt
 *
 * @brief   Global Interrupt Handle
 *
 * @return  none
 */
void WCHNET_HandleGlobalInt(void)
{
    u8 intstat;
    u16 i;
    u8 socketint;

    intstat = WCHNET_GetGlobalInt();                              //get global interrupt flag
    if (intstat & GINT_STAT_UNREACH)                              //Unreachable interrupt
    {
        printf("GINT_STAT_UNREACH\r\n");
    }
    if (intstat & GINT_STAT_IP_CONFLI)                            //IP conflict
    {
        printf("GINT_STAT_IP_CONFLI\r\n");
    }
    if (intstat & GINT_STAT_PHY_CHANGE)                           //PHY status change
    {
        i = WCHNET_GetPHYStatus();
        if (i & PHY_Linked_Status)
            printf("PHY Link Success\r\n");
    }
    if (intstat & GINT_STAT_SOCKET) {                             //socket related interrupt
        for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++) {
            socketint = WCHNET_GetSocketInt(i);
            if (socketint)
                WCHNET_HandleSockInt(i, socketint);
        }
    }
}

/*********************************************************************
 * @fn      eth_ProcessEvent
 *
 * @brief   eth Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16_t eth_ProcessEvent(uint8_t task_id, uint16_t events)
{
    //  VOID task_id; // TMOS required parameter that isn't used in this function

    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;

        if((pMsg = tmos_msg_receive(eth_TaskID)) != NULL)
        {
            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }
    if(events & ETH_SENG_DATA_EVENT)
    {
        uint32_t stata;
        uint8_t data[2] = {0x77};
        stata = eth_send(data, 2);
        if( stata )
        {
            printf("eth send fail %x \n",stata);
        }
        tmos_start_task(eth_TaskID, ETH_SENG_DATA_EVENT, 1600);
        return (events ^ ETH_SENG_DATA_EVENT);
    }

    // Discard unknown events
    return 0;
}

/*********************************************************************
 * @fn      eth_send
 *
 * @brief   eth_send
 *
 * @return  @ERR_T
 */
uint8_t eth_send( uint8_t *SendBuf, uint32_t l)
{
    uint32_t len;
    len = l;
    return WCHNET_SocketSend(SocketId, SendBuf, &len);
}

/*********************************************************************
 * @fn      eth_init
 *
 * @brief   eth_init
 *
 * @return  none
 */
void eth_init(void)
{
    uint8_t i;
    eth_TaskID = TMOS_ProcessEventRegister(eth_ProcessEvent);
    printf("net version:%x\n", WCHNET_GetVer());
    if ( WCHNET_LIB_VER != WCHNET_GetVer()) {
        printf("version error.\n");
    }
    WCHNET_GetMacAddr(MACAddr);                                  //get the chip MAC address
    printf("mac addr:");
    for(i = 0; i < 6; i++)
    {
        printf("%x ",MACAddr[i]);
    }
    printf("\n");
    TIM2_Init();
    i = ETH_LibInit(IPAddr, GWIPAddr, IPMask, MACAddr);          //Ethernet library initialize
    mStopIfError(i);
    if (i == WCHNET_ERR_SUCCESS)
        printf("WCHNET_LibInit Success\r\n");
#if KEEPLIVE_ENABLE                                              //Configure keeplive parameters
    {
        struct _KEEP_CFG cfg;

        cfg.KLIdle = 20000;
        cfg.KLIntvl = 15000;
        cfg.KLCount = 9;
        WCHNET_ConfigKeepLive(&cfg);
    }
#endif
    memset(socket, 0xff, WCHNET_MAX_SOCKET_NUM);
    for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++)
        WCHNET_CreateTcpSocket();                                //Create TCP Socket
}

/*********************************************************************
 * @fn      eth_process
 *
 * @brief   eth_process
 *
 * @return  none
 */
void eth_process(void)
{
    /*Ethernet library main task function,
     * which needs to be called cyclically*/
    WCHNET_MainTask();
    /*Query the Ethernet global interrupt,
     * if there is an interrupt, call the global interrupt handler*/
    if(WCHNET_QueryGlobalInt())
    {
        WCHNET_HandleGlobalInt();
    }
}

/******************************** endfile @ main ******************************/
