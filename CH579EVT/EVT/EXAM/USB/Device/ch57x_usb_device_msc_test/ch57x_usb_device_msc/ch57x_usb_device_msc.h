#ifndef _U_DISK_H
#define _U_DISK_H

//#include "Headfile.h"
#include "CH57x_common.h"


#define CODE_FLASH
//#define EXTERNAL_FLASH    //W25Q16 -- 2MByte

#define DevEP0SIZE	0x40

//�޸ĺ㶨���ʱ��ע���޸Ĳ�����������ǰΪ����4k��������Ҫ��Ӧ�޸�����������ɡ�

#ifdef CODE_FLASH
#define DISK_SEC_NUM   0x00000064       //��������  ��100��������
#define DISK_SEC_LAST  DISK_SEC_NUM-1   //���һ���߼�������ַ				  
#define DISK_SEC_LEN   0x00000200       //������С  ÿ������512�ֽڴ�С
#endif

#ifdef EXTERNAL_FLASH
#define DISK_SEC_NUM   1024              //0x00000200       //��������  ��100��������
#define DISK_SEC_LAST  DISK_SEC_NUM-1   //���һ���߼�������ַ				  
#define DISK_SEC_LEN   0x00001000       //������С  ÿ������4096�ֽڴ�С
#endif
 
/*SCSI��Small Computer System Interface����ָ�*/
#define FORMAT_UNIT 	0x04
#define INQUIRY 		0x12
#define MODE_SELECT 	0x15
#define MODE_SENSE5 	0x5A
#define MODE_SENSE 		0x1A
#define PER_RES_IN 		0x5E
#define PER_RES_OUT 	0x5F
#define PRE_OR_MED 		0x1E
#define READ 			0x28
#define READ_CAPACITY 	0x25
#define RELEASE 		0x17
#define REQUEST_SENSE 	0x03
#define RESERVE 		0x16
#define STA_STO_UNIT 	0x1B
#define SYN_CACHE 		0x35
#define TES_UNIT 		0x00
#define VERIFY 			0x2F
#define WRITE 			0x2A
#define WRITE_BUFFER 	0x3B


extern __align(4) UINT8 EP0_Databuf[64];	//ep0(64)
extern __align(4) UINT8 EP1_Databuf[64+64];     //ep1_out(64)+ep1_in(64)

#endif
