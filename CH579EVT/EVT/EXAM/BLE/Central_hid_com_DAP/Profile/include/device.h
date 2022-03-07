#ifndef __DEVICE_H
#define __DEVICE_H

#define __STATIC_FORCEINLINE	static __forceinline
#include "stdint.h"
#include "cmsis_compiler.h"

#include "CONFIG.h"
#include "CH57x_common.h"

#include "log.h"

#define GPIOA		0
#define GPIOB		1


#define LED_RUNNIN_Pin			GPIO_Pin_6
#define LED_CONNECTED_Pin   GPIO_Pin_1
#define LED_GPIO_Port       GPIOA


/**
\defgroup DAP_Config_PortIO_gr CMSIS-DAP Hardware I/O Pin Access
\ingroup DAP_ConfigIO_gr
@{

Standard I/O Pins of the CMSIS-DAP Hardware Debug Port support standard JTAG mode
and Serial Wire Debug (SWD) mode. In SWD mode only 2 pins are required to implement the debug
interface of a device. The following I/O Pins are provided:

JTAG I/O Pin                 | SWD I/O Pin          | CMSIS-DAP Hardware pin mode
---------------------------- | -------------------- | ---------------------------------------------
TCK: Test Clock              | SWCLK: Clock         | Output Push/Pull
TMS: Test Mode Select        | SWDIO: Data I/O      | Output Push/Pull; Input (for receiving data)
TDI: Test Data Input         |                      | Output Push/Pull
TDO: Test Data Output        |                      | Input
nTRST: Test Reset (optional) |                      | Output Open Drain with pull-up resistor
nRESET: Device Reset         | nRESET: Device Reset | Output Open Drain with pull-up resistor


DAP Hardware I/O Pin Access Functions
-------------------------------------
The various I/O Pins are accessed by functions that implement the Read, Write, Set, or Clear to
these I/O Pins.

For the SWDIO I/O Pin there are additional functions that are called in SWD I/O mode only.
This functions are provided to achieve faster I/O that is possible with some advanced GPIO
peripherals that can independently write/read a single I/O pin without affecting any other pins
of the same I/O port. The following SWDIO I/O Pin functions are provided:
 - \ref PIN_SWDIO_OUT_ENABLE to enable the output mode from the DAP hardware.
 - \ref PIN_SWDIO_OUT_DISABLE to enable the input mode to the DAP hardware.
 - \ref PIN_SWDIO_IN to read from the SWDIO I/O pin with utmost possible speed.
 - \ref PIN_SWDIO_OUT to write to the SWDIO I/O pin with utmost possible speed.
*/

#define JTAG_TCK_Pin 	GPIO_Pin_0
#define JTAG_TCK_GPIO_Port GPIOA

#define JTAG_TMS_Pin GPIO_Pin_2
#define JTAG_TMS_GPIO_Port GPIOA

#define JTAG_nRESET_Pin GPIO_Pin_15
#define JTAG_nRESET_GPIO_Port GPIOA

#define JTAG_TDI_Pin GPIO_Pin_7
#define JTAG_TDI_GPIO_Port GPIOA

#define JTAG_TDO_Pin GPIO_Pin_11
#define JTAG_TDO_GPIO_Port GPIOA

#define JTAG_nTRST_Pin GPIO_Pin_13
#define JTAG_nTRST_GPIO_Port GPIOA

#define GPIO_SetPin(a, b);		 	{if(a==GPIOA){GPIOA_SetBits(b);}else{GPIOB_SetBits(b);}}
#define GPIO_ResetPin(a,b);			{if(a==GPIOA){GPIOA_ResetBits(b);}else{GPIOB_ResetBits(b);}}
#define GPIO_SetMode(a,b,c);		{if(a==GPIOA){GPIOA_ModeCfg(b, c);}else{GPIOB_ModeCfg(b, c);}}

void USBD_Initialize(int n);

void USBD_Connect(int);

void USBD_HID0_Initialize (void);

void DAP_LED_Init(void);
#endif
