#include "bsp_led.h"

static led_param_t led_list[] =
    {
        {"led_blue", BSP_LED1_PIN, GPIO_ModeOut_PP_5mA, bsp_gpioa_cfgpin, bsp_gpioa_setpin, bsp_gpioa_readpin, {0}},
        {"led_green", BSP_LED2_PIN, GPIO_ModeOut_PP_5mA, bsp_gpioa_cfgpin, bsp_gpioa_setpin, bsp_gpioa_readpin, {0}},
};

int __bsp_led1_init()
{
    bsp_led1_init_func(GPIO_ModeOut_PP_5mA);
    return 0;
}

int __bsp_led1_deinit()
{
    return 0;
}

int __bsp_led1_write(int status)
{
    if (status == 0)
    {
        bsp_led1_off_func();
        return 0;
    }
    else if (status == 1)
    {
        bsp_led1_on_func();
        return 0;
    }
    return -1;
}

int __bsp_led1_read()
{
    return bsp_led1_readpin_func();
}

int __bsp_led2_init()
{
    bsp_led2_init_func(GPIO_ModeOut_PP_5mA);
    return 0;
}

int __bsp_led2_deinit()
{
    return 0;
}

int __bsp_led2_write(int status)
{
    if (status == 0)
    {
        bsp_led2_off_func();
        return 0;
    }
    else if (status == 1)
    {
        bsp_led2_on_func();
        return 0;
    }
    return -1;
}

int __bsp_led2_read()
{
    return bsp_led2_readpin_func();
}

int __bsp_led_init(hal_device_t *dev)
{
    HAL_DEV_NULL_CHECK(dev);

    for (int i = 0; i < array_size(led_list); i++)
    {
        if (strcmp(dev->name, led_list[i].name) == 0)
        {
            UINT32 key_gpio_pin = led_list[i].pin;
            GPIOModeTypeDef key_gpio_mode = led_list[i].mode;

            led_list[i].cfg_gpio_func(key_gpio_pin, key_gpio_mode);

            return HAL_OK;
        }
    }

    return HAL_ERROR;
}

int __bsp_key_control(hal_device_t *dev, int cmd, void *args)
{
    HAL_DEV_NULL_CHECK(dev);
    HAL_DEV_INIT_CHECK(dev);
    for (int i = 0; i < array_size(led_list); i++)
    {
        if (strcmp(dev->name, led_list[i].name) == 0)
        {
            UINT32 key_gpio_pin = led_list[i].pin;

            led_list[i].set_gpio_func(key_gpio_pin, cmd);

            return HAL_OK;
        }
    }
    return HAL_ERROR;
}

static hal_device_ops_t dops = {
    .init = __bsp_led_init,
    .control = __bsp_key_control,
};

void bsp_led_register(void)
{
    for (int i = 0; i < array_size(led_list); i++)
    {
        led_list[i].dev.name = led_list[i].name;
        led_list[i].dev.dops = &dops;
        hal_device_register(&led_list[i].dev);
    }
}
