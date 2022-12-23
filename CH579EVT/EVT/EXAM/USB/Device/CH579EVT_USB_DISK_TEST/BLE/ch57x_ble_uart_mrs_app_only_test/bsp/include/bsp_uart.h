#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include <stdio.h>
#include "stdint.h"
#include "CH57x_common.h"

#define BSP_UART_MAX_NUMBER     2


int bsp_uart0_init();

int bsp_uart0_deinit();

int bsp_uart0_read(uint8_t *buff, uint16_t len);

int bsp_uart0_write(uint8_t *buff, uint16_t len);

#endif