#ifndef __APP_FLASH_H__
#define __APP_FLASH_H__

#include "CH57x_common.h"

#include "bsp_spi.h"
#include "bsp_log.h"
#include "hal_device.h"
#include "hal_spi.h"



#define APP_FLASH_DEVICE_NAME               "spi0"
#define APP_FLASH_SPI_NUMBER                0
#define USER_BSP_LIB                        1

#define  APP_FLASH_CMD_STATUS1         0x05
#define  APP_FLASH_CMD_WR_ENABLE       0x06
#define  APP_FLASH_CMD_ERASE_4KBYTE    0x20
#define  APP_FLASH_CMD_ERASE_32KBYTE   0x52
#define  APP_FLASH_CMD_ERASE_CHIP      0xC7
#define  APP_FLASH_CMD_READ_DATA       0x03
#define  APP_FLASH_CMD_PAGE_PROG       0x02
#define  APP_FLASH_CMD_FAST_READ       0x0B
#define  APP_FLASH_CMD_DEVICE_ID       0x90


#if (APP_FLASH_SPI_NUMBER   ==  0)
/********************************* 引脚定义 ************************************
 *    PA3  <===========>  SCS
 *    PA0  <===========>  SCK
 *    PA1  <===========>  DI/MOSI
 *    PA2  <===========>  DO/MISO
 *******************************************************************************/
#define APP_FLASH_FUNC_INIT                 bsp_spi0_init
#define APP_FLASH_FUNC_DEINIT               bsp_spi0_deinit
#define APP_FLASH_FUNC_READ                 bsp_spi0_read
#define APP_FLASH_FUNC_WRITE                bsp_spi0_write
#elif (APP_FLASH_SPI_NUMBER   ==  1)

#endif

/*******************************************************************************
* Function Name  : EraseExternal4KFlash_SPI
* Description    : 擦除4K Flash  擦除一个扇区
* Input          : Dst_Addr 0-1 ffff ffff ,清除任意地址所在的扇区。
* Output         : None
* Return         : None
*******************************************************************************/
void EraseExternal4KFlash_SPI( UINT32 Dst_Addr );
/*******************************************************************************
* Function Name  : EraseExternalFlash_SPI
* Description    : 擦除32K Flash  擦除一个扇区
* Input          : Dst_Addr 0-1 ffff ffff ,清除任意地址所在的扇区。
* Output         : None
* Return         : None
*******************************************************************************/
void EraseExternal32KFlash_SPI( UINT32 Dst_Addr );
/*******************************************************************************
* Function Name  : EraseExternal4KFlash_SPI
* Description    : 擦除全部flash
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EraseExternalAllFlash_SPI( void );

/*******************************************************************************
* Function Name  : PageWriteExternalFlash_SPI
* Description    : 页写；SPI在一页(0~65535)内写入少于256个字节的数据
* Input          : RcvBuffer:数据存储区
*                  StarAddr:开始写入的地址
*                  Len:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
* Output         : None
* Return         : None
*******************************************************************************/
void PageWriteExternalFlash_SPI(UINT32 StarAddr, uint16_t addr_len, UINT16 Len, PUINT8 RcvBuffer);
/*******************************************************************************
* Function Name  : BlukWriteExternalFlash_SPI
* Description    : 无检验写SPI FLASH
*                  必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
*                  具有自动换页功能
*                  在指定地址开始写入指定长度的数据,但是要确保地址不越界!
* Input          : SendBuffer:数据存储区
*                  StarAddr:开始写入的地址
*                  Len:要写入的字节数(最大65535)
* Output         : None
* Return         : None
*******************************************************************************/
void BlukWriteExternalFlash_SPI(UINT32 StarAddr,UINT16 Len,PUINT8 SendBuffer);
/*******************************************************************************
* Function Name  : ReadExternalFlash_SPI
* Description    : 读取地址的数据.
*******************************************************************************/
void ReadExternalFlash_SPI(UINT32 StarAddr, uint16_t addr_len, UINT16 Len, PUINT8 RcvBuffer);
/*******************************************************************************
* Function Name  : BlukReadExternalFlash_SPI
* Description    : 读取起始地址(StarAddr)内多个字节(Len)的数据.存入缓冲区RcvBuffer中
* Input          : StarAddr -Destination Address 000000H - 1FFFFFH
                   Len 读取数据长度
                   RcvBuffer 接收缓冲区起始地址
* Output         : None
* Return         : None
*******************************************************************************/
void BlukReadExternalFlash_SPI(UINT32 StarAddr,UINT16 Len,PUINT8 RcvBuffer);
/*******************************************************************************
* Function Name  : SPIFlash_ReadID
* Description    : SPI Flash读取芯片ID
* Input          : None
* Output         : None
* Return         : 0XEF13,表示芯片型号为W25Q80
*                  0XEF14,表示芯片型号为W25Q16
*                  0XEF15,表示芯片型号为W25Q32
*                  0XEF16,表示芯片型号为W25Q64
*                  0XEF17,表示芯片型号为W25Q128
*******************************************************************************/
UINT16 SPIFlash_ReadID(void);

int app_flash_init();

#endif