#include "textmenu_list.h"


/**
 * ********** 菜单列表操作接口 **********
 */

#undef SYSLOG_TAG
#undef SYSLOG_LVL
#define SYSLOG_TAG  ("MENU.LIST")
#define SYSLOG_LVL  (TEXTMENU_LIST_LOG_LVL)
#include <inc_syslog.h>

extern menu_t menu;

menu_list_t *MENU_ListConstruct(const char *_nameStr, uint32_t _size, menu_list_t *_prev)
{
    assert(_prev);
    menu_list_t *list;
    list = (menu_list_t*)calloc(1, sizeof(menu_list_t));
    assert(list);
    list->listSize = _size;
    list->listNum = 0;
    list->menu = (menu_itemIfce_t**)calloc(_size, sizeof(menu_itemIfce_t *));
    assert(list->menu);
    strncpy(list->nameStr, _nameStr, MENU_NAME_STR_SIZE);
    MENU_ListInsert(list, MENU_ItemConstruct(&menu_itemAdapter_menuType, (void *)_prev, "Back", 0, 0));
    ((menu_item_menuHandle_t*)(list->menu[0]->p_handle))->data = _prev;
    ++menu.status.listCnt;
    return list;
}

void MENU_ListDestruct(menu_list_t *_list)
{
    free(_list->menu);
    free(_list);
    --menu.status.listCnt;
}

status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item)
{
    assert(_list);
    assert(_item);

    if (_list->listNum == _list->listSize)
    {
        return kStatus_Fail;
    }
    _list->menu[_list->listNum] = _item;
    _item->list_id = _list->listNum++;

    return kStatus_Success;
}

void MENU_ListPrintDisp(menu_list_t *_list)
{
    menu.dispStrBuf.strbuf[0][snprintf(menu.dispStrBuf.strbuf[0], TEXTMENU_DISPLAY_STRBUF_COL + 1, "##%-13.13s*%2.2ld/%2.2ld", _list->nameStr, _list->slct_p + 1, _list->listNum)] = ' ';
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
    for(int c = 0; c < TEXTMENU_DISPLAY_STRBUF_COL + 1; ++c)
    {
        menu.dispStrBuf.fcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_F;
	    menu.dispStrBuf.bcolor[0][c] = TEXTMENU_DISPLAY_PAL_IDX_TITBAR_B;
    }
#endif // ! TEXTMENU_USE_PALETTE
    uint32_t printCnt = _list->listNum < TEXTMENU_DISPLAY_STRBUF_ROW - 1 ? _list->listNum : TEXTMENU_DISPLAY_STRBUF_ROW - 1;
    for (uint8_t i = 0; i < printCnt; ++i)
    {
        MENU_ItemPrintSlot(_list->menu[_list->disp_p + i], i + 1);
    }

    uint8_t selectRow = _list->slct_p - _list->disp_p + 1;
    menu.dispStrBuf.strbuf[selectRow][0] = '>';
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
    for(int c = 0; c < TEXTMENU_DISPLAY_STRBUF_COL + 1; ++c)
    {
        menu.dispStrBuf.fcolor[selectRow][c] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F;
	    menu.dispStrBuf.bcolor[selectRow][c] = TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B;
    }
#endif // ! TEXTMENU_USE_PALETTE
}

void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t *const _op)
{
    switch (*_op)
    {
    case MENU_BUTTON_MAKE_OP(5wayStick_ok, long):
        //return

        if (menu.status.currList != menu.menuRoot)
        {
            menu.status.currList = ((menu_item_menuHandle_t*)(_list->menu[0]->p_handle))->data;
        }
    case MENU_BUTTON_MAKE_OP(5wayStick_ok, lrpt):
        *_op = 0;
        break;
    case MENU_BUTTON_MAKE_OP(5wayStick_up, shrt):
    case MENU_BUTTON_MAKE_OP(5wayStick_up, long):
    case MENU_BUTTON_MAKE_OP(5wayStick_up, lrpt):
        //menu up
        if (_list->slct_p > 0)
        {
            --_list->slct_p;
        }
        while (_list->disp_p > _list->slct_p)
        {
            --_list->disp_p;
        }
        *_op = 0;
        break;
    case MENU_BUTTON_MAKE_OP(5wayStick_dn, shrt):
    case MENU_BUTTON_MAKE_OP(5wayStick_dn, long):
    case MENU_BUTTON_MAKE_OP(5wayStick_dn, lrpt):
        //menu dn
        if (_list->slct_p < _list->listNum - 1)
        {
            ++_list->slct_p;
        }
        while (_list->slct_p > (TEXTMENU_DISPLAY_STRBUF_ROW - 1) && _list->disp_p <= _list->slct_p - (TEXTMENU_DISPLAY_STRBUF_ROW - 1))
        {
            ++_list->disp_p;
        }
        *_op = 0;
        break;
    default:
        //direct op
        MENU_ItemDirectKeyOp(_list->menu[_list->slct_p], _op);
        break;
    }
}


menu_list_t *MENU_DirGetList(const char *str)
{
    assert(str);
    /** 识别路径首字符 */
    if(str[0] != '/')
    {
        return NULL;
    }
    /** 字符串长度保护 */
    uint32_t str_length = strlen(str);
    if(str_length > 256)
    {
        return NULL;
    }
    /** 环境准备 */
    char *str_copy = (char*)malloc(str_length + 1U);
    strncpy(str_copy, str, str_length + 1U);
    char *pch = strtok(str_copy, "/");
    menu_list_t *currDirList = menu.menuRoot;
    /** 启动识别 */
    while(pch != NULL)
    {
        bool isFound = false;
        for(uint32_t i = 0; i < currDirList->listNum; ++i)
        {
            menu_itemIfce_t *it = currDirList->menu[i];
            if(it->adapter == &menu_itemAdapter_menuType && 0 == strcmp(it->nameStr, pch))
            {
                isFound = true;
                currDirList = ((menu_item_menuHandle_t*)it->p_handle)->data;
                break;
            }
        }
        if(false == isFound)
        {
            free(str_copy);
            return NULL;
        }
        pch = strtok(NULL, "/");
    }
    free(str_copy);
    return currDirList;
}

menu_itemIfce_t *MENU_DirGetItem(const menu_list_t *dir, const char *str)
{
    assert(dir);
    assert(str);
    for(uint32_t i = 0; i < dir->listNum; ++i)
    {
        menu_itemIfce_t *it = dir->menu[i];
        if(0 == strcmp(it->nameStr, str))
        {
            return it;
        }
    }
    return NULL;
}
