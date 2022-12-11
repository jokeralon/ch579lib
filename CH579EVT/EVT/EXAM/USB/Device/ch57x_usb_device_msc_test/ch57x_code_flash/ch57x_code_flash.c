/*
 * CH579 CodeFlash 最后50KB 充当U盘的存储区
 * 0x32000 ~ 0x3E7FF 
 * 
 */

#include "ch57x_code_flash.h"

/*******************************************************************************
* Function Name  : CodeFlash_WriteBuf
* Description    : CodeFlash 连续多个双字写入
* Input          : addr: 32位地址，需要4对齐  
				   pdat: 待写入数据缓存区首地址
				    len: 待写入数据字节长度
* Return         : FAILED  - 错误
				   SUCCESS - 成功
*******************************************************************************/
UINT8 CodeFlash_WriteBuf(UINT32 addr, UINT16 len,PUINT8 pdat)
{
    UINT32  add = addr + START_ADDR;
    UINT8   status = 0;	
	UINT8  i;
	UINT32 dat;
	
	//len = len + 4- len%4;
	
	add = add + 4 - add%4;  //4字节对齐
	
	if(add>0x3E7FF)  return FAILED;
	
    //EnableCodeFlash();
    for(i=0; i<len; i+=4)
    {
		dat = *(pdat+3+i);
		dat = dat<<8 | *(pdat+2+i);
		dat = dat<<8 | *(pdat+1+i);
		dat = dat<<8 | *(pdat+i);
		
		//printf("dat = %08lx \n" ,dat);
		status = FlashWriteDW( add, dat );        	
        add += 4;
    }
    //LockFlashALL();
    return ( status );
}

/*******************************************************************************
* Function Name  : CodeFlash_WriteBuf
* Description    : CodeFlash 连续多个双字读
* Input          : addr: 32位地址，需要4对齐  
				   len: 待写入数据字节长度
				   Buf: 写入的buf
*******************************************************************************/
UINT8 CodeFlash_ReadBuf(UINT32 addr, UINT16 len, PUINT8 Buf)
{
	UINT32  add = addr + START_ADDR; 
	UINT8  i;
	
	if(add>0x3E7FF)  return FAILED;
	
	//len = len + 4- len%4;
	
	add = add + 4- add%4;  //4字节对齐
	
    for(i=0; i<len; i+=4)
    {
        *(Buf+i) = *(PUINT32)add & 0xff;
		*(Buf+i+1) = *(PUINT32)add >> 8 & 0xff;
		*(Buf+i+2) = *(PUINT32)add >> 16 & 0xff;
		*(Buf+i+3) = *(PUINT32)add >> 24 & 0xff;		
        add += 4;
    }
	return SUCCESS;
}

/*******************************************************************************
* Function Name  : CodeFlash_BlockErase
* Description    : CodeFlash 块擦除，一次擦除512B
* Input          : addr: 32位地址，需要512对齐		   				
* Return         : FAILED  - 错误
				   SUCCESS - 成功
*******************************************************************************/
UINT8 CodeFlash_BlockErase(UINT32 addr)
{
    UINT8  status = 0;
	UINT32 add = addr + START_ADDR;

	if(add>0x3E7FF)  return FAILED;
	
    //EnableCodeFlash();
    status = FlashBlockErase( add );
    //LockFlashALL();
    return ( status );
}


