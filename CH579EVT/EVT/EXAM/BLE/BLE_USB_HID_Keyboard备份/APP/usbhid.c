/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2018/12/15
* Description        : 自定义USB设备（CH372设备），提供8个非0通道(上传+下传)，实现数据先下传，然后数据内容取反上传
*******************************************************************************/

#include "CONFIG.h"
#include "CH57x_common.h"
#include "CH57xBLE_LIB.h"
#include "hidkbd.h"
#include "key.h"


#define     DevEP0SIZE					0x40

#define     HIDCONFIGDESCLEN			41

#define     GAMEPAD_SIZ_REPORT_DESC			     61
#define     KEYBOARD_SIZ_REPORT_DESC    		 67
#define     MOUSE_SIZ_REPORT_DESC    			 62	


// 设备描述符
const UINT8  MyDevDescr[] = { 
0x12, 														//1b bLength
0x01, 														//1b bDescriptorType
0x10, 0x01,												//2b bcdUSB
0x00, 														//1b bDeviceClass
0x00, 														//1b bDeviceSubClass
0x00,															//1b bDeviceProtocol
DevEP0SIZE,							 					//1b bMaxPackeerSize0
0x48, 0x43, 											//2b idVendor	 厂商ID
0x00, 0x55,  											//2b idProduct 产品ID
0x00, 0x01, 											//2b bcdDevice
0x01, 														//1b iManufacturer
0x02, 														//1b iProduct
0x00, 														//1b iSerialNumber
0x01,															//1b iNumConfigurations
};	

// 配置描述符
const UINT8  MyCfgDescr[] = { 
// configuration_descriptor hid_configuration_descriptor
   0x09,                               // 1b bLength			
   0x02,                               // 1b bDescriptorType
   HIDCONFIGDESCLEN,0X00,							 // 2b wTotalLength (= 9+9+9+7+7)
   0x01,                               // 1b bNumInterfaces
   0x01,                               // 1b bConfigurationValue
   0x00,                               // 1b iConfiguration
   0xC0,                               // 1b bmAttributes
   0xc0,                               // 1b bMaxPower (in 2mA units)

// interface_descriptor hid_interface_descriptor
   0x09,                               //1b bLength
   0x04,                               //1b bDescriptorType
   0x00,                               //1b bNumInterfaces	接口号
   0x00,                               //1b bAlterateSetting	可替换设置值
   0x02,                               //1b bNumEndpoints	 	端点0以外的端点数
   0x03,                               //1b bInterfaceClass (3 = HID)
   0x01,                               //1b bInterSubfaceClass
   0x01,                               //1b bInterfaceProtocol
   0x00,                               //1b iInterface		 
// class_descriptor hid_descriptor
   0x09,                               // bLength
   0x21,                               // bDescriptorType
   0x10,0x01,                          // bcdHID
   0x21,                               // bCountryCode
   0x01,                               // bNumDescriptors
   0x22,                               // bDescriptorType
   GAMEPAD_SIZ_REPORT_DESC,0X00,		   // wItemLength (tot. len. of report descriptor)
// IN endpoint (mandatory for HID)
// endpoint1_descriptor hid_endpoint_in_descriptor
   0x07,                               // bLength
   0x05,                               // bDescriptorType
   0x81,                               // bEndpointAddress
   0x03,                               // bmAttributes	00-????,01-????,10-???,11-????
   0x08,0x00,                 		   // MaxPacketSize (LITTLE ENDIAN)
   0x0A,                               // bInterval
// OUT endpoint (optional for HID)
// endpoint1_descriptor hid_endpoint_out_descriptor
   0x07,                               // bLength
   0x05,                               // bDescriptorType
   0x01,                               // bEndpointAddress
   0x03,                               // bmAttributes
   0x01,0x00,		                   // MaxPacketSize (LITTLE ENDIAN)
   0x0A,                               // bInterval



};
// 语言描述符
const UINT8  MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };
// 厂家信息
const UINT8  MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
// 产品信息
const UINT8  MyProdInfo[] = { 0x0C, 0x03, 'C', 0, 'H', 0, '5', 0, '7', 0, 'x', 0 };

