#include "textmenu_kvdb.h"

#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

#include "item/nullType.h"
#include "item/variType.h"
#include "item/varfType.h"
#include "item/boolType.h"
#include "item/byteType.h"
#include "item/procType.h"
#include "item/menuType.h"


/**
 * @brief : 菜单状态机。
 * @ {
 */
extern menu_list_t *menu_currList;             ///< 状态变量：指向当前所在的菜单列表。
extern menu_itemIfce_t *menu_currItem;         ///< 状态变量：指向当前所在的菜单项，仅位于菜单项内时有效。
extern menu_list_t *menu_menuRoot;             ///< 根菜单指针。
extern menu_list_t *menu_manageList;           ///< 管理菜单指针。
extern int32_t menu_statusFlag;                ///< 状态标志位
/**
 * @ }
 */

const char menu_itemNameStr_RegnSel[MENU_NAME_STR_SIZE] = {'R','e','g','n','S','e','l','(','0','-',('0' + TEXTMENU_NVM_REGION_CNT - 1),')','\0'};
int32_t menu_currRegionNum[3] = { 0, 0, TEXTMENU_NVM_REGION_CNT - 1 };    ///< 当前局部存储区号

/*!
 * @addtogroup menu_kvdb
 * @{
 */
#define SYSLOG_TAG  ("MENU.KVDB")
#define SYSLOG_LVL  (TEXTMENU_KVDB_LOG_LVL)
#include <inc_syslog.h>

status_t MENU_KVDB_MetadataInit(menu_kvdb_metadata_t *_data)
{
    _data->swVersion = APP_MENU_VERSION;
    _data->globalItemCnt = 0U;
    _data->regionItemCnt = 0U;
    _data->regionCnt = TEXTMENU_NVM_REGION_CNT;

    menu_iterator_t *iter = MENU_IteratorConstruct();
    if(NULL == iter)
    {
        SYSLOG_W("Metadata init failed! Iterator constructor failed.");
        return kStatus_Fail;
    }

    do
    {
        menu_itemIfce_t *item = MENU_IteratorDerefItem(iter);

        //Type constrain, only vari & varf will be saved.
        if((item->adapter != &menu_itemAdapter_variType) && (item->adapter != &menu_itemAdapter_varfType))
        {
            continue;
        }

        //Flag constrain, only global or region defined will be saved.
        if(item->pptFlag | menuItem_data_global)
        {
            ++_data->globalItemCnt;
            continue;
        }
        else if(item->pptFlag | menuItem_data_global)
        {
            ++_data->regionItemCnt;
            continue;
        }

    }while(kStatus_Success == MENU_IteratorIncrease(iter));

    MENU_IteratorDestruct(iter);

    SYSLOG_I("Metadata init success.");
    return kStatus_Success;
}

status_t MENU_KVDB_MetadataRead(menu_kvdb_metadata_t *_data)
{
    uint32_t size = 0U;
    if(kStatus_Success != MENU_PORT_KVDB_GetSize(MENU_KVDB_METADATA_KEY, &size) || (0U == size))
    {
        SYSLOG_I("Metadata read failed! Metadata not found.");
        return kStatus_MENU_KVDB_KeyAbsence;
    }
    else if(sizeof(menu_kvdb_metadata_t) != size)
    {
        SYSLOG_I("Metadata read failed! Metadata size incorrect.");
        return kStatus_Fail;
    }
    if(kStatus_Success != MENU_PORT_KVDB_ReadValue(MENU_KVDB_METADATA_KEY, _data, sizeof(menu_kvdb_metadata_t)))
    {
        SYSLOG_W("Metadata read failed! Read value failed.");
        return kStatus_MENU_KVDB_ReadError;
    }

    SYSLOG_I("Metadata read success.");
    return kStatus_Success;
} 

status_t MENU_KVDB_MetadataSave(menu_kvdb_metadata_t *_data)
{
    assert(_data);
    if(kStatus_Success != (MENU_KVDB_METADATA_KEY, _data, sizeof(menu_kvdb_metadata_t)))
    {
        SYSLOG_W("Metadata save failed! Save value failed.");
        return kStatus_MENU_KVDB_SaveError;
    }
    else
    {
        SYSLOG_I("Metadata save success.");
        return kStatus_Success;
    }
} 

