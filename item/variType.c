#include "variType.h"

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

/**
 * @brief : 整数类型菜单项的操作句柄及操作函数。
 */
const menu_itemAdapter_t menu_itemAdapter_variType =
{
    .ItemConstruct = MENU_ItemConstruct_variType,
    .ItemGetData = MENU_ItemGetData_variType,
    .ItemSetData = MENU_ItemSetData_variType,
    .ItemPrintSlot = MENU_ItemPrintSlot_variType,
    .ItemDirectKeyOp = MENU_ItemDirectKeyOp_variType,
    .ItemPrintDisp = MENU_ItemPrintDisp_variType,
    .ItemKeyOp = MENU_ItemKeyOp_variType,
};

const int32_t variAdjustLut[6] =
{ 1, 10, 100, 1000, 10000, 100000 };

extern menu_t menu;

void MENU_ItemGetContent_variType(int32_t *const v, int32_t *const e, int32_t data)
{
    *v = data;
    *e = 0;
    while (-10 < *e && *e < 10)
    {
        if (*v > 9999)
        {
            *v *= 0.1;
            ++(*e);
            continue;
        }
        if (*v < -9999)
        {
            *v *= 0.1;
            ++(*e);
            continue;
        }
        break;
    }
}
void MENU_ItemSetContent_variType(int32_t *const data, int32_t v, int32_t e)
{
    //int32_t v = v, e = e;
    while (1)
    {
        if (e > 0)
        {
            v *= 10;
            e--;
            continue;
        }
        if (e < 0)
        {
            v *= 0.1;
            e++;
            continue;
        }
        break;
    }
    *data = v;
}

