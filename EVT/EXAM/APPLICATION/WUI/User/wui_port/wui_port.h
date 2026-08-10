#ifndef __WUI_PORT_H
#define __WUI_PORT_H

#include <stdint.h>
#include "wui.h"

void HW_Key_event(uint32_t _key_code);

const wui_hal_t* wui_get_hal(void);

#endif // __WUI_PORT_H
