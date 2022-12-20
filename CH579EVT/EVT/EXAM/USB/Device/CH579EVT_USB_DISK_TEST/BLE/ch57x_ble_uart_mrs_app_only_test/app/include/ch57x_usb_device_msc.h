#ifndef _U_DISK_H
#define _U_DISK_H

//#include "Headfile.h"
#include "CH57x_common.h"
#include "spi_flash.h"

// #define CODE_FLASH
#define EXTERNAL_FLASH    //W25Q64 -- 8MByte

#define DevEP0SIZE	0x40

//修改恒定义的时候注意修改擦除函数，当前为擦除4k函数。需要相应修改这个函数即可。

#ifdef CODE_FLASH
#define DISK_SEC_NUM   200       //总扇区数  共100物理扇区
#define DISK_SEC_LAST  DISK_SEC_NUM-1   //最后一个逻辑扇区地址
#define DISK_SEC_LEN   0x00000200       //扇区大小  每个扇区512字节大小
#endif

#ifdef EXTERNAL_FLASH
// #define DISK_SEC_NUM   1538              //6MB       //总扇区数  共100物理扇区
#define DISK_SEC_NUM   2048              //8MB       //总扇区数  共100物理扇区

#define DISK_SEC_LAST  DISK_SEC_NUM-1   //最后一个逻辑扇区地址
#define DISK_SEC_LEN   0x00001000       //扇区大小  每个扇区4096字节大小
#endif
 
/*SCSI（Small Computer System Interface）的指令集*/
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


extern UINT8 EP0_Databuf[64] __attribute__ ((aligned (4)));	//ep0(64)
extern UINT8 EP1_Databuf[64+64] __attribute__ ((aligned (4)));     //ep1_out(64)+ep1_in(64)

#endif
