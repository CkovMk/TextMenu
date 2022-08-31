[TOC]

## API文档

### 顶层API（`TEXTMENU`和`TEXTMENU_CORE`）

- 菜单初始化
  
  ```c
  /** @brief : 菜单全局初始化。
   * 该函数会初始化必要的变量，创建根菜单和其他最基本的菜单列表和菜单项。
   *
   * @param  {void} undefined :
   */
  void MENU_Init(void);
  ```

  将进行初始化菜单运行所必需的变量、初始化NVM（如果启用）相关变量、初始化按钮驱动（如果启用）、添加管理菜单等操作，并调用`void MENU_DataSetUp(void);`函数配置用户菜单。使用菜单前必须首先调用。
  
  

- 设置菜单数据

  ```C
  /**
   * @brief : 初始化各菜单列表和菜单项。
   * 注意：该函数在外部定义，不需初始化最基本的菜单列表和菜单项。该函数禁止用户手动调用。
   *
   * @param  {void} undefined :
   */
  void MENU_DataSetUp(void);
  ```

  在这里配置菜单。本模组内包含了该函数的一个弱符号实现，仅用于菜单测试。您可以创建您自己的实现，并在其中插入您所需的数据、子菜单和函数等项目。

  

- 打印屏幕

  ```C
  /**
   * @brief : 打印屏幕。
   * 该函数是整个菜单的屏幕打印入口，将自动根据菜单的状态打印屏幕。
   *
   * @param  {void} undefined :
   */
  void MENU_PrintDisp(void);
  ```

  用于刷新屏幕。大多数时候屏幕刷新都是自动进行的，您无需手动刷新。

  

- 按键操作

  ```C
  /**
   * @brief : 响应按键操作。
   * 该函数是整个菜单的按键响应入口，将自动根据菜单的状态调用按键响应函数。
   * 该函数不会更新屏幕显示。
   *
   * @param  {void} undefined :
   */
  void MENU_KeyOp(menu_keyOp_t * const _op);
  ```

  用于解析按键操作并根据菜单逻辑给出响应。如果执行按键操作后需要刷新屏幕，其将自动产生一次屏幕刷新事件，但可能不会立即进行屏幕刷新。在使用本模组自带的按键处理函数时，您无需手动调用该函数。反之，如果您自行编写了按键识别代码，您可以通过调用此函数向菜单传输指令。

  

- 路径查询

  ```c
  /**
   * @brief : 根据字符串路径查找菜单列表。
   *
   * @param str : 字符串路径，以"/"分隔。注意：路径名应与跳转类型的菜单项
   *              名称一致，而不是与菜单列表名称一致。事实上，建议将菜单列表
   *              的名称与跳转类型菜单项的名称设为相同。
   * @retval 返回找到的菜单列表的指针。如果未找到则返回nullptr。
   */
  menu_list_t *MENU_DirGetList(const char *str);

  /**
   * @brief : 在菜单列表中搜索给定名称的菜单项。
   *
   * @param dir : 菜单项所在的菜单列表指针。
   * @param str : 菜单项名称字符串。
   * @retval 返回找到的菜单列表的指针。如果未找到则返回nullptr。
   */
  menu_itemIfce_t *MENU_DirGetItem(const menu_list_t *dir, const char *str);
  ```

- 菜单挂起与恢复

	```c++
	/**
	 * @brief 挂起菜单。这将暂时禁用菜单的事件处理，包括按键输入和屏幕打印。
	 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
	 */
	void MENU_Suspend(void);
	
	/**
	 * @brief 恢复菜单。这将启用菜单的事件处理，包括按键输入和屏幕打印。
	 *          注意：调用多次MENU_Suspend()将需要调用相同次数的MENU_Resume()才能将菜单恢复至运行状态。
	 */
	void MENU_Resume(void);
	```
	
	调用这些函数以禁用和恢复菜单运行。
	
- 服务函数

    ```c++
    /**
     * @brief : 菜单事件处理函数。
     *      在事件处理任务中调用此函数。
     */
    void MENU_EventService(void);
    ```

  



### 菜单项（`TEXTMENU_ITEM`）

菜单项是菜单管理的最小单元。

