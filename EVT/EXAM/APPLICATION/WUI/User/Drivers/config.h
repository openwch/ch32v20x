/*********************************************************************
 * File Name          : config.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/8/15
 * Description        : file for Config.
 *********************************************************************************
* Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* SPI FLASH and LCD_SCREEN GPIO Definition */
#define FLASH_CS_PORT GPIOA
#define FLASH_CS_RCC  RCC_APB2Periph_GPIOA
#define FLASH_CS_PIN  (GPIO_Pin_4)

#define LCD_LED_RCC   RCC_APB2Periph_GPIOB
#define LCD_LED_PORT  GPIOB
#define LCD_LED_PIN   (GPIO_Pin_9)

#define LCD_DC_RCC    RCC_APB2Periph_GPIOB
#define LCD_DC_PORT   GPIOB
#define LCD_DC_PIN    (GPIO_Pin_10)

#define LCD_RST_RCC   RCC_APB2Periph_GPIOB
#define LCD_RST_PORT  GPIOB
#define LCD_RST_PIN   (GPIO_Pin_11)

#define LCD_CS_RCC    RCC_APB2Periph_GPIOB
#define LCD_CS_PORT   GPIOB
#define LCD_CS_PIN    (GPIO_Pin_12)

#endif
