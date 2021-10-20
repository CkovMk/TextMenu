/** @brief : 布尔类型菜单项操作函数。 */
void MENU_ItemConstruct_boolType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemGetData_boolType(menu_itemIfce_t *_item, void *_data);
void MENU_ItemSetData_boolType(menu_itemIfce_t *_item, void *_data);
//used when in menuList
void MENU_ItemPrintSlot_boolType(menu_itemIfce_t *_item, uint32_t _slotNum);
void MENU_ItemDirectKeyOp_boolType(menu_itemIfce_t *_item,
        menu_keyOp_t *const _op);
//used when in menuItem
void MENU_ItemPrintDisp_boolType(menu_itemIfce_t *_item);
void MENU_ItemKeyOp_boolType(menu_itemIfce_t *_item, menu_keyOp_t *const _op);