#include "mykeyboard.h"
#include "ws2812.h"
//extern uint8 buf[HID_KEYBOARD_IN_RPT_LEN];
extern uint8 buf[8];



uint8 keyboard_mode_flag;

/* ws2812.h */
extern uint8_t rgb_r,rgb_g, rgb_b;

uint8 Keyboard_Taskid;

tmosEvents Keyboard_ProcessEvent( tmosTaskID task_id, tmosEvents events );

uint8 Keyboard_temp[COL_NUM][ROW_NUM]={0};

uint8 Keyboard_Map[COL_NUM][ROW_NUM]=
{
{0x1f, 0x00, 0x1e, 0x21, 0x24},	
{0x00, 0x00, 0x1f, 0x22, 0x25},
{0x00, 0x00, 0x20, 0x23, 0x26},
{0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, EC11_SW}
};

uint8 Keyboard_Value_Map[COL_NUM][ROW_NUM]=
{
{MODE_SW, 0x00, 0x1e, 0x21, 0x24},	
{0x00, 0x00, 0x1f, 0x22, 0x25},
{0x00, 0x00, 0x20, 0x23, 0x26},
{0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, EC11_SW}
};

uint32 Keyboard_COL_PIN[COL_NUM]=
{
	COL0_PIN	,
	COL1_PIN	,
	COL2_PIN	,
	COL3_PIN	,
	COL4_PIN	,
};
uint8 Keyboard_COL_PORT[COL_NUM]=
{
	COL0_PORT,
	COL1_PORT,
	COL2_PORT,
	COL3_PORT,
	COL4_PORT,
};

uint32 Keyboard_ROW_PIN[ROW_NUM]=
{
	ROW0_PIN	,
	ROW1_PIN	,
	ROW2_PIN	,
	ROW3_PIN	,
	ROW4_PIN	,
};
uint8 Keyboard_ROW_PORT[ROW_NUM]=
{
	ROW0_PORT,
	ROW1_PORT,
	ROW2_PORT,
	ROW3_PORT,
	ROW4_PORT,
};

void keyBoard_GPIO_SetMode( uint8 port, uint32 pin, GPIOModeTypeDef  Mode)
{
	if( port )
	{
		GPIOB_ModeCfg(pin, Mode);
	}
	else
	{
		GPIOA_ModeCfg(pin, Mode);
	}
}


uint8 keyBoard_GPIO_ReadPin( uint8 port, uint32 pin)
{
	if( port )
	{
		if (GPIOB_ReadPortPin(pin) == 0)
			return 0;
		else
			return 1;
	}
	else
	{
		if (GPIOA_ReadPortPin(pin) == 0)
			return 0;
		else
			return 1;
	}
}

void keyBoard_GPIO_SetPin( uint8 port, uint32 pin, uint8 status)
{
	if( port )
	{
		if(status == GPIO_HIGH)
			GPIOB_SetBits(pin);
		else
			GPIOB_ResetBits(pin);
	}
	else
	{
		if(status == GPIO_HIGH)
			GPIOA_SetBits(pin);
		else
			GPIOA_ResetBits(pin);
	}
}

void keyBoard_GPIO_Init()
{
//	keyBoard_GPIO_SetMode(COL0_PORT, COL0_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(COL1_PORT, COL1_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(COL2_PORT, COL2_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(COL3_PORT, COL3_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(COL4_PORT, COL4_PIN, GPIO_ModeOut_PP_5mA);
//	
//	
//	keyBoard_GPIO_SetMode(ROW0_PORT, ROW0_PIN, GPIO_ModeIN_PU);
//	keyBoard_GPIO_SetMode(ROW1_PORT, ROW1_PIN, GPIO_ModeIN_PU);
//	keyBoard_GPIO_SetMode(ROW2_PORT, ROW2_PIN, GPIO_ModeIN_PU);
//	keyBoard_GPIO_SetMode(ROW3_PORT, ROW3_PIN, GPIO_ModeIN_PU);
//	keyBoard_GPIO_SetMode(ROW4_PORT, ROW4_PIN, GPIO_ModeIN_PU);
	GPIOB_ModeCfg( GPIO_Pin_7, GPIO_ModeOut_PP_5mA );
	GPIOA_ModeCfg( GPIO_Pin_5, GPIO_ModeOut_PP_5mA );
	GPIOA_ResetBits(GPIO_Pin_5);
	
	GPIOA_ModeCfg( GPIO_Pin_5, GPIO_ModeIN_PD );
	
	rgb_r = 0x00; rgb_g = 0x00; rgb_b = 0x10;
}

	

