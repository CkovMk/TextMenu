#include "boolType.h"

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

/**
 * @brief : 布尔类型菜单项的操作句柄及操作函数。
 */
const menu_itemAdapter_t menu_itemAdapter_boolType =
{
    .ItemConstruct = MENU_ItemConstruct_boolType,
    .ItemGetData = MENU_ItemGetData_boolType,
    .ItemSetData = MENU_ItemSetData_boolType,
    .ItemPrintSlot = MENU_ItemPrintSlot_boolType,
    .ItemDirectKeyOp = MENU_ItemDirectKeyOp_boolType,
    .ItemPrintDisp = MENU_ItemPrintDisp_boolType,
    .ItemKeyOp = MENU_ItemKeyOp_boolType,
};

void MENU_ItemConstruct_boolType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_boolType;
    _item->p_handle = malloc(sizeof(menu_item_boolHandle_t));
    menu_item_boolHandle_t *p_boolType = (menu_item_boolHandle_t*)(_item->p_handle);
    p_boolType->data = (bool*)_data;
}
void MENU_ItemGetData_boolType(menu_itemIfce_t *_item, void *_data)
{
}
void MENU_ItemSetData_boolType(menu_itemIfce_t *_item, void *_data)
{
}
//used when in menuList
void MENU_ItemPrintSlot_boolType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " bool Type - - - - ");
}
void MENU_ItemDirectKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
}
//used when in menuItem
void MENU_ItemPrintDisp_boolType(menu_itemIfce_t *_item)
{
    assert(0); //should never end up here.
}
void MENU_ItemKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    assert(0); //should never end up here.
}
