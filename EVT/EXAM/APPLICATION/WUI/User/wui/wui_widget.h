/********************************** (C) COPYRIGHT  *******************************
 * File Name          : wui_widget.h
 * Author             : WCH
 * Version            : V1.2.0
 * Date               : 2026/07/30
 * Description        : This file contains widget type definitions, properties structures,
 *                      and API functions for widget manipulation.
 *********************************************************************************/
#ifndef WUI_WIDGET_H
#define WUI_WIDGET_H
#include <stdint.h>
#include "wui.h"


/*******************************************************************************
 *                              TYPE DEFINITIONS
 ******************************************************************************/

/* Widget ID types */
typedef uint16_t wui_str_id_t;           /* String resource ID */
typedef uint16_t wui_font_id_t;          /* Font resource ID */
typedef uint16_t wui_bitmap_font_id_t;   /* Bitmap font resource ID */
typedef uint16_t wui_imgtable_id_t;      /* Image table resource ID */
typedef uint16_t wui_swgroup_id_t;       /* Switch group resource ID */

/*******************************************************************************
 *                              WIDGET TYPES
 ******************************************************************************/

/**
 * @brief Widget type enumeration
 */
typedef enum
{
    Wui_w_None,         /* Invalid widget type */
    Wui_w_Anime,        /* Animation widget */
    Wui_w_Switch,       /* Switch widget */
    Wui_w_Res0,         /* Reserved 0 */    
    Wui_w_MultiState,   /* Multi-state widget */
    Wui_w_StaticImage,  /* Static image widget */
    Wui_w_QRCode,       /* QR code widget */
    Wui_w_Rect,         /* Rectangle widget */
    Wui_w_Res1,         /* Reserved 1 */
    Wui_w_Res2,         /* Reserved 2 */
    Wui_w_Res3,         /* Reserved 3 */
    Wui_w_ProgressBar,  /* Progress bar widget */
    Wui_w_Button,       /* Button widget */
    Wui_w_ImgSlider,    /* Image slider widget */
    Wui_w_SimpleString, /* Simple string widget */
    Wui_w_ExString,     /* Extended string widget */
    Wui_w_COUNT         /* Widget type count */
} _wui_widget_type_t;

typedef uint16_t wui_widget_type_t;

/*******************************************************************************
 *                              CONSTANTS & MACROS
 ******************************************************************************/

/* Progress bar direction */
#define WUI_PB_HORIZONTAL        0    /* Horizontal progress bar */
#define WUI_PB_VERTICAL          1    /* Vertical progress bar */

/* Image slider direction */
#define WUI_IMGSLIDER_HORIZONTAL 0    /* Horizontal image slider */
#define WUI_IMGSLIDER_VERTICAL   1    /* Vertical image slider */

/* Simple string alignment */
#define WUI_SIMPLESTRING_ALIGN_LEFT   0    /* Left alignment */
#define WUI_SIMPLESTRING_ALIGN_RIGHT  1    /* Right alignment */
#define WUI_SIMPLESTRING_ALIGN_CENTER 2    /* Center alignment */

/* Extended string type */
#define WUI_EXSTRING_TYPE_NORMAL  0    /* Normal type */

/* Extended string alignment */
#define WUI_EXSTRING_ALIGN_LEFT   0    /* Left alignment */
#define WUI_EXSTRING_ALIGN_RIGHT  1    /* Right alignment */
#define WUI_EXSTRING_ALIGN_CENTER 2    /* Center alignment */

/*******************************************************************************
 *                          WIDGET PROPERTIES
 ******************************************************************************/

/* Animation widget properties */
typedef struct
{
    uint16_t start_img_id;  /* Start image ID */
    uint16_t frame_count;   /* Total frame count */
    uint8_t  fps;           /* Frames per second */
    uint8_t  loop;          /* Loop mode (0: no loop, 1: loop) */
} wui_widget_anime_props_t;

