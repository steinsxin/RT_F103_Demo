# RT_F103_Demo
RT-Thread 示例工程，适用于 **STM32F103C8T6**开发板

## 📁 项目结构说明

此工程基于 **RT-Thread** 嵌入式实时操作系统，演示了基本的线程创建与 LED 控制。适合用于初学者入门 RT-Thread 和 STM32。



## 🛠️ 开发环境要求

- 开发板：STM32F103C8T6
- 开发环境：Clion / STM32CubeMX / OpenOCD / GCC 工具链
- 操作系统：RT-Thread Nano 版本



## ⚙️ 启动配置说明

### 1.设置程序入口（startup 文件）

在 `Core/Startup/startup_stm32f103c8tx.s` 中，确认以 `entry` 作为应用程序入口：

```
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point. */
    bl entry
    bx lr
.size Reset_Handler, .-Reset_Handler
```

### 2.启用堆实现（内存分配方式）

请在 `rtconfig.h` 中配置 一种堆管理方式，必须启用，否则**动态内存分配将失败！

#### ✅ 推荐配置（一般用途）

```
#define RT_USING_MEMHEAP_AS_HEAP
```

- 使用 `rt_memheap_init()` 初始化。
- 稳定可靠，适用于大多数动态内存管理场景。

#### 🧩 轻量级堆（内存较紧张场景）

```
#define RT_USING_SMALL_MEM_AS_HEAP
```

- 使用 `rt_smem_init()`。
- 占用资源少，但功能相对有限。

#### 🧱 Slab 分配器（固定块大小，较少使用）

```
#define RT_USING_SLAB_AS_HEAP
```

- 使用 `rt_slab_init()`。
- 适用于特定用途，不推荐初学者使用。



## 💡 示例：创建一个 LED 闪烁线程

```
/* 定义线程控制块 */
static struct rt_thread led_thread;

/* 定义线程栈（对齐） */
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led_thread_stack[1024];

/* 线程入口函数 */
static void led_thread_entry(void* parameter)
{
    while (1)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        rt_thread_delay(1000);   // 延时 1000 tick

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        rt_thread_delay(1000);   // 延时 1000 tick
    }
}

/* main 函数中初始化并启动线程 */
int main(void)
{
    rt_thread_init(&led_thread,                 // 线程控制块
                   "led1",                      // 线程名称
                   led_thread_entry,            // 线程入口函数
                   RT_NULL,                     // 参数
                   rt_led_thread_stack,         // 栈起始地址
                   sizeof(rt_led_thread_stack), // 栈大小
                   3,                           // 优先级
                   20);                         // 时间片
    rt_thread_startup(&led_thread);             // 启动线程
}
```

## 🧪 注意事项

- 请确认系统堆初始化已完成，调用如下函数，并传入正确的堆区地址和大小：

```
rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
```

- 如果你使用 `rt_thread_create()` 并返回 `NULL`，通常是因为：
  - 堆未启用或未初始化；
  - 堆空间不足；
  - 堆类型与初始化函数不匹配；
