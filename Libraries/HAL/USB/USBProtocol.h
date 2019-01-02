/***************************************************************************************
* File name    :	UsbProtocol.h
* Function     :	The header of UsbProtocol.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _USBPROTOCOL_H_
#define _USBPROTOCOL_H_

#define	USB_SUSPENDED_DIS		  	0	
#define	USB_SUSPENDED_8DIV_HFROSC  	1	
#define	USB_SUSPENDED_32K 			2

#define	USB_SUSPENDED	(0x8000)

/* Interrupt register bit masks */ 
#define M_INTR_SUSPEND 0x01 
#define M_INTR_RESUME 0x02 
#define M_INTR_RESET 0x04 
#define M_INTR_EP0 0x0001 
#define M_INTR_EP1 0x0002
#define M_INTR_EP2 0x0004
#define M_INTR_EP3 0x0008
#define M_INTR_EP4 0x0010
/*************************************************************************************
 * System Fuctions Declaration														 *
 ************************************************************************************/
typedef void (*DoMyUSBSleepProc)(void); 

void USB_DoWithPackage(void);
void USB_BulkPoll(void);
void InitUSBSleepCallBackFunc(UINT8 bUSBSuspendMode,DoMyUSBSleepProc pDoMyUSBSleep);
void USB_DoSuspend(void);

#endif