/* Switch widget properties*/
typedef struct
{
    uint16_t on_img_id;  /* ON state image ID */
    uint16_t off_img_id; /* OFF state image ID */
    uint8_t  is_on;      /* Current state (0: OFF, 1: ON) */
} wui_widget_switch_props_t;

/* Multi-state widget properties */
typedef struct
{
    wui_imgtable_id_t imgtable_id;    /* Image table ID */
    uint8_t           state_count;    /* Number of states */
    uint8_t           default_state;  /* Default state index */
} wui_widget_multistate_props_t;

/* Static image widget properties */
typedef struct
{
    uint16_t img_id;  /* Image ID */
} wui_widget_staticimg_props_t;

/* QR code widget properties */
typedef struct
{
    uint16_t img_id;  /* Image ID */
} wui_widget_qrcode_props_t;

/* Rectangle widget properties */
typedef struct
{
    uint16_t color;  /* Rectangle color */
} wui_widget_rect_props_t;

/* Button widget properties */
typedef struct
{
    uint16_t normal_img_id;  /* Normal state image ID */
    uint16_t pressed_img_id; /* Pressed state image ID */
    uint8_t  is_pressed;     /* Current state (0: released, 1: pressed) */
} wui_widget_button_props_t;

/* Progress bar widget properties */
typedef struct
{
    uint8_t  direction;     /* Direction (HORIZONTAL/VERTICAL) */
    uint8_t  min_value;     /* Minimum value */
    uint8_t  max_value;     /* Maximum value */
    uint8_t  current_value; /* Current value */
    uint16_t bg_color;      /* Background color */
    uint16_t fill_color;    /* Fill color */
    uint16_t thumb_color;   /* Thumb color */
    uint16_t thumb_w;       /* Thumb width */
} wui_widget_pb_props_t;

/* Image slider widget properties */
typedef struct
{
    uint16_t full_img_id;      /* Full image ID , Must be in raw format */
    uint16_t empty_img_id;      /* Empty image ID , Must be in raw format*/
    uint8_t  current_value;  /* Initialization percentage (0-100) */
    uint8_t  direction;      /* Direction (HORIZONTAL/VERTICAL) */
} wui_widget_imgslider_props_t;

/* Simple string widget properties */
typedef struct
{
    wui_str_id_t         str_id;    /* String resource ID */
    uint16_t             fg_color;  /* Foreground color */
    uint16_t             bg_color;  /* Background color */
    uint8_t              align;     /* Alignment (LEFT/RIGHT/CENTER) */
    wui_bitmap_font_id_t font_id;   /* Bitmap font ID */
} wui_widget_simplestring_props_t;

/* Extended string widget properties */
typedef struct
{
    uint8_t type;      /* String type */
    uint8_t align;     /* Alignment (LEFT/RIGHT/CENTER) */
    union
    {
        struct
        {
            uint16_t fg_color;  /* Foreground color */
            uint16_t bg_color;  /* Background color */
        };
        void* res;              /* Reserved pointer */
    };
    wui_str_id_t  str_id;   /* String resource ID */
    wui_font_id_t font_id;  /* Font ID */
} wui_widget_exstring_props_t;



/*******************************************************************************
 *                          WIDGET DESCRIPTOR
 ******************************************************************************/

/* This is the main structure for defining a widget */
typedef struct
{
    uint16_t widget_id;  /* Unique widget ID */
    uint16_t type;       /* Widget type */
    uint16_t x, y, w, h; /* Widget position and size */
    uint8_t  flags;      /* Widget flags */
    union
    {
        wui_widget_anime_props_t        anime;        /* Animation properties */
        wui_widget_switch_props_t       switch_;      /* Switch properties */
        // wui_widget_swgroup_props_t      swgroup;      /* Switch group properties */
        wui_widget_multistate_props_t   multistate;   /* Multi-state properties */
        wui_widget_staticimg_props_t    staticimg;    /* Static image properties */
        wui_widget_qrcode_props_t       qrcode;       /* QR code properties */
        wui_widget_rect_props_t         rect;         /* Rectangle properties */
        wui_widget_pb_props_t           progress;     /* Progress bar properties */
        wui_widget_button_props_t       button;       /* Button properties */
        wui_widget_imgslider_props_t    imgslider;    /* Image slider properties */
        wui_widget_simplestring_props_t simplestring; /* Simple string properties */
        wui_widget_exstring_props_t     exstring;     /* Extended string properties */
    } props;
} wui_widget_desc_t;

