#ifndef __BSP_DEVICE_H__
#define __BSP_DEVICE_H__
#include <stdio.h>
#include <stdint.h>
#include "CH57x_common.h"

#define BSP_DEVICE_LIST_NUMBER   10

typedef int (*dev_open)(const char * pathname, int flags);
typedef int (*dev_read)(int fd, void * buf, int count);
typedef int (*dev_write)(int fd, void * buf, int count);
typedef int (*dev_close)(int fd);

typedef struct bsp_device_s
{
    char name[20];
    dev_open open;
    dev_read read;
    dev_write write;
    dev_close close;
}bsp_device_t;

typedef struct bsp_device_list_s
{
    bsp_device_t *dev_list[BSP_DEVICE_LIST_NUMBER];
}bsp_device_list_t;


int bsp_device_register(bsp_device_t *device);

int bsp_device_unregister(const char *device_name);

int bsp_device_open(const char * pathname, int flags);

int bsp_device_write(int fd, void *buf, int count);

int bsp_device_read(int fd, void *buf, int count);

int bsp_device_close(int fd);

#endif