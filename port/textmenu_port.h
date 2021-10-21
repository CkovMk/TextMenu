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

#ifndef D_MK66F18_APP_MENU_PORT_H
#define D_MK66F18_APP_MENU_PORT_H

#include <inc_stdlib.h>
#include "hitsic_common.h"

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

#define kStatusGroup_MENU (205U)

/**
 * @name 调试输出
 * @ {
 */

/*! 核心逻辑 LOG级别定义 */
#define TEXTMENU_MAIN_LOG_LVL    (3U)

/*! 数据存储 LOG级别定义 */
#define TEXTMENU_KVDB_LOG_LVL    (3U)

/*! 按键处理 LOG级别定义 */
#define TEXTMENU_BUTN_LOG_LVL    (3U)

/*! 菜单项目 LOG级别定义 */
#define TEXTMENU_ITEM_LOG_LVL    (3U)

/*! 菜单列表 LOG级别定义 */
#define TEXTMENU_LIST_LOG_LVL    (3U)

/*! 菜单迭代 LOG级别定义 */
#define TEXTMENU_ITER_LOG_LVL    (2U)

/* @ } */

/** @brief : 根菜单最大容量 */
#define TEXTMENU_ROOT_SIZE 35 //maximum size of Root Menu

/**
 * @name 事件任务接口
 * @brief : 菜单消息调用所使用的中断信息。
 * 可以使用任何当前工程中未使用的中断。注意中断号和中断服务函数
 * 必须对应。优先级不可过高。
 * @ {
 */
#include <MK66F18.h>
#include "fsl_common.h"
#define TEXTMENU_SERVICE_IRQHandler (Reserved85_IRQHandler)  ///< 要使用的中断服务函数
#define TEXTMENU_SERVICE_IRQn (Reserved85_IRQn)              ///< 要使用的中断号
#define TEXTMENU_SERVICE_IRQPrio (12u)                       ///< 中断优先级，需要设置一个较低的值，以免打断重要任务。

#define TEXTMENU_SERVICE_SEM_GIVE() NVIC_SetPendingIRQ(TEXTMENU_SERVICE_IRQn)
#define TEXTMENU_SERVICE_SEM_TAKE() NVIC_ClearPendingIRQ(TEXTMENU_SERVICE_IRQn)
/* @ } */



/**
 * @name 屏幕打印接口
 * @ {
 */


/**
 * @brief 字符缓存大小（行、列）
 * 
 * 行、列均为实际能够显示的行数和列数，不需要考虑C语言的'\0'。
 */
#define TEXTMENU_DISPLAY_STRBUF_ROW (8u)
#define TEXTMENU_DISPLAY_STRBUF_COL (21u)

/**
 * @brief 启用色盘
 */
#define TEXTMENU_USE_PALETTE (0U)

#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)

#define TEXTMENU_DISPLAY_PALETTE_SIZE (8U) // uint8_t, 1~255

typedef uint8_t menu_dispColor_t;

#define TEXTMENU_DISPLAY_PALETTE_DEF \
    {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

#define TEXTMENU_DISPLAY_PAL_IDX_NORMAL_F (1U) ///< 前景色-正常
#define TEXTMENU_DISPLAY_PAL_IDX_NORMAL_B (0U) ///< 背景色-正常
#define TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F (1U) ///< 前景色-高亮
#define TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B (0U) ///< 背景色-高亮
#define TEXTMENU_DISPLAY_PAL_IDX_TITBAR_F (1U) ///< 前景色-标题
#define TEXTMENU_DISPLAY_PAL_IDX_TITBAR_B (0U) ///< 背景色-标题

#endif // ! TEXTMENU_USE_PALETTE



/**
 * @brief
 *
 * @param _buf
 */
void MENU_PORT_DisplayOutput(menu_strBuf_t *_buf);

/* @ } */

/**
 *  @name 键值数据库
 *  @ {
 */
/*! @brief 是否启用非易失性键值数据库存储支持。 */
#define TEXTMENU_USE_KVDB (1U)

#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

/**
 * ********** NVM存储变量定义 **********
 */

//gl = global
//rg = region
//addr = address

/**
 * @brief : 局部存储 Region Storage
 */
#define TEXTMENU_NVM_REGION_CNT (3) 					///< 局部存储区的数量

/**
 * @brief : 键值数据库接口
 */


status_t MENU_PORT_KVDB_GetSize(char const *_key, uint32_t *_size);

status_t MENU_PORT_KVDB_ReadValue(char const *_key, void *_data , uint32_t _size);

status_t MENU_PORT_KVDB_SaveValue(char const *_key, void const *_data, uint32_t _size);

status_t MENU_PORT_KVDB_DeltValue(char const *_key);

#endif // ! TEXTMENU_USE_KVDB

/* @ } */

#endif // ! HITSIC_USE_APP_MENU


#endif // ! D_MK66F18_APP_MENU_PORT_H

