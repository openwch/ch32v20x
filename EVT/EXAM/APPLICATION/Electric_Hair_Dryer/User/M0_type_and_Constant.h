/********************************** (C) COPYRIGHT *******************************
 * File Name          : Mo_type_and_Constant.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/04/23
 * Description        : Data type definition, constant definition, state definition. 
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __M0_TYPE_AND_CONSTANT_H
#define __M0_TYPE_AND_CONSTANT_H

/* Includes -----------------------------------------------------------------*/
#include "stdint.h"
#include "IQmath_RV32.h"

/* Exported types -----------------------------------------------------------*/
//电压电流结构体原型定义
typedef struct
{
  _iq24 PhaseA;
  _iq24 PhaseB;
  _iq24 PhaseC;
} Component_3S_Type;

typedef struct
{
  _iq24 Alpha;
  _iq24 Beta;
} Component_2S_Type;

typedef struct
{
  _iq24 cd;
  _iq24 cq;
  _iq24 peak;
} Component_2R_Type;

typedef enum
{
  IDLE, INIT, DIRCHECK ,PRESTART, POSITION, START , RUN, STOP, BRAKE, WAIT, FAULT
} Runningstatus_Type;

typedef enum
{
 INITIAL, INJECTION, RAMP
}Motor_Startup_Status_Type;

typedef struct
{
  _iq24 Cos;
  _iq24 Sin;
}Struct_Cossin_Type;

typedef enum {FALSE = 0, TRUE = !FALSE} Truth_Verify_Type;

typedef enum {DIRDOWN = 0, DIRUP = !DIRDOWN} Direction_Type;

typedef struct
{
    uint16_t CalFreq;      
    _iq24 CalT;             
    _iq24 Kpreal;           
    _iq24 Kireal;          
    _iq24 Kdreal;           
    uint16_t Bw;            

    _iq24 Kp;               
    _iq24 Kpadj;            
    _iq24 Ki;              
    _iq24 Kiadj;            
    _iq24 Kd;               
    _iq24 Kdadj;            

    _iq24 Lower_Lim;        
    _iq24 Upper_Lim;        

    _iq24 Integral;         
    _iq24 Differential;     

    _iq24 Err_New;          
    _iq24 Err_Prev1;       
    _iq24 Err_Prev2;        
    _iq24 Output_New;       
    _iq24 Output_Prev;      

    _iq24 Deltaoutput;      
    _iq24 Deltalim;         

    _iq24 Propotion;        
    _iq24 Integer;          
    _iq24 Difference;       
} PID_Structure_Type;

typedef struct
{
    _iq24 Out_Pre;          
    _iq24 Out_New;         
    _iq24 Input_Coef;       
    _iq24 Output_Coef;      
} LPF_First_Order_Type;

typedef struct
{
    _iq24 In_Pre;          
    _iq24 Out_Pre;          
    _iq24 Out_New;          
    _iq24 Input_Coef;      
    _iq24 Output_Coef;      
} HPF_First_Order_Type;

typedef struct
{
    _iq24 Out_Pre1;         
    _iq24 Out_Pre2;         
    _iq24 Out_New;          

    _iq24 Input_Coef;       
    _iq24 Output_Coef1;     
    _iq24 Output_Coef2;     
} LPF_Second_Order_Type;

typedef struct
{
    _iq24 In_Pre1;         
    _iq24 In_Pre2;          
    _iq24 Out_Pre1;         
    _iq24 Out_Pre2;         
    _iq24 Out_New;          

    _iq24 Input_Coef;       
    _iq24 Output_Coef1;     
    _iq24 Output_Coef2;     
} HPF_Second_Order_Type;

typedef struct
{
    _iq24 In_Pre1;         
    _iq24 In_Pre2;         
    _iq24 Out_Pre1;         
    _iq24 Out_Pre2;         
    _iq24 Out_New;          

    _iq24 Input_Coef;       
    _iq24 Output_Coef1;     
    _iq24 Output_Coef2;     
} BPF_Second_Order_Type;

typedef struct
{
    uint16_t CalFreq;       
    _iq24 CalT;             
    int32_t K1CerrC;       
    int32_t K2CerrE;        

    _iq24 kCC;              
    _iq24 kVC;              
    _iq24 kEC;              
    _iq24 kCeC;             
    _iq24 kSC;              
    _iq24 kEE;              
    _iq24 kCE;              
    uint16_t kCE_Adj;       
    uint8_t Caltimes;
} LBG_Paramter_Type;

typedef struct
{
    _iq24 K_Volt;          
    _iq24 K1_Curr;          
    _iq24 K2_Curr;         
    _iq24 K3_Curr;         
} LBG_EMF_Init_Type;

typedef struct
{
    _iq24 EalphaNew;       
    _iq24 EalphaPrev;       
    _iq24 EbetaNew;         
    _iq24 EbetaPrev;        
    _iq24 Ealpha_wphase;    
    _iq24 Ebeta_wphase;     
    _iq24 EUlim;            
    _iq24 ELlim;            

    _iq24 CalphaNew;        
    _iq24 CalphaPrev;       
    _iq24 CbetaNew;         
    _iq24 CbetaPrev;        
    _iq24 CUlim;            
    _iq24 CLlim;            

    _iq24 K_WetoPos;        

    _iq24 ThetaErr;         
    _iq24 PosiElecPU;       
    _iq24 PosiElecPU_Pre;   

    _iq24 Angspeed_Inst;    
    _iq24 Angspeed_Aver1;   
    _iq24 Angspeed_Aver2;   
    _iq24 Angspeed_Filter;  
    _iq24 Direction;        
    _iq24 Angspeed_Aver20;  
    _iq24 Deltaspeed;       

    _iq24 EmfPeak;          
    _iq24 EmfErrBandup;     
    _iq24 EmfErrBanddown;   
    _iq24 EmfErr;           
    _iq24 Speedband;        

    LPF_First_Order_Type    SpeedLPF1st_M;  
    Struct_Cossin_Type      Triangle;       

    _iq24 PosiElecPU_Cali;  
    _iq24 PosiElecPU_Com;   
    _iq24 Kposcali;         
    uint8_t Posicom_Enable; 

    LPF_First_Order_Type EMFLPF1st_M;   
    Truth_Verify_Type Inimark;          

    uint8_t Obs_Calcnt; 
} LBG_Cal_Structure_Type; 

typedef struct
{
    int32_t Kpreal;         
    int32_t Kireal;         
    _iq24 Kp;               
    _iq24 Ki;               
    _iq24 Error_New;        
    _iq24 Error_Prev;       
    _iq24 Output_New;       
    _iq24 Output_Prev;      
    _iq24 Lower_Lim;        
    _iq24 Upper_Lim;        
}Speed_PLL_Type;

typedef struct
{
    uint8_t Enable;         
    int32_t Gain;           
    _iq24 Err_CoefT;        
    _iq24 EMF_CoefT;        
    _iq24 EMF_Adj;          
    _iq24 Alpha_Err;        
    _iq24 Alpha_Pre;        
    _iq24 Alpha_New;        
    _iq24 Beta_Err;         
    _iq24 Beta_Pre;         
    _iq24 Beta_New;         
} Observer_Adaptor_type;

typedef struct
{
    _iq24 Vpolar;           
    _iq24 Vmag;             
    _iq24 Vmagmax;          
    _iq24 Vmagmin;          
    uint32_t Counter;       

    uint16_t TimeStep;      
    uint16_t TimeEffect;    
    _iq24 AngleInit;        
    _iq24 AngleStep;       
    uint8_t SectorSet;      
    uint8_t SectorAct;      
    _iq24 AngleInj;        
    _iq24 Position;        
    _iq24 Posi_Cali;        
    _iq24 CurrMax;          
    uint8_t SectorMax;      
    Truth_Verify_Type Finish;   
    _iq24 Start_id;         
    _iq24 Start_iq;         
    uint8_t Verifytime;     
    uint8_t Cycletime;      
    uint8_t Sectorbuff;     
    uint8_t Cyclemax;       
    Truth_Verify_Type Uniform;  

    Component_3S_Type   Curr3S;     
    Component_2S_Type   Curr2S;     
    Component_2R_Type   Curr2R;     
    Struct_Cossin_Type Triangle;    
    Component_2R_Type   VoltRef2R;  
    Component_2R_Type   VoltRef2RC; 
    Component_2S_Type   VoltRef2SC; 
    _iq24 Phaseloss_Currthre;       
    uint8_t Phaseloss_Cntthre;      
    uint8_t Phaseloss_Cnt;         
}HFVQI_Start_Type;

typedef struct
{
    Truth_Verify_Type Overspeed;    
    Truth_Verify_Type Underspeed;   
    Truth_Verify_Type Reliable;     
    Truth_Verify_Type Convergence;  
    Truth_Verify_Type Stable;      
    Truth_Verify_Type Timeout;      
    Truth_Verify_Type Stall;        
    Truth_Verify_Type Reverse;    

    _iq24 OverspeedThresh;          
    uint16_t Overspeed_CNT;         
    uint16_t OverspeedcntThresh;    

    _iq24 UnderspeedThresh;         
    uint16_t Underspeed_CNT;        
    uint16_t UnderspeedcntThresh;   

    _iq24 VarianceThresh;           
    _iq24 ConvergeUpper;            
    _iq24 ConvergeLower;            
    uint16_t Converged_CNT;         
    uint16_t ConvergeCntThresh;     
    _iq24 ConvergeEmfBandup;        
    _iq24 ConvergeEmfBanddown;      

    uint16_t Unstable_CNT;          
    uint16_t UnstableCntThresh;     
    _iq24 UnstableSpeedThresh;      

    _iq24 StallSpeedBand;           
    _iq24 StallSpeed;               
    _iq24 StallCurr;                
    uint16_t Stall_CNT;             
    uint16_t StallCntThresh;        

    uint16_t Reverse_CNT;           
    uint16_t ReverseCntThresh;      
    _iq24 ReverseSpeedThresh;       
}Observer_Status_Type;


//SVPWM计算结构体
typedef struct
{
    _iq18 BusVolt;              
    _iq16 T_IQ16;               
    uint16_t T;                 
    uint16_t HALF_T;            
    uint16_t Quard_T;           
    _iq16 coef1;                
    _iq16 coef2;                
    _iq16 hx;                   
    _iq16 hy;
    _iq16 hz;
    uint8_t  sector;            
    _iq16 ht1;                  
    _iq16 ht2;                  
    uint16_t tv1;               
    uint16_t tv2;               
    uint16_t tv1half;           
    uint16_t tv2half;           
    uint16_t tvhalf;            
    uint16_t tv0quard;          
    uint16_t tv0cali;           
    uint16_t cc1;               
    uint16_t cc2;               
    uint16_t cc3;               
    uint16_t cc1Load;           
    uint16_t cc2Load;           
    uint16_t cc3Load;           
    uint16_t cc1Loadup;         
    uint16_t cc2Loadup;         
    uint16_t cc3Loadup;         
    uint16_t cc4Load;           
    uint8_t VoltLimMark;        
    _iq24 Maxmodulesqure;       
    _iq24 Maxmodule;            

    _iq16 Ualpha_coef1;         
    _iq16 Ubeta_coef2;          

    uint16_t PWMUcali1;         
    uint16_t PWMUcali2;         
    uint16_t PWMVcali1;         
    uint16_t PWMVcali2;         
    uint16_t PWMWcali1;         
    uint16_t PWMWcali2;        

} SVPWM_1R_Type;

