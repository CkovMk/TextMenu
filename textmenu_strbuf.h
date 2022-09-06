#ifndef TEXTMENU_STRBUF_H
#define TEXTMENU_STRBUF_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "textmenu_port.h"

/** 字符缓存 */

typedef struct _menu_strBuf
{
    char strbuf[TEXTMENU_DISPLAY_STRBUF_ROW][TEXTMENU_DISPLAY_STRBUF_COL + 1]; // used by '\0'
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
    uint8_t fcolor[TEXTMENU_DISPLAY_STRBUF_ROW][TEXTMENU_DISPLAY_STRBUF_COL];
    uint8_t bcolor[TEXTMENU_DISPLAY_STRBUF_ROW][TEXTMENU_DISPLAY_STRBUF_COL];
#endif // ! TEXTMENU_USE_PALETTE
}menu_strBuf_t;

typedef union _menu_strBufPrintSlotCfg
{
    uint16_t titleIndex, titlePadding;
    uint16_t flagsIndex, flagsPadding;
    uint16_t valueIndex, valuePadding;
}menu_strBufPrintSlotCfg_t;

// index = 65535 means disable this item

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *
 * @param _buf
 */
void MENU_PORT_DisplayOutput(menu_strBuf_t *_buf);

#ifdef __cplusplus
}
#endif

#endif // ! TEXTMENU_STRBUF_H
