/**
  ******************************************************************************
  * @file    usbd_customhid.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_customhid.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CUSTOMHID_H
#define __USB_CUSTOMHID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CUSTOM_HID
  * @brief This file is the Header file for USBD_customhid.c
  * @{
  */


/** @defgroup USBD_CUSTOM_HID_Exported_Defines
  * @{
  */
#define CUSTOM_HID_EPIN_ADDR                 0x81U
#define CUSTOM_HID_EPIN_SIZE                 0x40U  /* Important: Endpoint packetsize should be 64! Zach Lee */

#define CUSTOM_HID_EPOUT_ADDR                0x01U
#define CUSTOM_HID_EPOUT_SIZE                0x40U  /* Important: Endpoint packetsize should be 64! Zach Lee */

#define USB_CUSTOM_HID_CONFIG_DESC_SIZ       41U
#define USB_CUSTOM_HID_DESC_SIZ              9U

#ifndef CUSTOM_HID_HS_BINTERVAL
#define CUSTOM_HID_HS_BINTERVAL            0x01U  /* Speed up the communication via interrupt ep. Zach Lee */
#endif /* CUSTOM_HID_HS_BINTERVAL */

#ifndef CUSTOM_HID_FS_BINTERVAL
#define CUSTOM_HID_FS_BINTERVAL            0x01U  /* Speed up the communication via interrupt ep. Zach Lee */
#endif /* CUSTOM_HID_FS_BINTERVAL */

#ifndef USBD_CUSTOMHID_OUTREPORT_BUF_SIZE
#define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE  0x02U  /* OUTPUT REPORT size was defined in usbd_conf.h. Zach Lee */
#endif /* USBD_CUSTOMHID_OUTREPORT_BUF_SIZE */
#ifndef USBD_CUSTOM_HID_REPORT_DESC_SIZE
#define USBD_CUSTOM_HID_REPORT_DESC_SIZE   163U   /* REPORT DESC size was defined in usbd_conf.h. Zach Lee */
#endif /* USBD_CUSTOM_HID_REPORT_DESC_SIZE */

#define CUSTOM_HID_DESCRIPTOR_TYPE           0x21U
#define CUSTOM_HID_REPORT_DESC               0x22U

#define CUSTOM_HID_REQ_SET_PROTOCOL          0x0BU
#define CUSTOM_HID_REQ_GET_PROTOCOL          0x03U

#define CUSTOM_HID_REQ_SET_IDLE              0x0AU
#define CUSTOM_HID_REQ_GET_IDLE              0x02U

#define CUSTOM_HID_REQ_SET_REPORT            0x09U
#define CUSTOM_HID_REQ_GET_REPORT            0x01U
/**
  * @}
  */



#define USB_EP1_IN_ADDR 										 0x81U
#define USB_EP1_OUT_ADDR										 0x01U
#define USB_EP2_IN_ADDR 										 0x82U
#define USB_EP2_OUT_ADDR										 0x02U
#define USB_EP3_IN_ADDR 										 0x83U
#define USB_EP3_OUT_ADDR										 0x03U
#define USB_EP4_IN_ADDR 										 0x84U
#define USB_EP4_OUT_ADDR										 0x04U

/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef enum
{
  CUSTOM_HID_IDLE = 0U,
  CUSTOM_HID_BUSY,
}
CUSTOM_HID_StateTypeDef;

/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_CUSTOM_HID_SendReport(uint8_t  eport,
                                   uint8_t *report,
                                   uint16_t len);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_CUSTOMHID_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
