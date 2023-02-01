/********************************** (C) COPYRIGHT *******************************
 * File Name          : eth.h
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2021/11/18
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef eth_H
#define eth_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// ETH Task Events
#define ETH_SENG_DATA_EVENT         1<<1

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * eth_init
 */
extern void eth_init(void);

/*
 * eth_process
 */
extern void eth_process(void);

/*
 * eth_send
 */
uint8_t eth_send( uint8_t *SendBuf, uint32_t l);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
