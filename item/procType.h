#ifndef TEXTMENU_ITEM_PROCTYPE_H
#define TEXTMENU_ITEM_PROCTYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"

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

/** @brief : 函数类型菜单项操作函数。 */
void MENU_ItemConstruct_procType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_procType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemSetData_procType(menu_itemIfce_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_procType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_procType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_procType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_procType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);

#endif // ! TEXTMENU_ITEM_PROCTYPE_H
