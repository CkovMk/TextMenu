#include "procType.h"

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

extern menu_t menu;

#ifdef __cplusplus
extern "C" {
#endif

void MENU_ItemConstruct_procType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_procType;
    _item->p_handle = malloc(sizeof(menu_item_procHandle_t));
    menu_item_procHandle_t *p_procType = (menu_item_procHandle_t*)(_item->p_handle);
    p_procType->data = (menu_itemProcHandler_t)_data;
}
void MENU_ItemGetData_procType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
    _data->size = 0U;
}
void MENU_ItemSetData_procType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
}
//used when in menuList
void MENU_ItemPrintSlot_procType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " :%-16.16s", _item->nameStr)] = ' ';
}
void MENU_ItemDirectKeyOp_procType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_procHandle_t *p_procType = (menu_item_procHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        {
            menu.status.currItem = _item;
            *_op = 0;
            break;
        }

        default:
        break;
    }
}
//used when in menuItem
void MENU_ItemPrintDisp_procType(menu_itemIfce_t *_item)
{
    menu_item_procHandle_t *p_procType = (menu_item_procHandle_t*)(_item->p_handle);
    if(_item->pptFlag & menuItem_proc_uiDisplay)    //FIXME: rework this logic
    {
        //menu_keyOp_t op_temp = MENU_BUTTON_MAKE_OP(5wayStick_nl, disp);
        p_procType->data(NULL);
        return;
    }
    else
    {
        menu.dispStrBuf.strbuf[0][snprintf(menu.dispStrBuf.strbuf[0], TEXTMENU_DISPLAY_STRBUF_COL + 1, "##%-15.15s *", _item->nameStr)] = ' ';
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
        for(int c = 0; c < TEXTMENU_DISPLAY_STRBUF_COL + 1; ++c)
        {
            menu.dispStrBuf.fcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_F;
	        menu.dispStrBuf.bcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_B;
        }
#endif // ! TEXTMENU_USE_PALETTE
        menu.dispStrBuf.strbuf[7][snprintf(menu.dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "    SOK>AC LOK>WA    ")] = ' ';
    }
}
void MENU_ItemKeyOp_procType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_procHandle_t *p_procType = (menu_item_procHandle_t*)(_item->p_handle);
    switch (*_op)
    {
    case MENU_BUTTON_MAKE_OP(5wayStick_ok, long):
        menu.status.currItem = NULL;
        *_op = 0U;
        break;
    default:
        p_procType->data(_op);
        if(_item->pptFlag & menuItem_proc_runOnce)
        {
            menu.status.currItem = NULL;
        }
        break;
    }
}

#ifdef __cplusplus
}
#endif

/**
 * @brief : 函数类型菜单项的操作句柄及操作函数。
 */
const menu_itemAdapter_t menu_itemAdapter_procType =
{
    .ItemConstruct = MENU_ItemConstruct_procType,
    .ItemGetData = MENU_ItemGetData_procType,
    .ItemSetData = MENU_ItemSetData_procType,
    .ItemPrintSlot = MENU_ItemPrintSlot_procType,
    .ItemDirectKeyOp = MENU_ItemDirectKeyOp_procType,
    .ItemPrintDisp = MENU_ItemPrintDisp_procType,
    .ItemKeyOp = MENU_ItemKeyOp_procType,
};
