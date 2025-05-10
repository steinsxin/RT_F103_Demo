//
// Created by Steins_Xin on 2025/3/31.
//
#include "USBDevice.h"

//! 目前printf不能使用格式化字符的方法
void USBDevice(){
    printf("Hello Steins_Xin STM32 FS\r\n");
    HAL_Delay(1000);
}

/**
 * USB_SendData
 * @param data
 * @param len
 * @return
 */
int USB_SendData(uint8_t* data, uint16_t len) {
    if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) return -1;

    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
    while (hcdc->TxState != 0) {}

    return (CDC_Transmit_FS(data, len) == USBD_OK) ? 0 : -2;
}

/**
 * printf 重定向
 * @param format
 * @param ...
 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE* f)
#endif


PUTCHAR_PROTOTYPE
{
    USB_SendData((uint8_t*)&ch, 1);
    return ch;
}