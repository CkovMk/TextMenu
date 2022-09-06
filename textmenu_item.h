#ifndef TEXTMENU_ITEM_H
#define TEXTMENU_ITEM_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "textmenu_core.h"
#include "textmenu_keyop.h"

/*!
 * @addtogroup menu_type
 * @{
 */

// /**
//  * @brief : 菜单项类别函数命名宏定义
//  */
// #define MENU_ITEM_MAKE_FUNCTION(funcName, type) (funcName##_##type)
// #define MENU_ITEM_MAKE_HANDLE(p_Item, type) ((p_Item->handle).p_##type##)
// #define MENU_ITEM_CALL_FUNCTION(funcName, type, p_Item, ...) (MENU_ITEM_MAKE_FUNCTION(funcName, type)(p_Item, ##__VA_ARGS__))

// /**
//  * @brief : 菜单项分类调用switch-case语句的宏定义。
//  * 使用此语句简化menu_itemIfce操作函数内的switch-case调用。
//  *
//  * @param funcName 	: 要分类切换的函数名称。
//  * @param p_Item 	: 指向menu_itemIfce_t结构体的指针。
//  * @param ... 		: 除上述指针以外的其他参数。没有其他参数则不写。
//  *
//  * @example :
//  *
//  * 		void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t * const _op)
//  * 		{
//  * 			MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, _item, _op);
//  * 		}
//  */
// #define MENU_ITEM_SWITCH_CASE(funcName, p_Item, ...)                        \
//     switch (p_Item->type)                                                   \
//     {                                                                       \
//     case nullType:                                                          \
//         MENU_ITEM_CALL_FUNCTION(funcName, nullType, p_Item, ##__VA_ARGS__); \
//         break;                                                              \
//     case variType:                                                          \
//         MENU_ITEM_CALL_FUNCTION(funcName, variType, p_Item, ##__VA_ARGS__); \
//         break;                                                              \
//     case varfType:                                                          \
//         MENU_ITEM_CALL_FUNCTION(funcName, varfType, p_Item, ##__VA_ARGS__); \
//         break;                                                              \
//     case boolType:                                                          \
//         MENU_ITEM_CALL_FUNCTION(funcName, boolType, p_Item, ##__VA_ARGS__); \
//         break;                                                              \
//     case procType:                                                          \
//         MENU_ITEM_CALL_FUNCTION(funcName, procType, p_Item, ##__VA_ARGS__); \
//         break;                                                              \
//     case menuType:                                                          \
//         MENU_ITEM_CALL_FUNCTION(funcName, menuType, p_Item, ##__VA_ARGS__); \
//         break;                                                              \
//     default:                                                                \
//         break;                                                              \
//     }

// test marco :
// MENU_ITEM_MAKE_FUNCTION(item, nullType, KeyOp);
// MENU_ITEM_CALL_FUNCTION(MENU_ItemConstruct, item, variType, &data);
// MENU_ITEM_SWITCH_CASE(MENU_ItemKeyOp, item, &op);


/**
 * @brief : 菜单项属性枚举类型。
 * 标志位枚举。
 */
