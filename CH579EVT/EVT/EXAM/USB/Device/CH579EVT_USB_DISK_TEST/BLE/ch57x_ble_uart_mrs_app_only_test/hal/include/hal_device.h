#ifndef __HAL_DEVICE_H__
#define __HAL_DEVICE_H__
#include <stdio.h>
#include <stdint.h>

#define HAL_DEVICE_LIST_NUMBER   10

typedef int (*dev_open)(const char * pathname, int flags);
typedef int (*dev_read)(int fd, void * buf, int count);
typedef int (*dev_write)(int fd, void * buf, int count);
typedef int (*dev_close)(int fd);

typedef struct hal_device_s
{
    char name[20];
    dev_open open;
    dev_read read;
    dev_write write;
    dev_close close;
}hal_device_t;

typedef struct hal_device_list_s
{
    hal_device_t *dev_list[HAL_DEVICE_LIST_NUMBER];
}hal_device_list_t;


int hal_device_register(hal_device_t *device);

int hal_device_unregister(const char *device_name);

int hal_device_open(const char * pathname, int flags);

int hal_device_write(int fd, void *buf, int count);

int hal_device_read(int fd, void *buf, int count);

int hal_device_close(int fd);

#endif