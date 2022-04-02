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

/** @brief : 跳转类型菜单项操作函数。 */
void MENU_ItemConstruct_menuType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_menuType(menu_itemIfce_t *_item, menu_itemData_t *_data);
void MENU_ItemSetData_menuType(menu_itemIfce_t *_item, menu_itemData_t *_data);
//used when in menuList
void MENU_ItemPrintSlot_menuType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_menuType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_menuType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);

#endif // ! TEXTMENU_ITEM_MENUTYPE_H
