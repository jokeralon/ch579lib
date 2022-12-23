#include "bsp_device.h"

bsp_device_list_t global_device_list;

static int bsp_device_find(const char *device_name)
{
    for(int index = 0; index<BSP_DEVICE_LIST_NUMBER;index++ )
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

int bsp_device_register(bsp_device_t *device)
{

    int index = 0;
    for(; index < BSP_DEVICE_LIST_NUMBER; index++)
    {
        if( global_device_list.dev_list[index] == NULL )
        {
            global_device_list.dev_list[index] = device;

            return index;
        }
    }

    return -1;
}

int bsp_device_unregister(const char *device_name)
{
    int index = bsp_device_find(device_name);
    if( index < 0 )
        return -1;
    
    global_device_list.dev_list[index] = NULL;

    return 0;
}

int bsp_device_open(const char * pathname, int flags)
{
    bsp_device_t *dev;
    int fd = bsp_device_find(pathname);
    if(fd < 0)
        return fd;
    dev = global_device_list.dev_list[fd];

    (*dev).open(pathname, flags);
    return fd;
}
int bsp_device_write(int fd, void *buf, int count)
{
    bsp_device_t *dev;
    dev = global_device_list.dev_list[fd];

    return (*dev).write(fd, buf, count);
}
int bsp_device_read(int fd, void *buf, int count)
{
    bsp_device_t *dev;
    dev = global_device_list.dev_list[fd];

    return (*dev).read(fd, buf, count);
}
int bsp_device_close(int fd)
{
    bsp_device_t *dev;
    dev = global_device_list.dev_list[fd];

    return (*dev).close(fd);
}