typedef struct
{
    int32_t Value1;
    int32_t Value2;
}ADC_Result_Buff_Type;


typedef struct
{
    ADC_Result_Buff_Type  ADC_Result;  
    _iq16 RatedCurr_ConvInv;            
    uint16_t OffsetValue1;             
    uint16_t OffsetValue2;              
    uint8_t AdcChannel1;                
    uint8_t AdcChannel2;               

    _iq24 DCVoltPres;                   
    _iq24 DCVoltAver;                   

    LPF_First_Order_Type    VdcLPF1st;  
    _iq24 DCVolt_Filter;                
    _iq18 DCVoltRatedADC;               
    _iq18 DCVoltRatedADCInv;            
    _iq24 DCVoltAverSizeInv1;          
    _iq24 DCVoltAverSizeInv2;           
    _iq24 DCVoltcali;                   
} ADCStruc_Type;

typedef struct
{
    uint16_t Cali1;
    uint16_t Cali2;
    uint16_t Cali3;
    uint16_t Cali4;
    uint16_t Cali5;
    uint16_t Cali6;
}PWMCali_Buff_Type;

typedef struct
{
    uint16_t Trig1;
    uint16_t Trig2;
    Truth_Verify_Type  Trig1_limmark;
    Truth_Verify_Type  Trig2_limmark;
}ADC_Trig_Buff_Type;

typedef struct
{
    uint16_t Trig1;
    uint16_t Trig2;
    uint16_t Trig3;
    uint16_t Trig4;
    uint16_t Trig5;
    uint16_t Trig6;
}DMA_Trig_Buff_Type;


typedef struct
{
    PWMCali_Buff_Type           Cali_Buff;
    ADC_Trig_Buff_Type          ADC_Trig;
    DMA_Trig_Buff_Type          DMA_Trig;

    uint8_t  CaliMark;             
    uint16_t CaliValue1;            
    uint16_t CaliValue2;            
    uint16_t ADCTimeMin;            
    uint16_t ADCTimeMinDouble;      
    uint16_t ADCTimeMinHalf;        
    uint16_t Currconverttime;       
} PWMCali_Type;

typedef struct
{
    _iq24 Inv1;         
    _iq24 Inv2;         
    uint8_t   Length;   
    uint8_t   Index;    
} BuffPara_Type;

typedef struct
{
    _iq24 Kcom;                         
    _iq24 Wecom;                       
    _iq24 Wecomupper;                   
    _iq24 Wecomlower;                   
    _iq24 Powerinst;                    
    _iq24 Powerfilter;                  
    HPF_First_Order_Type Power_HPF1st;  
}IF_Vibrationcom_type;

typedef struct
{
    Component_2R_Type Curr_2R;      //电流闭环启动观测两相旋转坐标系电流，用于闭环的初始化
    Component_2R_Type VoltRef_2R;   //电流闭环启动观测两相旋转坐标系电压给定，用于闭环的初始化
    Struct_Cossin_Type Triangle;    //电流闭环启动观测两相旋转坐标三角函数，用于闭环的初始化

    _iq24 ForceInjCurrStep;         //强制定位电流上升步长(unit-A/s_per)
    _iq24 ForceInjCurrRef;          //强制定位电流实时值(unit-A_per)
    _iq24 ForceInjCurrTarget;       //强制定位电流目标值(unit-A_per)
    _iq24 ForceInjCurrAngle;        //强制定位电流矢量位置角(unit-PU)
    uint32_t ForceInjCounter;       //强制定位定时计数器
    uint32_t ForceInjStage;         //强制定位结束判定时间

    _iq24 RampInjCurrTarget;        //电流闭环启动电流目标值(unit-A_per)
    _iq24 RampInjCurrStep;          //电流闭环启动电流上升步长(unit-A_per)
    _iq24 RampInjCurrRef;           //电流闭环电流给定实时值(unit-A_per)
    _iq24 RampInjSpeedTarget;       //电流闭环启动转速目标值(unit-rad/s_per)
    _iq24 RampInjSpeedStep;         //电流闭环启动转速上升步长(unit-rad/s_per)
    _iq24 RampInjSpeedRef;          //电流闭环启动实时转速(unit-rad/s_per)
    _iq24 RampInjSpeedRef2;         //电流闭环启动实时转速_修正后(unit-rad/s_per)
    _iq24 RampInjAngle;             //电流闭环启动电流矢量角(unit-PU)
    _iq24 K_WetoPosi;               //同步电角速度转子电气角度积分折算系数
    uint32_t RampInjCounter;        //电流闭环启动定时计数器
    uint32_t RampInjStage1;         //电流闭环启动阶段1
    uint32_t RampInjStage2;         //电流闭环启动阶段2
    _iq24 CurrErrThresh;            //电流闭环启动阶段电流误差阈值(unit-A_per)
    _iq24 Speedadj;                 //电流闭环阶段环路切换转速调整值(unit-rad/s_per)
    _iq24 Curradj;                  //电流闭环阶段环路切换电流调整值(unit-A_per)
    Motor_Startup_Status_Type   Status; //电机启动状态

    IF_Vibrationcom_type IF_Vicom;  //电流闭环启动转速波动抑制结构体
    uint8_t InjStage;
}InjStart_Type;//电机启动控制结构体原型


//加减速控制结构体
typedef struct
{
    _iq24 Preramp;                  //斜坡前给定转速(unit-rad/s_per)
    _iq24 Aftramp;                  //斜坡后给定转速(unit-rad/s_per)
    _iq24 AccStep;                  //加速步长(unit-rad/s^2_per)
    _iq24 DecStep;                  //减速步长(unit-rad/s^2_per)
    _iq24 PoweroffDecStep;          //下电减速步长(unit-rad/s^2_per)
    _iq24 Aftrampcom;               //斜坡后给定补偿后转速(unit-rad/s_per)

    _iq24 Startfinishspeed;         //启动结束判定转速(unit-rad/s_per)
    _iq24 Faststopspeed;            //快速制动转速(unit-rad/s_per)
    Truth_Verify_Type Startfinish;  //启动完成标志

    Truth_Verify_Type BreakStop;    //定子短路制动停止状态
    Truth_Verify_Type Break;        //定子短路制动状态（TRUE-处于制动状态，FALSE-非制动状态）
    _iq24 Breakspeed;               //定子短路制动转速(unit-Rad/s_per)
    uint16_t Breakwait_cnt;         //定子短路制动计数器
    uint16_t Breakwait_cntset;      //定子短路制动定时设定值

    uint16_t Precharge_cnt;         //预充电计数器
    uint16_t Precharge_cntset;      //预充电定时设定值

    uint8_t  Speed_Rank;            //档位数
    uint8_t  Speed_adncnt;          //转速调整计数值
    _iq24 Speed1;                   //给定转速1(unit-Rad/s_per)
    _iq24 Speed2;                   //给定转速2(unit-Rad/s_per)
    _iq24 Speed3;                   //给定转速3(unit-Rad/s_per)
    uint8_t Stopmode;               //给停机模式，1：下电停机，2：按键停机

    LPF_First_Order_Type VdcLPF1st; //转速限制用母线电压滤波器
    _iq24 Vdcact;                   //滤波后母线电压(unit-V_per)
    _iq24 Voltbegin;                //限制起始电压(unit-V_per)
    _iq24 Voltrelease;              //限制取消电压(unit-V_per)
    Truth_Verify_Type Speedlimmark; //转速限制标志
    _iq24 Vdclimcoef;               //母线电压限制转速系数
    _iq24 Vdclimspeed;              //母线电压限制转速系(unit-rad/s_per)

    int8_t  Direction;             //转速方向
}Speedrampctr_Type;

//D轴电流处理结构体
typedef struct DCurrProcess
{
    _iq24 Target;               //d轴电流给定目标值1(unit-A_per)
    _iq24 Target1;              //d轴电流给定目标值1(unit-A_per)
    _iq24 Target2;              //d轴电流给定目标值2(unit-A_per)
    _iq24 Weakpoint;            //弱磁转速(unit-rad/s_per)
    _iq24 AdjustStep;           //调整步长(unit-A/s_per)
    _iq24 AdjustStep1;          //调整步长1(unit-A/s_per)
    _iq24 AdjustStep2;          //调整步长2(unit-A/s_per)
    uint8_t Stage;              //阶段，0-快减，1-慢减
}DCurrProcess_Type;

typedef struct
{
    _iq24 CurrThreshold;        //过流阈值(unit-A_per)
    uint16_t CntThreshold;      //满足条件计数阈值
    uint16_t Cnt;               //满足条件计数值
} InstPhaseOC_Type;             //瞬时过流保护结构体

typedef struct
{
    _iq24 OverThresh;           //过压阈值(unit-V_per)
    uint16_t OverCntThresh;     //过压保护计数阈值
    uint16_t OverCnt;           //过压保护计数值
    Truth_Verify_Type Overvolt;//过压标志

    _iq24 UnderThresh;          //欠压阈值(unit-V_per)
    uint16_t UnderCntThresh;    //欠压保护计数阈值
    uint16_t UnderCnt;          //欠压保护计数值
    Truth_Verify_Type Undervolt;//欠压标志

    uint16_t PeakOverCntThresh;     //峰值过压保护计数阈值
    uint16_t PeakOverCnt;           //峰值过压保护计数值
    uint16_t PeakOverDetectiontime; //峰值过压保护检测时间
    uint16_t PeakOverDetectiontCnt;  //峰值过压保护检测时间计数
} DCbusProtection_Type;         //母线电压保护结构体

typedef struct
{
    _iq24 CurrCal;              //过载计算电流(unit-A_per)
    _iq24 CurrThresh;           //过载阈值(unit-A_per)
    uint16_t CntThresh;         //满足条件计数阈值
    uint16_t Cnt;               //满足条件计数值
    _iq24 CalCoef;              //计算系数
} OverLoad_Type;                //过载保护结构体

typedef struct
{
    _iq24 CurrThresh;           //缺相电流阈值(unit-A_per)
    _iq24 CurrAabs;             //A相电流绝对值(unit-A_per)
    _iq24 CurrBabs;             //B相电流绝对值(unit-A_per)
    _iq24 CurrCabs;             //C相电流绝对值(unit-A_per)

    uint16_t CntThresh;         //缺相条件计数阈值
    uint16_t SiglephaseCntA;    //U相缺相满足条件计数值
    uint16_t SiglephaseCntB;    //V相缺相满足条件计数值
    uint16_t SiglephaseCntC;    //W相缺相满足条件计数值
    uint16_t MultiphaseCnt;     //多相缺相满足条件计数值

    BuffPara_Type  CurrBuffPara;  //电流平均值计算参数

} Phaseloss_Type;               //缺相保护结构体

