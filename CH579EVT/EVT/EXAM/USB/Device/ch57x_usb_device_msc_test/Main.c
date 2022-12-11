/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2019/4/28
* Description 		 : 定时器例子
*******************************************************************************/

#include "CH57x_common.h"

#include "ch57x_usb_device_msc.h"





void DebugInit(void)		
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

void hex_dump(uint8_t *data,uint32_t length) {
    for(uint32_t i=0;i<length;i++) {
        PRINT("%02x ",data[i]);
    }
    PRINT("\r\n");
}











int main()
{     
 
    SetSysClock( CLK_SOURCE_HSE_32MHz );
    PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );		// 打开PLL
    DelayMs(5);
    
    DebugInit();
    PRINT( "Start @ChipID=%02X\n", R8_CHIP_ID );
    
    
    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
    USB_DeviceInit();
    NVIC_EnableIRQ( USB_IRQn );

 
    while(1){
      


    }   
}



