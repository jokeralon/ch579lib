#ifndef __MYKEYBOARD_H
#define __MYKEYBOARD_H

#include "CONFIG.h"
#include "CH57x_common.h"

#define BLE_MODE		1
#define USB_MODE		!BLE_MODE

#define KEYBOARD_SCANF_EVENT	0xee

#define GPIO_PortA		0
#define GPIO_PortB		1

#define ROW_NUM				5
#define COL_NUM				5

#define GPIO_HIGH			1
#define GPIO_LOW			0
#define GPIO_NONE			2

#define EC11_SW				0xff
#define MODE_SW				0xf2
//COL ÁÐ
//ROW ÐÐ

#define ROW0_PIN			GPIO_Pin_7
#define ROW0_PORT			GPIO_PortB
#define ROW1_PIN			GPIO_Pin_16
#define ROW1_PORT			GPIO_PortB
#define ROW2_PIN			GPIO_Pin_17
#define ROW2_PORT			GPIO_PortB
#define ROW3_PIN			GPIO_Pin_8
#define ROW3_PORT			GPIO_PortB
#define ROW4_PIN			GPIO_Pin_9
#define ROW4_PORT			GPIO_PortB

#define COL0_PIN			GPIO_Pin_5
#define COL0_PORT			GPIO_PortA
#define COL1_PIN			GPIO_Pin_13
#define COL1_PORT			GPIO_PortB
#define COL2_PIN			GPIO_Pin_12
#define COL2_PORT			GPIO_PortB
#define COL3_PIN			GPIO_Pin_19
#define COL3_PORT			GPIO_PortB
#define COL4_PIN			GPIO_Pin_18
#define COL4_PORT			GPIO_PortB

void Keyboard_Init(void);

void keyBoard_GPIO_Init(void);
void keyBoard_Scanf( void );
void keyBoard_GPIO_SetPin( uint8 port, uint32 pin, uint8 status);
uint8 keyBoard_GPIO_ReadPin( uint8 port, uint32 pin);
uint8 keyBoard_SendValue( void );
#endif