typedef struct
{
    _iq18 TempConDuty;                  //温度折算系数
    _iq24 TempRatedADCInv;              //3.3vAD采样整数的倒数
    _iq24 TempPres;                     //ADC采样结果当前值
    _iq24 TempAver;                     //ADC采样结果均值
    _iq24 TempAverSizeInv1;             //温度均值计算系数1
    _iq24 TempAverSizeInv2;             //温度均值计算系数2

    _iq24    NTCOverThresh;             
    _iq24    NTCOpenThresh;             
    uint16_t NTCOverCntThresh;         
    uint16_t NTCOpenCntThresh;          
    uint16_t NTCOverCnt;               
    uint16_t NTCOpenCnt;               
    Truth_Verify_Type NTCOverStatus;    
    Truth_Verify_Type NTCOpenStatus;    
    Truth_Verify_Type NTCFlickerStatus; 
} Temperature_ctl_Type;

typedef struct
{
    Truth_Verify_Type  Zcrosseffective;    
    uint16_t  Zcrossset;                    
    uint16_t  Zcrosscnt;                    
    uint8_t ZcrossEvent;                    

    uint16_t  Zcrosslostcnt;                
    uint16_t  Zcrosslostset;                
    Truth_Verify_Type  Zcrosslost;          
    uint8_t Zcrosseffectivecnt;             
    uint16_t ZcrossDelayTrigset;            
    uint16_t ZcrossDelayTrigcnt;            

    uint16_t  SecondZcrossCnt;              
    uint16_t  SecondZcrossSet;             
    uint8_t   Prestatus;                    
    uint8_t   Aftstatus;                    
    Truth_Verify_Type   Inittatus;          
    Truth_Verify_Type   Risingseffective;   
    Truth_Verify_Type   SecondZsseffective; 
} Zcross_Type;


typedef struct
{
    volatile  uint16_t Cnt;
    volatile  uint16_t Duty_cycle;
    volatile  uint16_t Duty;
       uint16_t Period;
       uint16_t MaxPeriod;
     volatile  uint16_t Toncnt;
       uint16_t Tonset;
     volatile  uint8_t  TonEvent;            
       Truth_Verify_Type TonStatus;         
}Phase_Type;

typedef struct
{
        uint8_t  NumData;                    
        uint8_t  Serial_Num;                
        uint8_t  Serial_Nummax;                
        uint8_t  Serial_Nummin;                 
        uint8_t  ChangSerial_Num;            
        uint8_t DelayProcess_Flag;          
        volatile  uint16_t Cnt;
        volatile  uint16_t Duty_cycle;
        uint16_t Period;
        uint16_t HalfPeriod;
        uint16_t MaxPeriod;
        Truth_Verify_Type DutychangeFlag;  

        uint8_t Duty_TS;
        uint8_t Dutymax_T1S1;          
        uint8_t Dutymin_T1S1;           
        uint8_t Dutydefault_T1S1;      

        uint8_t Dutymax_T1S2;           
        uint8_t Dutymin_T1S2;           
        uint8_t Dutydefault_T1S2;       

        uint8_t Dutymax_T1S3;           
        uint8_t Dutymin_T1S3;           
        uint8_t Dutydefault_T1S3;       

        uint8_t Dutymax_T2S1;           
        uint8_t Dutymin_T2S1;           
        uint8_t Dutydefault_T2S1;       

        uint8_t Dutymax_T2S2;           
        uint8_t Dutymin_T2S2;           
        uint8_t Dutydefault_T2S2;       

        uint8_t Dutymax_T2S3;           
        uint8_t Dutymin_T2S3;           
        uint8_t Dutydefault_T2S3;       

        uint8_t Dutymax_T3S1;           
        uint8_t Dutymin_T3S1;           
        uint8_t Dutydefault_T3S1;       

        uint8_t Dutymax_T3S2;           
        uint8_t Dutymin_T3S2;           
        uint8_t Dutydefault_T3S2;       

        uint8_t Dutymax_T3S3;           
        uint8_t Dutymin_T3S3;           
        uint8_t Dutydefault_T3S3;      
}Period_Type;

typedef struct
{
    _iq24 ACVoltcali;                   
    _iq24 ACVoltcaliMin;                
    _iq24 ACVoltcaliPrev;               
    _iq24 ACVoltcali_Square;
    uint16_t  TonEnable_cntset;         
    uint16_t  TonEnable_cnt;            
    volatile  uint8_t  TonEnable;         

    Zcross_Type Zero;                   
    Period_Type PeriodCtl;              
    Phase_Type  PhaseCtl;               
}TRIAC_ctl_Type;

typedef struct
{
    uint16_t    SHkey_checkcnt;          
    uint16_t    SHkey_checkinterval;     
    uint16_t    SHkey_effcnt_thresh;     
    uint16_t    SHkey_ineff_cntthresh;   
    uint16_t    SHkey_effcnt1;           
    uint16_t    SHkey_ineff_cnt1;        
    uint16_t    SHkey_effcnt2;           
    uint16_t    SHkey_ineff_cnt2;        
    uint16_t    SHkey_Powerdown;         
    Truth_Verify_Type    SHkey_Speedset; 

    uint16_t    SRkey_effcnt_thresh;     
    uint16_t    SRkey_ineff_cntthresh;   
    uint16_t    SRkey_effcnt;           
    uint16_t    SRkey_ineff_cnt;        

    Truth_Verify_Type  Skeyeffective;   
    Truth_Verify_Type  Skeystop;        
    uint8_t            Skeystopcnt;     
    Direction_Type Skey_rank_Dir;        

    uint8_t Tkey_rankset;                
    uint8_t Tkey_rank;                   
    Direction_Type Tkey_rank_Dir;        
    Truth_Verify_Type Tkeycyc;          
    uint16_t Tkey_cyccntthresh;         
    uint16_t Tkey_shtcntthreshup;       
    uint16_t Tkey_shtcntthreshdown;     
    uint16_t Tkeycnt;                   

    uint16_t    TCkey_effcnt_thresh;     
    uint16_t    TCkey_ineff_cntthresh;   
    Truth_Verify_Type TCkeycold;         
    uint16_t    TCkey_effcnt;           
    uint16_t    TCkey_ineff_cnt;        
    Truth_Verify_Type  Tkeyeffective;   

    uint16_t    ACkey_checkcnt;             
    uint16_t    ACkey_checkinterval;        
    uint16_t    ACkey_effcnt_thresh;        
    uint16_t    ACkey_ineff_cntthresh;      
    uint16_t    ACkey_effcnt;               
    uint16_t    ACkey_ineff_cnt;            
    uint16_t    ACkey_Poweron;              

    uint16_t EEwritewaitset;            
    uint16_t EEwritewaitcnt;            
    uint8_t  EEwritecycset;             
    uint8_t  EEwritecyccnt;             
    uint8_t Keytowrite;                 

    Truth_Verify_Type Keymemreadmark;  
} Key_Manager_Type;

typedef struct
{
    uint16_t Fn;
    _iq18 Vn;
    _iq18 In;
    _iq24 Flux;
    uint16_t Pn;
    _iq24 Rs;
    _iq24 Ld;
    _iq24 Lq;
    _iq24 Rstotal;
    _iq24 Inertia;
    _iq24 Friction;
    uint16_t Webase;
    uint16_t Wmbase;
    _iq18 Vbase;
    _iq18 Ebase;
    _iq18 Ibase;
    _iq24 Fluxbase;
    _iq24 Rbase;
    _iq24 Lbase;
    _iq18 Pbase;
    _iq24 Tcoef;
    _iq18 Tbase;
    _iq24 Inertiabase;
    _iq24 Frictionbase;
} Motor_Base_Type;

typedef struct
{
   volatile Truth_Verify_Type Poweron;      
    uint16_t Poweron_CntThresh;     
    uint16_t Poweron_Cnt;           
    _iq24 Poweron_Volt;             
    _iq24 Poweroff_Volt1;           
    _iq24 Poweroff_Volt2;           
    _iq24 DCVoltAverSizeInv1;       
    _iq24 DCVoltAverSizeInv2;      
    _iq24 DCVoltAver;               
} Powercontrol_Type;                

//电机整体状态结构体原型
typedef struct
{
    Component_2R_Type  VRef2R;      
    Component_2R_Type  VRef2RC;     
    Component_2S_Type  VRef2S;      
    Component_2S_Type  VRef2SC;     
    Component_2R_Type  CRef2R;      
    Component_3S_Type  C3S;         
    Component_2S_Type  C2S;         
    Component_2R_Type  C2R_H;       
    Component_2R_Type  C2R_L;      
    Component_2R_Type  C2R;         
    Struct_Cossin_Type Cossin;      
    Component_2R_Type  C2R_Obs;     
    Component_2S_Type  C2S_Obs;     

    LPF_First_Order_Type  IdLPF1stH;    
    LPF_First_Order_Type  IdLPF1stL;    

    LPF_First_Order_Type  IqLPF1stH;    
    LPF_First_Order_Type  IqLPF1stL;    

    LPF_First_Order_Type  IdLPF1stObs;  
    LPF_First_Order_Type  IqLPF1stObs;  

    LPF_First_Order_Type  IdLPF1stInj;  
    LPF_First_Order_Type  IqLPF1stInj;  
} Motor_Sructure_Type;

//电机PID调节器结构体
typedef struct
{
    PID_Structure_Type Speed;   
    PID_Structure_Type dCurr;   
    PID_Structure_Type qCurr;   
} Motor_PID_Type;

//LBG观测器结构体原型
typedef struct
{
    LBG_Paramter_Type       Para;               
    Speed_PLL_Type          Speed_PLL;          
    LBG_Cal_Structure_Type  Cal_Struc;         
    Observer_Adaptor_type   Adaptor;            
    LBG_EMF_Init_Type       EMF_InitPara;       
    BuffPara_Type           SpeedBuffPara;      
} Motor_LBG_Type;

//定时管理结构体原型
typedef struct
{
    Truth_Verify_Type       Event_10ms;         
    uint16_t                Event_10ms_cnt;     
    uint16_t                Event_10ms_cntset;  

    Truth_Verify_Type       Event_1ms;          
    uint16_t                Event_1ms_cnt;      
    uint16_t                Event_1ms_cntset;   

    Truth_Verify_Type       Event_2s;           
    uint16_t                Event_2s_cnt;       
    uint16_t                Event_2s_cntset;    
} Eventmanager_Type;

typedef enum {NONE = 0, CHECK = 1, INTERVAL = 2, LAST = 3} Poweroncheckstage_Type;

