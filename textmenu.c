/**
 * Copyright 2018 - 2020 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @file 	:	app_menu.c
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1.1
 * 
 * @date 	:	v0.1-beta.0 2019.10.28
 * @date 		v0.1.1		2019.11.02
 * 
 * @note    :   依赖库：drv_ftfx_flash、drv_button、drv_disp_ssd1306
 依赖库必须先初始化。
 * @note 	:	预发布版本，代码不完整，仅供学习。
 */

#include "textmenu.h"

 /*!
  * @addtogroup menu
  * @{
  */

#define SYSLOG_TAG  ("MENU.MAIN")
#define SYSLOG_LVL  (TEXTMENU_MAIN_LOG_LVL)
#include <inc_syslog.h>

/**
 * 菜单定义
 */

menu_list_t *menu_currList;             ///< 状态变量：指向当前所在的菜单列表。
menu_itemIfce_t *menu_currItem;         ///< 状态变量：指向当前所在的菜单项，仅位于菜单项内时有效。
menu_list_t *menu_menuRoot;             ///< 根菜单指针。
menu_list_t *menu_manageList;           ///< 管理菜单指针。
int32_t menu_statusFlag;                ///< 状态标志位

volatile uint32_t menu_suspendCnt = 0U;

/**
 * ********** 菜单顶层操作接口 **********
 */

void MENU_Init(void)
{
    SYSLOG_I("Init Begin: v%d.%d.%d", HITSIC_VERSION_MAJOR(APP_MENU_VERSION), HITSIC_VERSION_MINOR(APP_MENU_VERSION), HITSIC_VERSION_PATCH(APP_MENU_VERSION));

	menu_menuRoot = MENU_ListConstruct("MenuRoot", TEXTMENU_ROOT_SIZE, (menu_list_t *)1);
	assert(menu_menuRoot);
	MENU_ItemDestruct(menu_menuRoot->menu[0]);
	--menu_menuRoot->listNum;
	//menu_menuRoot->menu[0]->handle.p_menuType->data = menu_menuRoot;
	menu_currList = menu_menuRoot;
	menu_currItem = NULL;
	menu_statusFlag = 0;

	menu_manageList = MENU_ListConstruct("MenuManager", 21, menu_menuRoot);
	assert(menu_manageList);
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_menuType, menu_manageList, "MenuManager", 0, 0));
	{
#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)
		MENU_KVDB_MenuDataSetup(menu_manageList);
#endif // ! TEXTMENU_USE_KVDB
	}
	MENU_DataSetUp();

#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

	SYSLOG_I("Init: kvdb init begin.");

	menu_kvdb_metadata_t currMeta, kvdbMeta;
	MENU_KVDB_MetadataInit(&currMeta);

	void *currReg = NULL, *kvdbReg = NULL;
	uint32_t currRegSize = 0U, kvdbRegSize = 0U;


	status_t kvdbMetaStatus = kStatus_Success;
	status_t kvdbRegStatus = kStatus_Success;

	kvdbMetaStatus = MENU_KVDB_MetadataRead(&kvdbMeta);

	kvdbRegStatus = MENU_KVDB_RegistryRead(kvdbReg, &kvdbRegSize);


	if((kStatus_MENU_KVDB_KeyAbsence == kvdbMetaStatus) && (kStatus_MENU_KVDB_KeyAbsence == kvdbRegStatus))
	{
		//SYSLOG_I("Init: kvdb registry read failed!");

		SYSLOG_I("Init: kvdb metadata read failed!");

		SYSLOG_I("Create new metadata in kvdb.");
		kvdbMetaStatus = MENU_KVDB_MetadataSave(&currMeta);
		if(kStatus_Success != kvdbMetaStatus)
		{
			assert(0);
		}

		SYSLOG_I("Create new registry in kvdb.");
		kvdbRegStatus = MENU_KVDB_RegistryInit(currReg, &currRegSize);
		if(kStatus_Success != kvdbRegStatus)
		{
			assert(0);
		}
		kvdbRegStatus = MENU_KVDB_RegistrySave(currReg, currRegSize);
		if(kStatus_Success != kvdbRegStatus)
		{
			assert(0);
		}
	}
	else if((kStatus_MENU_KVDB_KeyAbsence == kvdbMetaStatus) && (kStatus_Success == kvdbRegStatus))
	{
		SYSLOG_A("Init: critical error. Registry read success while metadata not found.");
		SYSLOG_A("Please format KVDB and retry.");
		assert(0);
	}
	else if((kStatus_Success == kvdbMetaStatus) && (kStatus_MENU_KVDB_KeyAbsence == kvdbRegStatus))
	{
		SYSLOG_A("Init: critical error. Metadata read success while registry not found.");
		SYSLOG_A("Init: Please format KVDB and retry.");
		assert(0);
	}
	else if((kStatus_Success == kvdbMetaStatus) && (kStatus_Success == kvdbRegStatus))
	{
		SYSLOG_I("Init: Metadata and registry read success.");

	}
	else
	{
		SYSLOG_A("Init: Kvdb init failed.");
		assert(0);
	}




