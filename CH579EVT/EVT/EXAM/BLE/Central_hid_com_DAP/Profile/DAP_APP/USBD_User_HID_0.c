
#include "CONFIG.h"
#include "CH57x_common.h"
#include "hal.h"
#include "DAP_config.h"
#include "DAP.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "osObject.h"

#include "log.h"
#include "main.h"

#if (USBD_HID0_OUT_REPORT_MAX_SZ != DAP_PACKET_SIZE)
#error "USB HID0 Output Report Size must match DAP Packet Size"
#endif
#if (USBD_HID0_IN_REPORT_MAX_SZ != DAP_PACKET_SIZE)
#error "USB HID Input Report Size must match DAP Packet Size"
#endif

extern osThreadId_t DAP_ThreadId;

static volatile uint16_t USB_RequestIndexI; // Request  Index In
static volatile uint16_t USB_RequestIndexO; // Request  Index Out
static volatile uint16_t USB_RequestCountI; // Request  Count In
static volatile uint16_t USB_RequestCountO; // Request  Count Out

static volatile uint16_t USB_ResponseIndexI; // Response Index In
static volatile uint16_t USB_ResponseIndexO; // Response Index Out
static volatile uint16_t USB_ResponseCountI; // Response Count In
static volatile uint16_t USB_ResponseCountO; // Response Count Out
static volatile uint8_t USB_ResponseIdle;    // Response Idle  Flag

