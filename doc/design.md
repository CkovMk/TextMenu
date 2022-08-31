[TOC]

## 设计文档

### 术语与概述

- 菜单：管理菜单项和菜单表的结构。处理菜单的操作、显示、数据。
- 菜单项：对数据进行操作的单元，表现为菜单列表中的元素。可接受操作。
- 菜单项界面：显示一个菜单项的详细内容，供用户对数据进行精细操作的界面。
- 菜单列表：对菜单项进行管理的结构，表现为菜单项组成的列表。可接受操作。
- 菜单列表界面：用于显示多个菜单项，供用户选择和调节的界面。
- 操作：按键等输入设备对菜单的输入。
- 显示槽：一个菜单项在菜单表界面中的显示空间。

### 核心逻辑

#### 菜单项与菜单列表（命令模式）

- 菜单项结构体

  定义如下结构体用于存放一个菜单项所包含的全部信息：

  ```c
  /** @brief : 菜单项接口结构体。 */
  typedef struct _menu_itemIfce_t
  {
      menu_itemType_t type; ///< 此菜单项的类型。
      uint32_t pptFlag; ///< 此菜单项的属性标志位。
      uint32_t list_id, unique_id; ///< 此菜单项在本列表内的序号（从0开始）、全局唯一序号（从0开始）
      uint32_t saveAddr; ///< 此菜单在本区域内的偏移地址。从0开始，以1步进。注意，全局数据区和局部数据区的地址分开来算。
      char nameStr[MENU_NAME_STR_SIZE]; ///< 此菜单项的名称字符串。最大长度为MENU_NAME_STR_SIZE - 1 字节。
      union menu_itemIfce_handle_t ///< 菜单项操作句柄的共用体。使用时根据此菜单项的类型调取对应项访问。
      {
          void *p_void;
          menu_item_nullHandle_t *p_nullType;
          menu_item_variHandle_t *p_variType;
          menu_item_varfHandle_t *p_varfType;
          menu_item_boolHandle_t *p_boolType;
          menu_item_procHandle_t *p_procType;
          menu_item_menuHandle_t *p_menuType;
      } handle;
      const menu_itemAdapter_t* adapter; ///< 指向存放菜单项命令函数指针的结构体。参考C++虚表
  } menu_itemIfce_t;
  ```

  `pptFlag` ：一个标志位枚举的实例，用于设置该菜单项的属性。参考枚举类型 `menu_itemPropety_t` 。

  `handle`：这是一个共用体，其内容根据菜单类型变化。例如，`variType` 的菜单项，`handle` 中有效的指针为 `p_variType` ，依此类推。而 `p_variType` 的类型 `menu_item_variHandle_t` 是 `variType` 的适配器类型，包含该类型所需的目标指针和所有辅助数据。`handle` 中的指针所指向的内存在构造时分配，析构时释放。

- 菜单项适配器

  这里以整数类型为例：

  ```c
  /** @brief : 整数类型菜单项句柄。 */
  typedef struct
  {
      int32_t *data;
      int32_t bData;
      int32_t v,e;
      int32_t cur;
  } menu_item_variHandle_t;
  ```

  该结构体包含了指向被操作变量的指针 `int32_t *data;`、被操作变量的副本 `int32_t bData;`、科学计数法使用的变量 `int32_t v,e;`、调参指针 `int32_t cur;`。这些变量被对应菜单项类型的操作接口访问和使用。

- 菜单项操作接口

  这里以整数类型为例：

  ```c
  /** @brief : 整数类型菜单项操作函数。 */
  const menu_itemAdapter_t menu_itemAdapter_variType =
  {
      .ItemConstruct = MENU_ItemConstruct_variType,
      .ItemGetData = MENU_ItemGetData_variType,
      .ItemSetData = MENU_ItemSetData_variType,
      .ItemPrintSlot = MENU_ItemPrintSlot_variType,
      .ItemDirectKeyOp = MENU_ItemDirectKeyOp_variType,
      .ItemPrintDisp = MENU_ItemPrintDisp_variType,
      .ItemKeyOp = MENU_ItemKeyOp_variType,
  };
  void MENU_ItemConstruct_variType(menu_itemIfce_t *_item, void *_data);
  void MENU_ItemGetData_variType(menu_itemIfce_t *_item, void *_data);
  void MENU_ItemSetData_variType(menu_itemIfce_t *_item, void *_data);
  //used when in menuList
  void MENU_ItemPrintSlot_variType(menu_itemIfce_t *_item, uint32_t _slotNum);
  void MENU_ItemDirectKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
  //used when in menuItem
  void MENU_ItemPrintDisp_variType(menu_itemIfce_t *_item);
  void MENU_ItemKeyOp_variType(menu_itemIfce_t *_item, menu_keyOp_t * const _op);
  ```

  每个菜单类型都应包含上述7个命令函数。这些函数建议完全按照此格式命名。菜单项中的`adapter`将会指向由这些函数的函数指针构成的结构体`menu_itemAdapter_variType`。在调用一个特定的`menuItem_t`的命令函数时，将会根据其中存储的`adapter`调用这些命令函数。

