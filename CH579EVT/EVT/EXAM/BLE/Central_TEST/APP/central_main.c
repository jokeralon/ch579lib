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
#include "usb_vcp.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__align(4) u32 MEM_BUF[BLE_MEMHEAP_SIZE/4];

#if (defined (BLE_MAC)) && (BLE_MAC == TRUE)
u8C MacAddr[6] = {0x84,0xC2,0xE4,0x03,0x02,0x02};
#endif

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
	
  GPIOA_ModeCfg( GPIO_Pin_All, GPIO_ModeIN_PU );
  GPIOB_ModeCfg( GPIO_Pin_All&(~(GPIO_Pin_11|GPIO_Pin_10)), GPIO_ModeIN_PU );
#ifdef DEBUG
  GPIOA_SetBits(bTXD1);
  GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
  UART1_DefInit( );
#endif   
  CH57X_BLEInit( );
	HAL_Init( );
	GAPRole_CentralInit( );
	USB_VCP_Init();
	Central_Init( ); 
	DelayMs(1000);
  usb_printf("%s\r\n",VER_LIB);
	while(1){
		TMOS_SystemProcess( );
	}
}

/******************************** endfile @ main ******************************/
