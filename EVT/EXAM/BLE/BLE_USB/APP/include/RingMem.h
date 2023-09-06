/********************************** (C) COPYRIGHT *******************************
 * File Name          : RingMem.h
 * Author             : WCH
 * Version            : V1.4
 * Date               : 2019/12/20
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

#ifndef RINGMEM_H
#define RINGMEM_H

#ifndef SUCCESS
  #define SUCCESS    0
#endif
#ifndef ENABLE
  #define ENABLE    1
#endif
#ifndef DISABLE
  #define DISABLE    0
#endif

/*********************************************************************
 * TYPEDEFS
 */

typedef unsigned char  uint8_t;
typedef unsigned short uint16;
typedef unsigned long  uint32_t;
typedef int (*RingMemProtection_t)(uint8_t enable);

//Buffer structure
typedef struct
{
    uint8_t volatile *pData;       //Buffer first address
    uint8_t volatile *pWrite;      //Pointer
    uint8_t volatile *pRead;       //Pointer
    uint8_t volatile *pEnd;        //The last address of the buffer area
    uint32_t volatile RemanentLen; //Remaining space size
    uint32_t volatile CurrentLen;  //Already used space
    uint32_t volatile MaxLen;      //Total space size
} RingMemParm_t;

/*********************************************************************
 * Global Variables
 */

/*********************************************************************
 * FUNCTIONS
 */

extern void RingMemInit(RingMemParm_t *Parm, uint8_t *StartAddr, uint32_t MaxLen, RingMemProtection_t Protection);

extern uint8_t RingMemWrite(RingMemParm_t *Parm, uint8_t *pData, uint32_t len);

extern uint8_t RingMemRead(RingMemParm_t *Parm, uint8_t *pData, uint32_t len);

extern uint8_t RingMemCopy(RingMemParm_t *Parm, uint8_t *pData, uint32_t len);

extern uint8_t RingMemDelete(RingMemParm_t *Parm, uint32_t len);

extern uint8_t RingAddInStart(RingMemParm_t *Parm, uint8_t *pData, uint32_t len);

extern uint8_t RingReturnSingleData(RingMemParm_t *Parm, uint32_t Num);
/*********************************************************************
*********************************************************************/

#endif
