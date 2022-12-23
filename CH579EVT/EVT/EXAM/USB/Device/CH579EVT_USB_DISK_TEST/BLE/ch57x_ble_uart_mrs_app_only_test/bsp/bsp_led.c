#include "bsp_led.h"

int __bsp_led1_init()
{
    bsp_led1_init_func(GPIO_ModeOut_PP_5mA);
    return 0;
}

int __bsp_led1_deinit()
{
    return 0;
}

int __bsp_led1_write(int status)
{
    if( status == 0 )
    {
        bsp_led1_off_func();
        return 0;
    }else if(status == 1)
    {
        bsp_led1_on_func();
        return 0;
    }
    return -1;
}

int __bsp_led1_read()
{
    return bsp_led1_readpin_func();
}

int __bsp_led2_init()
{
    bsp_led2_init_func(GPIO_ModeOut_PP_5mA);
    return 0;
}

int __bsp_led2_deinit()
{
    return 0;
}

int __bsp_led2_write(int status)
{
    if( status == 0 )
    {
        bsp_led2_off_func();
        return 0;
    }else if(status == 1)
    {
        bsp_led2_on_func();
        return 0;
    }
    return -1;
}

int __bsp_led2_read()
{
    return bsp_led2_readpin_func();
}
