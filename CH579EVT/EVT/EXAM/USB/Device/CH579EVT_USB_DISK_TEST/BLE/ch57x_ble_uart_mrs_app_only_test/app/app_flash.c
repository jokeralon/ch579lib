#include "app_flash.h"

hal_spi_config_t spi0_config = {
    .name = APP_FLASH_DEVICE_NAME,
    .init = APP_FLASH_FUNC_INIT,
    .deinit = APP_FLASH_FUNC_DEINIT,
    .read = APP_FLASH_FUNC_READ,
    .write = APP_FLASH_FUNC_WRITE,
    .callback = NULL,
};

int app_flash_fd = -1;



/*******************************************************************************
 * Function Name  : app_flash_read_status_reg
 * Description    : 用来读取状态寄存器,并返回状态寄存器的值
 * Input          : None
 * Output         : None
 * Return         : ExFlashRegStatus
 *******************************************************************************/
uint8_t app_flash_read_status_reg()
{
    UINT8 ExFlashRegStatus;

    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_STATUS1,
        .addr = 0x00,
        .addr_len = 0,
        .len = 1,
        .buff = &ExFlashRegStatus,
    };

    bsp_device_read(app_flash_fd, &spi_data_buff, 0);

    return ExFlashRegStatus;
}

/*******************************************************************************
 * Function Name  : app_flash_wait_busy
 * Description    : 等待芯片空闲(在执行Byte-Program, Sector-Erase, Block-Erase, Chip-Erase操作后)
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void app_flash_wait_busy(void)
{
    while ((app_flash_read_status_reg()) & 0x01 == 0x01)
    {
        ; // 等待直到Flash空闲
    }
}

/*******************************************************************************
 * Function Name  : app_flash_write_enable
 * Description    : 写使能,同样可以用于使能写状态寄存器
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void app_flash_write_enable(void)
{
    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_WR_ENABLE,
        .addr = 0x00,
        .addr_len = 0,
        .len = 0,
        .buff = NULL,
    };

    bsp_device_write(app_flash_fd, &spi_data_buff, 0);
}

/*******************************************************************************
 * Function Name  : app_flash_write_bytes
 * Description    : 页写；SPI在一页(0~65535)内写入少于256个字节的数据
 * Input          : RcvBuffer:数据存储区
 *                  StarAddr:开始写入的地址
 *                  Len:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
 * Output         : None
 * Return         : None
 *******************************************************************************/
static void app_flash_write_bytes(UINT32 StarAddr, uint16_t addr_len, UINT16 Len, PUINT8 RcvBuffer)
{
    UINT16 i;
    app_flash_write_enable(); // SET WEL
    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_PAGE_PROG,
        .addr = StarAddr,
        .addr_len = addr_len,
        .len = Len,
        .buff = RcvBuffer,
    };

    bsp_device_write(app_flash_fd, &spi_data_buff, 0);
    app_flash_wait_busy(); // 等待写入结束
}
/*******************************************************************************
 * Function Name  : ReadExternalFlash_SPI
 * Description    : 读取地址的数据.
 *******************************************************************************/
static void app_flash_read_bytes(UINT32 StarAddr, uint16_t addr_len, UINT16 Len, PUINT8 RcvBuffer)
{
    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_READ_DATA,
        .addr = StarAddr,
        .addr_len = addr_len,
        .len = Len,
        .buff = RcvBuffer,
    };

    bsp_device_read(app_flash_fd, &spi_data_buff, 0);
}


/*******************************************************************************
 * Function Name  : app_flash_erase_4k_flash
 * Description    : 擦除4K Flash  擦除一个扇区
 * Input          : Dst_Addr 0-1 ffff ffff ,清除任意地址所在的扇区。
 * Output         : None
 * Return         : None
 *******************************************************************************/
void app_flash_erase_4k_flash(UINT32 Dst_Addr)
{
    app_flash_write_enable();
    app_flash_wait_busy();

    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_ERASE_4KBYTE,
        .addr = Dst_Addr,
        .addr_len = 3,
        .len = 0,
        .buff = NULL,
    };

    bsp_device_write(app_flash_fd, &spi_data_buff, 0);
    app_flash_wait_busy();
}
/*******************************************************************************
 * Function Name  : app_flash_erase_4k_flash
 * Description    : 擦除全部flash
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void app_flash_erase_all_flash(void)
{
    app_flash_write_enable();
    app_flash_wait_busy();

    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_ERASE_CHIP,
        .addr = 0x00,
        .addr_len = 0,
        .len = 0,
        .buff = NULL,
    };

    bsp_device_write(app_flash_fd, &spi_data_buff, 0);

    app_flash_wait_busy();
}
/*******************************************************************************
 * Function Name  : EraseExternalFlash_SPI
 * Description    : 擦除32K Flash  擦除一个扇区
 * Input          : Dst_Addr 0-1 ffff ffff ,清除任意地址所在的扇区。
 * Output         : None
 * Return         : None
 *******************************************************************************/
