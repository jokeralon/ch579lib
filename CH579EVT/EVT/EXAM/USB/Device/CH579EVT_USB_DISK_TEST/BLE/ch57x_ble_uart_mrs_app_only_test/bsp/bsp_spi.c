#include "bsp_spi.h"

int bsp_spi0_init()
{
    GPIOA_SetBits(bMOSI | bSCK0 | bSCS);
    GPIOA_ModeCfg(bMOSI | bSCK0 | bSCS, GPIO_ModeOut_PP_5mA); // MOSI/SCK/CS
    GPIOA_SetBits(bMISO);
    GPIOA_ModeCfg(bMISO, GPIO_ModeIN_PU); // MOSI/SCK/CS

    SPI0_MasterDefInit();
    SPI0_DataMode(Mode3_HighBitINFront);
    SPI0_CLKCfg(4);
}

int bsp_spi0_deinit()
{
    
    ;
}

/*******************************************************************************
 * Function Name  : ReadExternalFlash_SPI
 * Description    : 读取地址的数据.
 *******************************************************************************/
int bsp_spi0_read(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff)
{
    GPIOA_ResetBits(bSCS);
    SPI0_MasterSendByte(cmd);                 // 读命令
    // SPI0_MasterSendByte(((addr & 0xFFFFFF) >> 16)); // 发送3字节地址
    // SPI0_MasterSendByte(((addr & 0xFFFF) >> 8));
    // SPI0_MasterSendByte(addr & 0xFF);
    
    for( int i=0; i< addr_len; i++ )
    {
        uint8_t temp = (((addr) >> ( (addr_len-i-1)*8 )) & 0xff);
        SPI0_MasterSendByte(temp);
    }
    if (buff != NULL)
        SPI0_MasterRecv(buff, len);
    GPIOA_SetBits(bSCS);
}

int bsp_spi0_write(uint8_t cmd, uint32_t addr, uint16_t addr_len, uint16_t len, uint8_t *buff)
{
    GPIOA_ResetBits(bSCS);
    SPI0_MasterSendByte(cmd);                 // 发送写页命令
    // SPI0_MasterSendByte(((addr & 0xFFFFFF) >> 16)); // 发送24bit地址
    // SPI0_MasterSendByte(((addr & 0xFFFF) >> 8));
    // SPI0_MasterSendByte(addr & 0xFF);

    
    for( int i=0; i< addr_len; i++ )
    {
        uint8_t temp = (((addr) >> ( (addr_len-i-1)*8 )) & 0xff);
        SPI0_MasterSendByte(temp);
    }

    for (int i=0; i != len; i++)
    {
        if (buff != NULL)
            SPI0_MasterSendByte(buff[i]); // 循环写数
    }
    GPIOA_SetBits(bSCS);
}

