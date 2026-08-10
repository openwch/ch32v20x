#ifndef WUI_PAGE_NONE_H
#define WUI_PAGE_NONE_H

#include "wui.h"
#include "wui_user/wui_page.h"
#include "wui_widget.h"

typedef enum
{
    WIDGET_NONE_BG = 0,
} none_widget_id_t;

static const wui_widget_desc_t widgets_none[] = {
    [WIDGET_NONE_BG] = {
        .widget_id  = WIDGET_NONE_BG,
        .type       = Wui_w_Rect,
        .x          = 0,
        .y          = 0,
        .w          = LCD_WIDTH,
        .h          = LCD_HEIGHT,
        .props.rect = {
            .color = LCD_black,
        },
    },
};

#endif // WUI_PAGE_NONE_H
