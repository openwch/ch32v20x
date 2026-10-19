/********************************** (C) COPYRIGHT *******************************
 * File Name          : app.h
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2021/11/18
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef app_H
#define app_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define APP_NODE_EVT                   (1 << 0)
#define APP_DELETE_NODE_TIMEOUT_EVT    (1 << 1)
#define APP_DELETE_LOCAL_NODE_EVT      (1 << 2)
#define APP_DELETE_NODE_INFO_EVT       (1 << 3)

#define CMD_PROVISION                  0xA1
#define CMD_DELETE_NODE                0xA2
#define CMD_DELETE_NODE_ACK            0x82
#define CMD_DELETE_NODE_INFO           0xA3
#define CMD_LOCAL_RESET                0xAF

#define PERIPHERAL_CMD_LEN             1
#define PROVISION_NET_KEY_LEN          16
#define ADDRESS_LEN                    2

// Í¨¹ýble½ÓÊÕµ½µÄÅäÍø²ÎÊý£¬°üº¬ 1×Ö½ÚÃüÁîÂë+16×Ö½ÚÍøÂçÃÜÔ¿+2×Ö½ÚÍøÂçµØÖ·
#define PROVISION_DATA_LEN             (PERIPHERAL_CMD_LEN + PROVISION_NET_KEY_LEN + ADDRESS_LEN)
// É¾³ý½ÚµãÃüÁî£¬°üº¬ 1×Ö½ÚÃüÁîÂë+2×Ö½ÚÐèÒªÉ¾³ýµÄ½ÚµãµØÖ·
#define DELETE_NODE_DATA_LEN           (PERIPHERAL_CMD_LEN + ADDRESS_LEN)
// É¾³ý½ÚµãÃüÁîÓ¦´ð£¬°üº¬ 1×Ö½ÚÃüÁîÂë
#define DELETE_NODE_ACK_DATA_LEN       (PERIPHERAL_CMD_LEN)
// É¾³ý´æ´¢µÄ½ÚµãÐÅÏ¢ÃüÁî£¬°üº¬ 1×Ö½ÚÃüÁîÂë
#define DELETE_NODE_INFO_DATA_LEN      (PERIPHERAL_CMD_LEN)
// ±¾µØ¸´Î»ÃüÁî£¬°üº¬ 1×Ö½ÚÃüÁîÂë
#define LOCAL_RESET_DATA_LEN           (PERIPHERAL_CMD_LEN)

/******************************************************************************/

typedef struct
{
    uint16_t node_addr;
    uint16_t elem_count;
    uint16_t net_idx;
    uint16_t retry_cnt : 12,
        fixed : 1,
        blocked : 1;

} node_t;

typedef union
{
    struct
    {
        uint8_t cmd;                            /* ÃüÁîÂë CMD_PROVISION */
        uint8_t net_key[PROVISION_NET_KEY_LEN]; /* ÍøÂçÃÜÔ¿ */
        uint8_t addr[ADDRESS_LEN];              /* ÍøÂçµØÖ· */
    } provision;                                /* ÅäÍøÃüÁî */
    struct
    {
        uint8_t cmd;                /* ÃüÁîÂë CMD_DELETE_NODE */
        uint8_t addr[ADDRESS_LEN];  /* ½ÚµãµØÖ· */
    } delete_node;                  /* É¾³ý½ÚµãÃüÁî */
    struct
    {
        uint8_t cmd;                /* ÃüÁîÂë CMD_DELETE_NODE_ACK */
    } delete_node_ack;              /* É¾³ý½ÚµãÃüÁîÓ¦´ð */
    struct
    {
        uint8_t cmd;                /* ÃüÁîÂë CMD_DELETE_NODE_INFO */
    } delete_node_info;             /* É¾³ý´æ´¢µÄ½ÚµãÐÅÏ¢ÃüÁî */
    struct
    {
        uint8_t cmd;                /* ÃüÁîÂë CMD_LOCAL_RESET */
    } local_reset;                  /* ±¾µØ»Ö¸´³ö³§ÉèÖÃÃüÁî */
    struct
    {
        uint8_t buf[20]; /* ½ÓÊÕÊý¾Ý°ü*/
    } data;
}app_mesh_manage_t;

void App_Init(void);

/******************************************************************************/

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
