#ifndef TEXTMENU_ITEM_NULLTYPE_H
#define TEXTMENU_ITEM_NULLTYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"
#include "../textmenu_list.h"

/** @brief : 占位类型菜单项句柄。 */
typedef struct
{
    void *data;
} menu_item_nullHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_nullType;

#endif // ! TEXTMENU_ITEM_NULLTYPE_H