#endif // ! TEXTMENU_USE_KVDB

	NVIC_SetPriority(TEXTMENU_SERVICE_IRQn, TEXTMENU_SERVICE_IRQPrio); // FIXME: move this outside
	NVIC_EnableIRQ(TEXTMENU_SERVICE_IRQn); // FIXME: move this outside
	menu_suspendCnt = 0U;
	SYSLOG_I("Init Complete");
}

__WEAK void MENU_DataSetUp(void)
{
    SYSLOG_W("Using default menu structure (__WEAK). If not intended, define your own menu FIRST.");
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_nullType, NULL, "DATA", 0, 0));

	static int32_t region_i = 4096, global_i = 1024, readonly_i = 1998;
	static float region_f = 32.768, global_f = 3.14, longname_f = 12.14;
	static int32_t forceSciData = 202000;
	static menu_list_t *testList;
	testList = MENU_ListConstruct("TestList", 50, menu_menuRoot);
	assert(testList);
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_variType, &readonly_i, "readonly", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad));
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_menuType, testList, "TestList", 0, 0));
	{
		MENU_ListInsert(testList, MENU_ItemConstruct(&menu_itemAdapter_variType, &global_i, "global_i", 10, menuItem_data_global));
		MENU_ListInsert(testList, MENU_ItemConstruct(&menu_itemAdapter_varfType, &global_f, "global_f", 11, menuItem_data_global));
		MENU_ListInsert(testList, MENU_ItemConstruct(&menu_itemAdapter_variType, &region_i, "region_i", 1, menuItem_data_region));
		MENU_ListInsert(testList, MENU_ItemConstruct(&menu_itemAdapter_varfType, &region_f, "region_f", 2, menuItem_data_region));
	}
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_varfType, &longname_f, "C.M.'s Birthday", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_noPreview));
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_variType, &forceSciData, "forceSci", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_forceSci));
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_varfType, &longname_f, "123", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_noPreview));
	MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(&menu_itemAdapter_variType, &forceSciData, "fci", 0, menuItem_data_ROFlag | menuItem_data_NoSave | menuItem_data_NoLoad | menuItem_disp_forceSci));
}



void MENU_KeypadSignal(menu_keyOp_t _op)
{
    menu_keyOpBuff = _op;
    MENU_StatusFlagSet(menu_message_buttonOp);
    TEXTMENU_SERVICE_SEM_GIVE();
}



void MENU_PitIsr(void)
{
	MENU_StatusFlagSet(menu_message_printDisp);
	TEXTMENU_SERVICE_SEM_GIVE();
}

void MENU_EventService(void)
{
	if (menu_statusFlag & menu_message_buttonOp)
	{
		//TEXTMENU_PRINTF("Verbose: MENU: Key %d pressed.\n", menu_keyOpBuff);
		MENU_KeyOp(&menu_keyOpBuff);
	}
	if (menu_statusFlag & menu_message_printDisp)
	{
		MENU_PrintDisp();
	}
}

void MENU_Suspend(void)
{
	//assert(menu_pitHandle);
	if (0U == menu_suspendCnt)
	{
	    MENU_PORT_LowLevelSuspend();
		SYSLOG_I("Suspended.");
	}
	++menu_suspendCnt;
	SYSLOG_D("SuspendCnt = %d", menu_suspendCnt);
}

void MENU_Resume(void)
{
	//assert(menu_pitHandle);
	menu_suspendCnt = menu_suspendCnt == 0 ? menu_suspendCnt : menu_suspendCnt - 1;
	SYSLOG_D("SuspendCnt = %d", menu_suspendCnt);
	if (0U == menu_suspendCnt)
	{
		menu_statusFlag = 0;
		MENU_PORT_LowLevelResume();
		SYSLOG_I("Resumed.");
	}
}

//#undef SYSLOG_TAG
//#undef SYSLOG_LVL

/* @} */

