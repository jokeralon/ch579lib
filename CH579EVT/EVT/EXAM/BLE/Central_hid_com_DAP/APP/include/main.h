/********************************** (C) COPYRIGHT *******************************
* File Name          : central.h
* Author             : WCH
* Version            : V1.0
* Date               : 2018/11/12
* Description        : 观察应用主函数及任务系统初始化
*******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
	
#include "central.h"
#include "USBD_User_HID_0.h"
#include "usbd_conf.h"

/*********************************************************************
 * CONSTANTS
 */

#define DAP_WIRELESS										0
#if (DAP_WIRELESS == 1)
#define DAP_WIRELESS_CENTRAL						1
#define DAP_WIRELESS_PERIPHERAL					!DAP_WIRELESS_CENTRAL
#endif


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif 
