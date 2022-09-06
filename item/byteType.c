#include "byteType.h"

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

extern menu_t menu;

#ifdef __cplusplus
extern "C" {
#endif

void MENU_ItemGetContent_byteType(bool *const bits, uint8_t data)
{
    for(uint32_t i = 0U; i < 8U; ++i)
    {
    	bits[i] = (data >> i) & 0x01U;
    }
}
void MENU_ItemSetContent_byteType(uint8_t *const data, bool const *bits)
{
    (*data) = 0U;
    for(uint32_t i = 0U; i < 8U; ++i)
    {
    	(*data) |= (bits[i] << i);
    }
}

void MENU_ItemConstruct_byteType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_byteType;
    _item->p_handle = malloc(sizeof(menu_item_byteHandle_t));
    menu_item_byteHandle_t *p_byteType = (menu_item_byteHandle_t*)(_item->p_handle);
    p_byteType->data = (uint8_t*)_data;
    p_byteType->cur = 0;
    MENU_ItemGetContent_byteType(p_byteType->bits, *p_byteType->data);
}
void MENU_ItemGetData_byteType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
    menu_item_byteHandle_t *p_byteType = (menu_item_byteHandle_t*)(_item->p_handle);
    
    _data->size = sizeof(uint32_t);
    _data->pData = p_byteType->data;
    SYSLOG_V("Get byteType Data: %12.12d", *(p_byteType->data));
}
void MENU_ItemSetData_byteType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
    menu_item_byteHandle_t *p_byteType = (menu_item_byteHandle_t*)(_item->p_handle);
    uint32_t *pData = p_byteType->data;

    if(4U == _data->size)
    {
        *pData = (*(uint32_t *)(_data->pData)) & 0xffU;
        SYSLOG_V("Set byteType data: %12.12d", *pData);
    }
    else
    {
        SYSLOG_W("Set byteType data: Failed. -size-match");
    }
}
//used when in menuList
void MENU_ItemPrintSlot_byteType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu_item_byteHandle_t *p_byteType = (menu_item_byteHandle_t*)(_item->p_handle);
    MENU_ItemGetContent_byteType(p_byteType->bits, *p_byteType->data);
    if (_item->pptFlag & menuItem_disp_noPreview)
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-16.16s  ->", _item->nameStr)];
    }
    else
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s B-0x%2.2x ", _item->nameStr, *(p_byteType->data))];
    }
}
void MENU_ItemDirectKeyOp_byteType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_byteHandle_t *p_byteType = (menu_item_byteHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        {
        	MENU_ItemGetContent_byteType(p_byteType->bits, *p_byteType->data);
            menu.status.currItem = _item;
            *_op = 0;
            break;
        }
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, lrpt):
        {
            if (!(_item->pptFlag & (menuItem_data_ROFlag | menuItem_disp_noPreview)))
            {
                --*p_byteType->data;

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
                ++*p_byteType->data;
            }
            *_op = 0;
            break;
        }
        default:
        //appui_menu_t::getInstance().currItem = NULL;
        break;
    }
}
//used when in menuItem
void MENU_ItemPrintDisp_byteType(menu_itemIfce_t *_item)
{
    menu_item_byteHandle_t *p_byteType = (menu_item_byteHandle_t*)(_item->p_handle);

    menu.dispStrBuf.strbuf[0][snprintf(menu.dispStrBuf.strbuf[0], TEXTMENU_DISPLAY_STRBUF_COL + 1, "##%-15.15s *", _item->nameStr)] = ' ';
    if (_item->pptFlag & menuItem_data_global)
    {
        menu.dispStrBuf.strbuf[0][19] = 'G';
        menu.dispStrBuf.strbuf[0][20] = 'L';
    }
    else if (_item->pptFlag & menuItem_data_region)
    {
        menu.dispStrBuf.strbuf[0][19] = 'R';
        menu.dispStrBuf.strbuf[0][20] = 'G';
    }
    else
    {
        menu.dispStrBuf.strbuf[0][19] = 'N';
        menu.dispStrBuf.strbuf[0][20] = 'O';
    }
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
    for(int c = 0; c < TEXTMENU_DISPLAY_STRBUF_COL + 1; ++c)
    {
        menu.dispStrBuf.fcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_F;
	    menu.dispStrBuf.bcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_B;
    }
#endif // ! TEXTMENU_USE_PALETTE

    menu.dispStrBuf.strbuf[2][snprintf(menu.dispStrBuf.strbuf[2], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Cur: B-0x%2.2x (%3.3d)", *p_byteType->data, *p_byteType->data)] = ' ';
    if (_item->pptFlag & menuItem_data_ROFlag)
    {
        menu.dispStrBuf.strbuf[7][snprintf(menu.dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "     READ   ONLY     ")] = ' ';
    }
    else
    {
        menu.dispStrBuf.strbuf[3][snprintf(menu.dispStrBuf.strbuf[3], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Adj: B-0x%2.2x (%3.3d)", p_byteType->bData, p_byteType->bData)] = ' ';
        menu.dispStrBuf.strbuf[4][snprintf(menu.dispStrBuf.strbuf[4], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Mod: %1.1d%1.1d%1.1d%1.1d%1.1d%1.1d%1.1d%1.1d",
        		p_byteType->bits[7],p_byteType->bits[6],p_byteType->bits[5],p_byteType->bits[4],p_byteType->bits[3],p_byteType->bits[2],p_byteType->bits[1],p_byteType->bits[0])] = ' ';
        int32_t pos = 14 - p_byteType->cur;
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
        menu.dispStrBuf.fcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F;
	    menu.dispStrBuf.bcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B;
#endif // ! TEXTMENU_USE_PALETTE
        menu.dispStrBuf.strbuf[5][pos] = '^';
        menu.dispStrBuf.strbuf[7][snprintf(menu.dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "    SOK>AC LOK>WA    ")] = ' ';
    }
}
void MENU_ItemKeyOp_byteType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_byteHandle_t *p_byteType = (menu_item_byteHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        if (!(_item->pptFlag & menuItem_data_ROFlag))
        {
            (*p_byteType->data) = (p_byteType->bData);
        }
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, long):
        menu.status.currItem = NULL;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, long):
        p_byteType->cur = 7;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, long):
        p_byteType->cur = 0;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, shrt):
        if (p_byteType->cur < 7)
        {
            ++p_byteType->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, shrt):
        if (p_byteType->cur > 0)
        {
            --p_byteType->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_up, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, lrpt):
        p_byteType->bits[p_byteType->cur] = true;
        MENU_ItemSetContent_byteType(&p_byteType->bData, p_byteType->bits);
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, lrpt):
		p_byteType->bits[p_byteType->cur] = false;
        MENU_ItemSetContent_byteType(&p_byteType->bData, p_byteType->bits);
        *_op = 0;
        break;
        default:
        //appui_menu_t::getInstance().currItem = NULL;
        break;
    }
}

#ifdef __cplusplus
}
#endif

/**
 * @brief : 整数类型菜单项的操作句柄及操作函数。
 */
const menu_itemAdapter_t menu_itemAdapter_byteType =
{
    .ItemConstruct = MENU_ItemConstruct_byteType,
    .ItemGetData = MENU_ItemGetData_byteType,
    .ItemSetData = MENU_ItemSetData_byteType,
    .ItemPrintSlot = MENU_ItemPrintSlot_byteType,
    .ItemDirectKeyOp = MENU_ItemDirectKeyOp_byteType,
    .ItemPrintDisp = MENU_ItemPrintDisp_byteType,
    .ItemKeyOp = MENU_ItemKeyOp_byteType,
};