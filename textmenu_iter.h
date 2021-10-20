/**
 * @brief 菜单项迭代器结构体
 */
typedef struct _menu_iterator_t
{
    menu_list_t **listQue;
    uint32_t listQueSize;
    uint32_t listNum;
    uint32_t itemNum;
}menu_iterator_t;

/**************************************
 ************ 菜单项迭代器 **********
 **************************************/

/**
 * @brief : 初始化菜单迭代器。
 */
void MENU_IteratorSetup(menu_iterator_t *_iter);

/**
 * @brief : 构造函数，为菜单项迭代器指针分配内存并初始化该菜单迭代器。
 *
 * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
 * @return {menu_iterator_t}    : 返回构造的迭代器指针。失败返回空指针。
 */
menu_iterator_t* MENU_IteratorConstruct(void);

/**
 * @brief : 析构函数，释放菜单项迭代器的内存。
 *
 * @param  {menu_iterator_t*} _iter : 要析构的迭代器指针。
 */
void MENU_IteratorDestruct(menu_iterator_t *_iter);

/**
 * @brief : 解引用迭代器。获得当前所在菜单列表。
 *
 * @param  {menu_iterator_t*} _iter : 迭代器指针。
 */
menu_list_t* MENU_IteratorDerefList(menu_iterator_t *_iter);

/**
 * @brief : 解引用迭代器，获得当前菜单项。
 *
 * @param  {menu_iterator_t*} _iter : 迭代器指针。
 */
menu_itemIfce_t* MENU_IteratorDerefItem(menu_iterator_t *_iter);

/**
 * @brief : 迭代器递增。
 *
 * @param  {menu_iterator_t*} _iter : 迭代器指针。
 * @return {status_t}               : 成功返回kStatus_Success，如果已到达菜单结尾，则返回kStauts_Fail。
 */
status_t MENU_IteratorIncrease(menu_iterator_t *_iter);