/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2018/12/15
* Description 		 : SPI0演示 Master/Slave 模式数据收发
*******************************************************************************/

#include "CH57x_common.h"


unsigned char sta;  // 状态变量     
unsigned char rx_dr_flag;//接收成功标志位
 
#define RX_DR (sta & 0x40)  // 接收成功中断标志
#define TX_DS (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10) // 重发溢出中断标志

#define NRF_MOSI  GPIO_Pin_14
#define NRF_MISO	GPIO_Pin_15
#define NRF_CSK		GPIO_Pin_13
#define NRF_CSN		GPIO_Pin_12
#define NRF_CE		GPIO_Pin_11
#define NRF_IRQ		GPIO_Pin_3

#define NRF_PORT	GPIOA

unsigned char const TX_ADDRESS[5]= {0x34,0x43,0x10,0x10,0x01};	//接收地址
unsigned char const RX_ADDRESS[5]= {0x34,0x43,0x10,0x10,0x01};	//本地地址


void Init_NRF24L01(void)
{
	GPIOA_ResetBits( NRF_CE );// 待机
	GPIOA_SetBits( NRF_CSN ); // SPI禁止
	GPIOA_ResetBits( NRF_CSK );// SPI时钟置低
	GPIOA_SetBits( NRF_IRQ ); // 中断复位
}



unsigned char SPI_RW(unsigned char byte)
{
    SPI0_MasterSendByte(byte);
 
	return SPI0_MasterRecvByte();
}

unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
{
    unsigned char status;
 
    GPIOA_ResetBits( NRF_CSN ); // SPI禁止
    status = SPI_RW(reg);
    SPI_RW(value);
    GPIOA_SetBits( NRF_CSN ); // SPI禁止
 
    return (status);
}

unsigned char SPI_Read(unsigned char reg)
{
    unsigned char reg_val;
 
    GPIOA_ResetBits( NRF_CSN ); // SPI禁止
    SPI_RW(reg);
    reg_val = SPI_RW(0);
    GPIOA_SetBits( NRF_CSN ); // SPI禁止
 
    return (reg_val);
}

__align(4) UINT8 spiBuff[]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};
__align(4) UINT8 spiBuffrev[16];

void DebugInit(void)		
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}


unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
    unsigned char status;
    unsigned char byte_ctr;
 
    GPIOA_ResetBits( NRF_CSN ); // SPI禁止
    SPI0_MasterSendByte(reg);
    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
    {
        pBuf[byte_ctr] = SPI0_MasterRecvByte();
    }
    GPIOA_SetBits( NRF_CSN ); // SPI禁止
 
    return (status);
}

unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
    unsigned char status;
    unsigned char byte_ctr;
 
    GPIOA_ResetBits( NRF_CSN ); // SPI禁止
    SPI0_MasterSendByte(reg);
    DelayUs(10);
    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
    {
        SPI0_MasterSendByte(*pBuf++);
    }
    GPIOA_SetBits( NRF_CSN ); // SPI禁止
 
    return (status);
}
#define WRITE_REG       0x20  // Define write command to register
#define READ_REG        0x00  // Define read command to register
#define TX_ADDR         0x10  // 'TX address' register address
unsigned char NRF24L01_Check(void)
{
	unsigned char buf[5]={0xa5,0xa5,0xa5,0xa5,0xa5};
	unsigned char i;

	SPI_Write_Buf(WRITE_REG+TX_ADDR,buf,5);//写入5字节的地址
	SPI_Read_Buf(READ_REG+TX_ADDR,buf,5);
	for(i=0;i<5;i++)
		{
			if(buf[i]!=0xa5)
				break;
	}
	if(i!=5)
		return 1;
	return 0;
}
int main()
{
    UINT8 i;
    
/* 配置串口调试 */   
    DebugInit();
    PRINT( "Start @ChipID=%02X\n", R8_CHIP_ID );

#if 1        
/* 主机模式 */ 
    PRINT( "1.spi0 mul master mode send data ...\n");   
    DelayMs( 100 );
    
    GPIOA_SetBits( GPIO_Pin_12 );
    GPIOA_ModeCfg(GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14, GPIO_ModeOut_PP_5mA);
		GPIOA_ModeCfg( GPIO_Pin_15, GPIO_ModeIN_PD);
	
	
	
	
//	while(1)
//	{
//		PRINT( "HIGH\r\n" );
//		 GPIOA_SetBits( GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14 );
//		DelayMs( 2000 );
//		PRINT( "LOW\r\n" );
//		 GPIOA_ResetBits( GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14 );
//		DelayMs( 2000 );
//		
//	}
    SPI0_MasterDefInit();
		R8_SPI0_CLOCK_DIV = 32;
	Init_NRF24L01();
	DelayMs( 1000 );
	PRINT( "Init_NRF24L01\r\n");  
//	DelayMs( 2000 );
		if( NRF24L01_Check() !=0 )
			PRINT( "ERROR\r\n");   
		else
			PRINT( "RIGHT\r\n");   
	
	while(1);
	
    
// 单字节发送
    GPIOA_ResetBits( GPIO_Pin_12 );    
    SPI0_MasterSendByte(0x55);
    GPIOA_SetBits( GPIO_Pin_12 );
    DelayMs( 1 );
    GPIOA_ResetBits( GPIO_Pin_12 );
    i = SPI0_MasterRecvByte();
    GPIOA_SetBits( GPIO_Pin_12 );
    DelayMs( 2 );
    
// FIFO 连续发送 
    GPIOA_ResetBits( GPIO_Pin_12 ); 
    SPI0_MasterTrans( spiBuff, 9 );
    GPIOA_SetBits( GPIO_Pin_12 );
    DelayMs( 1 );
    GPIOA_ResetBits( GPIO_Pin_12 ); 
    SPI0_MasterRecv( spiBuffrev, 12 );
    GPIOA_SetBits( GPIO_Pin_12 );
    DelayMs( 1 );
    
// DMA 连续发送 
    GPIOA_ResetBits( GPIO_Pin_12 ); 
    SPI0_MasterDMATrans(spiBuff, 12);
    GPIOA_SetBits( GPIO_Pin_12 );
    DelayMs( 1 );
    GPIOA_ResetBits( GPIO_Pin_12 ); 
    SPI0_MasterDMARecv(spiBuffrev, 12);
    GPIOA_SetBits( GPIO_Pin_12 );

    PRINT( "END ...\n");
    while(1);
#endif

#if 0
/* 设备模式 */ 
    PRINT( "1.spi0 mul slave mode \n");
    GPIOA_ModeCfg(GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, GPIO_ModeIN_PU);
    SPI0_SlaveInit(); 
    i = SPI0_SlaveRecvByte();
    SPI0_SlaveSendByte( ~i );
  
    SPI0_SlaveRecv( spiBuffrev, 9 );
    SPI0_SlaveTrans( spiBuffrev, 12 );  

    while(1); 
#endif
    
    while(1);    
     
}





