/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.1
* Date               : 2019/11/05
* Description        : 外设从机应用主函数及任务系统初始化
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */

#include "CH57x_common.h"
#include "ch57x_usb_device_msc.h"
#include "spi_flash.h"
#include "ff.h"

void DebugInit(void)
{
#if DEBUG == Debug_UART1
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit( );
#elif DEBUG == Debug_UART0
    GPIOB_SetBits(bTXD0);
    GPIOB_ModeCfg(bTXD0, GPIO_ModeOut_PP_5mA);
    UART0_DefInit( );
#endif
}




FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"fatfs test !!!!!!!!!!!!\r\n"; 

int main()
{
    SetSysClock( CLK_SOURCE_HSE_32MHz );
    PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );
    DelayMs(500);

    DebugInit();
    PRINT( "Start @ChipID=%02X\n", R8_CHIP_ID );

    UINT16    i;
    
    res_flash = f_mount(&fs,"0:",1);

/*----------------------- 格式化测试 -----------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		PRINT("》FLASH还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
		res_flash=f_mkfs("0:",0,0, NULL, 0);		

        PRINT("f_mkfs ret: %d\n", res_flash);					

		if(res_flash == FR_OK)
		{
			PRINT("》FLASH已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_flash = f_mount(NULL,"0:",1);			
      /* 重新挂载	*/			
			res_flash = f_mount(&fs,"0:",1);
		}
		else
		{
			PRINT("《《格式化失败。》》\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    PRINT("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    PRINT("！！可能原因：SPI Flash初始化不成功。\r\n");
		while(1);
  }
  else
  {
    PRINT("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -------------------*/
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	PRINT("\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_flash = f_open(&fnew, "0:test.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		PRINT("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      PRINT("》文件写入成功，写入字节数据：%d\n",fnum);
      PRINT("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      PRINT("！！文件写入失败：(%d)\n",res_flash);
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);
	}
	else
	{	
		PRINT("！！打开/创建文件失败。\r\n");
	}
	
/*------------------- 文件系统测试：读测试 --------------------------*/
	PRINT("****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew, "0:test.txt",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		PRINT("》打开文件成功。\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      PRINT("》文件读取成功,读到字节数据：%d\r\n",fnum);
      PRINT("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      PRINT("！！文件读取失败：(%d)\n",res_flash);
    }		
	}
	else
	{
		PRINT("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"0:",1);
  
  /* 操作完成，停机 */
	while(1)
	{
	}

    // PRINT("SPI1 Flash demo start ...\n");
    
    // for(i=0; i!=512; i++)
    // {
    //     send_buf[i] = i;
    // }

    // EraseExternal4KFlash_SPI(0);
    // BlukWriteExternalFlash_SPI( 0,512,send_buf);
    // BlukReadExternalFlash_SPI( 0,512,read_buf );


    // PRINT("send_buf: =================================\r\n");
    // for(i=0; i!=512; i++)
    // {
    //     PRINT("%02x ",(UINT8)send_buf[i]);
    // }
    // PRINT("done\n");

    

    // PRINT("read_buf: =================================\r\n");
    // for(i=0; i!=512; i++)
    // {
    //     PRINT("%02x ",(UINT8)read_buf[i]);
    // }
    // PRINT("done\n");

    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
    USB_DeviceInit();
    NVIC_EnableIRQ( USB_IRQn );


    while(1){



    }
}
/******************************** endfile @ main ******************************/
