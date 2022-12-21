#include "hal_device.h"

hal_device_list_t global_device_list;

static int hal_device_find(const char *device_name)
{
    for(int index = 0; index<HAL_DEVICE_LIST_NUMBER;index++ )
    {
        if( global_device_list.dev_list[index] == NULL )
        {
            continue;
        }
        if(strcmp(global_device_list.dev_list[index]->name, device_name) == 0)
        {
            return index;
        }
    }

    return -1;
}

int hal_device_register(hal_device_t *device)
{

    int index = 0;
    for(; index < HAL_DEVICE_LIST_NUMBER; index++)
    {
        if( global_device_list.dev_list[index] == NULL )
        {
            global_device_list.dev_list[index] = device;
            return index;
        }
    }

    return -1;
}

int hal_device_unregister(const char *device_name)
{
    int index = hal_device_find(device_name);
    if( index < 0 )
        return -1;
    
    global_device_list.dev_list[index] = NULL;

    return 0;
}

int hal_device_open(const char * pathname, int flags)
{
    hal_device_t *dev;
    int fd = hal_device_find(pathname);
    dev = global_device_list.dev_list[fd];

    (*dev).open(pathname, flags);

    return fd;
}
int hal_device_write(int fd, void *buf, int count)
{
    hal_device_t *dev;
    dev = global_device_list.dev_list[fd];

    return (*dev).write(fd, buf, count);
}
int hal_device_read(int fd, void *buf, int count)
{
    hal_device_t *dev;
    dev = global_device_list.dev_list[fd];

    return (*dev).read(fd, buf, count);
}
int hal_device_close(int fd)
{
    hal_device_t *dev;
    dev = global_device_list.dev_list[fd];

    return (*dev).close(fd);
}

