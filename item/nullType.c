#include "nullType.h"

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

/**
 * @brief : 占位类型菜单项的操作句柄及操作函数。
 */
const menu_itemAdapter_t menu_itemAdapter_nullType =
{
    .ItemConstruct = MENU_ItemConstruct_nullType,
    .ItemGetData = MENU_ItemGetData_nullType,
    .ItemSetData = MENU_ItemSetData_nullType,
    .ItemPrintSlot = MENU_ItemPrintSlot_nullType,
    .ItemDirectKeyOp = MENU_ItemDirectKeyOp_nullType,
    .ItemPrintDisp = MENU_ItemPrintDisp_nullType,
    .ItemKeyOp = MENU_ItemKeyOp_nullType,
};
void MENU_ItemConstruct_nullType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_nullType;
    _item->p_handle = malloc(sizeof(menu_item_nullHandle_t));
    menu_item_nullHandle_t *p_nullType = (menu_item_nullHandle_t*)(_item->p_handle);
    p_nullType->data = _data;
}
void MENU_ItemGetData_nullType(menu_itemIfce_t *_item, void *_data)
{
    //blank
}
void MENU_ItemSetData_nullType(menu_itemIfce_t *_item, void *_data)
{
    //blank
}
//used when in menuList
void MENU_ItemPrintSlot_nullType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " ------------------- ");
    menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum] + 3, TEXTMENU_DISPLAY_STRBUF_COL - 3, "%s", _item->nameStr) + 3] = '-';
}
void MENU_ItemDirectKeyOp_nullType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    *_op = 0;
}
//used when in menuItem
void MENU_ItemPrintDisp_nullType(menu_itemIfce_t *_item)
{
    assert(0); //should never end up here.
}
void MENU_ItemKeyOp_nullType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    assert(0); //should never end up here.
}
