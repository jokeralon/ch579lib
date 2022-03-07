/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2018/12/10
* Description        : ��������Ӧ��������������ϵͳ��ʼ��
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "CONFIG.h"
#include "CH57x_common.h"
#include "HAL.h"
#include "hiddev.h"
#include "hidkbd.h"

#include "mykeyboard.h"
#include "ws2812.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__align(4) u32 MEM_BUF[BLE_MEMHEAP_SIZE/4];

#if (defined (BLE_MAC)) && (BLE_MAC == TRUE)
u8C MacAddr[6] = {0x84,0xC2,0xE4,0x03,0x02,0x02};
#endif

/*******************************************************************************
* Function Name  : main
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main( void ) 
{
//  GPIOA_ModeCfg( GPIO_Pin_All, GPIO_ModeIN_PU );
  GPIOB_ModeCfg( GPIO_Pin_All&(~(GPIO_Pin_11|GPIO_Pin_10)), GPIO_ModeIN_PU );
			  /* �ⲿ32Mʱ�� */ 
	SetSysClock( CLK_SOURCE_HSE_32MHz );
	
	  /* ��PLL */ 
  PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );	
	
	
#ifdef DEBUG
  GPIOA_SetBits(bTXD1);
  GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
  UART1_DefInit( );
#endif   
  PRINT("%s\n",VER_LIB);
  CH57X_BLEInit( );
	HAL_Init( );
	
	keyBoard_GPIO_Init();
	GAPRole_PeripheralInit( );
	HidDev_Init( ); 
	HidEmu_Init( );
	
	GPIOA_ModeCfg( GPIO_Pin_3,GPIO_ModeIN_PU  );
	
	RGB_GPIO_Init();
	RGB_LED_Red();
	while(1){
		TMOS_SystemProcess( );
		if(GPIOA_ReadPortPin(GPIO_Pin_3) == 0)
			RGB_LED_Green();
		
	}
}

/******************************** endfile @ main ******************************/