typedef enum
{
    /** data config */
    menuItem_data_global = 1 << 0, ///< 该菜单项存储在全局数据区。
    menuItem_data_region = 1 << 1, ///< 该菜单项存储在局部数据区。
    menuItem_data_getPos = menuItem_data_global | menuItem_data_region, ///< 如果既没有定义全局存储区，又没有定义局部存储区，则该菜单项无法保存。
    menuItem_data_ROFlag = 1 << 2, ///< 该菜单项为只读。只读菜单项不允许在菜单内修改。
    menuItem_data_NoSave = 1 << 3, ///< 该菜单项默认不保存到NVM，需要手动保存。
    menuItem_data_NoLoad = 1 << 4, ///< 该菜单项默认不从NVM读取，需要手动读取。
    menuItem_data_getCfg = menuItem_data_global | menuItem_data_region
            | menuItem_data_ROFlag | menuItem_data_NoSave
            | menuItem_data_NoLoad,
    menuItem_dataExt_HasMinMax = 1 << 5, ///< 数据属性扩展：最小/最大值限制。该属性使用data指针偏移+1和+2的地址。

    /** display config */
    menuItem_disp_forceSci = 1 << 8, ///< 该菜单项强制使用科学计数法，适用于variType和varfType。
    menuItem_disp_noPreview = 1 << 10, ///< 该菜单项不会在菜单列表中显示数据。建议可在菜单项名称过长时使用。

    menuItem_proc_runOnce = 1 << 11, ///< 该菜单项只会运行一次。仅适用于procType。
    menuItem_proc_uiDisplay = 1 << 12, ///< 该菜单项会自行打印屏幕。仅适用于procType。

    /** error mask */
} menu_itemPropety_t;

//  /**
//   * @brief : 菜单项所支持的内容类型。
//   */
// typedef enum
// {
//     nullType, // null type
//     variType, // watch or set integer varibles
//     varfType, // watch or set float-point varibles
//     boolType,
//     procType, // run certain process
//     menuType, // jump to another menu
// } menu_itemType_t;


#if defined(TEXTMENU_FEATURE_EVENTCB) && (TEXTMENU_FEATURE_EVENTCB != 0U)

/**
 * @brief Basic event type supported by default.
 * 
 */
typedef enum _menu_itemEvent
{
    menu_itemEvent_onListSelect = 1U << 16U;
    menu_itemEvent_onListDeselect = 1U << 17U,

    menu_itemEvent_onCmdEnter = 1U << 20U,
    menu_itemEvent_onCmdExit = 1U << 21U,

    menu_itemEvent_onDataChange = 1U << 24U,
}menu_itemEvent_t;

/** 
 * @brief Event callback function prototype.
 * 
 * @param _eventFlag Event flag set by menu internal logic.
 * @param _clearMask Event flag to clear after callback execution.
 * @param _userData  User pre-defined value, used as coockies.
 */
typedef (void)(*menu_itemEventHandler_t)(uint32_t const _eventFlag, uint32_t *const _clearMask, void *_userData);

#endif // ! TEXTMENU_FEATURE_EVENTCB

/** 前置定义 */
typedef struct _menu_itemAdapter menu_itemAdapter_t;

/** @brief : 菜单项接口结构体。 */
typedef struct _menu_itemIfce
{
    //menu_itemType_t type;               ///< 此菜单项的类型。
    uint32_t pptFlag;                   ///< 此菜单项的属性标志位。
    uint16_t list_id, unique_id;        ///< 此菜单项在本列表内的序号（从0开始）、全局唯一序号（从0开始）
    uint16_t saveAddr;                  ///< 此菜单在本区域内的偏移地址。从0开始，以1步进。注意，全局数据区和局部数据区的地址分开来算。
    char nameStr[MENU_NAME_STR_SIZE];     ///< 此菜单项的名称字符串。最大长度为MENU_NAME_STR_SIZE - 1 字节。
//    union menu_itemIfce_handle_t        ///< 菜单项操作句柄的共用体。使用时根据此菜单项的类型调取对应项访问。
//    {
//        void *p_void;
//        menu_item_nullHandle_t *p_nullType;
//        menu_item_variHandle_t *p_variType;
//        menu_item_varfHandle_t *p_varfType;
//        menu_item_boolHandle_t *p_boolType;
//        menu_item_procHandle_t *p_procType;
//        menu_item_menuHandle_t *p_menuType;
//    } handle;
    void *p_handle;
    menu_itemAdapter_t const *adapter;  ///< 指向存放菜单项命令函数指针的结构体。参考C++虚表
#if defined(TEXTMENU_FEATURE_EVENTCB) && (TEXTMENU_FEATURE_EVENTCB != 0U)
    uint32_t eventEnable;                   /*! Or-ed bits of all enabled flags. */
    uint32_t eventFlag;
    void *eventUserData;                    /*! User data. */
    menu_itemEventHandler_t eventHandler;   /*! Callback handler function. */
#endif // ! TEXTMENU_FEATURE_EVENTCB
} menu_itemIfce_t;