void keyBoard_Scanf( void )
{
//	uint8 col_scanf_num = 0, row_scanf_num = 0;
//	for(col_scanf_num = 0; col_scanf_num< COL_NUM; col_scanf_num++)
//	{
//		keyBoard_GPIO_SetPin( Keyboard_COL_PORT[col_scanf_num], Keyboard_COL_PIN[col_scanf_num], GPIO_LOW );
//		for(row_scanf_num = 0; row_scanf_num< ROW_NUM; row_scanf_num++)
//		{
//			if( Keyboard_Map[col_scanf_num][row_scanf_num] != 0)
//			{
//	//			keyBoard_GPIO_Set( Keyboard_ROW_PORT[row_scanf_num], Keyboard_ROW_PIN[row_scanf_num], GPIO_ModeIN_PD, GPIO_NONE);
//				if( keyBoard_GPIO_ReadPin( Keyboard_ROW_PORT[row_scanf_num], Keyboard_ROW_PIN[row_scanf_num]) == 0)
//					Keyboard_temp[ col_scanf_num ][ row_scanf_num ] = 1;
//				else
//					Keyboard_temp[ col_scanf_num ][ row_scanf_num ] = 0;
//			}else
//				Keyboard_temp[ col_scanf_num ][ row_scanf_num ] = 0;
//		}
//		keyBoard_GPIO_SetPin( Keyboard_COL_PORT[col_scanf_num], Keyboard_COL_PIN[col_scanf_num], GPIO_HIGH );
//	}
}


uint8 keyBoard_SendValue( void )
{
	uint8 col_scanf_num = 0, row_scanf_num = 0, i=2;
	tmos_memset(buf, 0x00, sizeof(buf));
	
	tmos_memset(Keyboard_temp, 0x00, sizeof(Keyboard_temp));
	
//	keyBoard_Scanf();
//	
//	for(col_scanf_num = 0; col_scanf_num< COL_NUM; col_scanf_num++)
//	{
//		for(row_scanf_num = 0; row_scanf_num< ROW_NUM; row_scanf_num++)
//		{
//			if( Keyboard_temp[ col_scanf_num ][ row_scanf_num ] == 1)
//			{
//				/* 特殊键处理 */
//				if( Keyboard_Value_Map[ col_scanf_num ][ row_scanf_num ]  == EC11_SW )
//					rgb_r = 0;
//				else if( Keyboard_Value_Map[ col_scanf_num ][ row_scanf_num ] == MODE_SW )
//				{
//					keyboard_mode_flag = !keyboard_mode_flag;
//				}
//					
//				else
//				{
//					if(i>7)
//						return 1;
//					/* 普通键处理 */
//					buf[i] = Keyboard_Value_Map[ col_scanf_num ][ row_scanf_num ];
//					i++;
//				}
//			}
//			mDelayuS(1);
//		}
//	}
//	return 0;
	
}



void Keyboard_Init(void)
{
	/* 注册任务 */ 
	Keyboard_Taskid = TMOS_ProcessEventRegister(Keyboard_ProcessEvent);
	
	/* RGB GPIO初始化 */ 
	keyBoard_GPIO_Init();
	
	/* 开始 WS2812 任务 */ 
	tmos_start_task( Keyboard_Taskid , KEYBOARD_SCANF_EVENT ,1600 );
}



tmosEvents Keyboard_ProcessEvent( tmosTaskID task_id, tmosEvents events )
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
  if( events & KEYBOARD_SCANF_EVENT ){
		
		keyBoard_SendValue();
		
//		if( keyboard_mode_flag == BLE_MODE )
//		{
//			rgb_r = 0x00; rgb_g = 0x00; rgb_b = 0x10;
//		}
//		else
//		{
//			rgb_r = 0x00; rgb_g = 0x10; rgb_b = 0x00;
//		}
		
    tmos_start_task( Keyboard_Taskid , KEYBOARD_SCANF_EVENT ,64 ); //  625us / run  1000ms
		return events ^ KEYBOARD_SCANF_EVENT;
  }
  return 0;
}



