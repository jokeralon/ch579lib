#include "hal_device.h"
#include <string.h>

hal_device_t *hdev_list = NULL;

// 查找设备是否存在
static int hal_device_is_exists(hal_device_t *dev)
{
    HAL_DEV_NULL_CHECK(dev);
    hal_device_t *cur = hdev_list;
    while (NULL != cur)
    {
        if(stecmp(cur->name, dev->name) == 0)
            return HAL_ERROR;
        cur = cur->next;
    }
    return HAL_OK;
}

// 插入设备
static int hal_device_inster(hal_device_t *dev)
{
    hal_device_t *cur = hdev_list;
    if( NULL == dev )
        return HAL_ERROR;
    if(NULL == cur)
    {
        hdev_list = dev;
        hdev_list->next = NULL;
    }else
    {
        while(NULL != cur)
            cur = cur->next;
        cur->next = dev;
        dev->next = NULL;
    }
    return HAL_OK;
}

// 驱动注册
int hal_device_register(hal_device_t *dev)
{
    HAL_DEV_NULL_CHECK(dev);

    if((NULL==dev->name) || (NULL==dev->dops) || (hal_device_is_exists(dev)==HAL_ERROR))
    {
        return HAL_ERROR;
    }
    return hal_device_inster(dev);
}

hal_device_t *hal_device_find(const char *name)
{
    HAL_DEV_NULL_CHECK(name);
    hal_device_t *cur = hdev_list;
    while (NULL != cur)
    {
        if(strcmp(cur->name, name) == 0)
            return cur;
        cur = cur->next;
    }
    return NULL;
}


/*
    驱动读
*/
int hal_device_read(hal_device_t *dev,  int pos, void *buffer, int size)
{
    HAL_DEV_NULL_CHECK(dev);

    if(dev->dops->read)
        return dev->dops->read(dev, pos, buffer, size);

    return HAL_ERROR;
}
/*
    驱动写
*/
int hal_device_write(hal_device_t *dev, int pos, const void *buffer, int size)
{
    HAL_DEV_NULL_CHECK(dev);
    if(dev->dops->write)
        return dev->dops->write(dev, pos, buffer, size);

    return HAL_ERROR;
}
/*
    驱动控制
*/
int hal_device_ctrl(hal_device_t *dev,  int cmd, void *arg)
{
    HAL_DEV_NULL_CHECK(dev);
    if(dev->dops->control)
        return dev->dops->control(dev, cmd, arg);

    return HAL_ERROR;
}
/*
    驱动配置
*/
int hal_device_cfg(hal_device_t *dev, void *args, void *var)
{
    HAL_DEV_NULL_CHECK(dev);

    if(dev->dops->config)
        return dev->dops->config(dev, args, var);

    return HAL_ERROR;
}
/*
    设置驱动属于哪个任务
*/
void hal_device_set_owner(hal_device_t *dev, const void *owner)
{
    dev->owner = (void *)owner;
}


