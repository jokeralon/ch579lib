#include "hal_spi.h"

bsp_device_t *hal_spi_device_list[HAL_SPI_MAX_NUMBER];
hal_spi_config_t *local_spi_list[HAL_SPI_MAX_NUMBER];

static int hal_spi_find(const char *device_name)
{
    int index = 0;
    for (; index < HAL_SPI_MAX_NUMBER; index++)
    {
        if (local_spi_list[index] == NULL)
            continue;
        if (strcmp(local_spi_list[index]->name, device_name) == 0)
        {
            return index;
        }
    }

    return -1;
}

int hal_spi_open(const char *pathname, int flags)
{
    int i = 0;
    i = hal_spi_find(pathname);
    if (i < 0)
        return HAL_SPI_FAIL;

    return (*local_spi_list[i]).init();
}

int hal_spi_write(int fd, void *buf, int count)
{
    hal_spi_data_buff_t *spi_data = (hal_spi_data_buff_t *)buf;
    for (int i = 0; i < HAL_SPI_MAX_NUMBER; i++)
    {
        if (local_spi_list[i] == NULL)
            continue;
        if (local_spi_list[i]->fd == fd)
        {
            return local_spi_list[i]->write(spi_data->cmd, spi_data->addr, spi_data->addr_len, spi_data->len, spi_data->buff);
        }
    }
    return HAL_SPI_FAIL;
}

int hal_spi_read(int fd, void *buf, int count)
{
    hal_spi_data_buff_t *spi_data = (hal_spi_data_buff_t *)buf;
    for (int i = 0; i < HAL_SPI_MAX_NUMBER; i++)
    {
        if (local_spi_list[i] == NULL)
            continue;
        if (local_spi_list[i]->fd == fd)
        {
            return local_spi_list[i]->read(spi_data->cmd, spi_data->addr, spi_data->addr_len, spi_data->len, spi_data->buff);
        }
    }
    return HAL_SPI_FAIL;
}

int hal_spi_close(int fd)
{
    for (int i = 0; i < HAL_SPI_MAX_NUMBER; i++)
    {
        if (local_spi_list[i] == NULL)
            continue;
        if (local_spi_list[i]->fd == fd)
        {
            return (*local_spi_list[i]).deinit();
        }
    }

    return HAL_SPI_FAIL;
}
int hal_spi_init(hal_spi_config_t *spi_config)
{
    if (spi_config == NULL)
        return HAL_SPI_FAIL;

    for (int index = 0; index < HAL_SPI_MAX_NUMBER; index++)
    {
        if (hal_spi_device_list[index] != NULL)
        {
            if (strcmp(local_spi_list[index]->name, spi_config->name) == 0)
            {
                LOG_ERROR("%s redefinition!\n", spi_config->name);
                return HAL_SPI_FAIL;
            }
        }
    }
    for (int index = 0; index < HAL_SPI_MAX_NUMBER; index++)
    {
        if (hal_spi_device_list[index] == NULL)
        {
            local_spi_list[index] = spi_config;

            hal_spi_device_list[index] = malloc(sizeof(bsp_device_t));
            memcpy(hal_spi_device_list[index]->name, spi_config->name, sizeof(spi_config->name));

            hal_spi_device_list[index]->open = hal_spi_open;
            hal_spi_device_list[index]->write = hal_spi_write;
            hal_spi_device_list[index]->read = hal_spi_read;
            hal_spi_device_list[index]->close = hal_spi_close;

            local_spi_list[index]->fd = bsp_device_register(hal_spi_device_list[index]);

            if (local_spi_list[index]->fd < 0)
                return HAL_SPI_FAIL;
            return HAL_SPI_OK;
        }
    }
    return HAL_SPI_FAIL;
}
