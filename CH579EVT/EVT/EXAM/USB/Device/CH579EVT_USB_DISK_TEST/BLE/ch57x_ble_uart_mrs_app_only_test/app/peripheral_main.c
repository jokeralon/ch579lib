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
#include "hal_device.h"
#include "hal_spi.h"
#include "bsp_spi.h"
#include "bsp_log.h"
#include "app_flash.h"

FATFS fs;          /* FatFs文件系统对象 */
FIL fnew;          /* 文件对象 */
FRESULT res_flash; /* 文件操作结果 */
UINT fnum;         /* 文件成功读写数量 */
UINT8 uart0_read_len = 0;
UINT8 uart0_read_flag = 0;
UINT8 uart0_read_timeout = 0;
UINT8 uart0_read_buf[1024] = {0};
BYTE ReadBuffer[1024] = {0}; /* 读缓冲区 */
BYTE WriteBuffer[] =         /* 写缓冲区*/
    "fatfs test !!!!!!!!!!!!\r\n";
BYTE WriteBuffer2[] = /* 写缓冲区*/
    "asdfghjklzxcvbnmqwertyuiop\r\n";
UINT8 buf[512];
void DebugInit(void)
{
#if DEBUG == Debug_UART1
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
#elif DEBUG == Debug_UART0
    GPIOB_SetBits(bTXD0);
    GPIOB_ModeCfg(bTXD0, GPIO_ModeOut_PP_5mA);
    GPIOB_SetBits(bRXD0);
    GPIOB_ModeCfg(bRXD0, GPIO_ModeIN_PU);

    UART0_DefInit();
    UART0_ByteTrigCfg(UART_1BYTE_TRIG);
    UART0_INTCfg(ENABLE, RB_IER_LINE_STAT | RB_IER_RECV_RDY);
    NVIC_EnableIRQ(UART0_IRQn);
#endif
}

UINT8 work[4096];
int main()
{
    SetSysClock( CLK_SOURCE_HSE_32MHz );
    PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );		// ��PLL
    DelayMs(5);
    
    DebugInit();
    
    LOG_INFO( "Start @ChipID=%02X\n", R8_CHIP_ID );

    app_flash_init();

    res_flash = f_mount(&fs, "0:", 1);

    /*----------------------- 格式化测试 -----------------*/
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if (res_flash == FR_NO_FILESYSTEM)
    {
        LOG_INFO("FLASH does not have a file system yet, it will be formatted soon ...\r\n");
        /* 格式化 */
        res_flash=f_mkfs("0:",FM_FAT,0, work, sizeof(work));

        LOG_INFO("f_mkfs ret: %d\n", res_flash);

        if (res_flash == FR_OK)
        {
            LOG_INFO("FLASH have a file system yet\r\n");
            /* 格式化后，先取消挂载 */
            res_flash = f_mount(NULL, "0:", 1);
            /* 重新挂载	*/
            res_flash = f_mount(&fs, "0:", 1);
        }
        else
        {
            LOG_INFO("FLASH format error\r\n");
            while (1)
                ;
        }
    }
    else if (res_flash != FR_OK)
    {
        LOG_INFO("external FLASH mount file system error, code :%d\r\n", res_flash);
        while (1)
            ;
    }
    else
    {
        LOG_INFO("file system mount ok, Read and write tests are available");
    }

    /*----------------------- 文件系统测试：写测试 -------------------*/
    /* 打开文件，每次都以新建的形式打开，属性为可写 */
    LOG_INFO("\r\n****** File write test is about to be performed... ******\r\n");
    res_flash = f_open(&fnew, "0:test.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res_flash == FR_OK)
    {
        LOG_INFO("open/create FatFs read/write test file, \"test.txt\" create ok, write date to this file\r\n");
        /* 将指定存储区内容写入到文件内 */
        res_flash = f_write(&fnew, WriteBuffer, sizeof(WriteBuffer), &fnum);
        if (res_flash == FR_OK)
        {
            LOG_INFO("this file written ok, number of data: %d\n", fnum);
            LOG_INFO("writed data as: \r\n%s\r\n", WriteBuffer);
        }
        else
        {
            LOG_INFO("write file error, code: %d\n", res_flash);
        }
        /* 不再读写，关闭文件 */
        f_close(&fnew);
    }
    else
    {
        LOG_INFO("open/create file error\r\n");
    }

    /*------------------- 文件系统测试：读测试 --------------------------*/
    LOG_INFO("****** File read test coming... ******\r\n");
    res_flash = f_open(&fnew, "0:test.txt", FA_OPEN_EXISTING | FA_READ);
    if (res_flash == FR_OK)
    {
        LOG_INFO("open file ok\r\n");
        res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (res_flash == FR_OK)
        {
            LOG_INFO("The file was read successfully. Byte data was read: %d\r\n", fnum);
            LOG_INFO("The file data obtained by reading is:\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            LOG_INFO("read file error, code: %d\n", res_flash);
        }
    }
    else
    {
        LOG_INFO("open file error\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL, "0:", 1);

    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
    USB_DeviceInit();
    NVIC_EnableIRQ(USB_IRQn);

    while (1)
    {
        if (uart0_read_flag == 1)
        {
            UART0_SendString(uart0_read_buf, uart0_read_len);
            // fatfs_test(uart0_read_buf, uart0_read_len);
            uart0_read_len = 0;
            uart0_read_flag = 0;
        }
        else
        {
            DelayMs(20);
            uart0_read_timeout++;
            if (uart0_read_timeout > 10)
            {
                uart0_read_timeout = 0;
                memset(uart0_read_buf, '\0', sizeof(uart0_read_buf));
                uart0_read_len = 0;
                uart0_read_flag = 0;
            }
        }
    }
}

void UART0_IRQHandler(void)
{
    UINT8 i;
    switch (UART0_GetITFlag())
    {
    case UART_II_LINE_STAT: // 线路状态错误
        UART0_GetLinSTA();
        LOG_INFO("UART0 LINE ERROR");
        break;

    case UART_II_RECV_RDY: // 数据达到设置触发点
                           // for(i=0; i!=trigB; i++)
                           // {
                           //     uart0_read_buf[i] = UART0_RecvByte();
                           //     UART0_SendByte(uart0_read_buf[i]);
                           // }

        uart0_read_buf[uart0_read_len] = UART0_RecvByte();
        if ((uart0_read_len < 1024) && (uart0_read_flag == 0))
        {
            if (uart0_read_buf[uart0_read_len] == '\n')
            {
                uart0_read_flag = 1;
                return;
            }
            uart0_read_len++;
        }
        break;

    case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
        // memset(uart0_read_buf, '\0', sizeof(uart0_read_buf));
        // i = UART0_RecvString(uart0_read_buf);
        // UART0_SendString( uart0_read_buf, i );
        break;

    case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
        break;

    case UART_II_MODEM_CHG: // 只支持串口0
        break;

    default:
        break;
    }
}

/******************************** endfile @ main ******************************/
