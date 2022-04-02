#include "menuType.h"

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

/**
 * @brief : 跳转类型菜单项的操作句柄及操作函数。
 */
const menu_itemAdapter_t menu_itemAdapter_menuType =
{
    .ItemConstruct = MENU_ItemConstruct_menuType,
    .ItemGetData = MENU_ItemGetData_menuType,
    .ItemSetData = MENU_ItemSetData_menuType,
    .ItemPrintSlot = MENU_ItemPrintSlot_menuType,
    .ItemDirectKeyOp = MENU_ItemDirectKeyOp_menuType,
    .ItemPrintDisp = MENU_ItemPrintDisp_menuType,
    .ItemKeyOp = MENU_ItemKeyOp_menuType,
};

extern menu_t menu;

void MENU_ItemConstruct_menuType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_menuType;
    _item->p_handle = malloc(sizeof(menu_item_menuHandle_t));
    menu_item_menuHandle_t *p_menuType = (menu_item_menuHandle_t*)(_item->p_handle);
    p_menuType->data = (menu_list_t*)_data;
}
void MENU_ItemGetData_menuType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
    _data->size = 0U;
}
void MENU_ItemSetData_menuType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
}
//used when in menuList
void MENU_ItemPrintSlot_menuType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " [%s]", _item->nameStr)] = ' ';
}
void MENU_ItemDirectKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_menuHandle_t *p_menuType = (menu_item_menuHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        menu.status.currList = p_menuType->data;
        *_op = 0;
        break;

        default:
        break;
    }
}
//used when in menuItem
void MENU_ItemPrintDisp_menuType(menu_itemIfce_t *_item)
{
    assert(0); //should never end up here.
}
void MENU_ItemKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    assert(0); //should never end up here.
}
