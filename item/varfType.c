#include "varfType.h"

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

/**
 * @brief : 浮点类型菜单项的操作句柄及操作函数。
 */
const menu_itemAdapter_t menu_itemAdapter_varfType =
{
    .ItemConstruct = MENU_ItemConstruct_varfType,
    .ItemGetData = MENU_ItemGetData_varfType,
    .ItemSetData = MENU_ItemSetData_varfType,
    .ItemPrintSlot = MENU_ItemPrintSlot_varfType,
    .ItemDirectKeyOp = MENU_ItemDirectKeyOp_varfType,
    .ItemPrintDisp = MENU_ItemPrintDisp_varfType,
    .ItemKeyOp = MENU_ItemKeyOp_varfType,
};

const int32_t varfAdjustLut[6] =
{ 1, 10, 100, 1000, 10000, 100000 };

extern menu_t menu;

void MENU_ItemGetContent_varfType(int32_t *const v, int32_t *const e, float data)
{
    *e = 0;
    while (-10 < *e && *e < 10)
    {
        if (data > 0.0f && data < 999.0f)
        {
            data *= 10.0;
            --(*e);
            continue;
        }
        if (data > 9999.0f)
        {
            data /= 10.0f;
            ++(*e);
            continue;
        }
        if (data < 0.0f && data > -999.0f)
        {
            data *= 10.0f;
            --(*e);
            continue;
        }
        if (data < -9999.0)
        {
            data /= 10.0f;
            ++(*e);
            continue;
        }
        break;
    }
    *v = (int32_t)data;
}
void MENU_ItemSetContent_varfType(float *const data, int32_t v, int32_t e)
{
    float buf = (float)v;
    while (1)
    {
        if (e > 0)
        {
            buf *= 10.0f;
            e--;
            continue;
        }
        if (e < 0)
        {
            buf /= 10.0f;
            e++;
            continue;
        }
        break;
    }
    *data = buf;
}