typedef struct
{
    Truth_Verify_Type       Mark;               
    Poweroncheckstage_Type  Stage;              
    Truth_Verify_Type       Process;            

    uint16_t        Ledinttimeset;              
    uint8_t         Ledintnum;                  
    uint8_t         Ledintnumset;               
    uint16_t        Ledintnumtotal;             
    uint16_t        Ledintnumtotalset;          
    Direction_Type  Ledintdir;                  

    uint16_t        Ledlasttimeset;            
    uint16_t        Ledcnt;                     
} Poweroncheck_Type;

typedef struct
{
    uint16_t StopwaitCnt;           
    uint16_t StopwaitSet;          

    uint16_t FaultHoldingSet;       
    uint16_t FaultHoldingCnt;       

    Truth_Verify_Type       Faultstop;           

    uint16_t reset_cnt;             
    Poweroncheck_Type PSC;          
} Operation_Type;


//功率波动抑制结构体原型
typedef struct
{
    _iq24 Power;                        
    _iq24 PowerfilterH;                 
    _iq24 PowerfilterL;                 
    HPF_First_Order_Type Power_HPF1st;  
    LPF_First_Order_Type Power_LPF1st;  

    PID_Structure_Type PowerPID;        
    _iq24 Kpdefine;                     
    _iq24 Kidefine;                     
    _iq24 Kddefine;                     
    _iq24 Kpadj1;                       
    _iq24 Kpadj2;                       
    _iq24 Kpadj3;                       
    _iq24 Kpadj;                       

    uint16_t Startcom_cntset;           
    uint16_t Startcom_cnt;             

    _iq24 Speederrthre;                 
    uint16_t Speedcolse_cntset;         
    uint16_t Speedclose_cnt;            

    _iq24 Voltthre;                     
    _iq24 Speedcom;                     
}Powercom_type;

/* Exported constants -------------------------------------------------------*/
#define SPEED_CONTROL_M            0x0001      
#define FIRST_START_M              0x0002      
#define STARTUP_FAILURE_M          0x0004      
#define SPEED_ABNORMAL_M           0x0008      
#define BRAKE_RES_ON_M             0x0010      
//#define MOTOR_OVER_HEAT_M          0x0020      
#define MOTOR_OVER_CURR_SOFT_M     0x0040      
#define DC_OVER_CURR_SOFT_M        0x0080     
#define DC_OVER_CURR_HARD_M        0x0100      
#define MOTOR_STALL_M              0x0200      
#define DC_OVER_VOLT               0x0400      
#define DC_UNDER_VOLT              0x0800      
//#define MOTOR_REVERSE_M            0x1000      
#define MOTOR_OVERLOAD_M           0x2000      
#define POSITION_FAILUE_M          0x4000      
#define PHASELOSS_FALUT            0x8000      

#define NTC_OVER_ERR              0x0020      
#define NTC_OPEN_ERR               0x1000      


#define PI_2f                    6.28318531         //2*PI float：35897932384626433832795
#define PI_1_Q24                _IQ(3.14159265)     //PI
#define PI_2_Q24                _IQ(6.28318531)     //2*PI    7179586476925286766559
#define PI_2_Q18                _IQ18(6.28318531)   //2*PI
#define DIV_PI_2_Q24            _IQ(0.15915494)     //1/(2*PI)309189533576888376337251
#define PI_05_Q24               _IQ(1.57079633)     //0.5*PI67948966192313216916398
#define Three_Q24               _IQ(3.0)            //3.0
#define Three_Q18               _IQ18(3.0)          //3.0
#define Three_Q16               _IQ16(3.0)          //3.0
#define SQRT3_Q24               _IQ(1.73205081)     //SQRT(3)
#define SQRT3_Q18               _IQ18(1.73205081)   //SQRT(3)
#define SQRT3_Q16               _IQ16(1.732051)     //SQRT(3)
#define SQRT_3                  1.73205081          //SQRT(3)
#define SQRT2_Q24               _IQ(1.41421356)     //SQRT(2)
#define UNIT_Q24                _IQ(1.0)            //1.0
#define HALF_Q24                _IQ(0.5)            //0.5
#define QUARD_Q24               _IQ(0.25)           //0.5
#define DIV_SQRT3_Q24           _IQ(0.57735027)     //1/SQRT(3)
#define SQRT3_DIV2_Q24          _IQ(0.86602540)     //SQRT(3)
#define SQRT_2DIV3_Q24          _IQ(0.81649658)     //SQRT(2/3)
#define ONE_HALF_Q24            _IQ(1.50)           //1.5
#define DIV_MIN_Q24             _IQ(0.000001)       

#define U8_MAX     ((uint8_t)255)
#define S8_MAX     ((int8_t)127)
#define S8_MIN     ((int8_t)-128)
#define U16_MAX    ((uint16_t)65535u)
#define S16_MAX    ((int16_t)32767)
#define S16_MIN    ((int16_t)-32768)
#define U32_MAX    ((uint32_t)4294967295uL)
#define S32_MAX    ((int32_t)2147483647)
#define S32_MIN    ((int32_t)-2147483648)

#define SIN_TABLE_1024 {\
0x00000000,0x0001921F,0x0003243A,0x0004B64D,0x00064855,0x0007DA4D,0x00096C32,0x000AFE00,\
0x000C8FB2,0x000E2146,0x000FB2B7,0x00114401,0x0012D520,0x00146611,0x0015F6D0,0x00178758,\
0x001917A6,0x001AA7B7,0x001C3785,0x001DC70E,0x001F564E,0x0020E540,0x002273E1,0x0024022D,\
0x00259020,0x00271DB7,0x0028AAED,0x002A37BF,0x002BC428,0x002D5026,0x002EDBB3,0x003066CD,\
0x0031F170,0x00337B97,0x00350540,0x00368E65,0x00381704,0x00399F19,0x003B269F,0x003CAD94,\
0x003E33F2,0x003FB9B8,0x00413EE0,0x0042C367,0x00444749,0x0045CA83,0x00474D10,0x0048CEEE,\
0x004A5018,0x004BD08B,0x004D5043,0x004ECF3B,0x00504D72,0x0051CAE2,0x00534789,0x0054C362,\
0x00563E69,0x0057B89C,0x005931F7,0x005AAA75,0x005C2214,0x005D98D0,0x005F0EA4,0x0060838E,\
0x0061F78A,0x00636A94,0x0064DCA9,0x00664DC5,0x0067BDE5,0x00692D04,0x006A9B20,0x006C0835,\
0x006D7440,0x006EDF3C,0x00704927,0x0071B1FD,0x007319BA,0x0074805B,0x0075E5DD,0x00774A3C,\
0x0078AD74,0x007A0F83,0x007B7065,0x007CD016,0x007E2E93,0x007F8BD9,0x0080E7E4,0x008242B1,\
0x00839C3C,0x0084F483,0x00864B82,0x0087A135,0x0088F59A,0x008A48AD,0x008B9A6B,0x008CEAD0,\
0x008E39D9,0x008F8784,0x0090D3CC,0x00921EAF,0x0093682A,0x0094B039,0x0095F6D9,0x00973C07,\
0x00987FBF,0x0099C200,0x009B02C5,0x009C420C,0x009D7FD1,0x009EBC11,0x009FF6CA,0x00A12FF8,\
0x00A26799,0x00A39DA8,0x00A4D224,0x00A6050A,0x00A73655,0x00A86604,0x00A99414,0x00AAC081,\
0x00ABEB49,0x00AD1469,0x00AE3BDD,0x00AF61A4,0x00B085BA,0x00B1A81D,0x00B2C8C9,0x00B3E7BC,\
0x00B504F3,0x00B6206B,0x00B73A22,0x00B85215,0x00B96841,0x00BA7CA4,0x00BB8F3A,0x00BCA002,\
0x00BDAEF9,0x00BEBC1B,0x00BFC767,0x00C0D0D9,0x00C1D870,0x00C2DE28,0x00C3E200,0x00C4E3F4,\
0x00C5E403,0x00C6E229,0x00C7DE65,0x00C8D8B3,0x00C9D112,0x00CAC77F,0x00CBBBF7,0x00CCAE79,\
0x00CD9F02,0x00CE8D8F,0x00CF7A1F,0x00D064AF,0x00D14D3D,0x00D233C6,0x00D31848,0x00D3FAC2,\
0x00D4DB31,0x00D5B992,0x00D695E4,0x00D77025,0x00D84852,0x00D91E6A,0x00D9F269,0x00DAC44F,\
0x00DB941A,0x00DC61C6,0x00DD2D53,0x00DDF6BE,0x00DEBE05,0x00DF8327,0x00E04621,0x00E106F1,\
0x00E1C597,0x00E28210,0x00E33C59,0x00E3F472,0x00E4AA59,0x00E55E0B,0x00E60F87,0x00E6BECC,\
0x00E76BD7,0x00E816A7,0x00E8BF3B,0x00E96591,0x00EA09A6,0x00EAAB7A,0x00EB4B0B,0x00EBE858,\
0x00EC835E,0x00ED1C1D,0x00EDB293,0x00EE46BE,0x00EED89D,0x00EF682F,0x00EFF573,0x00F08066,\
0x00F10908,0x00F18F57,0x00F21352,0x00F294F8,0x00F31447,0x00F3913E,0x00F40BDD,0x00F48421,\
0x00F4FA0A,0x00F56D97,0x00F5DEC6,0x00F64D96,0x00F6BA07,0x00F72417,0x00F78BC5,0x00F7F110,\
0x00F853F7,0x00F8B47A,0x00F91297,0x00F96E4E,0x00F9C79D,0x00FA1E84,0x00FA7301,0x00FAC515,\
0x00FB14BE,0x00FB61FB,0x00FBACCD,0x00FBF531,0x00FC3B27,0x00FC7EAF,0x00FCBFC9,0x00FCFE72,\
0x00FD3AAB,0x00FD7474,0x00FDABCB,0x00FDE0B0,0x00FE1323,0x00FE4323,0x00FE70AF,0x00FE9BC8,\
0x00FEC46D,0x00FEEA9C,0x00FF0E57,0x00FF2F9D,0x00FF4E6D,0x00FF6AC7,0x00FF84AB,0x00FF9C18,\
0x00FFB10F,0x00FFC38E,0x00FFD397,0x00FFE128,0x00FFEC43,0x00FFF4E5,0x00FFFB10,0x00FFFEC4}

