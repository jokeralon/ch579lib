#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__
#include <stdio.h>
#include "stdint.h"
#include "CH57x_common.h"
#include "bsp_log.h"

#define BSP_SPI_MAX_NUMBER  1

#define BSP_SPI0_NUMBER         0

#define BSP_SPI0_ENABLE         1

#if BSP_SPI0_ENABLE == 1

    /********************************* 引脚定义 ************************************
     *    PA3  <===========>  SCS
     *    PA0  <===========>  SCK
     *    PA1  <===========>  DI/MOSI
     *    PA2  <===========>  DO/MISO
     *******************************************************************************/

    int __bsp_spi0_init(void);
    int __bsp_spi0_deinit(void);
    int __bsp_spi0_read(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff);
    int __bsp_spi0_write(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff);
    #define bsp_spi0_init                     __bsp_spi0_init
    #define bsp_spi0_deinit                   __bsp_spi0_deinit
    #define bsp_spi0_write                    __bsp_spi0_write
    #define bsp_spi0_read                     __bsp_spi0_read
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

#endif