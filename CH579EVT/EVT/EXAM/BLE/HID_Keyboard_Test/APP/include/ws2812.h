#ifndef __WS2812_H
#define __WS2812_H

#include "CONFIG.h"
#include "CH57x_common.h"

#define GPIO_PortA		0
#define GPIO_PortB		1

#define WS2812_PIN			GPIO_Pin_14
#define WS2812_PORT			GPIO_PortA

void RGB_GPIO_Init(void);
void RGB_LED_Red(void);
void RGB_LED_Green(void);
void RGB_LED_Blue(void);

#endif
