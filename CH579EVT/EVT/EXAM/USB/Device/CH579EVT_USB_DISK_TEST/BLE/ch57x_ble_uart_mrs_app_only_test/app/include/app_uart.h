#ifndef __APP_UART_H__
#define __APP_UART_H__

#include "bsp_log.h"
#include "bsp_device.h"
#include "hal_uart.h"

#define APP_UART_DEVICE_NAME                "uart0"

#define APP_UART_FUNC_INIT                 bsp_uart0_init
#define APP_UART_FUNC_DEINIT               bsp_uart0_deinit
#define APP_UART_FUNC_READ                 bsp_uart0_read
#define APP_UART_FUNC_WRITE                bsp_uart0_write

int app_uart_init();

#endif