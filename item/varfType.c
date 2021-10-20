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
    _item->handle.p_varfType = (menu_item_varfHandle_t*)malloc(sizeof(menu_item_varfHandle_t));
    _item->handle.p_varfType->data = (float*)_data;
    _item->handle.p_varfType->cur = 0;
    MENU_ItemGetContent_varfType(&_item->handle.p_varfType->v, &_item->handle.p_varfType->e, *_item->handle.p_varfType->data);
}
void MENU_ItemGetData_varfType(menu_itemIfce_t *_item, void *_data)
{
    *(float *)_data = *(_item->handle.p_varfType->data);
}
void MENU_ItemSetData_varfType(menu_itemIfce_t *_item, void *_data)
{
    menu_item_varfHandle_t *handle = _item->handle.p_varfType;
    if (_item->pptFlag & menuItem_dataExt_HasMinMax)
    {
        *(float *)_data = (*(float *)_data > *(handle->data + 1)) ? (*(float *)_data) : *(handle->data + 1);
        *(float *)_data = (*(float *)_data < *(handle->data + 2)) ? (*(float *)_data) : *(handle->data + 2);
    }
    *(handle->data) = *(float *)_data;
    SYSLOG_V("varfType Data Updated %12.4f", *(_item->handle.p_varfType->data));
}
//used when in menuList
void MENU_ItemPrintSlot_varfType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu_item_varfHandle_t *handle = _item->handle.p_varfType;
    MENU_ItemGetContent_varfType(&handle->v, &handle->e, *handle->data);
    if (_item->pptFlag & menuItem_disp_noPreview)
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-16.16s  ->", _item->nameStr)] = ' ';
    }
    else if ((!(_item->pptFlag & menuItem_disp_forceSci)) && ((*handle->data < 10000 && *handle->data > 0.001f) || (*handle->data > -10000 && *handle->data < -0.001f) || (*handle->data == 0.0f)))
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+-7f", _item->nameStr, *(handle->data))] = ' ';
    }
    else
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+4.4ld%+1.1ld", _item->nameStr, handle->v, handle->e)] = ' ';
    }
}
void MENU_ItemDirectKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_varfHandle_t *handle = _item->handle.p_varfType;
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        {
            MENU_ItemGetContent_varfType(&handle->v, &handle->e, *handle->data);
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
                MENU_ItemGetContent_varfType(&handle->v, &handle->e, *handle->data);
                MENU_ItemSetContent_varfType(handle->data, handle->v - 100, handle->e);
                if (_item->pptFlag & menuItem_dataExt_HasMinMax)
                {
                    *handle->data = (*(handle->data) > *(handle->data + 1)) ? (*handle->data) : *(handle->data + 1);
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
                MENU_ItemGetContent_varfType(&handle->v, &handle->e, *handle->data);
                MENU_ItemSetContent_varfType(handle->data, handle->v + 100, handle->e);
                if (_item->pptFlag & menuItem_dataExt_HasMinMax)
                {
                    *handle->data = (*(handle->data) < *(handle->data + 2)) ? (*handle->data) : *(handle->data + 2);
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
void MENU_ItemPrintDisp_varfType(menu_itemIfce_t *_item)
{
    menu_item_varfHandle_t *handle = _item->handle.p_varfType;
    MENU_ItemSetContent_varfType(&handle->bData, handle->v, handle->e);

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

    menu_dispStrBuf.strbuf[2][snprintf(menu_dispStrBuf.strbuf[2], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Cur: %+-10.4f", *handle->data)] = ' ';
    if (_item->pptFlag & menuItem_data_ROFlag)
    {
        menu_dispStrBuf.strbuf[7][snprintf(menu_dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "     READ   ONLY     ")] = ' ';
    }
    else
    {
        menu_dispStrBuf.strbuf[3][snprintf(menu_dispStrBuf.strbuf[3], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Adj: %+-10.4f", handle->bData)] = ' ';
        menu_dispStrBuf.strbuf[4][snprintf(menu_dispStrBuf.strbuf[4], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Mod: %+3.3lde%+1.1ld", handle->v, handle->e)] = ' ';
        int32_t pos = handle->cur < 0 ? 12 - handle->cur : 11 - handle->cur;
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
        menu_dispStrBuf.fcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F;
	    menu_dispStrBuf.bcolor[4][pos] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B;
#endif // ! TEXTMENU_USE_PALETTE
        menu_dispStrBuf.strbuf[5][pos] = '^';
        menu_dispStrBuf.strbuf[7][snprintf(menu_dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "    SOK>AC LOK>WA    ")] = ' ';
    }
}
void MENU_ItemKeyOp_varfType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_varfHandle_t *handle = _item->handle.p_varfType;
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        if (!(_item->pptFlag & menuItem_data_ROFlag))
        {
            MENU_ItemSetContent_varfType(handle->data, handle->v, handle->e);
            if (_item->pptFlag & menuItem_dataExt_HasMinMax)
            {
                *handle->data = (*(handle->data) > *(handle->data + 1)) ? (*handle->data) : *(handle->data + 1);
                *handle->data = (*(handle->data) < *(handle->data + 2)) ? (*handle->data) : *(handle->data + 2);
            }
        }
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, long):
        menu_currItem = NULL;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, long):
        handle->cur = 4;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, long):
        handle->cur = -2;
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_lf, shrt):
        if (handle->cur < 4)
        {
            ++handle->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_rt, shrt):
        if (handle->cur > -2)
        {
            --handle->cur;
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_up, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_up, lrpt):
        if (handle->cur == -2)
        {
            handle->e = (handle->e < 9) ? (handle->e + 1) : (9);
        }
        else if (handle->cur == -1)
        {
            handle->e = -handle->e;
        }
        else if (handle->cur == 4)
        {
            handle->v = -handle->v;
        }
        else
        {
            handle->v += menu_itemAdjustLut[handle->cur];
        }
        *_op = 0;
        break;
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, shrt):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, long):
        case MENU_BUTTON_MAKE_OP(5wayStick_dn, lrpt):
        if (handle->cur == -2)
        {
            handle->e = (handle->e > -9) ? (handle->e - 1) : (-9);
        }
        else if (handle->cur == -1)
        {
            handle->e = -handle->e;
        }
        else if (handle->cur == 4)
        {
            handle->v = -handle->v;
        }
        else
        {
            handle->v -= menu_itemAdjustLut[handle->cur];
        }
        *_op = 0;
        break;
        default:
        //appui_menu_t::getInstance().currItem = NULL;
        break;
    }

    while (handle->v > 9999 || handle->v < -9999)
    {
        handle->v /= 10;
        ++handle->e;
    }
    while ((handle->v > 0 && handle->v < 1000) || (handle->v < 0 && handle->v > -1000))
    {
        handle->v *= 10;
        --handle->e;
    }
}