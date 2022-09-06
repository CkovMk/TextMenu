#ifndef TEXTMENU_ITEM_PROCTYPE_H
#define TEXTMENU_ITEM_PROCTYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"
#include "../textmenu_list.h"

/**
 * @brief : 函数类型菜单项的响应句柄。
 * 函数类型菜单项激活时会调用一个没有参数和返回值的函数。
 */
typedef void (*menu_itemProcHandler_t)(menu_keyOp_t *const _op);

/** @brief : 函数类型菜单项句柄。 */
typedef struct
{
    menu_itemProcHandler_t data;
} menu_item_procHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_procType;

#endif // ! TEXTMENU_ITEM_PROCTYPE_H
