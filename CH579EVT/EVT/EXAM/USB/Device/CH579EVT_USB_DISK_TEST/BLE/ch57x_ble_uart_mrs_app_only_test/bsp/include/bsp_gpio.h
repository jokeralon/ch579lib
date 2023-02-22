#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include <stdio.h>
#include "stdint.h"
#include "CH57x_common.h"

inline static int bsp_gpioa_readpin(UINT32 pin)
{
    return GPIOA_ReadPortPin(pin);
}

inline static int bsp_gpiob_readpin(UINT32 pin)
{
    return GPIOB_ReadPortPin(pin);
}

inline static int bsp_gpioa_cfgpin( UINT32 pin, GPIOModeTypeDef mode )
{
    GPIOA_ModeCfg(pin, mode);
}

inline static int bsp_gpiob_cfgpin( UINT32 pin, GPIOModeTypeDef mode )
{
    GPIOB_ModeCfg(pin, mode);
}
inline static void bsp_gpioa_setpin(UINT32 pin, int onoff)
{
    if( onoff )
        GPIOA_SetBits(pin);
    else
        GPIOA_ResetBits(pin);
}   

inline static void bsp_gpiob_setpin(UINT32 pin, int onoff)
{
    if( onoff )
        GPIOB_SetBits(pin);
    else
        GPIOB_ResetBits(pin);
}   

#endif