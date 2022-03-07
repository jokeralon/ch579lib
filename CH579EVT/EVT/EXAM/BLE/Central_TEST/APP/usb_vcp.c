#include "usb_vcp.h"
#include "CH57x_common.h"
#include "string.h"

#define DevEP0SIZE 0x40

char UserTxBufferFS[HID_CDC_DATA_SIZE] ={0};

/* HID的报告描述符*/
/*定义了8字节发送：
**  第一字节表示特殊件是否按下：D0:Ctrl D1:Shift D2:Alt
**  第二字节保留，值为0
**  第三~第八字节:普通键键值的数组,最多能同时按下6个键
**定义了1字节接收:对应键盘上的LED灯,这里只用了两个位。
**	D0:Num Lock   D1:Cap Lock   D2:Scroll Lock   D3:Compose   D4:Kana*/

// 设备描述符
const UINT8  MyDevDescr[] = { 
//0x12, 0x01, 0x10, 0x01,0xFF, 0x80, 0x55, DevEP0SIZE,							 
//0x48, 0x43, 0x37, 0x55,  					// 厂商ID和产品ID
//0x00, 0x01, 0x01, 0x02, 0x00, 0x01,
	
        0x12, /*bLength：长度，设备描述符的长度为18字节*/
        0x01, /*bDescriptorType：类型，设备描述符的编号是0x01*/
        0x10, /*bcdUSB：所使用的USB版本为2.0*/
        0x01,
        0x02, /*bDeviceClass：设备所使用的类代码*/
        0x00, /*bDeviceSubClass：设备所使用的子类代码*/
        0x00, /*bDeviceProtocol：设备所使用的协议*/
        HID_CDC_DATA_SIZE, /*bMaxPacketSize：最大包长度为8字节*/
        0x83, /*idVendor：厂商ID为0x7788*/
        0x04,
        0x40, /*idProduct：产品ID为0x1122*/
        0x57,
        0x00, /*bcdDevice：设备的版本号为2.00*/
        0x02,
        1,   /*iManufacturer:厂商字符串的索引*/
        2,   /*iProduct：产品字符串的索引*/
        3,   /*iSerialNumber：设备的序列号字符串索引*/
        0x01 /*bNumConfiguration：设备有1种配置*/
};
// 配置描述符
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
	
				0x09,                              /*bLength：长度，设备字符串的长度为9字节*/
        USB_CONFIGURATION_DESCRIPTOR_TYPE, /*bDescriptorType：类型，配置描述符的类型编号为0x2*/
        IAD_HID_CDC_SIZ_CONFIG_DESC,       /*wTotalLength：配置描述符的总长度为0x43字节*/
        0x00,
        0x02, /*bNumInterfaces：配置所支持的接口数量1个*/
        0x01, /*bConfigurationValue：该配置的值*/
        0x00, /*iConfiguration：该配置的字符串的索引值，该值为0表示没有字符串*/
        0xC0, /* bmAttributes:设备的一些特性，0xc0表示自供电，不支持远程唤醒
						D7:保留必须为1，D6:是否自供电，D5：是否支持远程唤醒，D4~D0：保留设置为0*/
        0x32, /*从总线上获得的最大电流为100mA */
              //    0x96,         /*MaxPower：设备需要从总线上获取多少电流，单位为2mA，0x96表示300mA*/
  /* Interface descriptor type */
              /************** Descriptor of virtual com port ****************/
        /*Interface Descriptor*/
        0x09, 											/* bLength: Interface Descriptor size */											/*bLength：长度，接口描述符的长度为9字节 */
        0x04, 											/* bDescriptorType: Interface */                              /* bDescriptorType：接口描述符的类型为0x4 */
        0x00,											 	/* bInterfaceNumber: Number of Interface */                   /*bInterfaceNumber：该接口的编号*/
        0x00, 											/* bAlternateSetting: Alternate setting */                    /*bAlternateSetting：该接口的备用编号 */
        0x01, 											/* bNumEndpoints: One endpoints used */                       /*bNumEndpoints：该接口所使用的端点数*/
        0x02, 											/* bInterfaceClass: Communication Interface Class */          /*bInterfaceClass该接口所使用的类为CDC*/
        0x02, 											/* bInterfaceSubClass: Abstract Control Model */              /*bInterfaceSubClass：该接口所用的子类 1=BOOT, 0=no boot */
        0x01, 											/* bInterfaceProtocol: Common AT commands */                  /*nInterfaceProtocol :该接口使用的协议0=none, 1=keyboard, 2=mouse */
        0x00, 											/* iInterface: */                                             /*iInterface: 该接口字符串的索引 */
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
        /******************** EP2 端点描述符******************/
                                      /* 27 */
        0x07,                         /* bLength: 端点描述符的长度为7字节*/
        USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: 端点描述符的类型为0x05*/
        0x82,                         /* bEndpointAddress: 该端点(输入)的地址,D7:0(OUT),1(IN),D6~D4:保留,D3~D0:端点号*/
        0x03,                         /* bmAttributes: 端点的属性为为中断端点.
			D0~D1表示传输类型:0(控制传输),1(等时传输),2(批量传输),3(中断传输)
			非等时传输端点:D2~D7:保留为0
			等时传输端点：
			D2~D3表示同步的类型:0(无同步),1(异步),2(适配),3(同步)
			D4~D5表示用途:0(数据端点),1(反馈端点),2(暗含反馈的数据端点),3(保留)，D6~D7:保留,*/
        HID_CDC_DATA_SIZE,                         /* wMaxPacketSize: 该端点支持的最大包长度为8字节*/
        0x00,
        0xFF, /* bInterval: 轮询间隔(32ms) */
        /************** CDC 接口描述符****************/
        /* 09 */
        0x09,                          /*bLength：长度，接口描述符的长度为9字节 */
        USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType：接口描述符的类型为0x4 */
        0x01,                          /*bInterfaceNumber：该接口的编号*/
        0x00,                          /*bAlternateSetting：该接口的备用编号 */
        0x02,                          /*bNumEndpoints：该接口所使用的端点数*/
        0x0A,                          /*bInterfaceClass该接口所使用的类为CDC*/
        0x00,                          /*bInterfaceSubClass：该接口所用的子类 1=BOOT, 0=no boot */
        0x00,                          /*nInterfaceProtocol :该接口使用的协议0=none, 1=keyboard, 2=mouse */
        0,                             /*iInterface: 该接口字符串的索引 */
        /******************** EP3 OUT 端点描述符******************/
                                       /* 27 */
        0x07,                          /* bLength: 端点描述符的长度为7字节*/
        USB_ENDPOINT_DESCRIPTOR_TYPE,  /* bDescriptorType: 端点描述符的类型为0x05*/
        0x03,                          /* bEndpointAddress: 该端点(输入)的地址,D7:0(OUT),1(IN),D6~D4:保留,D3~D0:端点号*/
        0x02,                          /* bmAttributes: 端点的属性为为中断端点.
				D0~D1表示传输类型:0(控制传输),1(等时传输),2(批量传输),3(中断传输)
				非等时传输端点:D2~D7:保留为0
				等时传输端点：
				D2~D3表示同步的类型:0(无同步),1(异步),2(适配),3(同步)
				D4~D5表示用途:0(数据端点),1(反馈端点),2(暗含反馈的数据端点),3(保留)，D6~D7:保留,*/
				
        HID_CDC_DATA_SIZE,                          /* wMaxPacketSize: 该端点支持的最大包长度为8字节*/
        0x00,
        0x00,                         /* bInterval: 轮询间隔(32ms) */
                                      /******************** EP3 IN 端点描述符******************/
                                      /* 27 */
        0x07,                         /* bLength: 端点描述符的长度为7字节*/
        USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: 端点描述符的类型为0x05*/
        0x83,                         /* bEndpointAddress: 该端点(输入)的地址,D7:0(OUT),1(IN),D6~D4:保留,D3~D0:端点号*/
        0x02,                         /* bmAttributes: 端点的属性为为中断端点.
			D0~D1表示传输类型:0(控制传输),1(等时传输),2(批量传输),3(中断传输)
			非等时传输端点:D2~D7:保留为0
			等时传输端点：
			D2~D3表示同步的类型:0(无同步),1(异步),2(适配),3(同步)
			D4~D5表示用途:0(数据端点),1(反馈端点),2(暗含反馈的数据端点),3(保留)，D6~D7:保留,*/
        HID_CDC_DATA_SIZE,                         /* wMaxPacketSize: 该端点支持的最大包长度为8字节*/
        0x00,
        0x00, /* bInterval: 轮询间隔(32ms) */
				
};
// 语言描述符
const UINT8  MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };
// 厂家信息
const UINT8  MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
// 产品信息
const UINT8  MyProdInfo[] = { 0x0C, 0x03, 'C', 0, 'H', 0, '5', 0, '7', 0, 'x', 0 };

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
//    PWR_UnitModCfg( ENABLE, UNIT_SYS_PLL );		// 打开PLL
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
{ /* 用户可自定义 */
	UINT8 i;
	for(i=0; i<l; i++)
	{
		pEP1_IN_DataBuf[i] = ~pEP1_OUT_DataBuf[i];
	}
	DevEP1_IN_Deal( l );
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
	usb_printf("DevEP3_OUT_Deal\r\n");
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


/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

#include <stdarg.h>

 void USB_VCP_SendByte(UINT8 ch)
{
	pEP3_IN_DataBuf[0] = ch;
	DevEP3_IN_Deal( 1 );
}
int USB_VCP_SendStr(UINT8 *str, int str_len)
{
	if( str_len < HID_CDC_DATA_SIZE )	// 输入长度小于最大长度，直接发送
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



void USB_IRQHandler (void)		/* USB中断服务程序,使用寄存器组1 */
{
	USB_DevTransProcess();
}

