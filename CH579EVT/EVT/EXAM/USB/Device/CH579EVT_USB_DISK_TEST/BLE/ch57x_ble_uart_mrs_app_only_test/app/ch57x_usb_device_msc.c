#include "ch57x_usb_device_msc.h"

#include "ch57x_code_flash.h"

#include "app_flash.h"


/*
    ---------------------- Device Descriptor ----------------------
bLength                  : 0x12 (18 bytes)
bDescriptorType          : 0x01 (Device Descriptor)
bcdUSB                   : 0x110 (USB Version 1.1)
bDeviceClass             : 0x00 (defined by the interface descriptors)
bDeviceSubClass          : 0x00
bDeviceProtocol          : 0x00
bMaxPacketSize0          : 0x40 (64 bytes)
idVendor                 : 0x3344 (VIRPIL, UAB)
idProduct                : 0x3533
bcdDevice                : 0x0100
iManufacturer            : 0x01 (String Descriptor 1)
 Language 0x0409         : "wch.cn"
iProduct                 : 0x02 (String Descriptor 2)
 Language 0x0409         : "CH57x"
iSerialNumber            : 0x00 (No String Descriptor)
bNumConfigurations       : 0x01 (1 Configuration)
Data (HexDump)           : 12 01 10 01 00 00 00 40 44 33 33 35 00 01 01 02   .......@D335....
                           00 01                                             ..
*/

// 设备描述符
const UINT8  MyDevDescr[] = 
{
	0x12, 0x01, 0x10, 0x01,
    0x00, 0x00, 0x00, DevEP0SIZE,
    0x44, 0x33, 0x33, 0x35,   // 厂商ID和产品ID
    0x00, 0x01, 0x01, 0x02,
    0x00, 0x01
};

/*
    ------------------ Configuration Descriptor -------------------
bLength                  : 0x09 (9 bytes)
bDescriptorType          : 0x02 (Configuration Descriptor)
wTotalLength             : 0x0020 (32 bytes)
bNumInterfaces           : 0x01 (1 Interface)
bConfigurationValue      : 0x01 (Configuration 1)
iConfiguration           : 0x00 (No String Descriptor)
bmAttributes             : 0x80
 D7: Reserved, set 1     : 0x01
 D6: Self Powered        : 0x00 (no)
 D5: Remote Wakeup       : 0x00 (no)
 D4..0: Reserved, set 0  : 0x00
MaxPower                 : 0x32 (100 mA)
Data (HexDump)           : 09 02 20 00 01 01 00 80 32 09 04 00 00 02 08 06   .. .....2.......
                           50 00 07 05 01 02 40 00 00 07 05 81 02 40 00 00   P.....@......@..
*/

// 配置描述符
const UINT8  MyCfgDescr[] =  
{
	0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32,
    0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,                     
    0x07, 0x05, 0x01, 0x02, 0x40, 0x00, 0x00,
    0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00
};

/*
             ------ String Descriptor 0 ------
bLength                  : 0x04 (4 bytes)
bDescriptorType          : 0x03 (String Descriptor)
Language ID[0]           : 0x0409 (English - United States)
Data (HexDump)           : 04 03 09 04                                       ....
*/
// 语言描述符
const UINT8  MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };
/*
             ------ String Descriptor 1 ------
bLength                  : 0x0E (14 bytes)
bDescriptorType          : 0x03 (String Descriptor)
Language 0x0409          : "wch.cn"
Data (HexDump)           : 0E 03 77 00 63 00 68 00 2E 00 63 00 6E 00         ..w.c.h...c.n.
*/
// 厂家信息
const UINT8  MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
/*
             ------ String Descriptor 2 ------
bLength                  : 0x0C (12 bytes)
bDescriptorType          : 0x03 (String Descriptor)
Language 0x0409          : "CH57x"
Data (HexDump)           : 0C 03 43 00 48 00 35 00 37 00 78 00               ..C.H.5.7.x.
*/
// 产品信息
const UINT8  MyProdInfo[] = { 0x0C, 0x03, 'C', 0, 'H', 0, '5', 0, '7', 0, 'x', 0 };

