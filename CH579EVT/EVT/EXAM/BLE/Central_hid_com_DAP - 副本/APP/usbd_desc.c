#include "usb_vcp.h"
#include "CH57x_common.h"
#include "string.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "usbd_customhid.h"

#define DevEP0SIZE 0x40

char UserTxBufferFS[HID_CDC_DATA_SIZE] ={0};
/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */ /* A minimal Report Desc with INPUT/OUTPUT/FEATURE report. Zach Lee */
  0x06,0x00,0xFF,         /*  Usage Page (vendor defined) ($FF00) global */
  0x09,0x01,              /*  Usage (vendor defined) ($01) local */
  0xA1,0x01,              /*  Collection (Application) */
  0x15,0x00,              /*   LOGICAL_MINIMUM (0) */
  0x25,0xFF,              /*   LOGICAL_MAXIMUM (255) */
  0x75,0x08,              /*   REPORT_SIZE (8bit) */

  // Input Report
  0x95,64,                /*   Report Length (64 REPORT_SIZE) */
  0x09,0x01,              /*   USAGE (Vendor Usage 1) */
  0x81,0x02,              /*   Input(data,var,absolute) */

  // Output Report
  0x95,64,                /*   Report Length (64 REPORT_SIZE) */
  0x09,0x01,              /*   USAGE (Vendor Usage 1) */
  0x91,0x02,              /*   Output(data,var,absolute) */

  // Feature Report
  0x95,64,                /*   Report Length (64 REPORT_SIZE) */
  0x09,0x01,              /*   USAGE (Vendor Usage 1) */
  0xB1,0x02,              /*   Feature(data,var,absolute) */
  /* USER CODE END 0 */
  0xC0                    /*  END_COLLECTION	             */
};

/** USB standard device descriptor. */
//__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
__ALIGN_BEGIN uint8_t MyDevDescr[USB_LEN_DEV_DESC] __ALIGN_END =
{
  0x12,                       /*bLength */
  USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
  0x00,                       /*bcdUSB */
  0x02,
  0x00,                       /*bDeviceClass*/
  0x00,                       /*bDeviceSubClass*/
  0x00,                       /*bDeviceProtocol*/
  USB_MAX_EP0_SIZE,           /*bMaxPacketSize*/
  LOBYTE(USBD_VID),           /*idVendor*/
  HIBYTE(USBD_VID),           /*idVendor*/
  LOBYTE(USBD_PID_FS),        /*idProduct*/
  HIBYTE(USBD_PID_FS),        /*idProduct*/
  0x00,                       /*bcdDevice rel. 2.00*/
  0x02,
  USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
  USBD_IDX_PRODUCT_STR,       /*Index of product string*/
  USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
  USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
};

/* USB CUSTOM_HID device FS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_CUSTOM_HID_CfgFSDesc[USB_CUSTOM_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_CUSTOM_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xC0,         /*bmAttributes: bus powered */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

  /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x02,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: CUSTOM_HID*/
  0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x09,         /*bLength: CUSTOM_HID Descriptor size*/
  CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: CUSTOM_HID*/
  0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

  CUSTOM_HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  CUSTOM_HID_EPIN_SIZE, /*wMaxPacketSize: 2 Byte max */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
  /* 34 */

  0x07,          /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT, /* bDescriptorType: */
  CUSTOM_HID_EPOUT_ADDR,  /*bEndpointAddress: Endpoint Address (OUT)*/
  0x03, /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPOUT_SIZE,  /* wMaxPacketSize: 2 Bytes max  */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,  /* bInterval: Polling Interval */
  /* 41 */
};


/** USB lang indentifier descriptor. */
// 语言信息
//__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
__ALIGN_BEGIN uint8_t MyLangDescr[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
     USB_LEN_LANGID_STR_DESC,
     USB_DESC_TYPE_STRING,
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING)
};


// 厂家信息
const UINT8  MyManuInfo[] = { 0x14, 0x03, 'G', 0, 'e', 0, 'n', 0, 'i', 0, 'e', 0, 'K', 0, 'i', 0, 't', 0, 's', 0 };
// 产品信息
const UINT8  MyProdInfo[] = { 0x14, 0x03, 'C', 0, 'M', 0, 'S', 0, 'I', 0, 'S', 0, '-', 0, 'D', 0, 'A', 0, 'P', 0 };

const UINT8  MyConfInfo[] = { 0x1E, 0x03, 'D', 0, 'A', 0, 'P', 0, ' ', 0, 'H', 0, 'I', 0, 'D', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0, 'f', 0, 'i', 0, 'g', 0 };
	
