#include "bsp_key.h"

static key_param_t key_list[] =
    {
        {"key1", GPIO_Pin_22, GPIO_ModeIN_PU, bsp_gpiob_cfgpin, bsp_gpioa_setpin, bsp_gpiob_readpin, {0}},
};

int __bsp_key1_init()
{
    bsp_key1_init_func(GPIO_ModeIN_PU);
    LOG_DEBUG("key1 init\n");
    return 0;
}

int __bsp_key1_deinit()
{
    return 0;
}

int __bsp_key1_write(int status)
{
    if (status == 0)
    {
        bsp_key1_off_func();
        return 0;
    }
    else if (status == 1)
    {
        bsp_key1_on_func();
        return 0;
    }
    return -1;
}

int __bsp_key1_read()
{
    return bsp_key1_readpin_func();
}

int __bsp_key_init(hal_device_t *dev)
{
    HAL_DEV_NULL_CHECK(dev);
    for (int i = 0; i < array_size(key_list); i++)
    {
        if (strcmp(dev->name, key_list[i].name) == 0)
        {
            key_list[i].dev.init_flag = 1;
            UINT32 key_gpio_pin = key_list[i].pin;
            GPIOModeTypeDef key_gpio_mode = key_list[i].mode;

            key_list[i].cfg_gpio_func(key_gpio_pin, key_gpio_mode);
            return HAL_OK;
        }
    }
    return HAL_ERROR;
}

int __bsp_key_read(hal_device_t *dev, int pos, void *buffer, int size)
{
    HAL_DEV_NULL_CHECK(dev);
    HAL_DEV_INIT_CHECK(dev);
    for (int i = 0; i < array_size(key_list); i++)
    {
        if (strcmp(dev->name, key_list[i].name) == 0)
        {
            UINT32 key_gpio_pin = key_list[i].pin;
            return (key_list[i].read_gpio_func(key_gpio_pin));
        }
    }
    return HAL_ERROR;
}

static hal_device_ops_t dops = {
    .init = __bsp_key_init,
    .read = __bsp_key_read,
};

void bsp_key_register(void)
{
    for (int i = 0; i < array_size(key_list); i++)
    {
        key_list[i].dev.name = key_list[i].name;
        key_list[i].dev.dops = &dops;
        hal_device_register(&key_list[i].dev);
    }
}