void app_flash_erase_32k_flash(UINT32 Dst_Addr)
{
    app_flash_write_enable();
    app_flash_wait_busy();

    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_ERASE_32KBYTE,
        .addr = Dst_Addr,
        .addr_len = 3,
        .len = 0,
        .buff = NULL,
    };

    bsp_device_write(app_flash_fd, &spi_data_buff, 0);

    app_flash_wait_busy();
}
/*******************************************************************************
 * Function Name  : app_flash_write
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
void app_flash_write(UINT32 StarAddr, UINT16 Len, PUINT8 SendBuffer)
{
    UINT16 pageremain;

    pageremain = 256 - StarAddr % 256; // 单页剩余的字节数
    if (Len <= pageremain)
    {
        pageremain = Len; // 不大于256个字节
    }
    while (1)
    {
        app_flash_write_bytes(StarAddr, 3, pageremain, SendBuffer);
        if (Len == pageremain)
        {
            break; // 写入结束了
        }
        else
        {
            SendBuffer += pageremain;
            StarAddr += pageremain;
            Len -= pageremain; // 减去已经写入了的字节数
            if (Len > 256)
            {
                pageremain = 256; // 一次可以写入256个字节
            }
            else
            {
                pageremain = Len; // 不够256个字节了
            }
        }
    }
}

/*******************************************************************************
* Function Name  : app_flash_read
* Description    : 读取起始地址(StarAddr)内多个字节(Len)的数据.存入缓冲区RcvBuffer中
* Input          : StarAddr -Destination Address 000000H - 1FFFFFH
                   Len 读取数据长度
                   RcvBuffer 接收缓冲区起始地址
* Output         : None
* Return         : None
// *******************************************************************************/
void app_flash_read(UINT32 StarAddr, UINT16 Len, PUINT8 RcvBuffer)
{

    UINT16 pageremain;

    pageremain = 256 - StarAddr % 256; // 单页剩余的字节数
    if (Len <= pageremain)
    {
        pageremain = Len; // 不大于256个字节
    }
    while (1)
    {
        app_flash_read_bytes(StarAddr, 3, pageremain, RcvBuffer);
        if (Len == pageremain)
        {
            break; // 写入结束了
        }
        else
        {
            RcvBuffer += pageremain;
            StarAddr += pageremain;
            Len -= pageremain; // 减去已经写入了的字节数
            if (Len > 256)
            {
                pageremain = 256; // 一次可以写入256个字节
            }
            else
            {
                pageremain = Len; // 不够256个字节了
            }
        }
    }
}

/*******************************************************************************
 * Function Name  : app_flash_read_flashID
 * Description    : SPI Flash读取芯片ID
 * Input          : None
 * Output         : None
 * Return         : 0XEF13,表示芯片型号为W25Q80
 *                  0XEF14,表示芯片型号为W25Q16
 *                  0XEF15,表示芯片型号为W25Q32
 *                  0XEF16,表示芯片型号为W25Q64
 *                  0XEF17,表示芯片型号为W25Q128
 *******************************************************************************/
UINT16 app_flash_read_flashID(void)
{
    UINT16 temp = 0;

    uint8_t id[2];
    hal_spi_data_buff_t spi_data_buff = {
        .cmd = APP_FLASH_CMD_DEVICE_ID,
        .addr = 0,
        .addr_len = 3,
        .len = 2,
        .buff = id,
    };

    bsp_device_read(app_flash_fd, &spi_data_buff, 0);

    temp = (id[0] << 8) | id[1];

    return temp;
}

int app_flash_init()
{

    if (hal_spi_init(&spi0_config) == HAL_SPI_OK)
    {
        LOG_INFO("app flash init ok\n");
    }else
    {
        LOG_INFO("app flash init error\n");
    }

    app_flash_fd = bsp_device_open(APP_FLASH_DEVICE_NAME, 0);

    if (app_flash_fd >= 0)
    {
        LOG_INFO("app flash open ok, fd: %d\n", app_flash_fd);

        return 0;
    }

    LOG_ERROR("app flash init error\n");

    return -1;
}
