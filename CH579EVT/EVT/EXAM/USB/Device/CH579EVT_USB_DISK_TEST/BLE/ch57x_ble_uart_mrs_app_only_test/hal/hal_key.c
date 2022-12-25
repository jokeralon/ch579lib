#include "hal_key.h"

bsp_device_t *hal_key_device_list[HAL_KEY_MAX_NUMBER];
hal_key_config_t *local_key_list[HAL_KEY_MAX_NUMBER];

static int hal_key_find(const char *device_name)
{
    int index = 0;
    for (; index < HAL_KEY_MAX_NUMBER; index++)
    {
        if (local_key_list[index] == NULL)
            continue;
        if (strcmp(local_key_list[index]->name, device_name) == 0)
        {
            return index;
        }
    }

    return -1;
}

int hal_key_open(const char *pathname, int flags)
{
    int i = 0;
    i = hal_key_find(pathname);
    if (i < 0)
        return HAL_KEY_FAIL;

    return (*local_key_list[i]).init();
}

int hal_key_write(int fd, void *buf, int count)
{
    hal_key_data_buff_t *key_data = (hal_key_data_buff_t *)buf;
    for (int i = 0; i < HAL_KEY_MAX_NUMBER; i++)
    {
        if (local_key_list[i] == NULL)
            continue;
        if (local_key_list[i]->fd == fd)
        {
            return local_key_list[i]->write(key_data->status);
        }
    }
    return HAL_KEY_FAIL;
}

int hal_key_read(int fd, void *buf, int count)
{
    hal_key_data_buff_t *key_data = (hal_key_data_buff_t *)buf;
    for (int i = 0; i < HAL_KEY_MAX_NUMBER; i++)
    {
        if (local_key_list[i] == NULL)
            continue;
        if (local_key_list[i]->fd == fd)
        {
            return local_key_list[i]->read();
        }
    }
    return HAL_KEY_FAIL;
}

int hal_key_close(int fd)
{
    for (int i = 0; i < HAL_KEY_MAX_NUMBER; i++)
    {
        if (local_key_list[i] == NULL)
            continue;
        if (local_key_list[i]->fd == fd)
        {
            return (*local_key_list[i]).deinit();
        }
    }

    return HAL_KEY_FAIL;
}
int hal_key_init(hal_key_config_t *key_config)
{
    if (key_config == NULL)
        return HAL_KEY_FAIL;

    for (int index = 0; index < HAL_KEY_MAX_NUMBER; index++)
    {
        if (hal_key_device_list[index] != NULL)
        {
            if (strcmp(local_key_list[index]->name, key_config->name) == 0)
            {
                LOG_ERROR("%s redefinition!\n", key_config->name);
                return HAL_KEY_FAIL;
            }
        }
    }
    for (int index = 0; index < HAL_KEY_MAX_NUMBER; index++)
    {
        if (hal_key_device_list[index] == NULL)
        {
            local_key_list[index] = key_config;

            hal_key_device_list[index] = malloc(sizeof(bsp_device_t));
            memcpy(hal_key_device_list[index]->name, key_config->name, sizeof(key_config->name));

            hal_key_device_list[index]->open = hal_key_open;
            hal_key_device_list[index]->write = hal_key_write;
            hal_key_device_list[index]->read = hal_key_read;
            hal_key_device_list[index]->close = hal_key_close;

            local_key_list[index]->fd = bsp_device_register(hal_key_device_list[index]);

            if (local_key_list[index]->fd < 0)
                return HAL_KEY_FAIL;
            return HAL_KEY_OK;
        }
    }
    return HAL_KEY_FAIL;
}

