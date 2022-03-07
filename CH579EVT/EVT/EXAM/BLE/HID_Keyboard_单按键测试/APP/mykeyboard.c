#include "mykeyboard.h"

void keyBoard_GPIO_Init()
{
	if( ROW0_PORT )
	{
		GPIOB_ModeCfg(ROW0_PIN, GPIO_ModeIN_PU);
//		GPIOB_SetBits(ROW0_PIN);
	}
	else
	{
		GPIOA_ModeCfg(ROW0_PIN, GPIO_ModeIN_PU);
//		GPIOA_SetBits(ROW0_PIN);
	}
	
	if( COL0_PORT )
	{
		GPIOB_ModeCfg(COL0_PIN, GPIO_ModeOut_PP_5mA);
		GPIOA_ResetBits(COL0_PIN);
	}
	else
	{
		GPIOA_ModeCfg(COL0_PIN, GPIO_ModeOut_PP_5mA);
		GPIOA_ResetBits(COL0_PIN);
	}
}
	

uint8 keyBoard_Scanf( uint8 ROW_PORT,UINT32 ROW_PIN,uint8 COL_PORT,  UINT32 COL_PIN)
{
	if( COL_PORT )
		GPIOB_SetBits(COL_PIN);
	
	if( ROW_PORT )
		return GPIOB_ReadPortPin( ROW_PIN );
	else
		return GPIOA_ReadPortPin( ROW_PIN );
}

