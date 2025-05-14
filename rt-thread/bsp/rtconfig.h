/* RT-Thread config file */

#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

// <<< 使用配置向导进行设置 >>>

//==============================
// 基本配置 Basic Configuration
//==============================

// <o>线程优先级最大值 <8-256>
// <i>默认值：32，优先级数越大，可设置的优先级越多
#define RT_THREAD_PRIORITY_MAX  8

// <o>系统时钟节拍（每秒tick数）
// <i>默认值：1000，表示1ms一个tick
#define RT_TICK_PER_SECOND  1000

// <o>数据访问对齐大小
// <i>默认值：4，通常与CPU字长一致
#define RT_ALIGN_SIZE   4

// <o>对象名称最大长度 <2-16>
// <i>默认值：8，用于线程、信号量等内核对象的命名
#define RT_NAME_MAX    8

// <c1>启用组件初始化机制
#define RT_USING_COMPONENTS_INIT
// </c>

// 启用用户主线程入口（main 函数）
#define RT_USING_USER_MAIN

// <o>主线程栈大小 <1-4086>
// <i>默认值：512
#define RT_MAIN_THREAD_STACK_SIZE     256

//==============================
// 调试配置 Debug Configuration
//==============================

// <c1>启用内核调试功能
// #define RT_DEBUG
// </c>

// <o>组件初始化调试输出 <0-1>
#define RT_DEBUG_INIT 0

// <c1>线程栈溢出检测
// #define RT_USING_OVERFLOW_CHECK
// </c>

//==============================
// Hook钩子函数配置
//==============================

// <c1>启用通用钩子函数机制
// #define RT_USING_HOOK
// </c>

// <c1>启用空闲线程钩子函数
// #define RT_USING_IDLE_HOOK
// </c>

//==============================
// 软件定时器配置 Software Timer
//==============================

// <e>启用软件定时器
#define RT_USING_TIMER_SOFT         0
#if RT_USING_TIMER_SOFT == 0
    #undef RT_USING_TIMER_SOFT
#endif

// <o>定时器线程优先级 <0-31>
#define RT_TIMER_THREAD_PRIO        4

// <o>定时器线程栈大小 <0-8192>
#define RT_TIMER_THREAD_STACK_SIZE  512
// </e>

//==============================
// 进程间通信（IPC）配置
//==============================

// <c1>启用信号量
#define RT_USING_SEMAPHORE
// </c>

// <c1>启用互斥锁
// #define RT_USING_MUTEX
// </c>

// <c1>启用事件机制
// #define RT_USING_EVENT
// </c>

// <c1>启用信号机制
// #define RT_USING_SIGNALS
// </c>

// <c1>启用邮箱机制
#define RT_USING_MAILBOX
// </c>

// <c1>启用消息队列
// #define RT_USING_MESSAGEQUEUE
// </c>

//==============================
// 内存管理配置 Memory Management
//==============================

// <c1>启用内存池管理
// #define RT_USING_MEMPOOL
// </c>

// <c1>启用动态内存管理（小内存算法）
#define RT_USING_HEAP
#define RT_USING_SMALL_MEM
#define RT_USING_SMALL_MEM_AS_HEAP
// </c>

// <c1>使用超小内存配置
// #define RT_USING_TINY_SIZE
// </c>

//==============================
// 控制台配置 Console Configuration
//==============================

// <c1>启用控制台功能
#define RT_USING_CONSOLE
// </c>

// <o>控制台缓冲区大小 <1-1024>
#define RT_CONSOLEBUF_SIZE          256

//==============================
// FinSH Shell配置
//==============================

// <c1>启用FinSH命令行
#define RT_USING_FINSH
#define FINSH_USING_SYMTAB      // 支持Tab补全
#define FINSH_USING_DESCRIPTION // 支持命令描述
// </c>

//==============================
// 设备框架配置
//==============================
// <c1>启用设备框架
// #define RT_USING_DEVICE
// </c>

// <<< 配置结束 >>>

#endif
