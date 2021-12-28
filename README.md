# TextMenu

[TOC]

## 简介

TEXTMENU原隶属于HITSIC_MODULE的应用层，是一个轻量级的字符菜单。该菜单可以支持32位有符号整数型、32位有符号浮点型、布尔类型、uint8_t字节类型变量的监视与调节，具有子菜单功能，且可以插入可调用的函数。每个菜单项的属性可以单独设置。

### 组件

#### 本体组件

TEXTMENU_ITEM：菜单项接口。

TEXTMENU_LIST：菜单列表。

TEXTMENU_CORE：菜单核心逻辑

TEXTMENU_ITER：菜单项迭代器。封装对于整个菜单的宽度优先遍历。

TEXTMENU_KEYOP：按键操作定义。

TEXTMENU_STRBUF：字符缓存定义。

TEXTMENU_KVDB：键值数据库接口。

#### 环境依赖

**此菜单运行所需的资源：**

FIXME：补全这一部分