void MENU_ItemConstruct_varfType(menu_itemIfce_t *_item, void *_data)
{
    _item->adapter = &menu_itemAdapter_varfType;
    _item->p_handle = malloc(sizeof(menu_item_varfHandle_t));
    menu_item_varfHandle_t *p_varfType = (menu_item_varfHandle_t*)(_item->p_handle);
    p_varfType->data = (float*)_data;
    p_varfType->cur = 0;
    MENU_ItemGetContent_varfType(&p_varfType->v, &p_varfType->e, *p_varfType->data);
}
void MENU_ItemGetData_varfType(menu_itemIfce_t *_item, void *_data)
{
    menu_item_varfHandle_t *p_varfType = (menu_item_varfHandle_t*)(_item->p_handle);
    *(float *)_data = *(p_varfType->data);
}
void MENU_ItemSetData_varfType(menu_itemIfce_t *_item, void *_data)
{
    menu_item_varfHandle_t *p_varfType = (menu_item_varfHandle_t*)(_item->p_handle);
    if (_item->pptFlag & menuItem_dataExt_HasMinMax)
    {
        *(float *)_data = (*(float *)_data > *(p_varfType->data + 1)) ? (*(float *)_data) : *(p_varfType->data + 1);
        *(float *)_data = (*(float *)_data < *(p_varfType->data + 2)) ? (*(float *)_data) : *(p_varfType->data + 2);
    }
    *(p_varfType->data) = *(float *)_data;
    SYSLOG_V("varfType Data Updated %12.4f", *(p_varfType->data));
}
//used when in menuList
void MENU_ItemPrintSlot_varfType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu_item_varfHandle_t *p_varfType = (menu_item_varfHandle_t*)(_item->p_handle);
    MENU_ItemGetContent_varfType(&p_varfType->v, &p_varfType->e, *p_varfType->data);
    if (_item->pptFlag & menuItem_disp_noPreview)
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-16.16s  ->", _item->nameStr)] = ' ';
    }
    else if ((!(_item->pptFlag & menuItem_disp_forceSci)) && ((*p_varfType->data < 10000 && *p_varfType->data > 0.001f) || (*p_varfType->data > -10000 && *p_varfType->data < -0.001f) || (*p_varfType->data == 0.0f)))
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+-7f", _item->nameStr, *(p_varfType->data))] = ' ';
    }
    else
    {
        menu.dispStrBuf.strbuf[_slotNum][snprintf(menu.dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+4.4ld%+1.1ld", _item->nameStr, p_varfType->v, p_varfType->e)] = ' ';
    }
}
void MENU_ItemDirectKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_varfHandle_t *p_varfType = (menu_item_varfHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        {
            MENU_ItemGetContent_varfType(&p_varfType->v, &p_varfType->e, *p_varfType->data);
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
                MENU_ItemGetContent_varfType(&p_varfType->v, &p_varfType->e, *p_varfType->data);
                MENU_ItemSetContent_varfType(p_varfType->data, p_varfType->v - 100, p_varfType->e);
                if (_item->pptFlag & menuItem_dataExt_HasMinMax)
                {
                    *p_varfType->data = (*(p_varfType->data) > *(p_varfType->data + 1)) ? (*p_varfType->data) : *(p_varfType->data + 1);
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
                MENU_ItemGetContent_varfType(&p_varfType->v, &p_varfType->e, *p_varfType->data);
                MENU_ItemSetContent_varfType(p_varfType->data, p_varfType->v + 100, p_varfType->e);
                if (_item->pptFlag & menuItem_dataExt_HasMinMax)
                {
                    *p_varfType->data = (*(p_varfType->data) < *(p_varfType->data + 2)) ? (*p_varfType->data) : *(p_varfType->data + 2);
                }
            }
            *_op = 0;
            break;
        }
        default:
        //appui_menu_t::getInstance().currItem = varf;
        break;
    }
}
//used when in menuItem
void MENU_ItemPrintDisp_varfType(menu_itemIfce_t *_item)
{
    menu_item_varfHandle_t *p_varfType = (menu_item_varfHandle_t*)(_item->p_handle);

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

    menu.dispStrBuf.strbuf[2][snprintf(menu.dispStrBuf.strbuf[2], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Cur: %+-10.4f", *p_varfType->data)] = ' ';
    if (_item->pptFlag & menuItem_data_ROFlag)
    {
        menu.dispStrBuf.strbuf[7][snprintf(menu.dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "     READ   ONLY     ")] = ' ';
    }
    else
    {
        menu.dispStrBuf.strbuf[3][snprintf(menu.dispStrBuf.strbuf[3], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Adj: %+-10.4f", p_varfType->bData)] = ' ';
        menu.dispStrBuf.strbuf[4][snprintf(menu.dispStrBuf.strbuf[4], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Mod: %+3.3lde%+1.1ld", p_varfType->v, p_varfType->e)] = ' ';
        int32_t pos = p_varfType->cur < 0 ? 12 - p_varfType->cur : 11 - p_varfType->cur;
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
        menu.dispStrBuf.fcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F;
	    menu.dispStrBuf.bcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B;
#endif // ! TEXTMENU_USE_PALETTE
        menu.dispStrBuf.strbuf[5][pos] = '^';
        menu.dispStrBuf.strbuf[7][snprintf(menu.dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "    SOK>AC LOK>WA    ")] = ' ';
    }
}
void MENU_ItemKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_varfHandle_t *p_varfType = (menu_item_varfHandle_t*)(_item->p_handle);
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        if (!(_item->pptFlag & menuItem_data_ROFlag))
        {
            (*p_varfType->data) = (p_varfType->bData);
            if (_item->pptFlag & menuItem_dataExt_HasMinMax)
            {
                *p_varfType->data = (*(p_varfType->data) > *(p_varfType->data + 1)) ? (*p_varfType->data) : *(p_varfType->data + 1);
                *p_varfType->data = (*(p_varfType->data) < *(p_varfType->data + 2)) ? (*p_varfType->data) : *(p_varfType->data + 2);
            }
        }
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, long):
        menu.status.currItem = NULL;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, long):
        p_varfType->cur = 4;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, long):
        p_varfType->cur = -2;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, shrt):
        if (p_varfType->cur < 4)
        {
            ++p_varfType->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, shrt):
        if (p_varfType->cur > -2)
        {
            --p_varfType->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_up, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, lrpt):
        if (p_varfType->cur == -2)
        {
            p_varfType->e = (p_varfType->e < 9) ? (p_varfType->e + 1) : (9);
        }
        else if (p_varfType->cur == -1)
        {
            p_varfType->e = -p_varfType->e;
        }
        else if (p_varfType->cur == 4)
        {
            p_varfType->v = -p_varfType->v;
        }
        else
        {
            p_varfType->v += varfAdjustLut[p_varfType->cur];
        }
        MENU_ItemSetContent_varfType(&p_varfType->bData, p_varfType->v, p_varfType->e);
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, lrpt):
        if (p_varfType->cur == -2)
        {
            p_varfType->e = (p_varfType->e > -9) ? (p_varfType->e - 1) : (-9);
        }
        else if (p_varfType->cur == -1)
        {
            p_varfType->e = -p_varfType->e;
        }
        else if (p_varfType->cur == 4)
        {
            p_varfType->v = -p_varfType->v;
        }
        else
        {
            p_varfType->v -= varfAdjustLut[p_varfType->cur];
        }
        MENU_ItemSetContent_varfType(&p_varfType->bData, p_varfType->v, p_varfType->e);
        *_op = 0;
        break;
        default:
        //appui_menu_t::getInstance().currItem = varf;
        break;
    }

    while (p_varfType->v > 9999 || p_varfType->v < -9999)
    {
        p_varfType->v /= 10;
        ++p_varfType->e;
    }
    while ((p_varfType->v > 0 && p_varfType->v < 1000) || (p_varfType->v < 0 && p_varfType->v > -1000))
    {
        p_varfType->v *= 10;
        --p_varfType->e;
    }
}