const UINT8  MyIntFInfo[] = { 0x24, 0x03, 'D', 0, 'A', 0, 'P', 0, ' ', 0, 'H', 0, 'I', 0, 'D', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0 };


// 配置描述符
/* USB CUSTOM_HID device FS Configuration Descriptor */
__ALIGN_BEGIN static uint8_t MyCfgDescr[USB_CUSTOM_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_CUSTOM_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xC0,         /*bmAttributes: bus powered */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

  /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x02,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: CUSTOM_HID*/
  0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x09,         /*bLength: CUSTOM_HID Descriptor size*/
  CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: CUSTOM_HID*/
  0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

  CUSTOM_HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  CUSTOM_HID_EPIN_SIZE, /*wMaxPacketSize: 2 Byte max */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,          /*bInterval: Polling Interval */
  /* 34 */

  0x07,          /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT, /* bDescriptorType: */
  CUSTOM_HID_EPOUT_ADDR,  /*bEndpointAddress: Endpoint Address (OUT)*/
  0x03, /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPOUT_SIZE,  /* wMaxPacketSize: 2 Bytes max  */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,  /* bInterval: Polling Interval */
  /* 41 */
};


/* USB CUSTOM_HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_CUSTOM_HID_Desc[USB_CUSTOM_HID_DESC_SIZ] __ALIGN_END =
{
  /* 18 */
  0x09,         /*bLength: CUSTOM_HID Descriptor size*/
  CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: CUSTOM_HID*/
  0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_CUSTOM_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};
__ALIGN_BEGIN uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] __ALIGN_END = {
  USB_SIZ_STRING_SERIAL,
  USB_DESC_TYPE_STRING,
	0x36,0x00,
	0x44,0x00,
	0x38,0x00,
	0x46,0x00,
	0x33,0x00,
	0x36,0x00,
	0x38,0x00,
	0x34,0x00,
	0x34,0x00,
	0x38,0x00,
	0x35,0x00,
	0x32,0x00,
};





/**********************************************************/
UINT8   DevConfig;
UINT8   SetupReqCode;
UINT16  SetupReqLen;
const UINT8 *pDescr;

/******** 用户自定义分配端点RAM ****************************************/
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
                        errflag = 0xFF;				/* 非标准请求 */
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
									
									case USB_DESCR_TYP_REPORT:			// 上传报告描述符
										switch( (pSetupReqPak->wIndex)&0xff )
										{
											case 0:
												pDescr = (unsigned char*)&CUSTOM_HID_ReportDesc_FS;
												len = sizeof(CUSTOM_HID_ReportDesc_FS);
												break;
											
											default:
												errflag = 0xFF;   // 
										}
												break;
									
									case USB_DESCR_TYP_HID:					// 上传HID描述符
										switch( (pSetupReqPak->wIndex)&0xff )
										{
											case 0:
												pDescr = (unsigned char*)&USBD_CUSTOM_HID_Desc;
												len = sizeof(USBD_CUSTOM_HID_Desc);
												break;
											
											default:
												errflag = 0xFF;   // 
										}
												break;
									
//										case USB_DESC_TYPE_DEVICE_QUALIFIER:					// 上传QUALIF
//										switch( (pSetupReqPak->wIndex)&0xff )
//										{
//											case 0:
//												pDescr = (unsigned char*)&USBD_CUSTOM_HID_DeviceQualifierDesc;
//												len = sizeof(USBD_CUSTOM_HID_DeviceQualifierDesc);
//												break;
//											
//											default:
//												errflag = 0xFF;   // 
//										}
//												break;
									
									case USB_DESCR_TYP_STRING:
										switch( (pSetupReqPak->wValue)&0xff )
										{
											case 0:
												pDescr = MyLangDescr;
												len = MyLangDescr[0];
												break;
											case 1:
												pDescr = MyManuInfo;
												len = MyManuInfo[0];
												break;
											case 2:
												pDescr = MyProdInfo;
												len = MyProdInfo[0];
												break;
											case 3:
												pDescr = (unsigned char*)&USBD_StringSerial;
												len = sizeof(USBD_StringSerial);
												break;
											case 5:
												pDescr = MyConfInfo;
												len = MyConfInfo[0];
												break;
											case 4:
												pDescr = MyIntFInfo;
												len = MyIntFInfo[0];
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
					else	len = 0;		// 下传	
					
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
				len = R8_USB_RX_LEN;
				break;
			
			case UIS_TOKEN_OUT | 1:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK ) 
				{                       // 不同步的数据包将丢弃
					len = R8_USB_RX_LEN;
					DevEP1_OUT_Deal( len );
				}
				break;
			
			case UIS_TOKEN_IN | 1:
				R8_UEP1_CTRL = (R8_UEP1_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
				break;
			
			case UIS_TOKEN_OUT | 2:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK ) 
				{                       // 不同步的数据包将丢弃
					len = R8_USB_RX_LEN;
					DevEP2_OUT_Deal( len );
				}
				break;
			
			case UIS_TOKEN_IN | 2:
				R8_UEP2_CTRL = (R8_UEP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
				break;
			
			case UIS_TOKEN_OUT | 3:
				if ( R8_USB_INT_ST & RB_UIS_TOG_OK ) 
				{                       // 不同步的数据包将丢弃
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
		if ( R8_USB_MIS_ST & RB_UMS_SUSPEND ) {;}	// 挂起
		else		{;}								// 唤醒
		R8_USB_INT_FG = RB_UIF_SUSPEND;
	}
	else
	{
		R8_USB_INT_FG = intflag;
	}
}


