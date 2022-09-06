#ifndef TEXTMENU_ITEM_BYTETYPE_H
#define TEXTMENU_ITEM_BYTETYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"
#include "../textmenu_list.h"

/** @brief : 整数类型菜单项句柄。 */
typedef struct
{
    uint32_t *data;
    uint32_t bData;
    bool bits[8];
    int32_t cur;
} menu_item_byteHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_byteType;

#endif // ! TEXTMENU_ITEM_BYTETYPE_H
