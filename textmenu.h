/**
 * Copyright 2018 - 2021 HITSIC
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
 * @file 	:	app_menu.h
 * @author  :	Chekhov Mark/马奇科(qq:905497173)
 * @version :	v0.1.4
 * 
 * @date 	:	v0.1-beta.0 2019.10.28
 * @date 	:	v0.1.1		2019.11.02
 * @date    :   v0.1.2      2019.11.22
 * @date    :   v0.1.3      2020.07.31
 * @date    :   v0.1.4      2020.10.09
 * @date    :   v0.1.5      2020.10.30
 * @date    :   v0.1.6      2020.11.09
 * @date    :   v0.1.7      2020.11.22
 * @date    :   v1.0.0      2021.04.02
 * 
 */

#ifndef TEXTMENU_H
#define TEXTMENU_H
//#include <hitsic_common.h>

#include <textmenu_port.h>

#include <sys_pitmgr.h>

//#include "textmenu_def.h"

#include "textmenu_core.h"
#include "textmenu_list.h"
#include "textmenu_iter.h"
#include "textmenu_kvdb.h"

#include "item/nullType.h"
#include "item/variType.h"
#include "item/varfType.h"
#include "item/boolType.h"
#include "item/procType.h"
#include "item/menuType.h"

 /*!
  * @addtogroup menu
  * @{
  */

/**
 * @brief : 菜单状态机。
 * @ {
 */
extern menu_list_t *menu_currList;             ///< 状态变量：指向当前所在的菜单列表。
extern menu_itemIfce_t *menu_currItem;         ///< 状态变量：指向当前所在的菜单项，仅位于菜单项内时有效。
extern menu_list_t *menu_menuRoot;             ///< 根菜单指针。
extern menu_list_t *menu_manageList;           ///< 管理菜单指针。
extern int32_t menu_currRegionNum[3];    ///< 当前局部存储区号
extern int32_t menu_statusFlag;                ///< 状态标志位
/**
 * @ }
 */

/**************************************
 ************ 菜单顶层操作接口 **********
 **************************************/

/**
 * @brief : 菜单全局初始化。
 * 该函数会初始化必要的变量，创建根菜单和其他最基本的菜单列表和菜单项。
 *
 * @param  {void} undefined :
 */
void MENU_Init(void);

/**
 * @brief : 初始化各菜单列表和菜单项。
 * 注意：该函数在外部定义，不需初始化最基本的菜单列表和菜单项。该函数禁止用户手动调用。
 *
 * @param  {void} undefined :
 */
void MENU_DataSetUp(void);



/**
 * @brief 向菜单发送按键信号
 */
void MENU_KeypadSignal(menu_keyOp_t _op);

/**
 * @brief : 根据字符串路径查找菜单列表。
 *
 * @param str : 字符串路径，以"/"分隔。注意：路径名应与跳转类型的菜单项
 *              名称一致，而不是与菜单列表名称一致。事实上，建议将菜单列表
 *              的名称与跳转类型菜单项的名称设为相同。
 * @retval 返回找到的菜单列表的指针。如果未找到则返回nullptr。
 */
menu_list_t *MENU_DirGetList(const char *str);

/**
 * @brief : 在菜单列表中搜索给定名称的菜单项。
 *
 * @param dir : 菜单项所在的菜单列表指针。
 * @param str : 菜单项名称字符串。
 * @retval 返回找到的菜单列表的指针。如果未找到则返回nullptr。
 */
menu_itemIfce_t *MENU_DirGetItem(const menu_list_t *dir, const char *str);

#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

/**
 * @brief : 保存整个菜单到NVM。
 *
 * @param  {int32_t} _region :  所选择的局部存储区。
 */
void MENU_Data_NvmSave(int32_t _region);

/**
 * @brief : 保存整个菜单到NVM。
 * 该函数将使用全局变量 menu_currRegionNum[0] 中保存的局部存储区号。
 * 
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 从NVM读取整个菜单。
 *
 * @param  {int32_t} _region : 所选择的局部存储区。
 */
void MENU_Data_NvmRead(int32_t _region);

/**
 * @brief : 从NVM读取整个菜单。
 * 该函数将使用全局变量 menu_currRegionNum[0] 中保存的局部存储区号。
 *
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 保存当前局部存储区号到NVM。
 * 该数值设置为不自动保存。
 *
 */
void MENU_Data_NvmSaveRegionConfig(void);

/**
 * @brief : 保存当前局部存储区号到NVM。
 * 该数值设置为不自动保存。
 *
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 从NVM中读取当前局部存储区号。
 * 该数值设置为不自动保存。
 *
 */
void MENU_Data_NvmReadRegionConfig(void);

/**
 * @brief : 从NVM中读取当前局部存储区号。
 * 该数值设置为不自动保存。
 * 
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmReadRegionConfig_Boxed(menu_keyOp_t *const _op);

#endif // ! TEXTMENU_USE_KVDB

/**
 * @brief : 定时中断管理器句柄。
 *
 */
void MENU_PitIsr(void* userData);

/**
 * @brief : 菜单事件处理函数。
 *      在事件处理任务中调用此函数。
 */
void MENU_EventService(void);

/**
 * @brief 挂起菜单。这将暂时禁用菜单的事件处理，包括按键输入和屏幕打印。
 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
 */
void MENU_Suspend(void);

/**
 * @brief 恢复菜单。这将启用菜单的事件处理，包括按键输入和屏幕打印。
 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
 */
void MENU_Resume(void);

/*! @} */

#endif // ! TEXTMENU_H
