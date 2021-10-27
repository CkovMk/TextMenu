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
    
extern const int32_t menu_itemAdjustLut[];

extern menu_itemIfce_t *menu_currItem;
extern menu_list_t *menu_currList;

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
void MENU_ItemGetData_variType(menu_itemIfce_t *_item, void *_data)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    *(int32_t *)_data = *(p_variType->data);
}
void MENU_ItemSetData_variType(menu_itemIfce_t *_item, void *_data)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    if (_item->pptFlag & menuItem_dataExt_HasMinMax)
    {
        *(int32_t *)_data = (*(int32_t *)_data > *(p_variType->data + 1)) ? (*(int32_t *)_data) : *(p_variType->data + 1);
        *(int32_t *)_data = (*(int32_t *)_data < *(p_variType->data + 2)) ? (*(int32_t *)_data) : *(p_variType->data + 2);
    }
    *(p_variType->data) = *(int32_t *)_data;
    SYSLOG_V("variType Data Updated %12.12d", *(p_variType->data));
}
//used when in menuList
void MENU_ItemPrintSlot_variType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu_item_variHandle_t *p_variType = (menu_item_variHandle_t*)(_item->p_handle);
    MENU_ItemGetContent_variType(&p_variType->v, &p_variType->e, *p_variType->data);
    if (_item->pptFlag & menuItem_disp_noPreview)
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-16.16s  ->", _item->nameStr)] = ' ';
    }
    else if ((!(_item->pptFlag & menuItem_disp_forceSci)) && *p_variType->data < 1000000 && *p_variType->data > -1000000)
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+-7ld", _item->nameStr, *(p_variType->data))] = ' ';
    }
    else
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+4.4ld%+1.1ld", _item->nameStr, p_variType->v, p_variType->e)] = ' ';
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
            menu_currItem = _item;
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
    MENU_ItemSetContent_variType(&p_variType->bData, p_variType->v, p_variType->e);

    menu_dispStrBuf.strbuf[0][snprintf(menu_dispStrBuf.strbuf[0], TEXTMENU_DISPLAY_STRBUF_COL + 1, "##%-15.15s *", _item->nameStr)] = ' ';
    if (_item->pptFlag & menuItem_data_global)
    {
        menu_dispStrBuf.strbuf[0][19] = 'G';
        menu_dispStrBuf.strbuf[0][20] = 'L';
    }
    else if (_item->pptFlag & menuItem_data_region)
    {
        menu_dispStrBuf.strbuf[0][19] = 'R';
        menu_dispStrBuf.strbuf[0][20] = 'G';
    }
    else
    {
        menu_dispStrBuf.strbuf[0][19] = 'N';
        menu_dispStrBuf.strbuf[0][20] = 'O';
    }
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
    for(int c = 0; c < TEXTMENU_DISPLAY_STRBUF_COL + 1; ++c)
    {
        menu_dispStrBuf.fcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_F;
	    menu_dispStrBuf.bcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_B;
    }
#endif // ! TEXTMENU_USE_PALETTE

    menu_dispStrBuf.strbuf[2][snprintf(menu_dispStrBuf.strbuf[2], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Cur: %+-10ld", *p_variType->data)] = ' ';
    if (_item->pptFlag & menuItem_data_ROFlag)
    {
        menu_dispStrBuf.strbuf[7][snprintf(menu_dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "     READ   ONLY     ")] = ' ';
    }
    else
    {
        menu_dispStrBuf.strbuf[3][snprintf(menu_dispStrBuf.strbuf[3], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Adj: %+-10ld", p_variType->bData)] = ' ';
        menu_dispStrBuf.strbuf[4][snprintf(menu_dispStrBuf.strbuf[4], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Mod: %+4.4lde%+1.1ld", p_variType->v, p_variType->e)] = ' ';
        int32_t pos = p_variType->cur < 0 ? 12 - p_variType->cur : 11 - p_variType->cur;
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
        menu_dispStrBuf.fcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F;
	    menu_dispStrBuf.bcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B;
#endif // ! TEXTMENU_USE_PALETTE
        menu_dispStrBuf.strbuf[5][pos] = '^';
        menu_dispStrBuf.strbuf[7][snprintf(menu_dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "    SOK>AC LOK>WA    ")] = ' ';
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
            MENU_ItemSetContent_variType(p_variType->data, p_variType->v, p_variType->e);
            if (_item->pptFlag & menuItem_dataExt_HasMinMax)
            {
                *p_variType->data = (*(p_variType->data) > *(p_variType->data + 1)) ? (*p_variType->data) : *(p_variType->data + 1);
                *p_variType->data = (*(p_variType->data) < *(p_variType->data + 2)) ? (*p_variType->data) : *(p_variType->data + 2);
            }
        }
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, long):
        menu_currItem = NULL;
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
            p_variType->v += menu_itemAdjustLut[p_variType->cur];
        }
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
            p_variType->v -= menu_itemAdjustLut[p_variType->cur];
        }
        *_op = 0;
        break;
        default:
        //appui_menu_t::getInstance().currItem = NULL;
        break;
    }
}
