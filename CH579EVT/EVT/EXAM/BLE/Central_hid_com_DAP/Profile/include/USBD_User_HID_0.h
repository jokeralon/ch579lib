#ifndef __USBD_USER_HID_0_H
#define __USBD_USER_HID_0_H


// 设备接收函数
uint8_t USBD_HID0_SetReport (uint8_t rtype, uint8_t req, uint8_t rid, const uint8_t *buf, int32_t len);
extern void BlE_Central_SendData(uint8_t *data, uint8_t len);

#endif
