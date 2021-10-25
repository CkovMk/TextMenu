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

extern menu_itemIfce_t *menu_currItem;
extern menu_list_t *menu_currList;

void MENU_ItemConstruct_boolType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_boolType;
    _item->p_handle = malloc(sizeof(menu_item_boolHandle_t));
    menu_item_boolHandle_t *p_boolType = (menu_item_boolHandle_t*)(_item->p_handle);
    p_boolType->data = (bool*)_data;
}
void MENU_ItemGetData_boolType(menu_itemIfce_t *_item, void *_data)
{
	menu_item_boolHandle_t *p_boolType = (menu_item_boolHandle_t*)(_item->p_handle);
	*(bool *)_data = *(p_boolType->data);
}
void MENU_ItemSetData_boolType(menu_itemIfce_t *_item, void *_data)
{
	menu_item_boolHandle_t *p_boolType = (menu_item_boolHandle_t*)(_item->p_handle);
	*(p_boolType->data) = *(bool *)_data;
	SYSLOG_V("boolType Data Updated %12.12d", *(p_boolType->data));
}
//used when in menuList
void MENU_ItemPrintSlot_boolType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
	menu_item_boolHandle_t *p_boolType = (menu_item_boolHandle_t*)(_item->p_handle);
	if(*(p_boolType->data))
	{
		menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s      T", _item->nameStr)] = ' ';
	}
	else
	{
		menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s      F", _item->nameStr)] = ' ';
	}
}
void MENU_ItemDirectKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
	menu_item_boolHandle_t *p_boolType = (menu_item_boolHandle_t*)(_item->p_handle);
	switch (*_op)
	{
		case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
		{
			*_op = 0;
			break;
		}
		case MENU_BUTTON_MAKE_OP(5wayStick_lf, shrt):
		case MENU_BUTTON_MAKE_OP(5wayStick_lf, long):
		case MENU_BUTTON_MAKE_OP(5wayStick_lf, lrpt):
		{
			if (!(_item->pptFlag & (menuItem_data_ROFlag | menuItem_disp_noPreview)))
			{
				*(p_boolType->data) = !(*p_boolType->data);
			}
			*_op = 0;
			break;
		}
		case MENU_BUTTON_MAKE_OP(5wayStick_rt, shrt):
		case MENU_BUTTON_MAKE_OP(5wayStick_rt, long):
		case MENU_BUTTON_MAKE_OP(5wayStick_rt, lrpt):
		{
			if (!(_item->pptFlag & (menuItem_data_ROFlag | menuItem_disp_noPreview)))
			{
				*(p_boolType->data) = !(*p_boolType->data);
			}
			*_op = 0;
			break;
		}
		default:
		//appui_menu_t::getInstance().currItem = bool;
		break;
	}
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
