/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2018/12/15
* Description 		 : SPI0演示 Master/Slave 模式数据收发
*******************************************************************************/

#include "CH57x_common.h"
typedef unsigned char u8;
typedef unsigned int u16;

#define TRUE		1
#define FALSE		0

__align(4) UINT8 spiBuff[]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};
__align(4) UINT8 spiBuffrev[16];

#define WS2812_NUM	20
#define WS2812_Low	0x80
#define WS2812_High	0xfc

unsigned char rgb_buf[WS2812_NUM*3];
unsigned char rgb_buf_big[WS2812_NUM*3*8];
unsigned char rgb_num = 0;

unsigned char r[3] = {255,0,0};
unsigned char g[3] = {0,255,0};
unsigned char b[3] = {0,0,255};

unsigned char reset[3] = {0,0,0};

unsigned char CODE1 = 0xFC;
unsigned char CODE0 = 0xC0;


void LED_SPI_SendBuf()
{
	int Pixel2SPI=0;
	UINT8 i=0;
	for(Pixel2SPI = 0;Pixel2SPI<WS2812_NUM*3;Pixel2SPI++)
	{
		for(i = 0;i<8;i++)
		{
			if( (rgb_buf[Pixel2SPI] << i)& 0x80 )
			{
				SPI0_MasterDMATrans(&CODE1,1);
			}else
			{
				SPI0_MasterDMATrans(&CODE0,1);
			}
		}
	}
}

void LED_SPI_SendBits(u8 bits)
{
	int Pixel2SPI=0;
	for(Pixel2SPI = 0;Pixel2SPI<8;Pixel2SPI++)
	{
		if( (bits<<Pixel2SPI) & 0x80 )
		{
			SPI0_MasterDMATrans(&CODE1,1);
		}else
		{
			SPI0_MasterDMATrans(&CODE0,1);
		}
	}
}

void LED_SPI_SendPixel(unsigned char *rgb)
{	
	LED_SPI_SendBits(rgb[1]);//G
	LED_SPI_SendBits(rgb[0]);	//R
	LED_SPI_SendBits(rgb[2]);	//B
}
UINT8 LED_RGB2Buf(unsigned char *rgb)
{
	if(rgb_num<WS2812_NUM )
	{
		rgb_buf[rgb_num*3+0] = rgb[1];
		rgb_buf[rgb_num*3+1] = rgb[0];
		rgb_buf[rgb_num*3+2] = rgb[2];
		
		rgb_num++;
		
		return TRUE;
	}
	return FALSE;
	
}

UINT8 LED_RGB2Buf_BIG(unsigned char *rgb)
{
	UINT8 i;
	if(rgb_num<WS2812_NUM )
	{
		for(i=0;i<8;i++)
		{
			if( (rgb[1]<<i) & 0x80 )
			{
				rgb_buf_big[rgb_num*3*8+i] = CODE1;
			}else
			{
				rgb_buf_big[rgb_num*3*8+i] = CODE0;
			}
		}
		for(i=0;i<8;i++)
		{
			if( (rgb[0]<<i) & 0x80 )
			{
				rgb_buf_big[rgb_num*3*8+i+8] = CODE1;
			}else
			{
				rgb_buf_big[rgb_num*3*8+i+8] = CODE0;
			}
		}
			for(i=0;i<8;i++)
		{
			if( (rgb[2]<<i) & 0x80 )
			{
				rgb_buf_big[rgb_num*3*8+i+8+8] = CODE1;
			}else
			{
				rgb_buf_big[rgb_num*3*8+i+8+8] = CODE0;
			}
		}
		rgb_num++;
		return TRUE;
	}
		
	return FALSE;
	
}

void DebugInit(void)		
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

int main()
{
    UINT8 i=0; 
		UINT8 j=0;
/* 配置串口调试 */   
    DebugInit();
    PRINT( "Start @ChipID=%02X\n", R8_CHIP_ID );

#if 1        
/* 主机模式 */ 
    PRINT( "1.spi0 mul master mode send data ...\n");   
    DelayMs( 100 );
		  SetSysClock( CLK_SOURCE_HSE_32MHz );
	  /* 打开PLL */ 
    PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );	
	
	for(i=0;i<WS2812_NUM;i++)
	{
		switch(j)
		{
			case 3: j =0;
			case 0: LED_RGB2Buf_BIG(r);break;
			case 1: LED_RGB2Buf_BIG(g);break;
			case 2: LED_RGB2Buf_BIG(b);break;
			default:break;
			
		}
		j++;
		
	}
	
    GPIOA_SetBits( GPIO_Pin_12 );
    GPIOA_ModeCfg(GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14, GPIO_ModeOut_PP_5mA);
		GPIOA_ModeCfg(GPIO_Pin_1|GPIO_Pin_0, GPIO_ModeOut_PP_5mA);
    SPI0_MasterDefInit( );
		R8_SPI0_CLOCK_DIV = 4;
	DelayMs( 1000 );
	
	LED_SPI_SendPixel(reset);
	
	// WS2812 复位
	R8_SPI0_CTRL_MOD &= !RB_SPI_MOSI_OE;
	GPIOB_ResetBits( GPIO_Pin_14 );
	GPIOB_ModeCfg( GPIO_Pin_14, GPIO_ModeOut_PP_5mA );
	mDelayuS(80);
	R8_SPI0_CTRL_MOD |= RB_SPI_MOSI_OE;
	
	
	
	
	SPI0_MasterTrans(rgb_buf_big,WS2812_NUM*3*8);
	

//	
//	LED_SPI_SendPixel(r);
//	LED_SPI_SendPixel(g);
//	LED_SPI_SendPixel(b);


//	LED_SPI_SendPixel(r);
//	LED_SPI_SendPixel(g);
//	LED_SPI_SendPixel(b);
	while(1)
		{


			
		}
	
	
	
    
// 单字节发送
//    GPIOA_ResetBits( GPIO_Pin_12 );    
//    SPI0_MasterSendByte(0x55);
//    GPIOA_SetBits( GPIO_Pin_12 );
//    DelayMs( 1 );
//    GPIOA_ResetBits( GPIO_Pin_12 );
//    i = SPI0_MasterRecvByte();
//    GPIOA_SetBits( GPIO_Pin_12 );
//    DelayMs( 2 );
    
//// FIFO 连续发送 
//    GPIOA_ResetBits( GPIO_Pin_12 ); 
//    SPI0_MasterTrans( spiBuff, 9 );
//    GPIOA_SetBits( GPIO_Pin_12 );
//    DelayMs( 1 );
//    GPIOA_ResetBits( GPIO_Pin_12 ); 
//    SPI0_MasterRecv( spiBuffrev, 12 );
//    GPIOA_SetBits( GPIO_Pin_12 );
//    DelayMs( 1 );
//    
//// DMA 连续发送 
    GPIOA_ResetBits( GPIO_Pin_12 ); 
    SPI0_MasterDMATrans(spiBuff, 12);
		while(1);
    GPIOA_SetBits( GPIO_Pin_12 );
    DelayMs( 1 );
    GPIOA_ResetBits( GPIO_Pin_12 ); 
    SPI0_MasterDMARecv(spiBuffrev, 12);
    GPIOA_SetBits( GPIO_Pin_12 );

    PRINT( "END ...\n");
    while(1)
		{
			PRINT( "END ...\n");
			 DelayMs( 500 );
		}
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





