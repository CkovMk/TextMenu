#ifndef TEXTMENU_LIST_H
#define TEXTMENU_LIST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "textmenu_core.h"
#include "textmenu_item.h"

//#include "item/menuType.h"

/*************************************
 ************ 菜单列表结构体 **********
 *************************************/
struct _menu_list
{
    menu_itemIfce_t **menu; /// 菜单项指针的动态数组，用于存放指针。该数组内的指针析构时需要手动释放。
    uint32_t listSize, listNum; /// 当前菜单项指针列表的大小、当前列表内的菜单项数量。
    uint32_t disp_p, slct_p; /// 显示数组下标和选择数组下标。
    char nameStr[MENU_NAME_STR_SIZE]; /// 菜单列表名称字符串。
};

/** 菜单列表结构体 前置定义 */
typedef struct _menu_list menu_list_t;

/**************************************
 ************ 菜单列表操作接口 **********
 **************************************/

/**
 * @brief : 菜单列表的构造函数。
 *
 * @param  {const char*} _nameStr : 菜单列表的名称，显示在页面顶部。
 * @param  {uint32_t}    _size	  : 菜单列表的长度。须预留一位用于存储[Back]返回项。
 * @param  {menu_list_t*} _prev   : 指向该菜单列表的上级菜单列表，仅供返回时使用。
 *
 * @return {menu_list_t *}        : 返回构造的菜单列表结构体指针。
 */
menu_list_t* MENU_ListConstruct(const char *_nameStr, uint32_t _size,
        menu_list_t *_prev);

/**
 * @brief : 菜单列表的析构函数。
 *
 * @param  {menu_list_t*} _list : 要析构的菜单列表结构体指针。析构后该指针会被自动赋为NULL。
 */
void MENU_ListDestruct(menu_list_t *_list);

/**
 * @brief : 插入一个菜单项。
 *
 * @param  {menu_list_t*} _list     : 要访问的菜单列表的指针。
 * @param  {menu_itemIfce_t*} _item : 要插入的菜单项指针。该指针应为将亡值。
 * @return {status_t}               : 返回操作的结果。正常应返回kStatus_Success。
 */
status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item);

/**
 * @brief : 打印菜单列表。
 *
 * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
 */
void MENU_ListPrintDisp(menu_list_t *_list);

/**
 * @brief : 菜单列表中响应按键操作。
 *
 * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
 * @param  {menu_keyOp_t*} _op  : 按键操作指针。按键响应后会被清除为空操作。
 */
void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t *const _op);

#endif // ! TEXTMENU_LIST_H
