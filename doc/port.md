[TOC]

## 移植指南

#### 顶层配置

- MENU组件

  启用/禁用：由宏`CMODULE_USE_APP_MENU`控制。0为禁用。

- 调试输出

  ```c++
  /**
   * @name 调试输出
   * @ {
   */
  
  /*! 核心逻辑 LOG级别定义 */
  #define CMODULE_MENU_MAIN_LOG_LVL    (5U)
  
  /*! 数据存储 LOG级别定义 */
  #define CMODULE_MENU_KVDB_LOG_LVL    (2U)
  
  /*! 按键处理 LOG级别定义 */
  #define CMODULE_MENU_BUTN_LOG_LVL    (2U)
  
  /*! 菜单项目 LOG级别定义 */
  #define CMODULE_MENU_ITEM_LOG_LVL    (3U)
  
  /*! 菜单列表 LOG级别定义 */
  #define CMODULE_MENU_LIST_LOG_LVL    (3U)
  
  /* @ } */
  ```

  值越小，输出内容越少。值为0时将仅输出断言（ASSERT）级别的严重错误。详见SYSLOG模组。

- 根菜单最大容量

  由于本组件中的菜单列表需要在初始化时指定其大小，根菜单的大小由宏`CMODULE_MENU_ROOT_SIZE`决定。

- 事件任务接口

  本组件目前利用未使用的中断服务来模拟事件处理。需要配置的项有：

  ```c++
  /**
   * @brief : 菜单消息调用所使用的中断信息。
   * 可以使用任何当前工程中未使用的中断。注意中断号和中断服务函数
   * 必须对应。优先级不可过高。
   */
  #define CMODULE_MENU_SERVICE_IRQHandler (Reserved85_IRQHandler)	///< 要使用的中断服务函数
  #define CMODULE_MENU_SERVICE_IRQn (Reserved85_IRQn) 				///< 要使用的中断号
  #define CMODULE_MENU_SERVICE_IRQPrio (12u) 						///< 中断优先级，需要设置一个较低的值，以免打断重要任务。
  ```

  随后在`app_menu_port.cpp`中实现此函数。只需在服务函数中清除中断标志位，并调用`void MENU_EventService(void);`即可。

  ```c++
  void MENU_EventService(void);
  
  #ifdef __cplusplus
  extern "C"
  {
  #endif
  
  void CMODULE_MENU_SERVICE_IRQHandler(void)
  {
      NVIC_ClearPendingIRQ(CMODULE_MENU_SERVICE_IRQn);
      MENU_EventService();
  }
  
  #ifdef __cplusplus
  }
  #endif
  ```

  

#### 按键输入接口

- 启用/禁用

  由宏`CMODULE_MENU_USE_BUTTON`控制。如果不使用菜单自带的按键处理，您也可以调用顶层API`void MENU_KeyOp(menu_keyOp_t *const _op);`手动传入按键操作。**菜单自带的按键处理依赖于BUTTON组件。**

- GPIO配置

  由宏`CMODULE_MENU_BUTTON_5DIR_BSP_INIT`配置。该宏定义了一个长度为5的`button_t`结构体数组，该结构体的定义详见BUTTON组件。

  在MENU的移植文件中，每一个结构体仅需修改前两项：第一项为按键所在的GPIO外设，第二项为按键所在的GPIO编号。其他项保持默认即可。

  五个结构体按顺序依次为：确定、向上、向下、向左、向右。

  > 示例：
  >
  > ```c++
  > /** @brief : 菜单使用的五向按键初始化。每组数据前两个是GPIO和Pin，其余数据为0。 */
  > #define CMODULE_MENU_BUTTON_5DIR_BSP_INIT  \
  >  {                                     \
  >      {                                 \
  >          RTEPIN_DIGITAL_BUTTON_OK_GPIO, \
  >          RTEPIN_DIGITAL_BUTTON_OK_PIN,   \
  >          kPORT_InterruptOrDMADisabled, \
  >          0,                            \
  >          BUTTON_STAT_NONE,             \
  >          NULL,                         \
  >      },                                \
  >      {                                 \
  >      	RTEPIN_DIGITAL_BUTTON_UP_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_UP_PIN,   \
  >          kPORT_InterruptOrDMADisabled, \
  >          0,                            \
  >          BUTTON_STAT_NONE,             \
  >          NULL,                         \
  >      },                                \
  >      {                                 \
  >      	RTEPIN_DIGITAL_BUTTON_DN_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_DN_PIN,   \
  >          kPORT_InterruptOrDMADisabled, \
  >          0,                            \
  >          BUTTON_STAT_NONE,             \
  >          NULL,                         \
  >      },                                \
  >      {                                 \
  >      	RTEPIN_DIGITAL_BUTTON_LF_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_LF_PIN,   \
  >          kPORT_InterruptOrDMADisabled, \
  >          0,                            \
  >          BUTTON_STAT_NONE,             \
  >          NULL,                         \
  >      },                                \
  >      {                                 \
  >      	RTEPIN_DIGITAL_BUTTON_RT_GPIO,  \
  > 			RTEPIN_DIGITAL_BUTTON_RT_PIN,   \
  >          kPORT_InterruptOrDMADisabled, \
  >          0,                            \
  >          BUTTON_STAT_NONE,             \
  >          NULL,                         \
  >      },                                \
  >  }
  > ```



#### 屏幕显示接口

- 帧缓存

  启用/禁用：由宏`CMODULE_MENU_USE_FRAME_BUFFER`控制。

##### 帧缓存模式

首先需要在这里包含所需头文件,然后将三个宏函数连接到三个函数适配器。

```c++
#include "drv_disp_ssd1306.hpp"
#include "lib_graphic.hpp"
#define CMODULE_MENU_DISPLAY_BUFFER_CLEAR() (MENU_FrameBufferClear())
#define CMODULE_MENU_DISPLAY_PRINT(row, col, str) (MENU_FrameBufferPrint(row, col, str))
#define CMODULE_MENU_DISPLAY_BUFFER_UPDATE() (MENU_FrameBufferUpdate())
```

随后在`app_menu_port.hpp`内实现这三个函数。

`void MENU_FrameBufferClear(void);`用于清除缓存区。

`void MENU_FrameBufferPrint(uint16_t x, uint16_t y, char *str);`用于向缓存区打印字符串。

`void MENU_FrameBufferUpdate(void);`用于将缓存区上传到屏幕。

##### 无缓存模式

只需要适配宏`CMODULE_MENU_DISPLAY_PRINT`。该接口用于将字符打印至屏幕。

其他两个宏定义`CMODULE_MENU_DISPLAY_BUFFER_CLEAR`和`CMODULE_MENU_DISPLAY_BUFFER_UPDATE`置为`(0)`即可。



#### NVM存储接口

- NVM存储

  启用/禁用：由宏`CMODULE_MENU_USE_NVM`控制。

- NVM接口正在施工中，文档暂缓更新...