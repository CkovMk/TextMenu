#ifndef TEXTMENU_CORE_H
#define TEXTMENU_CORE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "textmenu_port.h"
#include "textmenu_strbuf.h"

/** @brief : 软件版本 */
#define APP_MENU_VERSION (HITSIC_MAKE_VERSION(1U, 0U, 0U))

/**
 * @brief : 菜单项和菜单列表名称的最大长度为16个字符。用于定义缓存区大小。
 */
#define MENU_NAME_STR_SIZE (16u)

#include "textmenu_item.h"
#include "textmenu_list.h"

/*! @brief Error codes for MENU. */
enum
{
    kStatus_MENU_KVDB_KeyAbsence = MAKE_STATUS(kStatusGroup_MENU, 10), /*!< Key provided is absent in KVDB. */
    kStatus_MENU_KVDB_ReadError  = MAKE_STATUS(kStatusGroup_MENU, 11), /*!< Value read from KVDB error. */
    kStatus_MENU_KVDB_SaveError  = MAKE_STATUS(kStatusGroup_MENU, 12), /*!< Value save to KVDB error. */
};

/**
 * BUTTON
 */

/**
 * @brief : 按键操作宏定义。
 *
 */
typedef enum _menu_keyOpCode
{
    menuOpCode_generic_nl = 0, ///< NULL

    /* 五向按键，UP+DN+LF+RT+OK */
    menuOpCode_5wayStick_ok = 1,
    menuOpCode_5wayStick_up = 2,
    menuOpCode_5wayStick_dn = 3,
    menuOpCode_5wayStick_lf = 4,
    menuOpCode_5wayStick_rt = 5,

    /* 三向按键，LF+RT+OK */
    menuOpCode_3wayStick_ok = 6,
    menuOpCode_3wayStick_lf = 7,
    menuOpCode_3wayStick_rt = 8,

    /* 3*4矩阵键盘，0~9 + '*' + '#' */
    menuOpCode_numpad3b4_n0 = 10,
    menuOpCode_numpad3b4_n1 = 11,
    menuOpCode_numpad3b4_n2 = 12,
    menuOpCode_numpad3b4_n3 = 13,
    menuOpCode_numpad3b4_n4 = 14,
    menuOpCode_numpad3b4_n5 = 15,
    menuOpCode_numpad3b4_n6 = 16,
    menuOpCode_numpad3b4_n7 = 17,
    menuOpCode_numpad3b4_n8 = 18,
    menuOpCode_numpad3b4_n9 = 19,
    menuOpCode_numpad3b4_as = 20, // '*', asterisk
    menuOpCode_numpad3b4_hs = 21, // '#', hash

    /* 双层编码器 */
    menuOpCode_2dEncoder_hp = 30, // 高层编码器，加
    menuOpCode_2dEncoder_hn = 31, // 高层编码器，减
    menuOpCode_2dEncoder_lp = 32, // 低层编码器，加
    menuOpCode_2dEncoder_ln = 33, // 低层编码器，减
    menuOpCode_2dEncoder_ok = 34, // 编码器按键按下

    /* 通用功能 */
    menuOpCode_genericfn_home = 200, // home
    menuOpCode_genericfn_back = 201, // back
    menuOpCode_genericfn_acpt = 202, // accept
    menuOpCode_genericfn_deny = 203, // deny
    menuOpCode_genericfn_info = 204, // info
} menu_keyOpCode_t;

typedef enum _menu_keyOpType_t
{
    menuOpType_shrt = 1 << 8,
    menuOpType_long = 2 << 8,
    menuOpType_lrpt = 3 << 8,
    menuOpType_doub = 4 << 8,
} menu_keyOpType_t;

typedef uint32_t menu_keyOp_t;

/**
 * @brief : 按键操作生成宏
 */
#define MENU_BUTTON_MAKE_OP(code, type) (menuOpCode_##code | menuOpType_##type)

extern menu_keyOp_t menu_keyOpBuff;



/**
 * @brief 菜单状态标志位枚举变量。
 */
enum menu_status_t
{
    //menu_data_valid = menu_dataValid_flag << menu_dataValid_mask, /// 菜单状态标志

    menu_error_fatalError = 1 << 23,        ///> 关键故障标志位。
    menu_warning_itemLost = 1 << 22,        ///> 数据缺失标志位。读取数据时发现数据缺失时置位，须手动清除。该标志位使用16位参数，表示数据缺失的总个数。
    menu_noUse2 = 1 << 21,                  ///> cm_backtrace错误打印标志位。发生cmb打印时设置，须手动清除。该标志位使用16位参数，表示数据的大小（字节数）。
    //menu_message_strBufOverride = 1 << 20,  ///> 字符串缓存超控标志位。该标志位置位时，菜单顶层逻辑将忽略字符串缓存，直接打印全缓存。进行一帧打印后自动清除。
    menu_message_buttonOp = 1 << 19,        ///> 按键操作消息。置位时将进行按键处理，处理完成后自动清除，并自动产生屏幕打印消息。
    menu_message_printDisp = 1 << 18,       ///> 屏幕打印消息。置位时将进行屏幕打印，处理完成后自动清除。
    menu_noUse6 = 1 << 17,
    menu_noUse7 = 1 << 16,

    menu_param_Mask16 = 0xffff, ///> 低16位全16位掩码标志位，用于读取低16位参数。如果传一个参数且取值大于255，应使用16位参数。
    menu_param_Mask8h = 0xff00, ///> 低16位中高8位掩码标志位，用于读取16位参数中的高8位。如果要返回两个参数，或参数取值小于256，可以使用两个8位参数。
    menu_param_Mask8l = 0x00ff, ///> 低16位中高8位掩码标志位，用于读取16位参数中的低8位。同上。
};

/**
 * @brief : 菜单项和菜单列表的计数器。
 * @ {
 */
extern uint32_t menu_itemCnt;               ///< 菜单项计数器
extern uint32_t menu_listCnt;               ///< 菜单列表计数器
/**
 * @ }
 */

/**
 * @brief : 菜单状态机。
 * @ {
 */
extern menu_list_t *menu_currList;          ///< 状态变量：指向当前所在的菜单列表
extern menu_itemIfce_t *menu_currItem;      ///< 状态变量：指向当前所在的菜单项，仅位于菜单项
extern menu_list_t *menu_menuRoot;          ///< 根菜单指针。
extern int32_t menu_currRegionNum[3];    ///< 当前局部存储区号
extern int32_t menu_statusFlag;             ///< 状态标志位
extern uint32_t menu_nvm_statusFlagAddr;    ///< 存储状态标志位的NVM存储地址
/**
 * @ }
 */

/**
 * @brief 设置MENU状态标志位
 */
static inline void MENU_StatusFlagSet(uint32_t _mask)
{
    menu_statusFlag |= _mask;
}

/**
 * @brief 清除MENU状态标志位
 */
static inline void MENU_StatusFlagClr(uint32_t _mask)
{
    menu_statusFlag &= (~_mask);
}

/**
 * @brief : 打印屏幕。
 * 该函数是整个菜单的屏幕打印入口，将自动根据菜单的状态打印屏幕。
 *
 * @param  {void} undefined :
 */
void MENU_PrintDisp(void);

/**
 * @brief : 响应按键操作。
 * 该函数是整个菜单的按键响应入口，将自动根据菜单的状态调用按键响应函数。
 * 该函数不会更新屏幕显示。
 *
 * @param  {void} undefined :
 */
void MENU_KeyOp(menu_keyOp_t *const _op);

#endif // ! TEXTMENU_CORE_H
