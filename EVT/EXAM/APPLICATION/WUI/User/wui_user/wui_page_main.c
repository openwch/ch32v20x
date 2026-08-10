#include "wui_user/wui_page_main.h"

// ================= Template Page =================
// Template: main menu
// Template interaction logic is generated from known template IDs.

// ================= Template Logic Hooks =================

#define MENU_COUNT 6

static const uint8_t menu_widget_ids[MENU_COUNT] = {
    MAIN_MAIN_SETTING,
    MAIN_MAIN_MODE,
    MAIN_MAIN_ANIM,
    MAIN_MAIN_REAL_DATA,
    MAIN_MAIN_LIGHT_COLOR,
    MAIN_MAIN_COLOR_CFG,
};

static const wui_page_t menu_pages[MENU_COUNT] = {
    WUI_PAGE_MAIN,
    WUI_PAGE_MAIN,
    WUI_PAGE_MAIN,
    WUI_PAGE_MAIN,
    WUI_PAGE_MAIN,
    WUI_PAGE_MAIN,
};

static uint8_t main_saved_selection = 0;
static uint8_t current_menu = 0;

static void menu_set(uint8_t index, uint8_t on)
{
    if (index < MENU_COUNT)
    {
       if (on)
       {
           wui_widget_button_press(menu_widget_ids[index]);
       }
       else
       {
           wui_widget_button_release(menu_widget_ids[index]);
       }
    }
}

static void main_on_key(uint32_t key)
{
    if (key == 'a')
    {
        menu_set(current_menu, 0);
        current_menu = (current_menu == 0) ? MENU_COUNT - 1 : current_menu - 1;
        menu_set(current_menu, 1);
    }
    else if (key == 'd')
    {
        menu_set(current_menu, 0);
        current_menu = (current_menu + 1 >= MENU_COUNT) ? 0 : current_menu + 1;
        menu_set(current_menu, 1);
    }
    else if (key == 'b')
    {
        main_saved_selection = current_menu;
        wui_page_switch(menu_pages[current_menu]);
    }
}

// ================= Page Functions =================

void wui_page_main_init(void)
{
    wui_page_init(widgets_main, sizeof(widgets_main) / sizeof(widgets_main[0]));
    current_menu = main_saved_selection;
    menu_set(current_menu, 1);
    wui_register_key_handler(main_on_key);
}

void wui_page_main_loop(void)
{
}
