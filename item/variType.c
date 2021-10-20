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
    _item->handle.p_variType = (menu_item_variHandle_t*)malloc(sizeof(menu_item_variHandle_t));
    _item->handle.p_variType->data = (int32_t*)_data;
    _item->handle.p_variType->cur = 0;
    MENU_ItemGetContent_variType(&_item->handle.p_variType->v, &_item->handle.p_variType->e, *_item->handle.p_variType->data);
}
void MENU_ItemGetData_variType(menu_itemIfce_t *_item, void *_data)
{
    *(int32_t *)_data = *(_item->handle.p_variType->data);
}
void MENU_ItemSetData_variType(menu_itemIfce_t *_item, void *_data)
{
    menu_item_variHandle_t *handle = _item->handle.p_variType;
    if (_item->pptFlag & menuItem_dataExt_HasMinMax)
    {
        *(int32_t *)_data = (*(int32_t *)_data > *(handle->data + 1)) ? (*(int32_t *)_data) : *(handle->data + 1);
        *(int32_t *)_data = (*(int32_t *)_data < *(handle->data + 2)) ? (*(int32_t *)_data) : *(handle->data + 2);
    }
    *(handle->data) = *(int32_t *)_data;
    SYSLOG_V("variType Data Updated %12.12d", *(_item->handle.p_variType->data));
}
//used when in menuList
void MENU_ItemPrintSlot_variType(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    menu_item_variHandle_t *handle = _item->handle.p_variType;
    MENU_ItemGetContent_variType(&handle->v, &handle->e, *handle->data);
    if (_item->pptFlag & menuItem_disp_noPreview)
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-16.16s  ->", _item->nameStr)] = ' ';
    }
    else if ((!(_item->pptFlag & menuItem_disp_forceSci)) && *handle->data < 1000000 && *handle->data > -1000000)
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+-7ld", _item->nameStr, *(handle->data))] = ' ';
    }
    else
    {
        menu_dispStrBuf.strbuf[_slotNum][snprintf(menu_dispStrBuf.strbuf[_slotNum], TEXTMENU_DISPLAY_STRBUF_COL + 1, " %-12.12s %+4.4ld%+1.1ld", _item->nameStr, handle->v, handle->e)] = ' ';
    }
}
void MENU_ItemDirectKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    menu_item_variHandle_t *handle = _item->handle.p_variType;
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        {
            MENU_ItemGetContent_variType(&handle->v, &handle->e, *handle->data);
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
                if (*handle->data < 10 && *handle->data > -10)
                {
                    --*handle->data;
                }
                else
                {
                    MENU_ItemGetContent_variType(&handle->v, &handle->e, *handle->data);
                    MENU_ItemSetContent_variType(handle->data, handle->v - 100, handle->e);
                }
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
                if (*handle->data < 100 && *handle->data > -100)
                {
                    ++*handle->data;
                }
                else
                {
                    MENU_ItemGetContent_variType(&handle->v, &handle->e, *handle->data);
                    MENU_ItemSetContent_variType(handle->data, handle->v + 100, handle->e);
                }
                if (_item->pptFlag & menuItem_dataExt_HasMinMax)
                {
                    *handle->data = (*(handle->data) < * (handle->data + 2)) ? (*handle->data) : *(handle->data + 2);
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
    menu_item_variHandle_t *handle = _item->handle.p_variType;
    MENU_ItemSetContent_variType(&handle->bData, handle->v, handle->e);

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

    menu_dispStrBuf.strbuf[2][snprintf(menu_dispStrBuf.strbuf[2], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Cur: %+-10ld", *handle->data)] = ' ';
    if (_item->pptFlag & menuItem_data_ROFlag)
    {
        menu_dispStrBuf.strbuf[7][snprintf(menu_dispStrBuf.strbuf[7], TEXTMENU_DISPLAY_STRBUF_COL + 1, "     READ   ONLY     ")] = ' ';
    }
    else
    {
        menu_dispStrBuf.strbuf[3][snprintf(menu_dispStrBuf.strbuf[3], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Adj: %+-10ld", handle->bData)] = ' ';
        menu_dispStrBuf.strbuf[4][snprintf(menu_dispStrBuf.strbuf[4], TEXTMENU_DISPLAY_STRBUF_COL + 1, "  Mod: %+4.4lde%+1.1ld", handle->v, handle->e)] = ' ';
        int32_t pos = handle->cur < 0 ? 12 - handle->cur : 11 - handle->cur;
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
    menu_item_variHandle_t *handle = _item->handle.p_variType;
    switch (*_op)
    {
        case MENU_BUTTON_MAKE_OP(5wayStick_ok, shrt):
        if (!(_item->pptFlag & menuItem_data_ROFlag))
        {
            MENU_ItemSetContent_variType(handle->data, handle->v, handle->e);
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
            handle->e = (handle->e < 5) ? (handle->e + 1) : (5);
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
            handle->e = (handle->e > -5) ? (handle->e - 1) : (-5);
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
}