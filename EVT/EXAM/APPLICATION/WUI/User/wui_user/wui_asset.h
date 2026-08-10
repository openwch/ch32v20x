/********************************** (C) COPYRIGHT  *******************************
 * File Name          : wui_asset.h
 * Author             : WCH
 * Version            : V1.2.0
 * Date               : 2026/07/30
 * Description        : Asset management functions
 *********************************************************************************/

#ifndef WUI_ASSET_H
#define WUI_ASSET_H

#include <stdint.h>
#include "wui.h"
#include "images.h"
#include "wui_widget.h"

enum
{
    WUI_STR_NONE = 0,
    WUI_STR_MAIN_MAIN_TIME,
    WUI_STR_COUNT
};

const char* wui_get_string(wui_str_id_t id);

enum
{
    WUI_FONT_NONE = 0,
    WUI_FONT_WUI_PAGE_MAIN_MAIN_TIME,
    WUI_FONT_COUNT
};

const wui_font_t* wui_get_font(wui_font_id_t id);

enum
{
    WUI_IMGTABLE_NONE = 0,
    WUI_IMGTABLE_MAIN_MAIN_BATTERY,
    WUI_IMGTABLE_MAIN_MAIN_CONNECTION,
    WUI_IMGTABLE_COUNT
};

const uint16_t* wui_get_imgtable(wui_imgtable_id_t id);


enum {
    WUI_BITMAP_FONT_DEFAULT = 0,
    WUI_BITMAP_FONT_6X12,
    WUI_BITMAP_FONT_8X16,

    WUI_BITMAP_FONT_COUNT
};

const wui_bitfont_t* wui_get_bitmap_font(wui_bitmap_font_id_t id);

const image_info_t* wui_get_image(uint32_t index);

#endif
