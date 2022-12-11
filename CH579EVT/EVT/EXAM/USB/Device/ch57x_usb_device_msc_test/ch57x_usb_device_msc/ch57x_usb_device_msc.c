#include "ch57x_usb_device_msc.h"

#include "ch57x_code_flash.h"


// �豸������
const UINT8  MyDevDescr[] = 
{
	0x12, 0x01, 0x10, 0x01,
    0x00, 0x00, 0x00, DevEP0SIZE,
    0x44, 0x33, 0x33, 0x35,   // ����ID�Ͳ�ƷID
    0x00, 0x01, 0x01, 0x02,
    0x00, 0x01
};
// ����������
const UINT8  MyCfgDescr[] =  
{
	0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32,
    0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,                     
    0x07, 0x05, 0x01, 0x02, 0x40, 0x00, 0x00,
    0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00
};
// ����������
const UINT8  MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };
// ������Ϣ
const UINT8  MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
// ��Ʒ��Ϣ
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
};   //����������

const UINT8  mode5sense3F[]=
{
	0x00, 0x06, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08,
	(DISK_SEC_NUM>>24)&0xFF, (DISK_SEC_NUM>>16)&0xFF, (DISK_SEC_NUM>>8)&0xFF, DISK_SEC_NUM&0xFF,
	00, 00, 02, 00 
};  //����������

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
	unsigned long mDataLength;	//���ݳ���
	unsigned char mdataLen[4];	//
} LEN;

UINT8 buffer[64]; 
UINT8 mdCBWTag[4];		//dCBWTag
MASS_PARA  MassPara;
UINT8 CH375BULKUP;				//�����ϴ�
UINT8 CH375BULKDOWN;				//�����´�
UINT8 CH375CSW;					//CSW�ϴ���־
UINT8 BcswStatus;		//CSW״̬
UINT8 mSenseKey;
UINT8 mASC;
UINT8 FSTALL;						//���ݴ����־
UINT8 lastFSTALL;
UINT8 pBuf_ReSelect = 0;
UINT32 Locate_Addr;
UINT8 *pBuf;


/**********************************************************/
UINT8   DevConfig;
UINT8   SetupReqCode;
UINT16  SetupReqLen;
const UINT8 *pDescr;

/******** �û��Զ������˵�RAM ****************************************/
__align(4) UINT8 EP0_Databuf[64];	//ep0(64)
__align(4) UINT8 EP1_Databuf[64+64];	//ep1_out(64)+ep1_in(64)


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
															//�����ϴ��˵�����һ��STALL����������� // may or may-not
			BcswStatus =2;
			break;

		case CASE4:     									/* Hi>Dn*/
		case CASE5:     									/* Hi>Di*/
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;										//�����ϴ��˵�����һ��STALL�����������
			BcswStatus= 1;									//CSW_GOOD or CSW_FAIL
			break;

		case CASE7:    										 /* Hi<Di*/
		case CASE8:     									/* Hi<>Do */
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;										//�����ϴ��˵�����һ��STALL�����������
			BcswStatus = 2;
			break;

		case CASE9:     									/* Ho>Dn*/
		case CASE11:    									/* Ho>Do*/
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_R_RES ;
			FSTALL=1;                                       //�����ϴ��˵�����һ��STALL�����������											
			BcswStatus =1;									//CSW_GOOD or CSW_FAIL
			break;

		case CASE10:    								    /* Ho<>Di */
		case CASE13:    								    /* Ho<Do*/
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;        //�����ϴ��˵�����һ��STALL�����������
			FSTALL=1;
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_R_RES ;										
			BcswStatus = 2;
			break;

		case CASECBW:   									/* invalid CBW */
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_R_RES ;		//����˵�����һ��STALL�����������
			BcswStatus = 2;
			break;

		case CASECMDFAIL:
			R8_UEP1_CTRL = R8_UEP1_CTRL | MASK_UEP_T_RES ;
			FSTALL=1;                                       //�����ϴ��˵�����һ��STALL�����������										
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
	pBuf = (unsigned char*)DBINQUITY;					////��ѯU����Ϣ
	if(LEN.mDataLength>36) LEN.mDataLength=36;
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}

void UFI_read10(void)
{
	//��ȡ����
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
	//ģʽ��ʶ
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
	//������ʶ
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
	BcswStatus=0;			//����U���Ƿ�׼����
	mSenseKey=0;
	mASC=0;
}
void UFI_perOrMed(void )   //�����Ƴ�����
{				
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}

