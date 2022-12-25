#include "bsp_key.h"

int __bsp_key1_init()
{
    bsp_key1_init_func(GPIO_ModeIN_PU);
    LOG_DEBUG("key1 init\n");
    return 0;
}

int __bsp_key1_deinit()
{
    return 0;
}

int __bsp_key1_write(int status)
{
    if( status == 0 )
    {
        bsp_key1_off_func();
        return 0;
    }else if(status == 1)
    {
        bsp_key1_on_func();
        return 0;
    }
    return -1;
}

int __bsp_key1_read()
{
    return bsp_key1_readpin_func();
}
