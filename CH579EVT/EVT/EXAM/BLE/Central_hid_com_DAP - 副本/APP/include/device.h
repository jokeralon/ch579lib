#ifndef __DEVICE_H
#define __DEVICE_H

#define __STATIC_FORCEINLINE	static __forceinline
#include "stdint.h"
#include "cmsis_compiler.h"


void USBD_Initialize(int n);

void USBD_Connect(int);

void USBD_HID0_Initialize (void);
#endif
