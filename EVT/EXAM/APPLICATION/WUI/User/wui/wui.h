/********************************** (C) COPYRIGHT  *******************************
 * File Name          : wui.h
 * Author             : WCH
 * Version            : V1.2.0
 * Date               : 2026/07/30
 * Description        : This file contains core type definitions, HAL interface,
 *                      event system, and main API functions.
 *********************************************************************************/
#ifndef WUI_H
#define WUI_H

#include <stdint.h>
#include <stddef.h>
#include "LCD.h"


/*******************************************************************************
 *                              TYPE DEFINITIONS
 ******************************************************************************/


typedef struct {
    uint32_t addr;
    uint16_t width;
    uint16_t height;
    uint32_t data_size;
} image_info_t;


// 
#define IMAGE_TYPE_RAW   0x0000
#define IMAGE_TYPE_RLE   0x2000
#define IMAGE_TYPE_CLUT  0x4000
#define IMAGE_TYPE_CLUT4 0x6000
#define IMAGE_TYPE_MASK  0xE000
#define IMAGE_INDEX_MASK 0x1FFF
#define MAKE_IMAGE_ID(type, idx) ((type) | ((idx) & IMAGE_INDEX_MASK))
#define IMAGE_GET_TYPE(id)       ((id) & IMAGE_TYPE_MASK)
#define IMAGE_GET_INDEX(id)      ((id) & IMAGE_INDEX_MASK)


/*******************************************************************************
 *                          HAL CALLBACK TYPES
 ******************************************************************************/


typedef void (*wui_touch_read_cb_t)(uint16_t* x, uint16_t* y, uint8_t* pressed);
typedef void (*wui_flash_read_cb_t)(uint8_t* buf, uint32_t addr, uint32_t size);
typedef void (*wui_lcd_set_windows_cb_t)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
typedef void (*wui_lcd_dma_start_cb_t)(void);
typedef void (*wui_lcd_dma_send_cb_t)(const uint8_t* buf, uint32_t len);
typedef void (*wui_lcd_dma_wait_cb_t)(void);
typedef void (*wui_lcd_dma_end_cb_t)(void);

/*******************************************************************************
 *                          HAL INTERFACE
 ******************************************************************************/

/* Contains all hardware abstraction layer function pointers */
typedef struct
{
    wui_touch_read_cb_t      touch_read;      /* Touch screen read function */
    wui_flash_read_cb_t      flash_read;      /* Flash read function */
    wui_lcd_set_windows_cb_t lcd_set_windows; /* LCD window set function */
    wui_lcd_dma_start_cb_t   lcd_dma_start;   /* LCD DMA start function */
    wui_lcd_dma_send_cb_t    lcd_dma_send;    /* LCD DMA send function */
    wui_lcd_dma_wait_cb_t    lcd_dma_wait;    /* LCD DMA wait function */
    wui_lcd_dma_end_cb_t     lcd_dma_end;     /* LCD DMA end function */
} wui_hal_t;

/*******************************************************************************
 *                          PAGE DEFINITIONS
 ******************************************************************************/

/*  Page descriptor structure */
typedef struct
{
    uint16_t    bg_color;    /* Background color */
    const char* page_name;   /* Page name string */
    void (*page_init)(void); /* Page initialization function */
    void (*page_loop)(void); /* Page loop function */
    void* user_data;         /* User data pointer */
} wui_page_desc_t;

/*******************************************************************************
 *                      INITIALIZATION
 ******************************************************************************/

/*  WUI initialization data structure */
typedef struct
{
    uint16_t width;      /* Display width */
    uint16_t height;     /* Display height */
    uint16_t page_count; /* Number of pages */
    uint8_t* heap_addr;  /* Heap memory address */
    size_t   heap_size;  /* Heap memory size */
} wui_init_data_t;



/*******************************************************************************
 *                          FONT SYSTEM
 ******************************************************************************/

/*  Font structure */
typedef struct
{
    uint16_t height;       /* Font height */
    uint16_t width;        /* Font width */
    uint8_t  cnt;          /* Character count */
    uint8_t  start_char;   /* Start character */
    uint16_t first_img_id; /* First image ID */
} wui_font_t;

/*  Bitmap font structure */
typedef struct
{
    uint8_t        width;       /* Font width */
    uint8_t        height;      /* Font height */
    uint8_t        start_char;  /* Start character */
    uint8_t        cnt;         /* Character count */
    const uint8_t* data;        /* Font data pointer */
} wui_bitfont_t;

/*******************************************************************************
 *                          EVENT SYSTEM
 ******************************************************************************/

/*  Event type enumeration */
typedef enum
{
    WUI_EVENT_NONE = 0, /* No event */

    WUI_EVENT_TOUCH_DOWN       = 1, /* Touch down event */
    WUI_EVENT_TOUCH_MOVE       = 2, /* Touch move event */
    WUI_EVENT_TOUCH_LONGPRESS  = 3, /* Long press event */
    WUI_EVENT_TOUCH_UP         = 4, /* Touch up event */

    WUI_EVENT_WIDGET_CLICK       = 0x10, /* Click event */
    WUI_EVENT_WIDGET_LONG_PRESS  = 0x11, /* Long press event */
    WUI_EVENT_WIDGET_CHANGE      = 0x20, /* Value change event */

} _wui_event_t;

typedef uint16_t wui_event_t;

/*******************************************************************************
 *                          TOUCH AREA
 ******************************************************************************/

typedef struct
{
    uint16_t widget_id; /* Widget ID */
    uint16_t x, y, w, h; /* Touch area position and size */
} wui_touch_area_t;

/*******************************************************************************
 *                          VERSION API
 ******************************************************************************/

const char* wui_get_version(void);

/*******************************************************************************
 *                      CORE INITIALIZATION API
 ******************************************************************************/

void wui_init(wui_init_data_t* data);
void wui_register_hal(const wui_hal_t* hal);


/*******************************************************************************
 *                      PAGE MANAGEMENT API
 ******************************************************************************/


void wui_page_switch(uint16_t page);
void wui_page_ui_init(void);
void wui_page_ui_loop(void);
const wui_page_desc_t* wui_page_get_desc(uint16_t page);
const char* wui_page_get_name(uint16_t page);

/*******************************************************************************
 *                      UI UPDATE API
 ******************************************************************************/

uint32_t wui_page_ui_update(void);
void wui_page_ui_tick(uint32_t delta_ms);

/*******************************************************************************
 *                      KEY EVENT API
 ******************************************************************************/

void wui_register_key_handler(void (*handler)(uint32_t key));
void wui_dispatch_key(void);
void wui_set_key_event(uint32_t key);

/*******************************************************************************
 *                      TOUCH LAYER API
 ******************************************************************************/


void wui_register_touch_layer(const wui_touch_area_t* areas, uint8_t count, void (*on_event)(uint16_t widget_id, wui_event_t event));
void wui_touch_area_set_enabled(uint16_t widget_id, uint8_t enabled);

#endif /* WUI_H */