#include "wui.h"
#include "wui_port.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "flash.h"
#include "images.h"
#include "LCD.h"
#include "spi.h"

void HW_Key_event(uint32_t _key_code)
{
    printf("[KEY] press %c\n", _key_code);
    wui_set_key_event(_key_code);
}

/*********************************************************************
 * @fn      touch_read
 *
 * @brief   Read touch screen coordinates and pressed state.
 *          v203 has no touch screen, so touch is disabled.
 *
 * @param   x - Pointer to store X coordinate.
 * @param   y - Pointer to store Y coordinate.
 * @param   pressed - Pointer to store pressed state.
 *
 * @return  none
 */
static void touch_read(uint16_t* x, uint16_t* y, uint8_t* pressed)
{
    *x       = 0;
    *y       = 0;
    *pressed = 0;
}

/*********************************************************************
 * @fn      spi_flash_read
 *
 * @brief   Read data from SPI Flash.
 *
 * @param   outBuffer - Pointer to data buffer to store read data.
 * @param   ReadAddr - Start address to read from.
 * @param   size - Number of bytes to read.
 *
 * @return  none
 */
static void spi_flash_read(uint8_t* outBuffer, uint32_t ReadAddr, uint32_t size)
{
    SPI_Flash_Read_dma_start(ReadAddr);
    SPI1_Read_DMA(outBuffer, (uint16_t)size);
    SPI1_wait_for_bsy();
    SPI_Flash_Read_dma_end();
}

/*********************************************************************
 * @fn      lcd_set_windows
 *
 * @brief   Set LCD display window.
 *
 * @param   x_start - Start X coordinate.
 * @param   y_start - Start Y coordinate.
 * @param   x_end - End X coordinate.
 * @param   y_end - End Y coordinate.
 *
 * @return  none
 */
static void lcd_set_windows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    LCD_SetWindows(x_start, y_start, x_end, y_end);
}

/*********************************************************************
 * @fn      lcd_dma_start
 *
 * @brief   Start LCD data transfer.
 *
 * @return  none
 */
static void lcd_dma_start(void)
{
    LCD_DC_SET();
    LCD_CS_CLR();
}

/*********************************************************************
 * @fn      lcd_dma_send
 *
 * @brief   Send data via LCD DMA.
 *
 * @param   buf - Pointer to data buffer to send.
 * @param   len - Length of data in bytes.
 *
 * @return  none
 */
static void lcd_dma_send(const uint8_t* buf, uint32_t len)
{
    SPI2_Write_DMA((uint8_t*)buf, (uint16_t)len);
}

/*********************************************************************
 * @fn      lcd_dma_wait
 *
 * @brief   Wait for LCD DMA transfer to complete.
 *
 * @return  none
 */
static void lcd_dma_wait(void)
{
    while (SPI2_DMA_TX_CH->CNTR > 0)
    {
    }

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET)
        ;
}

/*********************************************************************
 * @fn      lcd_dma_end
 *
 * @brief   End LCD DMA transfer.
 *
 * @return  none
 */
static void lcd_dma_end(void)
{
    LCD_CS_SET();
}

/*********************************************************************
 * @fn      wui_get_hal
 *
 * @brief   Get WUI HAL interface structure.
 *
 * @return  Pointer to HAL interface structure.
 */
const wui_hal_t* wui_get_hal(void)
{
    static const wui_hal_t hal = {
        .touch_read      = touch_read,
        .flash_read      = spi_flash_read,
        .lcd_set_windows = lcd_set_windows,
        .lcd_dma_start   = lcd_dma_start,
        .lcd_dma_send    = lcd_dma_send,
        .lcd_dma_wait    = lcd_dma_wait,
        .lcd_dma_end     = lcd_dma_end,
    };

    return &hal;
}