void MENU_ItemConstruct_variType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_variType;
    _item->p_handle = malloc(sizeof(menu_item_variHandle_t));
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    p_variType->data = (int32_t*)_data;
    p_variType->cur = 0;
    MENU_ItemGetContent_variType(&p_variType->v, &p_variType->e, *p_variType->data);
}
void MENU_ItemGetData_variType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    
    _data->size = sizeof(int32_t);
    _data->pData = p_variType->data;
    SYSLOG_V("Get variType Data: %12.12d", *(p_variType->data));
}
void MENU_ItemSetData_variType(menu_itemIfce_t *_item, menu_itemData_t *_data)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    int32_t *pData = p_variType->data;
    
    if(4U == _data->size)
    {
        *pData = *(int32_t *)(_data->pData);
        if (_item->pptFlag & menuItem_dataExt_HasMinMax)
        {
            *pData = (*pData > *(pData + 1)) ? (*pData) : *(pData + 1);
            *pData = (*pData < *(pData + 2)) ? (*pData) : *(pData + 2);
        }
        SYSLOG_V("Set variType data: %12.12d", *pData);
    }
    else
    {
        SYSLOG_W("Set variType data: Failed. -size-match");
    }
}
//used when in menuList
void MENU_ItemPrintSlot_variType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    MENU_ItemGetContent_variType(&p_variType->v, &p_variType->e, *p_variType->data);
    if (_item->pptFlag & menuItem_disp_noPreview)
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-16.16s  ->", _item->nameStr)] = ' ';
    }
    else if ((!(_item->pptFlag & menuItem_disp_forceSci)) && *p_variType->data < 1000000 && *p_variType->data > -1000000)
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+-7ld", _item->nameStr, *(p_variType->data))] = ' ';
    }
    else
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+4.4ld%+1.1ld", _item->nameStr, p_variType->v, p_variType->e)] = ' ';
    }
}
void MENU_ItemDirectKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        {
            MENU_ItemGetContent_variType(&p_variType->v, &p_variType->e, *p_variType->data);
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
                if (*p_variType->data < 10 && *p_variType->data > -10)
                {
                    --*p_variType->data;
                }
                else
                {
                    MENU_ItemGetContent_variType(&p_variType->v, &p_variType->e, *p_variType->data);
                    MENU_ItemSetContent_variType(p_variType->data, p_variType->v - 100, p_variType->e);
                }
                if (_item->pptFlag & menuItem_dataExt_HasMinMax)
                {
                    *p_variType->data = (*(p_variType->data) > *(p_variType->data + 1)) ? (*p_variType->data) : *(p_variType->data + 1);
                }
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
                if (*p_variType->data < 100 && *p_variType->data > -100)
                {
                    ++*p_variType->data;
                }
                else
                {
                    MENU_ItemGetContent_variType(&p_variType->v, &p_variType->e, *p_variType->data);
                    MENU_ItemSetContent_variType(p_variType->data, p_variType->v + 100, p_variType->e);
                }
                if (_item->pptFlag & menuItem_dataExt_HasMinMax)
                {
                    *p_variType->data = (*(p_variType->data) < * (p_variType->data + 2)) ? (*p_variType->data) : *(p_variType->data + 2);
                }
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
void MENU_ItemPrintDisp_variType(menu_itemIfce_t *_item)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);

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

    menu.dispStrBuf.strbuf[2][snprintf(menu.dispStrBuf.strbuf[2], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Cur: %+-10ld", *p_variType->data)] = ' ';
    if (_item->pptFlag & menuItem_data_ROFlag)
    {
        menu.dispStrBuf.strbuf[7][snprintf(menu.dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "     READ   ONLY     ")] = ' ';
    }
    else
    {
        menu.dispStrBuf.strbuf[3][snprintf(menu.dispStrBuf.strbuf[3], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Adj: %+-10ld", p_variType->bData)] = ' ';
        menu.dispStrBuf.strbuf[4][snprintf(menu.dispStrBuf.strbuf[4], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Mod: %+4.4lde%+1.1ld", p_variType->v, p_variType->e)] = ' ';
        int32_t pos = p_variType->cur < 0 ? 12 - p_variType->cur : 11 - p_variType->cur;
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
        menu.dispStrBuf.fcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F;
	    menu.dispStrBuf.bcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B;
#endif // ! TEXTMENU_USE_PALETTE
        menu.dispStrBuf.strbuf[5][pos] = '^';
        menu.dispStrBuf.strbuf[7][snprintf(menu.dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "    SOK>AC LOK>WA    ")] = ' ';
    }
}
void MENU_ItemKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        if (!(_item->pptFlag & menuItem_data_ROFlag))
        {
            (*p_variType->data) = (p_variType->bData);
            if (_item->pptFlag & menuItem_dataExt_HasMinMax)
            {
                *p_variType->data = (*(p_variType->data) > *(p_variType->data + 1)) ? (*p_variType->data) : *(p_variType->data + 1);
                *p_variType->data = (*(p_variType->data) < *(p_variType->data + 2)) ? (*p_variType->data) : *(p_variType->data + 2);
            }
        }
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, long):
        menu.status.currItem = NULL;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, long):
        p_variType->cur = 4;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, long):
        p_variType->cur = -2;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, shrt):
        if (p_variType->cur < 4)
        {
            ++p_variType->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, shrt):
        if (p_variType->cur > -2)
        {
            --p_variType->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_up, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, lrpt):
        if (p_variType->cur == -2)
        {
            p_variType->e = (p_variType->e < 5) ? (p_variType->e + 1) : (5);
        }
        else if (p_variType->cur == -1)
        {
            p_variType->e = -p_variType->e;
        }
        else if (p_variType->cur == 4)
        {
            p_variType->v = -p_variType->v;
        }
        else
        {
            p_variType->v += variAdjustLut[p_variType->cur];
        }
        MENU_ItemSetContent_variType(&p_variType->bData, p_variType->v, p_variType->e);
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, lrpt):
        if (p_variType->cur == -2)
        {
            p_variType->e = (p_variType->e > -5) ? (p_variType->e - 1) : (-5);
        }
        else if (p_variType->cur == -1)
        {
            p_variType->e = -p_variType->e;
        }
        else if (p_variType->cur == 4)
        {
            p_variType->v = -p_variType->v;
        }
        else
        {
            p_variType->v -= variAdjustLut[p_variType->cur];
        }
        MENU_ItemSetContent_variType(&p_variType->bData, p_variType->v, p_variType->e);
        *_op = 0;
        break;
        default:
        //appui_menu_t::getInstance().currItem = NULL;
        break;
    }
}
