#include "textmenu_core.h"

#define SYSLOG_TAG  ("MENU.MAIN")
#define SYSLOG_LVL  (TEXTMENU_MAIN_LOG_LVL)
#include <inc_syslog.h>

//menu_t menuInst;
menu_list_t *menu_currList;
menu_itemIfce_t *menu_currItem;
menu_list_t *menu_menuRoot;
menu_list_t *menu_manageList;
int32_t menu_currRegionNum[3] = { 0, 0, TEXTMENU_NVM_REGION_CNT - 1 };
int32_t menu_statusFlag;
uint32_t menu_nvm_statusFlagAddr;



menu_keyOp_t menu_keyOpBuff;

void MENU_PrintDisp(void)
{
    /** 清空缓存，准备打印 */
    for(int r = 0; r < TEXTMENU_DISPLAY_STRBUF_ROW; ++r)
	{
		for(int c = 0; c < TEXTMENU_DISPLAY_STRBUF_COL + 1; ++c)
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
