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

/** @brief : 整数类型菜单项操作函数。 */
void MENU_ItemConstruct_variType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_variType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemSetData_variType(menu_itemIfce_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_variType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_variType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_variType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);

#endif // ! TEXTMENU_ITEM_VARITYPE_H
