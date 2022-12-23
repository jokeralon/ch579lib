#include "bsp_log.h"

static int bsp_log_level = LEVEL_DEBUG;

int bsp_log_set_level(int level)
{
    bsp_log_level = level;
    return bsp_log_level;
}

int bsp_log_send_write(int level, const char *__format, ...)
{

    va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args, __format); //使args指向可变参数的第一个参数  
    vprintf(__format, args);  //必须用vprintf等带V的  
    va_end(args);       //结束可变参数的获取
}