status_t  MENU_KVDB_GenerateKey(menu_itemIfce_t const *const _item, char *const _str, uint32_t _size)
{//FIXME: functional but not optimal.
    if(_size < MENU_KVDB_REG_SIZE)
    {
        SYSLOG_W("Key generation failed! - no enough space.");
        return kStatus_Fail; 
    }
    if(_item->pptFlag & menuItem_data_global)
        { snprintf(_str, _size, "MENUG%4.4x", _item->saveAddr); }
    else if(_item->pptFlag & menuItem_data_region)
        { snprintf(_str, _size, "MENUR%4.4x", _item->saveAddr); }
    return kStatus_Success;
}

status_t MENU_KVDB_ResolveKey(char const * const _str, uint32_t *const _pptFlag, uint16_t *const _saveAddr)
{//FIXME: functional but not optimal.
    char c = ' ';
    sscanf(_str, "MENU%c%x", &c, _saveAddr);
    if('G' == c){*_pptFlag = menuItem_data_global;}
    else if('R' == c){*_pptFlag = menuItem_data_global;}
    return kStatus_Success;
}

status_t MENU_KVDB_RegistryInit(void *_data, uint32_t *_size)
{
    assert(NULL == _data);

    // first scan, determine how much ram to allocate
    menu_kvdb_metadata_t meta;
    MENU_KVDB_MetadataInit(&meta);

    *_size = (meta.globalItemCnt + meta.regionItemCnt) * MENU_KVDB_REG_SIZE;

    _data = malloc(*_size);
    if(NULL == _data)
    {
        *_size = 0U;
        SYSLOG_W("Registry init failed! Mem allocation failed.");
        return kStatus_Fail;
    }


    // second scan, generete registry
    char *str = (char*)_data;

    menu_iterator_t *iter = MENU_IteratorConstruct();
    if(NULL == iter)
    {
        SYSLOG_W("Registry init failed! Iterator constructor failed.");
        return kStatus_Fail;
    }

    do
    {
        menu_itemIfce_t *item = MENU_IteratorDerefItem(iter);

        //Type constrain, only vari & varf will be saved.
        if((item->adapter != &menu_itemAdapter_variType) && (item->adapter != &menu_itemAdapter_varfType))
        {
            continue;
        }

        //Flag constrain, only global defined will be saved.
        if(item->pptFlag | menuItem_data_global)
        {
            MENU_KVDB_GenerateKey(item, str, MENU_KVDB_REG_SIZE);
            SYSLOG_V("Registry add: %s", str);
            str += MENU_KVDB_REG_SIZE;
            continue;
        }

    }while(kStatus_Success == MENU_IteratorIncrease(iter));

    MENU_IteratorSetup(iter); // Reset iter to beginning

    do
    {
        menu_itemIfce_t *item = MENU_IteratorDerefItem(iter);

        //Type constrain, only vari & varf will be saved.
        if((item->adapter != &menu_itemAdapter_variType) && (item->adapter != &menu_itemAdapter_varfType))
        {
            continue;
        }

        //Flag constrain, only region defined will be saved.
        if(item->pptFlag | menuItem_data_global)
        {
            MENU_KVDB_GenerateKey(item, str, MENU_KVDB_REG_SIZE);
            SYSLOG_V("Registry add: %s", str);
            str += MENU_KVDB_REG_SIZE;
            continue;
        }

    }while(kStatus_Success == MENU_IteratorIncrease(iter));

    MENU_IteratorDestruct(iter);

    SYSLOG_I("Registry init success.");
    return kStatus_Success;
}

status_t MENU_KVDB_RegistryRead(void *_data, uint32_t *_size)
{
    assert(_data == NULL);

    if(kStatus_Success != MENU_PORT_KVDB_GetSize(MENU_KVDB_REGISTRY_KEY, _size) || (0U == *_size))
    {
        SYSLOG_I("Registry read failed! Registry not found.");
        return kStatus_MENU_KVDB_KeyAbsence;
    }

    _data = malloc(*_size);
    if(NULL == _data)
    {
        SYSLOG_W("Registry read failed! Mem allocation failed.");
        return kStatus_Fail;
    }

    if(kStatus_Success != MENU_PORT_KVDB_ReadValue(MENU_KVDB_REGISTRY_KEY, _data, *_size))
    {
        free(_data);
        SYSLOG_W("Registry read failed! Read value failed.");
        return kStatus_MENU_KVDB_ReadError;
    }

    SYSLOG_I("Registry read success. %6.6d byte(s) received.", *_size);
    return kStatus_Success;
}

