#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

#include "CH57x_common.h"

#include "bsp_spi.h"
#include "bsp_log.h"
#include "hal_device.h"
#include "hal_spi.h"



#define HAL_FLASH_DEVICE_NAME               "spi0"
#define HAL_FLASH_SPI_NUMBER                0
#define USER_BSP_LIB                        1

#define  HAL_FLASH_CMD_STATUS1         0x05
#define  HAL_FLASH_CMD_WR_ENABLE       0x06
#define  HAL_FLASH_CMD_ERASE_4KBYTE    0x20
#define  HAL_FLASH_CMD_ERASE_32KBYTE   0x52
#define  HAL_FLASH_CMD_ERASE_CHIP      0xC7
#define  HAL_FLASH_CMD_READ_DATA       0x03
#define  HAL_FLASH_CMD_PAGE_PROG       0x02
#define  HAL_FLASH_CMD_FAST_READ       0x0B
#define  HAL_FLASH_CMD_DEVICE_ID       0x90


#if (HAL_FLASH_SPI_NUMBER   ==  0)
/********************************* 引脚定义 ************************************
 *    PA3  <===========>  SCS
 *    PA0  <===========>  SCK
 *    PA1  <===========>  DI/MOSI
 *    PA2  <===========>  DO/MISO
 *******************************************************************************/
#define HAL_FLASH_FUNC_INIT                 bsp_spi0_init
#define HAL_FLASH_FUNC_DEINIT               bsp_spi0_deinit
#define HAL_FLASH_FUNC_READ                 bsp_spi0_read
#define HAL_FLASH_FUNC_WRITE                bsp_spi0_write
#elif (HAL_FLASH_SPI_NUMBER   ==  1)

#endif

/*******************************************************************************
* Function Name  : hal_flash_erase_4k_flash
* Description    : 擦除4K Flash  擦除一个扇区
* Input          : Dst_Addr 0-1 ffff ffff ,清除任意地址所在的扇区。
* Output         : None
* Return         : None
*******************************************************************************/
void hal_flash_erase_4k_flash( UINT32 Dst_Addr );
/*******************************************************************************
* Function Name  : EraseExternalFlash_SPI
* Description    : 擦除32K Flash  擦除一个扇区
* Input          : Dst_Addr 0-1 ffff ffff ,清除任意地址所在的扇区。
* Output         : None
* Return         : None
*******************************************************************************/
void hal_flash_erase_32k_flash( UINT32 Dst_Addr );
/*******************************************************************************
* Function Name  : hal_flash_erase_4k_flash
* Description    : 擦除全部flash
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_flash_erase_all_flash( void );

/*******************************************************************************
* Function Name  : hal_flash_write
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
void hal_flash_write(UINT32 StarAddr,UINT16 Len,PUINT8 SendBuffer);
/*******************************************************************************
* Function Name  : hal_flash_read
* Description    : 读取起始地址(StarAddr)内多个字节(Len)的数据.存入缓冲区RcvBuffer中
* Input          : StarAddr -Destination Address 000000H - 1FFFFFH
                   Len 读取数据长度
                   RcvBuffer 接收缓冲区起始地址
* Output         : None
* Return         : None
*******************************************************************************/
void hal_flash_read(UINT32 StarAddr,UINT16 Len,PUINT8 RcvBuffer);
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

int hal_flash_init();

#endif