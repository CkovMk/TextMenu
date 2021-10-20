/** 字符缓存 */

typedef struct _menu_strBuf
{
    char strbuf[TEXTMENU_DISPLAY_STRBUF_ROW][TEXTMENU_DISPLAY_STRBUF_COL + 1]; // used by '\0'
#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
    uint8_t fcolor[TEXTMENU_DISPLAY_STRBUF_ROW][TEXTMENU_DISPLAY_STRBUF_COL];
    uint8_t bcolor[TEXTMENU_DISPLAY_STRBUF_ROW][TEXTMENU_DISPLAY_STRBUF_COL];
#endif // ! TEXTMENU_USE_PALETTE
}menu_strBuf_t; // FIXME: move this to elsewhere...

extern menu_strBuf_t menu_dispStrBuf;

#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)
extern menu_dispColor_t palette[TEXTMENU_DISPLAY_PALETTE_SIZE];
#endif // ! TEXTMENU_USE_PALETTE