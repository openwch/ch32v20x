/********************************** (C) COPYRIGHT *******************************
 * File Name          : M5_Operation_and_Display.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Definition of relevant operation parameters such as buttons and displays
                        Definition of acceleration and deceleration parameters
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __MOTOR_OPERATION_AND_DISPLAY_H
#define __MOTOR_OPERATION_AND_DISPLAY_H

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
typedef enum {OFF = 0, ON = !OFF} LightStatus;

typedef struct
{
    uint8_t  FlashExcep;
    uint16_t FlashData[2];
    volatile uint32_t MemAddress;
    volatile FLASH_Status FLASHStatus;
    volatile TestStatus MemoryProgramStatus;

} Flash_Operation_Type;

typedef struct
{
    GPIO_TypeDef *Group;            
    uint16_t     Pin;               
} LEDGPIO_Type;
typedef struct
{
    uint16_t FlickInterval;        
    uint16_t FlickCnt;              
    uint8_t  FlickCtl;              
    LightStatus  LightFlag;            
} LED_Type;

typedef struct
{
    LED_Type LED4;
    LED_Type LED5;
    LED_Type LED6;
    LED_Type LED7;
    LED_Type LED8;
    LED_Type LED9;
    LEDGPIO_Type Pin1;
    LEDGPIO_Type Pin2;
    LEDGPIO_Type Pin3;
} Led_Manager_Type;//LED管理

/* Exported constants -------------------------------------------------------*/
/*********************** 转速操作定义 ******************************/
#define STARTUP_DIRECTION        1                      
#define SPEED_ADJUST_STEP       _IQ(0.05)               
#define MAX_ANGSPEED_POS_M      _IQ(1.0)               
#define MAX_ANGSPEED_NEG_M      _IQ(-1.0)               
#define MIN_ANGSPEED_POS_M      _IQ(0.025)              
#define MIN_ANGSPEED_NEG_M      _IQ(-0.025)             
#define SPEED_RAMP_FREQ_M        SPEED_LOOP_CAL_FREQ    
#define ACCE_TIME_M              0.5                    
#define DECE_TIME_M              0.4                    
#define POWEROFF_DECE_TIME_M     0.15                  

#define SPEEDPOINT1              _IQ(0.7965085)          
#define SPEEDPOINT2              _IQ(0.8545455)            
#define SPEEDPOINT3              _IQ(0.9094380)          
//#define SPEEDPOINT3              _IQ(0.92744135)         
//#define SPEEDPOINT3              _IQ(0.9383524)          
//#define SPEEDPOINT3              _IQ(0.954719)          

#define SPEEDPOINT_RANK          3                      

#define PRECHAGE_TIME            0.05                   

#define FAST_DECE_SPEED          _IQ(0.6)               
#define BREAKSPEED              _IQ(0.65)               
#define BREAKTIME                0.25                   
#define START_FINISH_SPEED      (FAST_DECE_SPEED +_IQ(0.055)) 


#define LED_PROCESS_FREQ            SPEED_LOOP_CAL_FREQ 
#define LED_FLICKER_TIME           0.5                 

#define OPERATION_CAL_FREQ      SPEED_LOOP_CAL_FREQ 
#define STOP_WAITING_TIME       0.5                 
#define FAULT_HOLDING_TIME      0.5                 
#define SYS_RESET_TIME          0.025               

#define POWERVERY_FREQ     SPEED_LOOP_CAL_FREQ  
#define POWERON_VOLT       _IQ(0.85)            
#define POWERON_TIME       0.1                  
#define POWEROFF_VOLT1     _IQ(0.7)             
#define POWEROFF_VOLT2     _IQ(0.5)            
#define POWERVERY_DCVOLTAVERSIZE    4         

#define VDCLPF_SPEEDLIM_CF_M         5                              
#define VDCLPF_SPEEDLIM_PERIOD_M    _IQ(1.0)/SPEED_LOOP_CAL_FREQ    
#define VDCLPF_SPEEDLIM_WcT_M       _IQmpyI32(_IQmpy(VDCLPF_SPEEDLIM_PERIOD_M,PI_2_Q24),VDCLPF_SPEEDLIM_CF_M)
#define VDC_SPEEDLIM_BEGIN          _IQ(0.89)      
#define VDC_SPEEDLIM_RELEASE        _IQ(0.915)            
#define VDC_SPEEDLIM_ADJ            _IQ(1.1)             

