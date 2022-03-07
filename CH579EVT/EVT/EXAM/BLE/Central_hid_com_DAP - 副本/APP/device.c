#include "device.h"
#include "cmsis_os2.h"
#include "usbd_customhid.h"

/**
  * All codes listed above are generated with STM32CubeMX.
  * I add some stub routines below to support the DAP module.
  */

/**
  * Replace CMSIS RTOS api
  */
static volatile int osFlags;  /* Use "volatile" to prevent GCC optimizing the code. */

void osKernelInitialize(void)
{
  osFlags = 0;
  return;
}
osThreadId_t osThreadNew(void (*func)(void *), void * n, void * ctx)
{
  (void)n;

  (*func)(ctx);
  return 0;
}
int osThreadFlagsWait(int mask, int b, int c)
{
  (void)b;
	(void)c;

  int ret;

  while((osFlags&mask) == 0)
  {
    ;
  }
  ret = osFlags; osFlags &= ~mask;
  return ret;
}
void osThreadFlagsSet(int tid, int f)
{
  (void)tid;

  osFlags |= f;
  return;
}
/**
  * @brief  I use USB Middleware from STMicro. The func "MX_USB_DEVICE_Init"
  *         is used to initialize the USB device peripheral.
  * @param  n: Unused
  */
void USBD_Initialize(int n)
{
  (void)n;

  /**
    * I inserted the call of USBD_HID0_Initialize/USBD_HID0_Uninitialize
    * into usbd_custom_hid_if.c. Please refer:
    *
    * line 179  @ usbd_custom_hid_if.c
    * line 191  @ usbd_custom_hid_if.c
    */
  /* USBD_HID0_Initialize(); */
  return;
}
void USBD_Connect(int n)
{
  (void)n;

  return;
}




/**
  * @brief  This routine is used to send a REPORT to the host.
  * @param  a: Unused
  * @param  b: Unused
  * @param  report: Data sent to host
  * @param  len: Data length, usually to be "DAP_PACKET_SIZE"
  */
void USBD_HID_GetReportTrigger(int a, int b, void * report, int len)
{
  (void)a;
  (void)b;

  USBD_CUSTOM_HID_SendReport(CUSTOM_HID_EPIN_ADDR, report, len);
  return;
}



