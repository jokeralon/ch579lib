#include "hal_uart.h"

bsp_device_t *hal_uart_device_list[HAL_UART_MAX_NUMBER];
hal_uart_config_t *local_uart_list[HAL_UART_MAX_NUMBER];

static int hal_uart_find(const char *device_name)
{
    int index = 0;
    for (; index < HAL_UART_MAX_NUMBER; index++)
    {
        if (local_uart_list[index] == NULL)
            continue;
        if (strcmp(local_uart_list[index]->name, device_name) == 0)
        {
            return index;
        }
    }

    return -1;
}

int hal_uart_open(const char *pathname, int flags)
{
    int i = 0;
    i = hal_uart_find(pathname);
    if (i < 0)
        return HAL_UART_FAIL;

    return (*local_uart_list[i]).init();
}

int hal_uart_write(int fd, void *buf, int count)
{
    hal_uart_data_buff_t *spi_data = (hal_uart_data_buff_t *)buf;
    for (int i = 0; i < HAL_UART_MAX_NUMBER; i++)
    {
        if (local_uart_list[i] == NULL)
            continue;
        if (local_uart_list[i]->fd == fd)
        {
            return local_uart_list[i]->write(spi_data->buff, spi_data->len);
        }
    }
    return HAL_UART_FAIL;
}

int hal_uart_read(int fd, void *buf, int count)
{
    hal_uart_data_buff_t *spi_data = (hal_uart_data_buff_t *)buf;
    for (int i = 0; i < HAL_UART_MAX_NUMBER; i++)
    {
        if (local_uart_list[i] == NULL)
            continue;
        if (local_uart_list[i]->fd == fd)
        {
            return local_uart_list[i]->read(spi_data->buff, spi_data->len);
        }
    }
    return HAL_UART_FAIL;
}

int hal_uart_close(int fd)
{
    for (int i = 0; i < HAL_UART_MAX_NUMBER; i++)
    {
        if (local_uart_list[i] == NULL)
            continue;
        if (local_uart_list[i]->fd == fd)
        {
            return (*local_uart_list[i]).deinit();
        }
    }

    return HAL_UART_FAIL;
}
int hal_uart_init(hal_uart_config_t *uart_config)
{
    if (uart_config == NULL)
        return HAL_UART_FAIL;

    for (int index = 0; index < HAL_UART_MAX_NUMBER; index++)
    {
        if (hal_uart_device_list[index] != NULL)
        {
            if (strcmp(local_uart_list[index]->name, uart_config->name) == 0)
            {
                return HAL_UART_FAIL;
            }
        }
    }


    for (int index = 0; index < HAL_UART_MAX_NUMBER; index++)
    {
        if (hal_uart_device_list[index] == NULL)
        {
            local_uart_list[index] = uart_config;

            hal_uart_device_list[index] = malloc(sizeof(bsp_device_t));
            memcpy(hal_uart_device_list[index]->name, uart_config->name, sizeof(uart_config->name));

            hal_uart_device_list[index]->open =  hal_uart_open;
            hal_uart_device_list[index]->write = hal_uart_write;
            hal_uart_device_list[index]->read =  hal_uart_read;
            hal_uart_device_list[index]->close = hal_uart_close;

            local_uart_list[index]->fd = bsp_device_register(hal_uart_device_list[index]);

            if (local_uart_list[index]->fd < 0)
                return HAL_UART_FAIL;

            return HAL_UART_OK;
        }
    }
    return HAL_UART_FAIL;
}
