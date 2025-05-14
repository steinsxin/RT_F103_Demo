# RT_F103_Demo
RT-Thread 示例工程，适用于 **STM32F103C8T6**开发板

## 📁 项目结构说明

此工程基于 **RT-Thread Nano** 嵌入式实时操作系统，演示了基本线程创建、LED 控制及 FinSH 命令行交互功能。适合初学者入门 RT-Thread 和 STM32 开发



## 🛠️ 开发环境要求

- **开发板**：STM32F103C8T6
- **开发环境**：
  - CLion / STM32CubeMX
  - OpenOCD / GCC 工具链
- **操作系统**：RT-Thread Nano 版本



## ⚙️ 启动配置说明

### 1.设置程序入口（startup 文件）

在 `Core/Startup/startup_stm32f103c8tx.s` 中确认入口为 `entry`：

```
bl __libc_init_array  
bl entry  /* 应用程序入口 */  
bx lr  
```

#### **动态内存分配（必选）**

在 `rtconfig.h` 中选择一种堆管理方式：

- ✅ 推荐：通用内存管理  ：

  ```
  #define RT_USING_MEMHEAP_AS_HEAP  
  ```

- 🧩 轻量级：小内存设备：

  ```
  #define RT_USING_SMALL_MEM_AS_HEAP  
  ```

- 🧱 高级：Slab 分配器（固定块） 

  ```
  #define RT_USING_SLAB_AS_HEAP  
  ```

### 2. FinSH 命令行组件集成

#### **启用 FinSH**

在 `rtconfig.h` 中配置：

```
#define RT_USING_CONSOLE  
#define RT_USING_FINSH  
#define FINSH_USING_SYMTAB        /* 支持 Tab 补全 */  
#define FINSH_USING_DESCRIPTION   /* 命令描述功能 */  
```

在 `shell.h` 中配置：

```
#ifndef FINSH_THREAD_PRIORITY
    #define FINSH_THREAD_PRIORITY 7
#endif
```

#### **串口输出/输入实现**

在 `board.c` 中实现以下函数：

- **控制台输出**（需适配硬件）：

  ```
  void rt_hw_console_output(const char *str)
  {
      uint8_t ch[1];
      rt_enter_critical(); // 进入临界区，避免中断打断输出
  
      while (*str)
      {
          if (*str == '\n')
          {
              ch[0] = '\r';
              if (USB_SendData(ch, 1) < 0)    break;
          }
  
          ch[0] = *str++;
          if (USB_SendData(ch, 1) < 0)    break;
      }
  
      rt_exit_critical(); // 退出临界区
  }
  ```

- **FinSH 输入**（需适配串口接收缓冲区）：

  ```
  char rt_hw_console_getchar(void)
  {
      if (rx_read_index != rx_write_index)
      {
          char ch = rx_buffer[rx_read_index++];
          if (rx_read_index >= RX_BUFFER_SIZE)
              rx_read_index = 0;
          return ch;
      }
      else
      {
          return -1; // 无数据
      }
  }
  ```

#### 链接脚本关键配置

在 `.ld` 文件中保留 RT-Thread 自动初始化段（rodata字段后）：

代码编译过程的最后一步链接，链接器会将我们未使用到的函数优化，不加入到最终的可执行文件中。

这里就要用的 ld 文件中的关键字 `KEEP`：

```
/* === RT-Thread auto-sections start === */
  .rtt_sections :
  {
    . = ALIGN(8);
    __fsymtab_start = .;
    KEEP(*(FSymTab))
    __fsymtab_end = .;

    . = ALIGN(8);
    __vsymtab_start = .;
    KEEP(*(VSymTab))
    __vsymtab_end = .;

    . = ALIGN(8);
    __rt_init_start = .;
    KEEP(*(SORT(.rti_fn*)))
    __rt_init_end = .;

    . = ALIGN(8);
    __rt_utest_tc_tab_start = .;
    KEEP(*(UtestTcTab))
    __rt_utest_tc_tab_end = .;

    . = ALIGN(8);
  } >FLASH
```



## 💡 示例代码

### LED 闪烁线程

```
static void led_thread_entry(void* parameter) {  
    while (1) {  
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);  
        rt_thread_delay(500);  // 500ms 延时  
    }  
}  

int main(void) {  
    rt_thread_init(&led_thread, "led", led_thread_entry, RT_NULL,  
                   rt_led_thread_stack, sizeof(rt_led_thread_stack), 3, 20);  
    rt_thread_startup(&led_thread);  
}  
```

### FinSH 自定义命令

```
void led_toggle(int argc, char** argv) {  
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);  
}  
MSH_CMD_EXPORT(led_toggle, "Toggle LED on PC13")
```



## 🧪 注意事项

1. **堆初始化**：确保 `rt_system_heap_init()` 在 `main()` 中调用，否则动态创建线程会失败。
2. **优先级冲突**：FinSH 线程优先级（默认 20）必须 ≤ `RT_THREAD_PRIORITY_MAX`（如设为 8 则需调整 FinSH 优先级至 7）。
3. **符号表优化**：链接脚本必须用 `KEEP` 保留 RT-Thread 的自动初始化段，否则 FinSH 命令无法注册。
