#include "app_led.h"

hal_led_config_t led1_config = {
    .name = APP_LED1_DEVICE_NAME,
    .init = APP_LED1_FUNC_INIT,
    .deinit = APP_LED1_FUNC_DEINIT,
    .read = APP_LED1_FUNC_READ,
    .write = APP_LED1_FUNC_WRITE,
    .callback = NULL,
};

hal_device_t *led_green_dev = NULL;
hal_device_t *led_blue_dev = NULL;

int app_led_fd = -1;

int app_led1_init()
{
    int ret = hal_led_init(&led1_config);

    led_green_dev = hal_device_find("led_green");
    HAL_DEV_NULL_CHECK(led_green_dev);
    led_blue_dev = hal_device_find("led_blue");
    HAL_DEV_NULL_CHECK(led_blue_dev);

    HAL_CHECK(hal_device_init(led_green_dev));
    HAL_CHECK(hal_device_init(led_blue_dev));

    HAL_CHECK(hal_device_ctrl(led_green_dev, 0, NULL));
    HAL_CHECK(hal_device_ctrl(led_blue_dev, 0, NULL));


    return -1;
}
