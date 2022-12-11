/*
 * CH579 CodeFlash ���50KB �䵱U�̵Ĵ洢��
 * 0x32000 ~ 0x3E7FF 
 * 
 */

#include "ch57x_code_flash.h"

/*******************************************************************************
* Function Name  : CodeFlash_WriteBuf
* Description    : CodeFlash �������˫��д��
* Input          : addr: 32λ��ַ����Ҫ4����  
				   pdat: ��д�����ݻ������׵�ַ
				    len: ��д�������ֽڳ���
* Return         : FAILED  - ����
				   SUCCESS - �ɹ�
*******************************************************************************/
UINT8 CodeFlash_WriteBuf(UINT32 addr, UINT16 len,PUINT8 pdat)
{
    UINT32  add = addr + START_ADDR;
    UINT8   status = 0;	
	UINT8  i;
	UINT32 dat;
	
	//len = len + 4- len%4;
	
	add = add + 4 - add%4;  //4�ֽڶ���
	
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
* Description    : CodeFlash �������˫�ֶ�
* Input          : addr: 32λ��ַ����Ҫ4����  
				   len: ��д�������ֽڳ���
				   Buf: д���buf
*******************************************************************************/
UINT8 CodeFlash_ReadBuf(UINT32 addr, UINT16 len, PUINT8 Buf)
{
	UINT32  add = addr + START_ADDR; 
	UINT8  i;
	
	if(add>0x3E7FF)  return FAILED;
	
	//len = len + 4- len%4;
	
	add = add + 4- add%4;  //4�ֽڶ���
	
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
* Description    : CodeFlash �������һ�β���512B
* Input          : addr: 32λ��ַ����Ҫ512����		   				
* Return         : FAILED  - ����
				   SUCCESS - �ɹ�
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