- 构造菜单项

	```c++
	/**
	 * @brief : 菜单项结构体的构造函数。
	 * 该函数分配一块内存，根据传入的参数填入内容，然后返回所产生的菜单项结构体指针。
	 *
	 * @param  {menu_itemAdapter_t} _type : 通过菜单项类型的适配器，确定要生成的菜单项类型。
	 * @param  {void*} _data           : 指向要操作的数据的指针。
	 * @param  {char*} _nameStr        : 菜单项名称字符串，不超过最大长度，以'\0'结尾。
	 * @param  {uint32_t} _saveAddr    : 变量要保存到的地址。对非数据类型及属性设置中只读的菜单项无效。
	 * @param  {uint32_t} _pptFlag     : 属性标志。
	 *
	 * @return {menu_itemIfce_t *}     : 返回所产生的菜单项结构体指针。
	 */
	menu_itemIfce_t* MENU_ItemConstruct(menu_itemType_t _type, void *_data,
	        const char *_nameStr, uint32_t _saveAddr, uint32_t _pptFlag);
	```
	
	目前支持的菜单项类型有：分隔线/空类型（`nullType`）、`int32_t`参数型（`variType`）、`float`参数型（`varfType`）、`bool`布尔类型、`uint8_t`按位字节型、菜单跳转型（`menuType`）、运行程序型（`procType`）。
	
	- `nullType`	
	
      _type：&menu_itemAdapter_nullType
      
	  _data：空指针。
	  
      _nameStr：显示在菜单中的名称。该位置将显示一条带有该名称的分隔线。
    
      _saveAddr：对此类菜单项无意义。
    
      _pptFlag：对此类菜单项无意义。
	
	- `variType`
	
      _type：&menu_itemAdapter_variType
      
      _data：指向要操作的int32_t型变量的指针。
      
      _nameStr：显示在菜单列表和二级页面的名称。
      
      _saveAddr：数据保存的地址号。同一数据区内的编号不能重复。
      
      _pptFlag：属性标志。多个互不冲突的标志位用按位或`|`连接。支持的属性标志位有：
	
      - `menuItem_data_global`：该变量存储在全局数据区。具有该属性的变量将在其二级调参页面的右上角显示`*GL`字样。
      
      - `menuItem_data_region`：该变量存储在局部数据区。具有该属性的变量将在其二级调参页面的右上角显示`*RG`字样。
      
	    > 需要保存至NVM的变量应至少设置以上两项其中之一。未设置以上两项中任何一项属性的变量将在其二级调参页面的右上角显示`*NO`字样，且后续所有与NVM操作相关的属性均不生效。
	
      - `menuItem_data_ROFlag`：该变量为只读，无法通过菜单修改。该属性不会影响NVM操作。
	
      - `menuItem_data_NoSave`：该变量默认不保存。管理菜单内名称为的`RegnSel(0-2)`的菜单项就应用了该属性。
	
      - `menuItem_data_NoLoad`：该变量默认不读取。管理菜单内名称为的`RegnSel(0-2)`的菜单项就应用了该属性。
	
      - `menuItem_disp_forceSci`：强制以科学计数法显示。默认为自适应模式，数字位数过多自动切换为科学计数法显示。
	
      - `menuItem_disp_noPreview`：关闭菜单列表内的变量数值显示，这将允许显示更长的菜单项名称。
	
      - `menuItem_dataExt_HasMinMax`：该（变量）具有最小/最大值的扩展属性。最小值存储在`data+1`地址，最大值存储在`data+2`地址。
	
    - `varfType`
    
      _type：&menu_itemAdapter_varfType

      _data：指向要操作的float型变量的指针。
      
      _nameStr：显示在菜单列表和二级页面的名称。
      
      _saveAddr：数据保存的地址号。同一数据区内的编号不能重复。
      
      _pptFlag：属性标志。多个互不冲突标志位用按位或`|`连接。支持的属性标志位与`variType`相同。
	
    - `boolType`
    
      _type：&menu_itemAdapter_boolType
      
	  
	  _data：指向要操作的bool型变量的指针。
	  
	  _nameStr：显示在菜单列表和二级页面的名称。
	  
	  _saveAddr：数据保存的地址号。同一数据区内的编号不能重复。
	  
	  _pptFlag：属性标志。多个互不冲突标志位用按位或`|`连接。支持的属性标志位有
	  
	  - `menuItem_data_global`：同`variType`。
	  
	  - `menuItem_data_region`：同`variType`。
	  
	  - `menuItem_data_ROFlag`：同`variType`。
	  
	    > 读写的布尔类型菜单项，在菜单内将显示为一个开关，真值时显示为`(==O)`，假值时显示为`(X==)`。
	    > 只读的布尔类型菜单项，在菜单内将显示为一个符号，真值时显示为`  >O `，假值时显示为` X<  `。
	  
	  - `menuItem_data_NoSave`：同`variType`。
	  
	  - `menuItem_data_NoLoad`：同`variType`。
	  
	  > 注意：boolType目前没有二级页面，无法在菜单内查看存储区设置，也无法使用`menuItem_disp_noPreview`标志禁用菜单列表页面的数据显示。
	  
	- `byteType`
	
	  _type：&menu_itemAdapter_byteType
	  
	  _data：指向要操作的uint8_t型变量的指针。
	  
	  _nameStr：显示在菜单列表和二级页面的名称。
	  
	  _saveAddr：数据保存的地址号。同一数据区内的编号不能重复。
	  
	  _pptFlag：属性标志。多个互不冲突标志位用按位或`|`连接。支持的属性标志位与`variType`相同。
	
	- `menuType`
	  _type：&menu_itemAdapter_menuType
	  
	  _data：指向要跳转到的菜单列表的指针。
	  
	  _nameStr：显示在菜单列表和二级页面的名称。显示时会自动用方括号`[]`括起来以提高辨识度。
	  
	  _saveAddr：对此类菜单项无意义。
	  
	  _pptFlag：对此类菜单项无意义。
	
	- `procType`
	
	  _type：&menu_itemAdapter_procType
	  
	  _data：指向要运行的程序的函数指针。该函数必须是`void (*)(menu_keyOp_t* cosnt)`类型。
	  
	  _nameStr：显示在菜单列表和二级页面的名称。显示时会自动在左边加冒号`:`以提高辨识度。
	  
	  _saveAddr：对此类菜单项无意义。
	  
	  _pptFlag：属性标志。多个互不冲突标志位用按位或`|`连接。支持的属性标志位有：
	  
	  - `menuItem_proc_runOnce`：该程序仅运行一次就立刻退出。
	  
	  - `menuItem_proc_uiDisplay`：该程序具备打印屏幕的功能。这将暂时关闭菜单在屏幕的显示。
	
	
	
