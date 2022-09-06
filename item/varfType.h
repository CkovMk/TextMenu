#ifndef TEXTMENU_ITEM_VARFTYPE_H
#define TEXTMENU_ITEM_VARFTYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"
#include "../textmenu_list.h"

/** @brief : 浮点类型菜单项句柄。 */
typedef struct
{
    float *data;
    float bData;
    int32_t v, e;
    int32_t cur;
} menu_item_varfHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_varfType;

#endif // ! TEXTMENU_ITEM_VARFTYPE_H
