#include "textmenu_item.h"

/*!
 * @addtogroup menu_type
 * @{
 */

#define SYSLOG_TAG  ("MENU.ITEM")
#define SYSLOG_LVL  (TEXTMENU_ITEM_LOG_LVL)
#include <inc_syslog.h>

/**
 * @brief : 菜单项和菜单列表的计数器。
 */
extern menu_t menu;

/**
 * ********** 菜单项操作接口 **********
 */

menu_itemIfce_t *MENU_ItemConstruct(menu_itemAdapter_t const *_type, void *_data, const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag)
{
    menu_itemIfce_t *item;
    item = (menu_itemIfce_t*)calloc(1, sizeof(menu_itemIfce_t));
    assert(item);
    item->pptFlag = _pptFlag;
    item->unique_id = menu.status.itemCnt++;
    item->saveAddr = _saveAddr;
    strncpy(item->nameStr, _nameStr, MENU_NAME_STR_SIZE);
    item->nameStr[MENU_NAME_STR_SIZE - 1] = '\0';

#if defined(TEXTMENU_FEATURE_EVENTCB) && (TEXTMENU_FEATURE_EVENTCB != 0U)
    item->eventEnable = 0U;
    item->eventFlag = 0U;
    item->eventUserData = NULL;
    item->eventHandler = NULL;
#endif // ! TEXTMENU_FEATURE_EVENTCB

    _type->ItemConstruct(item, _data);
    //MENU_ITEM_SWITCH_CASE(MENU_ItemConstruct, item, _data);
    return item;
}

void MENU_ItemDestruct(menu_itemIfce_t *_item)
{
    free(_item->p_handle);
    free(_item);
    _item = NULL;
    --menu.status.itemCnt;
}

void MENU_ItemGetData(menu_itemIfce_t *_item, menu_nvmData_t *_data)
{
    memcpy(_data->nameStr, _item->nameStr, MENU_NAME_STR_SIZE);
    //MENU_ITEM_SWITCH_CASE(MENU_ItemGetData, _item, &_data->data);
    _item->adapter->ItemGetData(_item, &_data->data);
}

void MENU_ItemSetData(menu_itemIfce_t *_item, menu_nvmData_t *_data)
{
    if (0 == strncmp(_data->nameStr, _item->nameStr, MENU_NAME_STR_SIZE))
    {
        //MENU_ITEM_SWITCH_CASE(MENU_ItemSetData, _item, &_data->data);
        _item->adapter->ItemSetData(_item, &_data->data);
    }
    else
    {
        //TODO: print log error HERE.
    }
}

void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum)
{
    //TEXTMENU_PRINTF("-Verbose MENU: printing slot menu %s, type=%d, slot=%d.", _item->nameStr, _item->type, _slotNum);
    //MENU_ITEM_SWITCH_CASE(MENU_ItemPrintSlot, _item, _slotNum);
    _item->adapter->ItemPrintSlot(_item, _slotNum);
}

void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    //MENU_ITEM_SWITCH_CASE(MENU_ItemDirectKeyOp, _item, _op);
    _item->adapter->ItemDirectKeyOp(_item, _op);
}

void MENU_ItemPrintDisp(menu_itemIfce_t *_item)
{
    //MENU_ITEM_SWITCH_CASE(MENU_ItemPrintDisp, _item);
    _item->adapter->ItemPrintDisp(_item);
}

void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op)
{
    //MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, _item, _op);
    _item->adapter->ItemKeyOp(_item, _op);
}

#if defined(TEXTMENU_FEATURE_EVENTCB) && (TEXTMENU_FEATURE_EVENTCB != 0U)


bool MENU_ItemEventCbIsRequested(void)
{
	return 0U != menu.status.eventCbItemCnt;
}

void MENU_ItemRequestEventCb(menu_itemIfce_t *const _item, uint32_t const _event)
{
    assert(menu.status.eventCbItemCnt < TEXTMENU_CONFIG_EVENTQ_LEN);
    _item->eventFlag |= _event;
    menu.status.eventCbItem[menu.status.eventCbItemCnt++] = _item;
    //FIXME: Atomic operation! should disable event thread.
}

#endif // ! TEXTMENU_FEATURE_EVENTCB

/* @} */