- 析构菜单项

    ```c++
    /**
     * @brief : 菜单项结构体析构函数。
     *
     * @param  {menu_itemIfce_t*} _item : 要析构的菜单项结构体指针。析构后该指针会被自动赋值为NULL。
     */
    void MENU_ItemDestruct(menu_itemIfce_t *_item);
    ```

- 获取菜单项内存放的数据指针

    ```c++
    /**
     * @brief : 获取菜单项内存放的数据指针。
     * TODO : 更新注释
     * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
     *
     * @return {void *}					: 返回数据指针。注意，无论何种类型，都会返回对应的数据指针。
     */
    void MENU_ItemGetData(menu_itemIfce_t *_item, menu_nvmData_t *_data);
    ```

- 设置菜单项内指针指向的数据的值
  
    ```c++
    /**
     * @brief : 设置菜单项内指针指向的数据的值。
     * 注意，该函数对非数据类型菜单和只读数据菜单无效。
     * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针
     * @param  {void*} _data            :
     */
    void MENU_ItemSetData(menu_itemIfce_t *_item, menu_nvmData_t *_data);
    ```

- 在菜单列表中打印一行菜单
  
    ```c++
    //used when in menuList
    
  /**
     * @brief : 在菜单列表中打印一行菜单。
     *
     * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
     * @param  {uint32_t} _slotNum      : 要打印位置（第几行）。对于12864 OLED而言，取值范围为0~7。
     */
    void MENU_ItemPrintSlot(menu_itemIfce_t *_item, uint32_t _slotNum);
  ```

- 在菜单列表中响应直接按键操作
  
    ```c++
    /**
     * @brief : 在菜单列表中响应直接按键操作。
     *
     * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
     * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
     */
    void MENU_ItemDirectKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
    ```

- 打印访问菜单项的二级页面

    ```c++
    /**
     * @brief : 打印访问菜单项的二级页面。
     *
     * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
     */
    void MENU_ItemPrintDisp(menu_itemIfce_t *_item);
    ```

