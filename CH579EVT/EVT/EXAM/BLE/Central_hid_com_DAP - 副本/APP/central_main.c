/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.1
* Date               : 2019/11/05
* Description        : 
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "CONFIG.h"
#include "CH57x_common.h"
#include "hal.h"
#include "central.h"
#include "DAP.h"
#include "usbd_desc.h"
#include "cmsis_os2.h"
/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__align(4) u32 MEM_BUF[BLE_MEMHEAP_SIZE/4];

#if (defined (BLE_MAC)) && (BLE_MAC == TRUE)
u8C MacAddr[6] = {0x84,0xC2,0xE4,0x03,0x02,0x02};
#endif

osThreadId_t DAP_ThreadId;
extern __NO_RETURN void DAP_Thread (void *argument);
/*******************************************************************************
* Function Name  : main
* Description    : 主函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main( void ) 
{
	SetSysClock( CLK_SOURCE_HSE_32MHz );
  PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );		// 打开PLL
	
//  GPIOA_ModeCfg( GPIO_Pin_All, GPIO_ModeIN_PU );
//  GPIOB_ModeCfg( GPIO_Pin_All&(~(GPIO_Pin_11|GPIO_Pin_10)), GPIO_ModeIN_PU );
#ifdef DEBUG
  GPIOA_SetBits(bTXD1);
  GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
  UART1_DefInit( );
#endif   
  PRINT("%s\n",VER_LIB);

  CH57X_BLEInit( );
	HAL_Init( );
	GAPRole_CentralInit( );
	
	R8_GLOB_CFG_INFO |= RB_CFG_DEBUG_EN;
	
	DAP_Setup();
	USBD_Initialize(0U);                  // USB Device Initialization
  USBD_Connect(0U);                     // USB Device Connect
	USBD_HID0_Initialize();
	DelayMs(200U);
	USB_Init();
	DAP_Task_Init();
	
	
	
//	Central_Init( ); 
	while(1){
		
		TMOS_SystemProcess( );
	}
	
}

/******************************** endfile @ main ******************************/