- 菜单表结构体

  引入如下菜单表结构体来管理菜单表：

  ```c
  typedef struct _menu_list_t
  {
      menu_itemIfce_t **menu;         /// 菜单项指针的动态数组，用于存放指针。该数组内的指针析构时需要手动释放。
      uint32_t listSize, listNum;     /// 当前菜单项指针列表的大小、当前列表内的菜单项数量。
      uint32_t disp_p, slct_p;        /// 显示数组下标和选择数组下标。
      char nameStr[MENU_NAME_STR_SIZE]; /// 菜单列表名称字符串。
                                      //_menu_list_t *prev;		    /// 指向上级菜单的指针。根菜单中该指针指向自身。
  } menu_list_t;
  ```

  `menu_itemIfce_t **menu;` ：一个元素类型为 `menu_itemIfce_t*` 的数组。该数组的大小由构造函数指定，并在构造时分配，一旦分配不再更改，直到析构时释放内存。

- 菜单表操作接口

#### 菜单状态变量（状态机）

- 顶层变量

  - 顶层处理变量

    ```c
    /**
     * @brief : 菜单状态机。
     * @ {
     */
    extern menu_list_t *menu_currList;          ///< 状态变量：指向当前所在的菜单列表
    extern menu_itemIfce_t *menu_currItem;      ///< 状态变量：指向当前所在的菜单项，仅位于菜单项
    extern menu_list_t *menu_menuRoot;          ///< 根菜单指针。
    extern int32_t &menu_currRegionNum;         ///< 当前局部存储区号
    extern int32_t menu_statusFlag;             ///< 状态标志位
    extern uint32_t menu_nvm_statusFlagAddr;    ///< 存储状态标志位的NVM存储地址
    extern int32_t &menu_nvmCopySrc, &menu_nvmCopyDst;
    /**
     * @ }
     */
    ```

  - 菜单状态标志位定义

    ```c
    enum menu_status_t
    {
        menu_data_valid = menu_dataValid_flag << menu_dataValid_mask, /// 菜单状态标志
    
        menu_error_fatalError = 1 << 23,        ///> 关键故障标志位。
        menu_warning_itemLost = 1 << 22,        ///> 数据缺失标志位。读取数据时发现数据缺失时置位，须手动清除。该标志位使用16位参数，表示数据缺失的总个数。
        menu_noUse2 = 1 << 21,                  ///> cm_backtrace错误打印标志位。发生cmb打印时设置，须手动清除。该标志位使用16位参数，表示数据的大小（字节数）。
        menu_message_strBufOverride = 1 << 20,  ///> 字符串缓存超控标志位。该标志位置位时，菜单顶层逻辑将忽略字符串缓存，直接打印全缓存。进行一帧打印后自动清除。
        menu_message_buttonOp = 1 << 19,        ///> 按键操作消息。置位时将进行按键处理，处理完成后自动清除，并自动产生屏幕打印消息。
        menu_message_printDisp = 1 << 18,       ///> 屏幕打印消息。置位时将进行屏幕打印，处理完成后自动清除。
        menu_noUse6 = 1 << 17,
        menu_noUse7 = 1 << 16,
    
        menu_param_Mask16 = 0xffff, ///> 低16位全16位掩码标志位，用于读取低16位参数。如果传一个参数且取值大于255，应使用16位参数。
        menu_param_Mask8h = 0xff00, ///> 低16位中高8位掩码标志位，用于读取16位参数中的高8位。如果要返回两个参数，或参数取值小于256，可以使用两个8位参数。
        menu_param_Mask8l = 0x00ff, ///> 低16位中高8位掩码标志位，用于读取16位参数中的低8位。同上。
    };
    ```

    


### 外围组件（责任链模式）

#### 按键输入

- 按键操作定义

  ```c
  typedef enum _menu_keyOpCode_t
  {
      menuOpCode_5wayStick_nl = 0, ///< NULL
      menuOpCode_5wayStick_ok = 1,
      menuOpCode_5wayStick_up,
      menuOpCode_5wayStick_dn,
      menuOpCode_5wayStick_lf,
      menuOpCode_5wayStick_rt,
  } menu_keyOpCode_t;
  
  typedef enum _menu_keyOpType_t
  {
      menuOpType_shrt = 1 << 8,
      menuOpType_long = 2 << 8,
      menuOpType_lrpt = 3 << 8,
  } menu_keyOpType_t;
  
  typedef uint32_t menu_keyOp_t;
  
  extern menu_keyOp_t menu_keyOpBuff;
  ```

  

