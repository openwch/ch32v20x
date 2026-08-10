#ifndef WUI_PAGE_H
#define WUI_PAGE_H

#include <stdint.h>
#include "stddef.h"
#include "wui.h"

typedef enum
{
    WUI_PAGE_NONE = 0,
    WUI_PAGE_MAIN,
    WUI_PAGE_COUNT,
} wui_page_t;

#define WUI_START_PAGE WUI_PAGE_MAIN

void wui_page_none_init(void);
void wui_page_none_loop(void);

void wui_page_main_init(void);
void wui_page_main_loop(void);

extern const wui_page_desc_t wui_pages[WUI_PAGE_COUNT];

#endif
