#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include <stdio.h>
#include "stdint.h"
#include "CH57x_common.h"

#define BSP_LED_MAX_NUMBER      2

#define BSP_LED_PORT_GPIOA      0
#define BSP_LED_PORT_GPIOB      1

#define BSP_LED1_ENABLE         1
#define BSP_LED2_ENABLE         1

#define BSP_LED1_NUMBER         1
#define BSP_LED2_NUMBER         2

#if BSP_LED1_ENABLE == 1

    #define BSP_LED1_PORT           BSP_LED_PORT_GPIOA
    #define BSP_LED1_PIN            GPIO_Pin_6

    #if BSP_LED1_PORT == BSP_LED_PORT_GPIOA
        #define bsp_led1_init_func(x)           GPIOA_ModeCfg(BSP_LED1_PIN, x)
        #define bsp_led1_on_func()              GPIOA_ResetBits(BSP_LED1_PIN)
        #define bsp_led1_off_func()             GPIOA_SetBits(BSP_LED1_PIN)
        #define bsp_led1_readpin_func()         GPIOA_ReadPortPin(BSP_LED1_PIN)
    #elif BSP_LED1_PORT == BSP_LED_PORT_GPIOB
        #define bsp_led1_init_func(x)           GPIOB_ModeCfg(BSP_LED1_PIN, x)
        #define bsp_led1_on_func()              GPIOB_ResetBits(BSP_LED1_PIN)
        #define bsp_led1_off_func()             GPIOB_SetBits(BSP_LED1_PIN)
        #define bsp_led1_readpin_func()         GPIOB_ReadPortPin(BSP_LED1_PIN)
    #endif
    int __bsp_led1_init(void);
    int __bsp_led1_deinit(void);
    int __bsp_led1_write(int status);
    int __bsp_led1_read(void);
    #define bsp_led1_init                     __bsp_led1_init
    #define bsp_led1_deinit                   __bsp_led1_deinit
    #define bsp_led1_write                   __bsp_led1_write
    #define bsp_led1_read                     __bsp_led1_read
#else
    #define bsp_led1_init_func(x)
    #define bsp_led1_on_func()        
    #define bsp_led1_off_func()       
    #define bsp_led1_readpin_func()   
    
    #define bsp_led1_init()  
    #define bsp_led1_deinit()
    #define bsp_led1_write(x)
    #define bsp_led1_read()  
#endif

#if BSP_LED2_ENABLE == 1

    #define BSP_LED2_PORT           BSP_LED_PORT_GPIOB
    #define BSP_LED2_PIN            GPIO_Pin_21

    #if BSP_LED2_PORT == BSP_LED_PORT_GPIOA
        #define bsp_led2_init_func(x)           GPIOA_ModeCfg(BSP_LED2_PIN, x)
        #define bsp_led2_on_func()              GPIOA_ResetBits(BSP_LED2_PIN)
        #define bsp_led2_off_func()             GPIOA_SetBits(BSP_LED2_PIN)
        #define bsp_led2_readpin_func()         GPIOA_ReadPortPin(BSP_LED2_PIN)
    #elif BSP_LED2_PORT == BSP_LED_PORT_GPIOB
        #define bsp_led2_init_func(x)           GPIOB_ModeCfg(BSP_LED2_PIN, x)
        #define bsp_led2_on_func()              GPIOB_ResetBits(BSP_LED2_PIN)
        #define bsp_led2_off_func()             GPIOB_SetBits(BSP_LED2_PIN)
        #define bsp_led2_readpin_func()         GPIOB_ReadPortPin(BSP_LED2_PIN)
    #endif

    int __bsp_led2_init(void);
    int __bsp_led2_deinit(void);
    int __bsp_led2_write(int status);
    int __bsp_led2_read(void);
    #define bsp_led2_init()                     __bsp_led2_init()
    #define bsp_led2_deinit()                   __bsp_led2_deinit()
    #define bsp_led2_write(x)                   __bsp_led2_write(x)
    #define bsp_led2_read()                     __bsp_led2_read()
#else
    #define bsp_led2_init_func(x)
    #define bsp_led2_on()        
    #define bsp_led2_off()       
    #define bsp_led2_readpin()   

    #define bsp_led2_init()  
    #define bsp_led2_deinit()
    #define bsp_led2_write(x)
    #define bsp_led2_read()  
#endif



#endif