typedef struct _menu_itemData_t
{
    uint32_t size;
    void *pData;
} menu_itemData_t;

/** @brief : Adapter struct for various item types. */
struct _menu_itemAdapter
{
    void (*ItemConstruct)(menu_itemIfce_t *_item, void *_data);
    void (*ItemGetData)(menu_itemIfce_t *_item, menu_itemData_t *_data);
    void (*ItemSetData)(menu_itemIfce_t *_item, menu_itemData_t *_data);
    void (*ItemPrintSlot)(menu_itemIfce_t *_item, uint32_t _slotNum);
    void (*ItemDirectKeyOp)(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
    void (*ItemPrintDisp)(menu_itemIfce_t *_item);
    void (*ItemKeyOp)(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
};


/*************************************
 ************ 菜单项操作接口 **********
 *************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : 菜单项结构体的构造函数。
 * 该函数分配一块内存，根据传入的参数填入内容，然后返回所产生的菜单项结构体指针。
 *
 * @param  {menu_itemType_t} _type : 要生成的菜单项类型。
 * @param  {void*} _data           : 指向要操作的数据的指针。
 * @param  {char*} _nameStr        : 菜单项名称字符串，不超过最大长度，以'\0'结尾。
 * @param  {uint32_t} _saveAddr    : 变量要保存到的地址。对非数据类型及属性设置中只读的菜单项无效。
 * @param  {uint32_t} _pptFlag     : 属性标志。
 *
 * @return {menu_itemIfce_t *}     : 返回所产生的菜单项结构体指针。
 */
menu_itemIfce_t* MENU_ItemConstruct(menu_itemAdapter_t const *_type, void *_data,
        const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag);

/**
 * @brief : 菜单项结构体析构函数。
 *
 * @param  {menu_itemIfce_t*} _item : 要析构的菜单项结构体指针。析构后该指针会被自动赋值为NULL。
 */
void MENU_ItemDestruct(menu_itemIfce_t *_item);

/**
 * @brief : 获取菜单项内存放的数据指针。
 * TODO : 更新注释
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
 *
 * @return {void *}					: 返回数据指针。注意，无论何种类型，都会返回对应的数据指针。
 */
void MENU_ItemGetData(menu_itemIfce_t *_item, menu_itemData_t *_data);

/**
 * @brief : 设置菜单项内指针指向的数据的值。
 * 注意，该函数对非数据类型菜单和只读数据菜单无效。
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
 * @param  {void*} _data            :
 */
void MENU_ItemSetData(menu_itemIfce_t *_item, menu_itemData_t *_data);

//used when in menuList

/**
 * @brief : 在菜单列表中打印一行菜单。
 *
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 * @param  {uint32_t} _slotNum      : 要打印位置（第几行）。对于12864 OLED而言，取值范围为0~7。
 */
void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum);

/**
 * @brief : 在菜单列表中响应直接按键操作。
 *
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
 */
void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op);

//used when in menuItem

/**
 * @brief : 打印访问菜单项的二级页面。
 *
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 */
void MENU_ItemPrintDisp(menu_itemIfce_t *_item);

/**
 * @brief : 访问菜单项的二级页面时响应按键操作。
 *
 * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
 * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
 */
void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op);



#if defined(TEXTMENU_FEATURE_EVENTCB) && (TEXTMENU_FEATURE_EVENTCB != 0U)


bool MENU_ItemEventCbIsRequested(void);

void MENU_ItemRequestEventCb(menu_itemIfce_t *const _item, uint32_t const _event);

#endif // ! TEXTMENU_FEATURE_EVENTCB

#ifdef __cplusplus
}
#endif

/* @} */

#endif // ! TEXTMENU_ITEM_H
