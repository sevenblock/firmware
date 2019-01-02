/***************************************************************************************
* File name    :	UsbBase.h
* Function     :	The header of UsbBase.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _USBBASE_H_
#define _USBBASE_H_ 
 
/*************************************************************************************
 * Constant	Macro-Definition													 	 *
 ************************************************************************************/
 #define USB_TIMEOUT 1000000

//--------------USB interrupt mode para define----------
#define USB_INTTERRUPT_MODE 0//中断方式 
#define USB_POLLING_MODE    1//轮询方式 
#define USB_SETUP_INT_MODE  2//只是SETUP采用中断方式  

#define CTRL_PACKET_LEN 64

#define ENDPOINT0 0
#define ENDPOINT1 1
#define ENDPOINT2 2
#define ENDPOINT3 3
#define ENDPOINT4 4

#define IN_ENDPOINT 1
#define OUT_ENDPOINT 1
#define CTRL_ENDPOINT 0  
 
#define FS_MAX_PACK_LEN 64
 

//--- GPIO for USB pull up resistor enable ---
#define GPIO_USB_PU

extern UINT8 gUsbMode; 

#define UsbBulkReceive     USB_BulkReceive
#define UsbBulkSend        USB_BulkSend
#define UsbCtrlSend        USB_CtrlSend
#define UsbCtrlReceive     USB_CtrlReceive	 
#define UsbInit            USB_Init 
#define UsbWriteFIFO       USB_WriteFIFO
#define UsbReadFIFO        USB_ReadFIFO 
#define USBDoWithPackage   USB_DoWithPackage
#define USBBulkPoll		   USB_BulkPoll	  
#define	USB_BulkReceive(pBuf,nLen)  USB_EndPointReceive(ENDPOINT1,pBuf,nLen)
#define	USB_BulkSend(pBuf,nLen)		USB_EndPointSend (ENDPOINT1,pBuf,nLen) 
/*************************************************************************************
 * System Fuctions Declaration														 *
 ************************************************************************************/
void USB_EndPointReceive(UINT8 nEndPoint,UINT8* pBuf,UINT16 nLen);
void USB_EndPointSend (UINT8 nEndPoint,UINT8* pBuf,UINT32 nLen);
void USB_CtrlSend (UINT8* pBuf, UINT16 nLen);
UINT8 USB_CtrlReceive(UINT8* pBuf,UINT16 Len);
void USB_SendACK(UINT8 nEndPoint);
void USB_SendStall(UINT8 nEndPoint);
void USB_Init(UINT8 cMode);
void USB_Reset(void);
void USB_RemoteWakeWp(void);
void USB_SetupReceive(UINT8 *pBuf);
void USB_SetAddress(UINT8 nAddr);
void USB_WriteFIFO(UINT8 nEP,UINT32 nLen,UINT8 *pBuf);
void USB_ReadFIFO(UINT8 nEP,UINT32 nLen,UINT8 *pBuf);
void USB_SendZero(UINT8 nEndPoint); 
void USB_RecvZero(UINT8 nEndPoint);
void USB_SendStall_Out(UINT8 nEndPoint);	
void USB_SendStall_In(UINT8 nEndPoint);  
#endif
