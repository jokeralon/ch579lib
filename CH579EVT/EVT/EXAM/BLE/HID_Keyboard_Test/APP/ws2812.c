#include "ws2812.h"

void RGB_GPIO_Init(void)
{
	if( WS2812_PORT )
	{
		GPIOB_ModeCfg(WS2812_PIN, GPIO_ModeOut_PP_5mA);
	}
	else
	{
		GPIOA_ModeCfg(WS2812_PIN, GPIO_ModeOut_PP_5mA);
	}
	
}

void RGB_LED_Write0(void)
{
//	RGB_LED_HIGH;
	
	GPIOA_SetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();
//	RGB_LED_LOW;
	GPIOA_ResetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}

void RGB_LED_Write1(void)
{
//	RGB_LED_HIGH;
	GPIOA_SetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
//	RGB_LED_LOW;
	GPIOA_ResetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();__nop();__nop();__nop();
}

void RGB_LED_Reset(void)
{
//	RGB_LED_LOW;
	GPIOA_ResetBits( GPIO_Pin_14 );
	GPIOA_ModeCfg( GPIO_Pin_14, GPIO_ModeOut_PP_5mA );
	mDelayuS(80);
}

void RGB_LED_Write_24Bits(uint8 g, uint8 r, uint8 b)
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		if(g & 0x80 )
			RGB_LED_Write1();
		else
			RGB_LED_Write0();
		
		g<<=1;
	}
	
	for(i=0;i<8;i++)
	{
		if(r & 0x80 )
			RGB_LED_Write1();
		else
			RGB_LED_Write0();
		
		r<<=1;
	}
	
	for(i=0;i<8;i++)
	{
		if(b & 0x80 )
			RGB_LED_Write1();
		else
			RGB_LED_Write0();
		
		b<<=1;
	}
	
}



void RGB_LED_Red(void)
{
	 uint8_t i;
	//4个LED全彩灯
	for(i=0;i<24;i++)
	{
	  RGB_LED_Write_24Bits(0x00, 0xff, 0x00);
	}
	RGB_LED_Reset();
}


void RGB_LED_Green(void)
{
	 uint8_t i;
	//4个LED全彩灯
	for(i=0;i<24;i++)
	{
	  RGB_LED_Write_24Bits(0xff, 0x00, 0x00);
	}
	RGB_LED_Reset();
}

void RGB_LED_Blue(void)
{
	 uint8_t i;
	//4个LED全彩灯
	for(i=0;i<24;i++)
	{
	  RGB_LED_Write_24Bits(0x00, 0x00, 0xff);
	}
	RGB_LED_Reset();
}
