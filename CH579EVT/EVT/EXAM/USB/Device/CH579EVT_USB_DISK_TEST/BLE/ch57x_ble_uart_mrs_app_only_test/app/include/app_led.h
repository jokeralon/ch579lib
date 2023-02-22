#ifndef __APP_LED_H__
#define __APP_LED_H__

#include "bsp_led.h"
#include "bsp_log.h"
#include "bsp_device.h"
#include "hal_led.h"
#include "hal_device.h"

#define APP_LED1_DEVICE_NAME                "led1"

#define APP_LED1_FUNC_INIT                 bsp_led1_init
#define APP_LED1_FUNC_DEINIT               bsp_led1_deinit
#define APP_LED1_FUNC_READ                 bsp_led1_read
#define APP_LED1_FUNC_WRITE                bsp_led1_write

int app_led1_init();

#endif