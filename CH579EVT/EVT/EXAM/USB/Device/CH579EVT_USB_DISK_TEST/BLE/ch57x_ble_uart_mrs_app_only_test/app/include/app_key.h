#ifndef __APP_KEY_H__
#define __APP_KEY_H__

#include "bsp_key.h"
#include "bsp_log.h"
#include "bsp_device.h"
#include "hal_key.h"

#define APP_KEY1_DEVICE_NAME                "key1"

#define APP_KEY1_FUNC_INIT                 bsp_key1_init
#define APP_KEY1_FUNC_DEINIT               bsp_key1_deinit
#define APP_KEY1_FUNC_READ                 bsp_key1_read
#define APP_KEY1_FUNC_WRITE                bsp_key1_write

int app_key1_init();

#endif