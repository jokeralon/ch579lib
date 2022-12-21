#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__
#include <stdio.h>
#include "stdint.h"
#include "CH57x_common.h"
#include "bsp_log.h"


int bsp_spi0_init();

int bsp_spi0_deinit();

int bsp_spi0_read(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff);

int bsp_spi0_write(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff);

#endif