/* HID的报告描述符*/
/*定义了8字节发送：
**  第一字节表示特殊件是否按下：D0:Ctrl D1:Shift D2:Alt
**  第二字节保留，值为0
**  第三~第八字节:普通键键值的数组,最多能同时按下6个键
**定义了1字节接收:对应键盘上的LED灯,这里只用了两个位。
**	D0:Num Lock   D1:Cap Lock   D2:Scroll Lock   D3:Compose   D4:Kana*/
const unsigned char  HIDREPORTDESC[]=
{
	/*short Item   D7~D4:bTag;D3~D2:bType;D1~D0:bSize
	**bTag ---主条目  	1000:输入(Input) 1001:输出(Output) 1011:特性(Feature)	1010:集合(Collection) 1100:关集合(End Collection) 
	**		  全局条目 	0000:用途页(Usage Page) 0001:逻辑最小值(Logical Minimum) 0010:逻辑最大值(Logical Maximum) 0011:物理最小值(Physical Minimum)
	**					0100:物理最大值(Physical Maximum) 0101:单元指数(Unit Exponet) 0110:单元(Unit) 0111:数据域大小(Report Size)
	**					1000:报告ID(Report ID) 1001:数据域数量(Report Count) 1010:压栈(Push) 1011:出栈(Pop) 1100~1111:保留(Reserved)
	**		  局部条目	0000:用途(Usage) 0001:用途最小值(Usage Minimum) 0010:用途最大值(Usage Maximum) 0011:标识符索引(Designator Index)
	**					0100:标识符最小值(Designator Minimum) 0101:标识符最大值(Designator Maximum) 0111:字符串索引(String Index) 1000:字符串最小值(String Minimum)   
	**					1001:字符串最大值(String Maximum) 1010:分隔符(Delimiter) 其他：保留(Reserved)
	**bType---00:主条目(main)  01:全局条目(globle)  10:局部条目(local)  11:保留(reserved)
	**bSize---00:0字节  01:1字节  10:2字节  11:4字节*/
	
	//0x05:0000 01 01 这是个全局条目，用途页选择为普通桌面页
	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
	//0x09:0000 10 01 这是个全局条目，用途选择为键盘
	0x09, 0x06, // USAGE (Keyboard)
	//0xa1:1010 00 01 这是个主条目，选择为应用集合，
	0xa1, 0x01, // COLLECTION (Application)
	//0x05:0000 01 11 这是个全局条目，用途页选择为键盘/按键
	0x05, 0x07, // USAGE_PAGE (Keyboard/Keypad)

	//0x19:0001 10 01 这是个局部条目，用途的最小值为0xe0，对应键盘上的左ctrl键
	0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
	//0x29:0010 10 01 这是个局部条目，用途的最大值为0xe7，对应键盘上的有GUI(WIN)键
	0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
	//0x15:0001 01 01 这是个全局条目，说明数据的逻辑值最小值为0
	0x15, 0x00, // LOGICAL_MINIMUM (0)
	//0x25:0010 01 01 这是个全局条目，说明数据的逻辑值最大值为1
	0x25, 0x01, // LOGICAL_MAXIMUM (1)

	//0x95:1001 01 01 这是个全局条目，数据域的数量为8个
	0x95, 0x08, // REPORT_COUNT (8)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为1位
	0x75, 0x01, // REPORT_SIZE (1)	   
	//0x81:1000 00 01 这是个主条目，有8*1bit数据域作为输入，属性为:Data,Var,Abs
	0x81, 0x02, // INPUT (Data,Var,Abs)

	//0x95:1001 01 01 这是个全局条目,数据域的数量为1个
	0x95, 0x01, // REPORT_COUNT (1)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为8位
	0x75, 0x08, // REPORT_SIZE (8)
	//0x81:1000 00 01 这是个主条目，有1*8bit数据域作为输入，属性为:Cnst,Var,Abs
	0x81, 0x03, // INPUT (Cnst,Var,Abs)

	//0x95:1001 01 01 这是个全局条目，数据域的数量为6个
	0x95, 0x06, // REPORT_COUNT (6)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为8位
	0x75, 0x08, // REPORT_SIZE (8)
	//0x25:0010 01 01 这是个全局条目，逻辑最大值为255
	0x25, 0xFF, // LOGICAL_MAXIMUM (255)
	//0x19:0001 10 01 这是个局部条目，用途的最小值为0
	0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
	//0x29:0010 10 01 这是个局部条目，用途的最大值为0x65
	0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
	//0x81:1000 00 01 这是个主条目，有6*8bit的数据域作为输入，属相为属性为:Data,Var,Abs
	0x81, 0x00, // INPUT (Data,Ary,Abs)

	//0x25:0010 01 01 这是个全局条目，逻辑的最大值为1
	0x25, 0x01, // LOGICAL_MAXIMUM (1)
	//0x95:1001 01 01 这是个全局条目，数据域的数量为2
	0x95, 0x02, // REPORT_COUNT (2)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为1位
	0x75, 0x01, // REPORT_SIZE (1)
	//0x05:0000 01 01 这是个全局条目，用途页选择为LED页
	0x05, 0x08, // USAGE_PAGE (LEDs)
	//0x19:0001 10 01 这是个局部条目，用途的最小值为0x01,对应键盘上的Num Lock
	0x19, 0x01, // USAGE_MINIMUM (Num Lock)
	//0x29:0010 10 01 这是个局部条目，用途的最大值为0x02,对应键盘上的Caps Lock
	0x29, 0x02, // USAGE_MAXIMUM (Caps Lock)
	//0x91:1001 00 01 这是个主条目，有2*1bit的数据域作为输出，属性为:Data,Var,Abs
	0x91, 0x02, // OUTPUT (Data,Var,Abs)

	//0x95:1001 01 01 这是个全局条目，数据域的数量为1个
	0x95, 0x01, // REPORT_COUNT (1)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为6bit,正好与前面的2bit组成1字节
	0x75, 0x06, // REPORT_SIZE (6)
	//0x91:1001 00 01 这是个主条目，有1*6bit数据域最为输出，属性为:Cnst,Var,Abs
	0x91, 0x03, // OUTPUT (Cnst,Var,Abs)

	0xc0        // END_COLLECTION
}; 
/**********************************************************/
UINT8   Ready;
UINT8   DevConfig;
UINT8   SetupReqCode;
UINT16  SetupReqLen;
const UINT8 *pDescr;


