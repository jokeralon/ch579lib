#include "usb_vcp.h"
#include "CH57x_common.h"
#include "string.h"

#define DevEP0SIZE 0x40

char UserTxBufferFS[HID_CDC_DATA_SIZE] ={0};

/* HID�ı���������*/
/*������8�ֽڷ��ͣ�
**  ��һ�ֽڱ�ʾ������Ƿ��£�D0:Ctrl D1:Shift D2:Alt
**  �ڶ��ֽڱ�����ֵΪ0
**  ����~�ڰ��ֽ�:��ͨ����ֵ������,�����ͬʱ����6����
**������1�ֽڽ���:��Ӧ�����ϵ�LED��,����ֻ��������λ��
**	D0:Num Lock   D1:Cap Lock   D2:Scroll Lock   D3:Compose   D4:Kana*/

// �豸������
const UINT8  MyDevDescr[] = { 
//0x12, 0x01, 0x10, 0x01,0xFF, 0x80, 0x55, DevEP0SIZE,							 
//0x48, 0x43, 0x37, 0x55,  					// ����ID�Ͳ�ƷID
//0x00, 0x01, 0x01, 0x02, 0x00, 0x01,
	
        0x12, /*bLength�����ȣ��豸�������ĳ���Ϊ18�ֽ�*/
        0x01, /*bDescriptorType�����ͣ��豸�������ı����0x01*/
        0x10, /*bcdUSB����ʹ�õ�USB�汾Ϊ2.0*/
        0x01,
        0x02, /*bDeviceClass���豸��ʹ�õ������*/
        0x00, /*bDeviceSubClass���豸��ʹ�õ��������*/
        0x00, /*bDeviceProtocol���豸��ʹ�õ�Э��*/
        HID_CDC_DATA_SIZE, /*bMaxPacketSize����������Ϊ8�ֽ�*/
        0x83, /*idVendor������IDΪ0x7788*/
        0x04,
        0x40, /*idProduct����ƷIDΪ0x1122*/
        0x57,
        0x00, /*bcdDevice���豸�İ汾��Ϊ2.00*/
        0x02,
        1,   /*iManufacturer:�����ַ���������*/
        2,   /*iProduct����Ʒ�ַ���������*/
        3,   /*iSerialNumber���豸�����к��ַ�������*/
        0x01 /*bNumConfiguration���豸��1������*/
};
// ����������
const UINT8  MyCfgDescr[] = { 
//0x09, 0x02, 0x4A, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32,
//0x09, 0x04, 0x00, 0x00, 0x08, 0xFF, 0x80, 0x55, 0x00,
//0x07, 0x05, 0x84, 0x02, 0x40, 0x00, 0x00,
//0x07, 0x05, 0x04, 0x02, 0x40, 0x00, 0x00,
//0x07, 0x05, 0x83, 0x02, 0x40, 0x00, 0x00,
//0x07, 0x05, 0x03, 0x02, 0x40, 0x00, 0x00,
//0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00,
//0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00,
//0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00,
//0x07, 0x05, 0x01, 0x02, 0x40, 0x00, 0x00,
	
				0x09,                              /*bLength�����ȣ��豸�ַ����ĳ���Ϊ9�ֽ�*/
        USB_CONFIGURATION_DESCRIPTOR_TYPE, /*bDescriptorType�����ͣ����������������ͱ��Ϊ0x2*/
        IAD_HID_CDC_SIZ_CONFIG_DESC,       /*wTotalLength���������������ܳ���Ϊ0x43�ֽ�*/
        0x00,
        0x02, /*bNumInterfaces��������֧�ֵĽӿ�����1��*/
        0x01, /*bConfigurationValue�������õ�ֵ*/
        0x00, /*iConfiguration�������õ��ַ���������ֵ����ֵΪ0��ʾû���ַ���*/
        0xC0, /* bmAttributes:�豸��һЩ���ԣ�0xc0��ʾ�Թ��磬��֧��Զ�̻���
						D7:��������Ϊ1��D6:�Ƿ��Թ��磬D5���Ƿ�֧��Զ�̻��ѣ�D4~D0����������Ϊ0*/
        0x32, /*�������ϻ�õ�������Ϊ100mA */
              //    0x96,         /*MaxPower���豸��Ҫ�������ϻ�ȡ���ٵ�������λΪ2mA��0x96��ʾ300mA*/
  /* Interface descriptor type */
              /************** Descriptor of virtual com port ****************/
        /*Interface Descriptor*/
        0x09, 											/* bLength: Interface Descriptor size */											/*bLength�����ȣ��ӿ��������ĳ���Ϊ9�ֽ� */
        0x04, 											/* bDescriptorType: Interface */                              /* bDescriptorType���ӿ�������������Ϊ0x4 */
        0x00,											 	/* bInterfaceNumber: Number of Interface */                   /*bInterfaceNumber���ýӿڵı��*/
        0x00, 											/* bAlternateSetting: Alternate setting */                    /*bAlternateSetting���ýӿڵı��ñ�� */
        0x01, 											/* bNumEndpoints: One endpoints used */                       /*bNumEndpoints���ýӿ���ʹ�õĶ˵���*/
        0x02, 											/* bInterfaceClass: Communication Interface Class */          /*bInterfaceClass�ýӿ���ʹ�õ���ΪCDC*/
        0x02, 											/* bInterfaceSubClass: Abstract Control Model */              /*bInterfaceSubClass���ýӿ����õ����� 1=BOOT, 0=no boot */
        0x01, 											/* bInterfaceProtocol: Common AT commands */                  /*nInterfaceProtocol :�ýӿ�ʹ�õ�Э��0=none, 1=keyboard, 2=mouse */
        0x00, 											/* iInterface: */                                             /*iInterface: �ýӿ��ַ��������� */
        /*Header Functional Descriptor*/
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header Func Desc */
        0x10, /* bcdCDC: spec release number */
        0x01,
        /*Call Management Functional Descriptor*/
        0x05, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x01, /* bDescriptorSubtype: Call Management Func Desc */
        0x00, /* bmCapabilities: D0+D1 */
        0x01, /* bDataInterface: 1 */
        /*ACM Functional Descriptor*/
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x02, /* bmCapabilities */
        /*Union Functional Descriptor*/
        0x05,                         /* bFunctionLength */
        0x24,                         /* bDescriptorType: CS_INTERFACE */
        0x06,                         /* bDescriptorSubtype: Union func desc */
        0x00,                         /* bMasterInterface: Communication class interface */
        0x01,                         /* bSlaveInterface0: Data Class Interface */
        /******************** EP2 �˵�������******************/
                                      /* 27 */
        0x07,                         /* bLength: �˵��������ĳ���Ϊ7�ֽ�*/
        USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: �˵�������������Ϊ0x05*/
        0x82,                         /* bEndpointAddress: �ö˵�(����)�ĵ�ַ,D7:0(OUT),1(IN),D6~D4:����,D3~D0:�˵��*/
        0x03,                         /* bmAttributes: �˵������ΪΪ�ж϶˵�.
			D0~D1��ʾ��������:0(���ƴ���),1(��ʱ����),2(��������),3(�жϴ���)
			�ǵ�ʱ����˵�:D2~D7:����Ϊ0
			��ʱ����˵㣺
			D2~D3��ʾͬ��������:0(��ͬ��),1(�첽),2(����),3(ͬ��)
			D4~D5��ʾ��;:0(���ݶ˵�),1(�����˵�),2(�������������ݶ˵�),3(����)��D6~D7:����,*/
        HID_CDC_DATA_SIZE,                         /* wMaxPacketSize: �ö˵�֧�ֵ���������Ϊ8�ֽ�*/
        0x00,
        0xFF, /* bInterval: ��ѯ���(32ms) */
        /************** CDC �ӿ�������****************/
        /* 09 */
        0x09,                          /*bLength�����ȣ��ӿ��������ĳ���Ϊ9�ֽ� */
        USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType���ӿ�������������Ϊ0x4 */
        0x01,                          /*bInterfaceNumber���ýӿڵı��*/
        0x00,                          /*bAlternateSetting���ýӿڵı��ñ�� */
        0x02,                          /*bNumEndpoints���ýӿ���ʹ�õĶ˵���*/
        0x0A,                          /*bInterfaceClass�ýӿ���ʹ�õ���ΪCDC*/
        0x00,                          /*bInterfaceSubClass���ýӿ����õ����� 1=BOOT, 0=no boot */
        0x00,                          /*nInterfaceProtocol :�ýӿ�ʹ�õ�Э��0=none, 1=keyboard, 2=mouse */
        0,                             /*iInterface: �ýӿ��ַ��������� */
        /******************** EP3 OUT �˵�������******************/
                                       /* 27 */
        0x07,                          /* bLength: �˵��������ĳ���Ϊ7�ֽ�*/
        USB_ENDPOINT_DESCRIPTOR_TYPE,  /* bDescriptorType: �˵�������������Ϊ0x05*/
        0x03,                          /* bEndpointAddress: �ö˵�(����)�ĵ�ַ,D7:0(OUT),1(IN),D6~D4:����,D3~D0:�˵��*/
        0x02,                          /* bmAttributes: �˵������ΪΪ�ж϶˵�.
				D0~D1��ʾ��������:0(���ƴ���),1(��ʱ����),2(��������),3(�жϴ���)
				�ǵ�ʱ����˵�:D2~D7:����Ϊ0
				��ʱ����˵㣺
				D2~D3��ʾͬ��������:0(��ͬ��),1(�첽),2(����),3(ͬ��)
				D4~D5��ʾ��;:0(���ݶ˵�),1(�����˵�),2(�������������ݶ˵�),3(����)��D6~D7:����,*/
				
        HID_CDC_DATA_SIZE,                          /* wMaxPacketSize: �ö˵�֧�ֵ���������Ϊ8�ֽ�*/
        0x00,
        0x00,                         /* bInterval: ��ѯ���(32ms) */
                                      /******************** EP3 IN �˵�������******************/
                                      /* 27 */
        0x07,                         /* bLength: �˵��������ĳ���Ϊ7�ֽ�*/
        USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: �˵�������������Ϊ0x05*/
        0x83,                         /* bEndpointAddress: �ö˵�(����)�ĵ�ַ,D7:0(OUT),1(IN),D6~D4:����,D3~D0:�˵��*/
        0x02,                         /* bmAttributes: �˵������ΪΪ�ж϶˵�.
			D0~D1��ʾ��������:0(���ƴ���),1(��ʱ����),2(��������),3(�жϴ���)
			�ǵ�ʱ����˵�:D2~D7:����Ϊ0
			��ʱ����˵㣺
			D2~D3��ʾͬ��������:0(��ͬ��),1(�첽),2(����),3(ͬ��)
			D4~D5��ʾ��;:0(���ݶ˵�),1(�����˵�),2(�������������ݶ˵�),3(����)��D6~D7:����,*/
        HID_CDC_DATA_SIZE,                         /* wMaxPacketSize: �ö˵�֧�ֵ���������Ϊ8�ֽ�*/
        0x00,
        0x00, /* bInterval: ��ѯ���(32ms) */
				
};
// ����������
const UINT8  MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };
// ������Ϣ
const UINT8  MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
// ��Ʒ��Ϣ
const UINT8  MyProdInfo[] = { 0x0C, 0x03, 'C', 0, 'H', 0, '5', 0, '7', 0, 'x', 0 };

