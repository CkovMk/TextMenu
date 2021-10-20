/** @brief : 跳转类型菜单项句柄。 */
typedef struct
{
    menu_list_t *data;
} menu_item_menuHandle_t;

/** @brief : 跳转类型菜单项操作函数。 */
void MENU_ItemConstruct_menuType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_menuType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemSetData_menuType(menu_itemIfce_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_menuType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_menuType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_menuType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_menuType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);