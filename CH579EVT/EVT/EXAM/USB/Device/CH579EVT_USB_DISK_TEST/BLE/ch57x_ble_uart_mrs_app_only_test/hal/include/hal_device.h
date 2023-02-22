#ifndef __HAL_DEVICE_H__
#define __HAL_DEVICE_H__

#include "bsp_log.h"

#define HAL_TRUE    1
#define HAL_FALSE   0

#define HAL_OK      0
#define HAL_ERROR   -1
#define HAL_NULL    -2

#define HAL_DEV_NULL_CHECK(x) \
{\
    if(NULL==x)\
    {\
        LOG_ERROR("HAL PARAM NULL!\r\n");\
    }\
}

#define HAL_DEV_INIT_CHECK(x) \
{\
    if(x->init_flag != 1)\
    {\
        LOG_ERROR("HAL NOT INIT!\r\n");\
    }\
}

#define HAL_CHECK(x) \
{\
    if(x!= HAL_OK)\
    {\
        LOG_ERROR("HAL ERROR CODE: %d!\r\n", x);\
    }\
}


typedef struct hal_device_ops_s hal_device_ops_t;
typedef struct hal_device_s hal_device_t;

struct hal_device_ops_s 
{
    int (*init) (hal_device_t *dev);
    int (*open) (hal_device_t *dev, int oflag);
    int (*close) (hal_device_t *dev);
    int  (*read)   (hal_device_t *dev, int pos, void *buffer, int size);
    int  (*write)  (hal_device_t *dev, int pos, const void *buffer, int size);
    int  (*control)(hal_device_t *dev, int cmd, void *args);
	int  (*config) (hal_device_t *dev, void *args, void *var);
};

struct hal_device_s
{
    const char *name;
    const struct hal_device_ops_s  *dops;
    void *owner;
    void *argv;
    int init_flag;
    int data;
    struct hal_device_s *next;
};




// 驱动注册
int hal_device_register(hal_device_t *dev);

hal_device_t *hal_device_find(const char *name);


/*
    驱动读
*/
int hal_device_read(hal_device_t *dev,  int pos, void *buffer, int size);
/*
    驱动写
*/
int hal_device_write(hal_device_t *dev, int pos, const void *buffer, int size);
/*
    驱动控制
*/
int hal_device_ctrl(hal_device_t *dev,  int cmd, void *arg);
/*
    驱动配置
*/
int hal_device_cfg(hal_device_t *dev, void *args, void *var);
/*
    驱动初始化
*/
int hal_device_init(hal_device_t *dev);
/*
    设置驱动属于哪个任务
*/
void hal_device_set_owner(hal_device_t *dev, const void *owner);
#endif
