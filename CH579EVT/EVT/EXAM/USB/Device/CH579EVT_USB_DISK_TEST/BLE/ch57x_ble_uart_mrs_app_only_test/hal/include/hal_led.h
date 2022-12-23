#ifndef __HAL_LED_H__
#define __HAL_LED_H__

#include "bsp_led.h"
#include "bsp_log.h"
#include "bsp_device.h"

#define HAL_LED_MAX_NUMBER  BSP_LED_MAX_NUMBER

typedef void (*hal_led_callback_t)(void *param, uint8_t event);
typedef int (*hal_led_init_t)();
typedef int (*hal_led_deinit_t)();
typedef int (*hal_led_read_t)();
typedef int (*hal_led_write_t)(int status);

typedef enum hal_led_code_e
{
    HAL_LED_NOT_INIT        = -4,
    HAL_LED_REINIT_ERROR = -3,
    HAL_LED_PARAM_ERROR = -2,
    HAL_LED_FAIL = -1,
    HAL_LED_OK = 0,
}hal_led_code_t;

typedef enum hal_led_event_e
{
    HAL_LED_EVENT_INIT_OK = 0,
    HAL_LED_EVENT_INIT_FAIL,
    HAL_LED_EVENT_WRITE_OK,
    HAL_LED_EVENT_WRITE_FAIL,
    HAL_LED_EVENT_READ_OK,
    HAL_LED_EVENT_READ_FAIL,
}hal_led_event_t;

typedef struct hal_led_data_buff_t
{
    uint8_t status;
}hal_led_data_buff_t;

typedef struct hal_led_config_s
{
    int fd;
    char name[20];
    hal_led_init_t init;
    hal_led_deinit_t deinit;
    hal_led_read_t read;
    hal_led_write_t write;
    hal_led_callback_t callback;
}hal_led_config_t;


int hal_led_init(hal_led_config_t *led_config);

#endif