static uint8_t USB_Request[DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Request  Buffer
static uint8_t USB_Response[DAP_PACKET_COUNT][DAP_PACKET_SIZE]; // Response Buffer

uint8_t DAP_Task_ID;

// Called during USBD_Initialize to initialize the USB HID class instance.
void USBD_HID0_Initialize(void)
{
  // Initialize variables
  USB_RequestIndexI = 0U;
  USB_RequestIndexO = 0U;
  USB_RequestCountI = 0U;
  USB_RequestCountO = 0U;
  USB_ResponseIndexI = 0U;
  USB_ResponseIndexO = 0U;
  USB_ResponseCountI = 0U;
  USB_ResponseCountO = 0U;
  USB_ResponseIdle = 1U;
}

// \brief Prepare HID Report data to send.
// \param[in]   rtype   report type:
//                - HID_REPORT_INPUT           = input report requested
//                - HID_REPORT_FEATURE         = feature report requested
// \param[in]   req     request type:
//                - USBD_HID_REQ_EP_CTRL       = control endpoint request
//                - USBD_HID_REQ_PERIOD_UPDATE = idle period expiration request
//                - USBD_HID_REQ_EP_INT        = previously sent report on interrupt endpoint request
// \param[in]   rid     report ID (0 if only one report exists).
// \param[out]  buf     buffer containing report data to send.
// \return              number of report data bytes prepared to send or invalid report requested.
//              - value >= 0: number of report data bytes prepared to send
//              - value = -1: invalid report requested
int32_t USBD_HID0_GetReport(uint8_t rtype, uint8_t req, uint8_t rid, uint8_t *buf)
{
  (void)rid;
  switch (rtype)
  {
  case HID_REPORT_INPUT:
    switch (req)
    {
    case USBD_HID_REQ_EP_CTRL:       // Explicit USB Host request via Control OUT Endpoint
    case USBD_HID_REQ_PERIOD_UPDATE: // Periodic USB Host request via Interrupt OUT Endpoint
      break;
    case USBD_HID_REQ_EP_INT: // Called after USBD_HID_GetReportTrigger to signal data obtained.
      LOG_DEBUG("USBD_HID_REQ_EP_INT\r\n");
      if (USB_ResponseCountI != USB_ResponseCountO)
      {
        // Load data from response buffer to be sent back
        memcpy(buf, USB_Response[USB_ResponseIndexO], DAP_PACKET_SIZE);
        USB_ResponseIndexO++;
        if (USB_ResponseIndexO == DAP_PACKET_COUNT)
        {
          USB_ResponseIndexO = 0U;
        }
        USB_ResponseCountO++;
        LOG_DEBUG(" Return USBD_HID0_GetReport\r\n");
        return ((int32_t)DAP_PACKET_SIZE);
      }
      else
      {
        USB_ResponseIdle = 1U;
      }
      break;
    }
    break;
  case HID_REPORT_FEATURE:
    break;
  }
  LOG_DEBUG(" Out USBD_HID0_GetReport\r\n");
  return (0);
}

// \brief Process received HID Report data.
// \param[in]   rtype   report type:
//                - HID_REPORT_OUTPUT    = output report received
//                - HID_REPORT_FEATURE   = feature report received
// \param[in]   req     request type:
//                - USBD_HID_REQ_EP_CTRL = report received on control endpoint
//                - USBD_HID_REQ_EP_INT  = report received on interrupt endpoint
// \param[in]   rid     report ID (0 if only one report exists).
// \param[in]   buf     buffer that receives report data.
// \param[in]   len     length of received report data.
// \return      true    received report data processed.
// \return      false   received report data not processed or request not supported.
uint8_t USBD_HID0_SetReport(uint8_t rtype, uint8_t req, uint8_t rid, const uint8_t *buf, int32_t len)
{
  (void)req;
  (void)rid;
  LOG_DEBUG(" Enter USBD_HID0_SetReport\r\n");

  //				LOG_DEBUG("buf: ");
  //			for(int i=0; i< USBD_CUSTOMHID_OUTREPORT_BUF_SIZE; i++)
  //			{
  //				PRINT(" %02X", buf[i]);
  //			}
  //			PRINT("\r\n");

  switch (rtype)
  {
  case HID_REPORT_OUTPUT:
    LOG_DEBUG("HID_REPORT_OUTPUT\r\n");
    if (len == 0)
    {
      break;
    }
    if (buf[0] == ID_DAP_TransferAbort)
    {
      DAP_TransferAbort = 1U;
      break;
    }
    if ((uint16_t)(USB_RequestCountI - USB_RequestCountO) == DAP_PACKET_COUNT)
    {
      LOG_DEBUG("(USB_RequestCountI - USB_RequestCountO) == DAP_PACKET_COUNT)\r\n");
      osThreadFlagsSet(DAP_ThreadId, 0x80U);
      break; // Discard packet when buffer is full
    }

    // Store received data into request buffer
    memcpy(USB_Request[USB_RequestIndexI], buf, (uint32_t)len);
    USB_RequestIndexI++;

    if (USB_RequestIndexI == DAP_PACKET_COUNT)
    {
      USB_RequestIndexI = 0U;
    }
    USB_RequestCountI++;
    osThreadFlagsSet(DAP_ThreadId, 0x01U);
    break;
  case HID_REPORT_FEATURE:
    break;
  }
  LOG_DEBUG(" Out USBD_HID0_SetReport\r\n");
  return true;
}

/**
 * @brief  This is a bridge routine to connect the USB Middleware and the DAP module.
 *
 *         When a OUTPUT REPORT from the host is got by USB peripheral, a interface func
 *         "CUSTOM_HID_OutEvent_FS" will be invoked by USB Middleware. I did a tiny
 *         modification for the interface func to call USBD_OutEvent. Please refer:
 *
 *         line 99   @ usbd_customhid.h
 *         line 160  @ usbd_custom_hid_if.c
 *         line 206  @ usbd_custom_hid_if.c
 */
void USBD_OutEvent(void)
{
  USBD_HID0_SetReport(HID_REPORT_OUTPUT, 0, 0, NULL, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
}

/**
 * @brief  This is a bredge routine to connect the USB Middleware and the DAP module.
 *
 *         After we use "USBD_HID_GetReportTrigger" to send a INPUT REPORT to the host,
 *         an interrupt will be generated by USB peripheral when the host responses an ACK.
 *         The func "USBD_CUSTOM_HID_DataIn" of usbd_customhid.c will be invoked. I added a
 *         new interface func into the structure "USBD_CUSTOM_HID_ItfTypeDef" and inserted
 *         a func call into "USBD_CUSTOM_HID_DataIn". Then this bridge func "USBD_InEvent"
 *         could be invoked by the USB Middleware. Please refer:
 *
 *         line 101  @ usbd_customhid.h
 *         line 218  @ usbd_custom_hid_if.c
 *         line 634  @ usbd_customhid.c
 */
void USBD_InEvent(void)
{
  int32_t len;

  if ((len = USBD_HID0_GetReport(HID_REPORT_INPUT, USBD_HID_REQ_EP_INT, 0, NULL)) > 0)
  {
    USBD_HID_GetReportTrigger(0, 0, NULL, len);
  }
}

// DAP Thread.
__NO_RETURN void DAP_Thread(void *argument)
{
  uint32_t flags;
  uint32_t n;
  (void)argument;

  for (;;)
  {
    osThreadFlagsWait(0x81U, osFlagsWaitAny, osWaitForever);
    LOG_DEBUG("osThreadFlagsWait Over\r\n");

    // Process pending requests
    while (USB_RequestCountI != USB_RequestCountO)
    {

      // Handle Queue Commands
      n = USB_RequestIndexO;
      while (USB_Request[n][0] == ID_DAP_QueueCommands)
      {
        USB_Request[n][0] = ID_DAP_ExecuteCommands;
        n++;
        if (n == DAP_PACKET_COUNT)
        {
          n = 0U;
        }
        if (n == USB_RequestIndexI)
        {
          flags = osThreadFlagsWait(0x81U, osFlagsWaitAny, osWaitForever);
          if (flags & 0x80U)
          {
            break;
          }
        }
      }

      // Execute DAP Command (process request and prepare response)
      DAP_ExecuteCommand(USB_Request[USB_RequestIndexO], USB_Response[USB_ResponseIndexI]);
      //			LOG_DEBUG( "USB_Response: " );
      //			for(int i=0;i<DAP_PACKET_SIZE;i++)
      //				printf("%02X ", USB_Response[USB_RequestIndexO][i]);
      //			printf("\r\n");

      // Update Request Index and Count
      USB_RequestIndexO++;
      if (USB_RequestIndexO == DAP_PACKET_COUNT)
      {
        USB_RequestIndexO = 0U;
      }
      USB_RequestCountO++;

      // Update Response Index and Count
      USB_ResponseIndexI++;
      if (USB_ResponseIndexI == DAP_PACKET_COUNT)
      {
        USB_ResponseIndexI = 0U;
      }
      USB_ResponseCountI++;

      if (USB_ResponseIdle)
      {
        if (USB_ResponseCountI != USB_ResponseCountO)
        {
          // Load data from response buffer to be sent back
          n = USB_ResponseIndexO++;
          if (USB_ResponseIndexO == DAP_PACKET_COUNT)
          {
            USB_ResponseIndexO = 0U;
          }
          USB_ResponseCountO++;
          USB_ResponseIdle = 0U;
          LOG_DEBUG("USBD_HID_GetReportTrigger\r\n");
          USBD_HID_GetReportTrigger(0U, 0U, USB_Response[n], DAP_PACKET_SIZE);
        }
      }
    }
  }
}

tmosEvents DAP_ProcessEvent(tmosTaskID task_id, tmosEvents events);
void DAP_Task_Init()
{
  DAP_Task_ID = TMOS_ProcessEventRegister(DAP_ProcessEvent);
  tmos_start_task(DAP_Task_ID, 1, 32);
}

tmosEvents DAP_ProcessEvent(tmosTaskID task_id, tmosEvents events)
{
  uint8 *msgPtr;

  if (events & SYS_EVENT_MSG)
  { // 处理HAL层消息，调用tmos_msg_receive读取消息，处理完成后删除消息。
    msgPtr = tmos_msg_receive(task_id);
    if (msgPtr)
    {
      /* De-allocate */
      tmos_msg_deallocate(msgPtr);
    }
    return events ^ SYS_EVENT_MSG;
  }
  if (events & 1)
  {
    while (1)
    {
      DAP_Thread(NULL);
    }
  }
  return 0;
}
