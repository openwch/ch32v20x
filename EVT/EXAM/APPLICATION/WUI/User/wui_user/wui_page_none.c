#include "wui_user/wui_page_none.h"

static void none_on_key(uint32_t key)
{
    wui_page_switch(WUI_START_PAGE);
}

void wui_page_none_init()
{
    wui_page_init(widgets_none, sizeof(widgets_none) / sizeof(widgets_none[0]));
    wui_register_key_handler(none_on_key);
}

void wui_page_none_loop()
{
}
