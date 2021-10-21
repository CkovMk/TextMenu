#include "textmenu_core.h"

#include "textmenu_item.h"
#include "textmenu_list.h"

#define SYSLOG_TAG  ("MENU.MAIN")
#define SYSLOG_LVL  (TEXTMENU_MAIN_LOG_LVL)
#include <inc_syslog.h>

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

void MENU_PrintDisp(void)
{
    /** 清空缓存，准备打印 */
    for(uint32_t r = 0U; r < TEXTMENU_DISPLAY_STRBUF_ROW; ++r)
	{
		for(uint32_t c = 0U; c < TEXTMENU_DISPLAY_STRBUF_COL + 1U; ++c)
		{
			menu_dispStrBuf.strbuf[r][c] = ' ';
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
    		menu_dispStrBuf.fcolor[r][c] = TEXTMENU_DISPLAY_PAL_IDX_NORMAL_F;
			menu_dispStrBuf.bcolor[r][c] = TEXTMENU_DISPLAY_PAL_IDX_NORMAL_B;
#endif // ! TEXTMENU_USE_PALETTE
		}
		menu_dispStrBuf.strbuf[r][TEXTMENU_DISPLAY_STRBUF_COL + 1] = '\0';
	}
	/** 根据责任链打印缓存 */
	if (menu_currItem == NULL)
	{
		MENU_ListPrintDisp(menu_currList);
	}
	else
	{
		MENU_ItemPrintDisp(menu_currItem);
	}
	for (uint8_t i = 0; i < TEXTMENU_DISPLAY_STRBUF_ROW; ++i)
	{
		if('\0' != menu_dispStrBuf.strbuf[i][TEXTMENU_DISPLAY_STRBUF_COL])
		{
			SYSLOG_W("Print display: row %d overflow!", i);
	    	menu_dispStrBuf.strbuf[i][TEXTMENU_DISPLAY_STRBUF_COL] = '\0';
		}
	}
	MENU_PORT_DisplayOutput(&menu_dispStrBuf);
	MENU_StatusFlagClr(menu_message_printDisp);
}

void MENU_KeyOp(menu_keyOp_t *const _op)
{
	if (menu_currItem == NULL)
	{
		MENU_ListKeyOp(menu_currList, _op);
	}
	else 
	{
		MENU_ItemKeyOp(menu_currItem, _op);
	}
	if (*_op != 0)
	{
		SYSLOG_W("KeyOp remained unclear. OP = %d", *_op);
	}
	MENU_StatusFlagClr(menu_message_buttonOp);
	MENU_StatusFlagSet(menu_message_printDisp); // FIXME: flag should only be set on display chang.
}
