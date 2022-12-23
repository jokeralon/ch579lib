#include "hal_led.h"

bsp_device_t *hal_led_device_list[HAL_LED_MAX_NUMBER];
hal_led_config_t *local_led_list[HAL_LED_MAX_NUMBER];

static int hal_led_find(const char *device_name)
{
    int index = 0;
    for (; index < HAL_LED_MAX_NUMBER; index++)
    {
        if (local_led_list[index] == NULL)
            continue;
        if (strcmp(local_led_list[index]->name, device_name) == 0)
        {
            return index;
        }
    }

    return -1;
}

int hal_led_open(const char *pathname, int flags)
{
    int i = 0;
    i = hal_led_find(pathname);
    if (i < 0)
        return HAL_LED_FAIL;

    return (*local_led_list[i]).init();
}

int hal_led_write(int fd, void *buf, int count)
{
    hal_led_data_buff_t *led_data = (hal_led_data_buff_t *)buf;
    for (int i = 0; i < HAL_LED_MAX_NUMBER; i++)
    {
        if (local_led_list[i] == NULL)
            continue;
        if (local_led_list[i]->fd == fd)
        {
            return local_led_list[i]->write(led_data->status);
        }
    }
    return HAL_LED_FAIL;
}

int hal_led_read(int fd, void *buf, int count)
{
    hal_led_data_buff_t *led_data = (hal_led_data_buff_t *)buf;
    for (int i = 0; i < HAL_LED_MAX_NUMBER; i++)
    {
        if (local_led_list[i] == NULL)
            continue;
        if (local_led_list[i]->fd == fd)
        {
            return local_led_list[i]->read();
        }
    }
    return HAL_LED_FAIL;
}

int hal_led_close(int fd)
{
    for (int i = 0; i < HAL_LED_MAX_NUMBER; i++)
    {
        if (local_led_list[i] == NULL)
            continue;
        if (local_led_list[i]->fd == fd)
        {
            return (*local_led_list[i]).deinit();
        }
    }

    return HAL_LED_FAIL;
}
int hal_led_init(hal_led_config_t *led_config)
{
    if (led_config == NULL)
        return HAL_LED_FAIL;

    for (int index = 0; index < HAL_LED_MAX_NUMBER; index++)
    {
        if (hal_led_device_list[index] != NULL)
        {
            if (strcmp(local_led_list[index]->name, led_config->name) == 0)
            {
                LOG_ERROR("%s redefinition!\n", led_config->name);
                return HAL_LED_FAIL;
            }
        }
    }
    for (int index = 0; index < HAL_LED_MAX_NUMBER; index++)
    {
        if (hal_led_device_list[index] == NULL)
        {
            local_led_list[index] = led_config;

            hal_led_device_list[index] = malloc(sizeof(bsp_device_t));
            memcpy(hal_led_device_list[index]->name, led_config->name, sizeof(led_config->name));

            hal_led_device_list[index]->open = hal_led_open;
            hal_led_device_list[index]->write = hal_led_write;
            hal_led_device_list[index]->read = hal_led_read;
            hal_led_device_list[index]->close = hal_led_close;

            local_led_list[index]->fd = bsp_device_register(hal_led_device_list[index]);

            if (local_led_list[index]->fd < 0)
                return HAL_LED_FAIL;
            return HAL_LED_OK;
        }
    }
    return HAL_LED_FAIL;
}

