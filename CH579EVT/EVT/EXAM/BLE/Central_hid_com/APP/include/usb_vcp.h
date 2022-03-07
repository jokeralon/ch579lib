#ifndef __USB_VCP_H
#define __USB_VCP_H

#include "CH57x_common.h"

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define IAD_HID_CDC_SIZ_CONFIG_DESC							0x43			// ÃèÊö·û³¤¶È

#define HID_CDC_DATA_SIZE												0x40


void USB_VCP_Init(void);

void USB_VCP_SendByte(UINT8 ch);
int USB_VCP_SendStr(UINT8 *str, int str_len);
void usb_printf(const char *format, ...);
#endif