/*******************************************************************************
 *                          API FUNCTIONS
 ******************************************************************************/


void wui_page_init(const wui_widget_desc_t* widgets, uint8_t widget_count);

/*******************************************************************************
 *                          ANIMATION API
 ******************************************************************************/

void    wui_widget_anime_play(uint16_t widget_id);
void    wui_widget_anime_pause(uint16_t widget_id);
void    wui_widget_anime_reset(uint16_t widget_id);
uint8_t wui_widget_anime_is_completed(uint16_t widget_id);

/*******************************************************************************
 *                          SWITCH API
 ******************************************************************************/

void    wui_widget_switch_toggle(uint16_t widget_id);
void    wui_widget_switch_set_on(uint16_t widget_id, uint8_t on);
uint8_t wui_widget_switch_is_on(uint16_t widget_id);

/*******************************************************************************
 *                      MULTI-STATE API
 ******************************************************************************/

void    wui_widget_multistate_next(uint16_t widget_id);
void    wui_widget_multistate_prev(uint16_t widget_id);
void    wui_widget_multistate_set_index(uint16_t widget_id, uint8_t index);
uint8_t wui_widget_multistate_get_index(uint16_t widget_id);

/*******************************************************************************
 *                      STATIC IMAGE API
 ******************************************************************************/

uint16_t wui_widget_staticimg_get_img_id(uint16_t widget_id);
void     wui_widget_staticimg_set_img_id(uint16_t widget_id, uint16_t img_id);

/*******************************************************************************
 *                      RECTANGLE API
 ******************************************************************************/

void     wui_widget_rect_set_color(uint16_t widget_id, uint16_t color);
uint16_t wui_widget_rect_get_color(uint16_t widget_id);

/*******************************************************************************
 *                      PROGRESS BAR API
 ******************************************************************************/

void    wui_widget_pb_set_value(uint16_t widget_id, uint8_t value);
uint8_t wui_widget_pb_get_value(uint16_t widget_id);

/*******************************************************************************
 *                      IMAGE SLIDER API
 ******************************************************************************/

void    wui_widget_imgslider_set_value(uint16_t widget_id, uint8_t current_value);
uint8_t wui_widget_imgslider_get_value(uint16_t widget_id);

/*******************************************************************************
 *                          BUTTON API
 ******************************************************************************/

void    wui_widget_button_press(uint16_t widget_id);
void    wui_widget_button_release(uint16_t widget_id);
uint8_t wui_widget_button_is_pressed(uint16_t widget_id);

/*******************************************************************************
 *                      SIMPLE STRING API
 ******************************************************************************/

void        wui_widget_simplestring_set_str(uint16_t widget_id, const char* str);
const char* wui_widget_simplestring_get_str(uint16_t widget_id);
void        wui_widget_simplestring_set_align(uint16_t widget_id, uint8_t align);
uint8_t     wui_widget_simplestring_get_align(uint16_t widget_id);
void        wui_widget_simplestring_set_color(uint16_t widget_id, uint16_t fg_color, uint16_t bg_color);

/*******************************************************************************
 *                      EXTENDED STRING API
 ******************************************************************************/

void        wui_widget_exstring_set_str(uint16_t widget_id, const char* str);
const char* wui_widget_exstring_get_str(uint16_t widget_id);
void        wui_widget_exstring_set_align(uint16_t widget_id, uint8_t align);
uint8_t     wui_widget_exstring_get_align(uint16_t widget_id);

#endif /* WUI_WIDGET_H */