/**********************************************************/
UINT8   DevConfig;
UINT8   SetupReqCode;
UINT16  SetupReqLen;
const UINT8 *pDescr;

/******** �û��Զ������˵�RAM ****************************************/
__align(4) UINT8 EP0_Databuf[64+64+64];	//ep0(64)+ep4_out(64)+ep4_in(64)
__align(4) UINT8 EP1_Databuf[64+64];	//ep1_out(64)+ep1_in(64)
__align(4) UINT8 EP2_Databuf[64+64];	//ep2_out(64)+ep2_in(64)
__align(4) UINT8 EP3_Databuf[64+64];	//ep3_out(64)+ep3_in(64)

void USB_DevTransProcess( void )
{
	UINT8  len, chtype;
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
                        errflag = 0xFF;				/* �Ǳ�׼���� */
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
									case 0x82:
										R8_UEP2_CTRL = (R8_UEP2_CTRL & ~( RB_UEP_T_TOG|MASK_UEP_T_RES )) | UEP_T_RES_NAK;
										break;
									case 0x02:
										R8_UEP2_CTRL = (R8_UEP2_CTRL & ~( RB_UEP_R_TOG|MASK_UEP_R_RES )) | UEP_R_RES_ACK;
										break;
									case 0x81:
										R8_UEP1_CTRL = (R8_UEP1_CTRL & ~( RB_UEP_T_TOG|MASK_UEP_T_RES )) | UEP_T_RES_NAK;
										break;
									case 0x01:
										R8_UEP1_CTRL = (R8_UEP1_CTRL & ~( RB_UEP_R_TOG|MASK_UEP_R_RES )) | UEP_R_RES_ACK;
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
					else	len = 0;		// �´�	
					
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
				len = R8_USB_RX_LEN;
				break;
			
			case UIS_TOKEN_OUT | 1:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK ) 
				{                       // ��ͬ�������ݰ�������
					len = R8_USB_RX_LEN;
					DevEP1_OUT_Deal( len );
				}
				break;
			
			case UIS_TOKEN_IN | 1:
				R8_UEP1_CTRL = (R8_UEP1_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
				break;
			
			case UIS_TOKEN_OUT | 2:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK ) 
				{                       // ��ͬ�������ݰ�������
					len = R8_USB_RX_LEN;
					DevEP2_OUT_Deal( len );
				}
				break;
			
			case UIS_TOKEN_IN | 2:
				R8_UEP2_CTRL = (R8_UEP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
				break;
			
			case UIS_TOKEN_OUT | 3:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK ) 
				{                       // ��ͬ�������ݰ�������
					len = R8_USB_RX_LEN;
					DevEP3_OUT_Deal( len );
				}
				break;
			
			case UIS_TOKEN_IN | 3:
				R8_UEP3_CTRL = (R8_UEP3_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
				break;
			
			case UIS_TOKEN_OUT | 4:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK ) 
				{   
					R8_UEP4_CTRL ^= RB_UEP_R_TOG;
					len = R8_USB_RX_LEN;
					DevEP4_OUT_Deal( len );
				}
				break;
			
			case UIS_TOKEN_IN | 4:
				R8_UEP4_CTRL ^=  RB_UEP_T_TOG;
				R8_UEP4_CTRL = (R8_UEP4_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
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
		R8_UEP2_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
		R8_UEP3_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
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



void DebugInit(void)		
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

void USB_VCP_Init(void)
{
//    SetSysClock( CLK_SOURCE_HSE_32MHz );
//    PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );		// ��PLL
//    DelayMs(5);
//    
//    DebugInit();
	
    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
    pEP2_RAM_Addr = EP2_Databuf;
    pEP3_RAM_Addr = EP3_Databuf;
    USB_DeviceInit();
    NVIC_EnableIRQ( USB_IRQn );
}

void DevEP1_OUT_Deal( UINT8 l )
{ /* �û����Զ��� */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP1_IN_DataBuf[i] = ~pEP1_OUT_DataBuf[i];
	}
	DevEP1_IN_Deal( l );
}

