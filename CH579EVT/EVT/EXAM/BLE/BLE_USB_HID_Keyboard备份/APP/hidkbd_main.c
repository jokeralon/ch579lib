/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2018/12/10
* Description        : 蓝牙键盘应用主函数及任务系统初始化
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "CONFIG.h"
#include "CH57x_common.h"
#include "HAL.h"
#include "hiddev.h"
#include "hidkbd.h"

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
//  GPIOA_ModeCfg( GPIO_Pin_All, GPIO_ModeIN_PU );
//  GPIOB_ModeCfg( GPIO_Pin_All&(~(GPIO_Pin_11|GPIO_Pin_10)), GPIO_ModeIN_PU );
//	
//	GPIOB_ModeCfg(GPIO_Pin_18 | GPIO_Pin_19, GPIO_ModeOut_PP_5mA );
	
	GPIOB_ModeCfg(GPIO_Pin_22 | GPIO_Pin_1, GPIO_ModeIN_PU );
	SetSysClock( CLK_SOURCE_HSE_32MHz );
    PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );		// 打开PLL
    DelayMs(5);
	
#ifdef DEBUG
  GPIOA_SetBits(bTXD1);
  GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
  UART1_DefInit( );
#endif   
  PRINT("%s\n",VER_LIB);
  CH57X_BLEInit( );
	HAL_Init( );
	GAPRole_PeripheralInit( );
	HidDev_Init( ); 
	HidEmu_Init( );
	
	UsbHid_Init();
//	pEP1_IN_DataBuf[2] = 0x04;

	while(1){
		TMOS_SystemProcess( );
		if( GPIOB_ReadPortPin( GPIO_Pin_22) == 0 )
		{
			R8_USB_CTRL &= ~RB_UC_HOST_MODE;
			R8_USB_CTRL &= ~(0x03 << 4);
			while( GPIOB_ReadPortPin( GPIO_Pin_22) == 0 );
		}
		if( GPIOB_ReadPortPin( GPIO_Pin_1) == 0 )
		{
			R8_USB_CTRL &= ~RB_UC_HOST_MODE;
			R8_USB_CTRL |= (0x03 << 4);
			while( GPIOB_ReadPortPin( GPIO_Pin_1) == 0 );
		}
		
		
//		DevEP1_IN_Deal(8);
//		DelayMs(500);
	}
}

/******************************** endfile @ main ******************************/
