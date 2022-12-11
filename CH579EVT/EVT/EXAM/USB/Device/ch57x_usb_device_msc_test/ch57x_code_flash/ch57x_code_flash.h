#ifndef  _CODE_FLASH_H
#define  _CODE_FLASH_H

//#include "Headfile.h"
#include "CH57x_common.h"


#define START_ADDR  0x32000


extern UINT8 CodeFlash_WriteBuf(UINT32 addr, UINT16 len,PUINT8 pdat);
extern UINT8 CodeFlash_ReadBuf(UINT32 addr, UINT16 len, PUINT8 Buf);
extern UINT8 CodeFlash_BlockErase(UINT32 addr);
#endif