/******** 用户自定义分配端点RAM ****************************************/
__align(4) UINT8 EP0_Databuf[64+64+64];	//ep0(64)+ep4_out(64)+ep4_in(64)
__align(4) UINT8 EP1_Databuf[64+64];	//ep1_out(64)+ep1_in(64)
__align(4) UINT8 EP2_Databuf[64+64];	//ep1_out(64)+ep1_in(64)
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
						switch(SetupReqCode){
							case 0x0a:break;		//这个一定要有
							case 0x09:break;
							default: errflag = 0xFF;
						}
							
              //errflag = 0xFF;				/* 非标准请求 */
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
									case USB_DESCR_TYP_REPORT:
										switch( (pSetupReqPak->wIndex)&0xff )
										{
											case 0:
												pDescr = (unsigned char*)&HIDREPORTDESC;
												len = GAMEPAD_SIZ_REPORT_DESC;
												break;
											default:
												errflag = 0xFF;   // 
										}
												break;
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
											default:
												errflag = 0xFF;   // 不支持的字符串描述符
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
				if(SetupReqCode == 0x09)
        {
					if(pEP0_DataBuf[0])
					{
							printf("Light on Num Lock LED!\n");
					}
					else if(pEP0_DataBuf[0] == 0)
					{
							printf("Light off Num Lock LED!\n");
					}
         }
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
 
 
uint8_t Usbhid_TaskId;

uint16_t UsbHid_ProcessEvent( uint8_t task_id, uint16_t events );

void UsbHid_Init( void)
{
		Usbhid_TaskId = TMOS_ProcessEventRegister(UsbHid_ProcessEvent);
    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
		pEP1_RAM_Addr = EP2_Databuf;
    USB_DeviceInit();
    NVIC_EnableIRQ( USB_IRQn );  
    
    // Setup a delayed profile startup
//    tmos_set_event( Usbhid_TaskId, START_DEVICE_EVT );
//    HAL_KEY_RegisterForKeys(task_id);
}


static void Usbhid_ProcessTMOSMsg( tmos_event_hdr_t *pMsg )
{
    uint8_t keyxor;
    keyChange_t *pKey = (keyChange_t *)pMsg;
    
    switch ( pMsg->event )
    {
        case KEY_CHANGE:
 //           keyxor = pKey->keys ^ pKey->keyold;
            if(keyxor)
            {
                tmos_set_event( Usbhid_TaskId, START_REPORT_EVT );
            }
            
            if(keyxor & 0x01)
            {
                if(pKey->keys &0x01)    //按下SW1
                {
                    PRINT("KEY1_PRESS.\n");
                }else{
                    PRINT("KEY1_RELESE.\n");
                }
            }
            
            if(keyxor & 0x02)
            {
                if(pKey->keys &0x02)    //按下SW2
                {
                    PRINT("KEY2_PRESS.\n");
                }else{
                    PRINT("KEY2_RELESE.\n");
                }
            }
            
            if(keyxor & 0x04)
            {
                if(pKey->keys &0x04)    //按下SW3
                {
                    PRINT("KEY3_PRESS.\n");
                }else{
                    PRINT("KEY3_RELESE.\n");
                }
            }
            
            if(keyxor & 0x08)
            {
                if(pKey->keys &0x08)    //按下SW4
                {
                    PRINT("KEY4_PRESS.\n");
                }else{
                    PRINT("KEY4_RELESE.\n");
                }
            }            
            
        break;
        
        default:			
        break;
    }
}

