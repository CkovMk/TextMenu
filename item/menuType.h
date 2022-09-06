#ifndef TEXTMENU_ITEM_MENUTYPE_H
#define TEXTMENU_ITEM_MENUTYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"
#include "../textmenu_list.h"

/** 菜单列表结构体 前置定义 */
typedef struct _menu_list menu_list_t;

/** @brief : 跳转类型菜单项句柄。 */
typedef struct
{
    menu_list_t *data;
} menu_item_menuHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_menuType;

#endif // ! TEXTMENU_ITEM_MENUTYPE_H
