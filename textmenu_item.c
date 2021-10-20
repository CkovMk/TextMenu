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
uint32_t menu_itemCnt = 0;
uint32_t menu_listCnt = 0;

const int32_t menu_itemAdjustLut[] =
{ 1, 10, 100, 1000, 10000, 100000 };




/**
 * ********** 菜单项操作接口 **********
 */

menu_itemIfce_t *MENU_ItemConstruct(menu_itemType_t _type, void *_data, const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag)
{

    menu_itemIfce_t *item;
    item = (menu_itemIfce_t*)calloc(1, sizeof(menu_itemIfce_t));
    assert(item);
    item->type = _type;
    item->pptFlag = _pptFlag;
    item->unique_id = menu_itemCnt++;
    item->saveAddr = _saveAddr;
    strncpy(item->nameStr, _nameStr, MENU_NAME_STR_SIZE);
    item->nameStr[MENU_NAME_STR_SIZE - 1] = '\0';

    MENU_ITEM_SWITCH_CASE(MENU_ItemConstruct, item, _data);
    return item;
}

void MENU_ItemDestruct(menu_itemIfce_t *_item)
{
    free(_item->handle.p_void);
    free(_item);
    _item = NULL;
    --menu_itemCnt;
}

void MENU_ItemGetData(menu_itemIfce_t *_item, menu_nvmData_t *_data)
{
    memcpy(_data->nameStr, _item->nameStr, MENU_NAME_STR_SIZE);
    _data->type = _item->type;
    //MENU_ITEM_SWITCH_CASE(MENU_ItemGetData, _item, &_data->data);
    _item->adapter->ItemGetData(_item, &_data->data);
}

void MENU_ItemSetData(menu_itemIfce_t *_item, menu_nvmData_t *_data)
{
    if (0 == strncmp(_data->nameStr, _item->nameStr, MENU_NAME_STR_SIZE) && _data->type == (uint32_t)_item->type)
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
    MENU_ITEM_SWITCH_CASE(MENU_ItemPrintSlot, _item, _slotNum);
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


/* @} */
