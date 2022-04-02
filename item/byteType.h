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

/** @brief : 整数类型菜单项操作函数。 */
void MENU_ItemConstruct_byteType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_byteType(menu_itemIfce_t *_item, menu_itemData_t *_data);
void MENU_ItemSetData_byteType(menu_itemIfce_t *_item, menu_itemData_t *_data);
//used when in menuList
void MENU_ItemPrintSlot_byteType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_byteType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_byteType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_byteType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);

#endif // ! TEXTMENU_ITEM_BYTETYPE_H