#define MAG100_LIMIT_TABLE {\
0x00FF017D,0x00FE05EC,0x00FD0D3D,0x00FC1764,0x00FB2451,0x00FA33F9,0x00F9464D,0x00F85B42,\
0x00F772CA,0x00F68CDB,0x00F5A968,0x00F4C866,0x00F3E9CA,0x00F30D89,0x00F23399,0x00F15BEE,\
0x00F08680,0x00EFB344,0x00EEE231,0x00EE133D,0x00ED4660,0x00EC7B90,0x00EBB2C4,0x00EAEBF5,\
0x00EA2719,0x00E96429,0x00E8A31D,0x00E7E3ED,0x00E72691,0x00E66B02,0x00E5B138,0x00E4F92E,\
0x00E442DB,0x00E38E38,0x00E2DB40,0x00E229EC,0x00E17A35,0x00E0CC15,0x00E01F86,0x00DF7482,\
0x00DECB03,0x00DE2304,0x00DD7C7F,0x00DCD76D,0x00DC33CB,0x00DB9192,0x00DAF0BE,0x00DA5149,\
0x00D9B32F,0x00D9166A,0x00D87AF6,0x00D7E0CE,0x00D747EE,0x00D6B050,0x00D619F2,0x00D584CD,\
0x00D4F0DE,0x00D45E22,0x00D3CC92,0x00D33C2D,0x00D2ACEE,0x00D21ED0,0x00D191D0,0x00D105EB,\
0x00D07B1C,0x00CFF161,0x00CF68B5,0x00CEE115,0x00CE5A7E,0x00CDD4ED,0x00CD505D,0x00CCCCCC,\
0x00CC4A37,0x00CBC89B,0x00CB47F3,0x00CAC83F,0x00CA497A,0x00C9CBA1,0x00C94EB2,0x00C8D2AB,\
0x00C85787,0x00C7DD45,0x00C763E1,0x00C6EB5A,0x00C673AC,0x00C5FCD5,0x00C586D3,0x00C511A2,\
0x00C49D42,0x00C429AE,0x00C3B6E6,0x00C344E6,0x00C2D3AC,0x00C26336,0x00C1F383,0x00C1848F,\
0x00C11658,0x00C0A8DD,0x00C03C1C,0x00BFD011,0x00BF64BD,0x00BEFA1B,0x00BE902B,0x00BE26EB,\
0x00BDBE58,0x00BD5671,0x00BCEF34,0x00BC889F,0x00BC22B0,0x00BBBD66,0x00BB58BF,0x00BAF4BA,\
0x00BA9153,0x00BA2E8B,0x00B9CC5F,0x00B96ACE,0x00B909D5,0x00B8A974,0x00B849AA,0x00B7EA73,\
0x00B78BD0,0x00B72DBE,0x00B6D03C,0x00B67349,0x00B616E3,0x00B5BB09,0x00B55FB9,0x00B504F3,\
0x00B4AAB4,0x00B450FC,0x00B3F7C8,0x00B39F19,0x00B346EC,0x00B2EF41,0x00B29816,0x00B24169,\
0x00B1EB3B,0x00B19589,0x00B14052,0x00B0EB95,0x00B09752,0x00B04386,0x00AFF032,0x00AF9D53,\
0x00AF4AE8,0x00AEF8F2,0x00AEA76E,0x00AE565B,0x00AE05B9,0x00ADB587,0x00AD65C3,0x00AD166C,\
0x00ACC782,0x00AC7903,0x00AC2AEF,0x00ABDD45,0x00AB9003,0x00AB432A,0x00AAF6B7,0x00AAAAAA,\
0x00AA5F03,0x00AA13BF,0x00A9C8DF,0x00A97E62,0x00A93446,0x00A8EA8B,0x00A8A130,0x00A85835,\
0x00A80F98,0x00A7C758,0x00A77F76,0x00A737EF,0x00A6F0C4,0x00A6A9F3,0x00A6637D,0x00A61D5F,\
0x00A5D799,0x00A5922C,0x00A54D15,0x00A50854,0x00A4C3E9,0x00A47FD3,0x00A43C11,0x00A3F8A2,\
0x00A3B586,0x00A372BC,0x00A33044,0x00A2EE1D,0x00A2AC45,0x00A26ABE,0x00A22985,0x00A1E89B,\
0x00A1A7FE,0x00A167AE,0x00A127AB,0x00A0E7F4,0x00A0A888,0x00A06967,0x00A02A90,0x009FEC03,\
0x009FADBF,0x009F6FC3,0x009F3210,0x009EF4A4,0x009EB77E,0x009E7A9F,0x009E3E06,0x009E01B2,\
0x009DC5A3,0x009D89D8,0x009D4E51,0x009D130D,0x009CD80C,0x009C9D4E,0x009C62D1,0x009C2895,\
0x009BEE9B,0x009BB4E0,0x009B7B66,0x009B422B,0x009B092F,0x009AD072,0x009A97F3,0x009A5FB1,\
0x009A27AD,0x0099EFE6,0x0099B85B,0x0099810C,0x009949F8,0x0099131F,0x0098DC82,0x0098A61E,\
0x00986FF5,0x00983A05,0x0098044E,0x0097CECF,0x00979989,0x0097647B,0x00972FA5,0x0096FB06,\
0x0096C69D,0x0096926B,0x00965E6F,0x00962AA9,0x0095F718,0x0095C3BC,0x00959094,0x00955DA1,\
0x00952AE2,0x0094F857,0x0094C5FE,0x009493D9,0x009461E6,0x00943025,0x0093FE97,0x0093CD3A}

#define MAG098_LIMIT_TABLE {\
0x00FEF70F,0x00FDF14E,0x00FCEEAD,0x00FBEF1C,0x00FAF28C,0x00F9F8ED,0x00F90231,0x00F80E4B,\
0x00F71D2C,0x00F62EC6,0x00F5430E,0x00F459F5,0x00F37370,0x00F28F73,0x00F1ADF2,0x00F0CEE1,\
0x00EFF235,0x00EF17E4,0x00EE3FE2,0x00ED6A26,0x00EC96A5,0x00EBC555,0x00EAF62D,0x00EA2923,\
0x00E95E2F,0x00E89546,0x00E7CE62,0x00E70978,0x00E64680,0x00E58573,0x00E4C648,0x00E408F8,\
0x00E34D7B,0x00E293C9,0x00E1DBDB,0x00E125AB,0x00E07130,0x00DFBE66,0x00DF0D43,0x00DE5DC3,\
0x00DDAFDE,0x00DD038F,0x00DC58D0,0x00DBAF99,0x00DB07E7,0x00DA61B2,0x00D9BCF5,0x00D919AB,\
0x00D877CD,0x00D7D758,0x00D73846,0x00D69A92,0x00D5FE36,0x00D5632E,0x00D4C974,0x00D43106,\
0x00D399DD,0x00D303F5,0x00D26F4A,0x00D1DBD8,0x00D1499A,0x00D0B88C,0x00D028AA,0x00CF99F0,\
0x00CF0C5A,0x00CE7FE4,0x00CDF48B,0x00CD6A4A,0x00CCE11E,0x00CC5904,0x00CBD1F7,0x00CB4BF5,\
0x00CAC6F9,0x00CA4302,0x00C9C00A,0x00C93E10,0x00C8BD10,0x00C83D06,0x00C7BDF1,0x00C73FCC,\
0x00C6C295,0x00C64649,0x00C5CAE5,0x00C55066,0x00C4D6CA,0x00C45E0D,0x00C3E62E,0x00C36F29,\
0x00C2F8FC,0x00C283A4,0x00C20F1F,0x00C19B6B,0x00C12884,0x00C0B66A,0x00C04518,0x00BFD48E,\
0x00BF64C8,0x00BEF5C5,0x00BE8782,0x00BE19FD,0x00BDAD34,0x00BD4125,0x00BCD5CE,0x00BC6B2D,\
0x00BC0140,0x00BB9804,0x00BB2F79,0x00BAC79B,0x00BA606A,0x00B9F9E3,0x00B99404,0x00B92ECC,\
0x00B8CA39,0x00B8664A,0x00B802FB,0x00B7A04D,0x00B73E3C,0x00B6DCC8,0x00B67BEF,0x00B61BAF,\
0x00B5BC07,0x00B55CF5,0x00B4FE77,0x00B4A08D,0x00B44334,0x00B3E66B,0x00B38A31,0x00B32E85,\
0x00B2D364,0x00B278CD,0x00B21EC0,0x00B1C53B,0x00B16C3C,0x00B113C2,0x00B0BBCC,0x00B06458,\
0x00B00D66,0x00AFB6F4,0x00AF6101,0x00AF0B8B,0x00AEB692,0x00AE6214,0x00AE0E11,0x00ADBA86,\
0x00AD6773,0x00AD14D7,0x00ACC2B0,0x00AC70FE,0x00AC1FC0,0x00ABCEF4,0x00AB7E99,0x00AB2EAF,\
0x00AADF34,0x00AA9027,0x00AA4187,0x00A9F354,0x00A9A58D,0x00A9582F,0x00A90B3B,0x00A8BEB0,\
0x00A8728C,0x00A826CF,0x00A7DB78,0x00A79086,0x00A745F7,0x00A6FBCC,0x00A6B204,0x00A6689C,\
0x00A61F96,0x00A5D6EF,0x00A58EA7,0x00A546BD,0x00A4FF31,0x00A4B801,0x00A4712D,0x00A42AB5,\
0x00A3E496,0x00A39ED1,0x00A35965,0x00A31451,0x00A2CF94,0x00A28B2E,0x00A2471E,0x00A20363,\
0x00A1BFFD,0x00A17CEA,0x00A13A2B,0x00A0F7BE,0x00A0B5A3,0x00A073D9,0x00A03260,0x009FF136,\
0x009FB05C,0x009F6FD1,0x009F2F93,0x009EEFA3,0x009EB000,0x009E70A9,0x009E319D,0x009DF2DD,\
0x009DB467,0x009D763B,0x009D3858,0x009CFABE,0x009CBD6C,0x009C8061,0x009C439E,0x009C0722,\
0x009BCAEB,0x009B8EFA,0x009B534E,0x009B17E6,0x009ADCC3,0x009AA1E2,0x009A6745,0x009A2CEA,\
0x0099F2D2,0x0099B8FA,0x00997F64,0x0099460E,0x00990CF8,0x0098D422,0x00989B8B,0x00986333,\
0x00982B19,0x0097F33C,0x0097BB9D,0x0097843B,0x00974D16,0x0097162C,0x0096DF7E,0x0096A90B,\
0x009672D3,0x00963CD6,0x00960712,0x0095D188,0x00959C37,0x0095671F,0x0095323F,0x0094FD98,\
0x0094C927,0x009494EE,0x009460EC,0x00942D21,0x0093F98B,0x0093C62B,0x00939301,0x0093600B,\
0x00932D4A,0x0092FABE,0x0092C865,0x00929640,0x0092644E,0x00923290,0x00920103,0x0091CFA9}