status_t MENU_KVDB_RegistrySave(void *_data, uint32_t _size)
{
    assert(_data);
    if(kStatus_Success != MENU_PORT_KVDB_SaveValue(MENU_KVDB_REGISTRY_KEY, _data, _size))
    {
        SYSLOG_W("Registry save failed! Save value failed.");
        return kStatus_MENU_KVDB_SaveError;
    }
    else
    {
        SYSLOG_I("Registry save success. %6.6d byte(s) saved.", _size);
        return kStatus_Success;
    }
}

void MENU_KVDB_DataSave(int32_t _region)
{
	if(_region < 0 || (uint32_t)_region >= TEXTMENU_NVM_REGION_CNT) { return; }
//    ++menu_nvm_eraseCnt;
	SYSLOG_I("Data Save Begin");
	SYSLOG_I("Global Data");
	menu_iterator_t *iter = MENU_IteratorConstruct();
	char regKeyStr[MENU_KVDB_REG_SIZE];
	menu_nvmData_t dataBuf;
	do{
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoSave))
	    {
	        MENU_ItemGetData(thisItem, &dataBuf);
			SYSLOG_D("Get Data.  menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
			MENU_KVDB_GenerateKey(thisItem, regKeyStr, MENU_KVDB_REG_SIZE);
			MENU_PORT_KVDB_SaveValue(regKeyStr, (void*)&dataBuf, sizeof(menu_nvmData_t));
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	SYSLOG_I("Global Data End");
	if (menu_currRegionNum[0] < 0 || menu_currRegionNum[0] >= TEXTMENU_NVM_REGION_CNT)
	{
	    SYSLOG_W("RegionNum illegal! Aborting.");
	    MENU_IteratorDestruct(iter);
		return;
	}
	SYSLOG_I("Nvm Region %d Data", menu_currRegionNum[0]);
	MENU_IteratorSetup(iter);
	do{
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoSave))
	    {
	        MENU_ItemGetData(thisItem, &dataBuf);
	        SYSLOG_D("Get Data.  menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
			MENU_KVDB_GenerateKey(thisItem, regKeyStr, MENU_KVDB_REG_SIZE);
			MENU_KVDB_KeyAppendRegionNum(regKeyStr, _region);
			MENU_PORT_KVDB_SaveValue(regKeyStr, (void*)&dataBuf, sizeof(menu_nvmData_t));
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	SYSLOG_I("Region %d Data End.", menu_currRegionNum[0]);
	MENU_IteratorDestruct(iter);
	SYSLOG_I("Save Complete");
}

void MENU_KVDB_DataSave_Boxed(menu_keyOp_t *const _op)
{
	MENU_KVDB_DataSave(menu_currRegionNum[0]);
	*_op = 0;
}

void MENU_KVDB_DataRead(int32_t _region)
{
    static_assert(sizeof(menu_nvmData_t) == 32, "sizeof menu_nvmData_t error !");
	if(_region < 0 || (uint32_t)_region >= TEXTMENU_NVM_REGION_CNT) { return; }
	SYSLOG_I("Read Begin");
	SYSLOG_I("Global Data");
	menu_iterator_t *iter = MENU_IteratorConstruct();
	char regKeyStr[MENU_KVDB_REG_SIZE];
	menu_nvmData_t dataBuf;
	do{
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_global && !(thisItem->pptFlag & menuItem_data_NoLoad))
	    {
			MENU_KVDB_GenerateKey(thisItem, regKeyStr, MENU_KVDB_REG_SIZE);
			MENU_PORT_KVDB_ReadValue(regKeyStr, &dataBuf, sizeof(menu_nvmData_t));
	        SYSLOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
	        MENU_ItemSetData(thisItem, &dataBuf);
	        //SYSLOG_D("Set Data.  menu: %-16.16s addr: %-4.4d .", thisItem->nameStr, thisItem->saveAddr);
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	SYSLOG_I("Global Data End.");
	if (menu_currRegionNum[0] < 0 || menu_currRegionNum[0] >= TEXTMENU_NVM_REGION_CNT)
	{
	    SYSLOG_W("RegionNum illegal! Aborting");
	    MENU_IteratorDestruct(iter);
		return;
	}
	SYSLOG_I("Region %d Data.", menu_currRegionNum[0]);
	MENU_IteratorSetup(iter);
	do{
	    menu_itemIfce_t *thisItem = MENU_IteratorDerefItem(iter);
	    if (thisItem->pptFlag & menuItem_data_region && !(thisItem->pptFlag & menuItem_data_NoLoad))
	    {
			MENU_KVDB_GenerateKey(thisItem, regKeyStr, MENU_KVDB_REG_SIZE);
			MENU_KVDB_KeyAppendRegionNum(regKeyStr, _region);
			MENU_PORT_KVDB_ReadValue(regKeyStr, &dataBuf, sizeof(menu_nvmData_t));
	        SYSLOG_D("Get Flash. menu: %-16.16s addr: %-4.4d data: 0x%-8.8x .", dataBuf.nameStr, thisItem->saveAddr, dataBuf.data);
	        MENU_ItemSetData(thisItem, &dataBuf);
	        //SYSLOG_D("Set Data.  menu: %-16.16s addr: %-4.4d .", thisItem->nameStr, thisItem->saveAddr);
	    }
	}while(kStatus_Success == MENU_IteratorIncrease(iter));
	SYSLOG_I("Region %d Data End", menu_currRegionNum[0]);
	MENU_IteratorDestruct(iter);
	SYSLOG_I("Read complete");
}

void MENU_KVDB_DataRead_Boxed(menu_keyOp_t *const _op)
{
	MENU_KVDB_DataRead(menu_currRegionNum[0]);
	*_op = 0;
}

void MENU_KVDB_DataSaveRegionConfig(void)
{
	SYSLOG_I("Saving region config ...");
	menu_nvmData_t dataBuf;
	menu_itemIfce_t *thisItem = MENU_DirGetItem(MENU_DirGetList("/MenuManager"), menu_itemNameStr_RegnSel);
	MENU_ItemGetData(thisItem, &dataBuf);
	char regKeyStr[MENU_KVDB_REG_SIZE];
	MENU_KVDB_GenerateKey(thisItem, regKeyStr, MENU_KVDB_REG_SIZE);
	MENU_PORT_KVDB_SaveValue(regKeyStr, (void*)&dataBuf, sizeof(menu_nvmData_t));
	SYSLOG_I("Save region config complete");
}
void MENU_KVDB_DataSaveRegionConfig_Boxed(menu_keyOp_t *const _op)
{
	MENU_KVDB_DataSaveRegionConfig();
	*_op = 0;
}

void MENU_KVDB_DataReadRegionConfig(void)
{
    SYSLOG_I("Reading region config ...");
	menu_nvmData_t dataBuf;
	menu_itemIfce_t *thisItem = MENU_DirGetItem(MENU_DirGetList("/MenuManager"), menu_itemNameStr_RegnSel);
	char regKeyStr[MENU_KVDB_REG_SIZE];
	MENU_KVDB_GenerateKey(thisItem, regKeyStr, MENU_KVDB_REG_SIZE);
	MENU_PORT_KVDB_ReadValue(regKeyStr, (void*)&dataBuf, sizeof(menu_nvmData_t));
	MENU_ItemSetData(thisItem, &dataBuf);
	SYSLOG_I("Read region config complete");
}

void MENU_KVDB_DataReadRegionConfig_Boxed(menu_keyOp_t *const _op)
{
	MENU_KVDB_DataReadRegionConfig();
	*_op = 0;
}

void MENU_KVDB_MenuDataSetup(menu_list_t *_list)
{
    menu_itemIfce_t *p = NULL;
	MENU_ListInsert(_list, MENU_ItemConstruct(&menu_itemAdapter_nullType, NULL, "SAVE", 0, 0));
	MENU_ListInsert(_list, p = MENU_ItemConstruct(&menu_itemAdapter_variType, menu_currRegionNum, menu_itemNameStr_RegnSel/*"RegnSel(0-N)"*/, 0, menuItem_data_global | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_dataExt_HasMinMax));
	//p->nameStr[10] = '0' + TEXTMENU_NVM_REGION_CNT - 1;
	MENU_ListInsert(_list, MENU_ItemConstruct(&menu_itemAdapter_procType, (void *)MENU_KVDB_DataSave_Boxed, "Save Data", 0, menuItem_proc_runOnce));
	MENU_ListInsert(_list, MENU_ItemConstruct(&menu_itemAdapter_procType, (void *)MENU_KVDB_DataRead_Boxed, "Load Data", 0, menuItem_proc_runOnce));
	MENU_ListInsert(_list, MENU_ItemConstruct(&menu_itemAdapter_procType, (void *)MENU_KVDB_DataSaveRegionConfig_Boxed, "RegnSave", 0, menuItem_proc_runOnce));
	MENU_ListInsert(_list, MENU_ItemConstruct(&menu_itemAdapter_nullType, NULL, "INFO", 0, 0));
}

/* @} */

#endif // ! TEXTMENU_USE_KVDB
