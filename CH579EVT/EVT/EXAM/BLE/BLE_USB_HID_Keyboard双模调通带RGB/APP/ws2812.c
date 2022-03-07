#include "ws2812.h"

uint8_t WS2812_Taskid = 0;
tmosEvents WS2812_ProcessEvent( tmosTaskID task_id, tmosEvents events );
uint8_t rgb_color[3];
uint8_t rgb_r=0,rgb_g=0, rgb_b=0;

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
	
	GPIOB_SetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();
//	RGB_LED_LOW;
	GPIOB_ResetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}

void RGB_LED_Write1(void)
{
//	RGB_LED_HIGH;
	GPIOB_SetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
//	RGB_LED_LOW;
	GPIOB_ResetBits( GPIO_Pin_14 );
	__nop();__nop();__nop();__nop();__nop();__nop();
}

void RGB_LED_Reset(void)
{
//	RGB_LED_LOW;
	GPIOB_ResetBits( GPIO_Pin_14 );
	GPIOB_ModeCfg( GPIO_Pin_14, GPIO_ModeOut_PP_5mA );
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

void RGB_24Bit(uint32 num, uint8 r, uint8 g, uint8 b)
{
	uint32 i;
	for( i=0;i<num;i++ )
	{
		RGB_LED_Write_24Bits(g, r, b);
	}
	RGB_LED_Reset();
}

/*******************************************************************************
 * @fn      PowerManage_Init
 *
 * @brief   电源管理服务初始化
 *
 * @param   void 
 *
 * @return  none
 *******************************************************************************/
void WS2812_Init(void)
{
	/* 注册任务 */ 
	WS2812_Taskid = TMOS_ProcessEventRegister(WS2812_ProcessEvent);
	
	/* RGB GPIO初始化 */ 
	RGB_GPIO_Init();
	
	/* 开始 WS2812 任务 */ 
	tmos_start_task( WS2812_Taskid , WS2812_EVENT_START ,1600 );
}



tmosEvents WS2812_ProcessEvent( tmosTaskID task_id, tmosEvents events )
{
	uint8 * msgPtr;

  if( events & SYS_EVENT_MSG ){  // 处理HAL层消息，调用tmos_msg_receive读取消息，处理完成后删除消息。
		msgPtr = tmos_msg_receive(task_id);
    if( msgPtr ){
      /* De-allocate */
      tmos_msg_deallocate( msgPtr );
    }
    return events ^ SYS_EVENT_MSG;
	}
  if( events & WS2812_EVENT_START ){
		
		RGB_24Bit(WS2812_NUM , rgb_r, rgb_g, rgb_b);
		
    tmos_start_task( WS2812_Taskid , WS2812_EVENT_START ,400 ); //  625us / run  1000ms
		return events ^ WS2812_EVENT_START;
  }
  return 0;
}