#define MAG096_LIMIT_TABLE {\
0x00FEEBF9,0x00FDDB69,0x00FCCE3B,0x00FBC45E,0x00FABDC2,0x00F9BA54,0x00F8BA06,0x00F7BCC6,\
0x00F6C286,0x00F5CB36,0x00F4D6C8,0x00F3E52D,0x00F2F659,0x00F20A3C,0x00F120CB,0x00F039F8,\
0x00EF55B8,0x00EE73FD,0x00ED94BC,0x00ECB7E9,0x00EBDD7A,0x00EB0563,0x00EA2F9A,0x00E95C14,\
0x00E88AC8,0x00E7BBAA,0x00E6EEB2,0x00E623D7,0x00E55B0E,0x00E4944F,0x00E3CF92,0x00E30CCD,\
0x00E24BF8,0x00E18D0B,0x00E0CFFF,0x00E014CA,0x00DF5B67,0x00DEA3CC,0x00DDEDF4,0x00DD39D6,\
0x00DC876C,0x00DBD6AF,0x00DB2798,0x00DA7A21,0x00D9CE43,0x00D923F9,0x00D87B3B,0x00D7D404,\
0x00D72E4F,0x00D68A15,0x00D5E751,0x00D545FD,0x00D4A614,0x00D40790,0x00D36A6D,0x00D2CEA6,\
0x00D23435,0x00D19B15,0x00D10342,0x00D06CB7,0x00CFD76F,0x00CF4367,0x00CEB099,0x00CE1F01,\
0x00CD8E9B,0x00CCFF63,0x00CC7155,0x00CBE46D,0x00CB58A6,0x00CACDFD,0x00CA446F,0x00C9BBF7,\
0x00C93491,0x00C8AE3B,0x00C828F1,0x00C7A4AE,0x00C72171,0x00C69F35,0x00C61DF7,0x00C59DB4,\
0x00C51E6A,0x00C4A013,0x00C422AF,0x00C3A639,0x00C32AAF,0x00C2B00E,0x00C23653,0x00C1BD7B,\
0x00C14584,0x00C0CE6B,0x00C0582C,0x00BFE2C6,0x00BF6E36,0x00BEFA7A,0x00BE878E,0x00BE1571,\
0x00BDA420,0x00BD3398,0x00BCC3D9,0x00BC54DE,0x00BBE6A6,0x00BB792E,0x00BB0C75,0x00BAA079,\
0x00BA3536,0x00B9CAAC,0x00B960D8,0x00B8F7B8,0x00B88F49,0x00B8278B,0x00B7C07C,0x00B75A18,\
0x00B6F45F,0x00B68F4E,0x00B62AE4,0x00B5C720,0x00B563FE,0x00B5017E,0x00B49F9D,0x00B43E5B,\
0x00B3DDB5,0x00B37DAB,0x00B31E39,0x00B2BF5F,0x00B2611B,0x00B2036B,0x00B1A64F,0x00B149C4,\
0x00B0EDC9,0x00B0925C,0x00B0377D,0x00AFDD2A,0x00AF8360,0x00AF2A20,0x00AED168,0x00AE7935,\
0x00AE2188,0x00ADCA5E,0x00AD73B7,0x00AD1D91,0x00ACC7EB,0x00AC72C3,0x00AC1E19,0x00ABC9EB,\
0x00AB7638,0x00AB22FF,0x00AAD03E,0x00AA7DF6,0x00AA2C23,0x00A9DAC6,0x00A989DE,0x00A93968,\
0x00A8E965,0x00A899D3,0x00A84AB0,0x00A7FBFD,0x00A7ADB8,0x00A75FE0,0x00A71274,0x00A6C573,\
0x00A678DC,0x00A62CAE,0x00A5E0E8,0x00A5958A,0x00A54A92,0x00A4FFFF,0x00A4B5D1,0x00A46C07,\
0x00A4229F,0x00A3D99A,0x00A390F6,0x00A348B2,0x00A300CD,0x00A2B947,0x00A2721F,0x00A22B54,\
0x00A1E4E5,0x00A19ED1,0x00A15919,0x00A113BA,0x00A0CEB4,0x00A08A07,0x00A045B2,0x00A001B3,\
0x009FBE0B,0x009F7AB8,0x009F37BA,0x009EF510,0x009EB2BA,0x009E70B7,0x009E2F05,0x009DEDA5,\
0x009DAC96,0x009D6BD7,0x009D2B68,0x009CEB47,0x009CAB75,0x009C6BF0,0x009C2CB8,0x009BEDCD,\
0x009BAF2D,0x009B70D9,0x009B32CF,0x009AF510,0x009AB79A,0x009A7A6C,0x009A3D88,0x009A00EA,\
0x0099C495,0x00998885,0x00994CBC,0x00991139,0x0098D5FB,0x00989B01,0x0098604B,0x009825D9,\
0x0097EBAA,0x0097B1BE,0x00977813,0x00973EAB,0x00970583,0x0096CC9C,0x009693F5,0x00965B8D,\
0x00962365,0x0095EB7C,0x0095B3D1,0x00957C63,0x00954533,0x00950E40,0x0094D78A,0x0094A110,\
0x00946AD1,0x009434CD,0x0093FF05,0x0093C976,0x00939422,0x00935F07,0x00932A26,0x0092F57D,\
0x0092C10C,0x00928CD4,0x009258D3,0x00922509,0x0091F176,0x0091BE1A,0x00918AF4,0x00915803,\
0x00912548,0x0090F2C2,0x0090C070,0x00908E52,0x00905C69,0x00902AB3,0x008FF930,0x008FC7E0}

#define MAG094_LIMIT_TABLE {\
0x00FEE02E,0x00FDC420,0x00FCABBE,0x00FB96F7,0x00FA85B5,0x00F977E6,0x00F86D77,0x00F76657,\
0x00F66274,0x00F561BE,0x00F46423,0x00F36995,0x00F27203,0x00F17D5E,0x00F08B99,0x00EF9CA3,\
0x00EEB071,0x00EDC6F3,0x00ECE01E,0x00EBFBE4,0x00EB1A38,0x00EA3B10,0x00E95E5E,0x00E88417,\
0x00E7AC30,0x00E6D69F,0x00E60357,0x00E53250,0x00E4637F,0x00E396DA,0x00E2CC56,0x00E203EC,\
0x00E13D92,0x00E0793E,0x00DFB6E8,0x00DEF687,0x00DE3814,0x00DD7B85,0x00DCC0D3,0x00DC07F6,\
0x00DB50E6,0x00DA9B9D,0x00D9E812,0x00D9363E,0x00D8861B,0x00D7D7A2,0x00D72ACD,0x00D67F93,\
0x00D5D5F0,0x00D52DDD,0x00D48754,0x00D3E24F,0x00D33EC8,0x00D29CB9,0x00D1FC1D,0x00D15CEE,\
0x00D0BF27,0x00D022C3,0x00CF87BC,0x00CEEE0E,0x00CE55B3,0x00CDBEA7,0x00CD28E4,0x00CC9467,\
0x00CC012A,0x00CB6F2A,0x00CADE61,0x00CA4ECB,0x00C9C065,0x00C9332A,0x00C8A716,0x00C81C24,\
0x00C79252,0x00C7099B,0x00C681FC,0x00C5FB70,0x00C575F5,0x00C4F186,0x00C46E20,0x00C3EBC0,\
0x00C36A62,0x00C2EA03,0x00C26A9F,0x00C1EC34,0x00C16EBF,0x00C0F23C,0x00C076A8,0x00BFFC00,\
0x00BF8242,0x00BF096A,0x00BE9176,0x00BE1A62,0x00BDA42D,0x00BD2ED3,0x00BCBA52,0x00BC46A7,\
0x00BBD3D0,0x00BB61CA,0x00BAF093,0x00BA8028,0x00BA1086,0x00B9A1AD,0x00B93398,0x00B8C647,\
0x00B859B6,0x00B7EDE3,0x00B782CD,0x00B71872,0x00B6AECE,0x00B645E0,0x00B5DDA6,0x00B5761F,\
0x00B50F47,0x00B4A91D,0x00B443A0,0x00B3DECD,0x00B37AA2,0x00B3171E,0x00B2B43F,0x00B25202,\
0x00B1F067,0x00B18F6C,0x00B12F0F,0x00B0CF4E,0x00B07027,0x00B01199,0x00AFB3A3,0x00AF5643,\
0x00AEF976,0x00AE9D3D,0x00AE4195,0x00ADE67C,0x00AD8BF2,0x00AD31F5,0x00ACD883,0x00AC7F9B,\
0x00AC273C,0x00ABCF64,0x00AB7812,0x00AB2144,0x00AACAFA,0x00AA7532,0x00AA1FEB,0x00A9CB23,\
0x00A976D9,0x00A9230D,0x00A8CFBC,0x00A87CE6,0x00A82A8A,0x00A7D8A5,0x00A78738,0x00A73641,\
0x00A6E5BF,0x00A695B1,0x00A64615,0x00A5F6EC,0x00A5A833,0x00A559E9,0x00A50C0E,0x00A4BEA1,\
0x00A471A0,0x00A4250B,0x00A3D8E0,0x00A38D1F,0x00A341C7,0x00A2F6D7,0x00A2AC4D,0x00A26229,\
0x00A2186B,0x00A1CF10,0x00A18619,0x00A13D84,0x00A0F550,0x00A0AD7D,0x00A0660A,0x00A01EF6,\
0x009FD840,0x009F91E8,0x009F4BEC,0x009F064B,0x009EC106,0x009E7C1B,0x009E378A,0x009DF351,\
0x009DAF70,0x009D6BE6,0x009D28B3,0x009CE5D6,0x009CA34D,0x009C6119,0x009C1F39,0x009BDDAC,\
0x009B9C71,0x009B5B87,0x009B1AEF,0x009ADAA7,0x009A9AAE,0x009A5B05,0x009A1BAA,0x0099DC9C,\
0x00999DDC,0x00995F68,0x00992141,0x0098E364,0x0098A5D3,0x0098688B,0x00982B8D,0x0097EED9,\
0x0097B26C,0x00977647,0x00973A6A,0x0096FED4,0x0096C383,0x00968879,0x00964DB3,0x00961332,\
0x0095D8F6,0x00959EFC,0x00956546,0x00952BD3,0x0094F2A1,0x0094B9B2,0x00948103,0x00944895,\
0x00941067,0x0093D879,0x0093A0C9,0x00936959,0x00933227,0x0092FB33,0x0092C47C,0x00928E02,\
0x009257C4,0x009221C3,0x0091EBFD,0x0091B672,0x00918123,0x00914C0D,0x00911732,0x0090E28F,\
0x0090AE27,0x009079F6,0x009045FE,0x0090123E,0x008FDEB6,0x008FAB65,0x008F784A,0x008F4566,\
0x008F12B8,0x008EE03F,0x008EADFC,0x008E7BEE,0x008E4A14,0x008E186E,0x008DE6FC,0x008DB5BE}

