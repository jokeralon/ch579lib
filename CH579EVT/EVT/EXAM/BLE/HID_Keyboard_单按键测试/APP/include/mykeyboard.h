#ifndef __MYKEYBOARD_H
#define __MYKEYBOARD_H

#include "CONFIG.h"
#include "CH57x_common.h"

#define GPIO_PortA		0
#define GPIO_PortB		1

//COL ÁÐ
//ROW ÐÐ

#define ROW0_PIN			GPIO_Pin_7
#define COL0_PIN			GPIO_Pin_1

#define ROW0_PORT			GPIO_PortB
#define COL0_PORT			GPIO_PortB

void keyBoard_GPIO_Init(void);
uint8 keyBoard_Scanf( uint8 ROW_PORT,UINT32 ROW_PIN,uint8 COL_PORT,  UINT32 COL_PIN);

#endif