const UINT8 DBINQUITY[] =
{
	0x00,           //Peripheral Device Type
	0x80, 			//
	0x02 ,			//ISO/ECMA
	0x02 ,			//
	0x1f ,			//Additional Length
	
	00 ,			//Reserved
	00 ,			//Reserved
	00 ,			//Reserved
	
	'w' ,			//Vendor Information
	'c' ,			//
	'h' ,			//
	'.' ,			//
	'c' ,			//
	'n' ,			//
	' ' ,			//
	' ' ,			//
	
	0xc7,			//Product Identification  
	0xdf, 			//
	0xba,			//
	0xe3,			//
	0xb5,			//
	0xe7,			//
	0xd7,			//
	0xd3,			//
	0x55,			//
	0xc5,			//
	0xcc,			//
	0xb7,			//
	0xbd,			//
	0xb0,			//
	0xb8,			//
	0x00,           //
	
	'1' ,			//Product Revision Level
	'.' ,			//
	'1' ,			//
	'0'  			//
};

const UINT8 DBCAPACITY[] = 
{ 
	( (DISK_SEC_LAST) >>24)&0xFF, ( (DISK_SEC_LAST) >>16)&0xFF, ( (DISK_SEC_LAST) >>8)&0xFF, DISK_SEC_LAST&0xFF , 
	(DISK_SEC_LEN>>24)&0xFF, (DISK_SEC_LEN>>16)&0xFF, (DISK_SEC_LEN>>8)&0xFF, DISK_SEC_LEN&0xFF
};    //last logic addr//block lenth

const UINT8  modesense3F[]=
{
	0x0b, 0x00, 0x00, 0x08, 
	(DISK_SEC_NUM>>24)&0xFF, (DISK_SEC_NUM>>16)&0xFF, (DISK_SEC_NUM>>8)&0xFF, DISK_SEC_NUM&0xFF, 
	00, 00, 02, 00 
};   //物理扇区数

const UINT8  mode5sense3F[]=
{
	0x00, 0x06, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08,
	(DISK_SEC_NUM>>24)&0xFF, (DISK_SEC_NUM>>16)&0xFF, (DISK_SEC_NUM>>8)&0xFF, DISK_SEC_NUM&0xFF,
	00, 00, 02, 00 
};  //物理扇区数

const UINT8 MAX_LUN[] = {0};

typedef union _CBWCB
{
	unsigned char buf1[16];
}CBWCB;

typedef union _MASS_PARA
{
	unsigned char buf[64];
	
	struct  _SENSE
	{
		unsigned char ErrorCode;
		unsigned char Reserved1;
		unsigned char SenseKey;
		unsigned char Information[4];
		unsigned char AddSenseLength;
		unsigned char Reserved2[4];
		unsigned char AddSenseCode;
		unsigned char AddSenseCodeQua;
		unsigned char Reserved3[4];
	}Sense;
	
	struct  _CBW
	{
		unsigned char dCBWsig[4];
		unsigned char dCBWTag[4];
		unsigned long dCBWDatL;
		unsigned char bmCBWFlags;
		unsigned char bCBWLUN;
		unsigned char bCBWCBLength;
		CBWCB         cbwcb;
	}cbw;
	
	struct _CSW
	{
		unsigned char buf2[13];
	}csw;
}MASS_PARA;

enum _HOST_DEV_DISAGREE 
{
	CASEOK = 0,
	CASE1,
	CASE2,
	CASE3,
	CASE4,
	CASE5,
	CASE6,
	CASE7,
	CASE8,
	CASE9,
	CASE10,
	CASE11,
	CASE12,
	CASE13,
	CASECBW,
	CASECMDFAIL
};

union 
{
	unsigned long mDataLength;	//数据长度
	unsigned char mdataLen[4];	//
} LEN;

UINT8 buffer[64]; 
UINT8 mdCBWTag[4];		//dCBWTag
MASS_PARA  MassPara;
UINT8 CH375BULKUP;				//数据上传
UINT8 CH375BULKDOWN;				//数据下传
UINT8 CH375CSW;					//CSW上传标志
UINT8 BcswStatus;		//CSW状态
UINT8 mSenseKey;
UINT8 mASC;
UINT8 FSTALL;						//数据错误标志
UINT8 lastFSTALL;
UINT8 pBuf_ReSelect = 0;
UINT32 Locate_Addr;
UINT8 *pBuf;


