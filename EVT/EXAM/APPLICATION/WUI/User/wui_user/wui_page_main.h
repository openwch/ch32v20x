#ifndef WUI_PAGE_MAIN_H
#define WUI_PAGE_MAIN_H

#include "wui.h"
#include "wui_user/wui_page.h"
#include "wui_widget.h"
#include "images.h"
#include "wui_asset.h"
#include <stdio.h>

// ================= Widget ID Enum =================
typedef enum
{
    MAIN_MAIN_BG_BLACK,
    MAIN_MAIN_BG,
    MAIN_MAIN_TIME,
    MAIN_MAIN_CONNECTION,
    MAIN_MAIN_BATTERY,
    MAIN_MAIN_SETTING,
    MAIN_MAIN_MODE,
    MAIN_MAIN_ANIM,
    MAIN_MAIN_REAL_DATA,
    MAIN_MAIN_LIGHT_COLOR,
    MAIN_MAIN_COLOR_CFG,
} main_widget_id_t;

// ================= Declarative Widgets =================
static const wui_widget_desc_t widgets_main[] = {
    [MAIN_MAIN_BG_BLACK] = {
        .widget_id = MAIN_MAIN_BG_BLACK,
        .type      = Wui_w_Rect,
        .x         = 0,
        .y         = 0,
        .w         = 320,
        .h         = 240,
        .props.rect = {
            .color = RGB888_TO_RGB565(0, 0, 0),
        },
    },

    [MAIN_MAIN_BG] = {
        .widget_id = MAIN_MAIN_BG,
        .type      = Wui_w_StaticImage,
        .x         = 0,
        .y         = 0,
        .w         = 320,
        .h         = 240,
        .props.staticimg = {
            .img_id = IMAGE_RES_IMG_TPL_MAIN_BG,
        },
    },

    [MAIN_MAIN_TIME] = {
        .widget_id = MAIN_MAIN_TIME,
        .type      = Wui_w_ExString,
        .x         = 12,
        .y         = 4,
        .w         = 50,
        .h         = 26,
        .props.exstring = {
            .str_id   = WUI_STR_MAIN_MAIN_TIME,
            .font_id  = WUI_FONT_WUI_PAGE_MAIN_MAIN_TIME,
            .bg_color = RGB888_TO_RGB565(0, 0, 0),
            .align    = WUI_EXSTRING_ALIGN_LEFT,
        },
    },

    [MAIN_MAIN_CONNECTION] = {
        .widget_id = MAIN_MAIN_CONNECTION,
        .type      = Wui_w_MultiState,
        .x         = 119,
        .y         = 4,
        .w         = 26,
        .h         = 26,
        .props.multistate = {
            .imgtable_id   = WUI_IMGTABLE_MAIN_MAIN_CONNECTION,
            .state_count   = 3,
            .default_state = 2,
        },
    },

    [MAIN_MAIN_BATTERY] = {
        .widget_id = MAIN_MAIN_BATTERY,
        .type      = Wui_w_MultiState,
        .x         = 280,
        .y         = 4,
        .w         = 26,
        .h         = 26,
        .props.multistate = {
            .imgtable_id   = WUI_IMGTABLE_MAIN_MAIN_BATTERY,
            .state_count   = 22,
            .default_state = 0,
        },
    },

    [MAIN_MAIN_SETTING] = {
        .widget_id = MAIN_MAIN_SETTING,
        .type      = Wui_w_Button,
        .x         = 24,
        .y         = 42,
        .w         = 73,
        .h         = 99,
        .props.button = {
            .pressed_img_id  = IMAGE_RES_IMG_TPL_MAIN_PAGESETTING2,
            .normal_img_id = IMAGE_RES_IMG_TPL_MAIN_PAGESETTING1,
            .is_pressed      = 0,
        },
    },

    [MAIN_MAIN_MODE] = {
        .widget_id = MAIN_MAIN_MODE,
        .type      = Wui_w_Button,
        .x         = 127,
        .y         = 42,
        .w         = 73,
        .h         = 99,
        .props.button = {
            .pressed_img_id  = IMAGE_RES_IMG_TPL_MAIN_PAGEMODEL2,
            .normal_img_id = IMAGE_RES_IMG_TPL_MAIN_PAGEMODEL1,
            .is_pressed      = 0,
        },
    },

    [MAIN_MAIN_ANIM] = {
        .widget_id = MAIN_MAIN_ANIM,
        .type      = Wui_w_Button,
        .x         = 226,
        .y         = 42,
        .w         = 73,
        .h         = 99,
        .props.button = {
            .pressed_img_id  = IMAGE_RES_IMG_TPL_MAIN_PAGEANIMAL2,
            .normal_img_id = IMAGE_RES_IMG_TPL_MAIN_PAGEANIMAL1,
            .is_pressed      = 0,
        },
    },

    [MAIN_MAIN_REAL_DATA] = {
        .widget_id = MAIN_MAIN_REAL_DATA,
        .type      = Wui_w_Button,
        .x         = 24,
        .y         = 141,
        .w         = 73,
        .h         = 99,
        .props.button = {
            .pressed_img_id  = IMAGE_RES_IMG_TPL_MAIN_PAGERELDATA2,
            .normal_img_id = IMAGE_RES_IMG_TPL_MAIN_PAGERELDATA1,
            .is_pressed      = 0,
        },
    },

    [MAIN_MAIN_LIGHT_COLOR] = {
        .widget_id = MAIN_MAIN_LIGHT_COLOR,
        .type      = Wui_w_Button,
        .x         = 127,
        .y         = 141,
        .w         = 73,
        .h         = 99,
        .props.button = {
            .pressed_img_id  = IMAGE_RES_IMG_TPL_MAIN_PAGELIGHTCOLOR2,
            .normal_img_id = IMAGE_RES_IMG_TPL_MAIN_PAGELIGHTCOLOR1,
            .is_pressed      = 0,
        },
    },

    [MAIN_MAIN_COLOR_CFG] = {
        .widget_id = MAIN_MAIN_COLOR_CFG,
        .type      = Wui_w_Button,
        .x         = 226,
        .y         = 141,
        .w         = 73,
        .h         = 99,
        .props.button = {
            .pressed_img_id  = IMAGE_RES_IMG_TPL_MAIN_PAGELIGHTCFG2,
            .normal_img_id = IMAGE_RES_IMG_TPL_MAIN_PAGELIGHTCFG1,
            .is_pressed      = 0,
        },
    },

};


#endif // WUI_PAGE_MAIN_H
