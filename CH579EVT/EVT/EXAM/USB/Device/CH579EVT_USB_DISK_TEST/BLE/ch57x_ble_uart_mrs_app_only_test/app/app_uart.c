#include "app_uart.h"

hal_uart_config_t uart0_config = {
    .name = APP_UART_DEVICE_NAME,
    .init = APP_UART_FUNC_INIT,
    .deinit = APP_UART_FUNC_DEINIT,
    .read = APP_UART_FUNC_READ,
    .write = APP_UART_FUNC_WRITE,
    .callback = NULL,
};

int app_uart_fd = -1;

int app_uart_init()
{
    int ret = hal_uart_init(&uart0_config);
    
    if (ret != HAL_UART_OK)
        return -1;

    app_uart_fd = bsp_device_open(APP_UART_DEVICE_NAME, 0);

    if (app_uart_fd >= 0)
    {
        
        LOG_INFO("app flash open ok, fd: %d\r\n", app_uart_fd);

        return 0;
    }

    LOG_ERROR("app flash init error\r\n");

    return -1;
}