#define KEY_CHECK_FREQ          SPEED_LOOP_CAL_FREQ 
#define EEWRITE_WAITING_TIME   0.005                
#define EEWRITE_CYCLE          2                    

#define SHKEY_CHECK_INTERVAL         0.040  
#define SHKEY_EFFECTIVE_TIME         0.010  
#define SHKEY_INEFFECTIVE_TIME       0.035  

#define TCKEY_EFFECTIVE_TIME         0.010  
#define TCKEY_INEFFECTIVE_TIME       0.035  

#define SRKEY_EFFECTIVE_TIME         0.1    
#define SRKEY_INEFFECTIVE_TIME       0.1    

#define TKEY_RANK_SET               3      
#define TKEY_SHORT_TIME_UP          0.5     
#define TKEY_SHORT_TIME_DOWM        0.1    
#define TKEY_LONG_TIME              2       

#define ACKEY_CHECK_INTERVAL         0.040  
#define ACKEY_EFFECTIVE_TIME         0.005  
#define ACKEY_INEFFECTIVE_TIME       0.035  

#define ZEROCROSS_FREQ              100           
#define ZEROCROSS_VERYFREQ          FREQ_SVPWM 
#define OC_ZEROCROSS_VERYTIME          5   
#define TR_ZEROCROSS_VERYTIME          7     

#define ZEROCROSSLOST_VERYTIME      0.05        
#define ZEROCROSSLOST_VERYFREQ      FREQ_SVPWM  

#define EVENT_CHECK_FREQ    SPEED_LOOP_CAL_FREQ 
#define EVENT_10ms          0.01    
#define EVENT_1ms           0.001   
#define EVENT_2s            2.00    

#define TRIAC_ENABLEWAIT             1.5        
#define TRIAC_OFF()                (GPIO_SetBits(GPIOB, GPIO_Pin_7))
#define TRIAC_ON()                 (GPIO_ResetBits(GPIOB, GPIO_Pin_7))

#define TRIAC_TIME                  660         
#define TRIAC_CONTOROL_PERIOD       20          

#ifdef ZEROCROSSOVER_OC       
#define TRIAC_TIME_TO_TRIGGER       0.005      
#else                         
#define TRIAC_TIME_TO_TRIGGER       0.003       
#endif

#define DUTYMAX_T1S1         13       
#define DUTYMIN_T1S1         3       
#define DUTYDEFAULT_T1S1     11       

#define DUTYMAX_T1S2         17      
#define DUTYMIN_T1S2         3       
#define DUTYDEFAULT_T1S2     14      

#define DUTYMAX_T1S3         19      
#define DUTYMIN_T1S3         3     
#define DUTYDEFAULT_T1S3     16      


#define DUTYMAX_T2S1         20      
#define DUTYMIN_T2S1         3       
#define DUTYDEFAULT_T2S1     16      
#define DUTYMAX_T2S2         20     
#define DUTYMIN_T2S2         3       
#define DUTYDEFAULT_T2S2     18     

#define DUTYMAX_T2S3         23      
#define DUTYMIN_T2S3         3       
#define DUTYDEFAULT_T2S3     21      


#define DUTYMAX_T3S1         22      
#define DUTYMIN_T3S1         3       
#define DUTYDEFAULT_T3S1     19      

#define DUTYMAX_T3S2         24      
#define DUTYMIN_T3S2         3       
#define DUTYDEFAULT_T3S2     20      

#define DUTYMAX_T3S3         27      
#define DUTYMIN_T3S3         3       
#define DUTYDEFAULT_T3S3     23     


#define NUM50HZ_DATA      (u8)3
#define NUM100HZ_DATA     (u8)6

#define PSC_LED_INTERVAL        0.2                     
#define PSC_LED_INTERVAL_TOTAL  5.0                       
#define PSC_LED_INTERVAL_NUM    3                       
#define PSC_LED_LAST            3.0                    
#define PSC_FREQ                SPEED_LOOP_CAL_FREQ     

#define PT_RS               _IQ18(10.0)     
#define TEMP_AVER_SIZE      (u32)8          

