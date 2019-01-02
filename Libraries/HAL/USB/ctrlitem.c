/***************************************************************************
* File name    :	CtrlItem.c
* Function     :	Do with Usb chapter 9 command to enum device
* Author       : 	howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :     
* ModifyRecord :
****************************************************************************/
#include "AS569.h"
 
SETUPSTATUS gSetupStatus;
 
/*=========================== Each Request =============================
do with the usb setup cmd,the command define refer to usb1.1 spec chapter 9
=======================================================================*/
//------------Descriptor of USB Device---------------------------  
const UINT8 DeviceDescFS[18]= 
{     
	0x12,  //bLength          //设备描述符的字节数大小，为0x12
	0x01,  //bDescriptorType  //设备描述符类型编号，为0x01
	0x10,  //bcdUSB           //USB规范版本号（BCD码）  0x0110
	0x01,  
	0x00,  //bDeviceClass     //USB分配的设备类代码，0x01~0xfe 为标准设备类，0xff 为厂商自定义类型 0x00 不是在设备描述符中定义的，如HID
	0x00,  //bDeviceSubClass  //USB分配的设备子类代码，同上，值由USB 规定和分配的
	0x00,  //bDeviceProtocl   //USB分配的设备协议代码，同上
	CTRL_PACKET_LEN, //bMaxPacketSize0 //端点0控制传输所支持的最大数据包长度，单位字节
	0x12,  //idVendor         //厂商编号VID=0x2012
	0x20,
	0x11,  //idProduct        //产品编号PID=0x2011
	0x20,
	0x00,  //bcdDevice        //设备出厂编号bcdDevice=0x0100
	0x00,  
	0x00,  //iManufacturer    //描述厂商字符串的索引Index of Vendor
	0x00,  //iProduct         //描述产品字符串的索引Index of Producr
	0x00,  //iSerialNumber    //描述设备序列号字符串的索引0x03;//Index of SN
	0x01   //bNumConfiguration//可能的配置数量
}; 


const UINT8 ConfigDescFs[32] = 
{  // -----------Configuration descriptor--------------
	0x09, //bLength               //配置描述符的字节数大小
	0x02, //bDescriptorType       //配置描述符类型编号
	0x20, //wTotalLength          //配置所返回的所有数量的大小
	0x00, 
	0x01, //bNumInterfaces        //此配置所支持的接口数量 
	0x01, //bCongfigurationValue  //Set_Configuration 命令需要的参数值
	0x00, //iConfiguration        //描述该配置的字符串的索引值
	0xa0, //bmAttributes          //供电模式的选择
	0x64, //MaxPower 200mA        //设备从总线提取的最大电流
	//---------------- Interface descriptor-----------
	0x09, //bLength               //接口描述符的字节数大小
	0x04, //bDescriptorType       //接口描述符类型编号
	0x00, //bInterfaceNumber      //接口的编号
	0x00, //bAlternateSetting     //备用的接口描述符编号
	0x02, //bNumEndpoints         //该接口使用端点数，不包括端点0
	0x08, //bInterfaceClass is scsi class    //接口类型
	0x06, //bInterfaceSubClass is scsi class //接口子类型
	0x50, //bInterfaceProtocol               //接口所遵循的协议
	0x00, //iInterface	 	                 //描述该接口的字符串索引值
	//---------------endpoint1 descriptor---------------- 
	0x07, //bLength                         //端点描述符的字节数大小
	0x05, //bDescriptorType                 //端点描述符类型编号
	EP1IN_NUMBER, //bEndpointAddress    //端点地址及输入输出属性
	0x02, //bmAttributes                    //端点的传输类型属性
	0x40, //wMaxPacketSize  //端点收、发的最大包的大小
	0x00, 
	0x00, //bInterval	                    //主机查询端点的时间间隔
	//---------------endpoint2 descriptor---------------- 	
	0x07, //bLength                         //端点描述符的字节数大小 
	0x05, //bDescriptorType                 //端点描述符类型编号
	EP1OUT_NUMBER, //bEndpointAddress   //端点地址及输入输出属性
	0x02, //bmAttributes                    //端点的传输类型属性
	0x40, //wMaxPacketSize  //端点收、发的最大包的大小
	0x00, 
	0x00  //bInterval 	                    //主机查询端点的时间间隔	
};

                         
const UINT8 Str0Desc[4] = 
{ 
	0x04,       //bLength;          //字符描述符的字节数大
	0x03,       //bDescriptorType   //字符描述符类型编号
	0x09, 0x04  //SomeDescriptor[]  //UNICODE 编码的字符串
};
 
