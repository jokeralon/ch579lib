#include "bsp_uart.h"

int bsp_uart0_init()
{
    GPIOB_SetBits(bTXD0);
    GPIOB_ModeCfg(bTXD0, GPIO_ModeOut_PP_5mA);
    GPIOB_SetBits(bRXD0);
    GPIOB_ModeCfg(bRXD0, GPIO_ModeIN_PU);

    UART0_DefInit();
    
    return 0;
}

int bsp_uart0_write(uint8_t *buff, uint16_t len)
{
    UART0_SendString((PUINT8)buff, (UINT16)len);
}

int bsp_uart0_read(uint8_t *buff, uint16_t len)
{
    return UART0_RecvString((PUINT8)buff);
}

int bsp_uart0_deinit()
{
    return 0;
}