#define MAG092_LIMIT_TABLE {\
0x00FED39E,0x00FDAB55,0x00FC870C,0x00FB66AC,0x00FA4A1F,0x00F93150,0x00F81C2A,0x00F70A99,\
0x00F5FC89,0x00F4F1E7,0x00F3EAA1,0x00F2E6A4,0x00F1E5E0,0x00F0E844,0x00EFEDBF,0x00EEF641,\
0x00EE01BB,0x00ED101E,0x00EC215B,0x00EB3563,0x00EA4C2A,0x00E965A1,0x00E881BB,0x00E7A06C,\
0x00E6C1A6,0x00E5E55F,0x00E50B8A,0x00E4341C,0x00E35F09,0x00E28C46,0x00E1BBCA,0x00E0ED89,\
0x00E02179,0x00DF5791,0x00DE8FC6,0x00DDCA10,0x00DD0666,0x00DC44BD,0x00DB850F,0x00DAC751,\
0x00DA0B7C,0x00D95187,0x00D8996C,0x00D7E321,0x00D72E9F,0x00D67BDF,0x00D5CADA,0x00D51B88,\
0x00D46DE3,0x00D3C1E3,0x00D31783,0x00D26EBB,0x00D1C786,0x00D121DD,0x00D07DBA,0x00CFDB17,\
0x00CF39EE,0x00CE9A3A,0x00CDFBF5,0x00CD5F19,0x00CCC3A2,0x00CC298A,0x00CB90CB,0x00CAF962,\
0x00CA6348,0x00C9CE79,0x00C93AF1,0x00C8A8AB,0x00C817A1,0x00C787D1,0x00C6F935,0x00C66BC8,\
0x00C5DF88,0x00C55470,0x00C4CA7B,0x00C441A6,0x00C3B9ED,0x00C3334C,0x00C2ADBF,0x00C22943,\
0x00C1A5D4,0x00C1236F,0x00C0A20F,0x00C021B2,0x00BFA255,0x00BF23F3,0x00BEA68A,0x00BE2A17,\
0x00BDAE96,0x00BD3405,0x00BCBA60,0x00BC41A5,0x00BBC9D0,0x00BB52DF,0x00BADCCE,0x00BA679C,\
0x00B9F345,0x00B97FC7,0x00B90D1F,0x00B89B4B,0x00B82A48,0x00B7BA13,0x00B74AAB,0x00B6DC0C,\
0x00B66E35,0x00B60122,0x00B594D3,0x00B52943,0x00B4BE72,0x00B4545D,0x00B3EB03,0x00B3825F,\
0x00B31A72,0x00B2B338,0x00B24CB0,0x00B1E6D8,0x00B181AD,0x00B11D2F,0x00B0B95A,0x00B0562D,\
0x00AFF3A6,0x00AF91C4,0x00AF3085,0x00AECFE6,0x00AE6FE7,0x00AE1085,0x00ADB1BF,0x00AD5393,\
0x00ACF600,0x00AC9903,0x00AC3C9C,0x00ABE0C9,0x00AB8588,0x00AB2AD7,0x00AAD0B6,0x00AA7723,\
0x00AA1E1C,0x00A9C5A0,0x00A96DAD,0x00A91643,0x00A8BF5F,0x00A86901,0x00A81327,0x00A7BDD0,\
0x00A768FA,0x00A714A4,0x00A6C0CE,0x00A66D75,0x00A61A98,0x00A5C837,0x00A57650,0x00A524E2,\
0x00A4D3EB,0x00A4836B,0x00A43361,0x00A3E3CB,0x00A394A8,0x00A345F8,0x00A2F7B9,0x00A2A9E9,\
0x00A25C89,0x00A20F97,0x00A1C312,0x00A176F9,0x00A12B4B,0x00A0E006,0x00A0952B,0x00A04AB8,\
0x00A000AD,0x009FB707,0x009F6DC7,0x009F24EB,0x009EDC73,0x009E945D,0x009E4CA9,0x009E0556,\
0x009DBE63,0x009D77D0,0x009D319B,0x009CEBC3,0x009CA648,0x009C6129,0x009C1C66,0x009BD7FC,\
0x009B93ED,0x009B5036,0x009B0CD7,0x009AC9D0,0x009A871F,0x009A44C4,0x009A02BF,0x0099C10E,\
0x00997FB0,0x00993EA6,0x0098FDEE,0x0098BD88,0x00987D73,0x00983DAF,0x0097FE3A,0x0097BF14,\
0x0097803C,0x009741B3,0x00970377,0x0096C587,0x009687E3,0x00964A8B,0x00960D7D,0x0095D0B9,\
0x0095943F,0x0095580E,0x00951C26,0x0094E085,0x0094A52C,0x00946A1A,0x00942F4D,0x0093F4C7,\
0x0093BA85,0x00938088,0x009346CF,0x00930D5A,0x0092D428,0x00929B38,0x0092628A,0x00922A1E,\
0x0091F1F3,0x0091BA08,0x0091825E,0x00914AF3,0x009113C7,0x0090DCDA,0x0090A62B,0x00906FBA,\
0x00903987,0x00900390,0x008FCDD5,0x008F9856,0x008F6313,0x008F2E0B,0x008EF93E,0x008EC4AB,\
0x008E9052,0x008E5C32,0x008E284B,0x008DF49D,0x008DC127,0x008D8DE8,0x008D5AE2,0x008D2812,\
0x008CF579,0x008CC316,0x008C90E9,0x008C5EF1,0x008C2D2F,0x008BFBA2,0x008BCA48,0x008B9923}

//0.90限幅表定义，幅值平方限制范围2.8100
#define MAG090_LIMIT_TABLE {\
0x00FEC637,0x00FD90E6,0x00FC5FF1,0x00FB333E,0x00FA0AB4,0x00F8E63A,0x00F7C5BA,0x00F6A91C,\
0x00F59049,0x00F47B2E,0x00F369B4,0x00F25BC8,0x00F15157,0x00F04A4D,0x00EF4697,0x00EE4625,\
0x00ED48E4,0x00EC4EC4,0x00EB57B5,0x00EA63A7,0x00E97289,0x00E8844E,0x00E798E6,0x00E6B044,\
0x00E5CA59,0x00E4E718,0x00E40675,0x00E32862,0x00E24CD2,0x00E173BB,0x00E09D10,0x00DFC8C5,\
0x00DEF6D0,0x00DE2725,0x00DD59BB,0x00DC8E86,0x00DBC57D,0x00DAFE96,0x00DA39C7,0x00D97707,\
0x00D8B64D,0x00D7F790,0x00D73AC7,0x00D67FEA,0x00D5C6F0,0x00D50FD1,0x00D45A86,0x00D3A706,\
0x00D2F54B,0x00D2454D,0x00D19704,0x00D0EA6A,0x00D03F77,0x00CF9626,0x00CEEE6F,0x00CE484C,\
0x00CDA3B6,0x00CD00A8,0x00CC5F1C,0x00CBBF0B,0x00CB206F,0x00CA8344,0x00C9E784,0x00C94D29,\
0x00C8B42E,0x00C81C8D,0x00C78642,0x00C6F148,0x00C65D9A,0x00C5CB32,0x00C53A0D,0x00C4AA26,\
0x00C41B78,0x00C38DFE,0x00C301B5,0x00C27699,0x00C1ECA4,0x00C163D3,0x00C0DC23,0x00C0558E,\
0x00BFD011,0x00BF4BA9,0x00BEC852,0x00BE4608,0x00BDC4C7,0x00BD448D,0x00BCC555,0x00BC471C,\
0x00BBC9DF,0x00BB4D9A,0x00BAD24C,0x00BA57EF,0x00B9DE81,0x00B96600,0x00B8EE68,0x00B877B6,\
0x00B801E8,0x00B78CFA,0x00B718EA,0x00B6A5B5,0x00B63359,0x00B5C1D2,0x00B5511E,0x00B4E13C,\
0x00B47227,0x00B403DE,0x00B3965E,0x00B329A5,0x00B2BDB0,0x00B2527E,0x00B1E80C,0x00B17E57,\
0x00B1155E,0x00B0AD1F,0x00B04597,0x00AFDEC3,0x00AF78A3,0x00AF1335,0x00AEAE75,0x00AE4A62,\
0x00ADE6FB,0x00AD843D,0x00AD2226,0x00ACC0B5,0x00AC5FE8,0x00ABFFBD,0x00ABA032,0x00AB4145,\
0x00AAE2F6,0x00AA8541,0x00AA2826,0x00A9CBA3,0x00A96FB6,0x00A9145E,0x00A8B999,0x00A85F66,\
0x00A805C2,0x00A7ACAE,0x00A75426,0x00A6FC2A,0x00A6A4B8,0x00A64DCF,0x00A5F76D,0x00A5A192,\
0x00A54C3B,0x00A4F768,0x00A4A316,0x00A44F46,0x00A3FBF5,0x00A3A922,0x00A356CC,0x00A304F2,\
0x00A2B392,0x00A262AC,0x00A2123E,0x00A1C247,0x00A172C6,0x00A123BA,0x00A0D522,0x00A086FC,\
0x00A03947,0x009FEC03,0x009F9F2F,0x009F52C8,0x009F06CF,0x009EBB42,0x009E7021,0x009E256A,\
0x009DDB1C,0x009D9136,0x009D47B8,0x009CFEA0,0x009CB5EE,0x009C6DA1,0x009C25B7,0x009BDE30,\
0x009B970B,0x009B5047,0x009B09E3,0x009AC3DF,0x009A7E39,0x009A38F1,0x0099F405,0x0099AF76,\
0x00996B42,0x00992769,0x0098E3E9,0x0098A0C2,0x00985DF4,0x00981B7C,0x0097D95C,0x00979791,\
0x0097561C,0x009714FB,0x0096D42E,0x009693B4,0x0096538D,0x009613B7,0x0095D432,0x009594FE,\
0x00955619,0x00951784,0x0094D93D,0x00949B44,0x00945D97,0x00942038,0x0093E324,0x0093A65C,\
0x009369DE,0x00932DAA,0x0092F1C0,0x0092B61F,0x00927AC6,0x00923FB6,0x009204EC,0x0091CA69,\
0x0091902C,0x00915634,0x00911C82,0x0090E314,0x0090A9EA,0x00907104,0x00903861,0x00900000,\
0x008FC7E0,0x008F9003,0x008F5866,0x008F210A,0x008EE9ED,0x008EB310,0x008E7C72,0x008E4613,\
0x008E0FF2,0x008DDA0E,0x008DA467,0x008D6EFD,0x008D39D0,0x008D04DE,0x008CD027,0x008C9BAC,\
0x008C676B,0x008C3364,0x008BFF97,0x008BCC02,0x008B98A7,0x008B6585,0x008B329A,0x008AFFE7,\
0x008ACD6B,0x008A9B26,0x008A6918,0x008A373F,0x008A059C,0x0089D42F,0x0089A2F7,0x008971F3}

