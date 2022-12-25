#include "app_key.h"

hal_key_config_t key1_config = {
    .name = APP_KEY1_DEVICE_NAME,
    .init = APP_KEY1_FUNC_INIT,
    .deinit = APP_KEY1_FUNC_DEINIT,
    .read = APP_KEY1_FUNC_READ,
    .write = APP_KEY1_FUNC_WRITE,
    .callback = NULL,
};

int app_key_fd = -1;

int app_key1_init()
{
    int ret = hal_key_init(&key1_config);
    
    if (ret != HAL_KEY_OK)
        return -1;

    app_key_fd = bsp_device_open(APP_KEY1_DEVICE_NAME, 0);

    if (app_key_fd >= 0)
    {
        
        LOG_INFO("app flash open ok, fd: %d\n", app_key_fd);

        hal_key_data_buff_t key_data = {
            .status = 1,
        };

        bsp_device_write(app_key_fd, &key_data, 0);

        return 0;
    }

    LOG_ERROR("app flash init error\n");

    return -1;
}