uint8_t  ctrl_data;
uint16_t UsbHid_ProcessEvent( uint8_t task_id, uint16_t events )
{
    PRINT("%s IN,evt=%X\n",__func__,events);
  
    if ( events & SYS_EVENT_MSG )
    {
        uint8 *pMsg;

        if ( (pMsg = tmos_msg_receive( Usbhid_TaskId )) != NULL )
        {
            Usbhid_ProcessTMOSMsg( (tmos_event_hdr_t *)pMsg );

            // Release the TMOS message
            tmos_msg_deallocate( pMsg );
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if ( events & START_DEVICE_EVT )
    {
        tmos_start_task( Usbhid_TaskId, START_GAMEPAD_REPORT_EVT, 1000 );
        return ( events ^ START_DEVICE_EVT );
    }

    if ( events & START_REPORT_EVT )
    {   
		if( ctrl_data & 0x01)
		{
			if(!GPIOA_ReadPortPin(GPIO_Pin_12))
			{
				pEP1_IN_DataBuf[6] |= 0x01;
			}else
			{
				pEP1_IN_DataBuf[6] &= ~0x01;
			}
			
			if(!GPIOA_ReadPortPin(GPIO_Pin_13))
			{
				pEP1_IN_DataBuf[6] |= 0x02;
			}else
			{
				pEP1_IN_DataBuf[6] &= ~0x02;
			}
			
			if(!GPIOA_ReadPortPin(GPIO_Pin_14))
			{
				pEP1_IN_DataBuf[6] |= 0x04;
			}else
			{
				pEP1_IN_DataBuf[6] &= ~0x04;
			}
			
			if(!GPIOA_ReadPortPin(GPIO_Pin_15))
			{
				pEP1_IN_DataBuf[6] |= 0x08;
			}else
			{
				pEP1_IN_DataBuf[6] &= ~0x08;
			}
			DevEP1_IN_Deal( 7 );
            
		}
        
        if( ctrl_data & 0x02 )
		{
			if(!GPIOA_ReadPortPin(GPIO_Pin_12))
			{
				pEP2_IN_DataBuf[0] |= 0x01;
			}else
			{
				pEP2_IN_DataBuf[0] &= ~0x01;
			}
			
			if(!GPIOA_ReadPortPin(GPIO_Pin_13))
			{
				pEP2_IN_DataBuf[0] |= 0x02;
			}else
			{
				pEP2_IN_DataBuf[0] &= ~0x02;
			}
			
			if(!GPIOA_ReadPortPin(GPIO_Pin_14))
			{
				pEP2_IN_DataBuf[0] |= 0x04;
			}else
			{
				pEP2_IN_DataBuf[0] &= ~0x04;
			}
			
			if(!GPIOA_ReadPortPin(GPIO_Pin_15))
			{
				pEP2_IN_DataBuf[0] |= 0x08;
			}else
			{
				pEP2_IN_DataBuf[0] &= ~0x08;
			}
			DevEP2_IN_Deal( 8 );
		} 
        
        if( ctrl_data & 0x04 )//
		{
			if(!GPIOA_ReadPortPin(GPIO_Pin_12))
			{
				pEP3_IN_DataBuf[1] = 1;
			}else if(!GPIOA_ReadPortPin(GPIO_Pin_13))
			{
				pEP3_IN_DataBuf[1] = 0xff;
			}else
			{
				pEP3_IN_DataBuf[1] = 0;
			}
			
			if(!GPIOA_ReadPortPin(GPIO_Pin_14))
			{
				pEP3_IN_DataBuf[2] = 1;
			}else if(!GPIOA_ReadPortPin(GPIO_Pin_15))
			{
				pEP3_IN_DataBuf[2] = 0xff;
			}else
			{
				pEP3_IN_DataBuf[2] = 0;
			}
			DevEP3_IN_Deal( 4 );
		} 
        
        return ( events ^ START_REPORT_EVT );
    }
		
    return 0;

}

void DevEP1_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */

}

void DevEP2_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */

}

void DevEP3_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */
	
}

void DevEP4_OUT_Deal( UINT8 l )
{ /* 用户可自定义 */
//	UINT8 i;
	
//	for(i=0; i<l; i++)
//	{
//		pEP4_IN_DataBuf[i] = ~pEP4_OUT_DataBuf[i];
//	}
//	DevEP4_IN_Deal( l );
}

void USB_IRQHandler (void)		/* USB中断服务程序,使用寄存器组1 */
{
	PRINT("\r\nUSB Connection\r\n");
	USB_DevTransProcess();
}


