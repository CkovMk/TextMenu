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

/** @brief : 浮点类型菜单项操作函数。 */
void MENU_ItemConstruct_varfType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_varfType(menu_itemIfce_t *_item, menu_itemData_t *_data);
void MENU_ItemSetData_varfType(menu_itemIfce_t *_item, menu_itemData_t *_data);
//used when in menuList
void MENU_ItemPrintSlot_varfType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_varfType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_varfType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);

#endif // ! TEXTMENU_ITEM_VARFTYPE_H
