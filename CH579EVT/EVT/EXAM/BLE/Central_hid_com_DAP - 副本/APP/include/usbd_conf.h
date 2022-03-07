#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__


/**
  * This header file originally belongs to Keil MDK.
  * I put some defination about USB into this fake file.
  * Zach Lee
  */

//     <o.0..15>Maximum Input Report Size (in bytes) <1-65535>
//     <i>Allocates memory and configures 'len' value for USBD_HID0_GetReport
//     <i>and USBD_HID_GetReportTrigger
#define USBD_HID0_IN_REPORT_MAX_SZ                64U

//     <o.0..15>Maximum Output Report Size (in bytes) <1-65535>
//     <i>Allocates memory and configures 'len' value for USBD_HID0_SetReport
//     <i>when rtype=HID_REPORT_OUTPUT.
#define USBD_HID0_OUT_REPORT_MAX_SZ               64U

void USBD_Initialize(int);
void USBD_Connect(int);
int USBD_Configured(int);
void USBD_HID_GetReportTrigger(int, int, void *, int);

#define HID_REPORT_INPUT            0x81
#define HID_REPORT_OUTPUT           0x91
#define HID_REPORT_FEATURE          0xB1

#define USBD_HID_REQ_EP_CTRL        0x01
#define USBD_HID_REQ_PERIOD_UPDATE  0x02
#define USBD_HID_REQ_EP_INT         0x03



#endif
