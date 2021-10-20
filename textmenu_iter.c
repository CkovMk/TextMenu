#include "textmenu_iter.h"

/**************************************
 ************ 菜单项迭代器 **********
 **************************************/

#undef SYSLOG_TAG
#undef SYSLOG_LVL
#define SYSLOG_TAG  ("MENU.ITER")
#define SYSLOG_LVL  (TEXTMENU_ITER_LOG_LVL)
#include <inc_syslog.h>

void MENU_IteratorSetup(menu_iterator_t *_iter)
{
    SYSLOG_I("Iterator Reset.");

    if(_iter->listQue != nullptr)
    {
        free(_iter->listQue);
        _iter->listQue = nullptr;
    }
    _iter->listQue = (menu_list_t**)calloc(menu_listCnt, sizeof(menu_list_t *));
    memset(_iter->listQue, 0U, menu_listCnt * sizeof(menu_list_t *));
    assert(_iter->listQue);

    _iter->listQueSize = menu_listCnt;

    extern menu_list_t *menu_menuRoot;
    extern menu_list_t *menu_manageList;

    _iter->listQue[0] = menu_manageList;
    _iter->listQue[1] = menu_menuRoot;

    _iter->listNum = 0U;
    _iter->itemNum = 0U;
}

menu_iterator_t* MENU_IteratorConstruct(void)
{
    SYSLOG_I("Iterator Construction");
    menu_iterator_t *ret = (menu_iterator_t*)malloc(sizeof(menu_iterator_t));
    assert(ret);
    memset(ret, 0U, sizeof(menu_iterator_t));
    MENU_IteratorSetup(ret);
    return ret;
}

void MENU_IteratorDestruct(menu_iterator_t *_iter)
{
    free(_iter->listQue);
    free(_iter);
    _iter = nullptr;
    SYSLOG_I("Iterator Destruction");
}

menu_list_t* MENU_IteratorDerefList(menu_iterator_t *_iter)
{
    return _iter->listQue[_iter->listNum];
}

menu_itemIfce_t* MENU_IteratorDerefItem(menu_iterator_t *_iter)
{
    return MENU_IteratorDerefList(_iter)->menu[_iter->itemNum];
}


status_t MENU_IteratorIncrease(menu_iterator_t *_iter)
{
    SYSLOG_I("Iterator Increase:");
    SYSLOG_V("Current Position: List.Item : %4.4d.%4.4d", _iter->listNum, _iter->itemNum);
    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(_iter);
    if (thisItem->type == menuType)
    {
        SYSLOG_V("\"menuType\" item, Scan previous records");
        for (uint32_t j = 0U; j < _iter->listQueSize; ++j)
        {
            if (_iter->listQue[j] == thisItem->handle.p_menuType->data)
            {
                SYSLOG_V("Previous records found, Aborting");
                break;
            }
            else if (_iter->listQue[j] == NULL)
            {
                _iter->listQue[j] = thisItem->handle.p_menuType->data;
                SYSLOG_V("No previous records found, New record added");
                SYSLOG_D("Add itemList %4.4d : [%-16.16s].", j, _iter->listQue[j]->nameStr);
                break;
            }
         }
     }

    if(++(_iter->itemNum) == _iter->listQue[_iter->listNum]->listNum)
    {
        ++(_iter->listNum);
        _iter->itemNum = 0U;
        if(_iter->listNum == _iter->listQueSize ||_iter->listQue[_iter->listNum] == nullptr)
        {
            SYSLOG_I("Iteration Ended.");
            return kStatus_Fail;
        }
        SYSLOG_D("Enter list [%s].", _iter->listQue[_iter->listNum]->nameStr);
    }

    SYSLOG_I("Iterator Increased");
    SYSLOG_D("Increased Position: List.Item : %4.4d.%4.4d", _iter->listNum, _iter->itemNum);
    return kStatus_Success;
}