void DevEP2_OUT_Deal( UINT8 l )
{ /* �û����Զ��� */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP2_IN_DataBuf[i] = ~pEP2_OUT_DataBuf[i];
	}
	DevEP2_IN_Deal( l );
}

void DevEP3_OUT_Deal( UINT8 l )
{ /* �û����Զ��� */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP3_IN_DataBuf[i] = pEP3_OUT_DataBuf[i];
	}
//	DevEP3_IN_Deal( l );
	usb_printf("DevEP3_OUT_Deal\r\n");
}

void DevEP4_OUT_Deal( UINT8 l )
{ /* �û����Զ��� */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP4_IN_DataBuf[i] = ~pEP4_OUT_DataBuf[i];
	}
	DevEP4_IN_Deal( l );
}


/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

#include <stdarg.h>

 void USB_VCP_SendByte(UINT8 ch)
{
	pEP3_IN_DataBuf[0] = ch;
	DevEP3_IN_Deal( 1 );
}
int USB_VCP_SendStr(UINT8 *str, int str_len)
{
	if( str_len < HID_CDC_DATA_SIZE )	// ���볤��С����󳤶ȣ�ֱ�ӷ���
	{		
		memcpy( pEP3_IN_DataBuf, str, str_len );
		DevEP3_IN_Deal( str_len );
	}else
		return -1;
	return 0;
}

void usb_printf(const char *format, ...)
{
    va_list args;
    uint32_t length;
 
    va_start(args, format);
    length = vsnprintf((char *)UserTxBufferFS, HID_CDC_DATA_SIZE, (char *)format, args);
    va_end(args);
    USB_VCP_SendStr(UserTxBufferFS, length);
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */



void USB_IRQHandler (void)		/* USB�жϷ������,ʹ�üĴ�����1 */
{
	USB_DevTransProcess();
}