/**********************************************************/
UINT8   DevConfig;
UINT8   SetupReqCode;
UINT16  SetupReqLen;
const UINT8 *pDescr;

/******** 用户自定义分配端点RAM ****************************************/
UINT8 EP0_Databuf[64] __attribute__ ((aligned (4)));	//ep0(64)
UINT8 EP1_Databuf[64+64] __attribute__ ((aligned (4)));	//ep1_out(64)+ep1_in(64)


/***************************************************************/
//BLOCK ONLY  The Thirteen Cases

void BulkThirteen(unsigned char Case)
{
	switch(Case)
	{
		case CASEOK:
		case CASE1:     									/* Hn=Dn*/
		case CASE6:     									/* Hi=Di*/
			BcswStatus = 0;
			break;
		case CASE12:    									/* Ho=Do*/
			BcswStatus = 0;
			break;

		case CASE2:     									/* Hn<Di*/
		case CASE3:     									/* Hn<Do*/

			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;
															//这里上传端点设置一个STALL，待主机清掉 // may or may-not
			BcswStatus =2;
			break;

		case CASE4:     									/* Hi>Dn*/
		case CASE5:     									/* Hi>Di*/
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;										//这里上传端点设置一个STALL，待主机清掉
			BcswStatus= 1;									//CSW_GOOD or CSW_FAIL
			break;

		case CASE7:    										 /* Hi<Di*/
		case CASE8:     									/* Hi<>Do */
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;										//这里上传端点设置一个STALL，待主机清掉
			BcswStatus = 2;
			break;

		case CASE9:     									/* Ho>Dn*/
		case CASE11:    									/* Ho>Do*/
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_R_RES ;
			FSTALL=1;                                       //这里上传端点设置一个STALL，待主机清掉
			BcswStatus =1;									//CSW_GOOD or CSW_FAIL
			break;

		case CASE10:    								    /* Ho<>Di */
		case CASE13:    								    /* Ho<Do*/
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;        //这里上传端点设置一个STALL，待主机清掉
			FSTALL=1;
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_R_RES ;										
			BcswStatus = 2;
			break;

		case CASECBW:   									/* invalid CBW */
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_R_RES ;		//这里端点设置一个STALL，待主机清掉
			BcswStatus = 2;
			break;

		case CASECMDFAIL:
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;                                       //这里上传端点设置一个STALL，待主机清掉
			BcswStatus= 1;
			break;

		default:
			break;
	}
}

				
void UFI_readCapacity(void )
{	
	if ( LEN.mDataLength > sizeof(DBCAPACITY) ) LEN.mDataLength = sizeof(DBCAPACITY);
	pBuf=(unsigned char*)DBCAPACITY;	
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}

void UFI_inquiry(void )
{
	pBuf = (unsigned char*)DBINQUITY;					////查询U盘信息
	if(LEN.mDataLength>36) LEN.mDataLength=36;
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}

void UFI_read10(void)
{
	//读取数据
	LEN.mDataLength=(((UINT32)MassPara.cbw.cbwcb.buf1[7]<<8) | (UINT32)MassPara.cbw.cbwcb.buf1[8])*DISK_SEC_LEN;
	Locate_Addr = ((UINT32)MassPara.cbw.cbwcb.buf1[2]<<24) | ((UINT32)MassPara.cbw.cbwcb.buf1[3]<<16) | ((UINT32)MassPara.cbw.cbwcb.buf1[4]<<8) | (UINT32)MassPara.cbw.cbwcb.buf1[5];
	
	Locate_Addr = Locate_Addr*DISK_SEC_LEN;
	
	//printf("Read addr:%ld  len:%ld \n",Locate_Addr,LEN.mDataLength);
	
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
	
	pBuf_ReSelect = 1;
}

