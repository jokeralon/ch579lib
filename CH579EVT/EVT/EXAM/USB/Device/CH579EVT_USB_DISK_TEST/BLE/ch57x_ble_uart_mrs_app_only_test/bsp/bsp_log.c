#include "bsp_log.h"

static int bsp_log_level = LEVEL_DEBUG;

int bsp_log_set_level(int level)
{
    bsp_log_level = level;
    return bsp_log_level;
}

int bsp_log_send_error(const char *__format, ...)
{
    if( bsp_log_level >= LEVEL_ERROR )
    {
        va_list arglist;
        	// 打印传入的参数
        va_start(arglist, __format);
        vprintf(__format, arglist);
        va_end(arglist);
    }
    return -1;
}
int bsp_log_send_warn(const char *__format, ...)
{
    if( bsp_log_level >= LEVEL_WARN )
    {
        va_list arglist;
        	// 打印传入的参数
        va_start(arglist, __format);
        vprintf(__format, arglist);
        va_end(arglist);
    }
    return -1;
}
int bsp_log_send_info(const char *__format, ...)
{
    if( bsp_log_level >= LEVEL_INFO )
    {
        va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
        va_start(args, __format); //使args指向可变参数的第一个参数  
        vprintf(__format, args);  //必须用vprintf等带V的  
        va_end(args);       //结束可变参数的获取
    }
    return -1;
}
int bsp_log_send_debug(const char *__format, ...)
{
    if( bsp_log_level >= LEVEL_DEBUG )
    {
        va_list arglist;
        	// 打印传入的参数
        va_start(arglist, __format);
        vprintf(__format, arglist);
        va_end(arglist);
    }
    return -1;
}
