#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include <stdio.h>
#include "stdint.h"
#include "CH57x_common.h"
#include "bsp_log.h"
#include "bsp_gpio.h"
#include "hal_device.h"
#include "hal_utils.h"

typedef struct key_init_s
{
    UINT32 key_gpio_pin;
    GPIOModeTypeDef key_gpio_mode;
    void (*key_gpio_set ) ( UINT32 pin, GPIOModeTypeDef mode );
}key_init_t;

typedef struct key_param_s
{
    const char *name;
    UINT32 pin;
    GPIOModeTypeDef mode;
    void (*cfg_gpio_func) ( UINT32 pin, GPIOModeTypeDef mode );
    void (*set_gpio_func) ( UINT32 pin, UINT32 onoff );
    int (*read_gpio_func) ( UINT32 pin );
    hal_device_t dev;
}key_param_t;

#define BSP_KEY_MAX_NUMBER      1

#define BSP_KEY_PORT_GPIOA      0
#define BSP_KEY_PORT_GPIOB      1

#define BSP_KEY1_ENABLE         1



#if BSP_KEY1_ENABLE == 1

    #define BSP_KEY1_PORT           BSP_KEY_PORT_GPIOB
    #define BSP_KEY1_PIN            GPIO_Pin_22

    #if BSP_KEY1_PORT == BSP_KEY_PORT_GPIOA
        #define bsp_key1_init_func(x)           GPIOA_ModeCfg(BSP_KEY1_PIN, x)
        #define bsp_key1_on_func()              GPIOA_ResetBits(BSP_KEY1_PIN)
        #define bsp_key1_off_func()             GPIOA_SetBits(BSP_KEY1_PIN)
        #define bsp_key1_readpin_func()         GPIOA_ReadPortPin(BSP_KEY1_PIN)
    #elif BSP_KEY1_PORT == BSP_KEY_PORT_GPIOB
        #define bsp_key1_init_func(x)           GPIOB_ModeCfg(BSP_KEY1_PIN, x)
        #define bsp_key1_on_func()              GPIOB_ResetBits(BSP_KEY1_PIN)
        #define bsp_key1_off_func()             GPIOB_SetBits(BSP_KEY1_PIN)
        #define bsp_key1_readpin_func()         GPIOB_ReadPortPin(BSP_KEY1_PIN)
    #endif
    int __bsp_key1_init(void);
    int __bsp_key1_deinit(void);
    int __bsp_key1_write(int status);
    int __bsp_key1_read(void);
    #define bsp_key1_init                     __bsp_key1_init
    #define bsp_key1_deinit                   __bsp_key1_deinit
    #define bsp_key1_write                   __bsp_key1_write
    #define bsp_key1_read                     __bsp_key1_read
#else
    #define bsp_key1_init_func(x)
    #define bsp_key1_on_func()        
    #define bsp_key1_off_func()       
    #define bsp_key1_readpin_func()   
    
    #define bsp_key1_init()  
    #define bsp_key1_deinit()
    #define bsp_key1_write(x)
    #define bsp_key1_read()  
#endif



#endif