//34~289
#define  DUTY_LIM_TABLE {\
0x00643,0x006D0,0x00766,0x00803,0x008A9,0x00957,0x00A0E,0x00ACE,\
0x00B96,0x00C68,0x00D42,0x00E26,0x00F14,0x0100B,0x0110C,0x01217,\
0x0132B,0x0144A,0x01574,0x016A7,0x017E5,0x0192E,0x01A81,0x01BE0,\
0x01D49,0x01EBD,0x0203C,0x021C6,0x0235C,0x024FD,0x026A9,0x02860,\
0x02A23,0x02BF2,0x02DCC,0x02FB1,0x031A2,0x0339F,0x035A8,0x037BC,\
0x039DB,0x03C07,0x03E3E,0x04081,0x042CF,0x0452A,0x0478F,0x04A01,\
0x04C7E,0x04F06,0x0519B,0x0543A,0x056E5,0x0599C,0x05C5D,0x05F2A,\
0x06202,0x064E5,0x067D4,0x06ACD,0x06DD1,0x070E0,0x073F9,0x0771D,\
0x07A4B,0x07D84,0x080C7,0x08414,0x0876B,0x08ACC,0x08E37,0x091AB,\
0x09529,0x098AF,0x09C3F,0x09FD8,0x0A37A,0x0A725,0x0AAD8,0x0AE93,\
0x0B256,0x0B622,0x0B9F5,0x0BDD0,0x0C1B2,0x0C59B,0x0C98C,0x0CD83,\
0x0D181,0x0D585,0x0D990,0x0DDA0,0x0E1B7,0x0E5D3,0x0E9F4,0x0EE1B,\
0x0F246,0x0F676,0x0FAAB,0x0FEE4,0x10321,0x10762,0x10BA6,0x10FEE,\
0x11439,0x11886,0x11CD7,0x12129,0x1257E,0x129D5,0x12E2D,0x13287,\
0x136E2,0x13B3E,0x13F9B,0x143F8,0x14855,0x14CB3,0x15110,0x1556C,\
0x159C8,0x15E23,0x1627C,0x166D4,0x16B2A,0x16F7E,0x173D0,0x17820,\
0x17C6D,0x180B7,0x184FD,0x18941,0x18D80,0x191BC,0x195F4,0x19A27,\
0x19E56,0x1A280,0x1A6A5,0x1AAC5,0x1AEDF,0x1B2F4,0x1B703,0x1BB0B,\
0x1BF0E,0x1C30A,0x1C6FF,0x1CAED,0x1CED5,0x1D2B5,0x1D68D,0x1DA5E,\
0x1DE27,0x1E1E8,0x1E5A1,0x1E951,0x1ECF9,0x1F098,0x1F42F,0x1F7BC,\
0x1FB40,0x1FEBB,0x2022C,0x20594,0x208F2,0x20C46,0x20F90,0x212D0,\
0x21605,0x21931,0x21C52,0x21F68,0x22273,0x22574,0x2286A,0x22B55,\
0x22E35,0x23109,0x233D3,0x23691,0x23944,0x23BEC,0x23E88,0x24119,\
0x2439E,0x24617,0x24885,0x24AE8,0x24D3E,0x24F89,0x251C9,0x253FC,\
0x25624,0x25841,0x25A51,0x25C56,0x25E4F,0x2603D,0x2621F,0x263F6,\
0x265C1,0x26780,0x26934,0x26ADD,0x26C7A,0x26E0C,0x26F93,0x2710F,\
0x27280,0x273E6,0x27541,0x27691,0x277D6,0x27911,0x27A42,0x27B68,\
0x27C84,0x27D96,0x27E9E,0x27F9C,0x28090,0x2817A,0x2825C,0x28333,\
0x28402,0x284C8,0x28585,0x28639,0x286E5,0x28788,0x28823,0x288B6,\
0x28942,0x289C5,0x28A42,0x28AB7,0x28B25,0x28B8C,0x28BEC,0x28C46,\
0x28C9A,0x28CE7,0x28D2F,0x28D71,0x28DAE,0x28DE6,0x28E18,0x28E46,\
0x28E70,0x28E95,0x28EB6,0x28ED3,0x28EEC,0x28F02,0x28F15,0x28F26};
/* Exported variables -------------------------------------------------------*/
extern Speedrampctr_Type    SpeedRamp_M;        
extern Powercontrol_Type    Powerctr_M;         
extern Led_Manager_Type    LED_M;               
extern Key_Manager_Type    Key_M;               
extern Eventmanager_Type   Emanager_M;         
extern Operation_Type      Operation_M;         
extern Temperature_ctl_Type Tempctl_M;          
extern uint8_t Keymembuff[EEWRITE_CYCLE];       
extern TRIAC_ctl_Type Triac_ctl;