void UFI_write(void )
{
	UINT8 i,num;
	LEN.mDataLength=(((UINT32)MassPara.cbw.cbwcb.buf1[7]<<8) | (UINT32)MassPara.cbw.cbwcb.buf1[8])*DISK_SEC_LEN;		//д���ݳ���
	Locate_Addr = ((UINT32)MassPara.cbw.cbwcb.buf1[2]<<24) | ((UINT32)MassPara.cbw.cbwcb.buf1[3]<<16) | ((UINT32)MassPara.cbw.cbwcb.buf1[4]<<8) | (UINT32)MassPara.cbw.cbwcb.buf1[5];
	Locate_Addr = Locate_Addr*DISK_SEC_LEN;
	//����
	num = MassPara.cbw.cbwcb.buf1[8];   //��д������
	
	#ifdef EXTERNAL_FLASH
    const sfud_flash *flash = sfud_get_device_table() + 0;
	for(i=0;i<num;i++){
		//EraseExternal4KFlash_SPI(Locate_Addr + i*DISK_SEC_LEN); 
        sfud_erase(flash, Locate_Addr + i*DISK_SEC_LEN, 4096);
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

void UFI_staStoUnit(void )  //����װ��ж���豸
{     
	CH375BULKDOWN=0;
	CH375BULKUP=0;
	BcswStatus=0;
	mSenseKey=0;
	mASC=0;
}
void UFI_verify(void )
{
	BcswStatus=0;		//У��洢���ռ�
	mSenseKey=0;
	mASC=0;
	//��������ֻ����Ϊ��ʾ����û�������������洢��
	//��ʵ������һ��һ��Ҫ����
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
					//LEN.mDataLength=BIG_ENDIAN(MassPara.cbw.dCBWDatL);			//��BOЭ�鴦��
					LEN.mdataLen[3] = *(unsigned char *)(&MassPara.cbw.dCBWDatL);  /* ��PC���ĵ��ֽ���ǰ��16λ������ת��ΪC51�ĸ��ֽ���ǰ������ */
					LEN.mdataLen[2] = *( (unsigned char *)(&MassPara.cbw.dCBWDatL) + 1 );
					LEN.mdataLen[1] = *( (unsigned char *)(&MassPara.cbw.dCBWDatL) + 2 );
					LEN.mdataLen[0] = *( (unsigned char *)(&MassPara.cbw.dCBWDatL) + 3 );
					mdCBWTag[0]=MassPara.buf[4];
                    mdCBWTag[1]=MassPara.buf[5];
                    mdCBWTag[2]=MassPara.buf[6];
	                mdCBWTag[3]=MassPara.buf[7];	//ȡ�����ݳ���
					if(LEN.mDataLength)
					{
						CH375BULKDOWN=(MassPara.cbw.bmCBWFlags&0X80)?0:1;	//�ж����ϴ������´�����
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
	unsigned char i;		//�������Ϊ0
	pBuf=&MassPara.buf[0];
	CH375CSW=0;				//�ϴ�CSW
	CH375BULKUP=0;			//ȡ�������ϴ�
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
	R8_UEP1_CTRL = (R8_UEP1_CTRL & ~ MASK_UEP_T_RES) | UEP_T_RES_ACK;  // �����ϴ�
}

//**********************************************************************************
void CH375bulkUpData(void)    //���ö˵�1�ϴ�����
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
			//BlukReadExternalFlash_SPI(Locate_Addr,len,&EP1_Databuf[MAX_PACKET_SIZE]);
        const sfud_flash *flash = sfud_get_device_table() + 0;
        sfud_read(flash, Locate_Addr, len, &EP1_Databuf[MAX_PACKET_SIZE]);

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
	R8_UEP1_CTRL = (R8_UEP1_CTRL & ~ MASK_UEP_T_RES) | UEP_T_RES_ACK;  // �����ϴ�
}

void mCH375BulkDownData(void)
{
	unsigned char len,i;
	
	len = R8_USB_RX_LEN;																								
	for(i=0;i!=len;i++)						
		buffer[i]=EP1_Databuf[i];	//�����ݶ��뵽������
	
	#ifdef EXTERNAL_FLASH
		//BlukWriteExternalFlash_SPI(Locate_Addr,len,&buffer[0]);
    
        const sfud_flash *flash = sfud_get_device_table() + 0;
        sfud_write(flash, Locate_Addr, len, &buffer[0]);
	#endif

	#ifdef CODE_FLASH
		CodeFlash_WriteBuf(Locate_Addr,len,buffer);
	#endif
	
	//printf("CodeFlash_WriteBuf \n");
	
	Locate_Addr += len;
	
	LEN.mDataLength-=len;		//ȫ�����ݳ��ȼ�����ǰ��õĳ���
	
	if(LEN.mDataLength==0)
	{							//�������Ϊ0,����CSW
		CH375BULKDOWN=0;
		mCH375UpCsw();		     //�ϴ�CSW
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
		switch ( R8_USB_INT_ST & ( MASK_UIS_TOKEN | MASK_UIS_ENDP ) )     // �����������ƺͶ˵��
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
                                SetupReqLen = len;                                 // �����ܳ���
                            }
                            len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen;  // ���δ��䳤��
                            memcpy( EP0_Databuf, pDescr, len );                   /* �����ϴ����� */
                            SetupReqLen -= len;
                            pDescr += len;	
						}
                    }
                    else                            /* ��׼���� */
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
												errflag = 0xFF;                               // ��֧�ֵ��ַ���������
												break;
										}
										break;
									
									default :
										errflag = 0xff;
										break;
								}
								if( SetupReqLen>len )	SetupReqLen = len;		//ʵ�����ϴ��ܳ���
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
								if ( ( pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )    // �˵�
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
										errflag = 0xFF;                                 // ��֧�ֵĶ˵�
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
				
				if( errflag == 0xff)		// �����֧��
				{
//					SetupReqCode = 0xFF;
                    R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  // STALL
				}
				else
				{
					if( chtype & 0x80 )		// �ϴ�
					{
						len = (SetupReqLen>DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
						SetupReqLen -= len;
					}
					else len = 0;		// �´�	
					
					R8_UEP0_T_LEN = len; 
                    R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // Ĭ�����ݰ���DATA1
				}				
				break;
			
			case UIS_TOKEN_IN:
                switch( SetupReqCode )
                {
					case USB_GET_DESCRIPTOR:
						len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen;  // ���δ��䳤��
						memcpy( pEP0_DataBuf, pDescr, len );                    /* �����ϴ����� */
						SetupReqLen -= len;
						pDescr += len;
						R8_UEP0_T_LEN = len;
						R8_UEP0_CTRL ^= RB_UEP_T_TOG;                             // ��ת
						break;
					case USB_SET_ADDRESS:
						R8_USB_DEV_AD = (R8_USB_DEV_AD&RB_UDA_GP_BIT) | SetupReqLen;
						R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
						break;
					default:
						R8_UEP0_T_LEN = 0;                                      // ״̬�׶�����жϻ�����ǿ���ϴ�0�������ݰ��������ƴ���
						R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
						break;
                }				
				break;
			
			case UIS_TOKEN_OUT:
				//len = R8_USB_RX_LEN;
				R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
				break;
			
			case UIS_TOKEN_OUT | 1:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK )  // ��ͬ�������ݰ�������
				{                      
					if(CH375BULKDOWN)  mCH375BulkDownData();	//����ϴ����ݽ׶�����������ϴ�
					else
					{	 										//���������´����ж��Ƿ�
						length = R8_USB_RX_LEN;
						if(!length) break;								//���ݰ�����Ϊ��������																		
						for(len=0;len!=length;len++)						
							MassPara.buf[len] = EP1_Databuf[len];	//�����ݶ��뵽������
						
						mCH375BulkOnly();
						
						if(!CH375BULKDOWN)
						{
							if(CH375BULKUP) CH375bulkUpData();					//�������������ϴ�
							else if(!FSTALL) mCH375UpCsw();								//û�������ϴ�����CSW�ϴ�
																			//���������ϴ����ݵ���
						}
					}	
				}
				break;
			
			case UIS_TOKEN_IN | 1:				
				if(CH375BULKUP) CH375bulkUpData();								//���������ϴ�
				else if(CH375CSW) mCH375UpCsw();								//�ϴ�CSW
				else
                R8_UEP1_CTRL = (R8_UEP1_CTRL & ~ MASK_UEP_T_RES) | UEP_T_RES_NAK;      // ��ͣ�ϴ�
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
		if ( R8_USB_MIS_ST & RB_UMS_SUSPEND ) {;}	// ����
		else		{;}								// ����
		R8_USB_INT_FG = RB_UIF_SUSPEND;
	}
	else
	{
		R8_USB_INT_FG = intflag;
	}
}

void USB_IRQHandler (void)		/* USB�жϷ������,ʹ�üĴ�����1 */
{
	USB_DevTransProcess();
}
