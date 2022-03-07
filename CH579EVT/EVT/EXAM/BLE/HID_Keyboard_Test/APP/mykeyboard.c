#include "mykeyboard.h"
#include "ws2812.h"

//extern uint8 buf[HID_KEYBOARD_IN_RPT_LEN];
extern uint8 buf[8];

uint8 Keyboard_temp[COL_NUM][ROW_NUM]={0};

uint8 Keyboard_Map[COL_NUM][ROW_NUM]=
{
{0x00, 0x00, 0x1e, 0x21, 0x24},	
{0x00, 0x00, 0x1f, 0x22, 0x25},
{0x1e, 0x1f, 0x20, 0x23, 0x26},
{0x00, 0x00, 0x00, 0x00, 0x00},	
{0x00, 0x00, 0x00, 0x00, 0xff},
};

uint8 Keyboard_Value_Map[COL_NUM][ROW_NUM]=
{
{0x00, 0x00, 0x1e, 0x21, 0x24},	
{0x00, 0x00, 0x1f, 0x22, 0x25},
{0x1e, 0x1f, 0x20, 0x23, 0x26},
{0x00, 0x00, 0x00, 0x00, 0x00},	
{0x00, 0x00, 0x00, 0x00, 0xff},
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
	keyBoard_GPIO_SetMode(COL0_PORT, COL0_PIN, GPIO_ModeOut_PP_5mA);
	keyBoard_GPIO_SetMode(COL1_PORT, COL1_PIN, GPIO_ModeOut_PP_5mA);
	keyBoard_GPIO_SetMode(COL2_PORT, COL2_PIN, GPIO_ModeOut_PP_5mA);
	keyBoard_GPIO_SetMode(COL3_PORT, COL3_PIN, GPIO_ModeOut_PP_5mA);
	keyBoard_GPIO_SetMode(COL4_PORT, COL4_PIN, GPIO_ModeOut_PP_5mA);
	
	
	keyBoard_GPIO_SetMode(ROW0_PORT, ROW0_PIN, GPIO_ModeIN_PD);
	keyBoard_GPIO_SetMode(ROW1_PORT, ROW1_PIN, GPIO_ModeIN_PD);
	keyBoard_GPIO_SetMode(ROW2_PORT, ROW2_PIN, GPIO_ModeIN_PD);
	keyBoard_GPIO_SetMode(ROW3_PORT, ROW3_PIN, GPIO_ModeIN_PD);
	keyBoard_GPIO_SetMode(ROW4_PORT, ROW4_PIN, GPIO_ModeIN_PD);
	
	
//	keyBoard_GPIO_SetMode(ROW0_PORT, ROW0_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(ROW1_PORT, ROW1_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(ROW2_PORT, ROW2_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(ROW3_PORT, ROW3_PIN, GPIO_ModeOut_PP_5mA);
//	keyBoard_GPIO_SetMode(ROW4_PORT, ROW4_PIN, GPIO_ModeOut_PP_5mA);
//	
//	keyBoard_GPIO_SetPin(COL0_PORT, COL0_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(COL1_PORT, COL1_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(COL2_PORT, COL2_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(COL3_PORT, COL3_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(COL4_PORT, COL4_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(ROW0_PORT, ROW0_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(ROW1_PORT, ROW1_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(ROW2_PORT, ROW2_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(ROW3_PORT, ROW3_PIN, GPIO_LOW);
//	keyBoard_GPIO_SetPin(ROW4_PORT, ROW4_PIN, GPIO_LOW);
}

	

void keyBoard_Scanf( void )
{
	uint8 col_scanf_num = 0, row_scanf_num = 0;
	keyBoard_GPIO_Init();
	for(col_scanf_num = 0; col_scanf_num< COL_NUM; col_scanf_num++)
	{
		keyBoard_GPIO_SetPin( Keyboard_COL_PORT[col_scanf_num], Keyboard_COL_PIN[col_scanf_num], GPIO_HIGH );
		
		for(row_scanf_num = 0; row_scanf_num< ROW_NUM; row_scanf_num++)
		{
			if( Keyboard_Map[col_scanf_num][row_scanf_num] != 0)
			{
				if( keyBoard_GPIO_ReadPin( Keyboard_ROW_PORT[row_scanf_num], Keyboard_ROW_PIN[row_scanf_num]) == 1)
					Keyboard_temp[ col_scanf_num ][ row_scanf_num ] = 1;
				else
					Keyboard_temp[ col_scanf_num ][ row_scanf_num ] = 0;
			}else
				Keyboard_temp[ col_scanf_num ][ row_scanf_num ] = 0;
		}
		keyBoard_GPIO_SetPin( Keyboard_COL_PORT[col_scanf_num], Keyboard_COL_PIN[col_scanf_num], GPIO_LOW );
	}
}


void keyBoard_SendValue( void )
{
	uint8 col_scanf_num = 0, row_scanf_num = 0, i=2;
	
	
	tmos_memset(buf, 0x00, sizeof(buf));
	
	tmos_memset(Keyboard_temp, 0x00, sizeof(Keyboard_temp));
	
	
	keyBoard_Scanf();
	
	for(col_scanf_num = 0; col_scanf_num< COL_NUM; col_scanf_num++)
	{
		for(row_scanf_num = 0; row_scanf_num< ROW_NUM; row_scanf_num++)
		{
			if( Keyboard_temp[ col_scanf_num ][ row_scanf_num ] == 1)
			{
				/* 特殊按键处理 */
				if( Keyboard_Value_Map[ col_scanf_num ][ row_scanf_num ] == 0xff )  //RGB
					RGB_LED_Blue();
				else
				{
					/* 普通按键处理 */
					buf[i] = Keyboard_Value_Map[ col_scanf_num ][ row_scanf_num ];
					i++;
				}
			}
		}
	}
}

