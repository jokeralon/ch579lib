#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include <stdio.h>
#include "stdint.h"
#include "bsp_device.h"
#include "bsp_uart.h"
#include "bsp_log.h"

#define HAL_UART_MAX_NUMBER  BSP_UART_MAX_NUMBER

typedef void (*hal_uart_callback_t)(void *param, uint8_t event);
typedef int  (*hal_uart_init_t)(void);
typedef int  (*hal_uart_deinit_t)(void);
typedef int  (*hal_uart_read_t)(uint8_t *buff, uint16_t len);
typedef int  (*hal_uart_write_t)(uint8_t *buff, uint16_t len);


typedef enum hal_uart_code_e
{
    HAL_UART_NOT_INIT        = -4,
    HAL_UART_REINIT_ERROR = -3,
    HAL_UART_PARAM_ERROR = -2,
    HAL_UART_FAIL = -1,
    HAL_UART_OK = 0,
}hal_uart_code_t;

typedef enum hal_uart_event_e
{
    HAL_UART_EVENT_INIT_OK = 0,
    HAL_UART_EVENT_INIT_FAIL,
    HAL_UART_EVENT_WRITE_OK,
    HAL_UART_EVENT_WRITE_FAIL,
    HAL_UART_EVENT_READ_OK,
    HAL_UART_EVENT_READ_FAIL,
}hal_uart_event_t;

typedef struct hal_uart_data_buff_t
{
    uint8_t *buff;
    uint16_t len;
}hal_uart_data_buff_t;

typedef struct hal_uart_config_s
{
    int fd;
    char name[20];
    hal_uart_init_t init;
    hal_uart_deinit_t deinit;
    hal_uart_read_t read;
    hal_uart_write_t write;
    hal_uart_callback_t callback;
}hal_uart_config_t;

int hal_uart_init(hal_uart_config_t *spi_config);

#endif