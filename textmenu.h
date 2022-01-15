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
 * @author  :	Chekhov Mark<maqikemsft@outlook.com>
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
#include "item/byteType.h"
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
// extern menu_list_t *menu.status.currList;             ///< 状态变量：指向当前所在的菜单列表。
// extern menu_itemIfce_t *menu.status.currItem;         ///< 状态变量：指向当前所在的菜单项，仅位于菜单项内时有效。
// extern menu_list_t *menu.menuRoot;             ///< 根菜单指针。
// extern menu_list_t *menu.manageList;           ///< 管理菜单指针。
// extern int32_t menu.status.flag;                ///< 状态标志位
// #if defined(TEXTMENU_FEATURE_EVENTCB) && (TEXTMENU_FEATURE_EVENTCB != 0U)
// extern menu_itemIfce_t *menu.status.eventCbItem;      ///< 状态变量：指向请求回调的菜单项。
// extern uint8_t menu.status.eventCbItemCnt;
// #endif // ! TEXTMENU_FEATURE_EVENTCB
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
 * @brief : 定时中断管理器句柄。
 *
 */
void MENU_PitIsr(void);

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
