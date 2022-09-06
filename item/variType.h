#ifndef TEXTMENU_ITEM_VARITYPE_H
#define TEXTMENU_ITEM_VARITYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"
#include "../textmenu_list.h"

/** @brief : 整数类型菜单项句柄。 */
typedef struct
{
    int32_t *data;
    int32_t bData;
    int32_t v, e;
    int32_t cur;
} menu_item_variHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_variType;

#endif // ! TEXTMENU_ITEM_VARITYPE_H
