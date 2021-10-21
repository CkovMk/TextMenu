#ifndef TEXTMENU_ITEM_BOOLYPE_H
#define TEXTMENU_ITEM_BOOLYPE_H

#include "../textmenu_core.h"
#include "../textmenu_item.h"

/** @brief : 布尔类型菜单项句柄。 */
typedef struct
{
    bool *data;
} menu_item_boolHandle_t;

extern menu_itemAdapter_t const menu_itemAdapter_boolType;

/** @brief : 布尔类型菜单项操作函数。 */
void MENU_ItemConstruct_boolType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_boolType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemSetData_boolType(menu_itemIfce_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_boolType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_boolType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_boolType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);

#endif // ! TEXTMENU_ITEM_BOOLYPE_H
