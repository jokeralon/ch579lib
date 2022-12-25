#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#include "bsp_key.h"
#include "bsp_log.h"
#include "bsp_device.h"

#define HAL_KEY_MAX_NUMBER  BSP_KEY_MAX_NUMBER

typedef void (*hal_key_callback_t)(void *param, uint8_t event);
typedef int (*hal_key_init_t)();
typedef int (*hal_key_deinit_t)();
typedef int (*hal_key_read_t)();
typedef int (*hal_key_write_t)(int status);

typedef enum hal_key_code_e
{
    HAL_KEY_NOT_INIT        = -4,
    HAL_KEY_REINIT_ERROR = -3,
    HAL_KEY_PARAM_ERROR = -2,
    HAL_KEY_FAIL = -1,
    HAL_KEY_OK = 0,
}hal_key_code_t;

typedef enum hal_key_event_e
{
    HAL_KEY_EVENT_INIT_OK = 0,
    HAL_KEY_EVENT_INIT_FAIL,
    HAL_KEY_EVENT_WRITE_OK,
    HAL_KEY_EVENT_WRITE_FAIL,
    HAL_KEY_EVENT_READ_OK,
    HAL_KEY_EVENT_READ_FAIL,
}hal_key_event_t;

typedef struct hal_key_data_buff_t
{
    uint8_t status;
}hal_key_data_buff_t;

typedef struct hal_key_config_s
{
    int fd;
    char name[20];
    hal_key_init_t init;
    hal_key_deinit_t deinit;
    hal_key_read_t read;
    hal_key_write_t write;
    hal_key_callback_t callback;
}hal_key_config_t;


int hal_key_init(hal_key_config_t *key_config);

#endif