void UFI_modeSense(void )
{
	//模式认识
	if(MassPara.cbw.cbwcb.buf1[2]==0x3F)
	{
		if ( LEN.mDataLength > sizeof(modesense3F) ) LEN.mDataLength = sizeof(modesense3F);
		pBuf = (unsigned char*)modesense3F;
		BcswStatus=0;
		mSenseKey=0;
		mASC=0;
	}
	else 
	{
		CH375BULKUP=0;
		mSenseKey=5;
		mASC=0x20;

		BcswStatus=1;
		BulkThirteen(CASECMDFAIL);
	}
}

void UFI_requestSense(void )
{
	//请求认识
	if ( FSTALL | lastFSTALL ) 
	{
		lastFSTALL=FSTALL;
		FSTALL=0;
		MassPara.Sense.ErrorCode=0x70;
		MassPara.Sense.Reserved1=0;
		MassPara.Sense.SenseKey=mSenseKey;
		MassPara.Sense.Information[0]=0;
		MassPara.Sense.Information[1]=0;
		MassPara.Sense.Information[2]=0;
		MassPara.Sense.Information[3]=0;
		MassPara.Sense.AddSenseLength=0x0a;
		MassPara.Sense.Reserved2[0]=0;
		MassPara.Sense.Reserved2[1]=0;
		MassPara.Sense.Reserved2[2]=0;
		MassPara.Sense.Reserved2[3]=0;
		MassPara.Sense.AddSenseCode=mASC;
		MassPara.Sense.AddSenseCodeQua=00;
		MassPara.Sense.Reserved3[0]=0;
		MassPara.Sense.Reserved3[1]=0;
		MassPara.Sense.Reserved3[2]=0;
		MassPara.Sense.Reserved3[3]=0;
		pBuf=MassPara.buf;
		BcswStatus=0;
	}
	else
	{
		lastFSTALL=FSTALL;
		FSTALL=0;
		MassPara.Sense.ErrorCode=0x70;
		MassPara.Sense.Reserved1=0;
		MassPara.Sense.SenseKey=0x00;
		MassPara.Sense.Information[0]=0;
		MassPara.Sense.Information[1]=0;
		MassPara.Sense.Information[2]=0;
		MassPara.Sense.Information[3]=0;
		MassPara.Sense.AddSenseLength=0x0a;
		MassPara.Sense.Reserved2[0]=0;
		MassPara.Sense.Reserved2[1]=0;
		MassPara.Sense.Reserved2[2]=0;
		MassPara.Sense.Reserved2[3]=0;
		MassPara.Sense.AddSenseCode=0x00;
		MassPara.Sense.AddSenseCodeQua=00;
		MassPara.Sense.Reserved3[0]=0;
		MassPara.Sense.Reserved3[1]=0;
		MassPara.Sense.Reserved3[2]=0;
		MassPara.Sense.Reserved3[3]=0;
		pBuf=MassPara.buf;
		BcswStatus=0;
	}
}

void UFI_testUnit(void )
{
	CH375BULKDOWN=0;
	CH375BULKUP=0;
	BcswStatus=0;			//测试U盘是否准备好
	mSenseKey=0;
	mASC=0;
}
void UFI_perOrMed(void )   //允许移出磁盘
{				
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}

void UFI_write(void )
{
	UINT8 i,num;
	LEN.mDataLength=(((UINT32)MassPara.cbw.cbwcb.buf1[7]<<8) | (UINT32)MassPara.cbw.cbwcb.buf1[8])*DISK_SEC_LEN;		//写数据长度
	Locate_Addr = ((UINT32)MassPara.cbw.cbwcb.buf1[2]<<24) | ((UINT32)MassPara.cbw.cbwcb.buf1[3]<<16) | ((UINT32)MassPara.cbw.cbwcb.buf1[4]<<8) | (UINT32)MassPara.cbw.cbwcb.buf1[5];
	Locate_Addr = Locate_Addr*DISK_SEC_LEN;
	//擦除
	num = MassPara.cbw.cbwcb.buf1[8];   //待写扇区数
	
	#ifdef EXTERNAL_FLASH
    // const sfud_flash *flash = sfud_get_device_table() + 0;
	for(i=0;i<num;i++){
		app_flash_erase_4k_flash(Locate_Addr + i*DISK_SEC_LEN); 
        // sfud_erase(flash, Locate_Addr + i*DISK_SEC_LEN, 4096);
    }
	#endif
	 
	#ifdef CODE_FLASH
	for(i=0;i<num;i++)
	     CodeFlash_BlockErase(Locate_Addr + i*DISK_SEC_LEN);
	#endif
		
	
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}