- 访问菜单项的二级页面时响应按键操作
  
    ```c++
    /**
     * @brief : 访问菜单项的二级页面时响应按键操作。
     *
     * @param  {menu_itemIfce_t*} _item : 要访问的菜单项的指针。
     * @param  {menu_keyOp_t*} _op      : 按键操作指针。按键响应后会被清除为空操作。
     */
    void MENU_ItemKeyOp(menu_itemIfce_t *_item, menu_keyOp_t *const _op);
    ```



### 菜单列表（`MENU_LIST`）

菜单列表是菜单项的父级结构，同时也作为”子菜单类型“的菜单项所保存的数据存在。

- 构造菜单列表
	
	```c++
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
	```
	
	
	
- 析构菜单列表
  
    ```c++
    /**
     * @brief : 菜单列表的析构函数。
     *
     * @param  {menu_list_t*} _list : 要析构的菜单列表结构体指针。析构后该指针会被自动赋为NULL。
     */
    void MENU_ListDestruct(menu_list_t *_list);
    ```

- 插入菜单项
  
    ```c++
    /**
     * @brief : 插入一个菜单项。
     *
     * @param  {menu_list_t*} _list     : 要访问的菜单列表的指针。
     * @param  {menu_itemIfce_t*} _item : 要插入的菜单项指针。该指针应为将亡值。
     * @return {status_t}               : 返回操作的结果。正常应返回kStatus_Success。
     */
    status_t MENU_ListInsert(menu_list_t *_list, menu_itemIfce_t *_item);
    ```

- 向显存中打印菜单列表
  
    ```c++
    /**
     * @brief : 打印菜单列表。
     *
     * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
     */
    void MENU_ListPrintDisp(menu_list_t *_list);
    ```

- 菜单列表响应按键操作
  
    ```c++
    /**
     * @brief : 菜单列表中响应按键操作。
     *
     * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
     * @param  {menu_keyOp_t*} _op  : 按键操作指针。按键响应后会被清除为空操作。
     */
    void MENU_ListKeyOp(menu_list_t *_list, menu_keyOp_t *const _op);
    ```



### 菜单项迭代器（`MENU_ITER`）

菜单项迭代器封装了对菜单数据结构的宽度优先搜索，提供了一种简单快捷的遍历菜单中所有菜单项的方式。

菜单项迭代器位于`TEXTMENU_ITER`中，包含以下API：

-  **初始化菜单迭代器。**
	
	```c++
	/**
	 * @brief : 初始化菜单迭代器。
	 */
	void MENU_IteratorSetup(menu_iterator_t *_iter);
	```
- **构造函数。**
	
	```c++
	/**
	 * @brief : 构造函数，为菜单项迭代器指针分配内存并初始化该菜单迭代器。
	 *
	 * @param  {menu_list_t*} _list : 要访问的菜单列表的指针。
	 * @return {menu_iterator_t}    : 返回构造的迭代器指针。失败返回空指针。
	 */
	menu_iterator_t* MENU_IteratorConstruct(void);
	```

- **析构函数。**
	
	```c++
	/**
	 * @brief : 析构函数，释放菜单项迭代器的内存。
	 *
	 * @param  {menu_iterator_t*} _iter : 要析构的迭代器指针。
	 */
	void MENU_IteratorDestruct(menu_iterator_t *_iter);
	```

- **解引用迭代器。获得当前所在菜单列表。**
	
	```c++
	/**
	 * @brief : 解引用迭代器。获得当前所在菜单列表。
	 *
	 * @param  {menu_iterator_t*} _iter : 迭代器指针。
	 */
	menu_list_t* MENU_IteratorDerefList(menu_iterator_t *_iter);
	```

- **解引用迭代器，获得当前菜单项。**
	
	```c++
	/**
	 * @brief : 解引用迭代器，获得当前菜单项。
	 *
	 * @param  {menu_iterator_t*} _iter : 迭代器指针。
	 */
	menu_itemIfce_t* MENU_IteratorDerefItem(menu_iterator_t *_iter);
	```

- **迭代器递增。**
	
	```c++
	/**
	 * @brief : 迭代器递增。
	 *
	 * @param  {menu_iterator_t*} _iter : 迭代器指针。
	 * @return {status_t}               : 成功返回kStatus_Success，如果已到达菜单结尾，则返回kStauts_Fail。
	 */
	status_t MENU_IteratorIncrease(menu_iterator_t *_iter);
	```







### 其他API

- 其他API的说明参见代码注释。





