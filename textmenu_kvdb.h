#ifndef TEXTMENU_KVDB_H
#define TEXTMENU_KVDB_H


#include "textmenu_port.h"
#if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "textmenu_core.h"
#include "textmenu_item.h"
#include "textmenu_iter.h"
/*!
 * @addtogroup menu_kvdb
 * @{
 */

/**
 * @brief : 键值数据库接口前置定义
 */

status_t MENU_PORT_KVDB_GetSize(char const *_key, uint32_t *_size);

status_t MENU_PORT_KVDB_ReadValue(char const *_key, void *_data , uint32_t _size);

status_t MENU_PORT_KVDB_SaveValue(char const *_key, void const *_data, uint32_t _size);

status_t MENU_PORT_KVDB_DeltValue(char const *_key);

/**
 * @brief 元数据键值。
 */
#define MENU_KVDB_METADATA_KEY "MENU_METADATA"

/**
 * @brief 元数据结构体。
 */
__PACKED typedef struct _menu_kvdb_metadata
{
    uint32_t swVersion; ///< 软件版本
    uint16_t globalItemCnt; ///< 全局数据区数据项数量
    uint16_t regionItemCnt; ///< 局部数据区数据项数量
    uint8_t regionCnt; ///< 局部数据区数量
    uint8_t dummy[3];
}menu_kvdb_metadata_t;

/**
 * @brief 生成元数据。
 * 
 * @param {menu_kvdb_metadata_t *} _data : 用于保存数据的指针。不得传入空指针。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。
 */
status_t MENU_KVDB_MetadataInit(menu_kvdb_metadata_t *_data);

/**
 * @brief 读取元数据
 * 
 * @param {menu_kvdb_metadata_t *} _data : 用于保存数据的指针。不得传入空指针。
 * @param {uint32_t *} _size : 保存读取数据的大小（字节）。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。
 */
status_t MENU_KVDB_MetadataRead(menu_kvdb_metadata_t *_data);

/**
 * @brief 保存元数据
 * 
 * @param {menu_kvdb_metadata_t *} _data : 想要保存的数据的指针。
 * @param {uint32_t *} _size : 数据的大小（字节）。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。
 */
status_t MENU_KVDB_MetadataSave(menu_kvdb_metadata_t *_data);



/**
 * @brief 注册表键值。
 */
#define MENU_KVDB_REGISTRY_KEY "MENU_REGISTRY"

/**
 * @brief 注册表项的大小。即每个菜单项对应的注册表项所占大小。
 */
#define MENU_KVDB_REG_SIZE (10U) // MENU + G/R + XXXX + '\0', 'R' will be replaced by Region Num when read actual data, where 'G' will stay in it's place

#define MENU_KVDB_KeyAppendRegionNum(key, regionNum) (key[4] = '0' + regionNum)

/**
 * @brief 生成键
 * 
 * @param {menuItem_t const *const} _item : 要生成键的菜单项指针。
 * @param {char *} _str : 存储键的字符串指针。
 * @param {uint32_t*} _size : 字符串指针的可用大小。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。当_size空间不足时返回kStatus_Fail。
 */
status_t MENU_KVDB_GenerateKey(menu_itemIfce_t const *const _item, char *_str, uint32_t _size);

/**
 * @brief 解析键
 * 
 * @param {char const * const} _str : 要解析的键字符串。
 * @param {uint32_t *const} _pptFlag : 解析出的存储区属性，可以为menuItem_data_global或menuItem_data_region。
 * @param {uint16_t *const} _saveAddr : 解析出的存储ID。
 * @retval {status_t} : 返回状态。总是返回kStatus_Success。
 */
status_t MENU_KVDB_ResolveKey(char const * const _str, uint32_t *const _pptFlag, uint16_t *const _saveAddr);


/**
 * @brief 生成注册表。
 * 
 * @param {void *} _data : 用于保存数据的指针。必须传入空指针。
 * @param {uint32_t *} _size : 保存生成数据的大小（字节）。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。
 */
status_t MENU_KVDB_RegistryInit(void *_data, uint32_t *_size);

/**
 * @brief 读取注册表
 * 
 * @param {void *} _data : 用于保存数据的指针。必须传入空指针。
 * @param {uint32_t *} _size : 保存读取数据的大小（字节）。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。
 */
status_t MENU_KVDB_RegistryRead(void *_data, uint32_t *_size);

/**
 * @brief 保存注册表
 * 
 * @param {void *} _data : 想要保存的数据的指针。
 * @param {uint32_t *} _size : 数据的大小（字节）。
 * @retval {status_t} : 返回状态。成功返回kStatus_Success。
 */
status_t MENU_KVDB_RegistrySave(void *_data, uint32_t _size);



/* @} */

#endif // ! TEXTMENU_USE_KVDB

#endif // ! TEXTMENU_KVDB_H