//0.88限幅表定义，幅值平方限制范围2.7744
#define MAG088_LIMIT_TABLE {\
0x00FEB7E6,0x00FD74AE,0x00FC3639,0x00FAFC68,0x00F9C71F,0x00F89642,0x00F769B8,0x00F64165,\
0x00F51D31,0x00F3FD03,0x00F2E0C5,0x00F1C860,0x00F0B3BC,0x00EFA2C6,0x00EE9569,0x00ED8B8F,\
0x00EC8527,0x00EB821D,0x00EA825F,0x00E985DB,0x00E88C80,0x00E7963D,0x00E6A302,0x00E5B2C0,\
0x00E4C566,0x00E3DAE6,0x00E2F331,0x00E20E39,0x00E12BF2,0x00E04C4C,0x00DF6F3C,0x00DE94B4,\
0x00DDBCA9,0x00DCE70E,0x00DC13D7,0x00DB42FA,0x00DA746C,0x00D9A821,0x00D8DE0F,0x00D8162B,\
0x00D7506D,0x00D68CC9,0x00D5CB37,0x00D50BAD,0x00D44E22,0x00D3928E,0x00D2D8E8,0x00D22127,\
0x00D16B43,0x00D0B734,0x00D004F3,0x00CF5477,0x00CEA5BA,0x00CDF8B3,0x00CD4D5D,0x00CCA3AF,\
0x00CBFBA3,0x00CB5533,0x00CAB057,0x00CA0D0A,0x00C96B45,0x00C8CB02,0x00C82C3B,0x00C78EEB,\
0x00C6F30B,0x00C65897,0x00C5BF88,0x00C527D9,0x00C49185,0x00C3FC87,0x00C368DA,0x00C2D678,\
0x00C2455E,0x00C1B586,0x00C126EC,0x00C0998A,0x00C00D5E,0x00BF8262,0x00BEF892,0x00BE6FE9,\
0x00BDE865,0x00BD6200,0x00BCDCB7,0x00BC5886,0x00BBD569,0x00BB535C,0x00BAD25C,0x00BA5265,\
0x00B9D373,0x00B95584,0x00B8D893,0x00B85C9E,0x00B7E1A1,0x00B76798,0x00B6EE81,0x00B67659,\
0x00B5FF1D,0x00B588C9,0x00B5135B,0x00B49ECF,0x00B42B23,0x00B3B855,0x00B34661,0x00B2D544,\
0x00B264FD,0x00B1F588,0x00B186E3,0x00B1190B,0x00B0ABFE,0x00B03FBA,0x00AFD43C,0x00AF6981,\
0x00AEFF88,0x00AE964F,0x00AE2DD2,0x00ADC60F,0x00AD5F06,0x00ACF8B3,0x00AC9314,0x00AC2E27,\
0x00ABC9EB,0x00AB665D,0x00AB037B,0x00AAA144,0x00AA3FB5,0x00A9DECD,0x00A97E89,0x00A91EE8,\
0x00A8BFE9,0x00A86189,0x00A803C6,0x00A7A6A0,0x00A74A14,0x00A6EE20,0x00A692C3,0x00A637FC,\
0x00A5DDC9,0x00A58427,0x00A52B17,0x00A4D296,0x00A47AA2,0x00A4233A,0x00A3CC5E,0x00A3760A,\
0x00A3203F,0x00A2CAFA,0x00A2763A,0x00A221FE,0x00A1CE45,0x00A17B0D,0x00A12854,0x00A0D61A,\
0x00A0845E,0x00A0331E,0x009FE258,0x009F920D,0x009F423A,0x009EF2DE,0x009EA3F8,0x009E5587,\
0x009E078B,0x009DBA01,0x009D6CE9,0x009D2041,0x009CD409,0x009C883F,0x009C3CE3,0x009BF1F4,\
0x009BA76F,0x009B5D56,0x009B13A5,0x009ACA5D,0x009A817D,0x009A3903,0x0099F0EF,0x0099A93F,\
0x009961F3,0x00991B0B,0x0098D484,0x00988E5E,0x00984898,0x00980332,0x0097BE2B,0x00977981,\
0x00973534,0x0096F143,0x0096ADAD,0x00966A72,0x00962790,0x0095E508,0x0095A2D7,0x009560FE,\
0x00951F7C,0x0094DE4F,0x00949D78,0x00945CF5,0x00941CC5,0x0093DCE9,0x00939D5F,0x00935E27,\
0x00931F40,0x0092E0A9,0x0092A261,0x00926469,0x009226BF,0x0091E962,0x0091AC53,0x00916F90,\
0x00913319,0x0090F6EC,0x0090BB0B,0x00907F73,0x00904425,0x00900920,0x008FCE63,0x008F93ED,\
0x008F59BF,0x008F1FD7,0x008EE635,0x008EACD8,0x008E73C0,0x008E3AED,0x008E025D,0x008DCA11,\
0x008D9207,0x008D5A40,0x008D22BA,0x008CEB76,0x008CB472,0x008C7DAF,0x008C472B,0x008C10E7,\
0x008BDAE1,0x008BA51A,0x008B6F90,0x008B3A44,0x008B0535,0x008AD062,0x008A9BCC,0x008A6771,\
0x008A3351,0x0089FF6C,0x0089CBC1,0x00899850,0x00896518,0x0089321A,0x0088FF54,0x0088CCC6,\
0x00889A70,0x00886852,0x0088366A,0x008804B9,0x0087D33F,0x0087A1FA,0x008770EB,0x00874010}

//0.86限幅表定义，幅值平方限制范围2.7396
#define MAG086_LIMIT_TABLE {\
0x00FEA895,0x00FD5683,0x00FC09A5,0x00FAC1DA,0x00F97F01,0x00F840FA,0x00F707A8,0x00F5D2ED,\
0x00F4A2AB,0x00F376C9,0x00F24F2B,0x00F12BB8,0x00F00C57,0x00EEF0EF,0x00EDD96A,0x00ECC5B2,\
0x00EBB5AF,0x00EAA94E,0x00E9A079,0x00E89B1D,0x00E79927,0x00E69A83,0x00E59F21,0x00E4A6ED,\
0x00E3B1D8,0x00E2BFD0,0x00E1D0C5,0x00E0E4A9,0x00DFFB6B,0x00DF14FD,0x00DE3150,0x00DD5057,\
0x00DC7204,0x00DB964A,0x00DABD1C,0x00D9E66D,0x00D91232,0x00D8405E,0x00D770E6,0x00D6A3BE,\
0x00D5D8DC,0x00D51036,0x00D449C0,0x00D38571,0x00D2C33E,0x00D2031F,0x00D14509,0x00D088F3,\
0x00CFCED6,0x00CF16A7,0x00CE605E,0x00CDABF4,0x00CCF95F,0x00CC4899,0x00CB9999,0x00CAEC57,\
0x00CA40CD,0x00C996F4,0x00C8EEC3,0x00C84834,0x00C7A341,0x00C6FFE3,0x00C65E14,0x00C5BDCD,\
0x00C51F08,0x00C481BE,0x00C3E5EB,0x00C34B89,0x00C2B291,0x00C21AFF,0x00C184CC,0x00C0EFF4,\
0x00C05C71,0x00BFCA3F,0x00BF3958,0x00BEA9B7,0x00BE1B59,0x00BD8E37,0x00BD024E,0x00BC7799,\
0x00BBEE13,0x00BB65B9,0x00BADE86,0x00BA5876,0x00B9D385,0x00B94FAF,0x00B8CCEF,0x00B84B44,\
0x00B7CAA7,0x00B74B17,0x00B6CC8E,0x00B64F0A,0x00B5D288,0x00B55703,0x00B4DC78,0x00B462E5,\
0x00B3EA46,0x00B37297,0x00B2FBD6,0x00B285FF,0x00B21110,0x00B19D06,0x00B129DD,0x00B0B793,\
0x00B04625,0x00AFD591,0x00AF65D3,0x00AEF6E9,0x00AE88D1,0x00AE1B87,0x00ADAF0A,0x00AD4356,\
0x00ACD86A,0x00AC6E43,0x00AC04DF,0x00AB9C3B,0x00AB3454,0x00AACD2A,0x00AA66BA,0x00AA0100,\
0x00A99BFD,0x00A937AC,0x00A8D40D,0x00A8711C,0x00A80ED9,0x00A7AD41,0x00A74C52,0x00A6EC0B,\
0x00A68C69,0x00A62D6B,0x00A5CF0F,0x00A57152,0x00A51434,0x00A4B7B3,0x00A45BCC,0x00A4007E,\
0x00A3A5C8,0x00A34BA8,0x00A2F21C,0x00A29923,0x00A240BB,0x00A1E8E2,0x00A19198,0x00A13ADA,\
0x00A0E4A7,0x00A08EFE,0x00A039DD,0x009FE544,0x009F912F,0x009F3D9F,0x009EEA91,0x009E9805,\
0x009E45F9,0x009DF46B,0x009DA35C,0x009D52C8,0x009D02B0,0x009CB312,0x009C63EC,0x009C153D,\
0x009BC705,0x009B7942,0x009B2BF3,0x009ADF17,0x009A92AD,0x009A46B3,0x0099FB29,0x0099B00E,\
0x00996560,0x00991B1E,0x0098D148,0x009887DC,0x00983EDA,0x0097F640,0x0097AE0E,0x00976642,\
0x00971EDC,0x0096D7DA,0x0096913C,0x00964B01,0x00960528,0x0095BFB0,0x00957A98,0x009535DF,\
0x0094F185,0x0094AD88,0x009469E9,0x009426A5,0x0093E3BC,0x0093A12D,0x00935EF9,0x00931D1C,\
0x0092DB98,0x00929A6B,0x00925995,0x00921914,0x0091D8E9,0x00919911,0x0091598D,0x00911A5C,\
0x0090DB7D,0x00909CF0,0x00905EB3,0x009020C7,0x008FE329,0x008FA5DB,0x008F68DB,0x008F2C28,\
0x008EEFC2,0x008EB3A8,0x008E77D9,0x008E3C56,0x008E011D,0x008DC62E,0x008D8B88,0x008D512A,\
0x008D1714,0x008CDD46,0x008CA3BF,0x008C6A7E,0x008C3182,0x008BF8CC,0x008BC05A,0x008B882D,\
0x008B5043,0x008B189B,0x008AE137,0x008AAA14,0x008A7333,0x008A3C93,0x008A0633,0x0089D013,\
0x00899A33,0x00896492,0x00892F2F,0x0088FA0A,0x0088C523,0x00889079,0x00885C0C,0x008827DB,\
0x0087F3E5,0x0087C02B,0x00878CAC,0x00875967,0x0087265C,0x0086F38B,0x0086C0F3,0x00868E93,\
0x00865C6C,0x00862A7D,0x0085F8C6,0x0085C745,0x008595FB,0x008564E8,0x0085340A,0x00850363}

/* Exported macro ------------------------------------------------------------*/
#define _IQMPY_MACRO(A,B)     (signed long)( ((signed long long)A *B )>>24)
#define _IQ18MPY_MACRO(A,B)   (signed long)( ((signed long long)A *B )>>18)
#define _IQ16MPY_MACRO(A,B)   (signed long)( ((signed long long)A *B )>>16)

/* Exported functions ------------------------------------------------------- */
#endif
