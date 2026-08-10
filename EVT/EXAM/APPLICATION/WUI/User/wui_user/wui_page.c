#include "wui_page.h"
#include "wui.h"
#include "images.h"
#include <stdio.h>


const wui_page_desc_t wui_pages[WUI_PAGE_COUNT] = {
    [WUI_PAGE_NONE] = {
        .bg_color = RGB888_TO_RGB565(0, 0, 0),
        .page_name = "None",
        .page_init = wui_page_none_init,
        .page_loop = wui_page_none_loop,
        .user_data = NULL,
    },
    [WUI_PAGE_MAIN] = {
        .bg_color = RGB888_TO_RGB565(0, 0, 0),
        .page_name = "Menu Page",
        .page_init = wui_page_main_init,
        .page_loop = wui_page_main_loop,
        .user_data = NULL,
    },
};