void UFI_staStoUnit(void )  //请求装载卸载设备
{     
	CH375BULKDOWN=0;
	CH375BULKUP=0;
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}
void UFI_verify(void )
{
	BcswStatus=0;		//校验存储器空间
	mSenseKey=0;
	mASC=0;
	//这里这里只是作为演示所以没有真正检测物理存储器
	//但实际上这一步一定要处理
}

void UFI_modeSense5(void )
{
	if(MassPara.cbw.cbwcb.buf1[2]==0x3F)
	{
	    if ( LEN.mDataLength > sizeof(mode5sense3F) ) LEN.mDataLength = sizeof(mode5sense3F);
		pBuf = (unsigned char*)mode5sense3F;
		BcswStatus=0;
		mSenseKey=0;
		mASC=0;
	}
	else 
	{
		CH375BULKUP=0;
		mSenseKey=5;
		mASC=0x20;
		BcswStatus=1;
		BulkThirteen(CASECMDFAIL);
	}
}

//UFI  CMD
void UFI_Hunding(void )
{		
	switch(MassPara.cbw.cbwcb.buf1[0])
	{
		case INQUIRY:
			UFI_inquiry();
		break;
		
		case WRITE:
			//printf("%2x\n",(UINT16)MassPara.cbw.cbwcb.buf1[0]);			
			UFI_write();
		break;
		
		case TES_UNIT:
			UFI_testUnit();
		break;
		
		case READ:
			UFI_read10();
		break;
		
		case REQUEST_SENSE:
			UFI_requestSense();
		break;
		
		case READ_CAPACITY:
			UFI_readCapacity();
		break;
		
		case VERIFY:
			UFI_verify();
			break;
		
	//	case 0x23:
	// 	break;
		
	//	case MODE_SELECT:
		//	UFI_modeSlect();
	//	break;
		
		case MODE_SENSE:
			UFI_modeSense();
		break;
		
		case MODE_SENSE5:
			UFI_modeSense5();
		break;
		
	//	case WRITE_BUFFER:
	//		UFI_writeBuf();
	//		break;
	//		case PREVENT:
	//	break;
		
	//	case FORMAT_UNIT:
	//		UFI_format();
	//		break;
	//		case RELEASE:
	//		break;
		
		case STA_STO_UNIT:
			UFI_staStoUnit();
			break;
		
		case PRE_OR_MED:
			UFI_perOrMed();
			break;
		
		default:
			mSenseKey=5;
			mASC=0x20;
			BcswStatus=1;
			CH375BULKUP=0;
			BulkThirteen(CASECBW);
			break;
	}
}

void mCH375BulkOnly(void)
{
	if(MassPara.buf[0]==0x55)
	{
		if(MassPara.buf[1]==0x53)
		{
			if(MassPara.buf[2]==0x42)
			{
				if(MassPara.buf[3]==0x43)
				{
					//LEN.mDataLength=BIG_ENDIAN(MassPara.cbw.dCBWDatL);			//做BO协议处理
					LEN.mdataLen[3] = *(unsigned char *)(&MassPara.cbw.dCBWDatL);  /* 将PC机的低字节在前的16位字数据转换为C51的高字节在前的数据 */
					LEN.mdataLen[2] = *( (unsigned char *)(&MassPara.cbw.dCBWDatL) + 1 );
					LEN.mdataLen[1] = *( (unsigned char *)(&MassPara.cbw.dCBWDatL) + 2 );
					LEN.mdataLen[0] = *( (unsigned char *)(&MassPara.cbw.dCBWDatL) + 3 );
					mdCBWTag[0]=MassPara.buf[4];
                    mdCBWTag[1]=MassPara.buf[5];
                    mdCBWTag[2]=MassPara.buf[6];
	                mdCBWTag[3]=MassPara.buf[7];	//取出数据长度
					if(LEN.mDataLength)
					{
						CH375BULKDOWN=(MassPara.cbw.bmCBWFlags&0X80)?0:1;	//判断是上传还是下传数据
						CH375BULKUP=(MassPara.cbw.bmCBWFlags&0X80)?1:0;
					}
					CH375CSW = 1;
					UFI_Hunding();
				}
				else
					R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			}
			else
				R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
		}
		else
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
	}
	else
	R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
}