const UINT8 Str1Desc[24] = 
{ 
	0x18, 
	0x03, 
	'U', 0x00,'S',0x00,'B',0x00,'K',0x00,'e',0x00,'y',0x00,
	' ',0x00,'C',0x00,'h',0x00,'i',0x00,'p',0x00
};	//Vendor*/
 
const UINT8 Str2Desc[30] = 
{ 
	0x1e, 
	0x03, 
	'U', 0x00, 'S', 0x00, 'B', 0x00, 'K', 0x00, 'e', 0x00, 'y', 0x00, ' ', 0x00, 
	'M', 0x00, 'o', 0x00, 'd', 0x00, 'u', 0x00, 'l', 0x00,'e',0x00
};	//Product info
                          
const UINT8 Str3Desc[22] = 
{ 
	0x16, 
	0x03, 
	'2', 0x00, '0', 0x00, '2', 0x00, '7', 0x00, '3', 0x00,
	'0', 0x00, '0', 0x00, '4', 0x00, '1', 0x00, '3', 0x00
};	//SN 
                            
const UINT8 StrDescLenGroup[4] = 
{
	sizeof(Str0Desc),
	sizeof(Str1Desc),
	sizeof(Str2Desc),
	sizeof(Str3Desc)
};
 

/***********************************************************************************
 * Subrountine:	Chap9_SetAddress
 * Function:	Set Address command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_SetAddress(SETUPPACK* SetupPack)	    //0x05
{
  
	USB_SendACK(ENDPOINT0); 
	
	USB_SetAddress(SetupPack->wValue); 
    
}

/***********************************************************************************
 * Subrountine:	Chap9_GetDescriptor
 * Function:	Get Descriptor command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_GetDescriptor(SETUPPACK* SetupPack)     //0x06
{	   
    UINT8 *pCtrlBuf=0 ; 
    UINT8 temp,desType;  
    UINT8 count;    
    UINT8 Flag = 0; 

    desType = (SetupPack->wValue>>8) & 0xFF;

    switch(desType)
    {
		case DEVICE_DESC_TYPE:
        {
        	if(SetupPack->wLength > sizeof(DeviceDescFS))   
        		SetupPack->wLength = sizeof(DeviceDescFS);
         
        	pCtrlBuf = (UINT8*)DeviceDescFS; 
                 
            break;
        }
        case CONFIGURATION_DESC_TYPE:
        {
             
        	if(SetupPack->wLength > sizeof(ConfigDescFs))
        		SetupPack->wLength = sizeof(ConfigDescFs);
        	pCtrlBuf = (UINT8*)ConfigDescFs;

			UsbRegs.INDEX=ENDPOINT1;
        	UsbRegs.INMAXP=FS_MAX_PACK_LEN;
       		UsbRegs.OUTMAXP=FS_MAX_PACK_LEN;
           	UsbRegs.INDEX=ENDPOINT0;
            
            break;
        }
            
        case STRING_DESC_TYPE :
        {
            temp = (SetupPack->wValue & 0xff);
            if(temp == 3)
            {
            	if(SetupPack->wLength > 22)   
            		SetupPack->wLength = 22;
            	pCtrlBuf = (UINT8*)Str3Desc;              
             
            }
            else if(temp < 3)
            {
            	if(SetupPack->wLength > StrDescLenGroup[temp])   
            		SetupPack->wLength = StrDescLenGroup[temp];
            	if(temp == 0)
                    pCtrlBuf = (UINT8*)Str0Desc;
            	else if(temp==1)
                	 pCtrlBuf = (UINT8*)Str1Desc;
            	else
                     pCtrlBuf = (UINT8*)Str2Desc;
            }else
            {
                SetupPack->wLength = 0;
            }
            break;
        }
		case TEST_BOS_TYPE:		// report descriptor
		{
			Flag = 1;
        	SetupPack->wLength = 0;
            break ;
		}
        default:
        {
        	SetupPack->wLength = 0;
            break ;
        }
      }
	if(Flag)
	{
		USB_SendStall_In(ENDPOINT0);
	}
	else
	{
		count = SetupPack->wLength;    
		USB_CtrlSend(pCtrlBuf, count); 
		if((count%CTRL_PACKET_LEN) == 0)
			USB_SendZero(ENDPOINT0); 
	}	
	return;           
}

/***********************************************************************************
 * Subrountine:	Chap9_SetConfiguration
 * Function:	Set Configuration command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_SetConfiguration(SETUPPACK* SetupPack)	//0x09
{
	gSetupStatus.bConfigNum = SetupPack->wValue;

	UsbRegs.INDEX=ENDPOINT1;
    UsbRegs.INMAXP=FS_MAX_PACK_LEN;
    UsbRegs.OUTMAXP=FS_MAX_PACK_LEN;
    UsbRegs.INDEX=ENDPOINT0; 
	USB_SendACK(ENDPOINT0);
}

/***********************************************************************************
 * Subrountine:	Chap9_SendACK
 * Function:	Default command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_SendACK(SETUPPACK* SetupPack)
{           
	USB_SendACK(ENDPOINT0);
} 

/***********************************************************************************
 * Subrountine:	Chap9_GetStatus
 * Function:	Get Status command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_GetStatus(SETUPPACK* SetupPack)
{    
	UINT8 nEndpoint;
	UINT8 pBuf[3];
	pBuf[0] = 0;
	pBuf[1] = 0;
	if((SetupPack->bmRequestType & 0x1f) == 0x02) //Endpoint
	{      
		if (SetupPack->wValue == 0x00)
		{
			nEndpoint = SetupPack->wIndex;
			if(nEndpoint == ENDPOINT0)
			    pBuf[0] = gSetupStatus.bEpHalt[0];
			if(nEndpoint == EP1OUT_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[1];
			if(nEndpoint == EP1IN_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[2];
			if(nEndpoint == EP2OUT_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[3];
			if(nEndpoint == EP2IN_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[4];
			if(nEndpoint == EP3OUT_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[5];
			if(nEndpoint == EP3IN_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[6];
			if(nEndpoint == EP4OUT_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[7];
			if(nEndpoint == EP4IN_NUMBER)
				pBuf[0] = gSetupStatus.bEpHalt[8];
		}
	}
	else if((SetupPack->bmRequestType & 0x1f) == 0x00)//Device
	{
		if (SetupPack->wValue == 0x00)
		{
			if(gSetupStatus.bRemoteWakeup == 1)
	  			pBuf[0] |= 0x02;
			else
			  	pBuf[0] &= ~0x02;
		}
	}
	else if((SetupPack->bmRequestType & 0x1f) == 0x01)//Interface
	{
	}
	else
	{
	}

	if(SetupPack->wLength >= 2)    
		USB_CtrlSend(pBuf, 2);   
	else
		USB_SendACK(ENDPOINT0); 
}

/***********************************************************************************
 * Subrountine:	Chap9_SetFeature
 * Function:	Set Feature command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_SetFeature(SETUPPACK* SetupPack)
{
	UINT8 nEndpoint;
  
    if((SetupPack->bmRequestType & 0x1f) == 0x02)// Endpoint
    {
    	if(SetupPack->wValue == 0x00)			//Halt Endpoint
    	{
    		// Enable the selected endpoint.
            nEndpoint = SetupPack->wIndex;
            if(nEndpoint==ENDPOINT0)
                gSetupStatus.bEpHalt[0] = 1;
            if(nEndpoint==EP1OUT_NUMBER)
                gSetupStatus.bEpHalt[1] = 1;
            if(nEndpoint==EP1IN_NUMBER)
                gSetupStatus.bEpHalt[2] = 1;
            if(nEndpoint==EP2OUT_NUMBER)
                gSetupStatus.bEpHalt[3] = 1;
            if(nEndpoint==EP2IN_NUMBER)
                gSetupStatus.bEpHalt[4] = 1;
            if(nEndpoint==EP3OUT_NUMBER)
            	gSetupStatus.bEpHalt[5] = 1;
            if(nEndpoint==EP3IN_NUMBER)
                gSetupStatus.bEpHalt[6] = 1;
            if(nEndpoint==EP4OUT_NUMBER)
                gSetupStatus.bEpHalt[7] = 1;
            if(nEndpoint==EP4IN_NUMBER)
                gSetupStatus.bEpHalt[8] = 1;
    	}
		else
		{
		}
    }
	else if((SetupPack->bmRequestType & 0x1f) == 0x00)//Device
	{
    	if(SetupPack->wValue == 0x01)			//DEVICE_REMOTE_WAKEUP
    	{
		   gSetupStatus.bRemoteWakeup = 1;
    	}
		else if(SetupPack->wValue == 0x02)			//TEST_MODE
		{
		}
		else
		{
		}
	}
	else if((SetupPack->bmRequestType & 0x1f) == 0x01)//Interface
	{
	}
	else
	{
	}

    USB_SendACK(ENDPOINT0); 
}

/***********************************************************************************
 * Subrountine:	Chap9_ClearFeature
 * Function:	Clear Feature command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_ClearFeature(SETUPPACK* SetupPack)
{
	UINT8 nEndpoint;
 
	if((SetupPack->bmRequestType & 0x1f) == 0x02)
	{      
		if (SetupPack->wValue == 0x00)			//Enable Endpoint
		{
			// Enable the selected endpoint.
            nEndpoint=SetupPack->wIndex;
		     if(nEndpoint==ENDPOINT0)
		     {
		     	gSetupStatus.bEpHalt[0]=0;
		     }            
         	else if(nEndpoint==EP1OUT_NUMBER)
         	{         		       	
                gSetupStatus.bEpHalt[1]=0;
	            UsbRegs.INDEX=ENDPOINT1;
                UsbRegs.OUTCSRL.bit.ClrDataTog=1;
                UsbRegs.OUTCSRL.bit.SendStall=0;
		        UsbRegs.OUTCSRL.bit.SentStall = 1;
            }        		        	
           else if(nEndpoint==EP1IN_NUMBER)
           	{      
                gSetupStatus.bEpHalt[2]=0;
	            UsbRegs.INDEX=ENDPOINT1;    
         	    UsbRegs.STAS.INCSRL.bit.SendStall=0;          
                UsbRegs.STAS.INCSRL.bit.ClrDataTog=1;
//				UsbRegs.OUTCSRL.bit.SentStall = 1;      
            }
         	else if(nEndpoint==EP2OUT_NUMBER)
         	{         		       	
                gSetupStatus.bEpHalt[3]=0;
	            UsbRegs.INDEX=ENDPOINT2;
                UsbRegs.OUTCSRL.bit.ClrDataTog=1;
                UsbRegs.OUTCSRL.bit.SendStall=0;
		        UsbRegs.OUTCSRL.bit.SentStall = 1;
            }         		        	
           else if(nEndpoint==EP2IN_NUMBER)
           	{      
                gSetupStatus.bEpHalt[4]=0;
	            UsbRegs.INDEX=ENDPOINT2;    
         	    UsbRegs.STAS.INCSRL.bit.SendStall=0;          
                UsbRegs.STAS.INCSRL.bit.ClrDataTog=1;
//				UsbRegs.OUTCSRL.bit.SentStall = 1;      
            }
         	else if(nEndpoint==EP3OUT_NUMBER)
         	{         		       	
                gSetupStatus.bEpHalt[5]=0;
	            UsbRegs.INDEX=ENDPOINT3;
                UsbRegs.OUTCSRL.bit.ClrDataTog=1;
                UsbRegs.OUTCSRL.bit.SendStall=0;
		        UsbRegs.OUTCSRL.bit.SentStall = 1;
            }        		        	
           else if(nEndpoint==EP3IN_NUMBER)
           	{      
                gSetupStatus.bEpHalt[6]=0;
	            UsbRegs.INDEX=ENDPOINT3;    
         	    UsbRegs.STAS.INCSRL.bit.SendStall=0;          
                UsbRegs.STAS.INCSRL.bit.ClrDataTog=1;
//				UsbRegs.OUTCSRL.bit.SentStall = 1;      
            }
         	else if(nEndpoint==EP4OUT_NUMBER)
         	{         		       	
                gSetupStatus.bEpHalt[7]=0;
	            UsbRegs.INDEX=ENDPOINT4;
                UsbRegs.OUTCSRL.bit.ClrDataTog=1;
                UsbRegs.OUTCSRL.bit.SendStall=0;
		        UsbRegs.OUTCSRL.bit.SentStall = 1;
            }         		        	
           else if(nEndpoint==EP4IN_NUMBER)
           	{      
                gSetupStatus.bEpHalt[8]=0;
	            UsbRegs.INDEX=ENDPOINT4;    
         	    UsbRegs.STAS.INCSRL.bit.SendStall=0;          
                UsbRegs.STAS.INCSRL.bit.ClrDataTog=1;
//				UsbRegs.OUTCSRL.bit.SentStall = 1;      
            }
         }
   }
	else if((SetupPack->bmRequestType & 0x1f) == 0x00)//Device
	{
    	if(SetupPack->wValue == 0x01)			//DEVICE_REMOTE_WAKEUP
    	{
		   gSetupStatus.bRemoteWakeup = 0;
    	}
		else if(SetupPack->wValue == 0x02)			//TEST_MODE
		{
		}
		else
		{
		}
	}
	else if((SetupPack->bmRequestType & 0x1f) == 0x01)//Interface
	{
	}
	else
	{
	}
   USB_SendACK(ENDPOINT0);  
}

/***********************************************************************************
 * Subrountine:	Chap9_GetConfig
 * Function:	Get Config command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_GetConfig(SETUPPACK* SetupPack)
{
	UINT8 pBuf[2];
	pBuf[0] = gSetupStatus.bConfigNum;
	if(SetupPack->wLength >= 1)
		USB_CtrlSend(pBuf, 1); 
	else
		USB_SendACK(ENDPOINT0); 
}

/***********************************************************************************
 * Subrountine: Chap9_GetInterface
 * Function:	Get Interface command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_GetInterface(SETUPPACK* SetupPack)
{
	UINT8 pBuf[2];
	pBuf[0] = gSetupStatus.bInterfaceNum;
	if(SetupPack->wLength >= 1)
		USB_CtrlSend(pBuf, 1);        
	else
		USB_SendACK(ENDPOINT0); 
}

/***********************************************************************************
 * Subrountine: Chap9_SetInterface
 * Function:	Set Interface command
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Chap9_SetInterface(SETUPPACK* SetupPack)
{
	gSetupStatus.bInterfaceNum = SetupPack->wValue;
	USB_SendACK(ENDPOINT0);  
}

/***********************************************************************************
 * Subrountine: Class_SendMaxLun
 * Function:	Class command SendMaxLun for SCSI device
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2009.07.09
 * ModifyRecord:
 **********************************************************************************/
void Class_SendMaxLun(SETUPPACK* SetupPack)      // 0xFE        
{
	UINT8 pBuf[2];
	pBuf[0] = 0; 
	
	USB_CtrlSend(pBuf, 1);     
}
 
/*--------------------------- End of ctrlitem.c ---------------------------*/ 

 
