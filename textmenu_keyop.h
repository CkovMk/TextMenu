#ifndef TEXTMENU_KEYOP_H
#define TEXTMENU_KEYOP_H

#include <stdint.h>
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

#endif // ! TEXTMENU_KEYOP_H