void mCH375UpCsw(void)
{
	unsigned char i;		//如果数据为0
	pBuf=&MassPara.buf[0];
	CH375CSW=0;				//上传CSW
	CH375BULKUP=0;			//取消数据上传
	MassPara.buf[0]=0x55;	//dCSWSignature
	MassPara.buf[1]=0x53;
	MassPara.buf[2]=0x42;
	MassPara.buf[3]=0x53;
	MassPara.buf[4]=mdCBWTag[0];
	MassPara.buf[5]=mdCBWTag[1];
	MassPara.buf[6]=mdCBWTag[2];
	MassPara.buf[7]=mdCBWTag[3];
	MassPara.buf[8]=0;
	MassPara.buf[9]=0;
	MassPara.buf[10]=LEN.mdataLen[1];
	MassPara.buf[11]=LEN.mdataLen[0];
	MassPara.buf[12]=BcswStatus;
	for(i = 0;i<13;i++)
	{
		EP1_Databuf[MAX_PACKET_SIZE+i] = *pBuf;
		pBuf++;
	}
	R8_UEP1_T_LEN = 13;
	R8_UEP1_CTRL = (R8_UEP1_CTRL & ~ MASK_UEP_T_RES) | UEP_T_RES_ACK;  // 允许上传
}

//**********************************************************************************
void CH375bulkUpData(void)    //调用端点1上传数据
{											
	unsigned char len,i;
	if(LEN.mDataLength>0x40)
	{
		len=0x40;
		LEN.mDataLength-=0x40;
	}
	else 
	{
		len= (unsigned char) LEN.mDataLength;
		LEN.mDataLength=0;
		CH375BULKUP=0;
	}
	
	if(pBuf_ReSelect)
	{
		#ifdef EXTERNAL_FLASH
			app_flash_read(Locate_Addr,len,&EP1_Databuf[MAX_PACKET_SIZE]);
        // const sfud_flash *flash = sfud_get_device_table() + 0;
        // sfud_read(flash, Locate_Addr, len, &EP1_Databuf[MAX_PACKET_SIZE]);

		#endif
		
		#ifdef CODE_FLASH
			CodeFlash_ReadBuf(Locate_Addr,len, &EP1_Databuf[MAX_PACKET_SIZE]);
		#endif
		//printf("CodeFlash_ReadBuf \n");
		
		Locate_Addr += len;		
		if(LEN.mDataLength==0)
			pBuf_ReSelect = 0;
	}
	else
	{
		for(i = 0;i<len;i++)
		{
			EP1_Databuf[MAX_PACKET_SIZE+i] = *pBuf;
			pBuf++;
		}
	}
	R8_UEP1_T_LEN = len;
	R8_UEP1_CTRL = (R8_UEP1_CTRL & ~ MASK_UEP_T_RES) | UEP_T_RES_ACK;  // 允许上传
}

void mCH375BulkDownData(void)
{
	unsigned char len,i;
	
	len = R8_USB_RX_LEN;																								
	for(i=0;i!=len;i++)						
		buffer[i]=EP1_Databuf[i];	//将数据读入到缓冲区
	
	#ifdef EXTERNAL_FLASH
		app_flash_write(Locate_Addr,len,&buffer[0]);
    
        // const sfud_flash *flash = sfud_get_device_table() + 0;
        // sfud_write(flash, Locate_Addr, len, &buffer[0]);
	#endif

	#ifdef CODE_FLASH
		CodeFlash_WriteBuf(Locate_Addr,len,buffer);
	#endif
	
	//printf("CodeFlash_WriteBuf \n");
	
	Locate_Addr += len;
	
	LEN.mDataLength-=len;		//全局数据长度减掉当前获得的长度
	
	if(LEN.mDataLength==0)
	{							//如果数据为0,则传送CSW
		CH375BULKDOWN=0;
		mCH375UpCsw();		     //上传CSW
	}
}


