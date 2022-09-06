#ifndef TEXTMENU_ITEM_BOOLYPE_H
#define TEXTMENU_ITEM_BOOLYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"
#include "../textmenu_list.h"

/** @brief : 布尔类型菜单项句柄。 */
typedef struct
{
    uint32_t *data;
} menu_item_boolHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_boolType;

#endif // ! TEXTMENU_ITEM_BOOLYPE_H
