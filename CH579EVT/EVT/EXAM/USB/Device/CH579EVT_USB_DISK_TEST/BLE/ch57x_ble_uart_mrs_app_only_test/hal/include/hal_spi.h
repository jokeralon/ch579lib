#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "bsp_spi.h"
#include "bsp_device.h"
#include "bsp_log.h"

#define HAL_SPI_MAX_NUMBER  BSP_SPI_MAX_NUMBER

typedef void (*hal_spi_callback_t)(void *param, uint8_t event);
typedef int (*hal_spi_init_t)(void);
typedef int (*hal_spi_deinit_t)(void);
typedef int (*hal_spi_read_t)(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff);
typedef int (*hal_spi_write_t)(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff);

typedef enum hal_spi_code_e
{
    HAL_SPI_NOT_INIT        = -4,
    HAL_SPI_REINIT_ERROR = -3,
    HAL_SPI_PARAM_ERROR = -2,
    HAL_SPI_FAIL = -1,
    HAL_SPI_OK = 0,
}hal_spi_code_t;

typedef enum hal_spi_event_e
{
    HAL_SPI_EVENT_INIT_OK = 0,
    HAL_SPI_EVENT_INIT_FAIL,
    HAL_SPI_EVENT_WRITE_OK,
    HAL_SPI_EVENT_WRITE_FAIL,
    HAL_SPI_EVENT_READ_OK,
    HAL_SPI_EVENT_READ_FAIL,
}hal_spi_event_t;

typedef struct hal_spi_data_buff_t
{
    uint8_t cmd;
    uint32_t addr;
    uint16_t addr_len;
    uint8_t *buff;
    uint16_t len;
}hal_spi_data_buff_t;

typedef struct hal_spi_config_s
{
    int fd;
    char name[20];
    hal_spi_init_t init;
    hal_spi_deinit_t deinit;
    hal_spi_read_t read;
    hal_spi_write_t write;
    hal_spi_callback_t callback;
}hal_spi_config_t;


int hal_spi_init(hal_spi_config_t *spi_config);

#endif