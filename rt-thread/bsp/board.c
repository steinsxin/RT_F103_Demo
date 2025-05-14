/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>

#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "gpio.h"

// USB接收队列
extern uint8_t rx_buffer[RX_BUFFER_SIZE];
extern volatile uint16_t rx_write_index;
extern volatile uint16_t rx_read_index;

extern int USB_SendData(uint8_t* data, uint16_t len);

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (10*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void SysTick_Handler(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
    extern void SystemClock_Config(void);
    
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();
    /* 
     * 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RT_TICK_PER_SECOND);

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USB_DEVICE_Init();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE
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
#endif

#ifdef RT_USING_FINSH
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
#endif

// CMD Test
void led_toggle(int argc, char** argv) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
MSH_CMD_EXPORT(led_toggle, "Toggle LED on PC13")