- 按键消息处理

  根据上述枚举定义，每条按键消息都由两部分组成：位于低八位的按键码和位于高八位的操作码。一旦有按键消息从按键处理模块产生，该按键消息将被写入缓存区 `menu_keyOpBuff`，然后置位菜单中断标志位中的按键消息位，最后置位NVIC中对应的中断有效标志。



#### 屏幕输出

菜单模组的屏幕显示由两级组成：字符缓存和全缓存。大多数情况下，菜单内部逻辑都是刷新字符缓存，仅在最终输出时将字符缓存转换为帧缓存。

##### 字符缓存

```c++
char menu_dispStrBuf[CMODULE_MENU_DISPLAY_STRBUF_ROW][CMODULE_MENU_DISPLAY_STRBUF_COL];
```



##### 全缓存（可选）

该缓存由移植文件提供。主要包括三个接口：

```c++
#define CMODULE_MENU_DISPLAY_BUFFER_CLEAR()			///> 清除缓存
#define CMODULE_MENU_DISPLAY_PRINT(row, col, str)	///> 打印字符
#define CMODULE_MENU_DISPLAY_BUFFER_UPDATE()			///> 上传缓存
```



#### KVDB键值数据库接口

FIXME


- 保存数据

  ```C
  /**
   * @brief : 保存整个菜单到NVM。
   *
   * @param  {int32_t} _region :  所选择的局部存储区。
   */
  void MENU_Data_NvmSave(int32_t _region);
  
  /**
   * @brief : 保存整个菜单到NVM。
   * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
   * 
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op);
  ```

  保存菜单内的数据到NVM（非易失性存储）。仅在启用NVM功能时有效。本模组不会自动保存参数到NVM。在程序中，您应该调用`void MENU_Data_NvmSave(int32_t _region);`来执行保存操作。`int32_t menu_currRegionNum;`是当前菜单选中的局部存储区。您也可以人为指定存储区，取值范围为0 ≤ `_region` ＜ `CMODULE_MENU_NVM_REGION_CNT`。`void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op);`是程序类型菜单项的服务函数，将被自动添加至管理菜单。您可以通过菜单执行数据保存操作。

  

- 读取数据

  ```C
  /**
   * @brief : 从NVM读取整个菜单。
   *
   * @param  {int32_t} _region : 所选择的局部存储区。
   */
  void MENU_Data_NvmRead(int32_t _region);
  
  /**
   * @brief : 从NVM读取整个菜单。
   * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
   *
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op);
  ```

  从NVM（非易失性存储）中读取数据到当前菜单。仅在启用NVM功能时有效。本模组不会自动读取参数到NVM。在程序中，您可以在初始化后立即调用`void MENU_Data_NvmSave(int32_t _region);`来执行读取操作。您可以人为指定存储区，取值范围为0 ≤ `_region` ＜ `CMODULE_MENU_NVM_REGION_CNT`，或先读取NVM中保存的分区号，再执行读取操作。`void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op);`是程序类型菜单项的服务函数，将被自动添加至管理菜单。您可以通过菜单执行数据读取操作。

  **注意：菜单内的数据在NVM中按地址存储，且设置了校验机制。如果修改了菜单结构或菜单名称，可能会导致数据丢失。请在修改菜单结构前备份数据。**

  

- 读写局部存储区设置

  ```c
  /**
   * @brief : 保存当前局部存储区号到NVM。
   * 该数值设置为不自动保存。
   * 
   */
  void MENU_Data_NvmSaveRegionConfig(void);
  
  /**
   * @brief : 保存当前局部存储区号到NVM。
   * 该数值设置为不自动保存。
   *
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op);
  ```

  用于保存局部存储区编号到NVM（非易失性存储）。该变量不会随其他变量一起保存。`void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op);`函数用于通过菜单保存该编号。

  

- 在数据区之间拷贝数据（不可用）

  ```C
  /**
   * @brief : 将一个局部存储区的数据拷贝到另一个局部存储区。
   *
   * @param  {int32_t} _srcRegion : 源存储序号。
   * @param  {int32_t} _dstRegion : 目的存储区序号。
   */
  void MENU_Data_NvmCopy(int32_t _srcRegion, int32_t _dstRegion);
  
  /**
   * @brief : 将一个局部存储区的数据拷贝到另一个局部存储区。
   * 该函数将使用全局变量 menu_nvmCopySrc 和 menu_nvmCopyDst 中存储的值。
   * 
   * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
   */
  void MENU_Data_NvmCopy_Boxed(menu_keyOp_t *const _op);
  ```

- 读取NVM状态标志（不可用）

  ```C
  /**
   * @brief : 读取NVM状态标志。
   *
   * @return {int32_t}        : 返回读取到的状态标志。
   */
  int32_t MENU_GetNvmStatus(void);
  ```

- 设置NVM状态标志（不可用）

  ```C
  /**
   * @brief : 设置NVM状态标志。
   *
   * @param  {int32_t} _status : 要设置的状态标志。
   */
  void MENU_SetNvmStatus(int32_t _status);
  ```