// host send data / device recv data
extern uint8_t USBD_HID0_SetReport (uint8_t rtype, uint8_t req, uint8_t rid, const uint8_t *buf, int32_t len);
extern int32_t USBD_HID0_GetReport (uint8_t rtype, uint8_t req, uint8_t rid, uint8_t *buf);
static uint8_t Report_buf[64];
int32_t len;
void DevEP1_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */
//	UINT8 i;
//	for(i=0; i<l; i++)
//	{
//		pEP1_IN_DataBuf[i] = ~pEP1_OUT_DataBuf[i];
//	}
//	DevEP1_IN_Deal( l );
	
	// USB从机接收
	printf("EP1 Recv :");
	for(int i=0; i< USBD_CUSTOMHID_OUTREPORT_BUF_SIZE; i++)
	{
		printf(" %02X", pEP1_OUT_DataBuf[i]);
	}
	printf("\r\n");
	
	USBD_HID0_SetReport(HID_REPORT_OUTPUT, 0, 0, pEP1_OUT_DataBuf, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
}

void DevEP2_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP2_IN_DataBuf[i] = ~pEP2_OUT_DataBuf[i];
	}
	DevEP2_IN_Deal( l );
}

void DevEP3_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP3_IN_DataBuf[i] = pEP3_OUT_DataBuf[i];
	}
//	DevEP3_IN_Deal( l );
}

void DevEP4_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP4_IN_DataBuf[i] = ~pEP4_OUT_DataBuf[i];
	}
	DevEP4_IN_Deal( l );
}



void USB_IRQHandler (void)		/* USB中断服务程序,使用寄存器组1 */
{
	USB_DevTransProcess();
}

void USB_Init(void)
{
    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
    pEP2_RAM_Addr = EP2_Databuf;
    pEP3_RAM_Addr = EP3_Databuf;
    NVIC_EnableIRQ( USB_IRQn );
    USB_DeviceInit();
}

// USB 从机发送
uint8_t USBD_CUSTOM_HID_SendReport(uint8_t  eport,
                                   uint8_t *report,
                                   uint16_t len)
{
	if( len > HID_CDC_DATA_SIZE )	// 输入长度小于最大长度，直接发送
	{		
		
		switch( eport )
		{
			case USB_EP1_IN_ADDR:
				printf("EP1 IN\r\n");
				memcpy( pEP1_IN_DataBuf, report, len );
				DevEP1_IN_Deal( len );
			
				len = USBD_HID0_GetReport(HID_REPORT_INPUT, USBD_HID_REQ_EP_INT, 0, Report_buf);
				if( len > 0 )
				{
					USBD_HID_GetReportTrigger(0, 0, Report_buf, len);
				}
				break;
			case USB_EP2_IN_ADDR:
				memcpy( pEP2_IN_DataBuf, report, len );
				DevEP2_IN_Deal( len );
				break;
			case USB_EP3_IN_ADDR:
				memcpy( pEP3_IN_DataBuf, report, len );
				DevEP3_IN_Deal( len );
				break;
			case USB_EP4_IN_ADDR:
				memcpy( pEP4_IN_DataBuf, report, len );
				DevEP4_IN_Deal( len );
				break;
			default:
				return -1;
		}
	}else
		return -1;
	return 0;
}


