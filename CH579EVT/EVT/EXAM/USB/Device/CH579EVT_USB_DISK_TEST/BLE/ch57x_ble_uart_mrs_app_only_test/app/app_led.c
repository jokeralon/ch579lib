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
    led_blue_dev = hal_device_find("led_blue");

    
    if (ret != HAL_LED_OK)
        return -1;

    app_led_fd = bsp_device_open(APP_LED1_DEVICE_NAME, 0);

    if (app_led_fd >= 0)
    {
        
        LOG_INFO("app flash open ok, fd: %d\n", app_led_fd);

        hal_led_data_buff_t led_data = {
            .status = 1,
        };

        bsp_device_write(app_led_fd, &led_data, 0);

        return 0;
    }

    LOG_ERROR("app flash init error\n");

    return -1;
}