extern u16 Temp_PhaseDuty[SPEEDPOINT_RANK][TKEY_RANK_SET+1];


/* Exported macro -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------ */
void Motor_Operation_Process(void);

void Speedrampctr_Parameter_Define(Speedrampctr_Type *Speed_Ramp);

void Speedrampctr_Start_Init_M(Speedrampctr_Type *Speed_Ramp);

void Speed_Set(Speedrampctr_Type *Speed_Ramp);

void Poweroperation_Initial(Powercontrol_Type *PowerOper);

void Poweroperation_Process(Speedrampctr_Type *SpeedRamp,ADCStruc_Type *ADCStruc,Powercontrol_Type *PowerOper,\
                            Runningstatus_Type *Runningstatus,PID_Structure_Type *Speedpid,_iq24 Speedact);

void KeyandLED_IOInit(void);

void KeyandLED_Parameter_Define(Key_Manager_Type *KeyM,Led_Manager_Type *LEDM);

void KeyandLED_Status_Ini(Key_Manager_Type *KeyM,Led_Manager_Type *LEDM);

void Tempkeycheck(Key_Manager_Type *KeyM);

void Speedkeycheck_Hold(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp);

void Speedkeycheck_Recover(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp);

void Coldkeycheck(Key_Manager_Type *KeyM);

void Key_Memwrite(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp,Flash_Operation_Type *FlashM);

void Key_Memread(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp,Flash_Operation_Type *FlashM);

void Fault_LEDflicker(Led_Manager_Type *LED_M);

void Time_Delay_Process(void);

void Time_Wait(uint16_t);

void Time_Delay_Set(uint16_t Delayset, uint16_t *Delaycounter);

Truth_Verify_Type Time_Delay_Elapse(uint16_t Delaycounter);

void Operation_Parameter_Define(Operation_Type *Operastruc);

void Operation_Status_Init(Operation_Type *Operastruc);

void Eventmanager_Initial(Eventmanager_Type *EventM);

void AC_Zerocross_Detect(Key_Manager_Type *KeyM);

void AC_Zerocross_Detect_OC(TRIAC_ctl_Type *TriacM);

void AC_Zerocross_Detect_TR(TRIAC_ctl_Type *TriacM);

void ACZerocross_Process(Temperature_ctl_Type *TempM,Key_Manager_Type *KeyM,TRIAC_ctl_Type *TriacM);

void Event_10ms_Process(Eventmanager_Type *EventM);

void Event_1ms_Process(Eventmanager_Type *EventM);

void Event_2s_Process(Eventmanager_Type *EventM);

void Triac_Parameter_Define(TRIAC_ctl_Type *TriacM,Temperature_ctl_Type *TempM);

void Triac_Status_Init(TRIAC_ctl_Type *TriacM,Temperature_ctl_Type *TempM);

void Temp_Set(Key_Manager_Type *KeyM,Speedrampctr_Type *Speed_Ramp,TRIAC_ctl_Type *TriacM);

void Triac_Control_Process(TRIAC_ctl_Type *TriacM);

void Temp_Ramp_Process(Temperature_ctl_Type *TempM);

void TempInit_Measure(Temperature_ctl_Type *TempM,ADC_TypeDef *ADCx);

void TriacPhAngle_Control_Process(Temperature_ctl_Type *TempM,Key_Manager_Type *KeyM,TRIAC_ctl_Type *TriacM);

void TriacZero_50HZControl_Process(Temperature_ctl_Type *TempM,Key_Manager_Type *KeyM,TRIAC_ctl_Type *TriacM,Speedrampctr_Type *Speed_Ramp);

void Triac_Timeoff(TRIAC_ctl_Type *TriacM);

void ACKEY_CHECK(Key_Manager_Type *KeyM);

void PSC_Process(Operation_Type *Operation);

void Led_Operation(Led_Manager_Type *LEDM);

void SpeedLed_Set(Speedrampctr_Type *Speed_Ramp,Led_Manager_Type *LEDM);

void TempLed_Set(Key_Manager_Type *KeyM,Led_Manager_Type *LEDM);

void LED_OFF(Led_Manager_Type *LEDM);

void LED_Flicker(LED_Type *LedM);

uint16_t Duty_lim(TRIAC_ctl_Type *TriacM,_iq24  value);
#endif
