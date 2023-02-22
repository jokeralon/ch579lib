#include "hal_device.h"
#include <string.h>

static hal_device_t *hdev_list = NULL;

static void hal_device_printf()
{
    hal_device_t *cur = hdev_list;
    int i=0;
    while (cur!=NULL)
    {
        LOG_DEBUG("device[%d]:%s\r\n", i,cur->name);
        cur = cur->next;
    }
    
}

// 查找设备是否存在
static int hal_device_is_exists(hal_device_t *dev)
{
    HAL_DEV_NULL_CHECK(dev);
    hal_device_t *cur = hdev_list;
    while (NULL != cur)
    {
        if(strcmp(cur->name, dev->name) == 0)
            return HAL_ERROR;
        cur = cur->next;
    }
    return HAL_OK;
}

// 插入设备
static int hal_device_inster(hal_device_t *dev)
{
    hal_device_t *cur = hdev_list;

    if( hdev_list == NULL )
    {
        hdev_list = dev;
        dev->next = NULL;
        LOG_ERROR("%s\r\n", hdev_list->name);
    }else
    {
        while (cur->next)
        {
            cur = cur->next;
        }
        cur = dev;
        cur->next = NULL;
        LOG_ERROR("%s\r\n", cur->name);
    }

hal_device_printf();
    


    // if(NULL == cur)
    // {
    //     cur = dev;
    //     LOG_ERROR("%s\r\n", cur->name);
    //     cur->next = NULL;
    // }else
    // {
    //     while(NULL != cur)
    //         cur = cur->next;
    //     cur->next = dev;
    //     dev->next = NULL;
    //     LOG_ERROR("%s\r\n", cur->name);
    // }
    return HAL_OK;
}

// 驱动注册
int hal_device_register(hal_device_t *dev)
{
    HAL_DEV_NULL_CHECK(dev);

    LOG_DEBUG("register : %s\r\n", dev->name);

    // if((NULL==dev->name) || (NULL==dev->dops) || (hal_device_is_exists(dev)==HAL_ERROR))
    // {
    //     return HAL_ERROR;
    // }
    return hal_device_inster(dev);
}

hal_device_t *hal_device_find(const char *name)
{
    HAL_DEV_NULL_CHECK(name);
    LOG_DEBUG("find : %s\r\n", name);
    hal_device_t *cur = hdev_list;
    while (NULL != cur)
    {
        LOG_DEBUG("|%s| |%s|\r\n",cur->name, name);
        if(strcmp(cur->name, name) == 0)
        {
            LOG_DEBUG("===\r\n");
            return cur;
        }
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
    驱动初始化
*/
int hal_device_init(hal_device_t *dev)
{
    HAL_DEV_NULL_CHECK(dev);

    if(dev->dops->init)
        return dev->dops->init(dev);

    return HAL_ERROR;
}
/*
    设置驱动属于哪个任务
*/
void hal_device_set_owner(hal_device_t *dev, const void *owner)
{
    dev->owner = (void *)owner;
}


