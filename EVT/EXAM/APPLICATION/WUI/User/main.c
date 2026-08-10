/*********************************************************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2026/08/06
 * Description        : Main program body.
 *********************************************************************************
* Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 A demo for wui
  pins: 
    FLASH_CS   -- PA4
    FLASH_DO   -- PA6
    FLASH_DI   -- PA7
    FLASH_CLK  -- PA5
    
    LCD_LED    -- PB9
    LCD_DC     -- PB10
    LCD_RST    -- PB11
    LCD_CS     -- PB12
    LCD_SCK    -- PB13
    LCD_MOSI   -- PB15


 */


#include <math.h>
#include "debug.h"
#include "string.h"
#include "lcd.h"
#include "flash.h"
#include "config.h"
#include "wui.h"
#include "wui_port.h"
#include "wui_user/wui_page.h"

/* Global define */

/* Global Variable */


#define TIM_PCK (8000ul)

/*********************************************************************
 * @fn      TIM4_init
 *
 * @brief   Initialize TIM2 for WCHGUI
 *        
 * @return  none.
 */
void TIM4_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    TIM_Cmd(TIM4, DISABLE);

    TIM_TimeBaseInitStructure.TIM_Period        = 0xffff;
    TIM_TimeBaseInitStructure.TIM_Prescaler     = ((SystemCoreClock) / TIM_PCK) - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    TIM_ARRPreloadConfig(TIM4, DISABLE);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);

    TIM_Cmd(TIM4, ENABLE);
}

/*********************************************************************
 * @fn      TIM3_Encoder_Init
 *
 * @brief   Initialize TIM1 for encoder
 *        
 * @return  none.
 */
void TIM3_Encoder_Init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef       TIM_ICInitStructure;
    GPIO_InitTypeDef        GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    {
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler     = 0x0;
    TIM_TimeBaseStructure.TIM_Period        = 160;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);
}

/*********************************************************************
 * @fn      check_direction
 *
 * @brief    Compute the absolute value of a signed 32-bit integer.
 *        
 * @param   a Signed 32-bit integer.
 *
 * @return  Absolute value of a 
 */
uint32_t static inline iabs(int32_t a)
{
    if (a < 0)
    {
        return -a;
    }
    return a;
}

/*********************************************************************
 * @fn      check_direction
 *
 * @brief   Detect the rotation direction of the encoder
 *        
 * @param   current_value The current count value
 *
 * @return  direction The direction of the encoder's rotation
 */
int32_t check_direction(int current_value)
{

    static int last_value = 0;

    if (current_value == last_value)
    {
        return 0;
    }

    int diff = current_value - last_value;
    if (iabs(diff) < 4)
    {
        return 0;
    }

    int32_t direction;

    if (iabs(diff) < 80)
    {
        if (diff > 0)
        {
            direction = 1;
        }
        else
        {
            direction = -1;
        }
    }
    else
    {
        if (diff > 0)
        {
            direction = -1;
        }
        else
        {
            direction = 1;
        }
    }

    last_value = current_value;
    return direction;
}

#define ENC_KEY_PORT GPIOA
#define ENC_KEY_PIN  (GPIO_Pin_15)

#define ENC_KEY()    ((ENC_KEY_PORT->INDR & ENC_KEY_PIN) == 0)

uint32_t key3_state = 0;
/*********************************************************************
 * @fn      Key_GPIOInit
 *
 * @brief   Initialization of the key pins of the encoder
 *        
 * @return  none.
 */
void Key_GPIOInit(void)
{

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin  = ENC_KEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(ENC_KEY_PORT, &GPIO_InitStructure);
}

static uint8_t wui_heap_buffer[4 * 1024];

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{

    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf(__TIME__ "\n");

    SPI_FLASH_Init();

    uint32_t id = SPI_Flash_ReadJedecDeviceID();
    printf("flash_id %x\n", id);

    LCD_Init();

    TIM3_Encoder_Init();
    TIM4_init();
    Key_GPIOInit();

    wui_init_data_t data = {
        .page_count = WUI_PAGE_COUNT,
        .width      = LCD_WIDTH,
        .height     = LCD_HEIGHT,
        .heap_addr  = wui_heap_buffer,
        .heap_size  = sizeof(wui_heap_buffer),
    };

    wui_init(&data);

    wui_register_hal(wui_get_hal());

    wui_page_switch(WUI_START_PAGE);

    while (1)
    {

        uint16_t now = TIM4->CNT;
        if (now > 16)
        {
            now       = now / 8;
            TIM4->CNT = 0;

            wui_page_ui_tick(now);
        }

        wui_page_ui_update();
        uint32_t tcnt = TIM3->CNT;

        int32_t dir = check_direction(tcnt);

        if (dir > 0)
        {
            HW_Key_event('a');
        }
        else if (dir < 0)
        {
            HW_Key_event('d');
        }

        uint32_t k3 = ENC_KEY();
        if (k3)
        {
            if (key3_state)
            {
            }
            else
            {
                key3_state = 1;
                HW_Key_event('b');
            }
        }
        else
        {
            key3_state = 0;
        }
    }

    while (1)
    {
    }
}