/*****************************************************************/
void USB_DevTransProcess( void )
{
	UINT8  len, chtype, length;
	UINT8  intflag, errflag = 0;
	
	intflag = R8_USB_INT_FG;
	if( intflag & RB_UIF_TRANSFER )
	{
		switch ( R8_USB_INT_ST & ( MASK_UIS_TOKEN | MASK_UIS_ENDP ) )     // 分析操作令牌和端点号
		{
			case UIS_TOKEN_SETUP:
                R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
				len = R8_USB_RX_LEN;
                if ( len == sizeof( USB_SETUP_REQ ) ) 
				{
					SetupReqLen = pSetupReqPak->wLength;
                    SetupReqCode = pSetupReqPak->bRequest;
					chtype = pSetupReqPak->bRequestType;
					
					len = 0;
                    errflag = 0;
					
					if ( ( pSetupReqPak->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )   
                    {
                        SetupReqCode = pSetupReqPak->bRequest;
						if(SetupReqCode == 0xFE)   //GET MAX LUN
						{
							pDescr = (PUINT8)( &MAX_LUN[0] ); 							
							len = 1;
						    if ( SetupReqLen > len )
                            {
                                SetupReqLen = len;                                 // 限制总长度
                            }
                            len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen;  // 本次传输长度
                            memcpy( EP0_Databuf, pDescr, len );                   /* 加载上传数据 */
                            SetupReqLen -= len;
                            pDescr += len;	
						}
                    }
                    else                            /* 标准请求 */
                    {
						switch( SetupReqCode )  
						{
							case USB_GET_DESCRIPTOR:
							{							
								switch( ((pSetupReqPak->wValue)>>8) )
								{	
									case USB_DESCR_TYP_DEVICE:
										pDescr = MyDevDescr;
										len = MyDevDescr[0];
										break;
									
									case USB_DESCR_TYP_CONFIG:
										pDescr = MyCfgDescr;
										len = MyCfgDescr[2];
										break;
									
									case USB_DESCR_TYP_STRING:
										switch( (pSetupReqPak->wValue)&0xff )
										{
											case 1:
												pDescr = MyManuInfo;
												len = MyManuInfo[0];
												break;
											case 2:
												pDescr = MyProdInfo;
												len = MyProdInfo[0];
												break;
											case 0:
												pDescr = MyLangDescr;
												len = MyLangDescr[0];
												break;
											default:
												errflag = 0xFF;                               // 不支持的字符串描述符
												break;
										}
										break;
									
									default :
										errflag = 0xff;
										break;
								}
								if( SetupReqLen>len )	SetupReqLen = len;		//实际需上传总长度
								len = (SetupReqLen >= DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;  
								memcpy( pEP0_DataBuf, pDescr, len );   
								pDescr += len;
							}
								break;
							
							case USB_SET_ADDRESS:
								SetupReqLen = (pSetupReqPak->wValue)&0xff;
								break;
							
							case USB_GET_CONFIGURATION:
								pEP0_DataBuf[0] = DevConfig;
								if ( SetupReqLen > 1 )		SetupReqLen = 1;
								break;
							
							case USB_SET_CONFIGURATION:
								DevConfig = (pSetupReqPak->wValue)&0xff;
								break;
							
							case USB_CLEAR_FEATURE:
								if ( ( pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )    // 端点
								{
									switch( (pSetupReqPak->wIndex)&0xff )
									{
									case 0x81:
										R8_UEP1_CTRL = (R8_UEP1_CTRL & ~( RB_UEP_T_TOG|MASK_UEP_T_RES )) | UEP_T_RES_NAK;
										if(CH375CSW) mCH375UpCsw();
										lastFSTALL = FSTALL;
										FSTALL = 0;
										break;
									case 0x01:
										R8_UEP1_CTRL = (R8_UEP1_CTRL & ~( RB_UEP_R_TOG|MASK_UEP_R_RES )) | UEP_R_RES_ACK;
										if(CH375CSW) mCH375UpCsw();
										lastFSTALL = FSTALL;
										FSTALL = 0;
										break;
									default:
										errflag = 0xFF;                                 // 不支持的端点
										break;
									}
								}
								else	errflag = 0xFF;    
								break;
							
							case USB_GET_INTERFACE:
								pEP0_DataBuf[0] = 0x00;
								 if ( SetupReqLen > 1 )		SetupReqLen = 1;
								break;
							
							case USB_GET_STATUS:
								pEP0_DataBuf[0] = 0x00;
								pEP0_DataBuf[1] = 0x00;
								if ( SetupReqLen > 2 )		SetupReqLen = 2;
								break;
							
							default:
								errflag = 0xff;
								break;
						}
					}
				}
				else	errflag = 0xff;
				
				if( errflag == 0xff)		// 错误或不支持
				{
//					SetupReqCode = 0xFF;
                    R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  // STALL
				}
				else
				{
					if( chtype & 0x80 )		// 上传
					{
						len = (SetupReqLen>DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
						SetupReqLen -= len;
					}
					else len = 0;		// 下传
					
					R8_UEP0_T_LEN = len; 
                    R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // 默认数据包是DATA1
				}				
				break;
			
			case UIS_TOKEN_IN:
                switch( SetupReqCode )
                {
					case USB_GET_DESCRIPTOR:
						len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen;  // 本次传输长度
						memcpy( pEP0_DataBuf, pDescr, len );                    /* 加载上传数据 */
						SetupReqLen -= len;
						pDescr += len;
						R8_UEP0_T_LEN = len;
						R8_UEP0_CTRL ^= RB_UEP_T_TOG;                             // 翻转
						break;
					case USB_SET_ADDRESS:
						R8_USB_DEV_AD = (R8_USB_DEV_AD&RB_UDA_GP_BIT) | SetupReqLen;
						R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
						break;
					default:
						R8_UEP0_T_LEN = 0;                                      // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
						R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
						break;
                }				
				break;
			
			case UIS_TOKEN_OUT:
				//len = R8_USB_RX_LEN;
				R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
				break;
			
			case UIS_TOKEN_OUT | 1:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK )  // 不同步的数据包将丢弃
				{                      
					if(CH375BULKDOWN)  mCH375BulkDownData();	//如果上传数据阶段则调用数据上传
					else
					{	 										//不是数据下传则判断是否
						length = R8_USB_RX_LEN;
						if(!length) break;								//数据包长度为零则跳出
						for(len=0;len!=length;len++)						
							MassPara.buf[len] = EP1_Databuf[len];	//将数据读入到缓冲区
						
						mCH375BulkOnly();
						
						if(!CH375BULKDOWN)
						{
							if(CH375BULKUP) CH375bulkUpData();					//调用批量数据上传
							else if(!FSTALL) mCH375UpCsw();								//没有数据上传调用CSW上传
																			//在这里做上传数据调用
						}
					}	
				}
				break;
			
			case UIS_TOKEN_IN | 1:				
				if(CH375BULKUP) CH375bulkUpData();								//调用数据上传
				else if(CH375CSW) mCH375UpCsw();								//上传CSW
				else
                R8_UEP1_CTRL = (R8_UEP1_CTRL & ~ MASK_UEP_T_RES) | UEP_T_RES_NAK;      // 暂停上传
				break;
			
			default :
				break;
		}
		R8_USB_INT_FG = RB_UIF_TRANSFER;
	}
	else if( intflag & RB_UIF_BUS_RST )
	{
		R8_USB_DEV_AD = 0;
		R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
		R8_UEP1_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
		R8_USB_INT_FG |= RB_UIF_BUS_RST;
	}
	else if( intflag & RB_UIF_SUSPEND )
	{
		if ( R8_USB_MIS_ST & RB_UMS_SUSPEND ) {;}	// 挂起
		else		{;}								// 唤醒
		R8_USB_INT_FG = RB_UIF_SUSPEND;
	}
	else
	{
		R8_USB_INT_FG = intflag;
	}
}

void USB_IRQHandler (void)		/* USB中断服务程序,使用寄存器组1 */
{
	USB_DevTransProcess();
}
